#!/bin/sh

# sh one-liner
# sh jslint_ci.sh shCiBuildWasm
# sh jslint_ci.sh shSqlmathUpdate

SQLMATH_CFLAG_WALL_LIST=" \
    -Wall \
    -Werror \
    -Wextra \
"
SQLMATH_CFLAG_WNO_LIST=" \
    -Werror \
    -Wno-all \
    -Wno-extra \
"

shCiArtifactUploadCustom() {(set -e
# This function will run custom-code to upload build-artifacts.
    git fetch origin artifact
    git checkout origin/artifact "branch-$GITHUB_BRANCH0"
    mv "branch-$GITHUB_BRANCH0"/* .
    git add -f _sqlmath* sqlmath_wasm.*
    # screenshot html
    PID_LIST=""
    shBrowserScreenshot \
        "https://$GITHUB_GITHUB_IO/branch-$GITHUB_BRANCH0/index.html" &
    PID_LIST="$PID_LIST $!"
    shBrowserScreenshot .artifact/apidoc.html &
    PID_LIST="$PID_LIST $!"
    shPidListWait screenshot "$PID_LIST"
)}

shCiBaseCustom() {(set -e
# This function will run custom-code for base-ci.
    shCiEmsdkExport
    FILE="$(node --input-type=module -e '
process.stdout.write(
    process.platform === "darwin"
    ? "lib_lightgbm.dylib"
    : process.platform === "win32"
    ? "lib_lightgbm.dll"
    : "lib_lightgbm.so"
);
' "$@")" # '
    # bugfix - Library not loaded: /usr/local/opt/libomp/lib/libomp.dylib
    if [ ! -f "sqlmath/$FILE" ]
    then
        case "$(uname)" in
        Darwin*)
            brew install libomp
            cp -L /opt/homebrew/opt/libomp/lib/libomp.dylib sqlmath/
            ;;
        esac
        pip install lightgbm=="$(printf "v4.6.0" | sed "s|v||")"
        cp "$(
            find "$(
                pip show ruff | grep Location | sed "s|Location: ||"
            )/lightgbm" | grep "$FILE"
        )" "sqlmath/$FILE"
    fi
    # .github_cache - restore
    if [ "$GITHUB_ACTION" ] && [ -d .github_cache ]
    then
        cp -a .github_cache/* . || true # js-hack - */
    fi
    # cleanup
    rm -rf *.egg-info _sqlmath* build/ sqlmath/_sqlmath* && mkdir -p build/
    PID_LIST=""
    #
    # python -m build --sdist
    # python -m cibuildwheel
    (
    if (shCiMatrixIsmainNodeversion)
    then
        python setup.py sdist
        pip install cibuildwheel
        python -m cibuildwheel --output-dir=dist/
    fi
    ) &
    PID_LIST="$PID_LIST $!"
    #
    # shCiBuildWasm
    (
    if (shCiMatrixIsmainName)
    then
        # install graphicsmagick
        if (! command -v gm >/dev/null)
        then
            sudo apt-get update
            sudo apt-get install -y graphicsmagick
        fi
        # mkdir .artifact/
        mkdir -p .artifact/
        shImageLogoCreate
        shCiBuildWasm
        # .github_cache - save
        if [ "$GITHUB_ACTION" ] && [ ! -d .github_cache/_emsdk ]
        then
            mkdir -p .github_cache
            cp -a "$EMSDK" .github_cache
        fi
    fi
    ) &
    PID_LIST="$PID_LIST $!"
    #
    shPidListWait build_ext "$PID_LIST"
    #
    # run nodejs-ci
    shCiTestNodejs
    #
    # upload artifact
    if (shCiMatrixIsmainNodeversion) && ( \
        [ "$GITHUB_BRANCH0" = alpha ] \
        || [ "$GITHUB_BRANCH0" = beta ] \
        || [ "$GITHUB_BRANCH0" = master ] \
    )
    then
        export GITHUB_UPLOAD_RETRY=0
        while true
        do
            GITHUB_UPLOAD_RETRY="$((GITHUB_UPLOAD_RETRY + 1))"
            if [ "$GITHUB_UPLOAD_RETRY" -gt 4 ]
            then
                return 1
            fi
            if (node --input-type=module --eval '
import moduleChildProcess from "child_process";
(function () {
    moduleChildProcess.spawn(
        "sh",
        ["jslint_ci.sh", "shCiBaseCustomArtifactUpload"],
        {stdio: ["ignore", 1, 2]}
    ).on("exit", process.exit);
}());
' "$@") # '
            then
                break
            fi
        done
    fi
)}

shCiBaseCustomArtifactUpload() {(set -e
# This function will upload build-artifacts to branch-gh-pages.
    COMMIT_MESSAGE="- upload artifact
- retry$GITHUB_UPLOAD_RETRY
- $GITHUB_BRANCH0
- $(printf "$GITHUB_SHA" | cut -c-8)
- $(uname)
"
    printf "\n\n$COMMIT_MESSAGE\n"
    # init .git/config
    git config --local user.email "github-actions@users.noreply.github.com"
    git config --local user.name "github-actions"
    # git clone origin/artifact
    rm -rf .tmp/artifact
    shGitCmdWithGithubToken clone origin .tmp/artifact \
        --branch=artifact --single-branch
    (
    cd .tmp/artifact/
    cp ../../.git/config .git/config
    # update dir branch-$GITHUB_BRANCH0
    mkdir -p "branch-$GITHUB_BRANCH0"
    case "$(uname)" in
    Darwin*)
        rm -f "branch-$GITHUB_BRANCH0/"*darwin*
        rm -f "branch-$GITHUB_BRANCH0/"*macos*
        ;;
    Linux*)
        rm -f "branch-$GITHUB_BRANCH0/"*linux*
        # save sdist
        rm -f "branch-$GITHUB_BRANCH0/"*.tar.gz
        cp ../../dist/sqlmath-*.tar.gz "branch-$GITHUB_BRANCH0"
        ;;
    MINGW*)
        rm -f "branch-$GITHUB_BRANCH0/"*-win*
        rm -f "branch-$GITHUB_BRANCH0/"*_win*
        ;;
    esac
    cp ../../_sqlmath* "branch-$GITHUB_BRANCH0"
    cp ../../sqlmath/_sqlmath* "branch-$GITHUB_BRANCH0"
    for FILE in \
        ../../.artifact/asset_image_logo_256.png \
        ../../sqlmath/lib_lightgbm.dll \
        ../../sqlmath/lib_lightgbm.dylib \
        ../../sqlmath/lib_lightgbm.so \
        ../../sqlmath/libomp.dylib \
        ../../sqlmath_wasm.js \
        ../../sqlmath_wasm.wasm
    do
        if [ -f "$FILE" ]
        then
            cp "$FILE" "branch-$GITHUB_BRANCH0"
        fi
    done
    # save cibuildwheel
    cp ../../dist/sqlmath-*.whl "branch-$GITHUB_BRANCH0"
    # git commit
    git add .
    git add -f "branch-$GITHUB_BRANCH0"/_sqlmath*
    if (git commit -am "$COMMIT_MESSAGE")
    then
        # git push
        shGitCmdWithGithubToken push origin artifact
        # git squash
        if (shCiMatrixIsmainName) && [ "$GITHUB_BRANCH0" = alpha ]
        then
            shGitCommitPushOrSquash "" 50
        fi
    fi
    # debug
    shGitLsTree
    )
)}

shCiBuildWasm() {(set -e
# This function will build binaries in wasm.
    shCiEmsdkExport
    # install emsdk
    shCiEmsdkInstall
    # cd ${EMSDK} && . ./emsdk_env.sh && cd ..
    # build wasm
    printf "shCiBuildWasm\n" 1>&2
    OPTION1="$OPTION1 -flto"
    # debug
    # OPTION1="$OPTION1 -O0"
    OPTION1="$OPTION1 -Os"
    # OPTION2="$OPTION2 -Oz"
    # OPTION1="$OPTION1 -fsanitize=address"
    for FILE in \
        sqlmath_base.c \
        sqlmath_custom.c \
        sqlmath_external_sqlite.c \
        sqlmath_external_zlib.c
    do
        OPTION2=""
        FILE2="build/$(basename "$FILE").wasm.o"
        case "$FILE" in
        sqlmath_base.c)
            OPTION1="$OPTION1 $SQLMATH_CFLAG_WALL_LIST"
            ;;
        sqlmath_custom.c)
            OPTION1="$OPTION1 $SQLMATH_CFLAG_WALL_LIST"
            ;;
        *)
            OPTION1="$OPTION1 $SQLMATH_CFLAG_WNO_LIST"
            # optimization - skip rebuild of rollup if possible
            if [ "$FILE2" -nt "$FILE" ]
            then
                printf "shCiBuildWasm - skip $FILE\n" 1>&2
                continue
            fi
        esac
        case "$FILE" in
        sqlmath_base.c)
            OPTION2="$OPTION2 -DSRC_SQLMATH_BASE_C2="
            ;;
        sqlmath_custom.c)
            OPTION2="$OPTION2 -DSRC_SQLMATH_CUSTOM_C2="
            ;;
        sqlmath_external_sqlite.c)
            OPTION2="$OPTION2 -DSRC_SQLITE_BASE_C2="
            ;;
        sqlmath_external_zlib.c)
            OPTION2="$OPTION2 -DSRC_ZLIB_C2="
            ;;
        *)
            # optimization - skip rebuild of rollup if possible
            if [ "$FILE2" -nt "$FILE" ]
            then
                printf "shCiBuildWasm - skip $FILE\n" 1>&2
                continue
            fi
        esac
        OPTION2="$OPTION2 -c $FILE -o $FILE2"
        emcc $OPTION1 $OPTION2
    done
    OPTION2=""
    #
    OPTION2="$OPTION2 -s EXPORTED_FUNCTIONS=_sqlite3_initialize"
    OPTION2="$OPTION2,_dbCall"
    OPTION2="$OPTION2,_dbFileLoadOrSave"
    OPTION2="$OPTION2,_jsbatonGetErrmsg"
    OPTION2="$OPTION2,_jsbatonGetInt64"
    OPTION2="$OPTION2,_jsbatonGetString"
    OPTION2="$OPTION2,_sqlite3_errmsg"
    OPTION2="$OPTION2,_sqlite3_free"
    OPTION2="$OPTION2,_sqlite3_malloc"
    #
    OPTION2="$OPTION2 -s EXPORTED_RUNTIME_METHODS=cwrap"
    OPTION2="$OPTION2 -s LLD_REPORT_UNDEFINED"
    #
    case "$1" in
    --debug)
        OPTION2="$OPTION2 -s ASSERTIONS=1 -s SAFE_HEAP=1"
        ;;
    *)
        OPTION2="$OPTION2 --closure 1"
        ;;
    esac
    emcc $OPTION1 $OPTION2 \
        --memory-init-file 0 \
        --pre-js sqlmath_wrapper_wasm.js \
        -o build/sqlmath_wasm.js \
        -s ALLOW_MEMORY_GROWTH=1 \
        -s ALLOW_TABLE_GROWTH=1 \
        -s NODEJS_CATCH_EXIT=0 \
        -s NODEJS_CATCH_REJECTION=0 \
        -s RESERVED_FUNCTION_POINTERS=64 \
        -s SINGLE_FILE=0 \
        -s WASM=1 \
        -s WASM_BIGINT \
        build/sqlmath_base.c.wasm.o \
        build/sqlmath_custom.c.wasm.o \
        build/sqlmath_external_sqlite.c.wasm.o \
        build/sqlmath_external_zlib.c.wasm.o \
        #
    printf '' > sqlmath_wasm.js
    printf "/*jslint-disable*/
// Copyright (c) 2021 Kai Zhu
// SPDX-License-Identifier: MIT
// $(date -u +"%Y-%m-%dT%H:%M:%S%z")
(function () {
\"use strict\";
" >> sqlmath_wasm.js
    cat build/sqlmath_wasm.js | tr -d "\r" >> sqlmath_wasm.js
    printf '
}());
/*jslint-enable*/
' >> sqlmath_wasm.js
    cp build/sqlmath_wasm.wasm .
    ls -l sqlmath_wasm.*
)}

shCiEmsdkExport() {
# This function will export emsdk env.
    export EMSCRIPTEN_VERSION=3.1.3
    export EMSDK="$PWD/_emsdk"
    # https://github.com/sql-js/sql.js/blob/v1.6.2/.devcontainer/Dockerfile
    if [ ! "$PATH_EMSDK" ]
    then
        export PATH_EMSDK="$EMSDK:$EMSDK/upstream/emscripten/"
        export PATH="$PATH_EMSDK:$PATH"
    fi
}

shCiEmsdkInstall() {(set -e
# This function will install emsdk.
    shCiEmsdkExport
    if [ -d "$EMSDK" ]
    then
        exit
    fi
    # https://github.com/emscripten-core/emsdk/blob/2.0.34/docker/Dockerfile
    git clone https://github.com/emscripten-core/emsdk.git $EMSDK
    #
    echo "## Install Emscripten"
    cd ${EMSDK}
    ./emsdk install ${EMSCRIPTEN_VERSION}
    echo "## Done"
    #
    # This generates configuration that contains all valid paths according to
    # installed SDK
    # TODO(sbc): We should be able to use just emcc -v here but it doesn't
    # currently create the sanity file.
    cd ${EMSDK}
    echo "## Generate standard configuration"
    ./emsdk activate ${EMSCRIPTEN_VERSION}
    chmod 777 ${EMSDK}/upstream/emscripten
    chmod -R 777 ${EMSDK}/upstream/emscripten/cache
    echo "int main() { return 0; }" > hello.c
    ${EMSDK}/upstream/emscripten/emcc -c hello.c
    cat ${EMSDK}/upstream/emscripten/cache/sanity.txt
    echo "## Done"
    #
    # Cleanup Emscripten installation and strip some symbols
    echo "## Aggressive optimization: Remove debug symbols"
    cd ${EMSDK} && . ./emsdk_env.sh
    # Remove debugging symbols from embedded node (extra 7MB)
    strip -s `which node`
    # Tests consume ~80MB disc space
    rm -fr ${EMSDK}/upstream/emscripten/tests
    # Fastcomp is not supported
    rm -fr ${EMSDK}/upstream/fastcomp
    # strip out symbols from clang (~extra 50MB disc space)
    find ${EMSDK}/upstream/bin -type f -exec strip -s {} + || true
    echo "## Done"
    #
    # download ports
    # touch "$EMSDK/.null.c"
    # emcc \
    #     -s USE_ZLIB \
    #     "$EMSDK/.null.c" -o "$EMSDK/.null_wasm.js"
)}

shIndentC() {(set -e
# This function will indent/prettify c file.
    if (uname | grep -q "MING\|MSYS")
    then
        ./indent.exe \
            --blank-lines-after-commas \
            --braces-on-func-def-line \
            --break-function-decl-args \
            --break-function-decl-args-end \
            --dont-line-up-parentheses \
            --k-and-r-style \
            --line-length78 \
            --no-tabs \
            -bfde \
            $@
        dos2unix $@
    fi
)}

shCiLintCustom() {(set -e
# This function will run custom-code to lint files.
    if [ "$GITHUB_ACTION" ]
    then
        pip install pycodestyle ruff
    fi
    shLintPython \
        setup.py \
        sqlmath/__init__.py \
        test.py
)}

shCiPublishNpmCustom() {(set -e
# This function will run custom-code to npm-publish package.
    # fetch artifact
    git fetch origin artifact --depth=1
    git checkout origin/artifact \
        branch-beta/_sqlmath* \
        branch-beta/sqlmath_wasm*
    cp -a branch-beta/_sqlmath* .
    cp -a branch-beta/sqlmath_wasm.* .
    # npm-publish
    npm publish --access public
)}

shCiPublishPypiCustom() {(set -e
# This function will run custom-code to npm-publish package.
    # fetch artifact
    git fetch origin artifact --depth=1
    git checkout origin/artifact branch-alpha/
    mkdir dist/
    cp -a branch-alpha/sqlmath-*.tar.gz dist/
    cp -a branch-alpha/sqlmath-*.whl dist/
    ls -la dist/
)}

shCiTestNodejs() {(set -e
# This function will run test in nodejs.
    # init .tmp
    mkdir -p .tmp
    # rebuild c-module
    export npm_config_mode_test=1
    if [ "$npm_config_fast" != true ]
    then
        # lint c-file
        python cpplint.py \
            --filter=-whitespace/comments \
            sqlmath_base.c \
            sqlmath_custom.c
        # lint js-file
        node jslint.mjs .
        # create file MANIFEST.in
        # git ls-tree -r --name-only HEAD | sed "s|^|include |" > MANIFEST.in
        if [ -d .git/ ]
        then
            git ls-tree -r --name-only HEAD | sed "s|^|include |" > MANIFEST.in
        fi
        # create PKG-INFO
        python setup.py build_pkg_info
        # build nodejs c-addon
        PID_LIST=""
        (
        unset npm_config_mode_test
        npm_config_mode_setup=1 node --input-type=module -e '
import {ciBuildExt} from "./sqlmath.mjs";
ciBuildExt({process});
' "$@" # '
        ) &
        PID_LIST="$PID_LIST $!"
        # build python c-extension
        python setup.py build_ext &
        PID_LIST="$PID_LIST $!"
        shPidListWait build_ext "$PID_LIST"
    fi;
    PID_LIST=""
    # test nodejs
    (
    rm -f *~ .test*.sqlite __data/.test*.sqlite
    COVERAGE_EXCLUDE="$COVERAGE_EXCLUDE --exclude=jslint.mjs"
    if (node --eval '
require("assert")(require("./package.json").name !== "sqlmath");
' >/dev/null 2>&1)
    then
        COVERAGE_EXCLUDE="$COVERAGE_EXCLUDE --exclude=sqlmath.mjs"
    fi
    NODE_TEST_OPTION="$NODE_TEST_OPTION --trace-uncaught --trace-warnings"
    shRunWithCoverage $COVERAGE_EXCLUDE node $NODE_TEST_OPTION test.mjs
    ) &
    PID_LIST="$PID_LIST $!"
    # test python
    python setup.py test &
    PID_LIST="$PID_LIST $!"
    shPidListWait test "$PID_LIST"
)}

shSqlmathUpdate() {(set -e
# This function will update files with ~/Documents/sqlmath/.
    . "$HOME/myci2.sh" : && shMyciUpdate
    if [ "$PWD/" = "$HOME/Documents/sqlmath/" ]
    then
        # shRollupFetch
        if [ ! -d .sqlite-autoconf-3500200 ]
        then
            for URL in \
https://github.com/madler/zlib/releases/download/v1.3.1/zlib-1.3.1.tar.gz \
https://www.sqlite.org/2025/sqlite-autoconf-3500200.tar.gz
            do
                curl -L "$URL" | tar -xz
            done
            for DIR in \
                sqlite-autoconf-3500200 \
                zlib-1.3.1
            do
                rm -rf ".$DIR"
                mv "$DIR" ".$DIR"
            done
        fi
        shRollupFetch asset_sqlmath_external_rollup.js
        shRollupFetch index.html
        shRollupFetch sqlmath_base.h
        shRollupFetch sqlmath_external_sqlite.c
        shRollupFetch sqlmath_external_zlib.c
        # shIndentC
        if (uname | grep -q "MING\|MSYS")
        then
            shIndentC sqlmath_base.c
            shIndentC sqlmath_base.h
            shIndentC sqlmath_custom.c
            shIndentC sqlmath_external_sqlite.c
            shIndentC sqlmath_external_zlib.c
        fi
        return
    fi
    if [ -d "$HOME/Documents/sqlmath/" ]
    then
        for FILE in \
            .ci.sh \
            asset_sqlmath_external_rollup.js \
            indent.exe \
            index.html \
            setup.py \
            sqlmath.mjs \
            sqlmath/__init__.py \
            sqlmath_base.c \
            sqlmath_base.h \
            sqlmath_browser.mjs \
            sqlmath_external_sqlite.c \
            sqlmath_external_zlib.c \
            sqlmath_wrapper_wasm.js
        do
            ln -f "$HOME/Documents/sqlmath/$FILE" "$FILE"
        done
    fi
    if (command -v shSqlmathUpdate2 >/dev/null)
    then
        shSqlmathUpdate2
    fi
    git --no-pager diff
)}
