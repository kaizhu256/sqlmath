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

// cl.exe /LD sqlmath_lgbm.c -link -dll -out:__sqlmath_lgbm.dll
// cl.exe /D_USRDLL /D_WINDLL sqlmath_lgbm.c lib_lightgbm.dll /link /DLL /OUT:__sqlmath_lgbm.dll // NOLINT
// gcc -g -shared sqlmath_lgbm.c -L . -l_lightgbm -o __sqlmath_lgbm.dll

// LINT_C_FILE


// header
#define SRC_SQLITE3EXT_H2
#undef SRC_SQLITE_BASE_C2
#include "sqlmath_external_sqlite.c"


// *INDENT-OFF*
SQLITE_EXTENSION_INIT1
#include "./sqlmath_lgbm.h"
#ifdef _WIN32
__declspec(dllexport)
#endif
// *INDENT-ON*


#define UNUSED_PARAMETER(x) (void)(x)


#define SQLITE3_CREATE_FUNCTION1(func, argc) \
    errcode = sqlite3_create_function(db, #func, argc, \
        SQLITE_DETERMINISTIC | SQLITE_DIRECTONLY | SQLITE_UTF8, NULL, \
        sql1_##func##_func, NULL, NULL); \
    if (errcode != SQLITE_OK) { return errcode; }


/*
file sqlmath_lgbm - start
*/

void lgbm_datasetfree(
    void *handle
) {
    LGBM_DatasetFree((DatasetHandle) handle);
}

void sql1_lgbm_datasetcreatefromfile_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will return fmod(dividend, divisor).
    UNUSED_PARAMETER(argc);

    //!! LIGHTGBM_C_EXPORT int LGBM_DatasetCreateFromFile(
    //!! const char *filename,
    //!! const char *parameters,
    //!! const DatasetHandle reference,
    //!! DatasetHandle * out
    //!! );

    //!! sqlite3_result_blob(context, arr, nn * sizeof(double), xdel);
    sqlite3_result_blob(context, NULL, 0,
        // destructor
        lgbm_datasetfree);
}


/* TODO: Change the entry point name so that "extension" is replaced by
** text derived from the shared library filename as follows:  Copy every
** ASCII alphabetic character from the filename after the last "/" through
** the next following ".", converting each character to lowercase, and
** discarding the first three characters if they are "lib".
*/
int sqlite3___sqlmath_lgbm_init(
    sqlite3 * db,
    char **pzErrMsg,
    const sqlite3_api_routines * pApi
) {
    UNUSED_PARAMETER(pApi);
    UNUSED_PARAMETER(pzErrMsg);
    int errcode = 0;
    /* Insert here calls to
     **     sqlite3_create_function_v2(),
     **     sqlite3_create_collation_v2(),
     **     sqlite3_create_module_v2(), and/or
     **     sqlite3_vfs_register()
     ** to register the new features that your extension adds.
     */
    SQLITE3_CREATE_FUNCTION1(lgbm_datasetcreatefromfile, 1);
    return 0;
}

/*
file sqlmath_lgbm - end
*/
