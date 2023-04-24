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
file sqlmath_h - start
*/
// *INDENT-OFF*
#if defined(__cplusplus)
extern "C" {
#endif
// *INDENT-ON*


#ifndef SQLMATH_H
#define SQLMATH_H


#include <assert.h>
#include <float.h>
#include <math.h>
#include <sqlite3ext.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
** A macro to hint to the compiler that a function should not be
** inlined.
*/
#if defined(__GNUC__)
#  define SQLITE_NOINLINE  __attribute__((noinline))
#elif defined(_MSC_VER) && _MSC_VER >= 1310
#  define SQLITE_NOINLINE  __declspec(noinline)
#else
#  define SQLITE_NOINLINE
#endif


#define JSBATON_ARGC 16
#define JS_MAX_SAFE_INTEGER 0x1fffffffffffff
#define JS_MIN_SAFE_INTEGER -0x1fffffffffffff
#define MAX(aa, bb) (((aa) < (bb)) ? (bb) : (aa))
#define MIN(aa, bb) (((aa) > (bb)) ? (bb) : (aa))
#define NAPI_VERSION 8
#define SGN(aa) (((aa) < 0) ? -1 : ((aa) > 0) ? 1 : 0)
#define SIZEOF_MESSAGE_DEFAULT 256
#define SQLITE_DATATYPE_BLOB            0x04
#define SQLITE_DATATYPE_FLOAT           0x02
#define SQLITE_DATATYPE_INTEGER         0x01
#define SQLITE_DATATYPE_INTEGER_0       0x11
#define SQLITE_DATATYPE_INTEGER_1       0x21
#define SQLITE_DATATYPE_NULL            0x05
#define SQLITE_DATATYPE_OFFSET          768
#define SQLITE_DATATYPE_SHAREDARRAYBUFFER       0x71
#define SQLITE_DATATYPE_TEXT            0x03
#define SQLITE_DATATYPE_TEXT_0          0x13
#define SQLITE_ERROR_DATATYPE_INVALID   0x10003
#define SQLITE_ERROR_JSON_ARRAY_INVALID         0x71
#define SQLITE_ERROR_ZSQL_NULL          0x10004
#define SQLITE_MAX_LENGTH2 1000000000
#define SQLITE_RESPONSETYPE_LASTBLOB 1
#define SQLMATH_API
#define SQLMATH_FUNC
#define SWAP(aa, bb) tmp = (aa); (aa) = (bb); (bb) = tmp
#define UNUSED_PARAMETER(x) ((void)(x))


// this function will exec <sql> and if <errcode> is not ok,
// throw <baton>->errmsg with given sqlite-<errcode>
#define JSBATON_ASSERT_OK() \
    if (errcode != SQLITE_OK) { \
        if (baton != NULL && baton->errmsg[0] == '\x00') { \
            sqlmathSnprintfTrace(baton->errmsg, "sqlite - ", ( \
                errcode == SQLITE_ERROR_DATATYPE_INVALID ? "invalid datatype" \
                : errcode == SQLITE_ERROR_ZSQL_NULL \
                    ? "sqlite - cannot execute null sql-string" \
                : db == NULL || sqlite3_errcode(db) == SQLITE_OK \
                    ? sqlite3_errstr(errcode) \
                : sqlite3_errmsg(db)), __func__, __FILE__, __LINE__); \
        } \
        goto catch_error; \
    }

// this function will if <cond> is falsy, terminate process with <msg>
#define NAPI_ASSERT_FATAL(cond, msg) \
    if (!(cond)) { \
        napi_fatal_error(__func__, NAPI_AUTO_LENGTH , msg, NAPI_AUTO_LENGTH); \
    }

// this function will assert <errcode> == napi_ok in <env>
#define NAPI_ASSERT_OK() \
    if (0 != napiAssertOk(env, __func__, __FILE__, __LINE__, errcode)) { \
        return NULL; \
    }

#define NAPI_EXPORT_MEMBER(name) \
    {#name, NULL, name, NULL, NULL, NULL, napi_default, NULL}

#define NAPI_JSPROMISE_CREATE(func) \
    static void __##napi##_##func(napi_env env, void *data) { \
        UNUSED_PARAMETER(env); \
        func((Jsbaton *) data); \
    } \
    static napi_value _##func(napi_env env, napi_callback_info info) { \
        return jspromiseCreate(env, info, __##napi##_##func); \
    } \

#define SQLITE3_CREATE_FUNCTION1(func, argc) \
    errcode = sqlite3_create_function(db, #func, argc, \
        SQLITE_DETERMINISTIC | SQLITE_DIRECTONLY | SQLITE_UTF8, NULL, \
        sql_##func##_func, NULL, NULL); \
    if (errcode != SQLITE_OK) { return errcode; }

#define SQLITE3_CREATE_FUNCTION2(func, argc) \
    errcode = sqlite3_create_function(db, #func, argc, \
        SQLITE_DETERMINISTIC | SQLITE_DIRECTONLY | SQLITE_UTF8, NULL, \
        NULL, sql_##func##_step, sql_##func##_final); \
    if (errcode != SQLITE_OK) { return errcode; }

#define SQLITE3_CREATE_FUNCTION3(func, argc) \
    errcode = sqlite3_create_window_function(db, #func, argc, \
        SQLITE_DETERMINISTIC | SQLITE_DIRECTONLY | SQLITE_UTF8, NULL, \
        sql_##func##_step, sql_##func##_final, \
        sql_##func##_value, sql_##func##_inverse, NULL); \
    if (errcode != SQLITE_OK) { return errcode; }

#define SQLITE3_RESULT_ERROR_CODE(errcode) \
    if (errcode) { \
        sqlite3_result_error_code(context, errcode); \
        goto catch_error; \
    }

#define SQLITE3_RESULT_ERROR_MALLOC(pp) \
    if (pp == NULL) { \
        sqlite3_result_error_nomem(context); \
        goto catch_error; \
    }

#define STR99_ALLOCA(str99) \
    sqlite3_str __##str99 = { 0 }; \
    sqlite3_str *str99 = &__##str99; \
    str99->mxAlloc = SQLITE_MAX_LENGTH2; \

#define STR99_RESULT_ERROR(str99) \
    errcode = sqlite3_str_errcode(str99); \
    if (errcode == SQLITE_ERROR_JSON_ARRAY_INVALID) { \
        sqlite3_str_reset(str99); \
        sqlite3_result_error(context, \
            "str99ArrayAppendJsonarray() - invalid JSON array", \
            SQLITE_ERROR_JSON_ARRAY_INVALID); \
        goto catch_error; \
    } \
    if (errcode) { \
        sqlite3_str_reset(str99); \
        sqlite3_result_error_code(context, errcode); \
        goto catch_error; \
    } \
    if (sqlite3_str_length(str99) <= 0) { \
        sqlite3_str_reset(str99); \
        sqlite3_result_null(context); \
        goto catch_error; \
    }


// file sqlmath_h - sqlite3
// *INDENT-OFF*
SQLITE_API const sqlite3_api_routines *sqlite3ApiGet();
typedef uint32_t u32;
typedef uint64_t u64;
typedef uint8_t u8;


// file sqlmath_h - str99
/*
** An objected used to accumulate the text of a string where we
** do not necessarily know how big the string will be in the end.
*/
struct sqlite3_str {
  sqlite3 *db;         /* Optional database for lookaside.  Can be NULL */
  char *zText;         /* The string collected so far */
  u32  nAlloc;         /* Amount of space allocated in zText */
  u32  mxAlloc;        /* Maximum allowed allocation.  0 for no malloc usage */
  u32  nChar;          /* Length of the string so far */
  u8   accError;       /* SQLITE_NOMEM or SQLITE_TOOBIG */
  u8   printfFlags;    /* SQLITE_PRINTF flags below */
};
// *INDENT-ON*
SQLITE_API void str99JsonAppendText(
    sqlite3_str * str99,
    const char *zIn,
    u32 nn
);
SQLMATH_API void str99ArrayAppendDouble(
    sqlite3_str * str99,
    const double val
);
SQLMATH_API void str99ArrayAppendJsonarray(
    sqlite3_str * str99,
    const char *json,
    int nn
);
SQLMATH_API void str99JsonAppendFloat64array(
    sqlite3_str * str99,
    const double *arr,
    int nn
);
SQLMATH_API void str99JsonAppendJenks(
    sqlite3_str * str99,
    int kk,
    const double *arr,
    int nn
);
SQLMATH_API void str99JsonAppendText(
    sqlite3_str * str99,
    const char *zIn,
    u32 nn
);
SQLMATH_API void str99ResultBlob(
    sqlite3_str * str99,
    sqlite3_context * context
);
SQLMATH_API void str99ResultText(
    sqlite3_str * str99,
    sqlite3_context * context
);


// file sqlmath_h - vector99
typedef struct vector99 {
    double *buf;
    int alloc;
    int size;
    double arg0;
    double arg1;
} vector99;
SQLMATH_API void vector99_buf_free(
    vector99 * vec99
);
SQLMATH_API int vector99_buf_malloc(
    vector99 * vec99
);
SQLMATH_API const int vector99_nomem(
    const vector99 * vec99
);
SQLMATH_API double vector99_pop_front(
    vector99 * vec99
);
SQLMATH_API int vector99_push_back(
    vector99 * vec99,
    double val
);


// file sqlmath_h - Jsbaton
typedef struct Jsbaton {
    int32_t nallc;              // offset - 0-4
    int32_t nused;              // offset - 4-8
    int64_t argv[JSBATON_ARGC]; // offset - 8-136
    int64_t bufv[JSBATON_ARGC]; // offset - 136-264
    int64_t cfuncname;          // offset - 264-272
    char errmsg[SIZEOF_MESSAGE_DEFAULT];        // offset 272-528
    void *napi_argv;
    void *napi_work;
    void *napi_deferred;
} Jsbaton;


// file sqlmath_h - SQLMATH_API
SQLMATH_API int __dbFileImportOrExport(
    sqlite3 * pInMemory,
    char *zFilename,
    const int isSave
);

SQLMATH_API void dbClose(
    Jsbaton * baton
);

SQLMATH_API void dbExec(
    Jsbaton * baton
);

SQLMATH_API void dbFileImportOrExport(
    Jsbaton * baton
);

SQLMATH_API void dbNoop(
    Jsbaton * baton
);

SQLMATH_API void dbOpen(
    Jsbaton * baton
);

SQLMATH_API int doubleSign(
    const double aa
);

SQLMATH_API int doubleSortCompare(
    const void *aa,
    const void *bb
);

SQLMATH_API const char *jsbatonValueErrmsg(
    Jsbaton * baton
);

SQLMATH_API const char *jsbatonValueStringArgi(
    Jsbaton * baton,
    int argi
);

SQLMATH_API int noop(
);

SQLMATH_API double quantile(
    double *arr,
    const int nn,
    const double pp
);

SQLMATH_API const char *sqlmathSnprintfTrace(
    char *buf,
    const char *prefix,
    const char *errmsg,
    const char *func,
    const char *file,
    int line
);


// *INDENT-OFF*
#if defined(__cplusplus)
} // extern "C"
#endif
// *INDENT-ON*
/*
file sqlmath_h - end
*/
#endif                          // SQLMATH_H


/*
file sqlmath_ext - start
*/
#ifdef SQLITE3_EXT_C2
static const sqlite3_api_routines *sqlite3_api = NULL;


/*jslint-disable*/
/*
shRawLibFetch
{
    "fetchList": [
        {
            "comment": true,
            "url": "https://github.com/sqlite/sqlite/blob/version-3.39.4/LICENSE.md"
        },
        {
            "url": "https://github.com/sqlite/sqlite/blob/version-3.39.4/ext/misc/compress.c"
        },
        {
            "url": "https://github.com/sqlite/sqlite/blob/version-3.39.4/ext/misc/noop.c"
        },
        {
            "url": "https://github.com/sqlite/sqlite/blob/version-3.39.4/ext/misc/regexp.c"
        },
        {
            "url": "https://github.com/sqlite/sqlite/blob/version-3.39.4/ext/misc/series.c"
        },
        {
            "url": "https://github.com/sqlite/sqlite/blob/version-3.39.4/ext/misc/shathree.c"
        }
    ],
    "replaceList": [
        {
            "aa": "(\\bcase .*?:|\\bdefault:) *?{$",
            "bb": "$1 /\\\\* *\\\\/ {",
            "flags": "gm",
            "substr": ""
        },
        {
            "aa": "\\b(?:unsigned long|unsigned short)\\b.*",
            "bb": "$& //NOLINT",
            "flags": "g",
            "substr": ""
        },
        {
            "aa": "\\n(?:typedef )?struct .*?\\{\\n[\\S\\s]+?\\n\\};\\n",
            "bb": "\n// *INDENT-OFF*$&// *INDENT-ON*\n",
            "flags": "g",
            "substr": ""
        },
        {
            "aa": "^(?:#include .*?|SQLITE_EXTENSION_INIT1)$",
            "bb": "//$&",
            "flags": "gm",
            "substr": ""
        }
    ]
}
-  int nArg = 0;          /\\* Number of arguments that seriesFilter() expects *\\/
+  int nArg = 0;
+// hack-sqlite - fix warning
+/\\* Number of arguments that seriesFilter() expects *\\/

-  pIn = sqlite3_value_blob(argv[0]);
-  nIn = sqlite3_value_bytes(argv[0]);
+  pIn = sqlite3_value_blob(argv[0]);
+// hack-sqlite - handle null-case compress
+  if (pIn == NULL) { sqlite3_result_error(context, "Cannot compress() NULL blob", -1); return; }
+  nIn = sqlite3_value_bytes(argv[0]);

-  pIn = sqlite3_value_blob(argv[0]);
-  nIn = sqlite3_value_bytes(argv[0]);
+  pIn = sqlite3_value_blob(argv[0]);
+// hack-sqlite - handle null-case uncompress
+  if (pIn == NULL) { sqlite3_result_error(context, "Cannot uncompress() NULL blob", -1); return; }
+  nIn = sqlite3_value_bytes(argv[0]);

-  unsigned int nIn;
+// hack-sqlite - fix warning
+  int nIn;
-  unsigned int nIn;
+// hack-sqlite - fix warning
+  int nIn;

-//#include <zlib.h>
+// hack-sqlite - inline zlib.h
+// #include <zlib.h>
+#ifdef EMSCRIPTEN
+#define Z_OK 0
+int compress(
+    unsigned char *,
+    uint32_t *,
+    const unsigned char *,
+    uint32_t
+);
+int uncompress(
+    unsigned char *,
+    uint32_t *,
+    const unsigned char *,
+    uint32_t
+);
+#else // EMSCRIPTEN
+#include <zlib.h>
+#endif // EMSCRIPTEN
*/


/*
repo https://github.com/sqlite/sqlite/tree/version-3.39.4
committed 2022-09-29T15:55:41Z
*/


/*
file https://github.com/sqlite/sqlite/blob/version-3.39.4/LICENSE.md
*/
/*
The author disclaims copyright to this source code.  In place of
a legal notice, here is a blessing:

  *   May you do good and not evil.
  *   May you find forgiveness for yourself and forgive others.
  *   May you share freely, never taking more than you give.
*/


/*
file https://github.com/sqlite/sqlite/blob/version-3.39.4/ext/misc/compress.c
*/
/*
** 2014-06-13
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
******************************************************************************
**
** This SQLite extension implements SQL compression functions
** compress() and uncompress() using ZLIB.
*/
//#include "sqlite3ext.h"
//SQLITE_EXTENSION_INIT1
// hack-sqlite - inline zlib.h
// #include <zlib.h>
#ifdef EMSCRIPTEN
#define Z_OK 0
int compress(
    unsigned char *,
    uint32_t *,
    const unsigned char *,
    uint32_t
);
int uncompress(
    unsigned char *,
    uint32_t *,
    const unsigned char *,
    uint32_t
);
#else                           // EMSCRIPTEN
#include <zlib.h>
#endif                          // EMSCRIPTEN

/*
** Implementation of the "compress(X)" SQL function.  The input X is
** compressed using zLib and the output is returned.
**
** The output is a BLOB that begins with a variable-length integer that
** is the input size in bytes (the size of X before compression).  The
** variable-length integer is implemented as 1 to 5 bytes.  There are
** seven bits per integer stored in the lower seven bits of each byte.
** More significant bits occur first.  The most significant bit (0x80)
** is a flag to indicate the end of the integer.
**
** This function, SQLAR, and ZIP all use the same "deflate" compression
** algorithm, but each is subtly different:
**
**   *  ZIP uses raw deflate.
**
**   *  SQLAR uses the "zlib format" which is raw deflate with a two-byte
**      algorithm-identification header and a four-byte checksum at the end.
**
**   *  This utility uses the "zlib format" like SQLAR, but adds the variable-
**      length integer uncompressed size value at the beginning.
**
** This function might be extended in the future to support compression
** formats other than deflate, by providing a different algorithm-id
** mark following the variable-length integer size parameter.
*/
static void compressFunc(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
    const unsigned char *pIn;
    unsigned char *pOut;
// hack-sqlite - fix warning
    int nIn;
    unsigned long int nOut;     //NOLINT
    unsigned char x[8];
    int rc;
    int i,
     j;

    pIn = sqlite3_value_blob(argv[0]);
// hack-sqlite - handle null-case compress
    if (pIn == NULL) {
        sqlite3_result_error(context, "Cannot compress() NULL blob", -1);
        return;
    }
    nIn = sqlite3_value_bytes(argv[0]);
    nOut = 13 + nIn + (nIn + 999) / 1000;
    pOut = sqlite3_malloc(nOut + 5);
    for (i = 4; i >= 0; i--) {
        x[i] = (nIn >> (7 * (4 - i))) & 0x7f;
    }
    for (i = 0; i < 4 && x[i] == 0; i++) {
    }
    for (j = 0; i <= 4; i++, j++)
        pOut[j] = x[i];
    pOut[j - 1] |= 0x80;
    rc = compress(&pOut[j], &nOut, pIn, nIn);
    if (rc == Z_OK) {
        sqlite3_result_blob(context, pOut, nOut + j, sqlite3_free);
    } else {
        sqlite3_free(pOut);
    }
}

/*
** Implementation of the "uncompress(X)" SQL function.  The argument X
** is a blob which was obtained from compress(Y).  The output will be
** the value Y.
*/
static void uncompressFunc(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
    const unsigned char *pIn;
    unsigned char *pOut;
// hack-sqlite - fix warning
    int nIn;
    unsigned long int nOut;     //NOLINT
    int rc;
    int i;

    pIn = sqlite3_value_blob(argv[0]);
// hack-sqlite - handle null-case uncompress
    if (pIn == NULL) {
        sqlite3_result_error(context, "Cannot uncompress() NULL blob", -1);
        return;
    }
    nIn = sqlite3_value_bytes(argv[0]);
    nOut = 0;
    for (i = 0; i < nIn && i < 5; i++) {
        nOut = (nOut << 7) | (pIn[i] & 0x7f);
        if ((pIn[i] & 0x80) != 0) {
            i++;
            break;
        }
    }
    pOut = sqlite3_malloc(nOut + 1);
    rc = uncompress(pOut, &nOut, &pIn[i], nIn - i);
    if (rc == Z_OK) {
        sqlite3_result_blob(context, pOut, nOut, sqlite3_free);
    } else {
        sqlite3_free(pOut);
    }
}


#ifdef _WIN32
__declspec(dllexport)
#endif
int sqlite3_compress_init(
    sqlite3 * db,
    char **pzErrMsg,
    const sqlite3_api_routines * pApi
) {
    int rc = SQLITE_OK;
    SQLITE_EXTENSION_INIT2(pApi);
    (void) pzErrMsg;            /* Unused parameter */
    rc = sqlite3_create_function(db, "compress", 1,
        SQLITE_UTF8 | SQLITE_INNOCUOUS, 0, compressFunc, 0, 0);
    if (rc == SQLITE_OK) {
        rc = sqlite3_create_function(db, "uncompress", 1,
            SQLITE_UTF8 | SQLITE_INNOCUOUS | SQLITE_DETERMINISTIC,
            0, uncompressFunc, 0, 0);
    }
    return rc;
}


/*
file https://github.com/sqlite/sqlite/blob/version-3.39.4/ext/misc/noop.c
*/
/*
** 2020-01-08
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
******************************************************************************
**
** This SQLite extension implements a noop() function used for testing.
**
** Variants:
**
**    noop(X)           The default.  Deterministic.
**    noop_i(X)         Deterministic and innocuous.
**    noop_do(X)        Deterministic and direct-only.
**    noop_nd(X)        Non-deterministic.
*/
//#include "sqlite3ext.h"
//SQLITE_EXTENSION_INIT1
//#include <assert.h>
//#include <string.h>

/*
** Implementation of the noop() function.
**
** The function returns its argument, unchanged.
*/
static void noopfunc(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
    assert(argc == 1);
    sqlite3_result_value(context, argv[0]);
}

#ifdef _WIN32
__declspec(dllexport)
#endif
int sqlite3_noop_init(
    sqlite3 * db,
    char **pzErrMsg,
    const sqlite3_api_routines * pApi
) {
    int rc = SQLITE_OK;
    SQLITE_EXTENSION_INIT2(pApi);
    (void) pzErrMsg;            /* Unused parameter */
    rc = sqlite3_create_function(db, "noop", 1,
        SQLITE_UTF8 | SQLITE_DETERMINISTIC, 0, noopfunc, 0, 0);
    if (rc)
        return rc;
    rc = sqlite3_create_function(db, "noop_i", 1,
        SQLITE_UTF8 | SQLITE_DETERMINISTIC | SQLITE_INNOCUOUS,
        0, noopfunc, 0, 0);
    if (rc)
        return rc;
    rc = sqlite3_create_function(db, "noop_do", 1,
        SQLITE_UTF8 | SQLITE_DETERMINISTIC | SQLITE_DIRECTONLY,
        0, noopfunc, 0, 0);
    if (rc)
        return rc;
    rc = sqlite3_create_function(db, "noop_nd", 1,
        SQLITE_UTF8, 0, noopfunc, 0, 0);
    return rc;
}


/*
file https://github.com/sqlite/sqlite/blob/version-3.39.4/ext/misc/regexp.c
*/
/*
** 2012-11-13
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
******************************************************************************
**
** The code in this file implements a compact but reasonably
** efficient regular-expression matcher for posix extended regular
** expressions against UTF8 text.
**
** This file is an SQLite extension.  It registers a single function
** named "regexp(A,B)" where A is the regular expression and B is the
** string to be matched.  By registering this function, SQLite will also
** then implement the "B regexp A" operator.  Note that with the function
** the regular expression comes first, but with the operator it comes
** second.
**
**  The following regular expression syntax is supported:
**
**     X*      zero or more occurrences of X
**     X+      one or more occurrences of X
**     X?      zero or one occurrences of X
**     X{p,q}  between p and q occurrences of X
**     (X)     match X
**     X|Y     X or Y
**     ^X      X occurring at the beginning of the string
**     X$      X occurring at the end of the string
**     .       Match any single character
**     \c      Character c where c is one of \{}()[]|*+?.
**     \c      C-language escapes for c in afnrtv.  ex: \t or \n
**     \uXXXX  Where XXXX is exactly 4 hex digits, unicode value XXXX
**     \xXX    Where XX is exactly 2 hex digits, unicode value XX
**     [abc]   Any single character from the set abc
**     [^abc]  Any single character not in the set abc
**     [a-z]   Any single character in the range a-z
**     [^a-z]  Any single character not in the range a-z
**     \b      Word boundary
**     \w      Word character.  [A-Za-z0-9_]
**     \W      Non-word character
**     \d      Digit
**     \D      Non-digit
**     \s      Whitespace character
**     \S      Non-whitespace character
**
** A nondeterministic finite automaton (NFA) is used for matching, so the
** performance is bounded by O(N*M) where N is the size of the regular
** expression and M is the size of the input string.  The matcher never
** exhibits exponential behavior.  Note that the X{p,q} operator expands
** to p copies of X following by q-p copies of X? and that the size of the
** regular expression in the O(N*M) performance bound is computed after
** this expansion.
*/
//#include <string.h>
//#include <stdlib.h>
//#include "sqlite3ext.h"
//SQLITE_EXTENSION_INIT1

/*
** The following #defines change the names of some functions implemented in
** this file to prevent name collisions with C-library functions of the
** same name.
*/
#define re_match   sqlite3re_match
#define re_compile sqlite3re_compile
#define re_free    sqlite3re_free

/* The end-of-input character */
#define RE_EOF            0     /* End of input */

/* The NFA is implemented as sequence of opcodes taken from the following
** set.  Each opcode has a single integer argument.
*/
#define RE_OP_MATCH       1     /* Match the one character in the argument */
#define RE_OP_ANY         2     /* Match any one character.  (Implements ".") */
#define RE_OP_ANYSTAR     3     /* Special optimized version of .* */
#define RE_OP_FORK        4     /* Continue to both next and opcode at iArg */
#define RE_OP_GOTO        5     /* Jump to opcode at iArg */
#define RE_OP_ACCEPT      6     /* Halt and indicate a successful match */
#define RE_OP_CC_INC      7     /* Beginning of a [...] character class */
#define RE_OP_CC_EXC      8     /* Beginning of a [^...] character class */
#define RE_OP_CC_VALUE    9     /* Single value in a character class */
#define RE_OP_CC_RANGE   10     /* Range of values in a character class */
#define RE_OP_WORD       11     /* Perl word character [A-Za-z0-9_] */
#define RE_OP_NOTWORD    12     /* Not a perl word character */
#define RE_OP_DIGIT      13     /* digit:  [0-9] */
#define RE_OP_NOTDIGIT   14     /* Not a digit */
#define RE_OP_SPACE      15     /* space:  [ \t\n\r\v\f] */
#define RE_OP_NOTSPACE   16     /* Not a digit */
#define RE_OP_BOUNDARY   17     /* Boundary between word and non-word */

/* Each opcode is a "state" in the NFA */
typedef unsigned short ReStateNumber;   //NOLINT

/* Because this is an NFA and not a DFA, multiple states can be active at
** once.  An instance of the following object records all active states in
** the NFA.  The implementation is optimized for the common case where the
** number of actives states is small.
*/
// *INDENT-OFF*
typedef struct ReStateSet {
  unsigned nState;            /* Number of current states */
  ReStateNumber *aState;      /* Current states */
} ReStateSet;

/* An input string read one character at a time.
*/
typedef struct ReInput ReInput;
struct ReInput {
  const unsigned char *z;  /* All text */
  int i;                   /* Next byte to read */
  int mx;                  /* EOF when i>=mx */
};
// *INDENT-ON*

/* A compiled NFA (or an NFA that is in the process of being compiled) is
** an instance of the following object.
*/
typedef struct ReCompiled ReCompiled;
// *INDENT-OFF*
struct ReCompiled {
  ReInput sIn;                /* Regular expression text */
  const char *zErr;           /* Error message to return */
  char *aOp;                  /* Operators for the virtual machine */
  int *aArg;                  /* Arguments to each operator */
  unsigned (*xNextChar)(ReInput*);  /* Next character function */
  unsigned char zInit[12];    /* Initial text to match */
  int nInit;                  /* Number of characters in zInit */
  unsigned nState;            /* Number of entries in aOp[] and aArg[] */
  unsigned nAlloc;            /* Slots allocated for aOp[] and aArg[] */
};
// *INDENT-ON*

/* Add a state to the given state set if it is not already there */
static void re_add_state(
    ReStateSet * pSet,
    int newState
) {
    unsigned i;
    for (i = 0; i < pSet->nState; i++)
        if (pSet->aState[i] == newState)
            return;
    pSet->aState[pSet->nState++] = (ReStateNumber) newState;
}

/* Extract the next unicode character from *pzIn and return it.  Advance
** *pzIn to the first byte past the end of the character returned.  To
** be clear:  this routine converts utf8 to unicode.  This routine is
** optimized for the common case where the next character is a single byte.
*/
static unsigned re_next_char(
    ReInput * p
) {
    unsigned c;
    if (p->i >= p->mx)
        return 0;
    c = p->z[p->i++];
    if (c >= 0x80) {
        if ((c & 0xe0) == 0xc0 && p->i < p->mx && (p->z[p->i] & 0xc0) == 0x80) {
            c = (c & 0x1f) << 6 | (p->z[p->i++] & 0x3f);
            if (c < 0x80)
                c = 0xfffd;
        } else if ((c & 0xf0) == 0xe0 && p->i + 1 < p->mx
            && (p->z[p->i] & 0xc0) == 0x80
            && (p->z[p->i + 1] & 0xc0) == 0x80) {
            c = (c & 0x0f) << 12 | ((p->z[p->i] & 0x3f) << 6) | (p->z[p->i +
                    1] & 0x3f);
            p->i += 2;
            if (c <= 0x7ff || (c >= 0xd800 && c <= 0xdfff))
                c = 0xfffd;
        } else if ((c & 0xf8) == 0xf0 && p->i + 3 < p->mx
            && (p->z[p->i] & 0xc0) == 0x80 && (p->z[p->i + 1] & 0xc0) == 0x80
            && (p->z[p->i + 2] & 0xc0) == 0x80) {
            c = (c & 0x07) << 18 | ((p->z[p->i] & 0x3f) << 12) | ((p->z[p->i +
                        1] & 0x3f) << 6)
                | (p->z[p->i + 2] & 0x3f);
            p->i += 3;
            if (c <= 0xffff || c > 0x10ffff)
                c = 0xfffd;
        } else {
            c = 0xfffd;
        }
    }
    return c;
}

static unsigned re_next_char_nocase(
    ReInput * p
) {
    unsigned c = re_next_char(p);
    if (c >= 'A' && c <= 'Z')
        c += 'a' - 'A';
    return c;
}

/* Return true if c is a perl "word" character:  [A-Za-z0-9_] */
static int re_word_char(
    int c
) {
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z')
        || (c >= 'A' && c <= 'Z') || c == '_';
}

/* Return true if c is a "digit" character:  [0-9] */
static int re_digit_char(
    int c
) {
    return (c >= '0' && c <= '9');
}

/* Return true if c is a perl "space" character:  [ \t\r\n\v\f] */
static int re_space_char(
    int c
) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v'
        || c == '\f';
}

/* Run a compiled regular expression on the zero-terminated input
** string zIn[].  Return true on a match and false if there is no match.
*/
static int re_match(
    ReCompiled * pRe,
    const unsigned char *zIn,
    int nIn
) {
    ReStateSet aStateSet[2],
    *pThis,
    *pNext;
    ReStateNumber aSpace[100];
    ReStateNumber *pToFree;
    unsigned int i = 0;
    unsigned int iSwap = 0;
    int c = RE_EOF + 1;
    int cPrev = 0;
    int rc = 0;
    ReInput in;

    in.z = zIn;
    in.i = 0;
    in.mx = nIn >= 0 ? nIn : (int) strlen((char const *) zIn);

    /* Look for the initial prefix match, if there is one. */
    if (pRe->nInit) {
        unsigned char x = pRe->zInit[0];
        while (in.i + pRe->nInit <= in.mx
            && (zIn[in.i] != x ||
                strncmp((const char *) zIn + in.i, (const char *) pRe->zInit,
                    pRe->nInit) != 0)
            ) {
            in.i++;
        }
        if (in.i + pRe->nInit > in.mx)
            return 0;
    }

    if (pRe->nState <= (sizeof(aSpace) / (sizeof(aSpace[0]) * 2))) {
        pToFree = 0;
        aStateSet[0].aState = aSpace;
    } else {
        pToFree = sqlite3_malloc64(sizeof(ReStateNumber) * 2 * pRe->nState);
        if (pToFree == 0)
            return -1;
        aStateSet[0].aState = pToFree;
    }
    aStateSet[1].aState = &aStateSet[0].aState[pRe->nState];
    pNext = &aStateSet[1];
    pNext->nState = 0;
    re_add_state(pNext, 0);
    while (c != RE_EOF && pNext->nState > 0) {
        cPrev = c;
        c = pRe->xNextChar(&in);
        pThis = pNext;
        pNext = &aStateSet[iSwap];
        iSwap = 1 - iSwap;
        pNext->nState = 0;
        for (i = 0; i < pThis->nState; i++) {
            int x = pThis->aState[i];
            switch (pRe->aOp[x]) {
            case RE_OP_MATCH:  /* */  {
                    if (pRe->aArg[x] == c)
                        re_add_state(pNext, x + 1);
                    break;
                }
            case RE_OP_ANY:    /* */  {
                    if (c != 0)
                        re_add_state(pNext, x + 1);
                    break;
                }
            case RE_OP_WORD:   /* */  {
                    if (re_word_char(c))
                        re_add_state(pNext, x + 1);
                    break;
                }
            case RE_OP_NOTWORD:        /* */  {
                    if (!re_word_char(c) && c != 0)
                        re_add_state(pNext, x + 1);
                    break;
                }
            case RE_OP_DIGIT:  /* */  {
                    if (re_digit_char(c))
                        re_add_state(pNext, x + 1);
                    break;
                }
            case RE_OP_NOTDIGIT:       /* */  {
                    if (!re_digit_char(c) && c != 0)
                        re_add_state(pNext, x + 1);
                    break;
                }
            case RE_OP_SPACE:  /* */  {
                    if (re_space_char(c))
                        re_add_state(pNext, x + 1);
                    break;
                }
            case RE_OP_NOTSPACE:       /* */  {
                    if (!re_space_char(c) && c != 0)
                        re_add_state(pNext, x + 1);
                    break;
                }
            case RE_OP_BOUNDARY:       /* */  {
                    if (re_word_char(c) != re_word_char(cPrev))
                        re_add_state(pThis, x + 1);
                    break;
                }
            case RE_OP_ANYSTAR:        /* */  {
                    re_add_state(pNext, x);
                    re_add_state(pThis, x + 1);
                    break;
                }
            case RE_OP_FORK:   /* */  {
                    re_add_state(pThis, x + pRe->aArg[x]);
                    re_add_state(pThis, x + 1);
                    break;
                }
            case RE_OP_GOTO:   /* */  {
                    re_add_state(pThis, x + pRe->aArg[x]);
                    break;
                }
            case RE_OP_ACCEPT: /* */  {
                    rc = 1;
                    goto re_match_end;
                }
            case RE_OP_CC_EXC: /* */  {
                    if (c == 0)
                        break;
                    /* fall-through */ goto re_op_cc_inc;
                }
            case RE_OP_CC_INC:
              re_op_cc_inc:    /* */  {
                    int j = 1;
                    int n = pRe->aArg[x];
                    int hit = 0;
                    for (j = 1; j > 0 && j < n; j++) {
                        if (pRe->aOp[x + j] == RE_OP_CC_VALUE) {
                            if (pRe->aArg[x + j] == c) {
                                hit = 1;
                                j = -1;
                            }
                        } else {
                            if (pRe->aArg[x + j] <= c
                                && pRe->aArg[x + j + 1] >= c) {
                                hit = 1;
                                j = -1;
                            } else {
                                j++;
                            }
                        }
                    }
                    if (pRe->aOp[x] == RE_OP_CC_EXC)
                        hit = !hit;
                    if (hit)
                        re_add_state(pNext, x + n);
                    break;
                }
            }
        }
    }
    for (i = 0; i < pNext->nState; i++) {
        if (pRe->aOp[pNext->aState[i]] == RE_OP_ACCEPT) {
            rc = 1;
            break;
        }
    }
  re_match_end:
    sqlite3_free(pToFree);
    return rc;
}

/* Resize the opcode and argument arrays for an RE under construction.
*/
static int re_resize(
    ReCompiled * p,
    int N
) {
    char *aOp;
    int *aArg;
    aOp = sqlite3_realloc64(p->aOp, N * sizeof(p->aOp[0]));
    if (aOp == 0)
        return 1;
    p->aOp = aOp;
    aArg = sqlite3_realloc64(p->aArg, N * sizeof(p->aArg[0]));
    if (aArg == 0)
        return 1;
    p->aArg = aArg;
    p->nAlloc = N;
    return 0;
}

/* Insert a new opcode and argument into an RE under construction.  The
** insertion point is just prior to existing opcode iBefore.
*/
static int re_insert(
    ReCompiled * p,
    int iBefore,
    int op,
    int arg
) {
    int i;
    if (p->nAlloc <= p->nState && re_resize(p, p->nAlloc * 2))
        return 0;
    for (i = p->nState; i > iBefore; i--) {
        p->aOp[i] = p->aOp[i - 1];
        p->aArg[i] = p->aArg[i - 1];
    }
    p->nState++;
    p->aOp[iBefore] = (char) op;
    p->aArg[iBefore] = arg;
    return iBefore;
}

/* Append a new opcode and argument to the end of the RE under construction.
*/
static int re_append(
    ReCompiled * p,
    int op,
    int arg
) {
    return re_insert(p, p->nState, op, arg);
}

/* Make a copy of N opcodes starting at iStart onto the end of the RE
** under construction.
*/
static void re_copy(
    ReCompiled * p,
    int iStart,
    int N
) {
    if (p->nState + N >= p->nAlloc && re_resize(p, p->nAlloc * 2 + N))
        return;
    memcpy(&p->aOp[p->nState], &p->aOp[iStart], N * sizeof(p->aOp[0]));
    memcpy(&p->aArg[p->nState], &p->aArg[iStart], N * sizeof(p->aArg[0]));
    p->nState += N;
}

/* Return true if c is a hexadecimal digit character:  [0-9a-fA-F]
** If c is a hex digit, also set *pV = (*pV)*16 + valueof(c).  If
** c is not a hex digit *pV is unchanged.
*/
static int re_hex(
    int c,
    int *pV
) {
    if (c >= '0' && c <= '9') {
        c -= '0';
    } else if (c >= 'a' && c <= 'f') {
        c -= 'a' - 10;
    } else if (c >= 'A' && c <= 'F') {
        c -= 'A' - 10;
    } else {
        return 0;
    }
    *pV = (*pV) * 16 + (c & 0xff);
    return 1;
}

/* A backslash character has been seen, read the next character and
** return its interpretation.
*/
static unsigned re_esc_char(
    ReCompiled * p
) {
    static const char zEsc[] = "afnrtv\\()*.+?[$^{|}]";
    static const char zTrans[] = "\a\f\n\r\t\v";
    int i,
     v = 0;
    char c;
    if (p->sIn.i >= p->sIn.mx)
        return 0;
    c = p->sIn.z[p->sIn.i];
    if (c == 'u' && p->sIn.i + 4 < p->sIn.mx) {
        const unsigned char *zIn = p->sIn.z + p->sIn.i;
        if (re_hex(zIn[1], &v)
            && re_hex(zIn[2], &v)
            && re_hex(zIn[3], &v)
            && re_hex(zIn[4], &v)
            ) {
            p->sIn.i += 5;
            return v;
        }
    }
    if (c == 'x' && p->sIn.i + 2 < p->sIn.mx) {
        const unsigned char *zIn = p->sIn.z + p->sIn.i;
        if (re_hex(zIn[1], &v)
            && re_hex(zIn[2], &v)
            ) {
            p->sIn.i += 3;
            return v;
        }
    }
    for (i = 0; zEsc[i] && zEsc[i] != c; i++) {
    }
    if (zEsc[i]) {
        if (i < 6)
            c = zTrans[i];
        p->sIn.i++;
    } else {
        p->zErr = "unknown \\ escape";
    }
    return c;
}

/* Forward declaration */
static const char *re_subcompile_string(
    ReCompiled *
);

/* Peek at the next byte of input */
static unsigned char rePeek(
    ReCompiled * p
) {
    return p->sIn.i < p->sIn.mx ? p->sIn.z[p->sIn.i] : 0;
}

/* Compile RE text into a sequence of opcodes.  Continue up to the
** first unmatched ")" character, then return.  If an error is found,
** return a pointer to the error message string.
*/
static const char *re_subcompile_re(
    ReCompiled * p
) {
    const char *zErr;
    int iStart,
     iEnd,
     iGoto;
    iStart = p->nState;
    zErr = re_subcompile_string(p);
    if (zErr)
        return zErr;
    while (rePeek(p) == '|') {
        iEnd = p->nState;
        re_insert(p, iStart, RE_OP_FORK, iEnd + 2 - iStart);
        iGoto = re_append(p, RE_OP_GOTO, 0);
        p->sIn.i++;
        zErr = re_subcompile_string(p);
        if (zErr)
            return zErr;
        p->aArg[iGoto] = p->nState - iGoto;
    }
    return 0;
}

/* Compile an element of regular expression text (anything that can be
** an operand to the "|" operator).  Return NULL on success or a pointer
** to the error message if there is a problem.
*/
static const char *re_subcompile_string(
    ReCompiled * p
) {
    int iPrev = -1;
    int iStart;
    unsigned c;
    const char *zErr;
    while ((c = p->xNextChar(&p->sIn)) != 0) {
        iStart = p->nState;
        switch (c) {
        case '|':
        case '$':
        case ')':              /* */  {
                p->sIn.i--;
                return 0;
            }
        case '(':              /* */  {
                zErr = re_subcompile_re(p);
                if (zErr)
                    return zErr;
                if (rePeek(p) != ')')
                    return "unmatched '('";
                p->sIn.i++;
                break;
            }
        case '.':              /* */  {
                if (rePeek(p) == '*') {
                    re_append(p, RE_OP_ANYSTAR, 0);
                    p->sIn.i++;
                } else {
                    re_append(p, RE_OP_ANY, 0);
                }
                break;
            }
        case '*':              /* */  {
                if (iPrev < 0)
                    return "'*' without operand";
                re_insert(p, iPrev, RE_OP_GOTO, p->nState - iPrev + 1);
                re_append(p, RE_OP_FORK, iPrev - p->nState + 1);
                break;
            }
        case '+':              /* */  {
                if (iPrev < 0)
                    return "'+' without operand";
                re_append(p, RE_OP_FORK, iPrev - p->nState);
                break;
            }
        case '?':              /* */  {
                if (iPrev < 0)
                    return "'?' without operand";
                re_insert(p, iPrev, RE_OP_FORK, p->nState - iPrev + 1);
                break;
            }
        case '{':              /* */  {
                int m = 0,
                    n = 0;
                int sz,
                 j;
                if (iPrev < 0)
                    return "'{m,n}' without operand";
                while ((c = rePeek(p)) >= '0' && c <= '9') {
                    m = m * 10 + c - '0';
                    p->sIn.i++;
                }
                n = m;
                if (c == ',') {
                    p->sIn.i++;
                    n = 0;
                    while ((c = rePeek(p)) >= '0' && c <= '9') {
                        n = n * 10 + c - '0';
                        p->sIn.i++;
                    }
                }
                if (c != '}')
                    return "unmatched '{'";
                if (n > 0 && n < m)
                    return "n less than m in '{m,n}'";
                p->sIn.i++;
                sz = p->nState - iPrev;
                if (m == 0) {
                    if (n == 0)
                        return "both m and n are zero in '{m,n}'";
                    re_insert(p, iPrev, RE_OP_FORK, sz + 1);
                    n--;
                } else {
                    for (j = 1; j < m; j++)
                        re_copy(p, iPrev, sz);
                }
                for (j = m; j < n; j++) {
                    re_append(p, RE_OP_FORK, sz + 1);
                    re_copy(p, iPrev, sz);
                }
                if (n == 0 && m > 0) {
                    re_append(p, RE_OP_FORK, -sz);
                }
                break;
            }
        case '[':              /* */  {
                int iFirst = p->nState;
                if (rePeek(p) == '^') {
                    re_append(p, RE_OP_CC_EXC, 0);
                    p->sIn.i++;
                } else {
                    re_append(p, RE_OP_CC_INC, 0);
                }
                while ((c = p->xNextChar(&p->sIn)) != 0) {
                    if (c == '[' && rePeek(p) == ':') {
                        return "POSIX character classes not supported";
                    }
                    if (c == '\\')
                        c = re_esc_char(p);
                    if (rePeek(p) == '-') {
                        re_append(p, RE_OP_CC_RANGE, c);
                        p->sIn.i++;
                        c = p->xNextChar(&p->sIn);
                        if (c == '\\')
                            c = re_esc_char(p);
                        re_append(p, RE_OP_CC_RANGE, c);
                    } else {
                        re_append(p, RE_OP_CC_VALUE, c);
                    }
                    if (rePeek(p) == ']') {
                        p->sIn.i++;
                        break;
                    }
                }
                if (c == 0)
                    return "unclosed '['";
                p->aArg[iFirst] = p->nState - iFirst;
                break;
            }
        case '\\':             /* */  {
                int specialOp = 0;
                switch (rePeek(p)) {
                case 'b':
                    specialOp = RE_OP_BOUNDARY;
                    break;
                case 'd':
                    specialOp = RE_OP_DIGIT;
                    break;
                case 'D':
                    specialOp = RE_OP_NOTDIGIT;
                    break;
                case 's':
                    specialOp = RE_OP_SPACE;
                    break;
                case 'S':
                    specialOp = RE_OP_NOTSPACE;
                    break;
                case 'w':
                    specialOp = RE_OP_WORD;
                    break;
                case 'W':
                    specialOp = RE_OP_NOTWORD;
                    break;
                }
                if (specialOp) {
                    p->sIn.i++;
                    re_append(p, specialOp, 0);
                } else {
                    c = re_esc_char(p);
                    re_append(p, RE_OP_MATCH, c);
                }
                break;
            }
        default:               /* */  {
                re_append(p, RE_OP_MATCH, c);
                break;
            }
        }
        iPrev = iStart;
    }
    return 0;
}

/* Free and reclaim all the memory used by a previously compiled
** regular expression.  Applications should invoke this routine once
** for every call to re_compile() to avoid memory leaks.
*/
static void re_free(
    ReCompiled * pRe
) {
    if (pRe) {
        sqlite3_free(pRe->aOp);
        sqlite3_free(pRe->aArg);
        sqlite3_free(pRe);
    }
}

/*
** Compile a textual regular expression in zIn[] into a compiled regular
** expression suitable for us by re_match() and return a pointer to the
** compiled regular expression in *ppRe.  Return NULL on success or an
** error message if something goes wrong.
*/
static const char *re_compile(
    ReCompiled ** ppRe,
    const char *zIn,
    int noCase
) {
    ReCompiled *pRe;
    const char *zErr;
    int i,
     j;

    *ppRe = 0;
    pRe = sqlite3_malloc(sizeof(*pRe));
    if (pRe == 0) {
        return "out of memory";
    }
    memset(pRe, 0, sizeof(*pRe));
    pRe->xNextChar = noCase ? re_next_char_nocase : re_next_char;
    if (re_resize(pRe, 30)) {
        re_free(pRe);
        return "out of memory";
    }
    if (zIn[0] == '^') {
        zIn++;
    } else {
        re_append(pRe, RE_OP_ANYSTAR, 0);
    }
    pRe->sIn.z = (unsigned char *) zIn;
    pRe->sIn.i = 0;
    pRe->sIn.mx = (int) strlen(zIn);
    zErr = re_subcompile_re(pRe);
    if (zErr) {
        re_free(pRe);
        return zErr;
    }
    if (rePeek(pRe) == '$' && pRe->sIn.i + 1 >= pRe->sIn.mx) {
        re_append(pRe, RE_OP_MATCH, RE_EOF);
        re_append(pRe, RE_OP_ACCEPT, 0);
        *ppRe = pRe;
    } else if (pRe->sIn.i >= pRe->sIn.mx) {
        re_append(pRe, RE_OP_ACCEPT, 0);
        *ppRe = pRe;
    } else {
        re_free(pRe);
        return "unrecognized character";
    }

    /* The following is a performance optimization.  If the regex begins with
     ** ".*" (if the input regex lacks an initial "^") and afterwards there are
     ** one or more matching characters, enter those matching characters into
     ** zInit[].  The re_match() routine can then search ahead in the input
     ** string looking for the initial match without having to run the whole
     ** regex engine over the string.  Do not worry able trying to match
     ** unicode characters beyond plane 0 - those are very rare and this is
     ** just an optimization. */
    if (pRe->aOp[0] == RE_OP_ANYSTAR && !noCase) {
        for (j = 0, i = 1;
            j < (int) sizeof(pRe->zInit) - 2 && pRe->aOp[i] == RE_OP_MATCH;
            i++) {
            unsigned x = pRe->aArg[i];
            if (x <= 127) {
                pRe->zInit[j++] = (unsigned char) x;
            } else if (x <= 0xfff) {
                pRe->zInit[j++] = (unsigned char) (0xc0 | (x >> 6));
                pRe->zInit[j++] = 0x80 | (x & 0x3f);
            } else if (x <= 0xffff) {
                pRe->zInit[j++] = (unsigned char) (0xe0 | (x >> 12));
                pRe->zInit[j++] = 0x80 | ((x >> 6) & 0x3f);
                pRe->zInit[j++] = 0x80 | (x & 0x3f);
            } else {
                break;
            }
        }
        if (j > 0 && pRe->zInit[j - 1] == 0)
            j--;
        pRe->nInit = j;
    }
    return pRe->zErr;
}

/*
** Implementation of the regexp() SQL function.  This function implements
** the build-in REGEXP operator.  The first argument to the function is the
** pattern and the second argument is the string.  So, the SQL statements:
**
**       A REGEXP B
**
** is implemented as regexp(B,A).
*/
static void re_sql_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
    ReCompiled *pRe;            /* Compiled regular expression */
    const char *zPattern;       /* The regular expression */
    const unsigned char *zStr;  /* String being searched */
    const char *zErr;           /* Compile error message */
    int setAux = 0;             /* True to invoke sqlite3_set_auxdata() */

    (void) argc;                /* Unused */
    pRe = sqlite3_get_auxdata(context, 0);
    if (pRe == 0) {
        zPattern = (const char *) sqlite3_value_text(argv[0]);
        if (zPattern == 0)
            return;
        zErr = re_compile(&pRe, zPattern, sqlite3_user_data(context) != 0);
        if (zErr) {
            re_free(pRe);
            sqlite3_result_error(context, zErr, -1);
            return;
        }
        if (pRe == 0) {
            sqlite3_result_error_nomem(context);
            return;
        }
        setAux = 1;
    }
    zStr = (const unsigned char *) sqlite3_value_text(argv[1]);
    if (zStr != 0) {
        sqlite3_result_int(context, re_match(pRe, zStr, -1));
    }
    if (setAux) {
        sqlite3_set_auxdata(context, 0, pRe, (void (*)(void *)) re_free);
    }
}

/*
** Invoke this routine to register the regexp() function with the
** SQLite database connection.
*/
#ifdef _WIN32
__declspec(dllexport)
#endif
int sqlite3_regexp_init(
    sqlite3 * db,
    char **pzErrMsg,
    const sqlite3_api_routines * pApi
) {
    int rc = SQLITE_OK;
    SQLITE_EXTENSION_INIT2(pApi);
    (void) pzErrMsg;            /* Unused */
    rc = sqlite3_create_function(db, "regexp", 2,
        SQLITE_UTF8 | SQLITE_INNOCUOUS | SQLITE_DETERMINISTIC,
        0, re_sql_func, 0, 0);
    if (rc == SQLITE_OK) {
        /* The regexpi(PATTERN,STRING) function is a case-insensitive version
         ** of regexp(PATTERN,STRING). */
        rc = sqlite3_create_function(db, "regexpi", 2,
            SQLITE_UTF8 | SQLITE_INNOCUOUS | SQLITE_DETERMINISTIC,
            (void *) db, re_sql_func, 0, 0);
    }
    return rc;
}


/*
file https://github.com/sqlite/sqlite/blob/version-3.39.4/ext/misc/series.c
*/
/*
** 2015-08-18
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
**
** This file demonstrates how to create a table-valued-function using
** a virtual table.  This demo implements the generate_series() function
** which gives similar results to the eponymous function in PostgreSQL.
** Examples:
**
**      SELECT * FROM generate_series(0,100,5);
**
** The query above returns integers from 0 through 100 counting by steps
** of 5.
**
**      SELECT * FROM generate_series(0,100);
**
** Integers from 0 through 100 with a step size of 1.
**
**      SELECT * FROM generate_series(20) LIMIT 10;
**
** Integers 20 through 29.
**
** HOW IT WORKS
**
** The generate_series "function" is really a virtual table with the
** following schema:
**
**     CREATE TABLE generate_series(
**       value,
**       start HIDDEN,
**       stop HIDDEN,
**       step HIDDEN
**     );
**
** Function arguments in queries against this virtual table are translated
** into equality constraints against successive hidden columns.  In other
** words, the following pairs of queries are equivalent to each other:
**
**    SELECT * FROM generate_series(0,100,5);
**    SELECT * FROM generate_series WHERE start=0 AND stop=100 AND step=5;
**
**    SELECT * FROM generate_series(0,100);
**    SELECT * FROM generate_series WHERE start=0 AND stop=100;
**
**    SELECT * FROM generate_series(20) LIMIT 10;
**    SELECT * FROM generate_series WHERE start=20 LIMIT 10;
**
** The generate_series virtual table implementation leaves the xCreate method
** set to NULL.  This means that it is not possible to do a CREATE VIRTUAL
** TABLE command with "generate_series" as the USING argument.  Instead, there
** is a single generate_series virtual table that is always available without
** having to be created first.
**
** The xBestIndex method looks for equality constraints against the hidden
** start, stop, and step columns, and if present, it uses those constraints
** to bound the sequence of generated values.  If the equality constraints
** are missing, it uses 0 for start, 4294967295 for stop, and 1 for step.
** xBestIndex returns a small cost when both start and stop are available,
** and a very large cost if either start or stop are unavailable.  This
** encourages the query planner to order joins such that the bounds of the
** series are well-defined.
*/
//#include "sqlite3ext.h"
//SQLITE_EXTENSION_INIT1
//#include <assert.h>
//#include <string.h>

#ifndef SQLITE_OMIT_VIRTUALTABLE


/* series_cursor is a subclass of sqlite3_vtab_cursor which will
** serve as the underlying representation of a cursor that scans
** over rows of the result
*/
typedef struct series_cursor series_cursor;
// *INDENT-OFF*
struct series_cursor {
  sqlite3_vtab_cursor base;  /* Base class - must be first */
  int isDesc;                /* True to count down rather than up */
  sqlite3_int64 iRowid;      /* The rowid */
  sqlite3_int64 iValue;      /* Current value ("value") */
  sqlite3_int64 mnValue;     /* Mimimum value ("start") */
  sqlite3_int64 mxValue;     /* Maximum value ("stop") */
  sqlite3_int64 iStep;       /* Increment ("step") */
};
// *INDENT-ON*

/*
** The seriesConnect() method is invoked to create a new
** series_vtab that describes the generate_series virtual table.
**
** Think of this routine as the constructor for series_vtab objects.
**
** All this routine needs to do is:
**
**    (1) Allocate the series_vtab object and initialize all fields.
**
**    (2) Tell SQLite (via the sqlite3_declare_vtab() interface) what the
**        result set of queries against generate_series will look like.
*/
static int seriesConnect(
    sqlite3 * db,
    void *pUnused,
    int argcUnused,
    const char *const *argvUnused,
    sqlite3_vtab ** ppVtab,
    char **pzErrUnused
) {
    sqlite3_vtab *pNew;
    int rc;

/* Column numbers */
#define SERIES_COLUMN_VALUE 0
#define SERIES_COLUMN_START 1
#define SERIES_COLUMN_STOP  2
#define SERIES_COLUMN_STEP  3

    (void) pUnused;
    (void) argcUnused;
    (void) argvUnused;
    (void) pzErrUnused;
    rc = sqlite3_declare_vtab(db,
        "CREATE TABLE x(value,start hidden,stop hidden,step hidden)");
    if (rc == SQLITE_OK) {
        pNew = *ppVtab = sqlite3_malloc(sizeof(*pNew));
        if (pNew == 0)
            return SQLITE_NOMEM;
        memset(pNew, 0, sizeof(*pNew));
        sqlite3_vtab_config(db, SQLITE_VTAB_INNOCUOUS);
    }
    return rc;
}

/*
** This method is the destructor for series_cursor objects.
*/
static int seriesDisconnect(
    sqlite3_vtab * pVtab
) {
    sqlite3_free(pVtab);
    return SQLITE_OK;
}

/*
** Constructor for a new series_cursor object.
*/
static int seriesOpen(
    sqlite3_vtab * pUnused,
    sqlite3_vtab_cursor ** ppCursor
) {
    series_cursor *pCur;
    (void) pUnused;
    pCur = sqlite3_malloc(sizeof(*pCur));
    if (pCur == 0)
        return SQLITE_NOMEM;
    memset(pCur, 0, sizeof(*pCur));
    *ppCursor = &pCur->base;
    return SQLITE_OK;
}

/*
** Destructor for a series_cursor.
*/
static int seriesClose(
    sqlite3_vtab_cursor * cur
) {
    sqlite3_free(cur);
    return SQLITE_OK;
}


/*
** Advance a series_cursor to its next row of output.
*/
static int seriesNext(
    sqlite3_vtab_cursor * cur
) {
    series_cursor *pCur = (series_cursor *) cur;
    if (pCur->isDesc) {
        pCur->iValue -= pCur->iStep;
    } else {
        pCur->iValue += pCur->iStep;
    }
    pCur->iRowid++;
    return SQLITE_OK;
}

/*
** Return values of columns for the row at which the series_cursor
** is currently pointing.
*/
static int seriesColumn(
    sqlite3_vtab_cursor * cur,  /* The cursor */
    sqlite3_context * ctx,      /* First argument to sqlite3_result_...() */
    int i                       /* Which column to return */
) {
    series_cursor *pCur = (series_cursor *) cur;
    sqlite3_int64 x = 0;
    switch (i) {
    case SERIES_COLUMN_START:
        x = pCur->mnValue;
        break;
    case SERIES_COLUMN_STOP:
        x = pCur->mxValue;
        break;
    case SERIES_COLUMN_STEP:
        x = pCur->iStep;
        break;
    default:
        x = pCur->iValue;
        break;
    }
    sqlite3_result_int64(ctx, x);
    return SQLITE_OK;
}

/*
** Return the rowid for the current row. In this implementation, the
** first row returned is assigned rowid value 1, and each subsequent
** row a value 1 more than that of the previous.
*/
static int seriesRowid(
    sqlite3_vtab_cursor * cur,
    sqlite_int64 * pRowid
) {
    series_cursor *pCur = (series_cursor *) cur;
    *pRowid = pCur->iRowid;
    return SQLITE_OK;
}

/*
** Return TRUE if the cursor has been moved off of the last
** row of output.
*/
static int seriesEof(
    sqlite3_vtab_cursor * cur
) {
    series_cursor *pCur = (series_cursor *) cur;
    if (pCur->isDesc) {
        return pCur->iValue < pCur->mnValue;
    } else {
        return pCur->iValue > pCur->mxValue;
    }
}

/* True to cause run-time checking of the start=, stop=, and/or step=
** parameters.  The only reason to do this is for testing the
** constraint checking logic for virtual tables in the SQLite core.
*/
#ifndef SQLITE_SERIES_CONSTRAINT_VERIFY
# define SQLITE_SERIES_CONSTRAINT_VERIFY 0
#endif

/*
** This method is called to "rewind" the series_cursor object back
** to the first row of output.  This method is always called at least
** once prior to any call to seriesColumn() or seriesRowid() or
** seriesEof().
**
** The query plan selected by seriesBestIndex is passed in the idxNum
** parameter.  (idxStr is not used in this implementation.)  idxNum
** is a bitmask showing which constraints are available:
**
**    1:    start=VALUE
**    2:    stop=VALUE
**    4:    step=VALUE
**
** Also, if bit 8 is set, that means that the series should be output
** in descending order rather than in ascending order.  If bit 16 is
** set, then output must appear in ascending order.
**
** This routine should initialize the cursor and position it so that it
** is pointing at the first row, or pointing off the end of the table
** (so that seriesEof() will return true) if the table is empty.
*/
static int seriesFilter(
    sqlite3_vtab_cursor * pVtabCursor,
    int idxNum,
    const char *idxStrUnused,
    int argc,
    sqlite3_value ** argv
) {
    series_cursor *pCur = (series_cursor *) pVtabCursor;
    int i = 0;
    (void) idxStrUnused;
    if (idxNum & 1) {
        pCur->mnValue = sqlite3_value_int64(argv[i++]);
    } else {
        pCur->mnValue = 0;
    }
    if (idxNum & 2) {
        pCur->mxValue = sqlite3_value_int64(argv[i++]);
    } else {
        pCur->mxValue = 0xffffffff;
    }
    if (idxNum & 4) {
        pCur->iStep = sqlite3_value_int64(argv[i++]);
        if (pCur->iStep == 0) {
            pCur->iStep = 1;
        } else if (pCur->iStep < 0) {
            pCur->iStep = -pCur->iStep;
            if ((idxNum & 16) == 0)
                idxNum |= 8;
        }
    } else {
        pCur->iStep = 1;
    }
    for (i = 0; i < argc; i++) {
        if (sqlite3_value_type(argv[i]) == SQLITE_NULL) {
            /* If any of the constraints have a NULL value, then return no rows.
             ** See ticket https://www.sqlite.org/src/info/fac496b61722daf2 */
            pCur->mnValue = 1;
            pCur->mxValue = 0;
            break;
        }
    }
    if (idxNum & 8) {
        pCur->isDesc = 1;
        pCur->iValue = pCur->mxValue;
        if (pCur->iStep > 0) {
            pCur->iValue -= (pCur->mxValue - pCur->mnValue) % pCur->iStep;
        }
    } else {
        pCur->isDesc = 0;
        pCur->iValue = pCur->mnValue;
    }
    pCur->iRowid = 1;
    return SQLITE_OK;
}

/*
** SQLite will invoke this method one or more times while planning a query
** that uses the generate_series virtual table.  This routine needs to create
** a query plan for each invocation and compute an estimated cost for that
** plan.
**
** In this implementation idxNum is used to represent the
** query plan.  idxStr is unused.
**
** The query plan is represented by bits in idxNum:
**
**  (1)  start = $value  -- constraint exists
**  (2)  stop = $value   -- constraint exists
**  (4)  step = $value   -- constraint exists
**  (8)  output in descending order
*/
static int seriesBestIndex(
    sqlite3_vtab * pVTab,
    sqlite3_index_info * pIdxInfo
) {
    int i,
     j;                         /* Loop over constraints */
    int idxNum = 0;             /* The query plan bitmask */
    int bStartSeen = 0;         /* EQ constraint seen on the START column */
    int unusableMask = 0;       /* Mask of unusable constraints */
    int nArg = 0;
// hack-sqlite - fix warning
/* Number of arguments that seriesFilter() expects */
    int aIdx[3];                /* Constraints on start, stop, and step */
    const struct sqlite3_index_constraint *pConstraint;

    /* This implementation assumes that the start, stop, and step columns
     ** are the last three columns in the virtual table. */
    assert(SERIES_COLUMN_STOP == SERIES_COLUMN_START + 1);
    assert(SERIES_COLUMN_STEP == SERIES_COLUMN_START + 2);

    aIdx[0] = aIdx[1] = aIdx[2] = -1;
    pConstraint = pIdxInfo->aConstraint;
    for (i = 0; i < pIdxInfo->nConstraint; i++, pConstraint++) {
        int iCol;               /* 0 for start, 1 for stop, 2 for step */
        int iMask;              /* bitmask for those column */
        if (pConstraint->iColumn < SERIES_COLUMN_START)
            continue;
        iCol = pConstraint->iColumn - SERIES_COLUMN_START;
        assert(iCol >= 0 && iCol <= 2);
        iMask = 1 << iCol;
        if (iCol == 0)
            bStartSeen = 1;
        if (pConstraint->usable == 0) {
            unusableMask |= iMask;
            continue;
        } else if (pConstraint->op == SQLITE_INDEX_CONSTRAINT_EQ) {
            idxNum |= iMask;
            aIdx[iCol] = i;
        }
    }
    for (i = 0; i < 3; i++) {
        if ((j = aIdx[i]) >= 0) {
            pIdxInfo->aConstraintUsage[j].argvIndex = ++nArg;
            pIdxInfo->aConstraintUsage[j].omit =
                !SQLITE_SERIES_CONSTRAINT_VERIFY;
        }
    }
    /* The current generate_column() implementation requires at least one
     ** argument (the START value).  Legacy versions assumed START=0 if the
     ** first argument was omitted.  Compile with -DZERO_ARGUMENT_GENERATE_SERIES
     ** to obtain the legacy behavior */
#ifndef ZERO_ARGUMENT_GENERATE_SERIES
    if (!bStartSeen) {
        sqlite3_free(pVTab->zErrMsg);
        pVTab->zErrMsg =
            sqlite3_mprintf
            ("first argument to \"generate_series()\" missing or unusable");
        return SQLITE_ERROR;
    }
#endif
    if ((unusableMask & ~idxNum) != 0) {
        /* The start, stop, and step columns are inputs.  Therefore if there
         ** are unusable constraints on any of start, stop, or step then
         ** this plan is unusable */
        return SQLITE_CONSTRAINT;
    }
    if ((idxNum & 3) == 3) {
        /* Both start= and stop= boundaries are available.  This is the
         ** the preferred case */
        pIdxInfo->estimatedCost = (double) (2 - ((idxNum & 4) != 0));
        pIdxInfo->estimatedRows = 1000;
        if (pIdxInfo->nOrderBy >= 1 && pIdxInfo->aOrderBy[0].iColumn == 0) {
            if (pIdxInfo->aOrderBy[0].desc) {
                idxNum |= 8;
            } else {
                idxNum |= 16;
            }
            pIdxInfo->orderByConsumed = 1;
        }
    } else {
        /* If either boundary is missing, we have to generate a huge span
         ** of numbers.  Make this case very expensive so that the query
         ** planner will work hard to avoid it. */
        pIdxInfo->estimatedRows = 2147483647;
    }
    pIdxInfo->idxNum = idxNum;
    return SQLITE_OK;
}

/*
** This following structure defines all the methods for the
** generate_series virtual table.
*/
static sqlite3_module seriesModule = {
    0,                          /* iVersion */
    0,                          /* xCreate */
    seriesConnect,              /* xConnect */
    seriesBestIndex,            /* xBestIndex */
    seriesDisconnect,           /* xDisconnect */
    0,                          /* xDestroy */
    seriesOpen,                 /* xOpen - open a cursor */
    seriesClose,                /* xClose - close a cursor */
    seriesFilter,               /* xFilter - configure scan constraints */
    seriesNext,                 /* xNext - advance a cursor */
    seriesEof,                  /* xEof - check for end of scan */
    seriesColumn,               /* xColumn - read data */
    seriesRowid,                /* xRowid - read data */
    0,                          /* xUpdate */
    0,                          /* xBegin */
    0,                          /* xSync */
    0,                          /* xCommit */
    0,                          /* xRollback */
    0,                          /* xFindMethod */
    0,                          /* xRename */
    0,                          /* xSavepoint */
    0,                          /* xRelease */
    0,                          /* xRollbackTo */
    0                           /* xShadowName */
};

#endif                          /* SQLITE_OMIT_VIRTUALTABLE */

#ifdef _WIN32
__declspec(dllexport)
#endif
int sqlite3_series_init(
    sqlite3 * db,
    char **pzErrMsg,
    const sqlite3_api_routines * pApi
) {
    int rc = SQLITE_OK;
    SQLITE_EXTENSION_INIT2(pApi);
#ifndef SQLITE_OMIT_VIRTUALTABLE
    if (sqlite3_libversion_number() < 3008012 && pzErrMsg != 0) {
        *pzErrMsg =
            sqlite3_mprintf
            ("generate_series() requires SQLite 3.8.12 or later");
        return SQLITE_ERROR;
    }
    rc = sqlite3_create_module(db, "generate_series", &seriesModule, 0);
#endif
    return rc;
}


/*
file https://github.com/sqlite/sqlite/blob/version-3.39.4/ext/misc/shathree.c
*/
/*
** 2017-03-08
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
******************************************************************************
**
** This SQLite extension implements functions that compute SHA3 hashes.
** Two SQL functions are implemented:
**
**     sha3(X,SIZE)
**     sha3_query(Y,SIZE)
**
** The sha3(X) function computes the SHA3 hash of the input X, or NULL if
** X is NULL.
**
** The sha3_query(Y) function evaluates all queries in the SQL statements of Y
** and returns a hash of their results.
**
** The SIZE argument is optional.  If omitted, the SHA3-256 hash algorithm
** is used.  If SIZE is included it must be one of the integers 224, 256,
** 384, or 512, to determine SHA3 hash variant that is computed.
*/
//#include "sqlite3ext.h"
//SQLITE_EXTENSION_INIT1
//#include <assert.h>
//#include <string.h>
//#include <stdarg.h>

#ifndef SQLITE_AMALGAMATION
typedef sqlite3_uint64 u64;
#endif                          /* SQLITE_AMALGAMATION */

/******************************************************************************
** The Hash Engine
*/
/*
** Macros to determine whether the machine is big or little endian,
** and whether or not that determination is run-time or compile-time.
**
** For best performance, an attempt is made to guess at the byte-order
** using C-preprocessor macros.  If that is unsuccessful, or if
** -DSHA3_BYTEORDER=0 is set, then byte-order is determined
** at run-time.
*/
#ifndef SHA3_BYTEORDER
# if defined(i386)     || defined(__i386__)   || defined(_M_IX86) ||    \
     defined(__x86_64) || defined(__x86_64__) || defined(_M_X64)  ||    \
     defined(_M_AMD64) || defined(_M_ARM)     || defined(__x86)   ||    \
     defined(__arm__)
#   define SHA3_BYTEORDER    1234
# elif defined(sparc)    || defined(__ppc__)
#   define SHA3_BYTEORDER    4321
# else
#   define SHA3_BYTEORDER 0
# endif
#endif


/*
** State structure for a SHA3 hash in progress
*/
typedef struct SHA3Context SHA3Context;
// *INDENT-OFF*
struct SHA3Context {
  union {
    u64 s[25];                /* Keccak state. 5x5 lines of 64 bits each */
    unsigned char x[1600];    /* ... or 1600 bytes */
  } u;
  unsigned nRate;        /* Bytes of input accepted per Keccak iteration */
  unsigned nLoaded;      /* Input bytes loaded into u.x[] so far this cycle */
  unsigned ixMask;       /* Insert next input into u.x[nLoaded^ixMask]. */
};
// *INDENT-ON*

/*
** A single step of the Keccak mixing function for a 1600-bit state
*/
static void KeccakF1600Step(
    SHA3Context * p
) {
    int i;
    u64 b0,
     b1,
     b2,
     b3,
     b4;
    u64 c0,
     c1,
     c2,
     c3,
     c4;
    u64 d0,
     d1,
     d2,
     d3,
     d4;
    static const u64 RC[] = {
        0x0000000000000001ULL, 0x0000000000008082ULL,
        0x800000000000808aULL, 0x8000000080008000ULL,
        0x000000000000808bULL, 0x0000000080000001ULL,
        0x8000000080008081ULL, 0x8000000000008009ULL,
        0x000000000000008aULL, 0x0000000000000088ULL,
        0x0000000080008009ULL, 0x000000008000000aULL,
        0x000000008000808bULL, 0x800000000000008bULL,
        0x8000000000008089ULL, 0x8000000000008003ULL,
        0x8000000000008002ULL, 0x8000000000000080ULL,
        0x000000000000800aULL, 0x800000008000000aULL,
        0x8000000080008081ULL, 0x8000000000008080ULL,
        0x0000000080000001ULL, 0x8000000080008008ULL
    };
# define a00 (p->u.s[0])
# define a01 (p->u.s[1])
# define a02 (p->u.s[2])
# define a03 (p->u.s[3])
# define a04 (p->u.s[4])
# define a10 (p->u.s[5])
# define a11 (p->u.s[6])
# define a12 (p->u.s[7])
# define a13 (p->u.s[8])
# define a14 (p->u.s[9])
# define a20 (p->u.s[10])
# define a21 (p->u.s[11])
# define a22 (p->u.s[12])
# define a23 (p->u.s[13])
# define a24 (p->u.s[14])
# define a30 (p->u.s[15])
# define a31 (p->u.s[16])
# define a32 (p->u.s[17])
# define a33 (p->u.s[18])
# define a34 (p->u.s[19])
# define a40 (p->u.s[20])
# define a41 (p->u.s[21])
# define a42 (p->u.s[22])
# define a43 (p->u.s[23])
# define a44 (p->u.s[24])
# define ROL64(a,x) ((a<<x)|(a>>(64-x)))

    for (i = 0; i < 24; i += 4) {
        c0 = a00 ^ a10 ^ a20 ^ a30 ^ a40;
        c1 = a01 ^ a11 ^ a21 ^ a31 ^ a41;
        c2 = a02 ^ a12 ^ a22 ^ a32 ^ a42;
        c3 = a03 ^ a13 ^ a23 ^ a33 ^ a43;
        c4 = a04 ^ a14 ^ a24 ^ a34 ^ a44;
        d0 = c4 ^ ROL64(c1, 1);
        d1 = c0 ^ ROL64(c2, 1);
        d2 = c1 ^ ROL64(c3, 1);
        d3 = c2 ^ ROL64(c4, 1);
        d4 = c3 ^ ROL64(c0, 1);

        b0 = (a00 ^ d0);
        b1 = ROL64((a11 ^ d1), 44);
        b2 = ROL64((a22 ^ d2), 43);
        b3 = ROL64((a33 ^ d3), 21);
        b4 = ROL64((a44 ^ d4), 14);
        a00 = b0 ^ ((~b1) & b2);
        a00 ^= RC[i];
        a11 = b1 ^ ((~b2) & b3);
        a22 = b2 ^ ((~b3) & b4);
        a33 = b3 ^ ((~b4) & b0);
        a44 = b4 ^ ((~b0) & b1);

        b2 = ROL64((a20 ^ d0), 3);
        b3 = ROL64((a31 ^ d1), 45);
        b4 = ROL64((a42 ^ d2), 61);
        b0 = ROL64((a03 ^ d3), 28);
        b1 = ROL64((a14 ^ d4), 20);
        a20 = b0 ^ ((~b1) & b2);
        a31 = b1 ^ ((~b2) & b3);
        a42 = b2 ^ ((~b3) & b4);
        a03 = b3 ^ ((~b4) & b0);
        a14 = b4 ^ ((~b0) & b1);

        b4 = ROL64((a40 ^ d0), 18);
        b0 = ROL64((a01 ^ d1), 1);
        b1 = ROL64((a12 ^ d2), 6);
        b2 = ROL64((a23 ^ d3), 25);
        b3 = ROL64((a34 ^ d4), 8);
        a40 = b0 ^ ((~b1) & b2);
        a01 = b1 ^ ((~b2) & b3);
        a12 = b2 ^ ((~b3) & b4);
        a23 = b3 ^ ((~b4) & b0);
        a34 = b4 ^ ((~b0) & b1);

        b1 = ROL64((a10 ^ d0), 36);
        b2 = ROL64((a21 ^ d1), 10);
        b3 = ROL64((a32 ^ d2), 15);
        b4 = ROL64((a43 ^ d3), 56);
        b0 = ROL64((a04 ^ d4), 27);
        a10 = b0 ^ ((~b1) & b2);
        a21 = b1 ^ ((~b2) & b3);
        a32 = b2 ^ ((~b3) & b4);
        a43 = b3 ^ ((~b4) & b0);
        a04 = b4 ^ ((~b0) & b1);

        b3 = ROL64((a30 ^ d0), 41);
        b4 = ROL64((a41 ^ d1), 2);
        b0 = ROL64((a02 ^ d2), 62);
        b1 = ROL64((a13 ^ d3), 55);
        b2 = ROL64((a24 ^ d4), 39);
        a30 = b0 ^ ((~b1) & b2);
        a41 = b1 ^ ((~b2) & b3);
        a02 = b2 ^ ((~b3) & b4);
        a13 = b3 ^ ((~b4) & b0);
        a24 = b4 ^ ((~b0) & b1);

        c0 = a00 ^ a20 ^ a40 ^ a10 ^ a30;
        c1 = a11 ^ a31 ^ a01 ^ a21 ^ a41;
        c2 = a22 ^ a42 ^ a12 ^ a32 ^ a02;
        c3 = a33 ^ a03 ^ a23 ^ a43 ^ a13;
        c4 = a44 ^ a14 ^ a34 ^ a04 ^ a24;
        d0 = c4 ^ ROL64(c1, 1);
        d1 = c0 ^ ROL64(c2, 1);
        d2 = c1 ^ ROL64(c3, 1);
        d3 = c2 ^ ROL64(c4, 1);
        d4 = c3 ^ ROL64(c0, 1);

        b0 = (a00 ^ d0);
        b1 = ROL64((a31 ^ d1), 44);
        b2 = ROL64((a12 ^ d2), 43);
        b3 = ROL64((a43 ^ d3), 21);
        b4 = ROL64((a24 ^ d4), 14);
        a00 = b0 ^ ((~b1) & b2);
        a00 ^= RC[i + 1];
        a31 = b1 ^ ((~b2) & b3);
        a12 = b2 ^ ((~b3) & b4);
        a43 = b3 ^ ((~b4) & b0);
        a24 = b4 ^ ((~b0) & b1);

        b2 = ROL64((a40 ^ d0), 3);
        b3 = ROL64((a21 ^ d1), 45);
        b4 = ROL64((a02 ^ d2), 61);
        b0 = ROL64((a33 ^ d3), 28);
        b1 = ROL64((a14 ^ d4), 20);
        a40 = b0 ^ ((~b1) & b2);
        a21 = b1 ^ ((~b2) & b3);
        a02 = b2 ^ ((~b3) & b4);
        a33 = b3 ^ ((~b4) & b0);
        a14 = b4 ^ ((~b0) & b1);

        b4 = ROL64((a30 ^ d0), 18);
        b0 = ROL64((a11 ^ d1), 1);
        b1 = ROL64((a42 ^ d2), 6);
        b2 = ROL64((a23 ^ d3), 25);
        b3 = ROL64((a04 ^ d4), 8);
        a30 = b0 ^ ((~b1) & b2);
        a11 = b1 ^ ((~b2) & b3);
        a42 = b2 ^ ((~b3) & b4);
        a23 = b3 ^ ((~b4) & b0);
        a04 = b4 ^ ((~b0) & b1);

        b1 = ROL64((a20 ^ d0), 36);
        b2 = ROL64((a01 ^ d1), 10);
        b3 = ROL64((a32 ^ d2), 15);
        b4 = ROL64((a13 ^ d3), 56);
        b0 = ROL64((a44 ^ d4), 27);
        a20 = b0 ^ ((~b1) & b2);
        a01 = b1 ^ ((~b2) & b3);
        a32 = b2 ^ ((~b3) & b4);
        a13 = b3 ^ ((~b4) & b0);
        a44 = b4 ^ ((~b0) & b1);

        b3 = ROL64((a10 ^ d0), 41);
        b4 = ROL64((a41 ^ d1), 2);
        b0 = ROL64((a22 ^ d2), 62);
        b1 = ROL64((a03 ^ d3), 55);
        b2 = ROL64((a34 ^ d4), 39);
        a10 = b0 ^ ((~b1) & b2);
        a41 = b1 ^ ((~b2) & b3);
        a22 = b2 ^ ((~b3) & b4);
        a03 = b3 ^ ((~b4) & b0);
        a34 = b4 ^ ((~b0) & b1);

        c0 = a00 ^ a40 ^ a30 ^ a20 ^ a10;
        c1 = a31 ^ a21 ^ a11 ^ a01 ^ a41;
        c2 = a12 ^ a02 ^ a42 ^ a32 ^ a22;
        c3 = a43 ^ a33 ^ a23 ^ a13 ^ a03;
        c4 = a24 ^ a14 ^ a04 ^ a44 ^ a34;
        d0 = c4 ^ ROL64(c1, 1);
        d1 = c0 ^ ROL64(c2, 1);
        d2 = c1 ^ ROL64(c3, 1);
        d3 = c2 ^ ROL64(c4, 1);
        d4 = c3 ^ ROL64(c0, 1);

        b0 = (a00 ^ d0);
        b1 = ROL64((a21 ^ d1), 44);
        b2 = ROL64((a42 ^ d2), 43);
        b3 = ROL64((a13 ^ d3), 21);
        b4 = ROL64((a34 ^ d4), 14);
        a00 = b0 ^ ((~b1) & b2);
        a00 ^= RC[i + 2];
        a21 = b1 ^ ((~b2) & b3);
        a42 = b2 ^ ((~b3) & b4);
        a13 = b3 ^ ((~b4) & b0);
        a34 = b4 ^ ((~b0) & b1);

        b2 = ROL64((a30 ^ d0), 3);
        b3 = ROL64((a01 ^ d1), 45);
        b4 = ROL64((a22 ^ d2), 61);
        b0 = ROL64((a43 ^ d3), 28);
        b1 = ROL64((a14 ^ d4), 20);
        a30 = b0 ^ ((~b1) & b2);
        a01 = b1 ^ ((~b2) & b3);
        a22 = b2 ^ ((~b3) & b4);
        a43 = b3 ^ ((~b4) & b0);
        a14 = b4 ^ ((~b0) & b1);

        b4 = ROL64((a10 ^ d0), 18);
        b0 = ROL64((a31 ^ d1), 1);
        b1 = ROL64((a02 ^ d2), 6);
        b2 = ROL64((a23 ^ d3), 25);
        b3 = ROL64((a44 ^ d4), 8);
        a10 = b0 ^ ((~b1) & b2);
        a31 = b1 ^ ((~b2) & b3);
        a02 = b2 ^ ((~b3) & b4);
        a23 = b3 ^ ((~b4) & b0);
        a44 = b4 ^ ((~b0) & b1);

        b1 = ROL64((a40 ^ d0), 36);
        b2 = ROL64((a11 ^ d1), 10);
        b3 = ROL64((a32 ^ d2), 15);
        b4 = ROL64((a03 ^ d3), 56);
        b0 = ROL64((a24 ^ d4), 27);
        a40 = b0 ^ ((~b1) & b2);
        a11 = b1 ^ ((~b2) & b3);
        a32 = b2 ^ ((~b3) & b4);
        a03 = b3 ^ ((~b4) & b0);
        a24 = b4 ^ ((~b0) & b1);

        b3 = ROL64((a20 ^ d0), 41);
        b4 = ROL64((a41 ^ d1), 2);
        b0 = ROL64((a12 ^ d2), 62);
        b1 = ROL64((a33 ^ d3), 55);
        b2 = ROL64((a04 ^ d4), 39);
        a20 = b0 ^ ((~b1) & b2);
        a41 = b1 ^ ((~b2) & b3);
        a12 = b2 ^ ((~b3) & b4);
        a33 = b3 ^ ((~b4) & b0);
        a04 = b4 ^ ((~b0) & b1);

        c0 = a00 ^ a30 ^ a10 ^ a40 ^ a20;
        c1 = a21 ^ a01 ^ a31 ^ a11 ^ a41;
        c2 = a42 ^ a22 ^ a02 ^ a32 ^ a12;
        c3 = a13 ^ a43 ^ a23 ^ a03 ^ a33;
        c4 = a34 ^ a14 ^ a44 ^ a24 ^ a04;
        d0 = c4 ^ ROL64(c1, 1);
        d1 = c0 ^ ROL64(c2, 1);
        d2 = c1 ^ ROL64(c3, 1);
        d3 = c2 ^ ROL64(c4, 1);
        d4 = c3 ^ ROL64(c0, 1);

        b0 = (a00 ^ d0);
        b1 = ROL64((a01 ^ d1), 44);
        b2 = ROL64((a02 ^ d2), 43);
        b3 = ROL64((a03 ^ d3), 21);
        b4 = ROL64((a04 ^ d4), 14);
        a00 = b0 ^ ((~b1) & b2);
        a00 ^= RC[i + 3];
        a01 = b1 ^ ((~b2) & b3);
        a02 = b2 ^ ((~b3) & b4);
        a03 = b3 ^ ((~b4) & b0);
        a04 = b4 ^ ((~b0) & b1);

        b2 = ROL64((a10 ^ d0), 3);
        b3 = ROL64((a11 ^ d1), 45);
        b4 = ROL64((a12 ^ d2), 61);
        b0 = ROL64((a13 ^ d3), 28);
        b1 = ROL64((a14 ^ d4), 20);
        a10 = b0 ^ ((~b1) & b2);
        a11 = b1 ^ ((~b2) & b3);
        a12 = b2 ^ ((~b3) & b4);
        a13 = b3 ^ ((~b4) & b0);
        a14 = b4 ^ ((~b0) & b1);

        b4 = ROL64((a20 ^ d0), 18);
        b0 = ROL64((a21 ^ d1), 1);
        b1 = ROL64((a22 ^ d2), 6);
        b2 = ROL64((a23 ^ d3), 25);
        b3 = ROL64((a24 ^ d4), 8);
        a20 = b0 ^ ((~b1) & b2);
        a21 = b1 ^ ((~b2) & b3);
        a22 = b2 ^ ((~b3) & b4);
        a23 = b3 ^ ((~b4) & b0);
        a24 = b4 ^ ((~b0) & b1);

        b1 = ROL64((a30 ^ d0), 36);
        b2 = ROL64((a31 ^ d1), 10);
        b3 = ROL64((a32 ^ d2), 15);
        b4 = ROL64((a33 ^ d3), 56);
        b0 = ROL64((a34 ^ d4), 27);
        a30 = b0 ^ ((~b1) & b2);
        a31 = b1 ^ ((~b2) & b3);
        a32 = b2 ^ ((~b3) & b4);
        a33 = b3 ^ ((~b4) & b0);
        a34 = b4 ^ ((~b0) & b1);

        b3 = ROL64((a40 ^ d0), 41);
        b4 = ROL64((a41 ^ d1), 2);
        b0 = ROL64((a42 ^ d2), 62);
        b1 = ROL64((a43 ^ d3), 55);
        b2 = ROL64((a44 ^ d4), 39);
        a40 = b0 ^ ((~b1) & b2);
        a41 = b1 ^ ((~b2) & b3);
        a42 = b2 ^ ((~b3) & b4);
        a43 = b3 ^ ((~b4) & b0);
        a44 = b4 ^ ((~b0) & b1);
    }
}

/*
** Initialize a new hash.  iSize determines the size of the hash
** in bits and should be one of 224, 256, 384, or 512.  Or iSize
** can be zero to use the default hash size of 256 bits.
*/
static void SHA3Init(
    SHA3Context * p,
    int iSize
) {
    memset(p, 0, sizeof(*p));
    if (iSize >= 128 && iSize <= 512) {
        p->nRate = (1600 - ((iSize + 31) & ~31) * 2) / 8;
    } else {
        p->nRate = (1600 - 2 * 256) / 8;
    }
#if SHA3_BYTEORDER==1234
    /* Known to be little-endian at compile-time. No-op */
#elif SHA3_BYTEORDER==4321
    p->ixMask = 7;              /* Big-endian */
#else
    {
        static unsigned int one = 1;
        if (1 == *(unsigned char *) &one) {
            /* Little endian.  No byte swapping. */
            p->ixMask = 0;
        } else {
            /* Big endian.  Byte swap. */
            p->ixMask = 7;
        }
    }
#endif
}

/*
** Make consecutive calls to the SHA3Update function to add new content
** to the hash
*/
static void SHA3Update(
    SHA3Context * p,
    const unsigned char *aData,
    unsigned int nData
) {
    unsigned int i = 0;
    if (aData == 0)
        return;
#if SHA3_BYTEORDER==1234
    if ((p->nLoaded % 8) == 0
        && ((aData - (const unsigned char *) 0) & 7) == 0) {
        for (; i + 7 < nData; i += 8) {
            p->u.s[p->nLoaded / 8] ^= *(u64 *) & aData[i];
            p->nLoaded += 8;
            if (p->nLoaded >= p->nRate) {
                KeccakF1600Step(p);
                p->nLoaded = 0;
            }
        }
    }
#endif
    for (; i < nData; i++) {
#if SHA3_BYTEORDER==1234
        p->u.x[p->nLoaded] ^= aData[i];
#elif SHA3_BYTEORDER==4321
        p->u.x[p->nLoaded ^ 0x07] ^= aData[i];
#else
        p->u.x[p->nLoaded ^ p->ixMask] ^= aData[i];
#endif
        p->nLoaded++;
        if (p->nLoaded == p->nRate) {
            KeccakF1600Step(p);
            p->nLoaded = 0;
        }
    }
}

/*
** After all content has been added, invoke SHA3Final() to compute
** the final hash.  The function returns a pointer to the binary
** hash value.
*/
static unsigned char *SHA3Final(
    SHA3Context * p
) {
    unsigned int i;
    if (p->nLoaded == p->nRate - 1) {
        const unsigned char c1 = 0x86;
        SHA3Update(p, &c1, 1);
    } else {
        const unsigned char c2 = 0x06;
        const unsigned char c3 = 0x80;
        SHA3Update(p, &c2, 1);
        p->nLoaded = p->nRate - 1;
        SHA3Update(p, &c3, 1);
    }
    for (i = 0; i < p->nRate; i++) {
        p->u.x[i + p->nRate] = p->u.x[i ^ p->ixMask];
    }
    return &p->u.x[p->nRate];
}

/* End of the hashing logic
*****************************************************************************/

/*
** Implementation of the sha3(X,SIZE) function.
**
** Return a BLOB which is the SIZE-bit SHA3 hash of X.  The default
** size is 256.  If X is a BLOB, it is hashed as is.
** For all other non-NULL types of input, X is converted into a UTF-8 string
** and the string is hashed without the trailing 0x00 terminator.  The hash
** of a NULL value is NULL.
*/
static void sha3Func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
    SHA3Context cx;
    int eType = sqlite3_value_type(argv[0]);
    int nByte = sqlite3_value_bytes(argv[0]);
    int iSize;
    if (argc == 1) {
        iSize = 256;
    } else {
        iSize = sqlite3_value_int(argv[1]);
        if (iSize != 224 && iSize != 256 && iSize != 384 && iSize != 512) {
            sqlite3_result_error(context,
                "SHA3 size should be one of: 224 256 " "384 512", -1);
            return;
        }
    }
    if (eType == SQLITE_NULL)
        return;
    SHA3Init(&cx, iSize);
    if (eType == SQLITE_BLOB) {
        SHA3Update(&cx, sqlite3_value_blob(argv[0]), nByte);
    } else {
        SHA3Update(&cx, sqlite3_value_text(argv[0]), nByte);
    }
    sqlite3_result_blob(context, SHA3Final(&cx), iSize / 8, SQLITE_TRANSIENT);
}

/* Compute a string using sqlite3_vsnprintf() with a maximum length
** of 50 bytes and add it to the hash.
*/
static void hash_step_vformat(
    SHA3Context * p,            /* Add content to this context */
    const char *zFormat,
    ...
) {
    va_list ap;
    int n;
    char zBuf[50];
    va_start(ap, zFormat);
    sqlite3_vsnprintf(sizeof(zBuf), zBuf, zFormat, ap);
    va_end(ap);
    n = (int) strlen(zBuf);
    SHA3Update(p, (unsigned char *) zBuf, n);
}

/*
** Implementation of the sha3_query(SQL,SIZE) function.
**
** This function compiles and runs the SQL statement(s) given in the
** argument. The results are hashed using a SIZE-bit SHA3.  The default
** size is 256.
**
** The format of the byte stream that is hashed is summarized as follows:
**
**       S<n>:<sql>
**       R
**       N
**       I<int>
**       F<ieee-float>
**       B<size>:<bytes>
**       T<size>:<text>
**
** <sql> is the original SQL text for each statement run and <n> is
** the size of that text.  The SQL text is UTF-8.  A single R character
** occurs before the start of each row.  N means a NULL value.
** I mean an 8-byte little-endian integer <int>.  F is a floating point
** number with an 8-byte little-endian IEEE floating point value <ieee-float>.
** B means blobs of <size> bytes.  T means text rendered as <size>
** bytes of UTF-8.  The <n> and <size> values are expressed as an ASCII
** text integers.
**
** For each SQL statement in the X input, there is one S segment.  Each
** S segment is followed by zero or more R segments, one for each row in the
** result set.  After each R, there are one or more N, I, F, B, or T segments,
** one for each column in the result set.  Segments are concatentated directly
** with no delimiters of any kind.
*/
static void sha3QueryFunc(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
    sqlite3 *db = sqlite3_context_db_handle(context);
    const char *zSql = (const char *) sqlite3_value_text(argv[0]);
    sqlite3_stmt *pStmt = 0;
    int nCol;                   /* Number of columns in the result set */
    int i;                      /* Loop counter */
    int rc;
    int n;
    const char *z;
    SHA3Context cx;
    int iSize;

    if (argc == 1) {
        iSize = 256;
    } else {
        iSize = sqlite3_value_int(argv[1]);
        if (iSize != 224 && iSize != 256 && iSize != 384 && iSize != 512) {
            sqlite3_result_error(context,
                "SHA3 size should be one of: 224 256 " "384 512", -1);
            return;
        }
    }
    if (zSql == 0)
        return;
    SHA3Init(&cx, iSize);
    while (zSql[0]) {
        rc = sqlite3_prepare_v2(db, zSql, -1, &pStmt, &zSql);
        if (rc) {
            char *zMsg = sqlite3_mprintf("error SQL statement [%s]: %s",
                zSql, sqlite3_errmsg(db));
            sqlite3_finalize(pStmt);
            sqlite3_result_error(context, zMsg, -1);
            sqlite3_free(zMsg);
            return;
        }
        if (!sqlite3_stmt_readonly(pStmt)) {
            char *zMsg =
                sqlite3_mprintf("non-query: [%s]", sqlite3_sql(pStmt));
            sqlite3_finalize(pStmt);
            sqlite3_result_error(context, zMsg, -1);
            sqlite3_free(zMsg);
            return;
        }
        nCol = sqlite3_column_count(pStmt);
        z = sqlite3_sql(pStmt);
        if (z) {
            n = (int) strlen(z);
            hash_step_vformat(&cx, "S%d:", n);
            SHA3Update(&cx, (unsigned char *) z, n);
        }

        /* Compute a hash over the result of the query */
        while (SQLITE_ROW == sqlite3_step(pStmt)) {
            SHA3Update(&cx, (const unsigned char *) "R", 1);
            for (i = 0; i < nCol; i++) {
                switch (sqlite3_column_type(pStmt, i)) {
                case SQLITE_NULL:      /* */  {
                        SHA3Update(&cx, (const unsigned char *) "N", 1);
                        break;
                    }
                case SQLITE_INTEGER:   /* */  {
                        sqlite3_uint64 u;
                        int j;
                        unsigned char x[9];
                        sqlite3_int64 v = sqlite3_column_int64(pStmt, i);
                        memcpy(&u, &v, 8);
                        for (j = 8; j >= 1; j--) {
                            x[j] = u & 0xff;
                            u >>= 8;
                        }
                        x[0] = 'I';
                        SHA3Update(&cx, x, 9);
                        break;
                    }
                case SQLITE_FLOAT:     /* */  {
                        sqlite3_uint64 u;
                        int j;
                        unsigned char x[9];
                        double r = sqlite3_column_double(pStmt, i);
                        memcpy(&u, &r, 8);
                        for (j = 8; j >= 1; j--) {
                            x[j] = u & 0xff;
                            u >>= 8;
                        }
                        x[0] = 'F';
                        SHA3Update(&cx, x, 9);
                        break;
                    }
                case SQLITE_TEXT:      /* */  {
                        int n2 = sqlite3_column_bytes(pStmt, i);
                        const unsigned char *z2 =
                            sqlite3_column_text(pStmt, i);
                        hash_step_vformat(&cx, "T%d:", n2);
                        SHA3Update(&cx, z2, n2);
                        break;
                    }
                case SQLITE_BLOB:      /* */  {
                        int n2 = sqlite3_column_bytes(pStmt, i);
                        const unsigned char *z2 =
                            sqlite3_column_blob(pStmt, i);
                        hash_step_vformat(&cx, "B%d:", n2);
                        SHA3Update(&cx, z2, n2);
                        break;
                    }
                }
            }
        }
        sqlite3_finalize(pStmt);
    }
    sqlite3_result_blob(context, SHA3Final(&cx), iSize / 8, SQLITE_TRANSIENT);
}


#ifdef _WIN32
__declspec(dllexport)
#endif
int sqlite3_shathree_init(
    sqlite3 * db,
    char **pzErrMsg,
    const sqlite3_api_routines * pApi
) {
    int rc = SQLITE_OK;
    SQLITE_EXTENSION_INIT2(pApi);
    (void) pzErrMsg;            /* Unused parameter */
    rc = sqlite3_create_function(db, "sha3", 1,
        SQLITE_UTF8 | SQLITE_INNOCUOUS | SQLITE_DETERMINISTIC,
        0, sha3Func, 0, 0);
    if (rc == SQLITE_OK) {
        rc = sqlite3_create_function(db, "sha3", 2,
            SQLITE_UTF8 | SQLITE_INNOCUOUS | SQLITE_DETERMINISTIC,
            0, sha3Func, 0, 0);
    }
    if (rc == SQLITE_OK) {
        rc = sqlite3_create_function(db, "sha3_query", 1,
            SQLITE_UTF8 | SQLITE_DIRECTONLY, 0, sha3QueryFunc, 0, 0);
    }
    if (rc == SQLITE_OK) {
        rc = sqlite3_create_function(db, "sha3_query", 2,
            SQLITE_UTF8 | SQLITE_DIRECTONLY, 0, sha3QueryFunc, 0, 0);
    }
    return rc;
}


/*
file none
*/
/*jslint-enable*/


#include "sqlmath_jenks.c"
typedef struct DbExecBindElem {
    const char *buf;
    char *key;
    int buflen;
    char datatype;
} DbExecBindElem;
// track how many sqlite-db open
static int dbCount = 0;


// file sqlmath_ext - SQLMATH_API
SQLMATH_API int __dbFileImportOrExport(
    sqlite3 * pInMemory,
    char *zFilename,
    const int isSave
) {
    // fprintf(stderr, "\nsqlmath.dbFileImportOrExport(pp=%p ff=%s ss=%d)\n",
    //     pInMemory, zFilename, isSave);
/*
** https://www.sqlite.org/backup.html
** This function is used to load the contents of a database file on disk
** into the "main" database of open database connection pInMemory, or
** to save the current contents of the database opened by pInMemory into
** a database file on disk. pInMemory is probably an in-memory database,
** but this function will also work fine if it is not.
**
** Parameter zFilename points to a nul-terminated string containing the
** name of the database file on disk to load from or save to. If parameter
** isSave is non-zero, then the contents of the file zFilename are
** overwritten with the contents of the database opened by pInMemory. If
** parameter isSave is zero, then the contents of the database opened by
** pInMemory are replaced by data loaded from the file zFilename.
**
** If the operation is successful, SQLITE_OK is returned. Otherwise, if
** an error occurs, an SQLite error code is returned.
*/
    int rc;                     /* Function return code */
    sqlite3 *pFile;             /* Database connection opened on zFilename */
    sqlite3_backup *pBackup;    /* Backup object used to copy data */
    sqlite3 *pTo;               /* Database to copy to (pFile or pInMemory) */
    sqlite3 *pFrom;             /* Database to copy from (pFile or pInMemory) */
    /* Open the database file identified by zFilename. Exit early if this fails
     ** for any reason. */
    rc = sqlite3_open(zFilename, &pFile);
    if (rc == SQLITE_OK) {
        /* If this is a 'load' operation (isSave==0), then data is copied
         ** from the database file just opened to database pInMemory.
         ** Otherwise, if this is a 'save' operation (isSave==1), then data
         ** is copied from pInMemory to pFile.  Set the variables pFrom and
         ** pTo accordingly. */
        pFrom = (isSave ? pInMemory : pFile);
        pTo = (isSave ? pFile : pInMemory);
        /* Set up the backup procedure to copy from the "main" database of
         ** connection pFile to the main database of connection pInMemory.
         ** If something goes wrong, pBackup will be set to NULL and an error
         ** code and message left in connection pTo.
         **
         ** If the backup object is successfully created, call backup_step()
         ** to copy data from pFile to pInMemory. Then call backup_finish()
         ** to release resources associated with the pBackup object.  If an
         ** error occurred, then an error code and message will be left in
         ** connection pTo. If no error occurred, then the error code belonging
         ** to pTo is set to SQLITE_OK.
         */
        pBackup = sqlite3_backup_init(pTo, "main", pFrom, "main");
        if (pBackup) {
            (void) sqlite3_backup_step(pBackup, -1);
            (void) sqlite3_backup_finish(pBackup);
        }
        rc = sqlite3_errcode(pTo);
    }
    /* Close the database connection opened on database file zFilename
     ** and return the result of this function. */
    (void) sqlite3_close(pFile);
    return rc;
}

SQLMATH_API void dbClose(
    Jsbaton * baton
) {
// int sqlite3_close_v2(sqlite3*);
    // declare var
    int errcode = 0;
    sqlite3 *db = (sqlite3 *) baton->argv[0];
    // call c-function
    errcode = sqlite3_close_v2(db);
    JSBATON_ASSERT_OK();
    if (db != NULL) {
        dbCount -= 1;
        // fprintf(stderr, "\nsqlmath.dbClose(argv0=%lld db=%lld dbCount=%d)\n",
        //     baton->argv[0], (int64_t) db, dbCount);
    }
  catch_error:
    (void) 0;
}

// SQLMATH_API dbexec - start
SQLMATH_API void dbExec(
    Jsbaton * baton
) {
// This function will run <zSql> in <db> and save any result (list of tables
// containing rows from SELECT/pragma/etc) as serialized json-string in <str99>.
    // declare var
    DbExecBindElem *bindElem = NULL;
    DbExecBindElem *bindList = NULL;
    const char **pzShared = ((const char **) baton->argv) + 8;
    const char *zBind = (const char *) baton + SQLITE_DATATYPE_OFFSET;
    const char *zSql = jsbatonValueStringArgi(baton, 1);
    const char *zTmp = NULL;
    double rTmp = 0;
    int bindByKey = (int) baton->argv[3];
    int bindIdx = 0;
    int bindListLength = (int) baton->argv[2];
    int errcode = 0;
    int ii = 0;
    int jj = 0;
    int nCol = 0;
    int responseType = (int) baton->argv[4];
    int64_t iTmp = 0;
    sqlite3 *db = (sqlite3 *) baton->argv[0];
    sqlite3_stmt *pStmt = NULL; /* The current SQL statement */
    static const char bindPrefix[] = "$:@";
    // str99 - init
    STR99_ALLOCA(str99);
    // fprintf(stderr, "\nsqlmath.dbExec(db=%lld blen=%d sql=%s)\n",
    //     (int64_t) db, bindListLength, zSql);
#ifndef EMSCRIPTEN
    // mutex enter
    sqlite3_mutex_enter(sqlite3_db_mutex(db));
#endif                          // EMSCRIPTEN
    if (zSql == NULL) {
        errcode = SQLITE_ERROR_ZSQL_NULL;
        JSBATON_ASSERT_OK();
    }
    // init bindList
    bindList =
        (DbExecBindElem *) sqlite3_malloc(bindListLength *
        sizeof(DbExecBindElem));
    memset(bindList, 0, bindListLength * sizeof(DbExecBindElem));
    if (bindListLength > 0 && bindList == NULL) {
        errcode = SQLITE_NOMEM;
        JSBATON_ASSERT_OK()
    }
    bindElem = bindList;
    ii = 1;
    while (ii <= bindListLength) {
        // init key and
        if (bindByKey) {
            zBind += 1;
            bindElem->key = (char *) zBind + 4;
            zBind += 4 + *(int32_t *) zBind;
        }
        // init datatype
        bindElem->datatype = zBind[0];
        zBind += 1;
        switch (bindElem->datatype) {
        case SQLITE_DATATYPE_BLOB:
        case SQLITE_DATATYPE_TEXT:
            bindElem->buflen = *(int32_t *) zBind;
            bindElem->buf = zBind + 4;
            zBind += 4 + bindElem->buflen;
            break;
        case SQLITE_DATATYPE_FLOAT:
        case SQLITE_DATATYPE_INTEGER:
            bindElem->buf = zBind;
            zBind += 8;
            break;
        case SQLITE_DATATYPE_INTEGER_0:
        case SQLITE_DATATYPE_INTEGER_1:
        case SQLITE_DATATYPE_NULL:
        case SQLITE_DATATYPE_TEXT_0:
            break;
        case SQLITE_DATATYPE_SHAREDARRAYBUFFER:
            bindElem->buflen = *(int32_t *) zBind;
            bindElem->buf = *pzShared;
            pzShared += 1;
            zBind += 4;
            break;
        default:
            fprintf(stderr, "\nsqlmath.dbExec(ii=%d datatype=%d len=%d)\n",
                ii, bindElem->datatype, bindElem->buflen);
            errcode = SQLITE_ERROR_DATATYPE_INVALID;
            JSBATON_ASSERT_OK();
        }
        bindElem += 1;
        ii += 1;
    }
    // bracket database [
    sqlite3_str_appendchar(str99, 1, '[');
    // loop over each table - start
    while (1) {
        // ignore whitespace
        while (*zSql == ' ' || *zSql == '\t' || *zSql == '\n'
            || *zSql == '\r') {
            zSql += 1;
        }
        errcode = sqlite3_prepare_v2(db, zSql, -1, &pStmt, &zTmp);
        JSBATON_ASSERT_OK();
        if (errcode != 0 || *zSql == '\x00') {
            break;
        }
        zSql = zTmp;
        // bind bindList to pStmt
        bindElem = bindList;
        ii = 1;
        while (ii <= bindListLength) {
            bindIdx = ii;
            jj = 0;
            while (jj < 3) {
                // if bindByKey, then get idx for given key
                if (bindByKey) {
                    bindElem->key[0] = bindPrefix[jj];
                    bindIdx =
                        sqlite3_bind_parameter_index(pStmt, bindElem->key);
                }
                if (bindIdx > 0) {
                    switch (bindElem->datatype) {
                    case SQLITE_DATATYPE_BLOB:
                        errcode =
                            sqlite3_bind_blob(pStmt, bindIdx, bindElem->buf,
                            bindElem->buflen, SQLITE_STATIC);
                        break;
                    case SQLITE_DATATYPE_FLOAT:
                        errcode =
                            sqlite3_bind_double(pStmt, bindIdx,
                            *(double *) bindElem->buf);
                        break;
                    case SQLITE_DATATYPE_INTEGER:
                        errcode =
                            sqlite3_bind_int64(pStmt, bindIdx,
                            *(int64_t *) bindElem->buf);
                        break;
                    case SQLITE_DATATYPE_INTEGER_0:
                        errcode = sqlite3_bind_int(pStmt, bindIdx, 0);
                        break;
                    case SQLITE_DATATYPE_INTEGER_1:
                        errcode = sqlite3_bind_int(pStmt, bindIdx, 1);
                        break;
                    case SQLITE_DATATYPE_NULL:
                        errcode = sqlite3_bind_null(pStmt, bindIdx);
                        break;
                    case SQLITE_DATATYPE_SHAREDARRAYBUFFER:
                        errcode =
                            sqlite3_bind_blob(pStmt, bindIdx, bindElem->buf,
                            bindElem->buflen, SQLITE_STATIC);
                        break;
                    case SQLITE_DATATYPE_TEXT_0:
                        errcode =
                            sqlite3_bind_text(pStmt, bindIdx, "", 0,
                            SQLITE_STATIC);
                        break;
                    case SQLITE_DATATYPE_TEXT:
                        errcode =
                            sqlite3_bind_text(pStmt, bindIdx, bindElem->buf,
                            bindElem->buflen, SQLITE_STATIC);
                        break;
                    default:
                        fprintf(stderr,
                            "\nsqlmath.dbExec(ii=%d  datatype=%d  len=%d)\n",
                            ii, bindElem->datatype, bindElem->buflen);
                        errcode = SQLITE_ERROR_DATATYPE_INVALID;
                    }
                    // ignore bind-range-error
                    if (errcode != SQLITE_RANGE) {
                        JSBATON_ASSERT_OK();
                    }
                }
                if (!bindByKey) {
                    break;
                }
                jj += 1;
            }
            bindElem += 1;
            ii += 1;
        }
        nCol = -1;
        // loop over each row - start
        while (1) {
            errcode = sqlite3_step(pStmt);
            if (errcode == SQLITE_DONE) {
                errcode = SQLITE_OK;
            }
            // if no next row, then break
            if (errcode != SQLITE_ROW) {
                // cleanup pStmt
                sqlite3_finalize(pStmt);
                pStmt = NULL;
                JSBATON_ASSERT_OK();
                break;
            }
            // switch (responseType) - start
            switch (responseType) {
            case SQLITE_RESPONSETYPE_LASTBLOB:
                // export last-value as blob
                if (nCol == -1) {
                    nCol = sqlite3_column_count(pStmt);
                }
                sqlite3_str_reset(str99);
                sqlite3_str_append(str99,
                    (const char *) sqlite3_column_blob(pStmt, nCol - 1),
                    sqlite3_column_bytes(pStmt, nCol - 1));
                break;
            default:
                // insert row of column-names
                if (nCol == -1) {
                    nCol = sqlite3_column_count(pStmt);
                    if (sqlite3_str_length(str99) > 1) {
                        sqlite3_str_appendchar(str99, 1, ',');
                        sqlite3_str_appendchar(str99, 1, '\n');
                        sqlite3_str_appendchar(str99, 1, '\n');
                    }
                    // bracket table [
                    sqlite3_str_appendchar(str99, 1, '[');
                    // bracket column [
                    sqlite3_str_appendchar(str99, 1, '[');
                    // loop over each column-name
                    ii = 0;
                    while (ii < nCol) {
                        if (ii > 0) {
                            sqlite3_str_appendchar(str99, 1, ',');
                        }
                        zTmp = sqlite3_column_name(pStmt, ii);
                        str99JsonAppendText(str99, zTmp, strlen(zTmp));
                        ii += 1;
                    }
                    // bracket column ]
                    sqlite3_str_appendchar(str99, 1, ']');
                }
                // bracket row [
                sqlite3_str_appendchar(str99, 1, ',');
                sqlite3_str_appendchar(str99, 1, '\n');
                sqlite3_str_appendchar(str99, 1, '[');
                // loop over each column-value
                ii = 0;
                while (ii < nCol) {
                    if (ii > 0) {
                        sqlite3_str_appendchar(str99, 1, ',');
                    }
                    switch (sqlite3_column_type(pStmt, ii)) {
                    case SQLITE_INTEGER:
                        iTmp = sqlite3_column_int64(pStmt, ii);
                        if (JS_MIN_SAFE_INTEGER <= iTmp
                            // convert integer to double
                            && iTmp <= JS_MAX_SAFE_INTEGER) {
                            sqlite3_str_append(str99,
                                (const char *) sqlite3_column_text(pStmt, ii),
                                sqlite3_column_bytes(pStmt, ii));
                        } else {
                            // convert integer to string
                            str99JsonAppendText(str99,
                                (const char *) sqlite3_column_text(pStmt, ii),
                                sqlite3_column_bytes(pStmt, ii));
                        }
                        break;
                    case SQLITE_FLOAT:
                        rTmp = sqlite3_column_double(pStmt, ii);
                        if (isnan(rTmp) || rTmp == -INFINITY
                            || rTmp == INFINITY) {
                            sqlite3_str_append(str99, "null", 4);
                        } else {
                            sqlite3_str_append(str99,
                                (const char *) sqlite3_column_text(pStmt, ii),
                                sqlite3_column_bytes(pStmt, ii));
                        }
                        break;
                    case SQLITE_TEXT:
                        // append text as json-escaped string
                        str99JsonAppendText(str99,
                            (const char *) sqlite3_column_text(pStmt, ii),
                            sqlite3_column_bytes(pStmt, ii));
                        break;
                        // case SQLITE_BLOB:
                    default:   /* case SQLITE_NULL: */
                        sqlite3_str_append(str99, "null", 4);
                        break;
                    }
                    ii += 1;
                }
                // bracket row ]
                sqlite3_str_appendchar(str99, 1, ']');
            }
            // switch (responseType) - end
        }
        // while (1)
        // loop over each row - end
        switch (responseType) {
        case SQLITE_RESPONSETYPE_LASTBLOB:
            break;
        default:
            if (nCol != -1) {
                // bracket table ]
                sqlite3_str_appendchar(str99, 1, ']');
            }
        }
    }
    // while (1)
    // loop over each table - end
    switch (responseType) {
    case SQLITE_RESPONSETYPE_LASTBLOB:
        break;
    default:
        // bracket database ]
        sqlite3_str_appendchar(str99, 1, ']');
        sqlite3_str_appendchar(str99, 1, '\n');
    }
    errcode = sqlite3_str_errcode(str99);
    JSBATON_ASSERT_OK();
    // copy str99 to baton
    baton->argv[7] = sqlite3_str_length(str99);
    baton->bufv[7] = (int64_t) str99->zText;
  catch_error:
    // cleanup pStmt
    sqlite3_finalize(pStmt);
    // cleanup bindList
    sqlite3_free(bindList);
    // cleanup str99
    if (errcode != SQLITE_OK && str99->zText) {
        sqlite3_free(str99->zText);
    }
#ifndef EMSCRIPTEN
    // mutex leave
    sqlite3_mutex_leave(sqlite3_db_mutex(db));
#endif                          // EMSCRIPTEN
}

// SQLMATH_API dbexec - end

SQLMATH_API void dbFileImportOrExport(
    Jsbaton * baton
) {
// This function will load/save <zFilename> to/from <db>
    // declare var
    int errcode = 0;
    sqlite3 *db = (sqlite3 *) baton->argv[0];
    // call __dbFileImportOrExport()
    errcode = __dbFileImportOrExport(   //
        db,                     //
        (char *) jsbatonValueStringArgi(baton, 1),      //
        (const int) baton->argv[2]);
    JSBATON_ASSERT_OK();
  catch_error:
    (void) 0;
}

SQLMATH_API void dbNoop(
    Jsbaton * baton
) {
// this function will do nothing
    UNUSED_PARAMETER(baton);
}

SQLMATH_API void dbOpen(
    Jsbaton * baton
) {
// int sqlite3_open_v2(
//   const char *filename,   /* Database filename (UTF-8) */
//   sqlite3 **ppDb,         /* OUT: SQLite db handle */
//   int flags,              /* Flags */
//   const char *zVfs        /* Name of VFS module to use */
// );
    // declare var
    int errcode = 0;
    sqlite3 *db;
    const char *filename = jsbatonValueStringArgi(baton, 0);
    const int flags = (int) baton->argv[2];
    // call c-function
    errcode = sqlite3ApiGet()->open_v2( //
        filename,               // filename
        &db,                    // db
        flags,                  // flags
        NULL);
    JSBATON_ASSERT_OK();
    dbCount += 1;
    // fprintf(stderr, "\nsqlmath.dbOpen(dbCount=%d ff=%s)\n", dbCount,
    //     filename);
    // save db
    baton->argv[0] = (int64_t) db;
  catch_error:
    (void) 0;
}

SQLMATH_API void vector99_buf_free(
    vector99 * vec99
) {
// this function will free <vec99>->buf and reset everything to zero
    if (vec99 == NULL) {
        return;
    }
    sqlite3_free(vec99->buf);
    memset(vec99, 0, sizeof(*vec99));
}

SQLMATH_API int vector99_buf_malloc(
    vector99 * vec99
) {
// this function will malloc <vec99>->buf, or return SQLITE_NOMEM
    static const int alloc0 = 256;
    if (vec99 == NULL) {
        return SQLITE_NOMEM;
    }
    vec99->buf = sqlite3_malloc(alloc0 * sizeof(double));
    if (vector99_nomem(vec99)) {
        return SQLITE_NOMEM;
    }
    vec99->alloc = alloc0;
    vec99->size = 0;
    vec99->arg0 = NAN;
    vec99->arg1 = NAN;
    return SQLITE_OK;
}

SQLMATH_API const int vector99_nomem(
    const vector99 * vec99
) {
// this function will check if <vec99>->buf is null
    return vec99 == NULL || vec99->buf == NULL;
}

SQLMATH_API double vector99_pop_front(
    vector99 * vec99
) {
// this function will pop_front <vec99>, or return NAN if empty
    if (vector99_nomem(vec99) || vec99->size <= 0) {
        return NAN;
    }
    double val = vec99->buf[0];
    vec99->size -= 1;
    memmove(vec99->buf, vec99->buf + 1, vec99->size * sizeof(double));
    return val;
}

SQLMATH_API int vector99_push_back(
    vector99 * vec99,
    double val
) {
// this function will push_back <val> to <vec99>,
// and dynamically grow <vec99> if necessary
    // error - nomem
    if (vector99_nomem(vec99)) {
        return SQLITE_NOMEM;
    }
    if (vec99->size >= vec99->alloc) {
        vec99->alloc *= 2;
        // error - toobig
        if (vec99->alloc > SQLITE_MAX_LENGTH2) {
            vector99_buf_free(vec99);
            return SQLITE_TOOBIG;
        }
        double *buf_old = vec99->buf;
        vec99->buf =
            sqlite3_realloc(vec99->buf, vec99->alloc * sizeof(double));
        // error - nomem
        if (vector99_nomem(vec99)) {
            // dont forget to free buf_old
            vec99->buf = buf_old;
            vector99_buf_free(vec99);
            return SQLITE_NOMEM;
        }
    }
    vec99->buf[vec99->size] = val;
    vec99->size += 1;
    return SQLITE_OK;
}

// SQLMATH_API vector99 - end

SQLMATH_API int doubleSign(
    const double aa
) {
// this function will return sign of <aa>
    return aa < 0 ? -1 : aa > 0 ? 1 : 0;
}

SQLMATH_API int doubleSortCompare(
    const void *aa,
    const void *bb
) {
// this function will compare <aa> with <bb>
    const double cc = *(double *) aa - *(double *) bb;
    return cc < 0 ? -1 : cc > 0 ? 1 : 0;
}

SQLMATH_API const char *jsbatonValueErrmsg(
    Jsbaton * baton
) {
// this function will return <baton>->errmsg
    return (const char *) baton->errmsg;
}

SQLMATH_API const char *jsbatonValueStringArgi(
    Jsbaton * baton,
    int argi
) {
// this function will return string-value from <baton> at given <argi>
    if (baton->argv[argi] == 0) {
        return NULL;
    }
    return ((const char *) baton) + ((size_t) baton->argv[argi] + 1 + 4);
}

SQLMATH_API int noop(
) {
// this function will do nothing except return 0
    return 0;
}

SQLMATH_API const char *sqlmathSnprintfTrace(
    char *buf,
    const char *prefix,
    const char *errmsg,
    const char *func,
    const char *file,
    int line
) {
// this function will write <errmsg> to <buf> with additional trace-info
    if (snprintf(buf, SIZEOF_MESSAGE_DEFAULT, "%s%s\n    at %s (%s:%d)",
            prefix, errmsg, func, file, line) < 0) {
        abort();
    }
    return (const char *) buf;
}

SQLMATH_API void str99ArrayAppendDouble(
    sqlite3_str * str99,
    const double val
) {
// this function will append double <val> to <str99>
    sqlite3_str_append(str99, (const char *) &val, 8);
}

SQLMATH_API void str99ArrayAppendJsonarray(
    sqlite3_str * str99,
    const char *json,
    int nn
) {
// this function will append binary-Float64Array from json-encoded-flat-array
    // declare var
    int ii = 0;
    int jj = 0;
    // validate json
    for (; ii < nn; ii += 1) {
        if (json[ii] == '[') {
            break;
        }
    }
    for (; nn > ii; nn -= 1) {
        if (json[nn - 1] == ']') {
            break;
        }
    }
    if (nn <= ii) {
        goto catch_error_json;
    }
    jj = ii;
    // str99 - append double
    for (; ii < nn; ii += 1) {
        // skip whitespace
        switch (json[jj]) {
        case '[':
        case '\x09':
        case '\x0a':
        case '\x0d':
        case '\x20':
            jj = ii;
            break;
        default:
            switch (json[ii]) {
            case ',':
            case ']':
                str99ArrayAppendDouble(str99, atof(json + jj));
                jj = ii + 1;
                break;
            }
            break;
        }
        switch (json[jj]) {
        case ',':
            goto catch_error_json;
        case ']':
            if (sqlite3_str_length(str99) > 0) {
                goto catch_error_json;
            }
            break;
        }
    }
    return;
  catch_error_json:
    sqlite3_str_reset(str99);
    str99->accError = SQLITE_ERROR_JSON_ARRAY_INVALID;
}

SQLMATH_API void str99JsonAppendFloat64array(
    sqlite3_str * str99,
    const double *arr,
    int nn
) {
// this function will append json-encoded-flat-array from binary-Float64Array
    sqlite3_str_appendchar(str99, 1, '[');
    while (1) {
        nn -= 1;
        if (nn <= 0) {
            break;
        }
        // append with comma
        sqlite3_str_appendf(str99, isfinite(*arr) ? "%!.15g," : "null,",
            *arr);
        arr += 1;
    }
    if (nn == 0) {
        // append with no comma
        sqlite3_str_appendf(str99, isfinite(*arr) ? "%!.15g" : "null", *arr);
    }
    sqlite3_str_appendchar(str99, 1, ']');
}

SQLMATH_API void str99JsonAppendJenks(
    sqlite3_str * str99,
    int kk,
    const double *arr,
    int nn
) {
// this function will append json-encoded-flat-array from jenks-result
    // jenks - null-case
    if (kk <= 0 || nn <= 0) {
        sqlite3_str_appendchar(str99, 1, '[');
        sqlite3_str_appendchar(str99, 1, ']');
        return;
    }
    // jenks - classify
    double *result = jenksCreate(kk, arr, nn);
    if (result == NULL) {
        str99->accError = SQLITE_NOMEM;
        return;
    }
    // str99 - to-json
    str99JsonAppendFloat64array(str99, result, 1 + ((int) result[0]) * 2);
    sqlite3_free(result);
}

SQLMATH_API void str99ResultBlob(
    sqlite3_str * str99,
    sqlite3_context * context
) {
// this function will return <str99> as result-blob in given <context>
    sqlite3_result_blob(context, (const char *) sqlite3_str_value(str99),
        sqlite3_str_length(str99),
        // destructor
        sqlite3_free);
}

SQLMATH_API void str99ResultText(
    sqlite3_str * str99,
    sqlite3_context * context
) {
// this function will return <str99> as result-text in given <context>
    sqlite3_result_text(context, (const char *) sqlite3_str_value(str99),
        sqlite3_str_length(str99),
        // destructor
        sqlite3_free);
}


// file sqlmath_ext - SQLMATH_FUNC
// SQLMATH_FUNC sql_avg_ema_func - start
SQLMATH_FUNC static void sql_avg_ema_value(
    sqlite3_context * context
) {
// this function will aggregate exponential-moving-average
    // vec99 - init
    vector99 *vec99 =
        (vector99 *) sqlite3_aggregate_context(context, sizeof(*vec99));
    SQLITE3_RESULT_ERROR_MALLOC(vec99);
    double result = vec99->buf[0];
    sqlite3_result_double(context, result);
    return;
  catch_error:
    vector99_buf_free(vec99);
}

SQLMATH_FUNC static void sql_avg_ema_final(
    sqlite3_context * context
) {
// this function will aggregate exponential-moving-average
    // vec99 - value
    sql_avg_ema_value(context);
    // vec99 - cleanup
    vector99 *vec99 =
        (vector99 *) sqlite3_aggregate_context(context, sizeof(*vec99));
    SQLITE3_RESULT_ERROR_MALLOC(vec99);
  catch_error:
    vector99_buf_free(vec99);
}

SQLMATH_FUNC static void sql_avg_ema_inverse(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will aggregate exponential-moving-average
    UNUSED_PARAMETER(argc);
    UNUSED_PARAMETER(argv);
    vector99 *vec99 =
        (vector99 *) sqlite3_aggregate_context(context, sizeof(*vec99));
    SQLITE3_RESULT_ERROR_MALLOC(vec99);
    vector99_pop_front(vec99);
    return;
  catch_error:
    vector99_buf_free(vec99);
}

SQLMATH_FUNC static void sql_avg_ema_step(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will aggregate exponential-moving-average
    UNUSED_PARAMETER(argc);
    // declare var
    int errcode = 0;
    // vec99 - init
    vector99 *vec99 =
        (vector99 *) sqlite3_aggregate_context(context, sizeof(*vec99));
    SQLITE3_RESULT_ERROR_MALLOC(vec99);
    if (vec99->size <= 0) {
        errcode = vector99_buf_malloc(vec99);
        SQLITE3_RESULT_ERROR_CODE(errcode);
        vec99->arg0 = sqlite3_value_double(argv[1]);    // alpha
    }
    // vec99 - calculate ema
    const double alpha = vec99->arg0;
    const double elem = sqlite3_value_double(argv[0]);
    for (int ii = 0; ii < vec99->size; ii += 1) {
        vec99->buf[ii] = alpha * elem + (1 - alpha) * vec99->buf[ii];
    }
    // vec99 - push_back elem
    errcode = vector99_push_back(vec99, elem);
    SQLITE3_RESULT_ERROR_CODE(errcode);
    return;
  catch_error:
    vector99_buf_free(vec99);
}

// SQLMATH_FUNC sql_avg_ema_func - end

// SQLMATH_FUNC sql_btobase64_func - start
static char *base64Encode(
    const unsigned char *blob,
    int *nn
) {
// this function will base64-encode <blob> to <text>
    if (blob == NULL || *nn < 0) {
        *nn = 0;
    }
    // declare var
    char *text = NULL;
    int aa = *nn - 3;
    int bb = 0;
    int ii = 0;
    int triplet = 0;
    static const char BASE64_ENCODE_TABLE[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    // init bb
    bb = 4 * ceil((double) *nn / 3);
    // init text
    text = sqlite3_malloc(MAX(bb, 4));
    // handle nomem
    if (text == NULL) {
        return NULL;
    }
    // handle null-case
    if (bb < 4) {
        text[0] = '\x00';
    }
    // base64-encode loop
    while (ii < aa) {
        triplet = blob[0];
        triplet = triplet << 8 | blob[1];
        triplet = triplet << 8 | blob[2];
        blob += 3;
        text[0] = BASE64_ENCODE_TABLE[(triplet >> 18) & 0x3f];
        text[1] = BASE64_ENCODE_TABLE[(triplet >> 12) & 0x3f];
        text[2] = BASE64_ENCODE_TABLE[(triplet >> 6) & 0x3f];
        text[3] = BASE64_ENCODE_TABLE[triplet & 0x3f];
        text += 4;
        ii += 3;
    }
    // base64-encode '='
    if (bb >= 4 && blob != NULL) {
        aa += 3;
        triplet = blob[0];
        triplet = ii + 1 < aa ? triplet << 8 | blob[1] : triplet << 8;
        triplet = ii + 2 < aa ? triplet << 8 | blob[2] : triplet << 8;
        blob += 3;
        text[0] = BASE64_ENCODE_TABLE[(triplet >> 18) & 0x3f];
        text[1] = BASE64_ENCODE_TABLE[(triplet >> 12) & 0x3f];
        text[2] =
            (ii + 1 < aa) ? BASE64_ENCODE_TABLE[(triplet >> 6) & 0x3f] : '=';
        text[3] = (ii + 2 < aa) ? BASE64_ENCODE_TABLE[triplet & 0x3f] : '=';
        text += 4;
        ii += 3;
    }
    // save bb
    *nn = bb;
    // return text
    return text - bb;
}

SQLMATH_FUNC static void sql_btobase64_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will convert blob to base64-encoded-text
    UNUSED_PARAMETER(argc);
    // declare var
    char *text = NULL;
    int nn = sqlite3_value_bytes(argv[0]);
    // base64-encode blob to text
    text =
        base64Encode((const unsigned char *) sqlite3_value_blob(argv[0]),
        &nn);
    // handle nomem
    if (text == NULL) {
        sqlite3_result_error_nomem(context);
        return;
    }
    sqlite3_result_text(context, (const char *) text, nn,
        // cleanup base64Encode()
        sqlite3_free);
}

// SQLMATH_FUNC sql_btobase64_func - end

SQLMATH_FUNC static void sql_btotext_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will convert blob to text
    UNUSED_PARAMETER(argc);
    sqlite3_result_text(context, (const char *) sqlite3_value_text(argv[0]),
        -1, SQLITE_TRANSIENT);
}


SQLMATH_FUNC static void sql_castrealorzero_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will cast <argv>[0] to double or zero
    UNUSED_PARAMETER(argc);
    double num = sqlite3_value_double(argv[0]);
    sqlite3_result_double(context, isfinite(num) ? num : 0);
}

SQLMATH_FUNC static void sql_casttextorempty_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will cast <argv>[0] to text or empty-string
    UNUSED_PARAMETER(argc);
    switch (sqlite3_value_type(argv[0])) {
        // case SQLITE_BLOB:
        // case SQLITE_FLOAT:
        // case SQLITE_INTEGER:
    case SQLITE_NULL:
        sqlite3_result_text(context, "", 0, SQLITE_STATIC);
        return;
    case SQLITE_TEXT:
        sqlite3_result_value(context, argv[0]);
        return;
    }
    sqlite3_result_text(context, (const char *) sqlite3_value_text(argv[0]),
// ^If the 3rd parameter to the sqlite3_result_text* interfaces
// is negative, then SQLite takes result text from the 2nd parameter
// through the first zero character.
//
// ^If the 3rd parameter to the sqlite3_result_text* interfaces
// is non-negative, then as many bytes (not characters) of the text
// pointed to by the 2nd parameter are taken as the application-defined
// function result.
//
// If the 3rd parameter is non-negative, then it
// must be the byte offset into the string where the NUL terminator would
// appear if the string where NUL terminated.  If any NUL characters occur
// in the string at a byte offset that is less than the value of the 3rd
// parameter, then the resulting string will contain embedded NULs and the
// result of expressions operating on strings with embedded NULs is undefined.
        -1,
// If you insist on round-tripping through sqlite3_value_text,
// then you must pass SQLITE_TRANSIENT for the last parameter
// - the pointer returned by sqlite3_value_text is only guaranteed
// to be valid until the custom function returns.
        SQLITE_TRANSIENT);
}

SQLMATH_FUNC static void sql_copyblob_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will copy blob/text/value <argv>[0]
    UNUSED_PARAMETER(argc);
    sqlite3_result_value(context, argv[0]);
}

SQLMATH_FUNC static void sql_cot_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will return cot(argv[0])
    UNUSED_PARAMETER(argc);
    switch (sqlite3_value_numeric_type(argv[0])) {
    case SQLITE_INTEGER:
    case SQLITE_FLOAT:
        if (argv[0] == 0) {
            sqlite3_result_double(context, INFINITY);
            return;
        }
        sqlite3_result_double(context,
            1.0 / tan(sqlite3_value_double(argv[0])));
    }
}

SQLMATH_FUNC static void sql_coth_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will return coth(argv[0])
    UNUSED_PARAMETER(argc);
    switch (sqlite3_value_numeric_type(argv[0])) {
    case SQLITE_INTEGER:
    case SQLITE_FLOAT:
        if (argv[0] == 0) {
            sqlite3_result_double(context, INFINITY);
            return;
        }
        sqlite3_result_double(context,
            1.0 / tanh(sqlite3_value_double(argv[0])));
    }
}

SQLMATH_FUNC static void sql_jenks_blob_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will calculate <kk> jenks-natrual-breaks in given <values>,
// and return a mallocd (double *) array with length (1 + kk * 2) of form:
// [
// (double) kk,
// (double) break_1, (double) count_1,
// (double) break_2, (double) count_2,
// ...,
// (double) break_k, (double) count_k
// ]
    UNUSED_PARAMETER(argc);
    // declare var
    const double *arr = (double *) sqlite3_value_blob(argv[1]);
    const int kk = sqlite3_value_int(argv[0]);
    const int nn = sqlite3_value_bytes(argv[1]) / sizeof(double);
    // jenks - null-case
    if (kk <= 0 || nn <= 0) {
        sqlite3_result_null(context);
        return;
    }
    // jenks - classify
    double *result = jenksCreate(kk, arr, nn);
    if (result == NULL) {
        sqlite3_result_error_nomem(context);
        return;
    }
    // jenks - result
    sqlite3_result_blob(context, (void *) result,
        (1 + ((int) result[0]) * 2) * 8, sqlite3_free);
}

// SQLMATH_FUNC sql_jenks_concat_func - start
static void str99arrCleanup(
    const int argc,
    sqlite3_str ** str99arr
) {
    for (int ii = 0; ii < argc; ii += 1) {
        sqlite3_str *str99 = str99arr[ii];
        if (str99 != NULL) {
            if (ii > 0) {
                sqlite3_str_reset(str99);
            }
            sqlite3_free(str99);
            str99arr[ii] = NULL;
        }
    }
}

SQLMATH_FUNC static void sql_jenks_concat_final(
    sqlite3_context * context
) {
// this function will calculate <kk> jenks-natrual-breaks in each column <ii>,
// and return a json array
    // str99arr - init
    sqlite3_str **str99arr =
        (sqlite3_str **) sqlite3_aggregate_context(context, 0);
    if (str99arr == NULL) {
        sqlite3_result_null(context);
        return;
    }
    // declare var
    const int argc = ((int *) (str99arr[0]))[0];
    const int kk = ((int *) (str99arr[0]))[1];
    int errcode = 0;
    // str99arr - cleanup
    str99arrCleanup(1, str99arr);
    // str99json - init
    STR99_ALLOCA(str99json);
    sqlite3_str_appendchar(str99json, 1, '[');
    for (int ii = 1; ii < argc; ii += 1) {
        sqlite3_str *str99 = str99arr[ii];
        STR99_RESULT_ERROR(str99);
        // jenks - classify
        if (ii > 1) {
            sqlite3_str_appendchar(str99json, 1, ',');
        }
        str99JsonAppendJenks(   //
            str99json,          // json
            kk,                 // kk
            (double *) str99->zText,    // array
            sqlite3_str_length(str99) / 8);     // nn
        STR99_RESULT_ERROR(str99json);
    }
    sqlite3_str_appendchar(str99json, 1, ']');
    STR99_RESULT_ERROR(str99json);
    // str99json - result
    str99ResultText(str99json, context);
  catch_error:
    // str99arr - cleanup
    str99arrCleanup(argc, str99arr);
}

SQLMATH_FUNC static void sql_jenks_concat_step(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will calculate <kk> jenks-natrual-breaks in each column <ii>,
// and return a json array
    // init kk
    if (argc < 2) {
        return;
    }
    const int kk = sqlite3_value_int(argv[0]);
    if (kk <= 0) {
        return;
    }
    // str99arr - init
    sqlite3_str **str99arr =
        (sqlite3_str **) sqlite3_aggregate_context(context,
        argc * sizeof(sqlite3_str *));
    if (str99arr == NULL) {
        return;
    }
    if (str99arr[0] == NULL) {
        for (int ii = 0; ii < argc; ii += 1) {
            str99arr[ii] = sqlite3_malloc(sizeof(sqlite3_str));
            sqlite3_str *str99 = str99arr[ii];
            if (str99 == NULL) {
                // str99arr - cleanup
                str99arrCleanup(argc, str99arr);
                sqlite3_result_error_nomem(context);
                return;
            }
            memset(str99, 0, sizeof(sqlite3_str));
            str99->mxAlloc = SQLITE_MAX_LENGTH2;
        }
        ((int *) (str99arr[0]))[0] = argc;
        ((int *) (str99arr[0]))[1] = kk;
    }
    // str99 - append double
    for (int ii = 1; ii < argc; ii += 1) {
        sqlite3_str *str99 = str99arr[ii];
        if (sqlite3_value_numeric_type(argv[ii]) != SQLITE_NULL) {
            str99ArrayAppendDouble(str99, sqlite3_value_double(argv[ii]));
        }
    }
}

// SQLMATH_FUNC sql_jenks_concat_func - end

SQLMATH_FUNC static void sql_jenks_json_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// This function will calculate <kk> jenks-natrual-breaks in given <values>,
// and return a json array with length (1 + kk * 2) of form:
// [
// (double) kk,
// (double) break_1, (double) count_1,
// (double) break_2, (double) count_2,
// ...,
// (double) break_k, (double) count_k
// ]
    UNUSED_PARAMETER(argc);
    // declare var
    int errcode = 0;
    // str99 - to-array
    STR99_ALLOCA(arr);
    str99ArrayAppendJsonarray(  //
        arr,                    // array
        (char *) sqlite3_value_blob(argv[1]),   // json
        sqlite3_value_bytes(argv[1]));  // nn
    STR99_RESULT_ERROR(arr);
    // jenks - classify
    STR99_ALLOCA(str99);
    str99JsonAppendJenks(       //
        str99,                  // json
        sqlite3_value_int(argv[0]),     // kk
        (double *) arr->zText,  // array
        sqlite3_str_length(arr) / 8);   // nn
    sqlite3_str_reset(arr);
    STR99_RESULT_ERROR(str99);
    // str99 - result
    str99ResultText(str99, context);
  catch_error:
    (void) 0;
}

SQLMATH_FUNC static void sql_jsonfromfloat64array_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will create json-encoded-flat-array from binary-Float64Array
    UNUSED_PARAMETER(argc);
    // declare var
    int errcode = 0;
    // str99 - init
    STR99_ALLOCA(str99);
    // str99 - jsonfrom
    str99JsonAppendFloat64array(        //
        str99,                  //
        (double *) sqlite3_value_blob(argv[0]), //
        sqlite3_value_bytes(argv[0]) / 8);
    STR99_RESULT_ERROR(str99);
    // str99 - result
    str99ResultText(str99, context);
  catch_error:
    (void) 0;
}

SQLMATH_FUNC static void sql_jsontofloat64array_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will create binary-Float64Array from json-encoded-flat-array
    UNUSED_PARAMETER(argc);
    // declare var
    int errcode = 0;
    // str99 - to-array
    STR99_ALLOCA(str99);
    str99ArrayAppendJsonarray(  //
        str99,                  // array
        (char *) sqlite3_value_blob(argv[0]),   // json
        sqlite3_value_bytes(argv[0]));  // nn
    STR99_RESULT_ERROR(str99);
    // str99 - result
    str99ResultBlob(str99, context);
  catch_error:
    (void) 0;
}

// SQLMATH_FUNC sql_marginoferror95_func - start
SQLMATH_API double marginoferror95(
    double nn,
    double pp
) {
// this function will calculate margin-of-error sqrt(pp*(1-pp)/nn)
    return 1.9599639845400542 * sqrt(pp * (1 - pp) / nn);
}

SQLMATH_FUNC static void sql_marginoferror95_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will calculate margin-of-error sqrt(pp*(1-pp)/nn)
    UNUSED_PARAMETER(argc);
    // declare var
    double nn = sqlite3_value_double(argv[0]);
    double pp = sqlite3_value_double(argv[1]);
    if (sqlite3_value_type(argv[0]) == SQLITE_NULL
        || sqlite3_value_type(argv[1]) == SQLITE_NULL || nn == 0) {
        sqlite3_result_null(context);
        return;
    }
    sqlite3_result_double(context, marginoferror95(nn, pp));
}

// SQLMATH_FUNC sql_marginoferror95_func - end

// SQLMATH_FUNC sql_matrix2d_concat_func - start
SQLMATH_FUNC static void sql_matrix2d_concat_final(
    sqlite3_context * context
) {
// this function will concat rows of nCol doubles to a 2d-matrix
    // declare var
    int errcode = 0;
    // str99 - init
    sqlite3_str *str99 =
        (sqlite3_str *) sqlite3_aggregate_context(context, 0);
    if (str99 == NULL) {
        sqlite3_result_null(context);
        return;
    }
    STR99_RESULT_ERROR(str99);
    // str99 - null-case
    if (sqlite3_str_length(str99) <= 2 * 8) {
        sqlite3_str_reset(str99);
        sqlite3_result_null(context);
        return;
    }
    double *arr = (double *) str99->zText;
    arr[0] = (0.125 * ((double) sqlite3_str_length(str99)) - 2) / arr[1];
    // str99 - result
    str99ResultBlob(str99, context);
  catch_error:
    (void) 0;
}

SQLMATH_FUNC static void sql_matrix2d_concat_step(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will concat rows of nCol doubles to a 2d-matrix
    // str99 - init
    sqlite3_str *str99 =
        (sqlite3_str *) sqlite3_aggregate_context(context, sizeof(*str99));
    if (str99 == NULL) {
        return;
    }
    // str99 - init zText
    if (sqlite3_str_length(str99) == 0) {
        str99->mxAlloc = SQLITE_MAX_LENGTH2;
        str99ArrayAppendDouble(str99, 0);
        str99ArrayAppendDouble(str99, (double) argc);
    }
    // str99 - append double
    for (int ii = 0; ii < argc; ii += 1) {
        str99ArrayAppendDouble(str99, sqlite3_value_double(argv[ii]));
    }
}

// SQLMATH_FUNC sql_matrix2d_concat_func - end

// SQLMATH_FUNC sql_quantile_func - start
static double quickselect(
    double *arr,
    const int nn,
    const int kk
) {
// this function will find <kk>-th element in <arr> using quickselect-algorithm
// https://www.stat.cmu.edu/~ryantibs/median/quickselect.c
    if (nn <= 0) {
        return 0;
    }
    double aa = *arr;
    double tmp = 0;
    int ii;
    int ir;
    int jj;
    int ll;
    int mid;
    ll = 0;
    ir = nn - 1;
    while (1) {
        if (ir <= ll + 1) {
            if (ir == ll + 1 && arr[ir] < arr[ll]) {
                SWAP(arr[ll], arr[ir]);
            }
            return arr[kk];
        } else {
            mid = (ll + ir) >> 1;
            SWAP(arr[mid], arr[ll + 1]);
            if (arr[ll] > arr[ir]) {
                SWAP(arr[ll], arr[ir]);
            }
            if (arr[ll + 1] > arr[ir]) {
                SWAP(arr[ll + 1], arr[ir]);
            }
            if (arr[ll] > arr[ll + 1]) {
                SWAP(arr[ll], arr[ll + 1]);
            }
            ii = ll + 1;
            jj = ir;
            aa = arr[ll + 1];
            while (1) {
                while (1) {
                    ii += 1;
                    if (arr[ii] >= aa) {
                        break;
                    }
                }
                while (1) {
                    jj -= 1;
                    if (arr[jj] <= aa) {
                        break;
                    }
                }
                if (jj < ii) {
                    break;
                }
                SWAP(arr[ii], arr[jj]);
            }
            arr[ll + 1] = arr[jj];
            arr[jj] = aa;
            if (jj >= kk) {
                ir = jj - 1;
            }
            if (jj <= kk) {
                ll = ii;
            }
        }
    }
}

SQLMATH_API double quantile(
    double *arr,
    const int nn,
    const double pp
) {
// this function will find <pp>-th-quantile element in <arr>
// using quickselect-algorithm
// https://www.stat.cmu.edu/~ryantibs/median/quickselect.c
    if (nn <= 0) {
        return 0;
    }
    const int kk = MAX(0, MIN(nn - 1, (const int) (pp * nn)));
    // handle even-case
    if ((0 < kk && kk + 1 <= nn) && (double) kk == (pp * nn)) {
        return 0.5 * (quickselect(arr, nn, kk) + quickselect(arr, nn,
                kk - 1));
    }
    // handle odd-case
    return quickselect(arr, nn, kk);
}

SQLMATH_FUNC static void sql_quantile_final(
    sqlite3_context * context
) {
// this function will aggregate kth-quantile element
    vector99 *vec99 =
        (vector99 *) sqlite3_aggregate_context(context, sizeof(*vec99));
    SQLITE3_RESULT_ERROR_MALLOC(vec99);
    // vec99 - null-case
    if (vec99->size <= 0) {
        sqlite3_result_null(context);
        goto catch_error;
    }
    sqlite3_result_double(context, quantile(vec99->buf, vec99->size,
            vec99->arg0));
  catch_error:
    vector99_buf_free(vec99);
}

SQLMATH_FUNC static void sql_quantile_step(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will aggregate kth-quantile element
    UNUSED_PARAMETER(argc);
    // declare var
    int errcode = 0;
    // vec99 - init
    vector99 *vec99 =
        (vector99 *) sqlite3_aggregate_context(context, sizeof(*vec99));
    SQLITE3_RESULT_ERROR_MALLOC(vec99);
    if (vec99->size <= 0) {
        errcode = vector99_buf_malloc(vec99);
        SQLITE3_RESULT_ERROR_CODE(errcode);
        vec99->arg0 = sqlite3_value_double(argv[1]);    // kth-quantile
    }
    // vec99 - append isfinite
    const double elem = sqlite3_value_double(argv[0]);
    if (sqlite3_value_type(argv[0]) != SQLITE_NULL && isfinite(elem)) {
        errcode = vector99_push_back(vec99, elem);
        SQLITE3_RESULT_ERROR_CODE(errcode);
    }
    return;
  catch_error:
    vector99_buf_free(vec99);
}

// SQLMATH_FUNC sql_quantile_func - end


SQLMATH_FUNC static void sql_roundorzero_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will round <argv>[0] to decimal <argv>[1]
    UNUSED_PARAMETER(argc);
    if (sqlite3_value_type(argv[0]) == SQLITE_NULL) {
        sqlite3_result_double(context, 0);
        return;
    }
    // declare var
    char *zBuf = NULL;
    double rr = sqlite3_value_double(argv[0]);
    int nn = sqlite3_value_int(argv[1]);
    nn = MIN(nn, 30);
    nn = MAX(nn, 0);
    // If YY==0 and XX will fit in a 64-bit int,
    // handle the rounding directly,
    // otherwise use printf.
    if (rr < -4503599627370496.0 || rr > +4503599627370496.0) {
    } else if (nn == 0) {
        // The value has no fractional part so there is nothing to round
        rr = (double) ((sqlite_int64) (rr + (rr < 0 ? -0.5 : +0.5)));
    } else {
        zBuf = sqlite3_mprintf("%.*f", nn, rr);
        if (zBuf == NULL) {
            sqlite3_result_error_nomem(context);
            return;
        }
        rr = strtod(zBuf, NULL);
        // cleanup sqlite3_mprintf()
        sqlite3_free(zBuf);
    }
    sqlite3_result_double(context, rr);
}

SQLMATH_FUNC static void sql_random1_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will generate high-quality random-float between 0 <= xx < 1
    UNUSED_PARAMETER(argc);
    UNUSED_PARAMETER(argv);
    static const double inv = 1.0 / 0x100000000;
    uint32_t xx[1];
    sqlite3_randomness(4, (void *) xx);
    sqlite3_result_double(context, ((double) *xx) * inv);
}

SQLMATH_FUNC static void sql_sign_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
/*
** Implementation of the sign() function
** return one of 3 possibilities +1,0 or -1 when the argument is respectively
** positive, 0 or negative.
** When the argument is NULL the result is also NULL (completly conventional)
*/
    UNUSED_PARAMETER(argc);
    switch (sqlite3_value_numeric_type(argv[0])) {
    case SQLITE_INTEGER:
    case SQLITE_FLOAT:
        {
            double rVal = sqlite3_value_double(argv[0]);
            sqlite3_result_double(context,
                (rVal > 0) ? 1 : (rVal < 0) ? -1 : 0);
        }
        return;
    }
}

SQLMATH_FUNC static void sql_squared_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
/*
** Implementation of the squared() function
** the argument is an integer.
** Since SQLite isn't strongly typed (almost untyped actually) this is a bit pedantic
*/
    UNUSED_PARAMETER(argc);
    switch (sqlite3_value_numeric_type(argv[0])) {
    case SQLITE_INTEGER:
    case SQLITE_FLOAT:
        {
            double rVal = sqlite3_value_double(argv[0]);
            sqlite3_result_double(context, rVal * rVal);
        }
        return;
    }
}

SQLMATH_FUNC static void sql_throwerror_func(
    sqlite3_context * context,
    int argc,
    sqlite3_value ** argv
) {
// this function will return sqlite3_result_error_code(argv[0])
    UNUSED_PARAMETER(argc);
    // declare var
    int errcode = sqlite3_value_int(argv[0]);
    if (0 <= errcode && errcode <= 28) {
        sqlite3_result_error_code(context, errcode);
        return;
    }
    sqlite3_result_error_code(context, SQLITE_INTERNAL);
}


// file sqlmath_ext - init
int sqlite3_sqlmath_ext_base_init(
    sqlite3 * db,
    char **pzErrMsg,
    const sqlite3_api_routines * pApi
) {
    UNUSED_PARAMETER(pzErrMsg);
    // declare var
    int errcode = 0;
    // init sqlite3_api
    sqlite3_api = pApi;
    SQLITE3_CREATE_FUNCTION1(btobase64, 1);
    SQLITE3_CREATE_FUNCTION1(btotext, 1);
    SQLITE3_CREATE_FUNCTION1(castrealorzero, 1);
    SQLITE3_CREATE_FUNCTION1(casttextorempty, 1);
    SQLITE3_CREATE_FUNCTION1(copyblob, 1);
    SQLITE3_CREATE_FUNCTION1(cot, 1);
    SQLITE3_CREATE_FUNCTION1(coth, 1);
    SQLITE3_CREATE_FUNCTION1(jenks_blob, 2);
    SQLITE3_CREATE_FUNCTION1(jenks_json, 2);
    SQLITE3_CREATE_FUNCTION1(jsonfromfloat64array, 1);
    SQLITE3_CREATE_FUNCTION1(jsontofloat64array, 1);
    SQLITE3_CREATE_FUNCTION1(marginoferror95, 2);
    SQLITE3_CREATE_FUNCTION1(roundorzero, 2);
    SQLITE3_CREATE_FUNCTION1(sign, 1);
    SQLITE3_CREATE_FUNCTION1(squared, 1);
    SQLITE3_CREATE_FUNCTION1(throwerror, 1);
    SQLITE3_CREATE_FUNCTION2(jenks_concat, -1);
    SQLITE3_CREATE_FUNCTION2(matrix2d_concat, -1);
    SQLITE3_CREATE_FUNCTION2(quantile, 2);
    SQLITE3_CREATE_FUNCTION3(avg_ema, 2);
    //
    errcode = sqlite3_compress_init(db, pzErrMsg, pApi);
    if (errcode != SQLITE_OK) {
        return errcode;
    }
    errcode = sqlite3_noop_init(db, pzErrMsg, pApi);
    if (errcode != SQLITE_OK) {
        return errcode;
    }
    errcode = sqlite3_series_init(db, pzErrMsg, pApi);
    if (errcode != SQLITE_OK) {
        return errcode;
    }
    errcode = sqlite3_regexp_init(db, pzErrMsg, pApi);
    if (errcode != SQLITE_OK) {
        return errcode;
    }
    errcode = sqlite3_shathree_init(db, pzErrMsg, pApi);
    if (errcode != SQLITE_OK) {
        return errcode;
    }
    //!! errcode = sqlite3_extension_init(db, pzErrMsg, pApi);
    //!! if (errcode != SQLITE_OK) { return errcode; }
    //
    errcode =
        sqlite3_create_function(db, "random1", 0,
        SQLITE_DIRECTONLY | SQLITE_UTF8, NULL, sql_random1_func, NULL, NULL);
    if (errcode != SQLITE_OK) {
        return errcode;
    }
    return 0;
}
#endif                          // SQLITE3_EXT_C2
/*
file sqlmath_ext - end
*/


/*
file sqlmath_napi - start
*/
#ifdef SQLMATH_NAPI


#ifdef WIN32
#include <windows.h>
#endif
#include <node_api.h>


// file sqlmath_napi - assert
static int napiAssertOk(
    napi_env env,
    const char *func,
    const char *file,
    int line,
    int errcode
) {
// this function will throw error if <errcode> != napi_ok
// derived from https://github.com/nodejs/node-addon-api/blob/3.2.1/napi-inl.h
// typedef struct {
//   const char* error_message;
//   void* engine_reserved;
//   uint32_t engine_error_code;
//   napi_status error_code;
// } napi_extended_error_info;
    if (errcode == napi_ok) {
        return errcode;
    }
    // declare var
    char buf[SIZEOF_MESSAGE_DEFAULT] = { 0 };
    bool is_exception_pending;
    const napi_extended_error_info *info;
    napi_value val = NULL;
    // We must retrieve the last error info before doing anything else, because
    // doing anything else will replace the last error info.
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 26812)
#endif
    errcode = napi_get_last_error_info(env, &info);
#ifdef _MSC_VER
#pragma warning(pop)
#endif
    NAPI_ASSERT_FATAL(errcode == 0, "napi_get_last_error_info");
    errcode = napi_is_exception_pending(env, &is_exception_pending);
    NAPI_ASSERT_FATAL(errcode == 0, "napi_is_exception_pending");
    // A pending exception takes precedence over any internal error status.
    if (is_exception_pending) {
        errcode = napi_get_and_clear_last_exception(env, &val);
        NAPI_ASSERT_FATAL(errcode == 0, "napi_get_and_clear_last_exception");
        napi_throw(env, val);
        return errcode;
    }
    errcode =
        napi_throw_error(env, NULL, sqlmathSnprintfTrace(buf, "",
            (info->error_message !=
                NULL ? info->error_message : "error in native code"), func,
            file, line));
    NAPI_ASSERT_FATAL(errcode == 0, "napi_throw_error");
    return errcode;
}


// file sqlmath_napi - promise
static void jsbatonBufferFinalize(
    napi_env env,
    void *finalize_data,
    void *finalize_hint
) {
// this function will finalize <finalize_data>
    UNUSED_PARAMETER(env);
    UNUSED_PARAMETER(finalize_hint);
    // cleanup baton->bufv[ii]
    sqlite3ApiGet()->free(finalize_data);
}

static Jsbaton *jsbatonCreate(
    napi_env env,
    napi_callback_info info
) {
// this function will create a baton for passing data between nodejs <-> c
    // declare var
    Jsbaton *baton = NULL;
    bool is_dataview;
    int errcode = 0;
    napi_value argv;
    size_t ii = 0;
    // init argv
    ii = 1;
    errcode = napi_get_cb_info(env, info, &ii, &argv, NULL, NULL);
    NAPI_ASSERT_OK();
    // init baton
    errcode = napi_get_element(env, argv, 0, (napi_value *) & baton);
    NAPI_ASSERT_OK();
    errcode =
        napi_get_dataview_info(env, (napi_value) baton, NULL,
        (void **) &baton, NULL, NULL);
    NAPI_ASSERT_OK();
    // save argv
    baton->napi_argv = argv;
    // init argv - external dataview
    for (ii = 0; ii < JSBATON_ARGC; ii += 1) {
        errcode = napi_get_element(env, baton->napi_argv, ii + 2, &argv);
        NAPI_ASSERT_OK();
        errcode = napi_is_dataview(env, argv, &is_dataview);
        NAPI_ASSERT_OK();
        if (is_dataview) {
            errcode =
                napi_get_dataview_info(env, argv, NULL,
                (void **) baton->argv + ii, NULL, NULL);
            NAPI_ASSERT_OK();
        }
    }
    // fprintf(stderr, "\nsqlmath.jsbatonCreate(cfuncname=%s)\n",
    //     jsbatonValueStringArgi(baton, 2 * JSBATON_ARGC));
    return baton;
}

static napi_value jsbatonExport(
    napi_env env,
    Jsbaton * baton
) {
// this function will export c-data to js-data in <jsbaton>
    // declare var
    int errcode = 0;
    napi_value val;
    size_t ii = 0;
    // export baton->argv and baton->bufv to baton->napi_argv
    while (ii < JSBATON_ARGC) {
        if (baton->bufv[ii] == 0) {
            // init argList[ii] = argv[ii]
            errcode = napi_create_bigint_int64(env, baton->argv[ii], &val);
            NAPI_ASSERT_OK();
        } else {
            // init argList[ii] = bufv[ii]
            errcode = napi_create_external_arraybuffer(env,     // napi_env env,
                (void *) baton->bufv[ii],       // void* external_data,
                (size_t) baton->argv[ii],       // size_t byte_length,
                jsbatonBufferFinalize,  // napi_finalize finalize_cb,
                NULL,           // void* finalize_hint,
                &val);          // napi_value* result
            NAPI_ASSERT_OK();
        }
        errcode = napi_set_element(env, baton->napi_argv, ii + 2, val);
        NAPI_ASSERT_OK();
        ii += 1;
    }
    return baton->napi_argv;
}

static napi_value jsstringCreate(
    napi_env env,
    const char *ss
) {
// This API creates a JavaScript string value from a UTF8-encoded C string.
// The native string is copied.
    // declare var
    int errcode = 0;
    napi_value result = NULL;
    // return result
    errcode = napi_create_string_utf8(env, ss, NAPI_AUTO_LENGTH, &result);
    NAPI_ASSERT_OK();
    return result;
}

static void jspromiseResolve(
    napi_env env,
    napi_status errcode,
    void *data
) {
// This function runs on the main thread after `jspromiseExecute` exits.
    NAPI_ASSERT_FATAL(errcode == 0, __func__);
    // init baton
    Jsbaton *baton = (Jsbaton *) data;
    // declare var
    napi_ref ref = (napi_ref) baton->napi_argv;
    uint32_t refcount = 1;
    // dereference result to allow gc
    errcode = napi_reference_unref(env, ref, &refcount);
    NAPI_ASSERT_FATAL(errcode == 0, "napi_reference_unref");
    NAPI_ASSERT_FATAL(refcount == 0, "memory leak");
    errcode =
        napi_get_reference_value(env, ref, (napi_value *) & baton->napi_argv);
    NAPI_ASSERT_FATAL(errcode == 0, "napi_get_reference_value");
    errcode = napi_delete_reference(env, ref);
    NAPI_ASSERT_FATAL(errcode == 0, "napi_delete_reference");
    // Resolve or reject the promise associated with the deferred depending on
    // whether the asynchronous action succeeded.
    if (baton->errmsg[0] == '\x00') {
        // resolve promise with result
        if (jsbatonExport(env, baton) == NULL) {
            return;
        }
        errcode =
            napi_resolve_deferred(env, baton->napi_deferred,
            baton->napi_argv);
        NAPI_ASSERT_FATAL(errcode == 0, "napi_resolve_deferred");
    } else {
        // declare var
        napi_value err;
        // create error
        errcode =
            napi_create_error(env, NULL, jsstringCreate(env, baton->errmsg),
            &err);
        NAPI_ASSERT_FATAL(errcode == 0, "napi_create_error");
        // reject promise with error
        errcode = napi_reject_deferred(env, baton->napi_deferred, err);
        NAPI_ASSERT_FATAL(errcode == 0, "napi_reject_deferred");
    }
    // Clean up the work item associated with this run.
    errcode = napi_delete_async_work(env, baton->napi_work);
    NAPI_ASSERT_FATAL(errcode == 0, "napi_delete_async_work");
    // Set both values to NULL so JavaScript can order a new run of the thread.
    baton->napi_work = NULL;
    baton->napi_deferred = NULL;
}

static napi_value jspromiseCreate(
    napi_env env,
    napi_callback_info info,
    napi_async_execute_callback jspromiseExecute
) {
// Create a deferred promise and an async queue work item.
    // init baton
    Jsbaton *baton = jsbatonCreate(env, info);
    if (baton == NULL) {
        return NULL;
    }
    // declare var
    char buf[SIZEOF_MESSAGE_DEFAULT] = { 0 };
    int errcode = 0;
    napi_value promise = 0;
    napi_value async_resource_name;
    // reference result to prevent gc
    errcode = napi_create_reference(env,        // napi_env env
        baton->napi_argv,       // napi_value value
        1,                      // uint32_t initial_refcount
        (napi_ref *) & baton->napi_argv);       // napi_ref* result
    NAPI_ASSERT_OK();
    // Ensure that no work is currently in progress.
    if (baton->napi_work != NULL) {
        napi_throw_error(env, NULL, sqlmathSnprintfTrace(buf, "",
                "sqlmath.jspromiseCreate()"
                " - Only one work item must exist at a time", __func__,
                __FILE__, __LINE__));
        return NULL;
    }
    // Create a deferred promise which we will resolve at the completion of the
    // work.
    errcode =
        napi_create_promise(env, (napi_deferred *) & (baton->napi_deferred),
        &promise);
    NAPI_ASSERT_OK();
    // init async_resource_name
    errcode =
        napi_create_string_utf8(env,
        "sqlmath.jspromiseCreate() - napi_create_async_work()",
        NAPI_AUTO_LENGTH, &async_resource_name);
    // Create an async work item, passing in the addon data, which will give the
    // worker thread access to the above-created deferred promise.
    errcode = napi_create_async_work(env,       // napi_env env,
        NULL,                   // napi_value async_resource,
        async_resource_name,    // napi_value async_resource_name,
        jspromiseExecute,       // napi_async_execute_callback execute,
        jspromiseResolve,       // napi_async_complete_callback complete,
        baton,                  // void* data,
        // napi_async_work* result);
        (napi_async_work *) & (baton->napi_work));
    NAPI_ASSERT_OK();
    // Queue the work item for execution.
    errcode = napi_queue_async_work(env, baton->napi_work);
    NAPI_ASSERT_OK();
    // This causes created `promise` to be returned to JavaScript.
    return promise;
}


// file sqlmath_napi - init
NAPI_JSPROMISE_CREATE(dbClose);
NAPI_JSPROMISE_CREATE(dbExec);
NAPI_JSPROMISE_CREATE(dbFileImportOrExport);
NAPI_JSPROMISE_CREATE(dbNoop);
NAPI_JSPROMISE_CREATE(dbOpen);

napi_value napi_module_sqlmath_init(
    napi_env env,
    napi_value exports
) {
// typedef struct {
//   // One of utf8name or name should be NULL.
//   const char* utf8name;
//   napi_value name;
//
//   napi_callback method;
//   napi_callback getter;
//   napi_callback setter;
//   napi_value value;
//
//   napi_property_attributes attributes;
//   void* data;
// } napi_property_descriptor;
    // coverage-hack
    noop();
    // declare var
    int errcode = 0;
    const napi_property_descriptor propList[] = {
        NAPI_EXPORT_MEMBER(_dbClose),
        NAPI_EXPORT_MEMBER(_dbExec),
        NAPI_EXPORT_MEMBER(_dbFileImportOrExport),
        NAPI_EXPORT_MEMBER(_dbNoop),
        NAPI_EXPORT_MEMBER(_dbOpen),
    };
    errcode = napi_define_properties(env, exports,
        sizeof(propList) / sizeof(napi_property_descriptor), propList);
    NAPI_ASSERT_OK();
    // init sqlite
    errcode = sqlite3_initialize();
    NAPI_ASSERT_OK();
    return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, napi_module_sqlmath_init)
#endif                          // SQLMATH_NAPI
/*
file sqlmath_napi - end
*/
