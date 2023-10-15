// MIT License
//
// Copyright (c) 2021 Kai Zhu
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


// LINT_C_FILE


/*
VCVARSALL="C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat"
ARCH_ARG=x86_amd64

_dosvs10varsall=`cmd //c echo $VCVARSALL`
_dosbash=`cmd //c echo \`which bash\``
echo "@echo off" > localdevenvtmp.bat
echo "$_dosbash -c \"export -p\" > localdevenvtmp.export0" >> localdevenvtmp.bat
echo "call $_dosvs10varsall $ARCH_ARG" >> localdevenvtmp.bat
echo "$_dosbash -c \"export -p\" > localdevenvtmp.export1" >> localdevenvtmp.bat

cl -Isqlite-autoconf-3420000 sqlmath_wrapper_lightgbm.c -link -dll -out:a00.dll

gcc -Isqlite-autoconf-3420000 -g -fPIC -dynamiclib sqlmath_wrapper_lightgbm.c -o .a00.dylib
gcc -Isqlite-autoconf-3420000 -g -fPIC -shared sqlmath_wrapper_lightgbm.c -o .a00.so
gcc -Isqlite-autoconf-3420000 -g -shared sqlmath_wrapper_lightgbm.c -o .a00.dll
 */

#include <sqlite3ext.h>
#include <stdio.h>
SQLITE_EXTENSION_INIT1;
// *INDENT-OFF*
#ifdef _WIN32
__declspec(dllexport)
#endif
// *INDENT-ON*


//
#define SQLMATH_FUNC
#define SQLITE3_CREATE_FUNCTION1(func, argc) \
    errcode = sqlite3_create_function(db, #func, argc, \
        SQLITE_DETERMINISTIC | SQLITE_DIRECTONLY | SQLITE_UTF8, NULL, \
        sql1_##func##_func, NULL, NULL); \
    if (errcode != SQLITE_OK) { return errcode; }
#define UNUSED_PARAMETER(x) ((void)(x))
SQLMATH_FUNC extern void sql1_lgb_hello_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will print "hello lightgbm!".
    UNUSED_PARAMETER(argc);
    sqlite3_result_text(context, "hello lightgbm!", 0, SQLITE_STATIC);
}

// file sqlmath_lgb - init
int sqlite3_sqlmath_lgb_init(
    sqlite3 * db,
    char **pzErrMsg,
    const sqlite3_api_routines * pApi
) {
    int errcode = SQLITE_OK;
    SQLITE_EXTENSION_INIT2(pApi);
    SQLITE3_CREATE_FUNCTION1(lgb_hello, 0);
    return errcode;
}
