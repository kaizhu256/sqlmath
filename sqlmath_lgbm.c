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


// header
#undef SRC_SQLITE_BASE_C2
#include "sqlmath_base.c"
#include "sqlmath_lgbm.h"


/*
file sqlmath_custom - start
*/

SQLMATH_FUNC static void sql1_lgbm_datasetcreatefromfile_func(
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
        LGBM_DatasetFree);
}

int sqlite3_sqlmath_custom_init(
    sqlite3 * db,
    char **pzErrMsg,
    const sqlite3_api_routines * pApi
) {
    // coverage-hack
    noop();
    UNUSED_PARAMETER(db);
    UNUSED_PARAMETER(pApi);
    UNUSED_PARAMETER(pzErrMsg);
    return 0;
}

/*
file sqlmath_custom - end
*/
