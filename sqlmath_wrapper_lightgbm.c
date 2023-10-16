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

gcc -Isqlite-autoconf-3420000/ -g -fPIC -dynamiclib sqlmath_wrapper_lightgbm.c -o .a00.dylib
gcc -Isqlite-autoconf-3420000/ -g -fPIC -shared sqlmath_wrapper_lightgbm.c -o .a00.so
gcc -Isqlite-autoconf-3420000/ -shared sqlmath_wrapper_lightgbm.c -o build/_lgb.dll
 */

#include <sqlite3ext.h>
#include <string.h>
#include "./lgb_c_api.h"
SQLITE_EXTENSION_INIT1;

int LGBM_DatasetFree(
    DatasetHandle handle
);

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
    sqlite3_result_text(context, "hello lightgbm!", -1, SQLITE_STATIC);
    LGBM_DatasetFree(NULL);
}


int sqlite3_lgb_init(
    sqlite3 * db,
    char **errmsg_ptr,
    const sqlite3_api_routines * api
) {
    (void) errmsg_ptr;
    SQLITE_EXTENSION_INIT2(api);
    static const int flags =
        SQLITE_UTF8 | SQLITE_INNOCUOUS | SQLITE_DETERMINISTIC;
    int errcode = SQLITE_OK;
    SQLITE3_CREATE_FUNCTION1(lgb_hello, 0);
}
