"""
setup.py.

npm_config_mode_debug2=1 python setup.py build_ext && python setup.py test
python -m build
"""

import asyncio
import distutils.dist
import json
import os
import pathlib
import re
import subprocess
import sys
import sysconfig

import setuptools
import setuptools.command.build_ext
import setuptools.command.install_lib
import setuptools.logging

DEBUG = True
_setup_stop_after = None
_setup_distribution = None


class SetupError(Exception):
    """Setup error."""


def assert_or_throw(condition, message=None):
    """This function will throw <message> if <condition> is falsy."""
    if not condition:
        raise SetupError(message)


def backend_build_in_tmp(
    setup_command,
    result_extension=None,
    result_directory=None,
    config_settings=None,
):
    """This function will run setup.py in <tmp_dist_dir>."""
    assert_or_throw(config_settings is None or config_settings == {})
    result_directory = pathlib.Path(result_directory).resolve()
    # Build in a temporary directory, then copy to the target.
    pathlib.Path(result_directory).mkdir(exist_ok=True)
    import tempfile
    with tempfile.TemporaryDirectory(
        dir=result_directory.as_posix(),
        prefix=".tmp-",
    ) as tmp_dist_dir:
        sys.argv = [
            *sys.argv[:1],
            *setup_command,
            "--dist-dir",
            tmp_dist_dir,
        ]
        backend_exec_setup_py()
        for result_file in pathlib.Path(tmp_dist_dir).iterdir():
            if result_file.name.endswith(result_extension):
                result_file.replace(result_directory / result_file.name)
                return result_file.name
        return None


def backend_build_sdist(sdist_directory, config_settings=None):
    """`build_sdist`: build an sdist in the folder and return the basename."""
    return backend_build_in_tmp(
        ["sdist", "--formats", "gztar"],
        result_extension=".tar.gz",
        result_directory=sdist_directory,
        config_settings=config_settings,
    )


def backend_build_wheel(
    wheel_directory,
    config_settings=None,
    metadata_directory=None, # noqa: ARG001
):
    """`build_wheel`: build a wheel in the folder and return the basename."""
    return backend_build_in_tmp(
        ["bdist_wheel"],
        result_extension=".whl",
        result_directory=wheel_directory,
        config_settings=config_settings,
    )


def backend_exec_setup_py():
    """This function will exec setup.py with locals __file__, __name__."""
    # Note that we can reuse our build directory between calls
    # Correctness comes first, then optimization later
    __file__ = pathlib.Path("setup.py").resolve(strict=True).as_posix()
    __name__ = "__main__" # noqa: A001
    with pathlib.Path(__file__).open() as file1:
        exec(file1.read(), locals()) # noqa: S102


def backend_get_requires_for_build_wheel(config_settings=None):
    """`get_requires_for_build_wheel`: get the `setup_requires` to build."""
    assert_or_throw(config_settings is None or config_settings == {})
    sys.argv = [*sys.argv[:1], "egg_info"]
    backend_exec_setup_py()
    return ["wheel"]


def backend_get_requires_for_build_sdist(config_settings=None):
    """`get_requires_for_build_sdist`: get the `setup_requires` to build."""
    assert_or_throw(config_settings is None or config_settings == {})
    sys.argv = [*sys.argv[:1], "egg_info"]
    backend_exec_setup_py()
    return []


def build_ext():
    """This function will build c-extension."""
    build_ext_init()
    subprocess.run(["python", "setup.py", "build_ext_async"], check=True)


async def build_ext_async(): # noqa: C901
    """This function will build c-extension."""

    async def build_ext_obj(cdefine):
        file_obj = f"build/{cdefine}.obj"
        match cdefine:
            case "SQLMATH_BASE":
                pass
            case "SRC_SQLITE_PYTHON":
                pass
            case _:
                if pathlib.Path(file_obj).exists():
                    return
        file_src = f"build/{cdefine}.c"
        arg_list = [
            *[f"-I{path}" for path in path_include],
            #
            f"-D{cdefine}_C2=",
            "-DSRC_SQLITE_BASE_C2=",
            "-D_REENTRANT=1",
        ]
        if npm_config_mode_debug and is_win32:
            arg_list += ["/W3"]
        elif npm_config_mode_debug:
            arg_list += ["-Wextra"]
        elif is_win32:
            if cdefine == "SRC_SQLITE_PYTHON":
                arg_list += [
                    "/W1",
                    "/wd4047",
                    "/wd4244",
                    "/wd4996",
                ]
            else:
                arg_list += [
                    "/W3",
                    "/wd4047",
                    "/wd4244",
                    "/wd4996",
                ]
        elif cdefine in [
            "SQLMATH_BASE",
            "SQLMATH_CUSTOM",
        ]:
            arg_list += ["-Wextra"]
        else:
            arg_list += [
                "-Wno-all",
                "-Wno-extra",
                "-Wno-implicit-fallthrough",
                "-Wno-incompatible-pointer-types",
                "-Wno-int-conversion",
                "-Wno-unused-parameter",
            ]
# https://github.com/nodejs/node-gyp/blob/v9.3.1/gyp/pylib/gyp/MSVSSettings.py
        if is_win32:
            arg_list = [
                exe_cl,
                *arg_list,
                #
                "/GL", # to link.exe /LTCG
                "/MT", # multithreaded, statically-linked
                "/O2",
                #
                "/c", f"/Tc{file_src}",
                f"/Fo{file_obj}",
                "/nologo",
            ]
        else:
            arg_list = [
                # bugfix - fix multi-word cc_compiler="gcc -pthread"
                *cc_compiler.split(" "),
                *arg_list,
                #
                *cc_ccshared.strip().split(" "),
                *cc_cflags.strip().split(" "),
                #
                "-DHAVE_UNISTD_H=",
                "-c", file_src,
                "-o", file_obj,
            ]
        if cdefine == "SRC_SQLITE_PYTHON":
            arg_list = [arg for arg in arg_list if arg != "-DHAVE_UNISTD_H="]
        print(f"build_ext - compile {file_obj}")
        await create_subprocess_exec_and_check(
            *arg_list,
            env=env,
            stdout=subprocess.DEVNULL if npm_config_mode_debug else None,
        )

    async def create_subprocess_exec_and_check(*args, **kwds):
        child = await asyncio.create_subprocess_exec(
            *[arg for arg in args if arg],
            **kwds,
        )
        await child.communicate()
        if child.returncode != 0:
            msg = f"returncode={child.returncode}"
            raise subprocess.SubprocessError(msg)
    #
    # build_ext - update version
    with pathlib.Path("package.json").open() as file1:
        package_json = json.load(file1)
        version = package_json["version"].split("-")[0]
    for filename in [
        "README.md",
        "pyproject.toml",
        "sqlmath/__init__.py",
    ]:
        with pathlib.Path(filename).open("r+", newline="\n") as file1:
            data0 = file1.read()
            data1 = data0
            # update version - README.md
            data1 = re.sub(
                "(sqlmath(?:-|==))\\d\\d\\d\\d\\.\\d\\d?\\.\\d\\d?",
                f"\\g<1>{version}",
                data1,
            )
            # update version - pyproject.toml
            data1 = re.sub("\nversion = .*", f'\nversion = "{version}"', data1)
            # update version - sqlmath/__init__.py
            data1 = re.sub(
                "\n__version__ = .*",
                f'\n__version__ = "{version}"',
                data1,
            )
            data1 = re.sub(
                "\n__version_info__ = .*",
                (
                    "\n__version_info__ = "
                    + str(tuple(version.split("."))).replace("'", '"')
                ),
                data1,
            )
            if package_json["name"] == "sqlmath" and data1 != data0:
                print(f"build_ext - update file {file1.name}")
                file1.seek(0)
                file1.write(data1)
                file1.truncate()
    #
    # build_ext - init sysconfig
    cc_ccshared = sysconfig.get_config_var("CCSHARED") or ""
    cc_cflags = sysconfig.get_config_var("CFLAGS") or ""
    cc_compiler = sysconfig.get_config_var("CC") or ""
    if sys.platform == "linux" and cc_compiler.startswith("gcc"):
        cc_compiler += " -ldl"
    cc_ldflags = sysconfig.get_config_var("LDFLAGS") or ""
    cc_ldshared = sysconfig.get_config_var("LDSHARED") or ""
    dir_wheel = f"build/bdist.{sysconfig.get_platform()}/wheel/sqlmath"
    file_lib = f"_sqlmath{sysconfig.get_config_var('EXT_SUFFIX')}"
    is_win32 = sys.platform == "win32"
    path_include = [
        sysconfig.get_path("platinclude"),
        sysconfig.get_path("include"),
    ]
    path_prefix = sysconfig.get_config_var("prefix")
    path_prefix_base = sys.base_exec_prefix
    path_library = [
        f"{sysconfig.get_config_var('prefix')}{os.sep}libs",
        sysconfig.get_config_var("prefix"),
    ]
    platform_vcvarsall = {
        "win-amd64": "x86_amd64",
        "win-arm32": "x86_arm",
        "win-arm64": "x86_arm64",
        "win32": "x86",
    }.get(sysconfig.get_platform())
    npm_config_mode_debug = os.getenv("npm_config_mode_debug") # noqa: SIM112
    #
    # build_ext - init env
    env = os.environ
    if is_win32:
        env = await asyncio.create_subprocess_exec(
            (
                (
                    os.getenv("PROGRAMFILES(X86)")
                    or os.getenv("PROGRAMFILES")
                )
                + "\\Microsoft Visual Studio"
                + "\\Installer"
                + "\\vswhere.exe"
            ),
            "-latest", "-prerelease",
            "-requires", "Microsoft.VisualStudio.Component.VC.Tools.x86.x64",
            "-property", "installationPath",
            "-products", "*",
            stdout=asyncio.subprocess.PIPE,
        )
        env = (
            await env.stdout.readline()
        ).decode("mbcs").strip()
        env = await asyncio.create_subprocess_exec(
            "cmd.exe",
            "/u",
            "/c",
            f"{env}\\VC\\Auxiliary\\Build\\vcvarsall.bat",
            platform_vcvarsall,
            "&&",
            "set",
            stdout=asyncio.subprocess.PIPE,
        )
        env = (
            await env.stdout.read()
        ).decode("utf-16le")
        env = {
            key.lower(): val
            for key, _, val in
            (line.partition("=") for line in env.splitlines())
            if key and val
        }
        await_list = []
        for exe in ["cl.exe", "link.exe"]:
            await_list.append( # noqa: PERF401
                (
                    await asyncio.create_subprocess_exec(
                        *["where", exe],
                        env=env,
                        stdout=asyncio.subprocess.PIPE,
                    )
                ).stdout.readline(),
            )
        [exe_cl, exe_link] = [
            str(exe.splitlines()[0], "utf8")
            for exe in await asyncio.gather(*await_list)
        ]
    #
    # build_ext - virtualenv
    for arr in [path_include, path_library]:
        for path in arr:
            if path_prefix_base != path_prefix:
                path2 = path.replace(path_prefix, path_prefix_base)
                if path2 not in arr:
                    arr.append(path2)
    #
    # build_ext - compile .obj file
    await asyncio.gather(*[
        build_ext_obj(cdefine)
        for cdefine in [
            "SRC_ZLIB_BASE",
            "SRC_ZLIB_TEST_EXAMPLE",
            "SRC_ZLIB_TEST_MINIGZIP",
            #
            "SRC_SQLITE_BASE",
            "SRC_SQLITE_PYTHON",
            #
            "SQLMATH_BASE",
            "SQLMATH_CUSTOM",
        ]
    ])
    #
    # build_ext - link c-extension
# https://github.com/kaizhu256/sqlmath/actions/runs/4886979281/jobs/8723014944
    arg_list = []
    arg_list += [ # must be ordered first
        "build/SRC_ZLIB_BASE.obj",
        #
        "build/SRC_SQLITE_BASE.obj",
        "build/SRC_SQLITE_PYTHON.obj",
        #
        "build/SQLMATH_BASE.obj",
        "build/SQLMATH_CUSTOM.obj",
    ]
    if is_win32:
        arg_list = [
            exe_link,
            *[f"/LIBPATH:{path}" for path in path_library],
            *arg_list,
            #
            "/INCREMENTAL:NO", # optimization - reduce filesize
            "/LTCG", # from cl.exe /GL
            "/MANIFEST:EMBED",
            "/MANIFESTUAC:NO",
            #
            "/DLL",
            "/EXPORT:PyInit__sqlmath",
            #
            f"/OUT:build/{file_lib}",
            "/nologo",
        ]
    else:
        arg_list = [
            *cc_ldshared.strip().split(" "),
            *arg_list,
            #
            *cc_ldflags.strip().split(" "),
            #
            "-o", f"build/{file_lib}",
        ]
    await create_subprocess_exec_and_check(*arg_list, env=env)
    #
    # build_ext - copy c-extension to bdist
    await create_subprocess_exec_and_check(
        "sh",
        "-c",
        f"""
(set -e
    mkdir -p "{dir_wheel}/"
    cp "build/{file_lib}" "sqlmath/{file_lib}"
    cp "build/{file_lib}" "{dir_wheel}/{file_lib}"
    cp sqlmath/*.py "{dir_wheel}/"
)
        """,
    )


def build_ext_init():
    """This function will build c-extension."""
    if pathlib.Path("build/SRC_SQLITE_BASE.c").exists():
        return
    subprocess.run(["sh", "-c", """
(set -e
    mkdir -p build/
    for C_DEFINE in \\
        SRC_ZLIB_BASE \\
        SRC_ZLIB_TEST_EXAMPLE \\
        SRC_ZLIB_TEST_MINIGZIP \\
        \\
        SRC_SQLITE_BASE \\
        SRC_SQLITE_PYTHON \\
        SRC_SQLITE_SHELL
    do
        printf "
#define SRC_SQLITE_BASE_C2
#define ${C_DEFINE}_C2
#include \\"../sqlite_rollup.c\\"
    " > build/$C_DEFINE.c
    done
    #
    for C_DEFINE in \\
        SQLMATH_BASE \\
        SQLMATH_CUSTOM
    do
        printf "
#define SRC_SQLITE_BASE_C2
#define ${C_DEFINE}_C2
#include \\"../$(printf $C_DEFINE | tr \\"[:upper:]\\" \\"[:lower:]\\").c\\"
        " > build/$C_DEFINE.c
    done
)
    """], check=True)


def debuginline(*argv):
    """This function will print <argv> to stderr and then return <argv>[0]."""
    print("\n\ndebuginline")
    print(*argv)
    print("\n")
    return argv[0]


def setup(): # noqa: PLR0911
    """
    The gateway to the Distutils: do everything your setup script needs.

    to do, in a highly flexible and user-driven way.  Briefly: create a
    Distribution instance; find and parse config files; parse the command
    line; run each Distutils command found there, customized by the options
    supplied to 'setup()' (as keyword arguments), in config files, and on
    the command line.

    The Distribution instance might be an instance of a class supplied via
    the 'distclass' keyword argument to 'setup'; if no such class is
    supplied, then the Distribution class (in dist.py) is instantiated.
    All other arguments to 'setup' (except for 'cmdclass') are used to set
    attributes of the Distribution instance.

    The 'cmdclass' argument, if supplied, is a dictionary mapping command
    names to command classes.  Each command encountered on the command line
    will be turned into a command class, which is in turn instantiated; any
    class found in 'cmdclass' is used in place of the default, which is
    (for command 'foo_bar') class 'foo_bar' in module
    'distutils.command.foo_bar'.  The command class must provide a
    'user_options' attribute which is a list of option specifiers for
    'distutils.fancy_getopt'.  Any command-line options between the current
    and the next command are used to set attributes of the current command
    object.

    When the entire command-line has been successfully parsed, calls the
    'run()' method on each command object in turn.  This method will be
    driven entirely by the Distribution object (which each command object
    has a reference to, thanks to its constructor), and the
    command-specific options that became attributes of each command
    object.
    """
    ext_modules = None
    match sys.argv[1]:
        case "build_ext":
            build_ext()
            return None
        case "build_ext_async":
            asyncio.set_event_loop(asyncio.new_event_loop())
            asyncio.get_event_loop().run_until_complete(build_ext_async())
            return None
        case "build_ext_init":
            build_ext_init()
            return None
        case "bdist_wheel":
            build_ext()
            ext_modules = [setuptools.Extension("_sqlmath", [])]
        case "test":
            # ugly-hack - Disable test certain github-actions env.
            if (
                sys.version_info >= (3, 12)
                or (
                    os.getenv("GITHUB_ACTION")
                    and os.getenv("CIBUILDWHEEL")
                    and sys.platform == "win32"
                )
            ):
                pass
            else:
                import sqlmath
                sqlmath.test_python_run()
            return None
        case "egg_info":
            pass
        case "sdist":
            pass
        case _:
            raise SetupError(sys.argv[1])
    global _setup_stop_after, _setup_distribution # noqa: PLW0602, PLW0603
    setuptools.logging.configure()
    # Create the Distribution instance, using the remaining arguments
    # (ie. everything except distclass) to initialize it
    dist = distutils.dist.Distribution({
        "ext_modules": ext_modules,
        "script_args": sys.argv[1:],
        "script_name": pathlib.Path(sys.argv[0]).name,
    })
    _setup_distribution = dist
    if _setup_stop_after == "init":
        return dist
    # Find and parse the config file(s): they will override options from
    # the setup script, but be overridden by the command line.
    dist.parse_config_files()
    if DEBUG:
        print("options (after parsing config files):")
        dist.dump_option_dicts()
    if _setup_stop_after == "config":
        return dist
    # Parse the command line and override config files; any
    # command-line errors are the end user's fault, so turn them into
    # SystemExit to suppress tracebacks.
    ok = dist.parse_command_line()
    if DEBUG:
        print("options (after parsing command line):")
        dist.dump_option_dicts()
    if _setup_stop_after == "commandline":
        return dist
    # And finally, run all the commands found on the command line,
    for command in dist.commands:
        """Do whatever it takes to run a command (including nothing at all,
        if the command has already been run).  Specifically: if we have
        already created and run the command named by 'command', return
        silently without doing anything.  If the command named by 'command'
        doesn't even have a command object yet, create one.  Then invoke
        'run()' on that command object (or an existing one).
        """
        if not ok or dist.have_run.get(command):
            continue
        print(f"\nsetup.py - run - {command}\n")
        cmd_obj = dist.get_command_obj(command)
        cmd_obj.ensure_finalized()
        cmd_obj.run()
        dist.have_run[command] = 1
    return dist


# monkey-patch setuptools to accept c-extension compiled in nodejs
setuptools.command.build_ext.build_ext.run = lambda self: self
setuptools.command.install_lib.install_lib.install = lambda self: self

if __name__ == "__main__":
    setup()
else:
    build_sdist = backend_build_sdist
    build_wheel = backend_build_wheel
    get_requires_for_build_sdist = backend_get_requires_for_build_sdist
    get_requires_for_build_wheel = backend_get_requires_for_build_wheel
    __all__ = [
        "build_sdist",
        "build_wheel",
        "get_requires_for_build_sdist",
        "get_requires_for_build_wheel",
    ]
