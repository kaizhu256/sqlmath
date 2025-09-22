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


#if !defined(SRC_SQLMATH_BASE_H3)
#define SRC_SQLMATH_BASE_H3


#if defined(SRC_SQLMATH_BASE_C2)


#define LIGHTGBM_C_EXPORT typedef
#ifdef _WIN32
#define LGBM_DLSYM(func) \
    func = (func##_t) GetProcAddress((HMODULE) lgbm_library, #func);
#else
#define LGBM_DLSYM(func) \
    func = (func##_t) dlsym(lgbm_library, #func);
#endif                          // _WIN32
// https://github.com/microsoft/LightGBM/blob/v4.6.0/include/LightGBM/arrow.h
typedef struct ArrowSchema {
    // Array type description
    const char *format;
    const char *name;
    const char *metadata;
    int64_t flags;
    int64_t n_children;
    struct ArrowSchema **children;
    struct ArrowSchema *dictionary;
    // Release callback
    void (
        *release
    ) (
        struct ArrowSchema *
    );
    // Opaque producer-specific data
    void *private_data;
} ArrowSchema;
typedef struct ArrowArray {
    // Array data description
    int64_t length;
    int64_t null_count;
    int64_t offset;
    int64_t n_buffers;
    int64_t n_children;
    const void **buffers;
    struct ArrowArray **children;
    struct ArrowArray *dictionary;
    // Release callback
    void (
        *release
    ) (
        struct ArrowArray *
    );
    // Opaque producer-specific data
    void *private_data;
} ArrowArray;
static void *lgbm_library = NULL;


#endif                          // SRC_SQLMATH_BASE_C2


#define MINIZ_NO_ZLIB_COMPATIBLE_NAMES
#define MINIZ_EXPORT


// *INDENT-OFF*
/*jslint-disable*/
/*
shRollupFetch
{
    "fetchList": [
        {
            "url": "https://github.com/richgel999/miniz/blob/3.1.0/miniz.h"
        },
        {
            "url": "https://github.com/richgel999/miniz/blob/3.1.0/miniz_common.h"
        },
        {
            "url": "https://github.com/richgel999/miniz/blob/3.1.0/miniz_tdef.h"
        },
        {
            "url": "https://github.com/richgel999/miniz/blob/3.1.0/miniz_tinfl.h"
        },
        {
            "url": "https://github.com/richgel999/miniz/blob/3.1.0/miniz_zip.h"
        },
        {
            "header": "\n#if defined(SRC_SQLMATH_BASE_C2)\n",
            "url": "https://github.com/richgel999/miniz/blob/3.1.0/miniz.c"
        },
        {
            "url": "https://github.com/richgel999/miniz/blob/3.1.0/miniz_tdef.c"
        },
        {
            "footer": "\n#endif // SRC_SQLMATH_BASE_C2\n",
            "url": "https://github.com/richgel999/miniz/blob/3.1.0/miniz_tinfl.c"
        },
        {
            "url": "https://github.com/intel/tinycrypt/blob/v0.2.8/lib/include/tinycrypt/sha256.h"
        },
        {
            "header": "\n#if defined(SRC_SQLMATH_BASE_C2)\n",
            "url": "https://github.com/intel/tinycrypt/blob/v0.2.8/lib/include/tinycrypt/constants.h"
        },
        {
            "url": "https://github.com/intel/tinycrypt/blob/v0.2.8/lib/include/tinycrypt/utils.h"
        },
        {
            "url": "https://github.com/intel/tinycrypt/blob/v0.2.8/lib/source/utils.c"
        },
        {
            "footer": "\n#endif // SRC_SQLMATH_BASE_C2\n",
            "replaceList": [
                {
                    "aa": "\\bcompress\\b",
                    "bb": "tc_sha256_compress",
                    "flags": "g",
                    "substr": ""
                }
            ],
            "url": "https://github.com/intel/tinycrypt/blob/v0.2.8/lib/source/sha256.c"
        },
        {
            "header": "\n#if defined(SRC_SQLMATH_BASE_C2)\n",
            "replaceList": [
                {
                    "aa": "\nLIGHTGBM_C_EXPORT ([^\\(]*?) (\\w*?)(\\([\\S\\s]*?\\));\n",
                    "bb": "\nLIGHTGBM_C_EXPORT $1 (*$2_t) $3;\nstatic $2_t $2 = NULL;\n",
                    "flags": "g",
                    "substr": ""
                }
            ],
            "url": "https://github.com/microsoft/LightGBM/blob/v4.6.0/include/LightGBM/c_api.h"
        },
        {
            "footer": "\n}\n#endif // SRC_SQLMATH_BASE_C2\n",
            "header": "static void LGBM_dlsym() {\n",
            "replaceList": [
                {
                    "aa": "\nLIGHTGBM_C_EXPORT ([^\\(]*?) (\\w*?)(\\([\\S\\s]*?\\));\n",
                    "bb": "\n^LGBM_DLSYM($2);\n",
                    "flags": "g",
                    "substr": ""
                },
                {
                    "aa": "^[^^].*?\n|^\n",
                    "bb": "",
                    "flags": "gm",
                    "substr": ""
                },
                {
                    "aa": "^[^z]",
                    "bb": "",
                    "flags": "gm",
                    "substr": ""
                }
            ],
            "url": "https://github.com/microsoft/LightGBM/blob/v4.6.0/include/LightGBM/c_api.h"
        },
        {
            "comment": true,
            "header": "\n#if defined(SRC_SQLMATH_NODEJS_C2)\n",
            "url": "https://github.com/nodejs/node/blob/v10.22.1/LICENSE"
        },
        {
            "url": "https://github.com/nodejs/node/blob/v10.22.1/src/node_api_types.h"
        },
        {
            "footer": "\n#endif // SRC_SQLMATH_NODEJS_C2\n",
            "replaceList": [
                {
                    "aa": "^#include \"node_api_types.h\"",
                    "bb": "// hack-nodejs - inline header\n// $&",
                    "flags": "gm",
                    "substr": ""
                }
            ],
            "url": "https://github.com/nodejs/node/blob/v10.22.1/src/node_api.h"
        }
    ],
    "replaceList": [
        {
            "aa": "^#include \"(?:miniz|miniz_common|miniz_export|miniz_tdef|miniz_tinfl|miniz_zip)\\.h\"",
            "bb": "// hack-miniz - inline header\n// $&",
            "flags": "gm",
            "substr": ""
        },
        {
            "aa": "^#include .\\b(?:LightGBM|tinycrypt|node_api_types)\\b",
            "bb": "// hack-nodejs - inline header\n// $&",
            "flags": "gm",
            "substr": ""
        }
    ]
}
-#define INLINE_FUNCTION inline
+// hack-lightgbm - fix warning
+#define INLINE_FUNCTION static inline

-void _set(void *to, uint8_t val, unsigned int len)
+void _set(void *to, uint8_t val, size_t len)

-void _set(void *to, uint8_t val, unsigned int len);
+void _set(void *to, uint8_t val, size_t len);
*/


/*
repo https://github.com/richgel999/miniz/tree/3.1.0
committed 2025-09-21T15:48:16Z
*/


/*
file https://github.com/richgel999/miniz/blob/3.1.0/miniz.h
*/
/* miniz.c 3.1.0 - public domain deflate/inflate, zlib-subset, ZIP reading/writing/appending, PNG writing
   See "unlicense" statement at the end of this file.
   Rich Geldreich <richgel99@gmail.com>, last updated Oct. 13, 2013
   Implements RFC 1950: http://www.ietf.org/rfc/rfc1950.txt and RFC 1951: http://www.ietf.org/rfc/rfc1951.txt

   Most API's defined in miniz.c are optional. For example, to disable the archive related functions just define
   MINIZ_NO_ARCHIVE_APIS, or to get rid of all stdio usage define MINIZ_NO_STDIO (see the list below for more macros).

   * Low-level Deflate/Inflate implementation notes:

     Compression: Use the "tdefl" API's. The compressor supports raw, static, and dynamic blocks, lazy or
     greedy parsing, match length filtering, RLE-only, and Huffman-only streams. It performs and compresses
     approximately as well as zlib.

     Decompression: Use the "tinfl" API's. The entire decompressor is implemented as a single function
     coroutine: see tinfl_decompress(). It supports decompression into a 32KB (or larger power of 2) wrapping buffer, or into a memory
     block large enough to hold the entire file.

     The low-level tdefl/tinfl API's do not make any use of dynamic memory allocation.

   * zlib-style API notes:

     miniz.c implements a fairly large subset of zlib. There's enough functionality present for it to be a drop-in
     zlib replacement in many apps:
        The z_stream struct, optional memory allocation callbacks
        deflateInit/deflateInit2/deflate/deflateReset/deflateEnd/deflateBound
        inflateInit/inflateInit2/inflate/inflateReset/inflateEnd
        compress, compress2, compressBound, uncompress
        CRC-32, Adler-32 - Using modern, minimal code size, CPU cache friendly routines.
        Supports raw deflate streams or standard zlib streams with adler-32 checking.

     Limitations:
      The callback API's are not implemented yet. No support for gzip headers or zlib static dictionaries.
      I've tried to closely emulate zlib's various flavors of stream flushing and return status codes, but
      there are no guarantees that miniz.c pulls this off perfectly.

   * PNG writing: See the tdefl_write_image_to_png_file_in_memory() function, originally written by
     Alex Evans. Supports 1-4 bytes/pixel images.

   * ZIP archive API notes:

     The ZIP archive API's where designed with simplicity and efficiency in mind, with just enough abstraction to
     get the job done with minimal fuss. There are simple API's to retrieve file information, read files from
     existing archives, create new archives, append new files to existing archives, or clone archive data from
     one archive to another. It supports archives located in memory or the heap, on disk (using stdio.h),
     or you can specify custom file read/write callbacks.

     - Archive reading: Just call this function to read a single file from a disk archive:

      void *mz_zip_extract_archive_file_to_heap(const char *pZip_filename, const char *pArchive_name,
        size_t *pSize, mz_uint zip_flags);

     For more complex cases, use the "mz_zip_reader" functions. Upon opening an archive, the entire central
     directory is located and read as-is into memory, and subsequent file access only occurs when reading individual files.

     - Archives file scanning: The simple way is to use this function to scan a loaded archive for a specific file:

     int mz_zip_reader_locate_file(mz_zip_archive *pZip, const char *pName, const char *pComment, mz_uint flags);

     The locate operation can optionally check file comments too, which (as one example) can be used to identify
     multiple versions of the same file in an archive. This function uses a simple linear search through the central
     directory, so it's not very fast.

     Alternately, you can iterate through all the files in an archive (using mz_zip_reader_get_num_files()) and
     retrieve detailed info on each file by calling mz_zip_reader_file_stat().

     - Archive creation: Use the "mz_zip_writer" functions. The ZIP writer immediately writes compressed file data
     to disk and builds an exact image of the central directory in memory. The central directory image is written
     all at once at the end of the archive file when the archive is finalized.

     The archive writer can optionally align each file's local header and file data to any power of 2 alignment,
     which can be useful when the archive will be read from optical media. Also, the writer supports placing
     arbitrary data blobs at the very beginning of ZIP archives. Archives written using either feature are still
     readable by any ZIP tool.

     - Archive appending: The simple way to add a single file to an archive is to call this function:

      mz_bool mz_zip_add_mem_to_archive_file_in_place(const char *pZip_filename, const char *pArchive_name,
        const void *pBuf, size_t buf_size, const void *pComment, mz_uint16 comment_size, mz_uint level_and_flags);

     The archive will be created if it doesn't already exist, otherwise it'll be appended to.
     Note the appending is done in-place and is not an atomic operation, so if something goes wrong
     during the operation it's possible the archive could be left without a central directory (although the local
     file headers and file data will be fine, so the archive will be recoverable).

     For more complex archive modification scenarios:
     1. The safest way is to use a mz_zip_reader to read the existing archive, cloning only those bits you want to
     preserve into a new archive using using the mz_zip_writer_add_from_zip_reader() function (which compiles the
     compressed file data as-is). When you're done, delete the old archive and rename the newly written archive, and
     you're done. This is safe but requires a bunch of temporary disk space or heap memory.

     2. Or, you can convert an mz_zip_reader in-place to an mz_zip_writer using mz_zip_writer_init_from_reader(),
     append new files as needed, then finalize the archive which will write an updated central directory to the
     original archive. (This is basically what mz_zip_add_mem_to_archive_file_in_place() does.) There's a
     possibility that the archive's central directory could be lost with this method if anything goes wrong, though.

     - ZIP archive support limitations:
     No spanning support. Extraction functions can only handle unencrypted, stored or deflated files.
     Requires streams capable of seeking.

   * This is a header file library, like stb_image.c. To get only a header file, either cut and paste the
     below header, or create miniz.h, #define MINIZ_HEADER_FILE_ONLY, and then include miniz.c from it.

   * Important: For best perf. be sure to customize the below macros for your target platform:
     #define MINIZ_USE_UNALIGNED_LOADS_AND_STORES 1
     #define MINIZ_LITTLE_ENDIAN 1
     #define MINIZ_HAS_64BIT_REGISTERS 1

   * On platforms using glibc, Be sure to "#define _LARGEFILE64_SOURCE 1" before including miniz.c to ensure miniz
     uses the 64-bit variants: fopen64(), stat64(), etc. Otherwise you won't be able to process large files
     (i.e. 32-bit stat() fails for me on files > 0x7FFFFFFF bytes).
*/
#pragma once

// hack-miniz - inline header
// #include "miniz_export.h"

/* Defines to completely disable specific portions of miniz.c:
   If all macros here are defined the only functionality remaining will be CRC-32 and adler-32. */

/* Define MINIZ_NO_STDIO to disable all usage and any functions which rely on stdio for file I/O. */
/*#define MINIZ_NO_STDIO */

/* If MINIZ_NO_TIME is specified then the ZIP archive functions will not be able to get the current time, or */
/* get/set file times, and the C run-time funcs that get/set times won't be called. */
/* The current downside is the times written to your archives will be from 1979. */
/*#define MINIZ_NO_TIME */

/* Define MINIZ_NO_DEFLATE_APIS to disable all compression API's. */
/*#define MINIZ_NO_DEFLATE_APIS */

/* Define MINIZ_NO_INFLATE_APIS to disable all decompression API's. */
/*#define MINIZ_NO_INFLATE_APIS */

/* Define MINIZ_NO_ARCHIVE_APIS to disable all ZIP archive API's. */
/*#define MINIZ_NO_ARCHIVE_APIS */

/* Define MINIZ_NO_ARCHIVE_WRITING_APIS to disable all writing related ZIP archive API's. */
/*#define MINIZ_NO_ARCHIVE_WRITING_APIS */

/* Define MINIZ_NO_ZLIB_APIS to remove all ZLIB-style compression/decompression API's. */
/*#define MINIZ_NO_ZLIB_APIS */

/* Define MINIZ_NO_ZLIB_COMPATIBLE_NAME to disable zlib names, to prevent conflicts against stock zlib. */
/*#define MINIZ_NO_ZLIB_COMPATIBLE_NAMES */

/* Define MINIZ_NO_MALLOC to disable all calls to malloc, free, and realloc.
   Note if MINIZ_NO_MALLOC is defined then the user must always provide custom user alloc/free/realloc
   callbacks to the zlib and archive API's, and a few stand-alone helper API's which don't provide custom user
   functions (such as tdefl_compress_mem_to_heap() and tinfl_decompress_mem_to_heap()) won't work. */
/*#define MINIZ_NO_MALLOC */

#ifdef MINIZ_NO_INFLATE_APIS
#define MINIZ_NO_ARCHIVE_APIS
#endif

#ifdef MINIZ_NO_DEFLATE_APIS
#define MINIZ_NO_ARCHIVE_WRITING_APIS
#endif

#if defined(__TINYC__) && (defined(__linux) || defined(__linux__))
/* TODO: Work around "error: include file 'sys\utime.h' when compiling with tcc on Linux */
#define MINIZ_NO_TIME
#endif

#include <stddef.h>

#if !defined(MINIZ_NO_TIME) && !defined(MINIZ_NO_ARCHIVE_APIS)
#include <time.h>
#endif

#if defined(_M_IX86) || defined(_M_X64) || defined(__i386__) || defined(__i386) || defined(__i486__) || defined(__i486) || defined(i386) || defined(__ia64__) || defined(__x86_64__)
/* MINIZ_X86_OR_X64_CPU is only used to help set the below macros. */
#define MINIZ_X86_OR_X64_CPU 1
#else
#define MINIZ_X86_OR_X64_CPU 0
#endif

/* Set MINIZ_LITTLE_ENDIAN only if not set */
#if !defined(MINIZ_LITTLE_ENDIAN)
#if defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__)

#if (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
/* Set MINIZ_LITTLE_ENDIAN to 1 if the processor is little endian. */
#define MINIZ_LITTLE_ENDIAN 1
#else
#define MINIZ_LITTLE_ENDIAN 0
#endif

#else

#if MINIZ_X86_OR_X64_CPU
#define MINIZ_LITTLE_ENDIAN 1
#else
#define MINIZ_LITTLE_ENDIAN 0
#endif

#endif
#endif

/* Using unaligned loads and stores causes errors when using UBSan */
#if defined(__has_feature)
#if __has_feature(undefined_behavior_sanitizer)
#define MINIZ_USE_UNALIGNED_LOADS_AND_STORES 0
#endif
#endif

/* Set MINIZ_USE_UNALIGNED_LOADS_AND_STORES only if not set */
#if !defined(MINIZ_USE_UNALIGNED_LOADS_AND_STORES)
#if MINIZ_X86_OR_X64_CPU
/* Set MINIZ_USE_UNALIGNED_LOADS_AND_STORES to 1 on CPU's that permit efficient integer loads and stores from unaligned addresses. */
#define MINIZ_USE_UNALIGNED_LOADS_AND_STORES 0
#define MINIZ_UNALIGNED_USE_MEMCPY
#else
#define MINIZ_USE_UNALIGNED_LOADS_AND_STORES 0
#endif
#endif

#if defined(_M_X64) || defined(_WIN64) || defined(__MINGW64__) || defined(_LP64) || defined(__LP64__) || defined(__ia64__) || defined(__x86_64__)
/* Set MINIZ_HAS_64BIT_REGISTERS to 1 if operations on 64-bit integers are reasonably fast (and don't involve compiler generated calls to helper functions). */
#define MINIZ_HAS_64BIT_REGISTERS 1
#else
#define MINIZ_HAS_64BIT_REGISTERS 0
#endif

#ifdef __cplusplus
extern "C"
{
#endif

    /* ------------------- zlib-style API Definitions. */

    /* For more compatibility with zlib, miniz.c uses unsigned long for some parameters/struct members. Beware: mz_ulong can be either 32 or 64-bits! */
    typedef unsigned long mz_ulong;

    /* mz_free() internally uses the MZ_FREE() macro (which by default calls free() unless you've modified the MZ_MALLOC macro) to release a block allocated from the heap. */
    MINIZ_EXPORT void mz_free(void *p);

#define MZ_ADLER32_INIT (1)
    /* mz_adler32() returns the initial adler-32 value to use when called with ptr==NULL. */
    MINIZ_EXPORT mz_ulong mz_adler32(mz_ulong adler, const unsigned char *ptr, size_t buf_len);

#define MZ_CRC32_INIT (0)
    /* mz_crc32() returns the initial CRC-32 value to use when called with ptr==NULL. */
    MINIZ_EXPORT mz_ulong mz_crc32(mz_ulong crc, const unsigned char *ptr, size_t buf_len);

    /* Compression strategies. */
    enum
    {
        MZ_DEFAULT_STRATEGY = 0,
        MZ_FILTERED = 1,
        MZ_HUFFMAN_ONLY = 2,
        MZ_RLE = 3,
        MZ_FIXED = 4
    };

/* Method */
#define MZ_DEFLATED 8

    /* Heap allocation callbacks.
    Note that mz_alloc_func parameter types purposely differ from zlib's: items/size is size_t, not unsigned long. */
    typedef void *(*mz_alloc_func)(void *opaque, size_t items, size_t size);
    typedef void (*mz_free_func)(void *opaque, void *address);
    typedef void *(*mz_realloc_func)(void *opaque, void *address, size_t items, size_t size);

    /* Compression levels: 0-9 are the standard zlib-style levels, 10 is best possible compression (not zlib compatible, and may be very slow), MZ_DEFAULT_COMPRESSION=MZ_DEFAULT_LEVEL. */
    enum
    {
        MZ_NO_COMPRESSION = 0,
        MZ_BEST_SPEED = 1,
        MZ_BEST_COMPRESSION = 9,
        MZ_UBER_COMPRESSION = 10,
        MZ_DEFAULT_LEVEL = 6,
        MZ_DEFAULT_COMPRESSION = -1
    };

#define MZ_VERSION "11.3.0"
#define MZ_VERNUM 0xB300
#define MZ_VER_MAJOR 11
#define MZ_VER_MINOR 3
#define MZ_VER_REVISION 0
#define MZ_VER_SUBREVISION 0

#ifndef MINIZ_NO_ZLIB_APIS

    /* Flush values. For typical usage you only need MZ_NO_FLUSH and MZ_FINISH. The other values are for advanced use (refer to the zlib docs). */
    enum
    {
        MZ_NO_FLUSH = 0,
        MZ_PARTIAL_FLUSH = 1,
        MZ_SYNC_FLUSH = 2,
        MZ_FULL_FLUSH = 3,
        MZ_FINISH = 4,
        MZ_BLOCK = 5
    };

    /* Return status codes. MZ_PARAM_ERROR is non-standard. */
    enum
    {
        MZ_OK = 0,
        MZ_STREAM_END = 1,
        MZ_NEED_DICT = 2,
        MZ_ERRNO = -1,
        MZ_STREAM_ERROR = -2,
        MZ_DATA_ERROR = -3,
        MZ_MEM_ERROR = -4,
        MZ_BUF_ERROR = -5,
        MZ_VERSION_ERROR = -6,
        MZ_PARAM_ERROR = -10000
    };

/* Window bits */
#define MZ_DEFAULT_WINDOW_BITS 15

    struct mz_internal_state;

    /* Compression/decompression stream struct. */
    typedef struct mz_stream_s
    {
        const unsigned char *next_in; /* pointer to next byte to read */
        unsigned int avail_in;        /* number of bytes available at next_in */
        mz_ulong total_in;            /* total number of bytes consumed so far */

        unsigned char *next_out; /* pointer to next byte to write */
        unsigned int avail_out;  /* number of bytes that can be written to next_out */
        mz_ulong total_out;      /* total number of bytes produced so far */

        char *msg;                       /* error msg (unused) */
        struct mz_internal_state *state; /* internal state, allocated by zalloc/zfree */

        mz_alloc_func zalloc; /* optional heap allocation function (defaults to malloc) */
        mz_free_func zfree;   /* optional heap free function (defaults to free) */
        void *opaque;         /* heap alloc function user pointer */

        int data_type;     /* data_type (unused) */
        mz_ulong adler;    /* adler32 of the source or uncompressed data */
        mz_ulong reserved; /* not used */
    } mz_stream;

    typedef mz_stream *mz_streamp;

    /* Returns the version string of miniz.c. */
    MINIZ_EXPORT const char *mz_version(void);

#ifndef MINIZ_NO_DEFLATE_APIS

    /* mz_deflateInit() initializes a compressor with default options: */
    /* Parameters: */
    /*  pStream must point to an initialized mz_stream struct. */
    /*  level must be between [MZ_NO_COMPRESSION, MZ_BEST_COMPRESSION]. */
    /*  level 1 enables a specially optimized compression function that's been optimized purely for performance, not ratio. */
    /*  (This special func. is currently only enabled when MINIZ_USE_UNALIGNED_LOADS_AND_STORES and MINIZ_LITTLE_ENDIAN are defined.) */
    /* Return values: */
    /*  MZ_OK on success. */
    /*  MZ_STREAM_ERROR if the stream is bogus. */
    /*  MZ_PARAM_ERROR if the input parameters are bogus. */
    /*  MZ_MEM_ERROR on out of memory. */
    MINIZ_EXPORT int mz_deflateInit(mz_streamp pStream, int level);

    /* mz_deflateInit2() is like mz_deflate(), except with more control: */
    /* Additional parameters: */
    /*   method must be MZ_DEFLATED */
    /*   window_bits must be MZ_DEFAULT_WINDOW_BITS (to wrap the deflate stream with zlib header/adler-32 footer) or -MZ_DEFAULT_WINDOW_BITS (raw deflate/no header or footer) */
    /*   mem_level must be between [1, 9] (it's checked but ignored by miniz.c) */
    MINIZ_EXPORT int mz_deflateInit2(mz_streamp pStream, int level, int method, int window_bits, int mem_level, int strategy);

    /* Quickly resets a compressor without having to reallocate anything. Same as calling mz_deflateEnd() followed by mz_deflateInit()/mz_deflateInit2(). */
    MINIZ_EXPORT int mz_deflateReset(mz_streamp pStream);

    /* mz_deflate() compresses the input to output, consuming as much of the input and producing as much output as possible. */
    /* Parameters: */
    /*   pStream is the stream to read from and write to. You must initialize/update the next_in, avail_in, next_out, and avail_out members. */
    /*   flush may be MZ_NO_FLUSH, MZ_PARTIAL_FLUSH/MZ_SYNC_FLUSH, MZ_FULL_FLUSH, or MZ_FINISH. */
    /* Return values: */
    /*   MZ_OK on success (when flushing, or if more input is needed but not available, and/or there's more output to be written but the output buffer is full). */
    /*   MZ_STREAM_END if all input has been consumed and all output bytes have been written. Don't call mz_deflate() on the stream anymore. */
    /*   MZ_STREAM_ERROR if the stream is bogus. */
    /*   MZ_PARAM_ERROR if one of the parameters is invalid. */
    /*   MZ_BUF_ERROR if no forward progress is possible because the input and/or output buffers are empty. (Fill up the input buffer or free up some output space and try again.) */
    MINIZ_EXPORT int mz_deflate(mz_streamp pStream, int flush);

    /* mz_deflateEnd() deinitializes a compressor: */
    /* Return values: */
    /*  MZ_OK on success. */
    /*  MZ_STREAM_ERROR if the stream is bogus. */
    MINIZ_EXPORT int mz_deflateEnd(mz_streamp pStream);

    /* mz_deflateBound() returns a (very) conservative upper bound on the amount of data that could be generated by deflate(), assuming flush is set to only MZ_NO_FLUSH or MZ_FINISH. */
    MINIZ_EXPORT mz_ulong mz_deflateBound(mz_streamp pStream, mz_ulong source_len);

    /* Single-call compression functions mz_compress() and mz_compress2(): */
    /* Returns MZ_OK on success, or one of the error codes from mz_deflate() on failure. */
    MINIZ_EXPORT int mz_compress(unsigned char *pDest, mz_ulong *pDest_len, const unsigned char *pSource, mz_ulong source_len);
    MINIZ_EXPORT int mz_compress2(unsigned char *pDest, mz_ulong *pDest_len, const unsigned char *pSource, mz_ulong source_len, int level);

    /* mz_compressBound() returns a (very) conservative upper bound on the amount of data that could be generated by calling mz_compress(). */
    MINIZ_EXPORT mz_ulong mz_compressBound(mz_ulong source_len);

#endif /*#ifndef MINIZ_NO_DEFLATE_APIS*/

#ifndef MINIZ_NO_INFLATE_APIS

    /* Initializes a decompressor. */
    MINIZ_EXPORT int mz_inflateInit(mz_streamp pStream);

    /* mz_inflateInit2() is like mz_inflateInit() with an additional option that controls the window size and whether or not the stream has been wrapped with a zlib header/footer: */
    /* window_bits must be MZ_DEFAULT_WINDOW_BITS (to parse zlib header/footer) or -MZ_DEFAULT_WINDOW_BITS (raw deflate). */
    MINIZ_EXPORT int mz_inflateInit2(mz_streamp pStream, int window_bits);

    /* Quickly resets a compressor without having to reallocate anything. Same as calling mz_inflateEnd() followed by mz_inflateInit()/mz_inflateInit2(). */
    MINIZ_EXPORT int mz_inflateReset(mz_streamp pStream);

    /* Decompresses the input stream to the output, consuming only as much of the input as needed, and writing as much to the output as possible. */
    /* Parameters: */
    /*   pStream is the stream to read from and write to. You must initialize/update the next_in, avail_in, next_out, and avail_out members. */
    /*   flush may be MZ_NO_FLUSH, MZ_SYNC_FLUSH, or MZ_FINISH. */
    /*   On the first call, if flush is MZ_FINISH it's assumed the input and output buffers are both sized large enough to decompress the entire stream in a single call (this is slightly faster). */
    /*   MZ_FINISH implies that there are no more source bytes available beside what's already in the input buffer, and that the output buffer is large enough to hold the rest of the decompressed data. */
    /* Return values: */
    /*   MZ_OK on success. Either more input is needed but not available, and/or there's more output to be written but the output buffer is full. */
    /*   MZ_STREAM_END if all needed input has been consumed and all output bytes have been written. For zlib streams, the adler-32 of the decompressed data has also been verified. */
    /*   MZ_STREAM_ERROR if the stream is bogus. */
    /*   MZ_DATA_ERROR if the deflate stream is invalid. */
    /*   MZ_PARAM_ERROR if one of the parameters is invalid. */
    /*   MZ_BUF_ERROR if no forward progress is possible because the input buffer is empty but the inflater needs more input to continue, or if the output buffer is not large enough. Call mz_inflate() again */
    /*   with more input data, or with more room in the output buffer (except when using single call decompression, described above). */
    MINIZ_EXPORT int mz_inflate(mz_streamp pStream, int flush);

    /* Deinitializes a decompressor. */
    MINIZ_EXPORT int mz_inflateEnd(mz_streamp pStream);

    /* Single-call decompression. */
    /* Returns MZ_OK on success, or one of the error codes from mz_inflate() on failure. */
    MINIZ_EXPORT int mz_uncompress(unsigned char *pDest, mz_ulong *pDest_len, const unsigned char *pSource, mz_ulong source_len);
    MINIZ_EXPORT int mz_uncompress2(unsigned char *pDest, mz_ulong *pDest_len, const unsigned char *pSource, mz_ulong *pSource_len);
#endif /*#ifndef MINIZ_NO_INFLATE_APIS*/

    /* Returns a string description of the specified error code, or NULL if the error code is invalid. */
    MINIZ_EXPORT const char *mz_error(int err);

/* Redefine zlib-compatible names to miniz equivalents, so miniz.c can be used as a drop-in replacement for the subset of zlib that miniz.c supports. */
/* Define MINIZ_NO_ZLIB_COMPATIBLE_NAMES to disable zlib-compatibility if you use zlib in the same project. */
#ifndef MINIZ_NO_ZLIB_COMPATIBLE_NAMES
    typedef unsigned char Byte;
    typedef unsigned int uInt;
    typedef mz_ulong uLong;
    typedef Byte Bytef;
    typedef uInt uIntf;
    typedef char charf;
    typedef int intf;
    typedef void *voidpf;
    typedef uLong uLongf;
    typedef void *voidp;
    typedef void *const voidpc;
#define Z_NULL 0
#define Z_NO_FLUSH MZ_NO_FLUSH
#define Z_PARTIAL_FLUSH MZ_PARTIAL_FLUSH
#define Z_SYNC_FLUSH MZ_SYNC_FLUSH
#define Z_FULL_FLUSH MZ_FULL_FLUSH
#define Z_FINISH MZ_FINISH
#define Z_BLOCK MZ_BLOCK
#define Z_OK MZ_OK
#define Z_STREAM_END MZ_STREAM_END
#define Z_NEED_DICT MZ_NEED_DICT
#define Z_ERRNO MZ_ERRNO
#define Z_STREAM_ERROR MZ_STREAM_ERROR
#define Z_DATA_ERROR MZ_DATA_ERROR
#define Z_MEM_ERROR MZ_MEM_ERROR
#define Z_BUF_ERROR MZ_BUF_ERROR
#define Z_VERSION_ERROR MZ_VERSION_ERROR
#define Z_PARAM_ERROR MZ_PARAM_ERROR
#define Z_NO_COMPRESSION MZ_NO_COMPRESSION
#define Z_BEST_SPEED MZ_BEST_SPEED
#define Z_BEST_COMPRESSION MZ_BEST_COMPRESSION
#define Z_DEFAULT_COMPRESSION MZ_DEFAULT_COMPRESSION
#define Z_DEFAULT_STRATEGY MZ_DEFAULT_STRATEGY
#define Z_FILTERED MZ_FILTERED
#define Z_HUFFMAN_ONLY MZ_HUFFMAN_ONLY
#define Z_RLE MZ_RLE
#define Z_FIXED MZ_FIXED
#define Z_DEFLATED MZ_DEFLATED
#define Z_DEFAULT_WINDOW_BITS MZ_DEFAULT_WINDOW_BITS
    /* See mz_alloc_func */
    typedef void *(*alloc_func)(void *opaque, size_t items, size_t size);
    /* See mz_free_func */
    typedef void (*free_func)(void *opaque, void *address);

#define internal_state mz_internal_state
#define z_stream mz_stream

#ifndef MINIZ_NO_DEFLATE_APIS
    /* Compatiblity with zlib API. See called functions for documentation */
    static int deflateInit(mz_streamp pStream, int level)
    {
        return mz_deflateInit(pStream, level);
    }
    static int deflateInit2(mz_streamp pStream, int level, int method, int window_bits, int mem_level, int strategy)
    {
        return mz_deflateInit2(pStream, level, method, window_bits, mem_level, strategy);
    }
    static int deflateReset(mz_streamp pStream)
    {
        return mz_deflateReset(pStream);
    }
    static int deflate(mz_streamp pStream, int flush)
    {
        return mz_deflate(pStream, flush);
    }
    static int deflateEnd(mz_streamp pStream)
    {
        return mz_deflateEnd(pStream);
    }
    static mz_ulong deflateBound(mz_streamp pStream, mz_ulong source_len)
    {
        return mz_deflateBound(pStream, source_len);
    }
    static int compress(unsigned char *pDest, mz_ulong *pDest_len, const unsigned char *pSource, mz_ulong source_len)
    {
        return mz_compress(pDest, pDest_len, pSource, source_len);
    }
    static int compress2(unsigned char *pDest, mz_ulong *pDest_len, const unsigned char *pSource, mz_ulong source_len, int level)
    {
        return mz_compress2(pDest, pDest_len, pSource, source_len, level);
    }
    static mz_ulong compressBound(mz_ulong source_len)
    {
        return mz_compressBound(source_len);
    }
#endif /*#ifndef MINIZ_NO_DEFLATE_APIS*/

#ifndef MINIZ_NO_INFLATE_APIS
    /* Compatiblity with zlib API. See called functions for documentation */
    static int inflateInit(mz_streamp pStream)
    {
        return mz_inflateInit(pStream);
    }

    static int inflateInit2(mz_streamp pStream, int window_bits)
    {
        return mz_inflateInit2(pStream, window_bits);
    }

    static int inflateReset(mz_streamp pStream)
    {
        return mz_inflateReset(pStream);
    }

    static int inflate(mz_streamp pStream, int flush)
    {
        return mz_inflate(pStream, flush);
    }

    static int inflateEnd(mz_streamp pStream)
    {
        return mz_inflateEnd(pStream);
    }

    static int uncompress(unsigned char* pDest, mz_ulong* pDest_len, const unsigned char* pSource, mz_ulong source_len)
    {
        return mz_uncompress(pDest, pDest_len, pSource, source_len);
    }

    static int uncompress2(unsigned char* pDest, mz_ulong* pDest_len, const unsigned char* pSource, mz_ulong* pSource_len)
    {
        return mz_uncompress2(pDest, pDest_len, pSource, pSource_len);
    }
#endif /*#ifndef MINIZ_NO_INFLATE_APIS*/

    static mz_ulong crc32(mz_ulong crc, const unsigned char *ptr, size_t buf_len)
    {
        return mz_crc32(crc, ptr, buf_len);
    }

    static mz_ulong adler32(mz_ulong adler, const unsigned char *ptr, size_t buf_len)
    {
        return mz_adler32(adler, ptr, buf_len);
    }

#define MAX_WBITS 15
#define MAX_MEM_LEVEL 9

    static const char* zError(int err)
    {
        return mz_error(err);
    }
#define ZLIB_VERSION MZ_VERSION
#define ZLIB_VERNUM MZ_VERNUM
#define ZLIB_VER_MAJOR MZ_VER_MAJOR
#define ZLIB_VER_MINOR MZ_VER_MINOR
#define ZLIB_VER_REVISION MZ_VER_REVISION
#define ZLIB_VER_SUBREVISION MZ_VER_SUBREVISION

#define zlibVersion mz_version
#define zlib_version mz_version()
#endif /* #ifndef MINIZ_NO_ZLIB_COMPATIBLE_NAMES */

#endif /* MINIZ_NO_ZLIB_APIS */

#ifdef __cplusplus
}
#endif

// hack-miniz - inline header
// #include "miniz_common.h"
// hack-miniz - inline header
// #include "miniz_tdef.h"
// hack-miniz - inline header
// #include "miniz_tinfl.h"
// hack-miniz - inline header
// #include "miniz_zip.h"


/*
file https://github.com/richgel999/miniz/blob/3.1.0/miniz_common.h
*/
#pragma once
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// hack-miniz - inline header
// #include "miniz_export.h"

/* ------------------- Types and macros */
typedef unsigned char mz_uint8;
typedef int16_t mz_int16;
typedef uint16_t mz_uint16;
typedef uint32_t mz_uint32;
typedef uint32_t mz_uint;
typedef int64_t mz_int64;
typedef uint64_t mz_uint64;
typedef int mz_bool;

#define MZ_FALSE (0)
#define MZ_TRUE (1)

/* Works around MSVC's spammy "warning C4127: conditional expression is constant" message. */
#ifdef _MSC_VER
#define MZ_MACRO_END while (0, 0)
#else
#define MZ_MACRO_END while (0)
#endif

#ifdef MINIZ_NO_STDIO
#define MZ_FILE void *
#else
#include <stdio.h>
#define MZ_FILE FILE
#endif /* #ifdef MINIZ_NO_STDIO */

#ifdef MINIZ_NO_TIME
typedef struct mz_dummy_time_t_tag
{
    mz_uint32 m_dummy1;
    mz_uint32 m_dummy2;
} mz_dummy_time_t;
#define MZ_TIME_T mz_dummy_time_t
#else
#define MZ_TIME_T time_t
#endif

#define MZ_ASSERT(x) assert(x)

#ifdef MINIZ_NO_MALLOC
#define MZ_MALLOC(x) NULL
#define MZ_FREE(x) (void)x, ((void)0)
#define MZ_REALLOC(p, x) NULL
#else
#define MZ_MALLOC(x) malloc(x)
#define MZ_FREE(x) free(x)
#define MZ_REALLOC(p, x) realloc(p, x)
#endif

#define MZ_MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MZ_MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MZ_CLEAR_OBJ(obj) memset(&(obj), 0, sizeof(obj))
#define MZ_CLEAR_ARR(obj) memset((obj), 0, sizeof(obj))
#define MZ_CLEAR_PTR(obj) memset((obj), 0, sizeof(*obj))

#if MINIZ_USE_UNALIGNED_LOADS_AND_STORES && MINIZ_LITTLE_ENDIAN
#define MZ_READ_LE16(p) *((const mz_uint16 *)(p))
#define MZ_READ_LE32(p) *((const mz_uint32 *)(p))
#else
#define MZ_READ_LE16(p) ((mz_uint32)(((const mz_uint8 *)(p))[0]) | ((mz_uint32)(((const mz_uint8 *)(p))[1]) << 8U))
#define MZ_READ_LE32(p) ((mz_uint32)(((const mz_uint8 *)(p))[0]) | ((mz_uint32)(((const mz_uint8 *)(p))[1]) << 8U) | ((mz_uint32)(((const mz_uint8 *)(p))[2]) << 16U) | ((mz_uint32)(((const mz_uint8 *)(p))[3]) << 24U))
#endif

#define MZ_READ_LE64(p) (((mz_uint64)MZ_READ_LE32(p)) | (((mz_uint64)MZ_READ_LE32((const mz_uint8 *)(p) + sizeof(mz_uint32))) << 32U))

#ifdef _MSC_VER
#define MZ_FORCEINLINE __forceinline
#elif defined(__GNUC__)
#define MZ_FORCEINLINE __inline__ __attribute__((__always_inline__))
#else
#define MZ_FORCEINLINE inline
#endif

#ifdef __cplusplus
extern "C"
{
#endif

    extern MINIZ_EXPORT void *miniz_def_alloc_func(void *opaque, size_t items, size_t size);
    extern MINIZ_EXPORT void miniz_def_free_func(void *opaque, void *address);
    extern MINIZ_EXPORT void *miniz_def_realloc_func(void *opaque, void *address, size_t items, size_t size);

#define MZ_UINT16_MAX (0xFFFFU)
#define MZ_UINT32_MAX (0xFFFFFFFFU)

#ifdef __cplusplus
}
#endif


/*
file https://github.com/richgel999/miniz/blob/3.1.0/miniz_tdef.h
*/
#pragma once
// hack-miniz - inline header
// #include "miniz_common.h"

#ifndef MINIZ_NO_DEFLATE_APIS

#ifdef __cplusplus
extern "C"
{
#endif
/* ------------------- Low-level Compression API Definitions */

/* Set TDEFL_LESS_MEMORY to 1 to use less memory (compression will be slightly slower, and raw/dynamic blocks will be output more frequently). */
#ifndef TDEFL_LESS_MEMORY
#define TDEFL_LESS_MEMORY 0
#endif

    /* tdefl_init() compression flags logically OR'd together (low 12 bits contain the max. number of probes per dictionary search): */
    /* TDEFL_DEFAULT_MAX_PROBES: The compressor defaults to 128 dictionary probes per dictionary search. 0=Huffman only, 1=Huffman+LZ (fastest/crap compression), 4095=Huffman+LZ (slowest/best compression). */
    enum
    {
        TDEFL_HUFFMAN_ONLY = 0,
        TDEFL_DEFAULT_MAX_PROBES = 128,
        TDEFL_MAX_PROBES_MASK = 0xFFF
    };

    /* TDEFL_WRITE_ZLIB_HEADER: If set, the compressor outputs a zlib header before the deflate data, and the Adler-32 of the source data at the end. Otherwise, you'll get raw deflate data. */
    /* TDEFL_COMPUTE_ADLER32: Always compute the adler-32 of the input data (even when not writing zlib headers). */
    /* TDEFL_GREEDY_PARSING_FLAG: Set to use faster greedy parsing, instead of more efficient lazy parsing. */
    /* TDEFL_NONDETERMINISTIC_PARSING_FLAG: Enable to decrease the compressor's initialization time to the minimum, but the output may vary from run to run given the same input (depending on the contents of memory). */
    /* TDEFL_RLE_MATCHES: Only look for RLE matches (matches with a distance of 1) */
    /* TDEFL_FILTER_MATCHES: Discards matches <= 5 chars if enabled. */
    /* TDEFL_FORCE_ALL_STATIC_BLOCKS: Disable usage of optimized Huffman tables. */
    /* TDEFL_FORCE_ALL_RAW_BLOCKS: Only use raw (uncompressed) deflate blocks. */
    /* The low 12 bits are reserved to control the max # of hash probes per dictionary lookup (see TDEFL_MAX_PROBES_MASK). */
    enum
    {
        TDEFL_WRITE_ZLIB_HEADER = 0x01000,
        TDEFL_COMPUTE_ADLER32 = 0x02000,
        TDEFL_GREEDY_PARSING_FLAG = 0x04000,
        TDEFL_NONDETERMINISTIC_PARSING_FLAG = 0x08000,
        TDEFL_RLE_MATCHES = 0x10000,
        TDEFL_FILTER_MATCHES = 0x20000,
        TDEFL_FORCE_ALL_STATIC_BLOCKS = 0x40000,
        TDEFL_FORCE_ALL_RAW_BLOCKS = 0x80000
    };

    /* High level compression functions: */
    /* tdefl_compress_mem_to_heap() compresses a block in memory to a heap block allocated via malloc(). */
    /* On entry: */
    /*  pSrc_buf, src_buf_len: Pointer and size of source block to compress. */
    /*  flags: The max match finder probes (default is 128) logically OR'd against the above flags. Higher probes are slower but improve compression. */
    /* On return: */
    /*  Function returns a pointer to the compressed data, or NULL on failure. */
    /*  *pOut_len will be set to the compressed data's size, which could be larger than src_buf_len on uncompressible data. */
    /*  The caller must free() the returned block when it's no longer needed. */
    MINIZ_EXPORT void *tdefl_compress_mem_to_heap(const void *pSrc_buf, size_t src_buf_len, size_t *pOut_len, int flags);

    /* tdefl_compress_mem_to_mem() compresses a block in memory to another block in memory. */
    /* Returns 0 on failure. */
    MINIZ_EXPORT size_t tdefl_compress_mem_to_mem(void *pOut_buf, size_t out_buf_len, const void *pSrc_buf, size_t src_buf_len, int flags);

    /* Compresses an image to a compressed PNG file in memory. */
    /* On entry: */
    /*  pImage, w, h, and num_chans describe the image to compress. num_chans may be 1, 2, 3, or 4. */
    /*  The image pitch in bytes per scanline will be w*num_chans. The leftmost pixel on the top scanline is stored first in memory. */
    /*  level may range from [0,10], use MZ_NO_COMPRESSION, MZ_BEST_SPEED, MZ_BEST_COMPRESSION, etc. or a decent default is MZ_DEFAULT_LEVEL */
    /*  If flip is true, the image will be flipped on the Y axis (useful for OpenGL apps). */
    /* On return: */
    /*  Function returns a pointer to the compressed data, or NULL on failure. */
    /*  *pLen_out will be set to the size of the PNG image file. */
    /*  The caller must mz_free() the returned heap block (which will typically be larger than *pLen_out) when it's no longer needed. */
    MINIZ_EXPORT void *tdefl_write_image_to_png_file_in_memory_ex(const void *pImage, int w, int h, int num_chans, size_t *pLen_out, mz_uint level, mz_bool flip);
    MINIZ_EXPORT void *tdefl_write_image_to_png_file_in_memory(const void *pImage, int w, int h, int num_chans, size_t *pLen_out);

    /* Output stream interface. The compressor uses this interface to write compressed data. It'll typically be called TDEFL_OUT_BUF_SIZE at a time. */
    typedef mz_bool (*tdefl_put_buf_func_ptr)(const void *pBuf, int len, void *pUser);

    /* tdefl_compress_mem_to_output() compresses a block to an output stream. The above helpers use this function internally. */
    MINIZ_EXPORT mz_bool tdefl_compress_mem_to_output(const void *pBuf, size_t buf_len, tdefl_put_buf_func_ptr pPut_buf_func, void *pPut_buf_user, int flags);

    enum
    {
        TDEFL_MAX_HUFF_TABLES = 3,
        TDEFL_MAX_HUFF_SYMBOLS_0 = 288,
        TDEFL_MAX_HUFF_SYMBOLS_1 = 32,
        TDEFL_MAX_HUFF_SYMBOLS_2 = 19,
        TDEFL_LZ_DICT_SIZE = 32768,
        TDEFL_LZ_DICT_SIZE_MASK = TDEFL_LZ_DICT_SIZE - 1,
        TDEFL_MIN_MATCH_LEN = 3,
        TDEFL_MAX_MATCH_LEN = 258
    };

/* TDEFL_OUT_BUF_SIZE MUST be large enough to hold a single entire compressed output block (using static/fixed Huffman codes). */
#if TDEFL_LESS_MEMORY
    enum
    {
        TDEFL_LZ_CODE_BUF_SIZE = 24 * 1024,
        TDEFL_OUT_BUF_SIZE = (TDEFL_LZ_CODE_BUF_SIZE * 13) / 10,
        TDEFL_MAX_HUFF_SYMBOLS = 288,
        TDEFL_LZ_HASH_BITS = 12,
        TDEFL_LEVEL1_HASH_SIZE_MASK = 4095,
        TDEFL_LZ_HASH_SHIFT = (TDEFL_LZ_HASH_BITS + 2) / 3,
        TDEFL_LZ_HASH_SIZE = 1 << TDEFL_LZ_HASH_BITS
    };
#else
enum
{
    TDEFL_LZ_CODE_BUF_SIZE = 64 * 1024,
    TDEFL_OUT_BUF_SIZE = (mz_uint)((TDEFL_LZ_CODE_BUF_SIZE * 13) / 10),
    TDEFL_MAX_HUFF_SYMBOLS = 288,
    TDEFL_LZ_HASH_BITS = 15,
    TDEFL_LEVEL1_HASH_SIZE_MASK = 4095,
    TDEFL_LZ_HASH_SHIFT = (TDEFL_LZ_HASH_BITS + 2) / 3,
    TDEFL_LZ_HASH_SIZE = 1 << TDEFL_LZ_HASH_BITS
};
#endif

    /* The low-level tdefl functions below may be used directly if the above helper functions aren't flexible enough. The low-level functions don't make any heap allocations, unlike the above helper functions. */
    typedef enum
    {
        TDEFL_STATUS_BAD_PARAM = -2,
        TDEFL_STATUS_PUT_BUF_FAILED = -1,
        TDEFL_STATUS_OKAY = 0,
        TDEFL_STATUS_DONE = 1
    } tdefl_status;

    /* Must map to MZ_NO_FLUSH, MZ_SYNC_FLUSH, etc. enums */
    typedef enum
    {
        TDEFL_NO_FLUSH = 0,
        TDEFL_SYNC_FLUSH = 2,
        TDEFL_FULL_FLUSH = 3,
        TDEFL_FINISH = 4
    } tdefl_flush;

    /* tdefl's compression state structure. */
    typedef struct
    {
        tdefl_put_buf_func_ptr m_pPut_buf_func;
        void *m_pPut_buf_user;
        mz_uint m_flags, m_max_probes[2];
        int m_greedy_parsing;
        mz_uint m_adler32, m_lookahead_pos, m_lookahead_size, m_dict_size;
        mz_uint8 *m_pLZ_code_buf, *m_pLZ_flags, *m_pOutput_buf, *m_pOutput_buf_end;
        mz_uint m_num_flags_left, m_total_lz_bytes, m_lz_code_buf_dict_pos, m_bits_in, m_bit_buffer;
        mz_uint m_saved_match_dist, m_saved_match_len, m_saved_lit, m_output_flush_ofs, m_output_flush_remaining, m_finished, m_block_index, m_wants_to_finish;
        tdefl_status m_prev_return_status;
        const void *m_pIn_buf;
        void *m_pOut_buf;
        size_t *m_pIn_buf_size, *m_pOut_buf_size;
        tdefl_flush m_flush;
        const mz_uint8 *m_pSrc;
        size_t m_src_buf_left, m_out_buf_ofs;
        mz_uint8 m_dict[TDEFL_LZ_DICT_SIZE + TDEFL_MAX_MATCH_LEN - 1];
        mz_uint16 m_huff_count[TDEFL_MAX_HUFF_TABLES][TDEFL_MAX_HUFF_SYMBOLS];
        mz_uint16 m_huff_codes[TDEFL_MAX_HUFF_TABLES][TDEFL_MAX_HUFF_SYMBOLS];
        mz_uint8 m_huff_code_sizes[TDEFL_MAX_HUFF_TABLES][TDEFL_MAX_HUFF_SYMBOLS];
        mz_uint8 m_lz_code_buf[TDEFL_LZ_CODE_BUF_SIZE];
        mz_uint16 m_next[TDEFL_LZ_DICT_SIZE];
        mz_uint16 m_hash[TDEFL_LZ_HASH_SIZE];
        mz_uint8 m_output_buf[TDEFL_OUT_BUF_SIZE];
    } tdefl_compressor;

    /* Initializes the compressor. */
    /* There is no corresponding deinit() function because the tdefl API's do not dynamically allocate memory. */
    /* pBut_buf_func: If NULL, output data will be supplied to the specified callback. In this case, the user should call the tdefl_compress_buffer() API for compression. */
    /* If pBut_buf_func is NULL the user should always call the tdefl_compress() API. */
    /* flags: See the above enums (TDEFL_HUFFMAN_ONLY, TDEFL_WRITE_ZLIB_HEADER, etc.) */
    MINIZ_EXPORT tdefl_status tdefl_init(tdefl_compressor *d, tdefl_put_buf_func_ptr pPut_buf_func, void *pPut_buf_user, int flags);

    /* Compresses a block of data, consuming as much of the specified input buffer as possible, and writing as much compressed data to the specified output buffer as possible. */
    MINIZ_EXPORT tdefl_status tdefl_compress(tdefl_compressor *d, const void *pIn_buf, size_t *pIn_buf_size, void *pOut_buf, size_t *pOut_buf_size, tdefl_flush flush);

    /* tdefl_compress_buffer() is only usable when the tdefl_init() is called with a non-NULL tdefl_put_buf_func_ptr. */
    /* tdefl_compress_buffer() always consumes the entire input buffer. */
    MINIZ_EXPORT tdefl_status tdefl_compress_buffer(tdefl_compressor *d, const void *pIn_buf, size_t in_buf_size, tdefl_flush flush);

    MINIZ_EXPORT tdefl_status tdefl_get_prev_return_status(tdefl_compressor *d);
    MINIZ_EXPORT mz_uint32 tdefl_get_adler32(tdefl_compressor *d);

    /* Create tdefl_compress() flags given zlib-style compression parameters. */
    /* level may range from [0,10] (where 10 is absolute max compression, but may be much slower on some files) */
    /* window_bits may be -15 (raw deflate) or 15 (zlib) */
    /* strategy may be either MZ_DEFAULT_STRATEGY, MZ_FILTERED, MZ_HUFFMAN_ONLY, MZ_RLE, or MZ_FIXED */
    MINIZ_EXPORT mz_uint tdefl_create_comp_flags_from_zip_params(int level, int window_bits, int strategy);

#ifndef MINIZ_NO_MALLOC
    /* Allocate the tdefl_compressor structure in C so that */
    /* non-C language bindings to tdefl_ API don't need to worry about */
    /* structure size and allocation mechanism. */
    MINIZ_EXPORT tdefl_compressor *tdefl_compressor_alloc(void);
    MINIZ_EXPORT void tdefl_compressor_free(tdefl_compressor *pComp);
#endif

#ifdef __cplusplus
}
#endif

#endif /*#ifndef MINIZ_NO_DEFLATE_APIS*/


/*
file https://github.com/richgel999/miniz/blob/3.1.0/miniz_tinfl.h
*/
#pragma once
// hack-miniz - inline header
// #include "miniz_common.h"
/* ------------------- Low-level Decompression API Definitions */

#ifndef MINIZ_NO_INFLATE_APIS

#ifdef __cplusplus
extern "C"
{
#endif
    /* Decompression flags used by tinfl_decompress(). */
    /* TINFL_FLAG_PARSE_ZLIB_HEADER: If set, the input has a valid zlib header and ends with an adler32 checksum (it's a valid zlib stream). Otherwise, the input is a raw deflate stream. */
    /* TINFL_FLAG_HAS_MORE_INPUT: If set, there are more input bytes available beyond the end of the supplied input buffer. If clear, the input buffer contains all remaining input. */
    /* TINFL_FLAG_USING_NON_WRAPPING_OUTPUT_BUF: If set, the output buffer is large enough to hold the entire decompressed stream. If clear, the output buffer is at least the size of the dictionary (typically 32KB). */
    /* TINFL_FLAG_COMPUTE_ADLER32: Force adler-32 checksum computation of the decompressed bytes. */
    enum
    {
        TINFL_FLAG_PARSE_ZLIB_HEADER = 1,
        TINFL_FLAG_HAS_MORE_INPUT = 2,
        TINFL_FLAG_USING_NON_WRAPPING_OUTPUT_BUF = 4,
        TINFL_FLAG_COMPUTE_ADLER32 = 8
    };

    /* High level decompression functions: */
    /* tinfl_decompress_mem_to_heap() decompresses a block in memory to a heap block allocated via malloc(). */
    /* On entry: */
    /*  pSrc_buf, src_buf_len: Pointer and size of the Deflate or zlib source data to decompress. */
    /* On return: */
    /*  Function returns a pointer to the decompressed data, or NULL on failure. */
    /*  *pOut_len will be set to the decompressed data's size, which could be larger than src_buf_len on uncompressible data. */
    /*  The caller must call mz_free() on the returned block when it's no longer needed. */
    MINIZ_EXPORT void *tinfl_decompress_mem_to_heap(const void *pSrc_buf, size_t src_buf_len, size_t *pOut_len, int flags);

/* tinfl_decompress_mem_to_mem() decompresses a block in memory to another block in memory. */
/* Returns TINFL_DECOMPRESS_MEM_TO_MEM_FAILED on failure, or the number of bytes written on success. */
#define TINFL_DECOMPRESS_MEM_TO_MEM_FAILED ((size_t)(-1))
    MINIZ_EXPORT size_t tinfl_decompress_mem_to_mem(void *pOut_buf, size_t out_buf_len, const void *pSrc_buf, size_t src_buf_len, int flags);

    /* tinfl_decompress_mem_to_callback() decompresses a block in memory to an internal 32KB buffer, and a user provided callback function will be called to flush the buffer. */
    /* Returns 1 on success or 0 on failure. */
    typedef int (*tinfl_put_buf_func_ptr)(const void *pBuf, int len, void *pUser);
    MINIZ_EXPORT int tinfl_decompress_mem_to_callback(const void *pIn_buf, size_t *pIn_buf_size, tinfl_put_buf_func_ptr pPut_buf_func, void *pPut_buf_user, int flags);

    struct tinfl_decompressor_tag;
    typedef struct tinfl_decompressor_tag tinfl_decompressor;

#ifndef MINIZ_NO_MALLOC
    /* Allocate the tinfl_decompressor structure in C so that */
    /* non-C language bindings to tinfl_ API don't need to worry about */
    /* structure size and allocation mechanism. */
    MINIZ_EXPORT tinfl_decompressor *tinfl_decompressor_alloc(void);
    MINIZ_EXPORT void tinfl_decompressor_free(tinfl_decompressor *pDecomp);
#endif

/* Max size of LZ dictionary. */
#define TINFL_LZ_DICT_SIZE 32768

    /* Return status. */
    typedef enum
    {
        /* This flags indicates the inflator needs 1 or more input bytes to make forward progress, but the caller is indicating that no more are available. The compressed data */
        /* is probably corrupted. If you call the inflator again with more bytes it'll try to continue processing the input but this is a BAD sign (either the data is corrupted or you called it incorrectly). */
        /* If you call it again with no input you'll just get TINFL_STATUS_FAILED_CANNOT_MAKE_PROGRESS again. */
        TINFL_STATUS_FAILED_CANNOT_MAKE_PROGRESS = -4,

        /* This flag indicates that one or more of the input parameters was obviously bogus. (You can try calling it again, but if you get this error the calling code is wrong.) */
        TINFL_STATUS_BAD_PARAM = -3,

        /* This flags indicate the inflator is finished but the adler32 check of the uncompressed data didn't match. If you call it again it'll return TINFL_STATUS_DONE. */
        TINFL_STATUS_ADLER32_MISMATCH = -2,

        /* This flags indicate the inflator has somehow failed (bad code, corrupted input, etc.). If you call it again without resetting via tinfl_init() it it'll just keep on returning the same status failure code. */
        TINFL_STATUS_FAILED = -1,

        /* Any status code less than TINFL_STATUS_DONE must indicate a failure. */

        /* This flag indicates the inflator has returned every byte of uncompressed data that it can, has consumed every byte that it needed, has successfully reached the end of the deflate stream, and */
        /* if zlib headers and adler32 checking enabled that it has successfully checked the uncompressed data's adler32. If you call it again you'll just get TINFL_STATUS_DONE over and over again. */
        TINFL_STATUS_DONE = 0,

        /* This flag indicates the inflator MUST have more input data (even 1 byte) before it can make any more forward progress, or you need to clear the TINFL_FLAG_HAS_MORE_INPUT */
        /* flag on the next call if you don't have any more source data. If the source data was somehow corrupted it's also possible (but unlikely) for the inflator to keep on demanding input to */
        /* proceed, so be sure to properly set the TINFL_FLAG_HAS_MORE_INPUT flag. */
        TINFL_STATUS_NEEDS_MORE_INPUT = 1,

        /* This flag indicates the inflator definitely has 1 or more bytes of uncompressed data available, but it cannot write this data into the output buffer. */
        /* Note if the source compressed data was corrupted it's possible for the inflator to return a lot of uncompressed data to the caller. I've been assuming you know how much uncompressed data to expect */
        /* (either exact or worst case) and will stop calling the inflator and fail after receiving too much. In pure streaming scenarios where you have no idea how many bytes to expect this may not be possible */
        /* so I may need to add some code to address this. */
        TINFL_STATUS_HAS_MORE_OUTPUT = 2
    } tinfl_status;

/* Initializes the decompressor to its initial state. */
#define tinfl_init(r)     \
    do                    \
    {                     \
        (r)->m_state = 0; \
    }                     \
    MZ_MACRO_END
#define tinfl_get_adler32(r) (r)->m_check_adler32

    /* Main low-level decompressor coroutine function. This is the only function actually needed for decompression. All the other functions are just high-level helpers for improved usability. */
    /* This is a universal API, i.e. it can be used as a building block to build any desired higher level decompression API. In the limit case, it can be called once per every byte input or output. */
    MINIZ_EXPORT tinfl_status tinfl_decompress(tinfl_decompressor *r, const mz_uint8 *pIn_buf_next, size_t *pIn_buf_size, mz_uint8 *pOut_buf_start, mz_uint8 *pOut_buf_next, size_t *pOut_buf_size, const mz_uint32 decomp_flags);

    /* Internal/private bits follow. */
    enum
    {
        TINFL_MAX_HUFF_TABLES = 3,
        TINFL_MAX_HUFF_SYMBOLS_0 = 288,
        TINFL_MAX_HUFF_SYMBOLS_1 = 32,
        TINFL_MAX_HUFF_SYMBOLS_2 = 19,
        TINFL_FAST_LOOKUP_BITS = 10,
        TINFL_FAST_LOOKUP_SIZE = 1 << TINFL_FAST_LOOKUP_BITS
    };

#if MINIZ_HAS_64BIT_REGISTERS
#define TINFL_USE_64BIT_BITBUF 1
#else
#define TINFL_USE_64BIT_BITBUF 0
#endif

#if TINFL_USE_64BIT_BITBUF
    typedef mz_uint64 tinfl_bit_buf_t;
#define TINFL_BITBUF_SIZE (64)
#else
typedef mz_uint32 tinfl_bit_buf_t;
#define TINFL_BITBUF_SIZE (32)
#endif

    struct tinfl_decompressor_tag
    {
        mz_uint32 m_state, m_num_bits, m_zhdr0, m_zhdr1, m_z_adler32, m_final, m_type, m_check_adler32, m_dist, m_counter, m_num_extra, m_table_sizes[TINFL_MAX_HUFF_TABLES];
        tinfl_bit_buf_t m_bit_buf;
        size_t m_dist_from_out_buf_start;
        mz_int16 m_look_up[TINFL_MAX_HUFF_TABLES][TINFL_FAST_LOOKUP_SIZE];
        mz_int16 m_tree_0[TINFL_MAX_HUFF_SYMBOLS_0 * 2];
        mz_int16 m_tree_1[TINFL_MAX_HUFF_SYMBOLS_1 * 2];
        mz_int16 m_tree_2[TINFL_MAX_HUFF_SYMBOLS_2 * 2];
        mz_uint8 m_code_size_0[TINFL_MAX_HUFF_SYMBOLS_0];
        mz_uint8 m_code_size_1[TINFL_MAX_HUFF_SYMBOLS_1];
        mz_uint8 m_code_size_2[TINFL_MAX_HUFF_SYMBOLS_2];
        mz_uint8 m_raw_header[4], m_len_codes[TINFL_MAX_HUFF_SYMBOLS_0 + TINFL_MAX_HUFF_SYMBOLS_1 + 137];
    };

#ifdef __cplusplus
}
#endif

#endif /*#ifndef MINIZ_NO_INFLATE_APIS*/


/*
file https://github.com/richgel999/miniz/blob/3.1.0/miniz_zip.h
*/
#pragma once
// hack-miniz - inline header
// #include "miniz_common.h"

/* ------------------- ZIP archive reading/writing */

#ifndef MINIZ_NO_ARCHIVE_APIS

#ifdef __cplusplus
extern "C"
{
#endif

    enum
    {
        /* Note: These enums can be reduced as needed to save memory or stack space - they are pretty conservative. */
        MZ_ZIP_MAX_IO_BUF_SIZE = 64 * 1024,
        MZ_ZIP_MAX_ARCHIVE_FILENAME_SIZE = 512,
        MZ_ZIP_MAX_ARCHIVE_FILE_COMMENT_SIZE = 512
    };

    typedef struct
    {
        /* Central directory file index. */
        mz_uint32 m_file_index;

        /* Byte offset of this entry in the archive's central directory. Note we currently only support up to UINT_MAX or less bytes in the central dir. */
        mz_uint64 m_central_dir_ofs;

        /* These fields are copied directly from the zip's central dir. */
        mz_uint16 m_version_made_by;
        mz_uint16 m_version_needed;
        mz_uint16 m_bit_flag;
        mz_uint16 m_method;

        /* CRC-32 of uncompressed data. */
        mz_uint32 m_crc32;

        /* File's compressed size. */
        mz_uint64 m_comp_size;

        /* File's uncompressed size. Note, I've seen some old archives where directory entries had 512 bytes for their uncompressed sizes, but when you try to unpack them you actually get 0 bytes. */
        mz_uint64 m_uncomp_size;

        /* Zip internal and external file attributes. */
        mz_uint16 m_internal_attr;
        mz_uint32 m_external_attr;

        /* Entry's local header file offset in bytes. */
        mz_uint64 m_local_header_ofs;

        /* Size of comment in bytes. */
        mz_uint32 m_comment_size;

        /* MZ_TRUE if the entry appears to be a directory. */
        mz_bool m_is_directory;

        /* MZ_TRUE if the entry uses encryption/strong encryption (which miniz_zip doesn't support) */
        mz_bool m_is_encrypted;

        /* MZ_TRUE if the file is not encrypted, a patch file, and if it uses a compression method we support. */
        mz_bool m_is_supported;

        /* Filename. If string ends in '/' it's a subdirectory entry. */
        /* Guaranteed to be zero terminated, may be truncated to fit. */
        char m_filename[MZ_ZIP_MAX_ARCHIVE_FILENAME_SIZE];

        /* Comment field. */
        /* Guaranteed to be zero terminated, may be truncated to fit. */
        char m_comment[MZ_ZIP_MAX_ARCHIVE_FILE_COMMENT_SIZE];

#ifdef MINIZ_NO_TIME
        MZ_TIME_T m_padding;
#else
    MZ_TIME_T m_time;
#endif
    } mz_zip_archive_file_stat;

    typedef size_t (*mz_file_read_func)(void *pOpaque, mz_uint64 file_ofs, void *pBuf, size_t n);
    typedef size_t (*mz_file_write_func)(void *pOpaque, mz_uint64 file_ofs, const void *pBuf, size_t n);
    typedef mz_bool (*mz_file_needs_keepalive)(void *pOpaque);

    struct mz_zip_internal_state_tag;
    typedef struct mz_zip_internal_state_tag mz_zip_internal_state;

    typedef enum
    {
        MZ_ZIP_MODE_INVALID = 0,
        MZ_ZIP_MODE_READING = 1,
        MZ_ZIP_MODE_WRITING = 2,
        MZ_ZIP_MODE_WRITING_HAS_BEEN_FINALIZED = 3
    } mz_zip_mode;

    typedef enum
    {
        MZ_ZIP_FLAG_CASE_SENSITIVE = 0x0100,
        MZ_ZIP_FLAG_IGNORE_PATH = 0x0200,
        MZ_ZIP_FLAG_COMPRESSED_DATA = 0x0400,
        MZ_ZIP_FLAG_DO_NOT_SORT_CENTRAL_DIRECTORY = 0x0800,
        MZ_ZIP_FLAG_VALIDATE_LOCATE_FILE_FLAG = 0x1000, /* if enabled, mz_zip_reader_locate_file() will be called on each file as its validated to ensure the func finds the file in the central dir (intended for testing) */
        MZ_ZIP_FLAG_VALIDATE_HEADERS_ONLY = 0x2000,     /* validate the local headers, but don't decompress the entire file and check the crc32 */
        MZ_ZIP_FLAG_WRITE_ZIP64 = 0x4000,               /* always use the zip64 file format, instead of the original zip file format with automatic switch to zip64. Use as flags parameter with mz_zip_writer_init*_v2 */
        MZ_ZIP_FLAG_WRITE_ALLOW_READING = 0x8000,
        MZ_ZIP_FLAG_ASCII_FILENAME = 0x10000,
        /*After adding a compressed file, seek back
        to local file header and set the correct sizes*/
        MZ_ZIP_FLAG_WRITE_HEADER_SET_SIZE = 0x20000,
        MZ_ZIP_FLAG_READ_ALLOW_WRITING = 0x40000
    } mz_zip_flags;

    typedef enum
    {
        MZ_ZIP_TYPE_INVALID = 0,
        MZ_ZIP_TYPE_USER,
        MZ_ZIP_TYPE_MEMORY,
        MZ_ZIP_TYPE_HEAP,
        MZ_ZIP_TYPE_FILE,
        MZ_ZIP_TYPE_CFILE,
        MZ_ZIP_TOTAL_TYPES
    } mz_zip_type;

    /* miniz error codes. Be sure to update mz_zip_get_error_string() if you add or modify this enum. */
    typedef enum
    {
        MZ_ZIP_NO_ERROR = 0,
        MZ_ZIP_UNDEFINED_ERROR,
        MZ_ZIP_TOO_MANY_FILES,
        MZ_ZIP_FILE_TOO_LARGE,
        MZ_ZIP_UNSUPPORTED_METHOD,
        MZ_ZIP_UNSUPPORTED_ENCRYPTION,
        MZ_ZIP_UNSUPPORTED_FEATURE,
        MZ_ZIP_FAILED_FINDING_CENTRAL_DIR,
        MZ_ZIP_NOT_AN_ARCHIVE,
        MZ_ZIP_INVALID_HEADER_OR_CORRUPTED,
        MZ_ZIP_UNSUPPORTED_MULTIDISK,
        MZ_ZIP_DECOMPRESSION_FAILED,
        MZ_ZIP_COMPRESSION_FAILED,
        MZ_ZIP_UNEXPECTED_DECOMPRESSED_SIZE,
        MZ_ZIP_CRC_CHECK_FAILED,
        MZ_ZIP_UNSUPPORTED_CDIR_SIZE,
        MZ_ZIP_ALLOC_FAILED,
        MZ_ZIP_FILE_OPEN_FAILED,
        MZ_ZIP_FILE_CREATE_FAILED,
        MZ_ZIP_FILE_WRITE_FAILED,
        MZ_ZIP_FILE_READ_FAILED,
        MZ_ZIP_FILE_CLOSE_FAILED,
        MZ_ZIP_FILE_SEEK_FAILED,
        MZ_ZIP_FILE_STAT_FAILED,
        MZ_ZIP_INVALID_PARAMETER,
        MZ_ZIP_INVALID_FILENAME,
        MZ_ZIP_BUF_TOO_SMALL,
        MZ_ZIP_INTERNAL_ERROR,
        MZ_ZIP_FILE_NOT_FOUND,
        MZ_ZIP_ARCHIVE_TOO_LARGE,
        MZ_ZIP_VALIDATION_FAILED,
        MZ_ZIP_WRITE_CALLBACK_FAILED,
        MZ_ZIP_TOTAL_ERRORS
    } mz_zip_error;

    typedef struct
    {
        mz_uint64 m_archive_size;
        mz_uint64 m_central_directory_file_ofs;

        /* We only support up to UINT32_MAX files in zip64 mode. */
        mz_uint32 m_total_files;
        mz_zip_mode m_zip_mode;
        mz_zip_type m_zip_type;
        mz_zip_error m_last_error;

        mz_uint64 m_file_offset_alignment;

        mz_alloc_func m_pAlloc;
        mz_free_func m_pFree;
        mz_realloc_func m_pRealloc;
        void *m_pAlloc_opaque;

        mz_file_read_func m_pRead;
        mz_file_write_func m_pWrite;
        mz_file_needs_keepalive m_pNeeds_keepalive;
        void *m_pIO_opaque;

        mz_zip_internal_state *m_pState;
    } mz_zip_archive;

    typedef struct
    {
        mz_zip_archive *pZip;
        mz_uint flags;

        int status;

        mz_uint64 read_buf_size, read_buf_ofs, read_buf_avail, comp_remaining, out_buf_ofs, cur_file_ofs;
        mz_zip_archive_file_stat file_stat;
        void *pRead_buf;
        void *pWrite_buf;

        size_t out_blk_remain;

        tinfl_decompressor inflator;

#ifdef MINIZ_DISABLE_ZIP_READER_CRC32_CHECKS
        mz_uint padding;
#else
    mz_uint file_crc32;
#endif
    } mz_zip_reader_extract_iter_state;

    /* -------- ZIP reading */

    /* Inits a ZIP archive reader. */
    /* These functions read and validate the archive's central directory. */
    MINIZ_EXPORT mz_bool mz_zip_reader_init(mz_zip_archive *pZip, mz_uint64 size, mz_uint flags);

    MINIZ_EXPORT mz_bool mz_zip_reader_init_mem(mz_zip_archive *pZip, const void *pMem, size_t size, mz_uint flags);

#ifndef MINIZ_NO_STDIO
    /* Read a archive from a disk file. */
    /* file_start_ofs is the file offset where the archive actually begins, or 0. */
    /* actual_archive_size is the true total size of the archive, which may be smaller than the file's actual size on disk. If zero the entire file is treated as the archive. */
    MINIZ_EXPORT mz_bool mz_zip_reader_init_file(mz_zip_archive *pZip, const char *pFilename, mz_uint32 flags);
    MINIZ_EXPORT mz_bool mz_zip_reader_init_file_v2(mz_zip_archive *pZip, const char *pFilename, mz_uint flags, mz_uint64 file_start_ofs, mz_uint64 archive_size);

    /* Read an archive from an already opened FILE, beginning at the current file position. */
    /* The archive is assumed to be archive_size bytes long. If archive_size is 0, then the entire rest of the file is assumed to contain the archive. */
    /* The FILE will NOT be closed when mz_zip_reader_end() is called. */
    MINIZ_EXPORT mz_bool mz_zip_reader_init_cfile(mz_zip_archive *pZip, MZ_FILE *pFile, mz_uint64 archive_size, mz_uint flags);
#endif

    /* Ends archive reading, freeing all allocations, and closing the input archive file if mz_zip_reader_init_file() was used. */
    MINIZ_EXPORT mz_bool mz_zip_reader_end(mz_zip_archive *pZip);

    /* -------- ZIP reading or writing */

    /* Clears a mz_zip_archive struct to all zeros. */
    /* Important: This must be done before passing the struct to any mz_zip functions. */
    MINIZ_EXPORT void mz_zip_zero_struct(mz_zip_archive *pZip);

    MINIZ_EXPORT mz_zip_mode mz_zip_get_mode(mz_zip_archive *pZip);
    MINIZ_EXPORT mz_zip_type mz_zip_get_type(mz_zip_archive *pZip);

    /* Returns the total number of files in the archive. */
    MINIZ_EXPORT mz_uint mz_zip_reader_get_num_files(mz_zip_archive *pZip);

    MINIZ_EXPORT mz_uint64 mz_zip_get_archive_size(mz_zip_archive *pZip);
    MINIZ_EXPORT mz_uint64 mz_zip_get_archive_file_start_offset(mz_zip_archive *pZip);
    MINIZ_EXPORT MZ_FILE *mz_zip_get_cfile(mz_zip_archive *pZip);

    /* Reads n bytes of raw archive data, starting at file offset file_ofs, to pBuf. */
    MINIZ_EXPORT size_t mz_zip_read_archive_data(mz_zip_archive *pZip, mz_uint64 file_ofs, void *pBuf, size_t n);

    /* All mz_zip funcs set the m_last_error field in the mz_zip_archive struct. These functions retrieve/manipulate this field. */
    /* Note that the m_last_error functionality is not thread safe. */
    MINIZ_EXPORT mz_zip_error mz_zip_set_last_error(mz_zip_archive *pZip, mz_zip_error err_num);
    MINIZ_EXPORT mz_zip_error mz_zip_peek_last_error(mz_zip_archive *pZip);
    MINIZ_EXPORT mz_zip_error mz_zip_clear_last_error(mz_zip_archive *pZip);
    MINIZ_EXPORT mz_zip_error mz_zip_get_last_error(mz_zip_archive *pZip);
    MINIZ_EXPORT const char *mz_zip_get_error_string(mz_zip_error mz_err);

    /* MZ_TRUE if the archive file entry is a directory entry. */
    MINIZ_EXPORT mz_bool mz_zip_reader_is_file_a_directory(mz_zip_archive *pZip, mz_uint file_index);

    /* MZ_TRUE if the file is encrypted/strong encrypted. */
    MINIZ_EXPORT mz_bool mz_zip_reader_is_file_encrypted(mz_zip_archive *pZip, mz_uint file_index);

    /* MZ_TRUE if the compression method is supported, and the file is not encrypted, and the file is not a compressed patch file. */
    MINIZ_EXPORT mz_bool mz_zip_reader_is_file_supported(mz_zip_archive *pZip, mz_uint file_index);

    /* Retrieves the filename of an archive file entry. */
    /* Returns the number of bytes written to pFilename, or if filename_buf_size is 0 this function returns the number of bytes needed to fully store the filename. */
    MINIZ_EXPORT mz_uint mz_zip_reader_get_filename(mz_zip_archive *pZip, mz_uint file_index, char *pFilename, mz_uint filename_buf_size);

    /* Attempts to locates a file in the archive's central directory. */
    /* Valid flags: MZ_ZIP_FLAG_CASE_SENSITIVE, MZ_ZIP_FLAG_IGNORE_PATH */
    /* Returns -1 if the file cannot be found. */
    MINIZ_EXPORT int mz_zip_reader_locate_file(mz_zip_archive *pZip, const char *pName, const char *pComment, mz_uint flags);
    MINIZ_EXPORT mz_bool mz_zip_reader_locate_file_v2(mz_zip_archive *pZip, const char *pName, const char *pComment, mz_uint flags, mz_uint32 *file_index);

    /* Returns detailed information about an archive file entry. */
    MINIZ_EXPORT mz_bool mz_zip_reader_file_stat(mz_zip_archive *pZip, mz_uint file_index, mz_zip_archive_file_stat *pStat);

    /* MZ_TRUE if the file is in zip64 format. */
    /* A file is considered zip64 if it contained a zip64 end of central directory marker, or if it contained any zip64 extended file information fields in the central directory. */
    MINIZ_EXPORT mz_bool mz_zip_is_zip64(mz_zip_archive *pZip);

    /* Returns the total central directory size in bytes. */
    /* The current max supported size is <= MZ_UINT32_MAX. */
    MINIZ_EXPORT size_t mz_zip_get_central_dir_size(mz_zip_archive *pZip);

    /* Extracts a archive file to a memory buffer using no memory allocation. */
    /* There must be at least enough room on the stack to store the inflator's state (~34KB or so). */
    MINIZ_EXPORT mz_bool mz_zip_reader_extract_to_mem_no_alloc(mz_zip_archive *pZip, mz_uint file_index, void *pBuf, size_t buf_size, mz_uint flags, void *pUser_read_buf, size_t user_read_buf_size);
    MINIZ_EXPORT mz_bool mz_zip_reader_extract_file_to_mem_no_alloc(mz_zip_archive *pZip, const char *pFilename, void *pBuf, size_t buf_size, mz_uint flags, void *pUser_read_buf, size_t user_read_buf_size);

    /* Extracts a archive file to a memory buffer. */
    MINIZ_EXPORT mz_bool mz_zip_reader_extract_to_mem(mz_zip_archive *pZip, mz_uint file_index, void *pBuf, size_t buf_size, mz_uint flags);
    MINIZ_EXPORT mz_bool mz_zip_reader_extract_file_to_mem(mz_zip_archive *pZip, const char *pFilename, void *pBuf, size_t buf_size, mz_uint flags);

    /* Extracts a archive file to a dynamically allocated heap buffer. */
    /* The memory will be allocated via the mz_zip_archive's alloc/realloc functions. */
    /* Returns NULL and sets the last error on failure. */
    MINIZ_EXPORT void *mz_zip_reader_extract_to_heap(mz_zip_archive *pZip, mz_uint file_index, size_t *pSize, mz_uint flags);
    MINIZ_EXPORT void *mz_zip_reader_extract_file_to_heap(mz_zip_archive *pZip, const char *pFilename, size_t *pSize, mz_uint flags);

    /* Extracts a archive file using a callback function to output the file's data. */
    MINIZ_EXPORT mz_bool mz_zip_reader_extract_to_callback(mz_zip_archive *pZip, mz_uint file_index, mz_file_write_func pCallback, void *pOpaque, mz_uint flags);
    MINIZ_EXPORT mz_bool mz_zip_reader_extract_file_to_callback(mz_zip_archive *pZip, const char *pFilename, mz_file_write_func pCallback, void *pOpaque, mz_uint flags);

    /* Extract a file iteratively */
    MINIZ_EXPORT mz_zip_reader_extract_iter_state *mz_zip_reader_extract_iter_new(mz_zip_archive *pZip, mz_uint file_index, mz_uint flags);
    MINIZ_EXPORT mz_zip_reader_extract_iter_state *mz_zip_reader_extract_file_iter_new(mz_zip_archive *pZip, const char *pFilename, mz_uint flags);
    MINIZ_EXPORT size_t mz_zip_reader_extract_iter_read(mz_zip_reader_extract_iter_state *pState, void *pvBuf, size_t buf_size);
    MINIZ_EXPORT mz_bool mz_zip_reader_extract_iter_free(mz_zip_reader_extract_iter_state *pState);

#ifndef MINIZ_NO_STDIO
    /* Extracts a archive file to a disk file and sets its last accessed and modified times. */
    /* This function only extracts files, not archive directory records. */
    MINIZ_EXPORT mz_bool mz_zip_reader_extract_to_file(mz_zip_archive *pZip, mz_uint file_index, const char *pDst_filename, mz_uint flags);
    MINIZ_EXPORT mz_bool mz_zip_reader_extract_file_to_file(mz_zip_archive *pZip, const char *pArchive_filename, const char *pDst_filename, mz_uint flags);

    /* Extracts a archive file starting at the current position in the destination FILE stream. */
    MINIZ_EXPORT mz_bool mz_zip_reader_extract_to_cfile(mz_zip_archive *pZip, mz_uint file_index, MZ_FILE *File, mz_uint flags);
    MINIZ_EXPORT mz_bool mz_zip_reader_extract_file_to_cfile(mz_zip_archive *pZip, const char *pArchive_filename, MZ_FILE *pFile, mz_uint flags);
#endif

#if 0
/* TODO */
	typedef void *mz_zip_streaming_extract_state_ptr;
	mz_zip_streaming_extract_state_ptr mz_zip_streaming_extract_begin(mz_zip_archive *pZip, mz_uint file_index, mz_uint flags);
	mz_uint64 mz_zip_streaming_extract_get_size(mz_zip_archive *pZip, mz_zip_streaming_extract_state_ptr pState);
	mz_uint64 mz_zip_streaming_extract_get_cur_ofs(mz_zip_archive *pZip, mz_zip_streaming_extract_state_ptr pState);
	mz_bool mz_zip_streaming_extract_seek(mz_zip_archive *pZip, mz_zip_streaming_extract_state_ptr pState, mz_uint64 new_ofs);
	size_t mz_zip_streaming_extract_read(mz_zip_archive *pZip, mz_zip_streaming_extract_state_ptr pState, void *pBuf, size_t buf_size);
	mz_bool mz_zip_streaming_extract_end(mz_zip_archive *pZip, mz_zip_streaming_extract_state_ptr pState);
#endif

    /* This function compares the archive's local headers, the optional local zip64 extended information block, and the optional descriptor following the compressed data vs. the data in the central directory. */
    /* It also validates that each file can be successfully uncompressed unless the MZ_ZIP_FLAG_VALIDATE_HEADERS_ONLY is specified. */
    MINIZ_EXPORT mz_bool mz_zip_validate_file(mz_zip_archive *pZip, mz_uint file_index, mz_uint flags);

    /* Validates an entire archive by calling mz_zip_validate_file() on each file. */
    MINIZ_EXPORT mz_bool mz_zip_validate_archive(mz_zip_archive *pZip, mz_uint flags);

    /* Misc utils/helpers, valid for ZIP reading or writing */
    MINIZ_EXPORT mz_bool mz_zip_validate_mem_archive(const void *pMem, size_t size, mz_uint flags, mz_zip_error *pErr);
#ifndef MINIZ_NO_STDIO
    MINIZ_EXPORT mz_bool mz_zip_validate_file_archive(const char *pFilename, mz_uint flags, mz_zip_error *pErr);
#endif

    /* Universal end function - calls either mz_zip_reader_end() or mz_zip_writer_end(). */
    MINIZ_EXPORT mz_bool mz_zip_end(mz_zip_archive *pZip);

    /* -------- ZIP writing */

#ifndef MINIZ_NO_ARCHIVE_WRITING_APIS

    /* Inits a ZIP archive writer. */
    /*Set pZip->m_pWrite (and pZip->m_pIO_opaque) before calling mz_zip_writer_init or mz_zip_writer_init_v2*/
    /*The output is streamable, i.e. file_ofs in mz_file_write_func always increases only by n*/
    MINIZ_EXPORT mz_bool mz_zip_writer_init(mz_zip_archive *pZip, mz_uint64 existing_size);
    MINIZ_EXPORT mz_bool mz_zip_writer_init_v2(mz_zip_archive *pZip, mz_uint64 existing_size, mz_uint flags);

    MINIZ_EXPORT mz_bool mz_zip_writer_init_heap(mz_zip_archive *pZip, size_t size_to_reserve_at_beginning, size_t initial_allocation_size);
    MINIZ_EXPORT mz_bool mz_zip_writer_init_heap_v2(mz_zip_archive *pZip, size_t size_to_reserve_at_beginning, size_t initial_allocation_size, mz_uint flags);

#ifndef MINIZ_NO_STDIO
    MINIZ_EXPORT mz_bool mz_zip_writer_init_file(mz_zip_archive *pZip, const char *pFilename, mz_uint64 size_to_reserve_at_beginning);
    MINIZ_EXPORT mz_bool mz_zip_writer_init_file_v2(mz_zip_archive *pZip, const char *pFilename, mz_uint64 size_to_reserve_at_beginning, mz_uint flags);
    MINIZ_EXPORT mz_bool mz_zip_writer_init_cfile(mz_zip_archive *pZip, MZ_FILE *pFile, mz_uint flags);
#endif

    /* Converts a ZIP archive reader object into a writer object, to allow efficient in-place file appends to occur on an existing archive. */
    /* For archives opened using mz_zip_reader_init_file, pFilename must be the archive's filename so it can be reopened for writing. If the file can't be reopened, mz_zip_reader_end() will be called. */
    /* For archives opened using mz_zip_reader_init_mem, the memory block must be growable using the realloc callback (which defaults to realloc unless you've overridden it). */
    /* Finally, for archives opened using mz_zip_reader_init, the mz_zip_archive's user provided m_pWrite function cannot be NULL. */
    /* Note: In-place archive modification is not recommended unless you know what you're doing, because if execution stops or something goes wrong before */
    /* the archive is finalized the file's central directory will be hosed. */
    MINIZ_EXPORT mz_bool mz_zip_writer_init_from_reader(mz_zip_archive *pZip, const char *pFilename);
    MINIZ_EXPORT mz_bool mz_zip_writer_init_from_reader_v2(mz_zip_archive *pZip, const char *pFilename, mz_uint flags);

    /* Adds the contents of a memory buffer to an archive. These functions record the current local time into the archive. */
    /* To add a directory entry, call this method with an archive name ending in a forwardslash with an empty buffer. */
    /* level_and_flags - compression level (0-10, see MZ_BEST_SPEED, MZ_BEST_COMPRESSION, etc.) logically OR'd with zero or more mz_zip_flags, or just set to MZ_DEFAULT_COMPRESSION. */
    MINIZ_EXPORT mz_bool mz_zip_writer_add_mem(mz_zip_archive *pZip, const char *pArchive_name, const void *pBuf, size_t buf_size, mz_uint level_and_flags);

    /* Like mz_zip_writer_add_mem(), except you can specify a file comment field, and optionally supply the function with already compressed data. */
    /* uncomp_size/uncomp_crc32 are only used if the MZ_ZIP_FLAG_COMPRESSED_DATA flag is specified. */
    MINIZ_EXPORT mz_bool mz_zip_writer_add_mem_ex(mz_zip_archive *pZip, const char *pArchive_name, const void *pBuf, size_t buf_size, const void *pComment, mz_uint16 comment_size, mz_uint level_and_flags,
                                                  mz_uint64 uncomp_size, mz_uint32 uncomp_crc32);

    MINIZ_EXPORT mz_bool mz_zip_writer_add_mem_ex_v2(mz_zip_archive *pZip, const char *pArchive_name, const void *pBuf, size_t buf_size, const void *pComment, mz_uint16 comment_size, mz_uint level_and_flags,
                                                     mz_uint64 uncomp_size, mz_uint32 uncomp_crc32, MZ_TIME_T *last_modified, const char *user_extra_data_local, mz_uint user_extra_data_local_len,
                                                     const char *user_extra_data_central, mz_uint user_extra_data_central_len);

    /* Adds the contents of a file to an archive. This function also records the disk file's modified time into the archive. */
    /* File data is supplied via a read callback function. User mz_zip_writer_add_(c)file to add a file directly.*/
    MINIZ_EXPORT mz_bool mz_zip_writer_add_read_buf_callback(mz_zip_archive *pZip, const char *pArchive_name, mz_file_read_func read_callback, void *callback_opaque, mz_uint64 max_size,
                                                             const MZ_TIME_T *pFile_time, const void *pComment, mz_uint16 comment_size, mz_uint level_and_flags, const char *user_extra_data_local, mz_uint user_extra_data_local_len,
                                                             const char *user_extra_data_central, mz_uint user_extra_data_central_len);

#ifndef MINIZ_NO_STDIO
    /* Adds the contents of a disk file to an archive. This function also records the disk file's modified time into the archive. */
    /* level_and_flags - compression level (0-10, see MZ_BEST_SPEED, MZ_BEST_COMPRESSION, etc.) logically OR'd with zero or more mz_zip_flags, or just set to MZ_DEFAULT_COMPRESSION. */
    MINIZ_EXPORT mz_bool mz_zip_writer_add_file(mz_zip_archive *pZip, const char *pArchive_name, const char *pSrc_filename, const void *pComment, mz_uint16 comment_size, mz_uint level_and_flags);

    /* Like mz_zip_writer_add_file(), except the file data is read from the specified FILE stream. */
    MINIZ_EXPORT mz_bool mz_zip_writer_add_cfile(mz_zip_archive *pZip, const char *pArchive_name, MZ_FILE *pSrc_file, mz_uint64 max_size,
                                                 const MZ_TIME_T *pFile_time, const void *pComment, mz_uint16 comment_size, mz_uint level_and_flags, const char *user_extra_data_local, mz_uint user_extra_data_local_len,
                                                 const char *user_extra_data_central, mz_uint user_extra_data_central_len);
#endif

    /* Adds a file to an archive by fully cloning the data from another archive. */
    /* This function fully clones the source file's compressed data (no recompression), along with its full filename, extra data (it may add or modify the zip64 local header extra data field), and the optional descriptor following the compressed data. */
    MINIZ_EXPORT mz_bool mz_zip_writer_add_from_zip_reader(mz_zip_archive *pZip, mz_zip_archive *pSource_zip, mz_uint src_file_index);

    /* Finalizes the archive by writing the central directory records followed by the end of central directory record. */
    /* After an archive is finalized, the only valid call on the mz_zip_archive struct is mz_zip_writer_end(). */
    /* An archive must be manually finalized by calling this function for it to be valid. */
    MINIZ_EXPORT mz_bool mz_zip_writer_finalize_archive(mz_zip_archive *pZip);

    /* Finalizes a heap archive, returning a pointer to the heap block and its size. */
    /* The heap block will be allocated using the mz_zip_archive's alloc/realloc callbacks. */
    MINIZ_EXPORT mz_bool mz_zip_writer_finalize_heap_archive(mz_zip_archive *pZip, void **ppBuf, size_t *pSize);

    /* Ends archive writing, freeing all allocations, and closing the output file if mz_zip_writer_init_file() was used. */
    /* Note for the archive to be valid, it *must* have been finalized before ending (this function will not do it for you). */
    MINIZ_EXPORT mz_bool mz_zip_writer_end(mz_zip_archive *pZip);

    /* -------- Misc. high-level helper functions: */

    /* mz_zip_add_mem_to_archive_file_in_place() efficiently (but not atomically) appends a memory blob to a ZIP archive. */
    /* Note this is NOT a fully safe operation. If it crashes or dies in some way your archive can be left in a screwed up state (without a central directory). */
    /* level_and_flags - compression level (0-10, see MZ_BEST_SPEED, MZ_BEST_COMPRESSION, etc.) logically OR'd with zero or more mz_zip_flags, or just set to MZ_DEFAULT_COMPRESSION. */
    /* TODO: Perhaps add an option to leave the existing central dir in place in case the add dies? We could then truncate the file (so the old central dir would be at the end) if something goes wrong. */
    MINIZ_EXPORT mz_bool mz_zip_add_mem_to_archive_file_in_place(const char *pZip_filename, const char *pArchive_name, const void *pBuf, size_t buf_size, const void *pComment, mz_uint16 comment_size, mz_uint level_and_flags);
    MINIZ_EXPORT mz_bool mz_zip_add_mem_to_archive_file_in_place_v2(const char *pZip_filename, const char *pArchive_name, const void *pBuf, size_t buf_size, const void *pComment, mz_uint16 comment_size, mz_uint level_and_flags, mz_zip_error *pErr);

#ifndef MINIZ_NO_STDIO
    /* Reads a single file from an archive into a heap block. */
    /* If pComment is not NULL, only the file with the specified comment will be extracted. */
    /* Returns NULL on failure. */
    MINIZ_EXPORT void *mz_zip_extract_archive_file_to_heap(const char *pZip_filename, const char *pArchive_name, size_t *pSize, mz_uint flags);
    MINIZ_EXPORT void *mz_zip_extract_archive_file_to_heap_v2(const char *pZip_filename, const char *pArchive_name, const char *pComment, size_t *pSize, mz_uint flags, mz_zip_error *pErr);
#endif

#endif /* #ifndef MINIZ_NO_ARCHIVE_WRITING_APIS */

#ifdef __cplusplus
}
#endif

#endif /* MINIZ_NO_ARCHIVE_APIS */


/*
file https://github.com/richgel999/miniz/blob/3.1.0/miniz.c
*/

#if defined(SRC_SQLMATH_BASE_C2)
/**************************************************************************
 *
 * Copyright 2013-2014 RAD Game Tools and Valve Software
 * Copyright 2010-2014 Rich Geldreich and Tenacious Software LLC
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 **************************************************************************/

// hack-miniz - inline header
// #include "miniz.h"

typedef unsigned char mz_validate_uint16[sizeof(mz_uint16) == 2 ? 1 : -1];
typedef unsigned char mz_validate_uint32[sizeof(mz_uint32) == 4 ? 1 : -1];
typedef unsigned char mz_validate_uint64[sizeof(mz_uint64) == 8 ? 1 : -1];

#ifdef __cplusplus
extern "C"
{
#endif

    /* ------------------- zlib-style API's */

    mz_ulong mz_adler32(mz_ulong adler, const unsigned char *ptr, size_t buf_len)
    {
        mz_uint32 i, s1 = (mz_uint32)(adler & 0xffff), s2 = (mz_uint32)(adler >> 16);
        size_t block_len = buf_len % 5552;
        if (!ptr)
            return MZ_ADLER32_INIT;
        while (buf_len)
        {
            for (i = 0; i + 7 < block_len; i += 8, ptr += 8)
            {
                s1 += ptr[0], s2 += s1;
                s1 += ptr[1], s2 += s1;
                s1 += ptr[2], s2 += s1;
                s1 += ptr[3], s2 += s1;
                s1 += ptr[4], s2 += s1;
                s1 += ptr[5], s2 += s1;
                s1 += ptr[6], s2 += s1;
                s1 += ptr[7], s2 += s1;
            }
            for (; i < block_len; ++i)
                s1 += *ptr++, s2 += s1;
            s1 %= 65521U, s2 %= 65521U;
            buf_len -= block_len;
            block_len = 5552;
        }
        return (s2 << 16) + s1;
    }

/* Karl Malbrain's compact CRC-32. See "A compact CCITT crc16 and crc32 C implementation that balances processor cache usage against speed": http://www.geocities.com/malbrain/ */
#if 0
    mz_ulong mz_crc32(mz_ulong crc, const mz_uint8 *ptr, size_t buf_len)
    {
        static const mz_uint32 s_crc32[16] = { 0, 0x1db71064, 0x3b6e20c8, 0x26d930ac, 0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
                                               0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c, 0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c };
        mz_uint32 crcu32 = (mz_uint32)crc;
        if (!ptr)
            return MZ_CRC32_INIT;
        crcu32 = ~crcu32;
        while (buf_len--)
        {
            mz_uint8 b = *ptr++;
            crcu32 = (crcu32 >> 4) ^ s_crc32[(crcu32 & 0xF) ^ (b & 0xF)];
            crcu32 = (crcu32 >> 4) ^ s_crc32[(crcu32 & 0xF) ^ (b >> 4)];
        }
        return ~crcu32;
    }
#elif defined(USE_EXTERNAL_MZCRC)
/* If USE_EXTERNAL_CRC is defined, an external module will export the
 * mz_crc32() symbol for us to use, e.g. an SSE-accelerated version.
 * Depending on the impl, it may be necessary to ~ the input/output crc values.
 */
mz_ulong mz_crc32(mz_ulong crc, const mz_uint8 *ptr, size_t buf_len);
#else
/* Faster, but larger CPU cache footprint.
 */
mz_ulong mz_crc32(mz_ulong crc, const mz_uint8 *ptr, size_t buf_len)
{
    static const mz_uint32 s_crc_table[256] = {
        0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA, 0x076DC419, 0x706AF48F, 0xE963A535,
        0x9E6495A3, 0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988, 0x09B64C2B, 0x7EB17CBD,
        0xE7B82D07, 0x90BF1D91, 0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE, 0x1ADAD47D,
        0x6DDDE4EB, 0xF4D4B551, 0x83D385C7, 0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC,
        0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5, 0x3B6E20C8, 0x4C69105E, 0xD56041E4,
        0xA2677172, 0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B, 0x35B5A8FA, 0x42B2986C,
        0xDBBBC9D6, 0xACBCF940, 0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59, 0x26D930AC,
        0x51DE003A, 0xC8D75180, 0xBFD06116, 0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F,
        0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924, 0x2F6F7C87, 0x58684C11, 0xC1611DAB,
        0xB6662D3D, 0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A, 0x71B18589, 0x06B6B51F,
        0x9FBFE4A5, 0xE8B8D433, 0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818, 0x7F6A0DBB,
        0x086D3D2D, 0x91646C97, 0xE6635C01, 0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E,
        0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457, 0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA,
        0xFCB9887C, 0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65, 0x4DB26158, 0x3AB551CE,
        0xA3BC0074, 0xD4BB30E2, 0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB, 0x4369E96A,
        0x346ED9FC, 0xAD678846, 0xDA60B8D0, 0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,
        0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086, 0x5768B525, 0x206F85B3, 0xB966D409,
        0xCE61E49F, 0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4, 0x59B33D17, 0x2EB40D81,
        0xB7BD5C3B, 0xC0BA6CAD, 0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A, 0xEAD54739,
        0x9DD277AF, 0x04DB2615, 0x73DC1683, 0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8,
        0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1, 0xF00F9344, 0x8708A3D2, 0x1E01F268,
        0x6906C2FE, 0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7, 0xFED41B76, 0x89D32BE0,
        0x10DA7A5A, 0x67DD4ACC, 0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5, 0xD6D6A3E8,
        0xA1D1937E, 0x38D8C2C4, 0x4FDFF252, 0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
        0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60, 0xDF60EFC3, 0xA867DF55, 0x316E8EEF,
        0x4669BE79, 0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236, 0xCC0C7795, 0xBB0B4703,
        0x220216B9, 0x5505262F, 0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04, 0xC2D7FFA7,
        0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D, 0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A,
        0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713, 0x95BF4A82, 0xE2B87A14, 0x7BB12BAE,
        0x0CB61B38, 0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21, 0x86D3D2D4, 0xF1D4E242,
        0x68DDB3F8, 0x1FDA836E, 0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777, 0x88085AE6,
        0xFF0F6A70, 0x66063BCA, 0x11010B5C, 0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45,
        0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2, 0xA7672661, 0xD06016F7, 0x4969474D,
        0x3E6E77DB, 0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0, 0xA9BCAE53, 0xDEBB9EC5,
        0x47B2CF7F, 0x30B5FFE9, 0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6, 0xBAD03605,
        0xCDD70693, 0x54DE5729, 0x23D967BF, 0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94,
        0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D
    };

    mz_uint32 crc32 = (mz_uint32)crc ^ 0xFFFFFFFF;
    const mz_uint8 *pByte_buf = (const mz_uint8 *)ptr;

    while (buf_len >= 4)
    {
        crc32 = (crc32 >> 8) ^ s_crc_table[(crc32 ^ pByte_buf[0]) & 0xFF];
        crc32 = (crc32 >> 8) ^ s_crc_table[(crc32 ^ pByte_buf[1]) & 0xFF];
        crc32 = (crc32 >> 8) ^ s_crc_table[(crc32 ^ pByte_buf[2]) & 0xFF];
        crc32 = (crc32 >> 8) ^ s_crc_table[(crc32 ^ pByte_buf[3]) & 0xFF];
        pByte_buf += 4;
        buf_len -= 4;
    }

    while (buf_len)
    {
        crc32 = (crc32 >> 8) ^ s_crc_table[(crc32 ^ pByte_buf[0]) & 0xFF];
        ++pByte_buf;
        --buf_len;
    }

    return ~crc32;
}
#endif

    void mz_free(void *p)
    {
        MZ_FREE(p);
    }

    MINIZ_EXPORT void *miniz_def_alloc_func(void *opaque, size_t items, size_t size)
    {
        (void)opaque, (void)items, (void)size;
        return MZ_MALLOC(items * size);
    }
    MINIZ_EXPORT void miniz_def_free_func(void *opaque, void *address)
    {
        (void)opaque, (void)address;
        MZ_FREE(address);
    }
    MINIZ_EXPORT void *miniz_def_realloc_func(void *opaque, void *address, size_t items, size_t size)
    {
        (void)opaque, (void)address, (void)items, (void)size;
        return MZ_REALLOC(address, items * size);
    }

    const char *mz_version(void)
    {
        return MZ_VERSION;
    }

#ifndef MINIZ_NO_ZLIB_APIS

#ifndef MINIZ_NO_DEFLATE_APIS

    int mz_deflateInit(mz_streamp pStream, int level)
    {
        return mz_deflateInit2(pStream, level, MZ_DEFLATED, MZ_DEFAULT_WINDOW_BITS, 9, MZ_DEFAULT_STRATEGY);
    }

    int mz_deflateInit2(mz_streamp pStream, int level, int method, int window_bits, int mem_level, int strategy)
    {
        tdefl_compressor *pComp;
        mz_uint comp_flags = TDEFL_COMPUTE_ADLER32 | tdefl_create_comp_flags_from_zip_params(level, window_bits, strategy);

        if (!pStream)
            return MZ_STREAM_ERROR;
        if ((method != MZ_DEFLATED) || ((mem_level < 1) || (mem_level > 9)) || ((window_bits != MZ_DEFAULT_WINDOW_BITS) && (-window_bits != MZ_DEFAULT_WINDOW_BITS)))
            return MZ_PARAM_ERROR;

        pStream->data_type = 0;
        pStream->adler = MZ_ADLER32_INIT;
        pStream->msg = NULL;
        pStream->reserved = 0;
        pStream->total_in = 0;
        pStream->total_out = 0;
        if (!pStream->zalloc)
            pStream->zalloc = miniz_def_alloc_func;
        if (!pStream->zfree)
            pStream->zfree = miniz_def_free_func;

        pComp = (tdefl_compressor *)pStream->zalloc(pStream->opaque, 1, sizeof(tdefl_compressor));
        if (!pComp)
            return MZ_MEM_ERROR;

        pStream->state = (struct mz_internal_state *)pComp;

        if (tdefl_init(pComp, NULL, NULL, comp_flags) != TDEFL_STATUS_OKAY)
        {
            mz_deflateEnd(pStream);
            return MZ_PARAM_ERROR;
        }

        return MZ_OK;
    }

    int mz_deflateReset(mz_streamp pStream)
    {
        if ((!pStream) || (!pStream->state) || (!pStream->zalloc) || (!pStream->zfree))
            return MZ_STREAM_ERROR;
        pStream->total_in = pStream->total_out = 0;
        tdefl_init((tdefl_compressor *)pStream->state, NULL, NULL, ((tdefl_compressor *)pStream->state)->m_flags);
        return MZ_OK;
    }

    int mz_deflate(mz_streamp pStream, int flush)
    {
        size_t in_bytes, out_bytes;
        mz_ulong orig_total_in, orig_total_out;
        int mz_status = MZ_OK;

        if ((!pStream) || (!pStream->state) || (flush < 0) || (flush > MZ_FINISH) || (!pStream->next_out))
            return MZ_STREAM_ERROR;
        if (!pStream->avail_out)
            return MZ_BUF_ERROR;

        if (flush == MZ_PARTIAL_FLUSH)
            flush = MZ_SYNC_FLUSH;

        if (((tdefl_compressor *)pStream->state)->m_prev_return_status == TDEFL_STATUS_DONE)
            return (flush == MZ_FINISH) ? MZ_STREAM_END : MZ_BUF_ERROR;

        orig_total_in = pStream->total_in;
        orig_total_out = pStream->total_out;
        for (;;)
        {
            tdefl_status defl_status;
            in_bytes = pStream->avail_in;
            out_bytes = pStream->avail_out;

            defl_status = tdefl_compress((tdefl_compressor *)pStream->state, pStream->next_in, &in_bytes, pStream->next_out, &out_bytes, (tdefl_flush)flush);
            pStream->next_in += (mz_uint)in_bytes;
            pStream->avail_in -= (mz_uint)in_bytes;
            pStream->total_in += (mz_uint)in_bytes;
            pStream->adler = tdefl_get_adler32((tdefl_compressor *)pStream->state);

            pStream->next_out += (mz_uint)out_bytes;
            pStream->avail_out -= (mz_uint)out_bytes;
            pStream->total_out += (mz_uint)out_bytes;

            if (defl_status < 0)
            {
                mz_status = MZ_STREAM_ERROR;
                break;
            }
            else if (defl_status == TDEFL_STATUS_DONE)
            {
                mz_status = MZ_STREAM_END;
                break;
            }
            else if (!pStream->avail_out)
                break;
            else if ((!pStream->avail_in) && (flush != MZ_FINISH))
            {
                if ((flush) || (pStream->total_in != orig_total_in) || (pStream->total_out != orig_total_out))
                    break;
                return MZ_BUF_ERROR; /* Can't make forward progress without some input.
                                      */
            }
        }
        return mz_status;
    }

    int mz_deflateEnd(mz_streamp pStream)
    {
        if (!pStream)
            return MZ_STREAM_ERROR;
        if (pStream->state)
        {
            pStream->zfree(pStream->opaque, pStream->state);
            pStream->state = NULL;
        }
        return MZ_OK;
    }

    mz_ulong mz_deflateBound(mz_streamp pStream, mz_ulong source_len)
    {
        (void)pStream;
        /* This is really over conservative. (And lame, but it's actually pretty tricky to compute a true upper bound given the way tdefl's blocking works.) */
        return MZ_MAX(128 + (source_len * 110) / 100, 128 + source_len + ((source_len / (31 * 1024)) + 1) * 5);
    }

    int mz_compress2(unsigned char *pDest, mz_ulong *pDest_len, const unsigned char *pSource, mz_ulong source_len, int level)
    {
        int status;
        mz_stream stream;
        memset(&stream, 0, sizeof(stream));

        /* In case mz_ulong is 64-bits (argh I hate longs). */
        if ((mz_uint64)(source_len | *pDest_len) > 0xFFFFFFFFU)
            return MZ_PARAM_ERROR;

        stream.next_in = pSource;
        stream.avail_in = (mz_uint32)source_len;
        stream.next_out = pDest;
        stream.avail_out = (mz_uint32)*pDest_len;

        status = mz_deflateInit(&stream, level);
        if (status != MZ_OK)
            return status;

        status = mz_deflate(&stream, MZ_FINISH);
        if (status != MZ_STREAM_END)
        {
            mz_deflateEnd(&stream);
            return (status == MZ_OK) ? MZ_BUF_ERROR : status;
        }

        *pDest_len = stream.total_out;
        return mz_deflateEnd(&stream);
    }

    int mz_compress(unsigned char *pDest, mz_ulong *pDest_len, const unsigned char *pSource, mz_ulong source_len)
    {
        return mz_compress2(pDest, pDest_len, pSource, source_len, MZ_DEFAULT_COMPRESSION);
    }

    mz_ulong mz_compressBound(mz_ulong source_len)
    {
        return mz_deflateBound(NULL, source_len);
    }

#endif /*#ifndef MINIZ_NO_DEFLATE_APIS*/

#ifndef MINIZ_NO_INFLATE_APIS

    typedef struct
    {
        tinfl_decompressor m_decomp;
        mz_uint m_dict_ofs, m_dict_avail, m_first_call, m_has_flushed;
        int m_window_bits;
        mz_uint8 m_dict[TINFL_LZ_DICT_SIZE];
        tinfl_status m_last_status;
    } inflate_state;

    int mz_inflateInit2(mz_streamp pStream, int window_bits)
    {
        inflate_state *pDecomp;
        if (!pStream)
            return MZ_STREAM_ERROR;
        if ((window_bits != MZ_DEFAULT_WINDOW_BITS) && (-window_bits != MZ_DEFAULT_WINDOW_BITS))
            return MZ_PARAM_ERROR;

        pStream->data_type = 0;
        pStream->adler = 0;
        pStream->msg = NULL;
        pStream->total_in = 0;
        pStream->total_out = 0;
        pStream->reserved = 0;
        if (!pStream->zalloc)
            pStream->zalloc = miniz_def_alloc_func;
        if (!pStream->zfree)
            pStream->zfree = miniz_def_free_func;

        pDecomp = (inflate_state *)pStream->zalloc(pStream->opaque, 1, sizeof(inflate_state));
        if (!pDecomp)
            return MZ_MEM_ERROR;

        pStream->state = (struct mz_internal_state *)pDecomp;

        tinfl_init(&pDecomp->m_decomp);
        pDecomp->m_dict_ofs = 0;
        pDecomp->m_dict_avail = 0;
        pDecomp->m_last_status = TINFL_STATUS_NEEDS_MORE_INPUT;
        pDecomp->m_first_call = 1;
        pDecomp->m_has_flushed = 0;
        pDecomp->m_window_bits = window_bits;

        return MZ_OK;
    }

    int mz_inflateInit(mz_streamp pStream)
    {
        return mz_inflateInit2(pStream, MZ_DEFAULT_WINDOW_BITS);
    }

    int mz_inflateReset(mz_streamp pStream)
    {
        inflate_state *pDecomp;
        if (!pStream)
            return MZ_STREAM_ERROR;

        pStream->data_type = 0;
        pStream->adler = 0;
        pStream->msg = NULL;
        pStream->total_in = 0;
        pStream->total_out = 0;
        pStream->reserved = 0;

        pDecomp = (inflate_state *)pStream->state;

        tinfl_init(&pDecomp->m_decomp);
        pDecomp->m_dict_ofs = 0;
        pDecomp->m_dict_avail = 0;
        pDecomp->m_last_status = TINFL_STATUS_NEEDS_MORE_INPUT;
        pDecomp->m_first_call = 1;
        pDecomp->m_has_flushed = 0;
        /* pDecomp->m_window_bits = window_bits */;

        return MZ_OK;
    }

    int mz_inflate(mz_streamp pStream, int flush)
    {
        inflate_state *pState;
        mz_uint n, first_call, decomp_flags = TINFL_FLAG_COMPUTE_ADLER32;
        size_t in_bytes, out_bytes, orig_avail_in;
        tinfl_status status;

        if ((!pStream) || (!pStream->state))
            return MZ_STREAM_ERROR;
        if (flush == MZ_PARTIAL_FLUSH)
            flush = MZ_SYNC_FLUSH;
        if ((flush) && (flush != MZ_SYNC_FLUSH) && (flush != MZ_FINISH))
            return MZ_STREAM_ERROR;

        pState = (inflate_state *)pStream->state;
        if (pState->m_window_bits > 0)
            decomp_flags |= TINFL_FLAG_PARSE_ZLIB_HEADER;
        orig_avail_in = pStream->avail_in;

        first_call = pState->m_first_call;
        pState->m_first_call = 0;
        if (pState->m_last_status < 0)
            return MZ_DATA_ERROR;

        if (pState->m_has_flushed && (flush != MZ_FINISH))
            return MZ_STREAM_ERROR;
        pState->m_has_flushed |= (flush == MZ_FINISH);

        if ((flush == MZ_FINISH) && (first_call))
        {
            /* MZ_FINISH on the first call implies that the input and output buffers are large enough to hold the entire compressed/decompressed file. */
            decomp_flags |= TINFL_FLAG_USING_NON_WRAPPING_OUTPUT_BUF;
            in_bytes = pStream->avail_in;
            out_bytes = pStream->avail_out;
            status = tinfl_decompress(&pState->m_decomp, pStream->next_in, &in_bytes, pStream->next_out, pStream->next_out, &out_bytes, decomp_flags);
            pState->m_last_status = status;
            pStream->next_in += (mz_uint)in_bytes;
            pStream->avail_in -= (mz_uint)in_bytes;
            pStream->total_in += (mz_uint)in_bytes;
            pStream->adler = tinfl_get_adler32(&pState->m_decomp);
            pStream->next_out += (mz_uint)out_bytes;
            pStream->avail_out -= (mz_uint)out_bytes;
            pStream->total_out += (mz_uint)out_bytes;

            if (status < 0)
                return MZ_DATA_ERROR;
            else if (status != TINFL_STATUS_DONE)
            {
                pState->m_last_status = TINFL_STATUS_FAILED;
                return MZ_BUF_ERROR;
            }
            return MZ_STREAM_END;
        }
        /* flush != MZ_FINISH then we must assume there's more input. */
        if (flush != MZ_FINISH)
            decomp_flags |= TINFL_FLAG_HAS_MORE_INPUT;

        if (pState->m_dict_avail)
        {
            n = MZ_MIN(pState->m_dict_avail, pStream->avail_out);
            memcpy(pStream->next_out, pState->m_dict + pState->m_dict_ofs, n);
            pStream->next_out += n;
            pStream->avail_out -= n;
            pStream->total_out += n;
            pState->m_dict_avail -= n;
            pState->m_dict_ofs = (pState->m_dict_ofs + n) & (TINFL_LZ_DICT_SIZE - 1);
            return ((pState->m_last_status == TINFL_STATUS_DONE) && (!pState->m_dict_avail)) ? MZ_STREAM_END : MZ_OK;
        }

        for (;;)
        {
            in_bytes = pStream->avail_in;
            out_bytes = TINFL_LZ_DICT_SIZE - pState->m_dict_ofs;

            status = tinfl_decompress(&pState->m_decomp, pStream->next_in, &in_bytes, pState->m_dict, pState->m_dict + pState->m_dict_ofs, &out_bytes, decomp_flags);
            pState->m_last_status = status;

            pStream->next_in += (mz_uint)in_bytes;
            pStream->avail_in -= (mz_uint)in_bytes;
            pStream->total_in += (mz_uint)in_bytes;
            pStream->adler = tinfl_get_adler32(&pState->m_decomp);

            pState->m_dict_avail = (mz_uint)out_bytes;

            n = MZ_MIN(pState->m_dict_avail, pStream->avail_out);
            memcpy(pStream->next_out, pState->m_dict + pState->m_dict_ofs, n);
            pStream->next_out += n;
            pStream->avail_out -= n;
            pStream->total_out += n;
            pState->m_dict_avail -= n;
            pState->m_dict_ofs = (pState->m_dict_ofs + n) & (TINFL_LZ_DICT_SIZE - 1);

            if (status < 0)
                return MZ_DATA_ERROR; /* Stream is corrupted (there could be some uncompressed data left in the output dictionary - oh well). */
            else if ((status == TINFL_STATUS_NEEDS_MORE_INPUT) && (!orig_avail_in))
                return MZ_BUF_ERROR; /* Signal caller that we can't make forward progress without supplying more input or by setting flush to MZ_FINISH. */
            else if (flush == MZ_FINISH)
            {
                /* The output buffer MUST be large to hold the remaining uncompressed data when flush==MZ_FINISH. */
                if (status == TINFL_STATUS_DONE)
                    return pState->m_dict_avail ? MZ_BUF_ERROR : MZ_STREAM_END;
                /* status here must be TINFL_STATUS_HAS_MORE_OUTPUT, which means there's at least 1 more byte on the way. If there's no more room left in the output buffer then something is wrong. */
                else if (!pStream->avail_out)
                    return MZ_BUF_ERROR;
            }
            else if ((status == TINFL_STATUS_DONE) || (!pStream->avail_in) || (!pStream->avail_out) || (pState->m_dict_avail))
                break;
        }

        return ((status == TINFL_STATUS_DONE) && (!pState->m_dict_avail)) ? MZ_STREAM_END : MZ_OK;
    }

    int mz_inflateEnd(mz_streamp pStream)
    {
        if (!pStream)
            return MZ_STREAM_ERROR;
        if (pStream->state)
        {
            pStream->zfree(pStream->opaque, pStream->state);
            pStream->state = NULL;
        }
        return MZ_OK;
    }
    int mz_uncompress2(unsigned char *pDest, mz_ulong *pDest_len, const unsigned char *pSource, mz_ulong *pSource_len)
    {
        mz_stream stream;
        int status;
        memset(&stream, 0, sizeof(stream));

        /* In case mz_ulong is 64-bits (argh I hate longs). */
        if ((mz_uint64)(*pSource_len | *pDest_len) > 0xFFFFFFFFU)
            return MZ_PARAM_ERROR;

        stream.next_in = pSource;
        stream.avail_in = (mz_uint32)*pSource_len;
        stream.next_out = pDest;
        stream.avail_out = (mz_uint32)*pDest_len;

        status = mz_inflateInit(&stream);
        if (status != MZ_OK)
            return status;

        status = mz_inflate(&stream, MZ_FINISH);
        *pSource_len = *pSource_len - stream.avail_in;
        if (status != MZ_STREAM_END)
        {
            mz_inflateEnd(&stream);
            return ((status == MZ_BUF_ERROR) && (!stream.avail_in)) ? MZ_DATA_ERROR : status;
        }
        *pDest_len = stream.total_out;

        return mz_inflateEnd(&stream);
    }

    int mz_uncompress(unsigned char *pDest, mz_ulong *pDest_len, const unsigned char *pSource, mz_ulong source_len)
    {
        return mz_uncompress2(pDest, pDest_len, pSource, &source_len);
    }

#endif /*#ifndef MINIZ_NO_INFLATE_APIS*/

    const char *mz_error(int err)
    {
        static struct
        {
            int m_err;
            const char *m_pDesc;
        } s_error_descs[] = {
            { MZ_OK, "" }, { MZ_STREAM_END, "stream end" }, { MZ_NEED_DICT, "need dictionary" }, { MZ_ERRNO, "file error" }, { MZ_STREAM_ERROR, "stream error" }, { MZ_DATA_ERROR, "data error" }, { MZ_MEM_ERROR, "out of memory" }, { MZ_BUF_ERROR, "buf error" }, { MZ_VERSION_ERROR, "version error" }, { MZ_PARAM_ERROR, "parameter error" }
        };
        mz_uint i;
        for (i = 0; i < sizeof(s_error_descs) / sizeof(s_error_descs[0]); ++i)
            if (s_error_descs[i].m_err == err)
                return s_error_descs[i].m_pDesc;
        return NULL;
    }

#endif /*MINIZ_NO_ZLIB_APIS */

#ifdef __cplusplus
}
#endif

/*
  This is free and unencumbered software released into the public domain.

  Anyone is free to copy, modify, publish, use, compile, sell, or
  distribute this software, either in source code form or as a compiled
  binary, for any purpose, commercial or non-commercial, and by any
  means.

  In jurisdictions that recognize copyright laws, the author or authors
  of this software dedicate any and all copyright interest in the
  software to the public domain. We make this dedication for the benefit
  of the public at large and to the detriment of our heirs and
  successors. We intend this dedication to be an overt act of
  relinquishment in perpetuity of all present and future rights to this
  software under copyright law.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
  IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
  OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
  OTHER DEALINGS IN THE SOFTWARE.

  For more information, please refer to <http://unlicense.org/>
*/


/*
file https://github.com/richgel999/miniz/blob/3.1.0/miniz_tdef.c
*/
/**************************************************************************
 *
 * Copyright 2013-2014 RAD Game Tools and Valve Software
 * Copyright 2010-2014 Rich Geldreich and Tenacious Software LLC
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 **************************************************************************/

// hack-miniz - inline header
// #include "miniz.h"

#ifndef MINIZ_NO_DEFLATE_APIS

#ifdef __cplusplus
extern "C"
{
#endif

    /* ------------------- Low-level Compression (independent from all decompression API's) */

    /* Purposely making these tables static for faster init and thread safety. */
    static const mz_uint16 s_tdefl_len_sym[256] = {
        257, 258, 259, 260, 261, 262, 263, 264, 265, 265, 266, 266, 267, 267, 268, 268, 269, 269, 269, 269, 270, 270, 270, 270, 271, 271, 271, 271, 272, 272, 272, 272,
        273, 273, 273, 273, 273, 273, 273, 273, 274, 274, 274, 274, 274, 274, 274, 274, 275, 275, 275, 275, 275, 275, 275, 275, 276, 276, 276, 276, 276, 276, 276, 276,
        277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 278, 278, 278, 278, 278, 278, 278, 278, 278, 278, 278, 278, 278, 278, 278, 278,
        279, 279, 279, 279, 279, 279, 279, 279, 279, 279, 279, 279, 279, 279, 279, 279, 280, 280, 280, 280, 280, 280, 280, 280, 280, 280, 280, 280, 280, 280, 280, 280,
        281, 281, 281, 281, 281, 281, 281, 281, 281, 281, 281, 281, 281, 281, 281, 281, 281, 281, 281, 281, 281, 281, 281, 281, 281, 281, 281, 281, 281, 281, 281, 281,
        282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282,
        283, 283, 283, 283, 283, 283, 283, 283, 283, 283, 283, 283, 283, 283, 283, 283, 283, 283, 283, 283, 283, 283, 283, 283, 283, 283, 283, 283, 283, 283, 283, 283,
        284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 285
    };

    static const mz_uint8 s_tdefl_len_extra[256] = {
        0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
        5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
        5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0
    };

    static const mz_uint8 s_tdefl_small_dist_sym[512] = {
        0, 1, 2, 3, 4, 4, 5, 5, 6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8, 9, 9, 9, 9, 9, 9, 9, 9, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 11, 11, 11, 11, 11, 11,
        11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 13,
        13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
        14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
        14, 14, 14, 14, 14, 14, 14, 14, 14, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
        15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
        16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
        16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
        16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
        17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
        17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
        17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17
    };

    static const mz_uint8 s_tdefl_small_dist_extra[512] = {
        0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5,
        5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
        6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
        6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
        7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
        7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
        7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
        7, 7, 7, 7, 7, 7, 7, 7
    };

    static const mz_uint8 s_tdefl_large_dist_sym[128] = {
        0, 0, 18, 19, 20, 20, 21, 21, 22, 22, 22, 22, 23, 23, 23, 23, 24, 24, 24, 24, 24, 24, 24, 24, 25, 25, 25, 25, 25, 25, 25, 25, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26,
        26, 26, 26, 26, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
        28, 28, 28, 28, 28, 28, 28, 28, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29
    };

    static const mz_uint8 s_tdefl_large_dist_extra[128] = {
        0, 0, 8, 8, 9, 9, 9, 9, 10, 10, 10, 10, 10, 10, 10, 10, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
        12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
        13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13
    };

    /* Radix sorts tdefl_sym_freq[] array by 16-bit key m_key. Returns ptr to sorted values. */
    typedef struct
    {
        mz_uint16 m_key, m_sym_index;
    } tdefl_sym_freq;
    static tdefl_sym_freq *tdefl_radix_sort_syms(mz_uint num_syms, tdefl_sym_freq *pSyms0, tdefl_sym_freq *pSyms1)
    {
        mz_uint32 total_passes = 2, pass_shift, pass, i, hist[256 * 2];
        tdefl_sym_freq *pCur_syms = pSyms0, *pNew_syms = pSyms1;
        MZ_CLEAR_ARR(hist);
        for (i = 0; i < num_syms; i++)
        {
            mz_uint freq = pSyms0[i].m_key;
            hist[freq & 0xFF]++;
            hist[256 + ((freq >> 8) & 0xFF)]++;
        }
        while ((total_passes > 1) && (num_syms == hist[(total_passes - 1) * 256]))
            total_passes--;
        for (pass_shift = 0, pass = 0; pass < total_passes; pass++, pass_shift += 8)
        {
            const mz_uint32 *pHist = &hist[pass << 8];
            mz_uint offsets[256], cur_ofs = 0;
            for (i = 0; i < 256; i++)
            {
                offsets[i] = cur_ofs;
                cur_ofs += pHist[i];
            }
            for (i = 0; i < num_syms; i++)
                pNew_syms[offsets[(pCur_syms[i].m_key >> pass_shift) & 0xFF]++] = pCur_syms[i];
            {
                tdefl_sym_freq *t = pCur_syms;
                pCur_syms = pNew_syms;
                pNew_syms = t;
            }
        }
        return pCur_syms;
    }

    /* tdefl_calculate_minimum_redundancy() originally written by: Alistair Moffat, alistair@cs.mu.oz.au, Jyrki Katajainen, jyrki@diku.dk, November 1996. */
    static void tdefl_calculate_minimum_redundancy(tdefl_sym_freq *A, int n)
    {
        int root, leaf, next, avbl, used, dpth;
        if (n == 0)
            return;
        else if (n == 1)
        {
            A[0].m_key = 1;
            return;
        }
        A[0].m_key += A[1].m_key;
        root = 0;
        leaf = 2;
        for (next = 1; next < n - 1; next++)
        {
            if (leaf >= n || A[root].m_key < A[leaf].m_key)
            {
                A[next].m_key = A[root].m_key;
                A[root++].m_key = (mz_uint16)next;
            }
            else
                A[next].m_key = A[leaf++].m_key;
            if (leaf >= n || (root < next && A[root].m_key < A[leaf].m_key))
            {
                A[next].m_key = (mz_uint16)(A[next].m_key + A[root].m_key);
                A[root++].m_key = (mz_uint16)next;
            }
            else
                A[next].m_key = (mz_uint16)(A[next].m_key + A[leaf++].m_key);
        }
        A[n - 2].m_key = 0;
        for (next = n - 3; next >= 0; next--)
            A[next].m_key = A[A[next].m_key].m_key + 1;
        avbl = 1;
        used = dpth = 0;
        root = n - 2;
        next = n - 1;
        while (avbl > 0)
        {
            while (root >= 0 && (int)A[root].m_key == dpth)
            {
                used++;
                root--;
            }
            while (avbl > used)
            {
                A[next--].m_key = (mz_uint16)(dpth);
                avbl--;
            }
            avbl = 2 * used;
            dpth++;
            used = 0;
        }
    }

    /* Limits canonical Huffman code table's max code size. */
    enum
    {
        TDEFL_MAX_SUPPORTED_HUFF_CODESIZE = 32
    };
    static void tdefl_huffman_enforce_max_code_size(int *pNum_codes, int code_list_len, int max_code_size)
    {
        int i;
        mz_uint32 total = 0;
        if (code_list_len <= 1)
            return;
        for (i = max_code_size + 1; i <= TDEFL_MAX_SUPPORTED_HUFF_CODESIZE; i++)
            pNum_codes[max_code_size] += pNum_codes[i];
        for (i = max_code_size; i > 0; i--)
            total += (((mz_uint32)pNum_codes[i]) << (max_code_size - i));
        while (total != (1UL << max_code_size))
        {
            pNum_codes[max_code_size]--;
            for (i = max_code_size - 1; i > 0; i--)
                if (pNum_codes[i])
                {
                    pNum_codes[i]--;
                    pNum_codes[i + 1] += 2;
                    break;
                }
            total--;
        }
    }

    static void tdefl_optimize_huffman_table(tdefl_compressor *d, int table_num, int table_len, int code_size_limit, int static_table)
    {
        int i, j, l, num_codes[1 + TDEFL_MAX_SUPPORTED_HUFF_CODESIZE];
        mz_uint next_code[TDEFL_MAX_SUPPORTED_HUFF_CODESIZE + 1];
        MZ_CLEAR_ARR(num_codes);
        if (static_table)
        {
            for (i = 0; i < table_len; i++)
                num_codes[d->m_huff_code_sizes[table_num][i]]++;
        }
        else
        {
            tdefl_sym_freq syms0[TDEFL_MAX_HUFF_SYMBOLS], syms1[TDEFL_MAX_HUFF_SYMBOLS], *pSyms;
            int num_used_syms = 0;
            const mz_uint16 *pSym_count = &d->m_huff_count[table_num][0];
            for (i = 0; i < table_len; i++)
                if (pSym_count[i])
                {
                    syms0[num_used_syms].m_key = (mz_uint16)pSym_count[i];
                    syms0[num_used_syms++].m_sym_index = (mz_uint16)i;
                }

            pSyms = tdefl_radix_sort_syms(num_used_syms, syms0, syms1);
            tdefl_calculate_minimum_redundancy(pSyms, num_used_syms);

            for (i = 0; i < num_used_syms; i++)
                num_codes[pSyms[i].m_key]++;

            tdefl_huffman_enforce_max_code_size(num_codes, num_used_syms, code_size_limit);

            MZ_CLEAR_ARR(d->m_huff_code_sizes[table_num]);
            MZ_CLEAR_ARR(d->m_huff_codes[table_num]);
            for (i = 1, j = num_used_syms; i <= code_size_limit; i++)
                for (l = num_codes[i]; l > 0; l--)
                    d->m_huff_code_sizes[table_num][pSyms[--j].m_sym_index] = (mz_uint8)(i);
        }

        next_code[1] = 0;
        for (j = 0, i = 2; i <= code_size_limit; i++)
            next_code[i] = j = ((j + num_codes[i - 1]) << 1);

        for (i = 0; i < table_len; i++)
        {
            mz_uint rev_code = 0, code, code_size;
            if ((code_size = d->m_huff_code_sizes[table_num][i]) == 0)
                continue;
            code = next_code[code_size]++;
            for (l = code_size; l > 0; l--, code >>= 1)
                rev_code = (rev_code << 1) | (code & 1);
            d->m_huff_codes[table_num][i] = (mz_uint16)rev_code;
        }
    }

#define TDEFL_PUT_BITS(b, l)                                       \
    do                                                             \
    {                                                              \
        mz_uint bits = b;                                          \
        mz_uint len = l;                                           \
        MZ_ASSERT(bits <= ((1U << len) - 1U));                     \
        d->m_bit_buffer |= (bits << d->m_bits_in);                 \
        d->m_bits_in += len;                                       \
        while (d->m_bits_in >= 8)                                  \
        {                                                          \
            if (d->m_pOutput_buf < d->m_pOutput_buf_end)           \
                *d->m_pOutput_buf++ = (mz_uint8)(d->m_bit_buffer); \
            d->m_bit_buffer >>= 8;                                 \
            d->m_bits_in -= 8;                                     \
        }                                                          \
    }                                                              \
    MZ_MACRO_END

#define TDEFL_RLE_PREV_CODE_SIZE()                                                                                       \
    {                                                                                                                    \
        if (rle_repeat_count)                                                                                            \
        {                                                                                                                \
            if (rle_repeat_count < 3)                                                                                    \
            {                                                                                                            \
                d->m_huff_count[2][prev_code_size] = (mz_uint16)(d->m_huff_count[2][prev_code_size] + rle_repeat_count); \
                while (rle_repeat_count--)                                                                               \
                    packed_code_sizes[num_packed_code_sizes++] = prev_code_size;                                         \
            }                                                                                                            \
            else                                                                                                         \
            {                                                                                                            \
                d->m_huff_count[2][16] = (mz_uint16)(d->m_huff_count[2][16] + 1);                                        \
                packed_code_sizes[num_packed_code_sizes++] = 16;                                                         \
                packed_code_sizes[num_packed_code_sizes++] = (mz_uint8)(rle_repeat_count - 3);                           \
            }                                                                                                            \
            rle_repeat_count = 0;                                                                                        \
        }                                                                                                                \
    }

#define TDEFL_RLE_ZERO_CODE_SIZE()                                                         \
    {                                                                                      \
        if (rle_z_count)                                                                   \
        {                                                                                  \
            if (rle_z_count < 3)                                                           \
            {                                                                              \
                d->m_huff_count[2][0] = (mz_uint16)(d->m_huff_count[2][0] + rle_z_count);  \
                while (rle_z_count--)                                                      \
                    packed_code_sizes[num_packed_code_sizes++] = 0;                        \
            }                                                                              \
            else if (rle_z_count <= 10)                                                    \
            {                                                                              \
                d->m_huff_count[2][17] = (mz_uint16)(d->m_huff_count[2][17] + 1);          \
                packed_code_sizes[num_packed_code_sizes++] = 17;                           \
                packed_code_sizes[num_packed_code_sizes++] = (mz_uint8)(rle_z_count - 3);  \
            }                                                                              \
            else                                                                           \
            {                                                                              \
                d->m_huff_count[2][18] = (mz_uint16)(d->m_huff_count[2][18] + 1);          \
                packed_code_sizes[num_packed_code_sizes++] = 18;                           \
                packed_code_sizes[num_packed_code_sizes++] = (mz_uint8)(rle_z_count - 11); \
            }                                                                              \
            rle_z_count = 0;                                                               \
        }                                                                                  \
    }

    static const mz_uint8 s_tdefl_packed_code_size_syms_swizzle[] = { 16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15 };

    static void tdefl_start_dynamic_block(tdefl_compressor *d)
    {
        int num_lit_codes, num_dist_codes, num_bit_lengths;
        mz_uint i, total_code_sizes_to_pack, num_packed_code_sizes, rle_z_count, rle_repeat_count, packed_code_sizes_index;
        mz_uint8 code_sizes_to_pack[TDEFL_MAX_HUFF_SYMBOLS_0 + TDEFL_MAX_HUFF_SYMBOLS_1], packed_code_sizes[TDEFL_MAX_HUFF_SYMBOLS_0 + TDEFL_MAX_HUFF_SYMBOLS_1], prev_code_size = 0xFF;

        d->m_huff_count[0][256] = 1;

        tdefl_optimize_huffman_table(d, 0, TDEFL_MAX_HUFF_SYMBOLS_0, 15, MZ_FALSE);
        tdefl_optimize_huffman_table(d, 1, TDEFL_MAX_HUFF_SYMBOLS_1, 15, MZ_FALSE);

        for (num_lit_codes = 286; num_lit_codes > 257; num_lit_codes--)
            if (d->m_huff_code_sizes[0][num_lit_codes - 1])
                break;
        for (num_dist_codes = 30; num_dist_codes > 1; num_dist_codes--)
            if (d->m_huff_code_sizes[1][num_dist_codes - 1])
                break;

        memcpy(code_sizes_to_pack, &d->m_huff_code_sizes[0][0], num_lit_codes);
        memcpy(code_sizes_to_pack + num_lit_codes, &d->m_huff_code_sizes[1][0], num_dist_codes);
        total_code_sizes_to_pack = num_lit_codes + num_dist_codes;
        num_packed_code_sizes = 0;
        rle_z_count = 0;
        rle_repeat_count = 0;

        memset(&d->m_huff_count[2][0], 0, sizeof(d->m_huff_count[2][0]) * TDEFL_MAX_HUFF_SYMBOLS_2);
        for (i = 0; i < total_code_sizes_to_pack; i++)
        {
            mz_uint8 code_size = code_sizes_to_pack[i];
            if (!code_size)
            {
                TDEFL_RLE_PREV_CODE_SIZE();
                if (++rle_z_count == 138)
                {
                    TDEFL_RLE_ZERO_CODE_SIZE();
                }
            }
            else
            {
                TDEFL_RLE_ZERO_CODE_SIZE();
                if (code_size != prev_code_size)
                {
                    TDEFL_RLE_PREV_CODE_SIZE();
                    d->m_huff_count[2][code_size] = (mz_uint16)(d->m_huff_count[2][code_size] + 1);
                    packed_code_sizes[num_packed_code_sizes++] = code_size;
                }
                else if (++rle_repeat_count == 6)
                {
                    TDEFL_RLE_PREV_CODE_SIZE();
                }
            }
            prev_code_size = code_size;
        }
        if (rle_repeat_count)
        {
            TDEFL_RLE_PREV_CODE_SIZE();
        }
        else
        {
            TDEFL_RLE_ZERO_CODE_SIZE();
        }

        tdefl_optimize_huffman_table(d, 2, TDEFL_MAX_HUFF_SYMBOLS_2, 7, MZ_FALSE);

        TDEFL_PUT_BITS(2, 2);

        TDEFL_PUT_BITS(num_lit_codes - 257, 5);
        TDEFL_PUT_BITS(num_dist_codes - 1, 5);

        for (num_bit_lengths = 18; num_bit_lengths >= 0; num_bit_lengths--)
            if (d->m_huff_code_sizes[2][s_tdefl_packed_code_size_syms_swizzle[num_bit_lengths]])
                break;
        num_bit_lengths = MZ_MAX(4, (num_bit_lengths + 1));
        TDEFL_PUT_BITS(num_bit_lengths - 4, 4);
        for (i = 0; (int)i < num_bit_lengths; i++)
            TDEFL_PUT_BITS(d->m_huff_code_sizes[2][s_tdefl_packed_code_size_syms_swizzle[i]], 3);

        for (packed_code_sizes_index = 0; packed_code_sizes_index < num_packed_code_sizes;)
        {
            mz_uint code = packed_code_sizes[packed_code_sizes_index++];
            MZ_ASSERT(code < TDEFL_MAX_HUFF_SYMBOLS_2);
            TDEFL_PUT_BITS(d->m_huff_codes[2][code], d->m_huff_code_sizes[2][code]);
            if (code >= 16)
                TDEFL_PUT_BITS(packed_code_sizes[packed_code_sizes_index++], "\02\03\07"[code - 16]);
        }
    }

    static void tdefl_start_static_block(tdefl_compressor *d)
    {
        mz_uint i;
        mz_uint8 *p = &d->m_huff_code_sizes[0][0];

        for (i = 0; i <= 143; ++i)
            *p++ = 8;
        for (; i <= 255; ++i)
            *p++ = 9;
        for (; i <= 279; ++i)
            *p++ = 7;
        for (; i <= 287; ++i)
            *p++ = 8;

        memset(d->m_huff_code_sizes[1], 5, 32);

        tdefl_optimize_huffman_table(d, 0, 288, 15, MZ_TRUE);
        tdefl_optimize_huffman_table(d, 1, 32, 15, MZ_TRUE);

        TDEFL_PUT_BITS(1, 2);
    }

    static const mz_uint mz_bitmasks[17] = { 0x0000, 0x0001, 0x0003, 0x0007, 0x000F, 0x001F, 0x003F, 0x007F, 0x00FF, 0x01FF, 0x03FF, 0x07FF, 0x0FFF, 0x1FFF, 0x3FFF, 0x7FFF, 0xFFFF };

#if MINIZ_USE_UNALIGNED_LOADS_AND_STORES && MINIZ_LITTLE_ENDIAN && MINIZ_HAS_64BIT_REGISTERS
    static mz_bool tdefl_compress_lz_codes(tdefl_compressor *d)
    {
        mz_uint flags;
        mz_uint8 *pLZ_codes;
        mz_uint8 *pOutput_buf = d->m_pOutput_buf;
        mz_uint8 *pLZ_code_buf_end = d->m_pLZ_code_buf;
        mz_uint64 bit_buffer = d->m_bit_buffer;
        mz_uint bits_in = d->m_bits_in;

#define TDEFL_PUT_BITS_FAST(b, l)                    \
    {                                                \
        bit_buffer |= (((mz_uint64)(b)) << bits_in); \
        bits_in += (l);                              \
    }

        flags = 1;
        for (pLZ_codes = d->m_lz_code_buf; pLZ_codes < pLZ_code_buf_end; flags >>= 1)
        {
            if (flags == 1)
                flags = *pLZ_codes++ | 0x100;

            if (flags & 1)
            {
                mz_uint s0, s1, n0, n1, sym, num_extra_bits;
                mz_uint match_len = pLZ_codes[0];
                mz_uint match_dist = (pLZ_codes[1] | (pLZ_codes[2] << 8));
                pLZ_codes += 3;

                MZ_ASSERT(d->m_huff_code_sizes[0][s_tdefl_len_sym[match_len]]);
                TDEFL_PUT_BITS_FAST(d->m_huff_codes[0][s_tdefl_len_sym[match_len]], d->m_huff_code_sizes[0][s_tdefl_len_sym[match_len]]);
                TDEFL_PUT_BITS_FAST(match_len & mz_bitmasks[s_tdefl_len_extra[match_len]], s_tdefl_len_extra[match_len]);

                /* This sequence coaxes MSVC into using cmov's vs. jmp's. */
                s0 = s_tdefl_small_dist_sym[match_dist & 511];
                n0 = s_tdefl_small_dist_extra[match_dist & 511];
                s1 = s_tdefl_large_dist_sym[match_dist >> 8];
                n1 = s_tdefl_large_dist_extra[match_dist >> 8];
                sym = (match_dist < 512) ? s0 : s1;
                num_extra_bits = (match_dist < 512) ? n0 : n1;

                MZ_ASSERT(d->m_huff_code_sizes[1][sym]);
                TDEFL_PUT_BITS_FAST(d->m_huff_codes[1][sym], d->m_huff_code_sizes[1][sym]);
                TDEFL_PUT_BITS_FAST(match_dist & mz_bitmasks[num_extra_bits], num_extra_bits);
            }
            else
            {
                mz_uint lit = *pLZ_codes++;
                MZ_ASSERT(d->m_huff_code_sizes[0][lit]);
                TDEFL_PUT_BITS_FAST(d->m_huff_codes[0][lit], d->m_huff_code_sizes[0][lit]);

                if (((flags & 2) == 0) && (pLZ_codes < pLZ_code_buf_end))
                {
                    flags >>= 1;
                    lit = *pLZ_codes++;
                    MZ_ASSERT(d->m_huff_code_sizes[0][lit]);
                    TDEFL_PUT_BITS_FAST(d->m_huff_codes[0][lit], d->m_huff_code_sizes[0][lit]);

                    if (((flags & 2) == 0) && (pLZ_codes < pLZ_code_buf_end))
                    {
                        flags >>= 1;
                        lit = *pLZ_codes++;
                        MZ_ASSERT(d->m_huff_code_sizes[0][lit]);
                        TDEFL_PUT_BITS_FAST(d->m_huff_codes[0][lit], d->m_huff_code_sizes[0][lit]);
                    }
                }
            }

            if (pOutput_buf >= d->m_pOutput_buf_end)
                return MZ_FALSE;

            memcpy(pOutput_buf, &bit_buffer, sizeof(mz_uint64));
            pOutput_buf += (bits_in >> 3);
            bit_buffer >>= (bits_in & ~7);
            bits_in &= 7;
        }

#undef TDEFL_PUT_BITS_FAST

        d->m_pOutput_buf = pOutput_buf;
        d->m_bits_in = 0;
        d->m_bit_buffer = 0;

        while (bits_in)
        {
            mz_uint32 n = MZ_MIN(bits_in, 16);
            TDEFL_PUT_BITS((mz_uint)bit_buffer & mz_bitmasks[n], n);
            bit_buffer >>= n;
            bits_in -= n;
        }

        TDEFL_PUT_BITS(d->m_huff_codes[0][256], d->m_huff_code_sizes[0][256]);

        return (d->m_pOutput_buf < d->m_pOutput_buf_end);
    }
#else
static mz_bool tdefl_compress_lz_codes(tdefl_compressor *d)
{
    mz_uint flags;
    mz_uint8 *pLZ_codes;

    flags = 1;
    for (pLZ_codes = d->m_lz_code_buf; pLZ_codes < d->m_pLZ_code_buf; flags >>= 1)
    {
        if (flags == 1)
            flags = *pLZ_codes++ | 0x100;
        if (flags & 1)
        {
            mz_uint sym, num_extra_bits;
            mz_uint match_len = pLZ_codes[0], match_dist = (pLZ_codes[1] | (pLZ_codes[2] << 8));
            pLZ_codes += 3;

            MZ_ASSERT(d->m_huff_code_sizes[0][s_tdefl_len_sym[match_len]]);
            TDEFL_PUT_BITS(d->m_huff_codes[0][s_tdefl_len_sym[match_len]], d->m_huff_code_sizes[0][s_tdefl_len_sym[match_len]]);
            TDEFL_PUT_BITS(match_len & mz_bitmasks[s_tdefl_len_extra[match_len]], s_tdefl_len_extra[match_len]);

            if (match_dist < 512)
            {
                sym = s_tdefl_small_dist_sym[match_dist];
                num_extra_bits = s_tdefl_small_dist_extra[match_dist];
            }
            else
            {
                sym = s_tdefl_large_dist_sym[match_dist >> 8];
                num_extra_bits = s_tdefl_large_dist_extra[match_dist >> 8];
            }
            MZ_ASSERT(d->m_huff_code_sizes[1][sym]);
            TDEFL_PUT_BITS(d->m_huff_codes[1][sym], d->m_huff_code_sizes[1][sym]);
            TDEFL_PUT_BITS(match_dist & mz_bitmasks[num_extra_bits], num_extra_bits);
        }
        else
        {
            mz_uint lit = *pLZ_codes++;
            MZ_ASSERT(d->m_huff_code_sizes[0][lit]);
            TDEFL_PUT_BITS(d->m_huff_codes[0][lit], d->m_huff_code_sizes[0][lit]);
        }
    }

    TDEFL_PUT_BITS(d->m_huff_codes[0][256], d->m_huff_code_sizes[0][256]);

    return (d->m_pOutput_buf < d->m_pOutput_buf_end);
}
#endif /* MINIZ_USE_UNALIGNED_LOADS_AND_STORES && MINIZ_LITTLE_ENDIAN && MINIZ_HAS_64BIT_REGISTERS */

    static mz_bool tdefl_compress_block(tdefl_compressor *d, mz_bool static_block)
    {
        if (static_block)
            tdefl_start_static_block(d);
        else
            tdefl_start_dynamic_block(d);
        return tdefl_compress_lz_codes(d);
    }

    static const mz_uint s_tdefl_num_probes[11] = { 0, 1, 6, 32, 16, 32, 128, 256, 512, 768, 1500 };

    static int tdefl_flush_block(tdefl_compressor *d, int flush)
    {
        mz_uint saved_bit_buf, saved_bits_in;
        mz_uint8 *pSaved_output_buf;
        mz_bool comp_block_succeeded = MZ_FALSE;
        int n, use_raw_block = ((d->m_flags & TDEFL_FORCE_ALL_RAW_BLOCKS) != 0) && (d->m_lookahead_pos - d->m_lz_code_buf_dict_pos) <= d->m_dict_size;
        mz_uint8 *pOutput_buf_start = ((d->m_pPut_buf_func == NULL) && ((*d->m_pOut_buf_size - d->m_out_buf_ofs) >= TDEFL_OUT_BUF_SIZE)) ? ((mz_uint8 *)d->m_pOut_buf + d->m_out_buf_ofs) : d->m_output_buf;

        d->m_pOutput_buf = pOutput_buf_start;
        d->m_pOutput_buf_end = d->m_pOutput_buf + TDEFL_OUT_BUF_SIZE - 16;

        MZ_ASSERT(!d->m_output_flush_remaining);
        d->m_output_flush_ofs = 0;
        d->m_output_flush_remaining = 0;

        *d->m_pLZ_flags = (mz_uint8)(*d->m_pLZ_flags >> d->m_num_flags_left);
        d->m_pLZ_code_buf -= (d->m_num_flags_left == 8);

        if ((d->m_flags & TDEFL_WRITE_ZLIB_HEADER) && (!d->m_block_index))
        {
            const mz_uint8 cmf = 0x78;
            mz_uint8 flg, flevel = 3;
            mz_uint header, i, mz_un = sizeof(s_tdefl_num_probes) / sizeof(mz_uint);

            /* Determine compression level by reversing the process in tdefl_create_comp_flags_from_zip_params() */
            for (i = 0; i < mz_un; i++)
                if (s_tdefl_num_probes[i] == (d->m_flags & 0xFFF))
                    break;

            if (i < 2)
                flevel = 0;
            else if (i < 6)
                flevel = 1;
            else if (i == 6)
                flevel = 2;

            header = cmf << 8 | (flevel << 6);
            header += 31 - (header % 31);
            flg = header & 0xFF;

            TDEFL_PUT_BITS(cmf, 8);
            TDEFL_PUT_BITS(flg, 8);
        }

        TDEFL_PUT_BITS(flush == TDEFL_FINISH, 1);

        pSaved_output_buf = d->m_pOutput_buf;
        saved_bit_buf = d->m_bit_buffer;
        saved_bits_in = d->m_bits_in;

        if (!use_raw_block)
            comp_block_succeeded = tdefl_compress_block(d, (d->m_flags & TDEFL_FORCE_ALL_STATIC_BLOCKS) || (d->m_total_lz_bytes < 48));

        /* If the block gets expanded, forget the current contents of the output buffer and send a raw block instead. */
        if (((use_raw_block) || ((d->m_total_lz_bytes) && ((d->m_pOutput_buf - pSaved_output_buf + 1U) >= d->m_total_lz_bytes))) &&
            ((d->m_lookahead_pos - d->m_lz_code_buf_dict_pos) <= d->m_dict_size))
        {
            mz_uint i;
            d->m_pOutput_buf = pSaved_output_buf;
            d->m_bit_buffer = saved_bit_buf, d->m_bits_in = saved_bits_in;
            TDEFL_PUT_BITS(0, 2);
            if (d->m_bits_in)
            {
                TDEFL_PUT_BITS(0, 8 - d->m_bits_in);
            }
            for (i = 2; i; --i, d->m_total_lz_bytes ^= 0xFFFF)
            {
                TDEFL_PUT_BITS(d->m_total_lz_bytes & 0xFFFF, 16);
            }
            for (i = 0; i < d->m_total_lz_bytes; ++i)
            {
                TDEFL_PUT_BITS(d->m_dict[(d->m_lz_code_buf_dict_pos + i) & TDEFL_LZ_DICT_SIZE_MASK], 8);
            }
        }
        /* Check for the extremely unlikely (if not impossible) case of the compressed block not fitting into the output buffer when using dynamic codes. */
        else if (!comp_block_succeeded)
        {
            d->m_pOutput_buf = pSaved_output_buf;
            d->m_bit_buffer = saved_bit_buf, d->m_bits_in = saved_bits_in;
            tdefl_compress_block(d, MZ_TRUE);
        }

        if (flush)
        {
            if (flush == TDEFL_FINISH)
            {
                if (d->m_bits_in)
                {
                    TDEFL_PUT_BITS(0, 8 - d->m_bits_in);
                }
                if (d->m_flags & TDEFL_WRITE_ZLIB_HEADER)
                {
                    mz_uint i, a = d->m_adler32;
                    for (i = 0; i < 4; i++)
                    {
                        TDEFL_PUT_BITS((a >> 24) & 0xFF, 8);
                        a <<= 8;
                    }
                }
            }
            else
            {
                mz_uint i, z = 0;
                TDEFL_PUT_BITS(0, 3);
                if (d->m_bits_in)
                {
                    TDEFL_PUT_BITS(0, 8 - d->m_bits_in);
                }
                for (i = 2; i; --i, z ^= 0xFFFF)
                {
                    TDEFL_PUT_BITS(z & 0xFFFF, 16);
                }
            }
        }

        MZ_ASSERT(d->m_pOutput_buf < d->m_pOutput_buf_end);

        memset(&d->m_huff_count[0][0], 0, sizeof(d->m_huff_count[0][0]) * TDEFL_MAX_HUFF_SYMBOLS_0);
        memset(&d->m_huff_count[1][0], 0, sizeof(d->m_huff_count[1][0]) * TDEFL_MAX_HUFF_SYMBOLS_1);

        d->m_pLZ_code_buf = d->m_lz_code_buf + 1;
        d->m_pLZ_flags = d->m_lz_code_buf;
        d->m_num_flags_left = 8;
        d->m_lz_code_buf_dict_pos += d->m_total_lz_bytes;
        d->m_total_lz_bytes = 0;
        d->m_block_index++;

        if ((n = (int)(d->m_pOutput_buf - pOutput_buf_start)) != 0)
        {
            if (d->m_pPut_buf_func)
            {
                *d->m_pIn_buf_size = d->m_pSrc - (const mz_uint8 *)d->m_pIn_buf;
                if (!(*d->m_pPut_buf_func)(d->m_output_buf, n, d->m_pPut_buf_user))
                    return (d->m_prev_return_status = TDEFL_STATUS_PUT_BUF_FAILED);
            }
            else if (pOutput_buf_start == d->m_output_buf)
            {
                int bytes_to_copy = (int)MZ_MIN((size_t)n, (size_t)(*d->m_pOut_buf_size - d->m_out_buf_ofs));
                memcpy((mz_uint8 *)d->m_pOut_buf + d->m_out_buf_ofs, d->m_output_buf, bytes_to_copy);
                d->m_out_buf_ofs += bytes_to_copy;
                if ((n -= bytes_to_copy) != 0)
                {
                    d->m_output_flush_ofs = bytes_to_copy;
                    d->m_output_flush_remaining = n;
                }
            }
            else
            {
                d->m_out_buf_ofs += n;
            }
        }

        return d->m_output_flush_remaining;
    }

#if MINIZ_USE_UNALIGNED_LOADS_AND_STORES
#ifdef MINIZ_UNALIGNED_USE_MEMCPY
    static mz_uint16 TDEFL_READ_UNALIGNED_WORD(const mz_uint8 *p)
    {
        mz_uint16 ret;
        memcpy(&ret, p, sizeof(mz_uint16));
        return ret;
    }
    static mz_uint16 TDEFL_READ_UNALIGNED_WORD2(const mz_uint16 *p)
    {
        mz_uint16 ret;
        memcpy(&ret, p, sizeof(mz_uint16));
        return ret;
    }
#else
#define TDEFL_READ_UNALIGNED_WORD(p) *(const mz_uint16 *)(p)
#define TDEFL_READ_UNALIGNED_WORD2(p) *(const mz_uint16 *)(p)
#endif
    static MZ_FORCEINLINE void tdefl_find_match(tdefl_compressor *d, mz_uint lookahead_pos, mz_uint max_dist, mz_uint max_match_len, mz_uint *pMatch_dist, mz_uint *pMatch_len)
    {
        mz_uint dist, pos = lookahead_pos & TDEFL_LZ_DICT_SIZE_MASK, match_len = *pMatch_len, probe_pos = pos, next_probe_pos, probe_len;
        mz_uint num_probes_left = d->m_max_probes[match_len >= 32];
        const mz_uint16 *s = (const mz_uint16 *)(d->m_dict + pos), *p, *q;
        mz_uint16 c01 = TDEFL_READ_UNALIGNED_WORD(&d->m_dict[pos + match_len - 1]), s01 = TDEFL_READ_UNALIGNED_WORD2(s);
        MZ_ASSERT(max_match_len <= TDEFL_MAX_MATCH_LEN);
        if (max_match_len <= match_len)
            return;
        for (;;)
        {
            for (;;)
            {
                if (--num_probes_left == 0)
                    return;
#define TDEFL_PROBE                                                                             \
    next_probe_pos = d->m_next[probe_pos];                                                      \
    if ((!next_probe_pos) || ((dist = (mz_uint16)(lookahead_pos - next_probe_pos)) > max_dist)) \
        return;                                                                                 \
    probe_pos = next_probe_pos & TDEFL_LZ_DICT_SIZE_MASK;                                       \
    if (TDEFL_READ_UNALIGNED_WORD(&d->m_dict[probe_pos + match_len - 1]) == c01)                \
        break;
                TDEFL_PROBE;
                TDEFL_PROBE;
                TDEFL_PROBE;
            }
            if (!dist)
                break;
            q = (const mz_uint16 *)(d->m_dict + probe_pos);
            if (TDEFL_READ_UNALIGNED_WORD2(q) != s01)
                continue;
            p = s;
            probe_len = 32;
            do
            {
            } while ((TDEFL_READ_UNALIGNED_WORD2(++p) == TDEFL_READ_UNALIGNED_WORD2(++q)) && (TDEFL_READ_UNALIGNED_WORD2(++p) == TDEFL_READ_UNALIGNED_WORD2(++q)) &&
                     (TDEFL_READ_UNALIGNED_WORD2(++p) == TDEFL_READ_UNALIGNED_WORD2(++q)) && (TDEFL_READ_UNALIGNED_WORD2(++p) == TDEFL_READ_UNALIGNED_WORD2(++q)) && (--probe_len > 0));
            if (!probe_len)
            {
                *pMatch_dist = dist;
                *pMatch_len = MZ_MIN(max_match_len, (mz_uint)TDEFL_MAX_MATCH_LEN);
                break;
            }
            else if ((probe_len = ((mz_uint)(p - s) * 2) + (mz_uint)(*(const mz_uint8 *)p == *(const mz_uint8 *)q)) > match_len)
            {
                *pMatch_dist = dist;
                if ((*pMatch_len = match_len = MZ_MIN(max_match_len, probe_len)) == max_match_len)
                    break;
                c01 = TDEFL_READ_UNALIGNED_WORD(&d->m_dict[pos + match_len - 1]);
            }
        }
    }
#else
static MZ_FORCEINLINE void tdefl_find_match(tdefl_compressor *d, mz_uint lookahead_pos, mz_uint max_dist, mz_uint max_match_len, mz_uint *pMatch_dist, mz_uint *pMatch_len)
{
    mz_uint dist, pos = lookahead_pos & TDEFL_LZ_DICT_SIZE_MASK, match_len = *pMatch_len, probe_pos = pos, next_probe_pos, probe_len;
    mz_uint num_probes_left = d->m_max_probes[match_len >= 32];
    const mz_uint8 *s = d->m_dict + pos, *p, *q;
    mz_uint8 c0 = d->m_dict[pos + match_len], c1 = d->m_dict[pos + match_len - 1];
    MZ_ASSERT(max_match_len <= TDEFL_MAX_MATCH_LEN);
    if (max_match_len <= match_len)
        return;
    for (;;)
    {
        for (;;)
        {
            if (--num_probes_left == 0)
                return;
#define TDEFL_PROBE                                                                               \
    next_probe_pos = d->m_next[probe_pos];                                                        \
    if ((!next_probe_pos) || ((dist = (mz_uint16)(lookahead_pos - next_probe_pos)) > max_dist))   \
        return;                                                                                   \
    probe_pos = next_probe_pos & TDEFL_LZ_DICT_SIZE_MASK;                                         \
    if ((d->m_dict[probe_pos + match_len] == c0) && (d->m_dict[probe_pos + match_len - 1] == c1)) \
        break;
            TDEFL_PROBE;
            TDEFL_PROBE;
            TDEFL_PROBE;
        }
        if (!dist)
            break;
        p = s;
        q = d->m_dict + probe_pos;
        for (probe_len = 0; probe_len < max_match_len; probe_len++)
            if (*p++ != *q++)
                break;
        if (probe_len > match_len)
        {
            *pMatch_dist = dist;
            if ((*pMatch_len = match_len = probe_len) == max_match_len)
                return;
            c0 = d->m_dict[pos + match_len];
            c1 = d->m_dict[pos + match_len - 1];
        }
    }
}
#endif /* #if MINIZ_USE_UNALIGNED_LOADS_AND_STORES */

#if MINIZ_USE_UNALIGNED_LOADS_AND_STORES && MINIZ_LITTLE_ENDIAN
#ifdef MINIZ_UNALIGNED_USE_MEMCPY
    static mz_uint32 TDEFL_READ_UNALIGNED_WORD32(const mz_uint8 *p)
    {
        mz_uint32 ret;
        memcpy(&ret, p, sizeof(mz_uint32));
        return ret;
    }
#else
#define TDEFL_READ_UNALIGNED_WORD32(p) *(const mz_uint32 *)(p)
#endif
    static mz_bool tdefl_compress_fast(tdefl_compressor *d)
    {
        /* Faster, minimally featured LZRW1-style match+parse loop with better register utilization. Intended for applications where raw throughput is valued more highly than ratio. */
        mz_uint lookahead_pos = d->m_lookahead_pos, lookahead_size = d->m_lookahead_size, dict_size = d->m_dict_size, total_lz_bytes = d->m_total_lz_bytes, num_flags_left = d->m_num_flags_left;
        mz_uint8 *pLZ_code_buf = d->m_pLZ_code_buf, *pLZ_flags = d->m_pLZ_flags;
        mz_uint cur_pos = lookahead_pos & TDEFL_LZ_DICT_SIZE_MASK;

        while ((d->m_src_buf_left) || ((d->m_flush) && (lookahead_size)))
        {
            const mz_uint TDEFL_COMP_FAST_LOOKAHEAD_SIZE = 4096;
            mz_uint dst_pos = (lookahead_pos + lookahead_size) & TDEFL_LZ_DICT_SIZE_MASK;
            mz_uint num_bytes_to_process = (mz_uint)MZ_MIN(d->m_src_buf_left, TDEFL_COMP_FAST_LOOKAHEAD_SIZE - lookahead_size);
            d->m_src_buf_left -= num_bytes_to_process;
            lookahead_size += num_bytes_to_process;

            while (num_bytes_to_process)
            {
                mz_uint32 n = MZ_MIN(TDEFL_LZ_DICT_SIZE - dst_pos, num_bytes_to_process);
                memcpy(d->m_dict + dst_pos, d->m_pSrc, n);
                if (dst_pos < (TDEFL_MAX_MATCH_LEN - 1))
                    memcpy(d->m_dict + TDEFL_LZ_DICT_SIZE + dst_pos, d->m_pSrc, MZ_MIN(n, (TDEFL_MAX_MATCH_LEN - 1) - dst_pos));
                d->m_pSrc += n;
                dst_pos = (dst_pos + n) & TDEFL_LZ_DICT_SIZE_MASK;
                num_bytes_to_process -= n;
            }

            dict_size = MZ_MIN(TDEFL_LZ_DICT_SIZE - lookahead_size, dict_size);
            if ((!d->m_flush) && (lookahead_size < TDEFL_COMP_FAST_LOOKAHEAD_SIZE))
                break;

            while (lookahead_size >= 4)
            {
                mz_uint cur_match_dist, cur_match_len = 1;
                mz_uint8 *pCur_dict = d->m_dict + cur_pos;
                mz_uint first_trigram = TDEFL_READ_UNALIGNED_WORD32(pCur_dict) & 0xFFFFFF;
                mz_uint hash = (first_trigram ^ (first_trigram >> (24 - (TDEFL_LZ_HASH_BITS - 8)))) & TDEFL_LEVEL1_HASH_SIZE_MASK;
                mz_uint probe_pos = d->m_hash[hash];
                d->m_hash[hash] = (mz_uint16)lookahead_pos;

                if (((cur_match_dist = (mz_uint16)(lookahead_pos - probe_pos)) <= dict_size) && ((TDEFL_READ_UNALIGNED_WORD32(d->m_dict + (probe_pos &= TDEFL_LZ_DICT_SIZE_MASK)) & 0xFFFFFF) == first_trigram))
                {
                    const mz_uint16 *p = (const mz_uint16 *)pCur_dict;
                    const mz_uint16 *q = (const mz_uint16 *)(d->m_dict + probe_pos);
                    mz_uint32 probe_len = 32;
                    do
                    {
                    } while ((TDEFL_READ_UNALIGNED_WORD2(++p) == TDEFL_READ_UNALIGNED_WORD2(++q)) && (TDEFL_READ_UNALIGNED_WORD2(++p) == TDEFL_READ_UNALIGNED_WORD2(++q)) &&
                             (TDEFL_READ_UNALIGNED_WORD2(++p) == TDEFL_READ_UNALIGNED_WORD2(++q)) && (TDEFL_READ_UNALIGNED_WORD2(++p) == TDEFL_READ_UNALIGNED_WORD2(++q)) && (--probe_len > 0));
                    cur_match_len = ((mz_uint)(p - (const mz_uint16 *)pCur_dict) * 2) + (mz_uint)(*(const mz_uint8 *)p == *(const mz_uint8 *)q);
                    if (!probe_len)
                        cur_match_len = cur_match_dist ? TDEFL_MAX_MATCH_LEN : 0;

                    if ((cur_match_len < TDEFL_MIN_MATCH_LEN) || ((cur_match_len == TDEFL_MIN_MATCH_LEN) && (cur_match_dist >= 8U * 1024U)))
                    {
                        cur_match_len = 1;
                        *pLZ_code_buf++ = (mz_uint8)first_trigram;
                        *pLZ_flags = (mz_uint8)(*pLZ_flags >> 1);
                        d->m_huff_count[0][(mz_uint8)first_trigram]++;
                    }
                    else
                    {
                        mz_uint32 s0, s1;
                        cur_match_len = MZ_MIN(cur_match_len, lookahead_size);

                        MZ_ASSERT((cur_match_len >= TDEFL_MIN_MATCH_LEN) && (cur_match_dist >= 1) && (cur_match_dist <= TDEFL_LZ_DICT_SIZE));

                        cur_match_dist--;

                        pLZ_code_buf[0] = (mz_uint8)(cur_match_len - TDEFL_MIN_MATCH_LEN);
#ifdef MINIZ_UNALIGNED_USE_MEMCPY
                        memcpy(&pLZ_code_buf[1], &cur_match_dist, sizeof(cur_match_dist));
#else
                        *(mz_uint16 *)(&pLZ_code_buf[1]) = (mz_uint16)cur_match_dist;
#endif
                        pLZ_code_buf += 3;
                        *pLZ_flags = (mz_uint8)((*pLZ_flags >> 1) | 0x80);

                        s0 = s_tdefl_small_dist_sym[cur_match_dist & 511];
                        s1 = s_tdefl_large_dist_sym[cur_match_dist >> 8];
                        d->m_huff_count[1][(cur_match_dist < 512) ? s0 : s1]++;

                        d->m_huff_count[0][s_tdefl_len_sym[cur_match_len - TDEFL_MIN_MATCH_LEN]]++;
                    }
                }
                else
                {
                    *pLZ_code_buf++ = (mz_uint8)first_trigram;
                    *pLZ_flags = (mz_uint8)(*pLZ_flags >> 1);
                    d->m_huff_count[0][(mz_uint8)first_trigram]++;
                }

                if (--num_flags_left == 0)
                {
                    num_flags_left = 8;
                    pLZ_flags = pLZ_code_buf++;
                }

                total_lz_bytes += cur_match_len;
                lookahead_pos += cur_match_len;
                dict_size = MZ_MIN(dict_size + cur_match_len, (mz_uint)TDEFL_LZ_DICT_SIZE);
                cur_pos = (cur_pos + cur_match_len) & TDEFL_LZ_DICT_SIZE_MASK;
                MZ_ASSERT(lookahead_size >= cur_match_len);
                lookahead_size -= cur_match_len;

                if (pLZ_code_buf > &d->m_lz_code_buf[TDEFL_LZ_CODE_BUF_SIZE - 8])
                {
                    int n;
                    d->m_lookahead_pos = lookahead_pos;
                    d->m_lookahead_size = lookahead_size;
                    d->m_dict_size = dict_size;
                    d->m_total_lz_bytes = total_lz_bytes;
                    d->m_pLZ_code_buf = pLZ_code_buf;
                    d->m_pLZ_flags = pLZ_flags;
                    d->m_num_flags_left = num_flags_left;
                    if ((n = tdefl_flush_block(d, 0)) != 0)
                        return (n < 0) ? MZ_FALSE : MZ_TRUE;
                    total_lz_bytes = d->m_total_lz_bytes;
                    pLZ_code_buf = d->m_pLZ_code_buf;
                    pLZ_flags = d->m_pLZ_flags;
                    num_flags_left = d->m_num_flags_left;
                }
            }

            while (lookahead_size)
            {
                mz_uint8 lit = d->m_dict[cur_pos];

                total_lz_bytes++;
                *pLZ_code_buf++ = lit;
                *pLZ_flags = (mz_uint8)(*pLZ_flags >> 1);
                if (--num_flags_left == 0)
                {
                    num_flags_left = 8;
                    pLZ_flags = pLZ_code_buf++;
                }

                d->m_huff_count[0][lit]++;

                lookahead_pos++;
                dict_size = MZ_MIN(dict_size + 1, (mz_uint)TDEFL_LZ_DICT_SIZE);
                cur_pos = (cur_pos + 1) & TDEFL_LZ_DICT_SIZE_MASK;
                lookahead_size--;

                if (pLZ_code_buf > &d->m_lz_code_buf[TDEFL_LZ_CODE_BUF_SIZE - 8])
                {
                    int n;
                    d->m_lookahead_pos = lookahead_pos;
                    d->m_lookahead_size = lookahead_size;
                    d->m_dict_size = dict_size;
                    d->m_total_lz_bytes = total_lz_bytes;
                    d->m_pLZ_code_buf = pLZ_code_buf;
                    d->m_pLZ_flags = pLZ_flags;
                    d->m_num_flags_left = num_flags_left;
                    if ((n = tdefl_flush_block(d, 0)) != 0)
                        return (n < 0) ? MZ_FALSE : MZ_TRUE;
                    total_lz_bytes = d->m_total_lz_bytes;
                    pLZ_code_buf = d->m_pLZ_code_buf;
                    pLZ_flags = d->m_pLZ_flags;
                    num_flags_left = d->m_num_flags_left;
                }
            }
        }

        d->m_lookahead_pos = lookahead_pos;
        d->m_lookahead_size = lookahead_size;
        d->m_dict_size = dict_size;
        d->m_total_lz_bytes = total_lz_bytes;
        d->m_pLZ_code_buf = pLZ_code_buf;
        d->m_pLZ_flags = pLZ_flags;
        d->m_num_flags_left = num_flags_left;
        return MZ_TRUE;
    }
#endif /* MINIZ_USE_UNALIGNED_LOADS_AND_STORES && MINIZ_LITTLE_ENDIAN */

    static MZ_FORCEINLINE void tdefl_record_literal(tdefl_compressor *d, mz_uint8 lit)
    {
        d->m_total_lz_bytes++;
        *d->m_pLZ_code_buf++ = lit;
        *d->m_pLZ_flags = (mz_uint8)(*d->m_pLZ_flags >> 1);
        if (--d->m_num_flags_left == 0)
        {
            d->m_num_flags_left = 8;
            d->m_pLZ_flags = d->m_pLZ_code_buf++;
        }
        d->m_huff_count[0][lit]++;
    }

    static MZ_FORCEINLINE void tdefl_record_match(tdefl_compressor *d, mz_uint match_len, mz_uint match_dist)
    {
        mz_uint32 s0, s1;

        MZ_ASSERT((match_len >= TDEFL_MIN_MATCH_LEN) && (match_dist >= 1) && (match_dist <= TDEFL_LZ_DICT_SIZE));

        d->m_total_lz_bytes += match_len;

        d->m_pLZ_code_buf[0] = (mz_uint8)(match_len - TDEFL_MIN_MATCH_LEN);

        match_dist -= 1;
        d->m_pLZ_code_buf[1] = (mz_uint8)(match_dist & 0xFF);
        d->m_pLZ_code_buf[2] = (mz_uint8)(match_dist >> 8);
        d->m_pLZ_code_buf += 3;

        *d->m_pLZ_flags = (mz_uint8)((*d->m_pLZ_flags >> 1) | 0x80);
        if (--d->m_num_flags_left == 0)
        {
            d->m_num_flags_left = 8;
            d->m_pLZ_flags = d->m_pLZ_code_buf++;
        }

        s0 = s_tdefl_small_dist_sym[match_dist & 511];
        s1 = s_tdefl_large_dist_sym[(match_dist >> 8) & 127];
        d->m_huff_count[1][(match_dist < 512) ? s0 : s1]++;
        d->m_huff_count[0][s_tdefl_len_sym[match_len - TDEFL_MIN_MATCH_LEN]]++;
    }

    static mz_bool tdefl_compress_normal(tdefl_compressor *d)
    {
        const mz_uint8 *pSrc = d->m_pSrc;
        size_t src_buf_left = d->m_src_buf_left;
        tdefl_flush flush = d->m_flush;

        while ((src_buf_left) || ((flush) && (d->m_lookahead_size)))
        {
            mz_uint len_to_move, cur_match_dist, cur_match_len, cur_pos;
            /* Update dictionary and hash chains. Keeps the lookahead size equal to TDEFL_MAX_MATCH_LEN. */
            if ((d->m_lookahead_size + d->m_dict_size) >= (TDEFL_MIN_MATCH_LEN - 1))
            {
                mz_uint dst_pos = (d->m_lookahead_pos + d->m_lookahead_size) & TDEFL_LZ_DICT_SIZE_MASK, ins_pos = d->m_lookahead_pos + d->m_lookahead_size - 2;
                mz_uint hash = (d->m_dict[ins_pos & TDEFL_LZ_DICT_SIZE_MASK] << TDEFL_LZ_HASH_SHIFT) ^ d->m_dict[(ins_pos + 1) & TDEFL_LZ_DICT_SIZE_MASK];
                mz_uint num_bytes_to_process = (mz_uint)MZ_MIN(src_buf_left, TDEFL_MAX_MATCH_LEN - d->m_lookahead_size);
                const mz_uint8 *pSrc_end = pSrc ? pSrc + num_bytes_to_process : NULL;
                src_buf_left -= num_bytes_to_process;
                d->m_lookahead_size += num_bytes_to_process;
                while (pSrc != pSrc_end)
                {
                    mz_uint8 c = *pSrc++;
                    d->m_dict[dst_pos] = c;
                    if (dst_pos < (TDEFL_MAX_MATCH_LEN - 1))
                        d->m_dict[TDEFL_LZ_DICT_SIZE + dst_pos] = c;
                    hash = ((hash << TDEFL_LZ_HASH_SHIFT) ^ c) & (TDEFL_LZ_HASH_SIZE - 1);
                    d->m_next[ins_pos & TDEFL_LZ_DICT_SIZE_MASK] = d->m_hash[hash];
                    d->m_hash[hash] = (mz_uint16)(ins_pos);
                    dst_pos = (dst_pos + 1) & TDEFL_LZ_DICT_SIZE_MASK;
                    ins_pos++;
                }
            }
            else
            {
                while ((src_buf_left) && (d->m_lookahead_size < TDEFL_MAX_MATCH_LEN))
                {
                    mz_uint8 c = *pSrc++;
                    mz_uint dst_pos = (d->m_lookahead_pos + d->m_lookahead_size) & TDEFL_LZ_DICT_SIZE_MASK;
                    src_buf_left--;
                    d->m_dict[dst_pos] = c;
                    if (dst_pos < (TDEFL_MAX_MATCH_LEN - 1))
                        d->m_dict[TDEFL_LZ_DICT_SIZE + dst_pos] = c;
                    if ((++d->m_lookahead_size + d->m_dict_size) >= TDEFL_MIN_MATCH_LEN)
                    {
                        mz_uint ins_pos = d->m_lookahead_pos + (d->m_lookahead_size - 1) - 2;
                        mz_uint hash = ((d->m_dict[ins_pos & TDEFL_LZ_DICT_SIZE_MASK] << (TDEFL_LZ_HASH_SHIFT * 2)) ^ (d->m_dict[(ins_pos + 1) & TDEFL_LZ_DICT_SIZE_MASK] << TDEFL_LZ_HASH_SHIFT) ^ c) & (TDEFL_LZ_HASH_SIZE - 1);
                        d->m_next[ins_pos & TDEFL_LZ_DICT_SIZE_MASK] = d->m_hash[hash];
                        d->m_hash[hash] = (mz_uint16)(ins_pos);
                    }
                }
            }
            d->m_dict_size = MZ_MIN(TDEFL_LZ_DICT_SIZE - d->m_lookahead_size, d->m_dict_size);
            if ((!flush) && (d->m_lookahead_size < TDEFL_MAX_MATCH_LEN))
                break;

            /* Simple lazy/greedy parsing state machine. */
            len_to_move = 1;
            cur_match_dist = 0;
            cur_match_len = d->m_saved_match_len ? d->m_saved_match_len : (TDEFL_MIN_MATCH_LEN - 1);
            cur_pos = d->m_lookahead_pos & TDEFL_LZ_DICT_SIZE_MASK;
            if (d->m_flags & (TDEFL_RLE_MATCHES | TDEFL_FORCE_ALL_RAW_BLOCKS))
            {
                if ((d->m_dict_size) && (!(d->m_flags & TDEFL_FORCE_ALL_RAW_BLOCKS)))
                {
                    mz_uint8 c = d->m_dict[(cur_pos - 1) & TDEFL_LZ_DICT_SIZE_MASK];
                    cur_match_len = 0;
                    while (cur_match_len < d->m_lookahead_size)
                    {
                        if (d->m_dict[cur_pos + cur_match_len] != c)
                            break;
                        cur_match_len++;
                    }
                    if (cur_match_len < TDEFL_MIN_MATCH_LEN)
                        cur_match_len = 0;
                    else
                        cur_match_dist = 1;
                }
            }
            else
            {
                tdefl_find_match(d, d->m_lookahead_pos, d->m_dict_size, d->m_lookahead_size, &cur_match_dist, &cur_match_len);
            }
            if (((cur_match_len == TDEFL_MIN_MATCH_LEN) && (cur_match_dist >= 8U * 1024U)) || (cur_pos == cur_match_dist) || ((d->m_flags & TDEFL_FILTER_MATCHES) && (cur_match_len <= 5)))
            {
                cur_match_dist = cur_match_len = 0;
            }
            if (d->m_saved_match_len)
            {
                if (cur_match_len > d->m_saved_match_len)
                {
                    tdefl_record_literal(d, (mz_uint8)d->m_saved_lit);
                    if (cur_match_len >= 128)
                    {
                        tdefl_record_match(d, cur_match_len, cur_match_dist);
                        d->m_saved_match_len = 0;
                        len_to_move = cur_match_len;
                    }
                    else
                    {
                        d->m_saved_lit = d->m_dict[cur_pos];
                        d->m_saved_match_dist = cur_match_dist;
                        d->m_saved_match_len = cur_match_len;
                    }
                }
                else
                {
                    tdefl_record_match(d, d->m_saved_match_len, d->m_saved_match_dist);
                    len_to_move = d->m_saved_match_len - 1;
                    d->m_saved_match_len = 0;
                }
            }
            else if (!cur_match_dist)
                tdefl_record_literal(d, d->m_dict[MZ_MIN(cur_pos, sizeof(d->m_dict) - 1)]);
            else if ((d->m_greedy_parsing) || (d->m_flags & TDEFL_RLE_MATCHES) || (cur_match_len >= 128))
            {
                tdefl_record_match(d, cur_match_len, cur_match_dist);
                len_to_move = cur_match_len;
            }
            else
            {
                d->m_saved_lit = d->m_dict[MZ_MIN(cur_pos, sizeof(d->m_dict) - 1)];
                d->m_saved_match_dist = cur_match_dist;
                d->m_saved_match_len = cur_match_len;
            }
            /* Move the lookahead forward by len_to_move bytes. */
            d->m_lookahead_pos += len_to_move;
            MZ_ASSERT(d->m_lookahead_size >= len_to_move);
            d->m_lookahead_size -= len_to_move;
            d->m_dict_size = MZ_MIN(d->m_dict_size + len_to_move, (mz_uint)TDEFL_LZ_DICT_SIZE);
            /* Check if it's time to flush the current LZ codes to the internal output buffer. */
            if ((d->m_pLZ_code_buf > &d->m_lz_code_buf[TDEFL_LZ_CODE_BUF_SIZE - 8]) ||
                ((d->m_total_lz_bytes > 31 * 1024) && (((((mz_uint)(d->m_pLZ_code_buf - d->m_lz_code_buf) * 115) >> 7) >= d->m_total_lz_bytes) || (d->m_flags & TDEFL_FORCE_ALL_RAW_BLOCKS))))
            {
                int n;
                d->m_pSrc = pSrc;
                d->m_src_buf_left = src_buf_left;
                if ((n = tdefl_flush_block(d, 0)) != 0)
                    return (n < 0) ? MZ_FALSE : MZ_TRUE;
            }
        }

        d->m_pSrc = pSrc;
        d->m_src_buf_left = src_buf_left;
        return MZ_TRUE;
    }

    static tdefl_status tdefl_flush_output_buffer(tdefl_compressor *d)
    {
        if (d->m_pIn_buf_size)
        {
            *d->m_pIn_buf_size = d->m_pSrc - (const mz_uint8 *)d->m_pIn_buf;
        }

        if (d->m_pOut_buf_size)
        {
            size_t n = MZ_MIN(*d->m_pOut_buf_size - d->m_out_buf_ofs, d->m_output_flush_remaining);
            memcpy((mz_uint8 *)d->m_pOut_buf + d->m_out_buf_ofs, d->m_output_buf + d->m_output_flush_ofs, n);
            d->m_output_flush_ofs += (mz_uint)n;
            d->m_output_flush_remaining -= (mz_uint)n;
            d->m_out_buf_ofs += n;

            *d->m_pOut_buf_size = d->m_out_buf_ofs;
        }

        return (d->m_finished && !d->m_output_flush_remaining) ? TDEFL_STATUS_DONE : TDEFL_STATUS_OKAY;
    }

    tdefl_status tdefl_compress(tdefl_compressor *d, const void *pIn_buf, size_t *pIn_buf_size, void *pOut_buf, size_t *pOut_buf_size, tdefl_flush flush)
    {
        if (!d)
        {
            if (pIn_buf_size)
                *pIn_buf_size = 0;
            if (pOut_buf_size)
                *pOut_buf_size = 0;
            return TDEFL_STATUS_BAD_PARAM;
        }

        d->m_pIn_buf = pIn_buf;
        d->m_pIn_buf_size = pIn_buf_size;
        d->m_pOut_buf = pOut_buf;
        d->m_pOut_buf_size = pOut_buf_size;
        d->m_pSrc = (const mz_uint8 *)(pIn_buf);
        d->m_src_buf_left = pIn_buf_size ? *pIn_buf_size : 0;
        d->m_out_buf_ofs = 0;
        d->m_flush = flush;

        if (((d->m_pPut_buf_func != NULL) == ((pOut_buf != NULL) || (pOut_buf_size != NULL))) || (d->m_prev_return_status != TDEFL_STATUS_OKAY) ||
            (d->m_wants_to_finish && (flush != TDEFL_FINISH)) || (pIn_buf_size && *pIn_buf_size && !pIn_buf) || (pOut_buf_size && *pOut_buf_size && !pOut_buf))
        {
            if (pIn_buf_size)
                *pIn_buf_size = 0;
            if (pOut_buf_size)
                *pOut_buf_size = 0;
            return (d->m_prev_return_status = TDEFL_STATUS_BAD_PARAM);
        }
        d->m_wants_to_finish |= (flush == TDEFL_FINISH);

        if ((d->m_output_flush_remaining) || (d->m_finished))
            return (d->m_prev_return_status = tdefl_flush_output_buffer(d));

#if MINIZ_USE_UNALIGNED_LOADS_AND_STORES && MINIZ_LITTLE_ENDIAN
        if (((d->m_flags & TDEFL_MAX_PROBES_MASK) == 1) &&
            ((d->m_flags & TDEFL_GREEDY_PARSING_FLAG) != 0) &&
            ((d->m_flags & (TDEFL_FILTER_MATCHES | TDEFL_FORCE_ALL_RAW_BLOCKS | TDEFL_RLE_MATCHES)) == 0))
        {
            if (!tdefl_compress_fast(d))
                return d->m_prev_return_status;
        }
        else
#endif /* #if MINIZ_USE_UNALIGNED_LOADS_AND_STORES && MINIZ_LITTLE_ENDIAN */
        {
            if (!tdefl_compress_normal(d))
                return d->m_prev_return_status;
        }

        if ((d->m_flags & (TDEFL_WRITE_ZLIB_HEADER | TDEFL_COMPUTE_ADLER32)) && (pIn_buf))
            d->m_adler32 = (mz_uint32)mz_adler32(d->m_adler32, (const mz_uint8 *)pIn_buf, d->m_pSrc - (const mz_uint8 *)pIn_buf);

        if ((flush) && (!d->m_lookahead_size) && (!d->m_src_buf_left) && (!d->m_output_flush_remaining))
        {
            if (tdefl_flush_block(d, flush) < 0)
                return d->m_prev_return_status;
            d->m_finished = (flush == TDEFL_FINISH);
            if (flush == TDEFL_FULL_FLUSH)
            {
                MZ_CLEAR_ARR(d->m_hash);
                MZ_CLEAR_ARR(d->m_next);
                d->m_dict_size = 0;
            }
        }

        return (d->m_prev_return_status = tdefl_flush_output_buffer(d));
    }

    tdefl_status tdefl_compress_buffer(tdefl_compressor *d, const void *pIn_buf, size_t in_buf_size, tdefl_flush flush)
    {
        MZ_ASSERT(d->m_pPut_buf_func);
        return tdefl_compress(d, pIn_buf, &in_buf_size, NULL, NULL, flush);
    }

    tdefl_status tdefl_init(tdefl_compressor *d, tdefl_put_buf_func_ptr pPut_buf_func, void *pPut_buf_user, int flags)
    {
        d->m_pPut_buf_func = pPut_buf_func;
        d->m_pPut_buf_user = pPut_buf_user;
        d->m_flags = (mz_uint)(flags);
        d->m_max_probes[0] = 1 + ((flags & 0xFFF) + 2) / 3;
        d->m_greedy_parsing = (flags & TDEFL_GREEDY_PARSING_FLAG) != 0;
        d->m_max_probes[1] = 1 + (((flags & 0xFFF) >> 2) + 2) / 3;
        if (!(flags & TDEFL_NONDETERMINISTIC_PARSING_FLAG))
            MZ_CLEAR_ARR(d->m_hash);
        d->m_lookahead_pos = d->m_lookahead_size = d->m_dict_size = d->m_total_lz_bytes = d->m_lz_code_buf_dict_pos = d->m_bits_in = 0;
        d->m_output_flush_ofs = d->m_output_flush_remaining = d->m_finished = d->m_block_index = d->m_bit_buffer = d->m_wants_to_finish = 0;
        d->m_pLZ_code_buf = d->m_lz_code_buf + 1;
        d->m_pLZ_flags = d->m_lz_code_buf;
        *d->m_pLZ_flags = 0;
        d->m_num_flags_left = 8;
        d->m_pOutput_buf = d->m_output_buf;
        d->m_pOutput_buf_end = d->m_output_buf;
        d->m_prev_return_status = TDEFL_STATUS_OKAY;
        d->m_saved_match_dist = d->m_saved_match_len = d->m_saved_lit = 0;
        d->m_adler32 = 1;
        d->m_pIn_buf = NULL;
        d->m_pOut_buf = NULL;
        d->m_pIn_buf_size = NULL;
        d->m_pOut_buf_size = NULL;
        d->m_flush = TDEFL_NO_FLUSH;
        d->m_pSrc = NULL;
        d->m_src_buf_left = 0;
        d->m_out_buf_ofs = 0;
        if (!(flags & TDEFL_NONDETERMINISTIC_PARSING_FLAG))
            MZ_CLEAR_ARR(d->m_dict);
        memset(&d->m_huff_count[0][0], 0, sizeof(d->m_huff_count[0][0]) * TDEFL_MAX_HUFF_SYMBOLS_0);
        memset(&d->m_huff_count[1][0], 0, sizeof(d->m_huff_count[1][0]) * TDEFL_MAX_HUFF_SYMBOLS_1);
        return TDEFL_STATUS_OKAY;
    }

    tdefl_status tdefl_get_prev_return_status(tdefl_compressor *d)
    {
        return d->m_prev_return_status;
    }

    mz_uint32 tdefl_get_adler32(tdefl_compressor *d)
    {
        return d->m_adler32;
    }

    mz_bool tdefl_compress_mem_to_output(const void *pBuf, size_t buf_len, tdefl_put_buf_func_ptr pPut_buf_func, void *pPut_buf_user, int flags)
    {
        tdefl_compressor *pComp;
        mz_bool succeeded;
        if (((buf_len) && (!pBuf)) || (!pPut_buf_func))
            return MZ_FALSE;
        pComp = (tdefl_compressor *)MZ_MALLOC(sizeof(tdefl_compressor));
        if (!pComp)
            return MZ_FALSE;
        succeeded = (tdefl_init(pComp, pPut_buf_func, pPut_buf_user, flags) == TDEFL_STATUS_OKAY);
        succeeded = succeeded && (tdefl_compress_buffer(pComp, pBuf, buf_len, TDEFL_FINISH) == TDEFL_STATUS_DONE);
        MZ_FREE(pComp);
        return succeeded;
    }

    typedef struct
    {
        size_t m_size, m_capacity;
        mz_uint8 *m_pBuf;
        mz_bool m_expandable;
    } tdefl_output_buffer;

    static mz_bool tdefl_output_buffer_putter(const void *pBuf, int len, void *pUser)
    {
        tdefl_output_buffer *p = (tdefl_output_buffer *)pUser;
        size_t new_size = p->m_size + len;
        if (new_size > p->m_capacity)
        {
            size_t new_capacity = p->m_capacity;
            mz_uint8 *pNew_buf;
            if (!p->m_expandable)
                return MZ_FALSE;
            do
            {
                new_capacity = MZ_MAX(128U, new_capacity << 1U);
            } while (new_size > new_capacity);
            pNew_buf = (mz_uint8 *)MZ_REALLOC(p->m_pBuf, new_capacity);
            if (!pNew_buf)
                return MZ_FALSE;
            p->m_pBuf = pNew_buf;
            p->m_capacity = new_capacity;
        }
        memcpy((mz_uint8 *)p->m_pBuf + p->m_size, pBuf, len);
        p->m_size = new_size;
        return MZ_TRUE;
    }

    void *tdefl_compress_mem_to_heap(const void *pSrc_buf, size_t src_buf_len, size_t *pOut_len, int flags)
    {
        tdefl_output_buffer out_buf;
        MZ_CLEAR_OBJ(out_buf);
        if (!pOut_len)
            return MZ_FALSE;
        else
            *pOut_len = 0;
        out_buf.m_expandable = MZ_TRUE;
        if (!tdefl_compress_mem_to_output(pSrc_buf, src_buf_len, tdefl_output_buffer_putter, &out_buf, flags))
            return NULL;
        *pOut_len = out_buf.m_size;
        return out_buf.m_pBuf;
    }

    size_t tdefl_compress_mem_to_mem(void *pOut_buf, size_t out_buf_len, const void *pSrc_buf, size_t src_buf_len, int flags)
    {
        tdefl_output_buffer out_buf;
        MZ_CLEAR_OBJ(out_buf);
        if (!pOut_buf)
            return 0;
        out_buf.m_pBuf = (mz_uint8 *)pOut_buf;
        out_buf.m_capacity = out_buf_len;
        if (!tdefl_compress_mem_to_output(pSrc_buf, src_buf_len, tdefl_output_buffer_putter, &out_buf, flags))
            return 0;
        return out_buf.m_size;
    }

    /* level may actually range from [0,10] (10 is a "hidden" max level, where we want a bit more compression and it's fine if throughput to fall off a cliff on some files). */
    mz_uint tdefl_create_comp_flags_from_zip_params(int level, int window_bits, int strategy)
    {
        mz_uint comp_flags = s_tdefl_num_probes[(level >= 0) ? MZ_MIN(10, level) : MZ_DEFAULT_LEVEL] | ((level <= 3) ? TDEFL_GREEDY_PARSING_FLAG : 0);
        if (window_bits > 0)
            comp_flags |= TDEFL_WRITE_ZLIB_HEADER;

        if (!level)
            comp_flags |= TDEFL_FORCE_ALL_RAW_BLOCKS;
        else if (strategy == MZ_FILTERED)
            comp_flags |= TDEFL_FILTER_MATCHES;
        else if (strategy == MZ_HUFFMAN_ONLY)
            comp_flags &= ~TDEFL_MAX_PROBES_MASK;
        else if (strategy == MZ_FIXED)
            comp_flags |= TDEFL_FORCE_ALL_STATIC_BLOCKS;
        else if (strategy == MZ_RLE)
            comp_flags |= TDEFL_RLE_MATCHES;

        return comp_flags;
    }

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4204) /* nonstandard extension used : non-constant aggregate initializer (also supported by GNU C and C99, so no big deal) */
#endif

    /* Simple PNG writer function by Alex Evans, 2011. Released into the public domain: https://gist.github.com/908299, more context at
     http://altdevblogaday.org/2011/04/06/a-smaller-jpg-encoder/.
     This is actually a modification of Alex's original code so PNG files generated by this function pass pngcheck. */
    void *tdefl_write_image_to_png_file_in_memory_ex(const void *pImage, int w, int h, int num_chans, size_t *pLen_out, mz_uint level, mz_bool flip)
    {
        /* Using a local copy of this array here in case MINIZ_NO_ZLIB_APIS was defined. */
        static const mz_uint s_tdefl_png_num_probes[11] = { 0, 1, 6, 32, 16, 32, 128, 256, 512, 768, 1500 };
        tdefl_compressor *pComp = (tdefl_compressor *)MZ_MALLOC(sizeof(tdefl_compressor));
        tdefl_output_buffer out_buf;
        int i, bpl = w * num_chans, y, z;
        mz_uint32 c;
        *pLen_out = 0;
        if (!pComp)
            return NULL;
        MZ_CLEAR_OBJ(out_buf);
        out_buf.m_expandable = MZ_TRUE;
        out_buf.m_capacity = 57 + MZ_MAX(64, (1 + bpl) * h);
        if (NULL == (out_buf.m_pBuf = (mz_uint8 *)MZ_MALLOC(out_buf.m_capacity)))
        {
            MZ_FREE(pComp);
            return NULL;
        }
        /* write dummy header */
        for (z = 41; z; --z)
            tdefl_output_buffer_putter(&z, 1, &out_buf);
        /* compress image data */
        tdefl_init(pComp, tdefl_output_buffer_putter, &out_buf, s_tdefl_png_num_probes[MZ_MIN(10, level)] | TDEFL_WRITE_ZLIB_HEADER);
        for (y = 0; y < h; ++y)
        {
            tdefl_compress_buffer(pComp, &z, 1, TDEFL_NO_FLUSH);
            tdefl_compress_buffer(pComp, (mz_uint8 *)pImage + (flip ? (h - 1 - y) : y) * bpl, bpl, TDEFL_NO_FLUSH);
        }
        if (tdefl_compress_buffer(pComp, NULL, 0, TDEFL_FINISH) != TDEFL_STATUS_DONE)
        {
            MZ_FREE(pComp);
            MZ_FREE(out_buf.m_pBuf);
            return NULL;
        }
        /* write real header */
        *pLen_out = out_buf.m_size - 41;
        {
            static const mz_uint8 chans[] = { 0x00, 0x00, 0x04, 0x02, 0x06 };
            mz_uint8 pnghdr[41] = { 0x89, 0x50, 0x4e, 0x47, 0x0d,
                                    0x0a, 0x1a, 0x0a, 0x00, 0x00,
                                    0x00, 0x0d, 0x49, 0x48, 0x44,
                                    0x52, 0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x00, 0x08,
                                    0x00, 0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x00, 0x49, 0x44, 0x41,
                                    0x54 };
            pnghdr[18] = (mz_uint8)(w >> 8);
            pnghdr[19] = (mz_uint8)w;
            pnghdr[22] = (mz_uint8)(h >> 8);
            pnghdr[23] = (mz_uint8)h;
            pnghdr[25] = chans[num_chans];
            pnghdr[33] = (mz_uint8)(*pLen_out >> 24);
            pnghdr[34] = (mz_uint8)(*pLen_out >> 16);
            pnghdr[35] = (mz_uint8)(*pLen_out >> 8);
            pnghdr[36] = (mz_uint8)*pLen_out;
            c = (mz_uint32)mz_crc32(MZ_CRC32_INIT, pnghdr + 12, 17);
            for (i = 0; i < 4; ++i, c <<= 8)
                ((mz_uint8 *)(pnghdr + 29))[i] = (mz_uint8)(c >> 24);
            memcpy(out_buf.m_pBuf, pnghdr, 41);
        }
        /* write footer (IDAT CRC-32, followed by IEND chunk) */
        if (!tdefl_output_buffer_putter("\0\0\0\0\0\0\0\0\x49\x45\x4e\x44\xae\x42\x60\x82", 16, &out_buf))
        {
            *pLen_out = 0;
            MZ_FREE(pComp);
            MZ_FREE(out_buf.m_pBuf);
            return NULL;
        }
        c = (mz_uint32)mz_crc32(MZ_CRC32_INIT, out_buf.m_pBuf + 41 - 4, *pLen_out + 4);
        for (i = 0; i < 4; ++i, c <<= 8)
            (out_buf.m_pBuf + out_buf.m_size - 16)[i] = (mz_uint8)(c >> 24);
        /* compute final size of file, grab compressed data buffer and return */
        *pLen_out += 57;
        MZ_FREE(pComp);
        return out_buf.m_pBuf;
    }
    void *tdefl_write_image_to_png_file_in_memory(const void *pImage, int w, int h, int num_chans, size_t *pLen_out)
    {
        /* Level 6 corresponds to TDEFL_DEFAULT_MAX_PROBES or MZ_DEFAULT_LEVEL (but we can't depend on MZ_DEFAULT_LEVEL being available in case the zlib API's where #defined out) */
        return tdefl_write_image_to_png_file_in_memory_ex(pImage, w, h, num_chans, pLen_out, 6, MZ_FALSE);
    }

#ifndef MINIZ_NO_MALLOC
    /* Allocate the tdefl_compressor and tinfl_decompressor structures in C so that */
    /* non-C language bindings to tdefL_ and tinfl_ API don't need to worry about */
    /* structure size and allocation mechanism. */
    tdefl_compressor *tdefl_compressor_alloc(void)
    {
        return (tdefl_compressor *)MZ_MALLOC(sizeof(tdefl_compressor));
    }

    void tdefl_compressor_free(tdefl_compressor *pComp)
    {
        MZ_FREE(pComp);
    }
#endif

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#ifdef __cplusplus
}
#endif

#endif /*#ifndef MINIZ_NO_DEFLATE_APIS*/


/*
file https://github.com/richgel999/miniz/blob/3.1.0/miniz_tinfl.c
*/
/**************************************************************************
 *
 * Copyright 2013-2014 RAD Game Tools and Valve Software
 * Copyright 2010-2014 Rich Geldreich and Tenacious Software LLC
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 **************************************************************************/

// hack-miniz - inline header
// #include "miniz.h"

#ifndef MINIZ_NO_INFLATE_APIS

#ifdef __cplusplus
extern "C"
{
#endif

    /* ------------------- Low-level Decompression (completely independent from all compression API's) */

#define TINFL_MEMCPY(d, s, l) memcpy(d, s, l)
#define TINFL_MEMSET(p, c, l) memset(p, c, l)

#define TINFL_CR_BEGIN  \
    switch (r->m_state) \
    {                   \
        case 0:
#define TINFL_CR_RETURN(state_index, result) \
    do                                       \
    {                                        \
        status = result;                     \
        r->m_state = state_index;            \
        goto common_exit;                    \
        case state_index:;                   \
    }                                        \
    MZ_MACRO_END
#define TINFL_CR_RETURN_FOREVER(state_index, result) \
    do                                               \
    {                                                \
        for (;;)                                     \
        {                                            \
            TINFL_CR_RETURN(state_index, result);    \
        }                                            \
    }                                                \
    MZ_MACRO_END
#define TINFL_CR_FINISH }

#define TINFL_GET_BYTE(state_index, c)                                                                                                                           \
    do                                                                                                                                                           \
    {                                                                                                                                                            \
        while (pIn_buf_cur >= pIn_buf_end)                                                                                                                       \
        {                                                                                                                                                        \
            TINFL_CR_RETURN(state_index, (decomp_flags & TINFL_FLAG_HAS_MORE_INPUT) ? TINFL_STATUS_NEEDS_MORE_INPUT : TINFL_STATUS_FAILED_CANNOT_MAKE_PROGRESS); \
        }                                                                                                                                                        \
        c = *pIn_buf_cur++;                                                                                                                                      \
    }                                                                                                                                                            \
    MZ_MACRO_END

#define TINFL_NEED_BITS(state_index, n)                \
    do                                                 \
    {                                                  \
        mz_uint c;                                     \
        TINFL_GET_BYTE(state_index, c);                \
        bit_buf |= (((tinfl_bit_buf_t)c) << num_bits); \
        num_bits += 8;                                 \
    } while (num_bits < (mz_uint)(n))
#define TINFL_SKIP_BITS(state_index, n)      \
    do                                       \
    {                                        \
        if (num_bits < (mz_uint)(n))         \
        {                                    \
            TINFL_NEED_BITS(state_index, n); \
        }                                    \
        bit_buf >>= (n);                     \
        num_bits -= (n);                     \
    }                                        \
    MZ_MACRO_END
#define TINFL_GET_BITS(state_index, b, n)    \
    do                                       \
    {                                        \
        if (num_bits < (mz_uint)(n))         \
        {                                    \
            TINFL_NEED_BITS(state_index, n); \
        }                                    \
        b = bit_buf & ((1 << (n)) - 1);      \
        bit_buf >>= (n);                     \
        num_bits -= (n);                     \
    }                                        \
    MZ_MACRO_END

/* TINFL_HUFF_BITBUF_FILL() is only used rarely, when the number of bytes remaining in the input buffer falls below 2. */
/* It reads just enough bytes from the input stream that are needed to decode the next Huffman code (and absolutely no more). It works by trying to fully decode a */
/* Huffman code by using whatever bits are currently present in the bit buffer. If this fails, it reads another byte, and tries again until it succeeds or until the */
/* bit buffer contains >=15 bits (deflate's max. Huffman code size). */
#define TINFL_HUFF_BITBUF_FILL(state_index, pLookUp, pTree)          \
    do                                                               \
    {                                                                \
        temp = pLookUp[bit_buf & (TINFL_FAST_LOOKUP_SIZE - 1)];      \
        if (temp >= 0)                                               \
        {                                                            \
            code_len = temp >> 9;                                    \
            if ((code_len) && (num_bits >= code_len))                \
                break;                                               \
        }                                                            \
        else if (num_bits > TINFL_FAST_LOOKUP_BITS)                  \
        {                                                            \
            code_len = TINFL_FAST_LOOKUP_BITS;                       \
            do                                                       \
            {                                                        \
                temp = pTree[~temp + ((bit_buf >> code_len++) & 1)]; \
            } while ((temp < 0) && (num_bits >= (code_len + 1)));    \
            if (temp >= 0)                                           \
                break;                                               \
        }                                                            \
        TINFL_GET_BYTE(state_index, c);                              \
        bit_buf |= (((tinfl_bit_buf_t)c) << num_bits);               \
        num_bits += 8;                                               \
    } while (num_bits < 15);

/* TINFL_HUFF_DECODE() decodes the next Huffman coded symbol. It's more complex than you would initially expect because the zlib API expects the decompressor to never read */
/* beyond the final byte of the deflate stream. (In other words, when this macro wants to read another byte from the input, it REALLY needs another byte in order to fully */
/* decode the next Huffman code.) Handling this properly is particularly important on raw deflate (non-zlib) streams, which aren't followed by a byte aligned adler-32. */
/* The slow path is only executed at the very end of the input buffer. */
/* v1.16: The original macro handled the case at the very end of the passed-in input buffer, but we also need to handle the case where the user passes in 1+zillion bytes */
/* following the deflate data and our non-conservative read-ahead path won't kick in here on this code. This is much trickier. */
#define TINFL_HUFF_DECODE(state_index, sym, pLookUp, pTree)                                                                         \
    do                                                                                                                              \
    {                                                                                                                               \
        int temp;                                                                                                                   \
        mz_uint code_len, c;                                                                                                        \
        if (num_bits < 15)                                                                                                          \
        {                                                                                                                           \
            if ((pIn_buf_end - pIn_buf_cur) < 2)                                                                                    \
            {                                                                                                                       \
                TINFL_HUFF_BITBUF_FILL(state_index, pLookUp, pTree);                                                                \
            }                                                                                                                       \
            else                                                                                                                    \
            {                                                                                                                       \
                bit_buf |= (((tinfl_bit_buf_t)pIn_buf_cur[0]) << num_bits) | (((tinfl_bit_buf_t)pIn_buf_cur[1]) << (num_bits + 8)); \
                pIn_buf_cur += 2;                                                                                                   \
                num_bits += 16;                                                                                                     \
            }                                                                                                                       \
        }                                                                                                                           \
        if ((temp = pLookUp[bit_buf & (TINFL_FAST_LOOKUP_SIZE - 1)]) >= 0)                                                          \
            code_len = temp >> 9, temp &= 511;                                                                                      \
        else                                                                                                                        \
        {                                                                                                                           \
            code_len = TINFL_FAST_LOOKUP_BITS;                                                                                      \
            do                                                                                                                      \
            {                                                                                                                       \
                temp = pTree[~temp + ((bit_buf >> code_len++) & 1)];                                                                \
            } while (temp < 0);                                                                                                     \
        }                                                                                                                           \
        sym = temp;                                                                                                                 \
        bit_buf >>= code_len;                                                                                                       \
        num_bits -= code_len;                                                                                                       \
    }                                                                                                                               \
    MZ_MACRO_END

    static void tinfl_clear_tree(tinfl_decompressor *r)
    {
        if (r->m_type == 0)
            MZ_CLEAR_ARR(r->m_tree_0);
        else if (r->m_type == 1)
            MZ_CLEAR_ARR(r->m_tree_1);
        else
            MZ_CLEAR_ARR(r->m_tree_2);
    }

    tinfl_status tinfl_decompress(tinfl_decompressor *r, const mz_uint8 *pIn_buf_next, size_t *pIn_buf_size, mz_uint8 *pOut_buf_start, mz_uint8 *pOut_buf_next, size_t *pOut_buf_size, const mz_uint32 decomp_flags)
    {
        static const mz_uint16 s_length_base[31] = { 3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31, 35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258, 0, 0 };
        static const mz_uint8 s_length_extra[31] = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0, 0, 0 };
        static const mz_uint16 s_dist_base[32] = { 1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193, 257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145, 8193, 12289, 16385, 24577, 0, 0 };
        static const mz_uint8 s_dist_extra[32] = { 0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13 };
        static const mz_uint8 s_length_dezigzag[19] = { 16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15 };
        static const mz_uint16 s_min_table_sizes[3] = { 257, 1, 4 };

        mz_int16 *pTrees[3];
        mz_uint8 *pCode_sizes[3];

        tinfl_status status = TINFL_STATUS_FAILED;
        mz_uint32 num_bits, dist, counter, num_extra;
        tinfl_bit_buf_t bit_buf;
        const mz_uint8 *pIn_buf_cur = pIn_buf_next, *const pIn_buf_end = pIn_buf_next + *pIn_buf_size;
        mz_uint8 *pOut_buf_cur = pOut_buf_next, *const pOut_buf_end = pOut_buf_next ? pOut_buf_next + *pOut_buf_size : NULL;
        size_t out_buf_size_mask = (decomp_flags & TINFL_FLAG_USING_NON_WRAPPING_OUTPUT_BUF) ? (size_t)-1 : ((pOut_buf_next - pOut_buf_start) + *pOut_buf_size) - 1, dist_from_out_buf_start;

        /* Ensure the output buffer's size is a power of 2, unless the output buffer is large enough to hold the entire output file (in which case it doesn't matter). */
        if (((out_buf_size_mask + 1) & out_buf_size_mask) || (pOut_buf_next < pOut_buf_start))
        {
            *pIn_buf_size = *pOut_buf_size = 0;
            return TINFL_STATUS_BAD_PARAM;
        }

        pTrees[0] = r->m_tree_0;
        pTrees[1] = r->m_tree_1;
        pTrees[2] = r->m_tree_2;
        pCode_sizes[0] = r->m_code_size_0;
        pCode_sizes[1] = r->m_code_size_1;
        pCode_sizes[2] = r->m_code_size_2;

        num_bits = r->m_num_bits;
        bit_buf = r->m_bit_buf;
        dist = r->m_dist;
        counter = r->m_counter;
        num_extra = r->m_num_extra;
        dist_from_out_buf_start = r->m_dist_from_out_buf_start;
        TINFL_CR_BEGIN

        bit_buf = num_bits = dist = counter = num_extra = r->m_zhdr0 = r->m_zhdr1 = 0;
        r->m_z_adler32 = r->m_check_adler32 = 1;
        if (decomp_flags & TINFL_FLAG_PARSE_ZLIB_HEADER)
        {
            TINFL_GET_BYTE(1, r->m_zhdr0);
            TINFL_GET_BYTE(2, r->m_zhdr1);
            counter = (((r->m_zhdr0 * 256 + r->m_zhdr1) % 31 != 0) || (r->m_zhdr1 & 32) || ((r->m_zhdr0 & 15) != 8));
            if (!(decomp_flags & TINFL_FLAG_USING_NON_WRAPPING_OUTPUT_BUF))
                counter |= (((1U << (8U + (r->m_zhdr0 >> 4))) > 32768U) || ((out_buf_size_mask + 1) < (size_t)((size_t)1 << (8U + (r->m_zhdr0 >> 4)))));
            if (counter)
            {
                TINFL_CR_RETURN_FOREVER(36, TINFL_STATUS_FAILED);
            }
        }

        do
        {
            TINFL_GET_BITS(3, r->m_final, 3);
            r->m_type = r->m_final >> 1;
            if (r->m_type == 0)
            {
                TINFL_SKIP_BITS(5, num_bits & 7);
                for (counter = 0; counter < 4; ++counter)
                {
                    if (num_bits)
                        TINFL_GET_BITS(6, r->m_raw_header[counter], 8);
                    else
                        TINFL_GET_BYTE(7, r->m_raw_header[counter]);
                }
                if ((counter = (r->m_raw_header[0] | (r->m_raw_header[1] << 8))) != (mz_uint)(0xFFFF ^ (r->m_raw_header[2] | (r->m_raw_header[3] << 8))))
                {
                    TINFL_CR_RETURN_FOREVER(39, TINFL_STATUS_FAILED);
                }
                while ((counter) && (num_bits))
                {
                    TINFL_GET_BITS(51, dist, 8);
                    while (pOut_buf_cur >= pOut_buf_end)
                    {
                        TINFL_CR_RETURN(52, TINFL_STATUS_HAS_MORE_OUTPUT);
                    }
                    *pOut_buf_cur++ = (mz_uint8)dist;
                    counter--;
                }
                while (counter)
                {
                    size_t n;
                    while (pOut_buf_cur >= pOut_buf_end)
                    {
                        TINFL_CR_RETURN(9, TINFL_STATUS_HAS_MORE_OUTPUT);
                    }
                    while (pIn_buf_cur >= pIn_buf_end)
                    {
                        TINFL_CR_RETURN(38, (decomp_flags & TINFL_FLAG_HAS_MORE_INPUT) ? TINFL_STATUS_NEEDS_MORE_INPUT : TINFL_STATUS_FAILED_CANNOT_MAKE_PROGRESS);
                    }
                    n = MZ_MIN(MZ_MIN((size_t)(pOut_buf_end - pOut_buf_cur), (size_t)(pIn_buf_end - pIn_buf_cur)), counter);
                    TINFL_MEMCPY(pOut_buf_cur, pIn_buf_cur, n);
                    pIn_buf_cur += n;
                    pOut_buf_cur += n;
                    counter -= (mz_uint)n;
                }
            }
            else if (r->m_type == 3)
            {
                TINFL_CR_RETURN_FOREVER(10, TINFL_STATUS_FAILED);
            }
            else
            {
                if (r->m_type == 1)
                {
                    mz_uint8 *p = r->m_code_size_0;
                    mz_uint i;
                    r->m_table_sizes[0] = 288;
                    r->m_table_sizes[1] = 32;
                    TINFL_MEMSET(r->m_code_size_1, 5, 32);
                    for (i = 0; i <= 143; ++i)
                        *p++ = 8;
                    for (; i <= 255; ++i)
                        *p++ = 9;
                    for (; i <= 279; ++i)
                        *p++ = 7;
                    for (; i <= 287; ++i)
                        *p++ = 8;
                }
                else
                {
                    for (counter = 0; counter < 3; counter++)
                    {
                        TINFL_GET_BITS(11, r->m_table_sizes[counter], "\05\05\04"[counter]);
                        r->m_table_sizes[counter] += s_min_table_sizes[counter];
                    }
                    MZ_CLEAR_ARR(r->m_code_size_2);
                    for (counter = 0; counter < r->m_table_sizes[2]; counter++)
                    {
                        mz_uint s;
                        TINFL_GET_BITS(14, s, 3);
                        r->m_code_size_2[s_length_dezigzag[counter]] = (mz_uint8)s;
                    }
                    r->m_table_sizes[2] = 19;
                }
                for (; (int)r->m_type >= 0; r->m_type--)
                {
                    int tree_next, tree_cur;
                    mz_int16 *pLookUp;
                    mz_int16 *pTree;
                    mz_uint8 *pCode_size;
                    mz_uint i, j, used_syms, total, sym_index, next_code[17], total_syms[16];
                    pLookUp = r->m_look_up[r->m_type];
                    pTree = pTrees[r->m_type];
                    pCode_size = pCode_sizes[r->m_type];
                    MZ_CLEAR_ARR(total_syms);
                    TINFL_MEMSET(pLookUp, 0, sizeof(r->m_look_up[0]));
                    tinfl_clear_tree(r);
                    for (i = 0; i < r->m_table_sizes[r->m_type]; ++i)
                        total_syms[pCode_size[i]]++;
                    used_syms = 0, total = 0;
                    next_code[0] = next_code[1] = 0;
                    for (i = 1; i <= 15; ++i)
                    {
                        used_syms += total_syms[i];
                        next_code[i + 1] = (total = ((total + total_syms[i]) << 1));
                    }
                    if ((65536 != total) && (used_syms > 1))
                    {
                        TINFL_CR_RETURN_FOREVER(35, TINFL_STATUS_FAILED);
                    }
                    for (tree_next = -1, sym_index = 0; sym_index < r->m_table_sizes[r->m_type]; ++sym_index)
                    {
                        mz_uint rev_code = 0, l, cur_code, code_size = pCode_size[sym_index];
                        if (!code_size)
                            continue;
                        cur_code = next_code[code_size]++;
                        for (l = code_size; l > 0; l--, cur_code >>= 1)
                            rev_code = (rev_code << 1) | (cur_code & 1);
                        if (code_size <= TINFL_FAST_LOOKUP_BITS)
                        {
                            mz_int16 k = (mz_int16)((code_size << 9) | sym_index);
                            while (rev_code < TINFL_FAST_LOOKUP_SIZE)
                            {
                                pLookUp[rev_code] = k;
                                rev_code += (1 << code_size);
                            }
                            continue;
                        }
                        if (0 == (tree_cur = pLookUp[rev_code & (TINFL_FAST_LOOKUP_SIZE - 1)]))
                        {
                            pLookUp[rev_code & (TINFL_FAST_LOOKUP_SIZE - 1)] = (mz_int16)tree_next;
                            tree_cur = tree_next;
                            tree_next -= 2;
                        }
                        rev_code >>= (TINFL_FAST_LOOKUP_BITS - 1);
                        for (j = code_size; j > (TINFL_FAST_LOOKUP_BITS + 1); j--)
                        {
                            tree_cur -= ((rev_code >>= 1) & 1);
                            if (!pTree[-tree_cur - 1])
                            {
                                pTree[-tree_cur - 1] = (mz_int16)tree_next;
                                tree_cur = tree_next;
                                tree_next -= 2;
                            }
                            else
                                tree_cur = pTree[-tree_cur - 1];
                        }
                        tree_cur -= ((rev_code >>= 1) & 1);
                        pTree[-tree_cur - 1] = (mz_int16)sym_index;
                    }
                    if (r->m_type == 2)
                    {
                        for (counter = 0; counter < (r->m_table_sizes[0] + r->m_table_sizes[1]);)
                        {
                            mz_uint s;
                            TINFL_HUFF_DECODE(16, dist, r->m_look_up[2], r->m_tree_2);
                            if (dist < 16)
                            {
                                r->m_len_codes[counter++] = (mz_uint8)dist;
                                continue;
                            }
                            if ((dist == 16) && (!counter))
                            {
                                TINFL_CR_RETURN_FOREVER(17, TINFL_STATUS_FAILED);
                            }
                            num_extra = "\02\03\07"[dist - 16];
                            TINFL_GET_BITS(18, s, num_extra);
                            s += "\03\03\013"[dist - 16];
                            TINFL_MEMSET(r->m_len_codes + counter, (dist == 16) ? r->m_len_codes[counter - 1] : 0, s);
                            counter += s;
                        }
                        if ((r->m_table_sizes[0] + r->m_table_sizes[1]) != counter)
                        {
                            TINFL_CR_RETURN_FOREVER(21, TINFL_STATUS_FAILED);
                        }
                        TINFL_MEMCPY(r->m_code_size_0, r->m_len_codes, r->m_table_sizes[0]);
                        TINFL_MEMCPY(r->m_code_size_1, r->m_len_codes + r->m_table_sizes[0], r->m_table_sizes[1]);
                    }
                }
                for (;;)
                {
                    mz_uint8 *pSrc;
                    for (;;)
                    {
                        if (((pIn_buf_end - pIn_buf_cur) < 4) || ((pOut_buf_end - pOut_buf_cur) < 2))
                        {
                            TINFL_HUFF_DECODE(23, counter, r->m_look_up[0], r->m_tree_0);
                            if (counter >= 256)
                                break;
                            while (pOut_buf_cur >= pOut_buf_end)
                            {
                                TINFL_CR_RETURN(24, TINFL_STATUS_HAS_MORE_OUTPUT);
                            }
                            *pOut_buf_cur++ = (mz_uint8)counter;
                        }
                        else
                        {
                            int sym2;
                            mz_uint code_len;
#if TINFL_USE_64BIT_BITBUF
                            if (num_bits < 30)
                            {
                                bit_buf |= (((tinfl_bit_buf_t)MZ_READ_LE32(pIn_buf_cur)) << num_bits);
                                pIn_buf_cur += 4;
                                num_bits += 32;
                            }
#else
                        if (num_bits < 15)
                        {
                            bit_buf |= (((tinfl_bit_buf_t)MZ_READ_LE16(pIn_buf_cur)) << num_bits);
                            pIn_buf_cur += 2;
                            num_bits += 16;
                        }
#endif
                            if ((sym2 = r->m_look_up[0][bit_buf & (TINFL_FAST_LOOKUP_SIZE - 1)]) >= 0)
                                code_len = sym2 >> 9;
                            else
                            {
                                code_len = TINFL_FAST_LOOKUP_BITS;
                                do
                                {
                                    sym2 = r->m_tree_0[~sym2 + ((bit_buf >> code_len++) & 1)];
                                } while (sym2 < 0);
                            }
                            counter = sym2;
                            bit_buf >>= code_len;
                            num_bits -= code_len;
                            if (counter & 256)
                                break;

#if !TINFL_USE_64BIT_BITBUF
                            if (num_bits < 15)
                            {
                                bit_buf |= (((tinfl_bit_buf_t)MZ_READ_LE16(pIn_buf_cur)) << num_bits);
                                pIn_buf_cur += 2;
                                num_bits += 16;
                            }
#endif
                            if ((sym2 = r->m_look_up[0][bit_buf & (TINFL_FAST_LOOKUP_SIZE - 1)]) >= 0)
                                code_len = sym2 >> 9;
                            else
                            {
                                code_len = TINFL_FAST_LOOKUP_BITS;
                                do
                                {
                                    sym2 = r->m_tree_0[~sym2 + ((bit_buf >> code_len++) & 1)];
                                } while (sym2 < 0);
                            }
                            bit_buf >>= code_len;
                            num_bits -= code_len;

                            pOut_buf_cur[0] = (mz_uint8)counter;
                            if (sym2 & 256)
                            {
                                pOut_buf_cur++;
                                counter = sym2;
                                break;
                            }
                            pOut_buf_cur[1] = (mz_uint8)sym2;
                            pOut_buf_cur += 2;
                        }
                    }
                    if ((counter &= 511) == 256)
                        break;

                    num_extra = s_length_extra[counter - 257];
                    counter = s_length_base[counter - 257];
                    if (num_extra)
                    {
                        mz_uint extra_bits;
                        TINFL_GET_BITS(25, extra_bits, num_extra);
                        counter += extra_bits;
                    }

                    TINFL_HUFF_DECODE(26, dist, r->m_look_up[1], r->m_tree_1);
                    num_extra = s_dist_extra[dist];
                    dist = s_dist_base[dist];
                    if (num_extra)
                    {
                        mz_uint extra_bits;
                        TINFL_GET_BITS(27, extra_bits, num_extra);
                        dist += extra_bits;
                    }

                    dist_from_out_buf_start = pOut_buf_cur - pOut_buf_start;
                    if ((dist == 0 || dist > dist_from_out_buf_start || dist_from_out_buf_start == 0) && (decomp_flags & TINFL_FLAG_USING_NON_WRAPPING_OUTPUT_BUF))
                    {
                        TINFL_CR_RETURN_FOREVER(37, TINFL_STATUS_FAILED);
                    }

                    pSrc = pOut_buf_start + ((dist_from_out_buf_start - dist) & out_buf_size_mask);

                    if ((MZ_MAX(pOut_buf_cur, pSrc) + counter) > pOut_buf_end)
                    {
                        while (counter--)
                        {
                            while (pOut_buf_cur >= pOut_buf_end)
                            {
                                TINFL_CR_RETURN(53, TINFL_STATUS_HAS_MORE_OUTPUT);
                            }
                            *pOut_buf_cur++ = pOut_buf_start[(dist_from_out_buf_start++ - dist) & out_buf_size_mask];
                        }
                        continue;
                    }
#if MINIZ_USE_UNALIGNED_LOADS_AND_STORES
                    else if ((counter >= 9) && (counter <= dist))
                    {
                        const mz_uint8 *pSrc_end = pSrc + (counter & ~7);
                        do
                        {
#ifdef MINIZ_UNALIGNED_USE_MEMCPY
                            memcpy(pOut_buf_cur, pSrc, sizeof(mz_uint32) * 2);
#else
                            ((mz_uint32 *)pOut_buf_cur)[0] = ((const mz_uint32 *)pSrc)[0];
                            ((mz_uint32 *)pOut_buf_cur)[1] = ((const mz_uint32 *)pSrc)[1];
#endif
                            pOut_buf_cur += 8;
                        } while ((pSrc += 8) < pSrc_end);
                        if ((counter &= 7) < 3)
                        {
                            if (counter)
                            {
                                pOut_buf_cur[0] = pSrc[0];
                                if (counter > 1)
                                    pOut_buf_cur[1] = pSrc[1];
                                pOut_buf_cur += counter;
                            }
                            continue;
                        }
                    }
#endif
                    while (counter > 2)
                    {
                        pOut_buf_cur[0] = pSrc[0];
                        pOut_buf_cur[1] = pSrc[1];
                        pOut_buf_cur[2] = pSrc[2];
                        pOut_buf_cur += 3;
                        pSrc += 3;
                        counter -= 3;
                    }
                    if (counter > 0)
                    {
                        pOut_buf_cur[0] = pSrc[0];
                        if (counter > 1)
                            pOut_buf_cur[1] = pSrc[1];
                        pOut_buf_cur += counter;
                    }
                }
            }
        } while (!(r->m_final & 1));

        /* Ensure byte alignment and put back any bytes from the bitbuf if we've looked ahead too far on gzip, or other Deflate streams followed by arbitrary data. */
        /* I'm being super conservative here. A number of simplifications can be made to the byte alignment part, and the Adler32 check shouldn't ever need to worry about reading from the bitbuf now. */
        TINFL_SKIP_BITS(32, num_bits & 7);
        while ((pIn_buf_cur > pIn_buf_next) && (num_bits >= 8))
        {
            --pIn_buf_cur;
            num_bits -= 8;
        }
        bit_buf &= ~(~(tinfl_bit_buf_t)0 << num_bits);
        MZ_ASSERT(!num_bits); /* if this assert fires then we've read beyond the end of non-deflate/zlib streams with following data (such as gzip streams). */

        if (decomp_flags & TINFL_FLAG_PARSE_ZLIB_HEADER)
        {
            for (counter = 0; counter < 4; ++counter)
            {
                mz_uint s;
                if (num_bits)
                    TINFL_GET_BITS(41, s, 8);
                else
                    TINFL_GET_BYTE(42, s);
                r->m_z_adler32 = (r->m_z_adler32 << 8) | s;
            }
        }
        TINFL_CR_RETURN_FOREVER(34, TINFL_STATUS_DONE);

        TINFL_CR_FINISH

    common_exit:
        /* As long as we aren't telling the caller that we NEED more input to make forward progress: */
        /* Put back any bytes from the bitbuf in case we've looked ahead too far on gzip, or other Deflate streams followed by arbitrary data. */
        /* We need to be very careful here to NOT push back any bytes we definitely know we need to make forward progress, though, or we'll lock the caller up into an inf loop. */
        if ((status != TINFL_STATUS_NEEDS_MORE_INPUT) && (status != TINFL_STATUS_FAILED_CANNOT_MAKE_PROGRESS))
        {
            while ((pIn_buf_cur > pIn_buf_next) && (num_bits >= 8))
            {
                --pIn_buf_cur;
                num_bits -= 8;
            }
        }
        r->m_num_bits = num_bits;
        r->m_bit_buf = bit_buf & ~(~(tinfl_bit_buf_t)0 << num_bits);
        r->m_dist = dist;
        r->m_counter = counter;
        r->m_num_extra = num_extra;
        r->m_dist_from_out_buf_start = dist_from_out_buf_start;
        *pIn_buf_size = pIn_buf_cur - pIn_buf_next;
        *pOut_buf_size = pOut_buf_cur - pOut_buf_next;
        if ((decomp_flags & (TINFL_FLAG_PARSE_ZLIB_HEADER | TINFL_FLAG_COMPUTE_ADLER32)) && (status >= 0))
        {
            const mz_uint8 *ptr = pOut_buf_next;
            size_t buf_len = *pOut_buf_size;
            mz_uint32 i, s1 = r->m_check_adler32 & 0xffff, s2 = r->m_check_adler32 >> 16;
            size_t block_len = buf_len % 5552;
            while (buf_len)
            {
                for (i = 0; i + 7 < block_len; i += 8, ptr += 8)
                {
                    s1 += ptr[0], s2 += s1;
                    s1 += ptr[1], s2 += s1;
                    s1 += ptr[2], s2 += s1;
                    s1 += ptr[3], s2 += s1;
                    s1 += ptr[4], s2 += s1;
                    s1 += ptr[5], s2 += s1;
                    s1 += ptr[6], s2 += s1;
                    s1 += ptr[7], s2 += s1;
                }
                for (; i < block_len; ++i)
                    s1 += *ptr++, s2 += s1;
                s1 %= 65521U, s2 %= 65521U;
                buf_len -= block_len;
                block_len = 5552;
            }
            r->m_check_adler32 = (s2 << 16) + s1;
            if ((status == TINFL_STATUS_DONE) && (decomp_flags & TINFL_FLAG_PARSE_ZLIB_HEADER) && (r->m_check_adler32 != r->m_z_adler32))
                status = TINFL_STATUS_ADLER32_MISMATCH;
        }
        return status;
    }

    /* Higher level helper functions. */
    void *tinfl_decompress_mem_to_heap(const void *pSrc_buf, size_t src_buf_len, size_t *pOut_len, int flags)
    {
        tinfl_decompressor decomp;
        void *pBuf = NULL, *pNew_buf;
        size_t src_buf_ofs = 0, out_buf_capacity = 0;
        *pOut_len = 0;
        tinfl_init(&decomp);
        for (;;)
        {
            size_t src_buf_size = src_buf_len - src_buf_ofs, dst_buf_size = out_buf_capacity - *pOut_len, new_out_buf_capacity;
            tinfl_status status = tinfl_decompress(&decomp, (const mz_uint8 *)pSrc_buf + src_buf_ofs, &src_buf_size, (mz_uint8 *)pBuf, pBuf ? (mz_uint8 *)pBuf + *pOut_len : NULL, &dst_buf_size,
                                                   (flags & ~TINFL_FLAG_HAS_MORE_INPUT) | TINFL_FLAG_USING_NON_WRAPPING_OUTPUT_BUF);
            if ((status < 0) || (status == TINFL_STATUS_NEEDS_MORE_INPUT))
            {
                MZ_FREE(pBuf);
                *pOut_len = 0;
                return NULL;
            }
            src_buf_ofs += src_buf_size;
            *pOut_len += dst_buf_size;
            if (status == TINFL_STATUS_DONE)
                break;
            new_out_buf_capacity = out_buf_capacity * 2;
            if (new_out_buf_capacity < 128)
                new_out_buf_capacity = 128;
            pNew_buf = MZ_REALLOC(pBuf, new_out_buf_capacity);
            if (!pNew_buf)
            {
                MZ_FREE(pBuf);
                *pOut_len = 0;
                return NULL;
            }
            pBuf = pNew_buf;
            out_buf_capacity = new_out_buf_capacity;
        }
        return pBuf;
    }

    size_t tinfl_decompress_mem_to_mem(void *pOut_buf, size_t out_buf_len, const void *pSrc_buf, size_t src_buf_len, int flags)
    {
        tinfl_decompressor decomp;
        tinfl_status status;
        tinfl_init(&decomp);
        status = tinfl_decompress(&decomp, (const mz_uint8 *)pSrc_buf, &src_buf_len, (mz_uint8 *)pOut_buf, (mz_uint8 *)pOut_buf, &out_buf_len, (flags & ~TINFL_FLAG_HAS_MORE_INPUT) | TINFL_FLAG_USING_NON_WRAPPING_OUTPUT_BUF);
        return (status != TINFL_STATUS_DONE) ? TINFL_DECOMPRESS_MEM_TO_MEM_FAILED : out_buf_len;
    }

    int tinfl_decompress_mem_to_callback(const void *pIn_buf, size_t *pIn_buf_size, tinfl_put_buf_func_ptr pPut_buf_func, void *pPut_buf_user, int flags)
    {
        int result = 0;
        tinfl_decompressor decomp;
        mz_uint8 *pDict = (mz_uint8 *)MZ_MALLOC(TINFL_LZ_DICT_SIZE);
        size_t in_buf_ofs = 0, dict_ofs = 0;
        if (!pDict)
            return TINFL_STATUS_FAILED;
        memset(pDict, 0, TINFL_LZ_DICT_SIZE);
        tinfl_init(&decomp);
        for (;;)
        {
            size_t in_buf_size = *pIn_buf_size - in_buf_ofs, dst_buf_size = TINFL_LZ_DICT_SIZE - dict_ofs;
            tinfl_status status = tinfl_decompress(&decomp, (const mz_uint8 *)pIn_buf + in_buf_ofs, &in_buf_size, pDict, pDict + dict_ofs, &dst_buf_size,
                                                   (flags & ~(TINFL_FLAG_HAS_MORE_INPUT | TINFL_FLAG_USING_NON_WRAPPING_OUTPUT_BUF)));
            in_buf_ofs += in_buf_size;
            if ((dst_buf_size) && (!(*pPut_buf_func)(pDict + dict_ofs, (int)dst_buf_size, pPut_buf_user)))
                break;
            if (status != TINFL_STATUS_HAS_MORE_OUTPUT)
            {
                result = (status == TINFL_STATUS_DONE);
                break;
            }
            dict_ofs = (dict_ofs + dst_buf_size) & (TINFL_LZ_DICT_SIZE - 1);
        }
        MZ_FREE(pDict);
        *pIn_buf_size = in_buf_ofs;
        return result;
    }

#ifndef MINIZ_NO_MALLOC
    tinfl_decompressor *tinfl_decompressor_alloc(void)
    {
        tinfl_decompressor *pDecomp = (tinfl_decompressor *)MZ_MALLOC(sizeof(tinfl_decompressor));
        if (pDecomp)
            tinfl_init(pDecomp);
        return pDecomp;
    }

    void tinfl_decompressor_free(tinfl_decompressor *pDecomp)
    {
        MZ_FREE(pDecomp);
    }
#endif

#ifdef __cplusplus
}
#endif

#endif /*#ifndef MINIZ_NO_INFLATE_APIS*/
#endif // SRC_SQLMATH_BASE_C2


/*
repo https://github.com/intel/tinycrypt/tree/v0.2.8
committed 2017-08-29T21:21:56Z
*/


/*
file https://github.com/intel/tinycrypt/blob/v0.2.8/lib/include/tinycrypt/sha256.h
*/
/* sha256.h - TinyCrypt interface to a SHA-256 implementation */

/*
 *  Copyright (C) 2017 by Intel Corporation, All Rights Reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *    - Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *    - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 *    - Neither the name of Intel Corporation nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file
 * @brief Interface to a SHA-256 implementation.
 *
 *  Overview:   SHA-256 is a NIST approved cryptographic hashing algorithm
 *              specified in FIPS 180. A hash algorithm maps data of arbitrary
 *              size to data of fixed length.
 *
 *  Security:   SHA-256 provides 128 bits of security against collision attacks
 *              and 256 bits of security against pre-image attacks. SHA-256 does
 *              NOT behave like a random oracle, but it can be used as one if
 *              the string being hashed is prefix-free encoded before hashing.
 *
 *  Usage:      1) call tc_sha256_init to initialize a struct
 *              tc_sha256_state_struct before hashing a new string.
 *
 *              2) call tc_sha256_update to hash the next string segment;
 *              tc_sha256_update can be called as many times as needed to hash
 *              all of the segments of a string; the order is important.
 *
 *              3) call tc_sha256_final to out put the digest from a hashing
 *              operation.
 */

#ifndef __TC_SHA256_H__
#define __TC_SHA256_H__

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TC_SHA256_BLOCK_SIZE (64)
#define TC_SHA256_DIGEST_SIZE (32)
#define TC_SHA256_STATE_BLOCKS (TC_SHA256_DIGEST_SIZE/4)

struct tc_sha256_state_struct {
	unsigned int iv[TC_SHA256_STATE_BLOCKS];
	uint64_t bits_hashed;
	uint8_t leftover[TC_SHA256_BLOCK_SIZE];
	size_t leftover_offset;
};

typedef struct tc_sha256_state_struct *TCSha256State_t;

/**
 *  @brief SHA256 initialization procedure
 *  Initializes s
 *  @return returns TC_CRYPTO_SUCCESS (1)
 *          returns TC_CRYPTO_FAIL (0) if s == NULL
 *  @param s Sha256 state struct
 */
int tc_sha256_init(TCSha256State_t s);

/**
 *  @brief SHA256 update procedure
 *  Hashes data_length bytes addressed by data into state s
 *  @return returns TC_CRYPTO_SUCCESS (1)
 *          returns TC_CRYPTO_FAIL (0) if:
 *                s == NULL,
 *                s->iv == NULL,
 *                data == NULL
 *  @note Assumes s has been initialized by tc_sha256_init
 *  @warning The state buffer 'leftover' is left in memory after processing
 *           If your application intends to have sensitive data in this
 *           buffer, remind to erase it after the data has been processed
 *  @param s Sha256 state struct
 *  @param data message to hash
 *  @param datalen length of message to hash
 */
int tc_sha256_update (TCSha256State_t s, const uint8_t *data, size_t datalen);

/**
 *  @brief SHA256 final procedure
 *  Inserts the completed hash computation into digest
 *  @return returns TC_CRYPTO_SUCCESS (1)
 *          returns TC_CRYPTO_FAIL (0) if:
 *                s == NULL,
 *                s->iv == NULL,
 *                digest == NULL
 *  @note Assumes: s has been initialized by tc_sha256_init
 *        digest points to at least TC_SHA256_DIGEST_SIZE bytes
 *  @warning The state buffer 'leftover' is left in memory after processing
 *           If your application intends to have sensitive data in this
 *           buffer, remind to erase it after the data has been processed
 *  @param digest unsigned eight bit integer
 *  @param Sha256 state struct
 */
int tc_sha256_final(uint8_t *digest, TCSha256State_t s);

#ifdef __cplusplus
}
#endif

#endif /* __TC_SHA256_H__ */


/*
file https://github.com/intel/tinycrypt/blob/v0.2.8/lib/include/tinycrypt/constants.h
*/

#if defined(SRC_SQLMATH_BASE_C2)
/* constants.h - TinyCrypt interface to constants */

/*
 *  Copyright (C) 2017 by Intel Corporation, All Rights Reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *    - Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *    - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 *    - Neither the name of Intel Corporation nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file
 * @brief -- Interface to constants.
 *
 */

#ifndef __TC_CONSTANTS_H__
#define __TC_CONSTANTS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#ifndef NULL
#define NULL ((void *)0)
#endif

#define TC_CRYPTO_SUCCESS 1
#define TC_CRYPTO_FAIL 0

#define TC_ZERO_BYTE 0x00

#ifdef __cplusplus
}
#endif

#endif /* __TC_CONSTANTS_H__ */


/*
file https://github.com/intel/tinycrypt/blob/v0.2.8/lib/include/tinycrypt/utils.h
*/
/* utils.h - TinyCrypt interface to platform-dependent run-time operations */

/*
 *  Copyright (C) 2017 by Intel Corporation, All Rights Reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *    - Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *    - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 *    - Neither the name of Intel Corporation nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file
 * @brief Interface to platform-dependent run-time operations.
 *
 */

#ifndef __TC_UTILS_H__
#define __TC_UTILS_H__

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Copy the the buffer 'from' to the buffer 'to'.
 * @return returns TC_CRYPTO_SUCCESS (1)
 *         returns TC_CRYPTO_FAIL (0) if:
 *                from_len > to_len.
 *
 * @param to OUT -- destination buffer
 * @param to_len IN -- length of destination buffer
 * @param from IN -- origin buffer
 * @param from_len IN -- length of origin buffer
 */
unsigned int _copy(uint8_t *to, unsigned int to_len,
	           const uint8_t *from, unsigned int from_len);

/**
 * @brief Set the value 'val' into the buffer 'to', 'len' times.
 *
 * @param to OUT -- destination buffer
 * @param val IN -- value to be set in 'to'
 * @param len IN -- number of times the value will be copied
 */
void _set(void *to, uint8_t val, size_t len);

/*
 * @brief AES specific doubling function, which utilizes
 * the finite field used by AES.
 * @return Returns a^2
 *
 * @param a IN/OUT -- value to be doubled
 */
uint8_t _double_byte(uint8_t a);

/*
 * @brief Constant-time algorithm to compare if two sequences of bytes are equal
 * @return Returns 0 if equal, and non-zero otherwise
 *
 * @param a IN -- sequence of bytes a
 * @param b IN -- sequence of bytes b
 * @param size IN -- size of sequences a and b
 */
int _compare(const uint8_t *a, const uint8_t *b, size_t size);

#ifdef __cplusplus
}
#endif

#endif /* __TC_UTILS_H__ */


/*
file https://github.com/intel/tinycrypt/blob/v0.2.8/lib/source/utils.c
*/
/* utils.c - TinyCrypt platform-dependent run-time operations */

/*
 *  Copyright (C) 2017 by Intel Corporation, All Rights Reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *    - Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *    - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 *    - Neither the name of Intel Corporation nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

// hack-nodejs - inline header
// #include <tinycrypt/utils.h>
// hack-nodejs - inline header
// #include <tinycrypt/constants.h>

#include <string.h>

#define MASK_TWENTY_SEVEN 0x1b

unsigned int _copy(uint8_t *to, unsigned int to_len,
		   const uint8_t *from, unsigned int from_len)
{
	if (from_len <= to_len) {
		(void)memcpy(to, from, from_len);
		return from_len;
	} else {
		return TC_CRYPTO_FAIL;
	}
}

void _set(void *to, uint8_t val, size_t len)
{
	(void)memset(to, val, len);
}

/*
 * Doubles the value of a byte for values up to 127.
 */
uint8_t _double_byte(uint8_t a)
{
	return ((a<<1) ^ ((a>>7) * MASK_TWENTY_SEVEN));
}

int _compare(const uint8_t *a, const uint8_t *b, size_t size)
{
	const uint8_t *tempa = a;
	const uint8_t *tempb = b;
	uint8_t result = 0;

	for (unsigned int i = 0; i < size; i++) {
		result |= tempa[i] ^ tempb[i];
	}
	return result;
}


/*
file https://github.com/intel/tinycrypt/blob/v0.2.8/lib/source/sha256.c
*/
/* sha256.c - TinyCrypt SHA-256 crypto hash algorithm implementation */

/*
 *  Copyright (C) 2017 by Intel Corporation, All Rights Reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *    - Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *    - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 *    - Neither the name of Intel Corporation nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

// hack-nodejs - inline header
// #include <tinycrypt/sha256.h>
// hack-nodejs - inline header
// #include <tinycrypt/constants.h>
// hack-nodejs - inline header
// #include <tinycrypt/utils.h>

static void tc_sha256_compress(unsigned int *iv, const uint8_t *data);

int tc_sha256_init(TCSha256State_t s)
{
	/* input sanity check: */
	if (s == (TCSha256State_t) 0) {
		return TC_CRYPTO_FAIL;
	}

	/*
	 * Setting the initial state values.
	 * These values correspond to the first 32 bits of the fractional parts
	 * of the square roots of the first 8 primes: 2, 3, 5, 7, 11, 13, 17
	 * and 19.
	 */
	_set((uint8_t *) s, 0x00, sizeof(*s));
	s->iv[0] = 0x6a09e667;
	s->iv[1] = 0xbb67ae85;
	s->iv[2] = 0x3c6ef372;
	s->iv[3] = 0xa54ff53a;
	s->iv[4] = 0x510e527f;
	s->iv[5] = 0x9b05688c;
	s->iv[6] = 0x1f83d9ab;
	s->iv[7] = 0x5be0cd19;

	return TC_CRYPTO_SUCCESS;
}

int tc_sha256_update(TCSha256State_t s, const uint8_t *data, size_t datalen)
{
	/* input sanity check: */
	if (s == (TCSha256State_t) 0 ||
	    data == (void *) 0) {
		return TC_CRYPTO_FAIL;
	} else if (datalen == 0) {
		return TC_CRYPTO_SUCCESS;
	}

	while (datalen-- > 0) {
		s->leftover[s->leftover_offset++] = *(data++);
		if (s->leftover_offset >= TC_SHA256_BLOCK_SIZE) {
			tc_sha256_compress(s->iv, s->leftover);
			s->leftover_offset = 0;
			s->bits_hashed += (TC_SHA256_BLOCK_SIZE << 3);
		}
	}

	return TC_CRYPTO_SUCCESS;
}

int tc_sha256_final(uint8_t *digest, TCSha256State_t s)
{
	unsigned int i;

	/* input sanity check: */
	if (digest == (uint8_t *) 0 ||
	    s == (TCSha256State_t) 0) {
		return TC_CRYPTO_FAIL;
	}

	s->bits_hashed += (s->leftover_offset << 3);

	s->leftover[s->leftover_offset++] = 0x80; /* always room for one byte */
	if (s->leftover_offset > (sizeof(s->leftover) - 8)) {
		/* there is not room for all the padding in this block */
		_set(s->leftover + s->leftover_offset, 0x00,
		     sizeof(s->leftover) - s->leftover_offset);
		tc_sha256_compress(s->iv, s->leftover);
		s->leftover_offset = 0;
	}

	/* add the padding and the length in big-Endian format */
	_set(s->leftover + s->leftover_offset, 0x00,
	     sizeof(s->leftover) - 8 - s->leftover_offset);
	s->leftover[sizeof(s->leftover) - 1] = (uint8_t)(s->bits_hashed);
	s->leftover[sizeof(s->leftover) - 2] = (uint8_t)(s->bits_hashed >> 8);
	s->leftover[sizeof(s->leftover) - 3] = (uint8_t)(s->bits_hashed >> 16);
	s->leftover[sizeof(s->leftover) - 4] = (uint8_t)(s->bits_hashed >> 24);
	s->leftover[sizeof(s->leftover) - 5] = (uint8_t)(s->bits_hashed >> 32);
	s->leftover[sizeof(s->leftover) - 6] = (uint8_t)(s->bits_hashed >> 40);
	s->leftover[sizeof(s->leftover) - 7] = (uint8_t)(s->bits_hashed >> 48);
	s->leftover[sizeof(s->leftover) - 8] = (uint8_t)(s->bits_hashed >> 56);

	/* hash the padding and length */
	tc_sha256_compress(s->iv, s->leftover);

	/* copy the iv out to digest */
	for (i = 0; i < TC_SHA256_STATE_BLOCKS; ++i) {
		unsigned int t = *((unsigned int *) &s->iv[i]);
		*digest++ = (uint8_t)(t >> 24);
		*digest++ = (uint8_t)(t >> 16);
		*digest++ = (uint8_t)(t >> 8);
		*digest++ = (uint8_t)(t);
	}

	/* destroy the current state */
	_set(s, 0, sizeof(*s));

	return TC_CRYPTO_SUCCESS;
}

/*
 * Initializing SHA-256 Hash constant words K.
 * These values correspond to the first 32 bits of the fractional parts of the
 * cube roots of the first 64 primes between 2 and 311.
 */
static const unsigned int k256[64] = {
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1,
	0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
	0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786,
	0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147,
	0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
	0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b,
	0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a,
	0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
	0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

static inline unsigned int ROTR(unsigned int a, unsigned int n)
{
	return (((a) >> n) | ((a) << (32 - n)));
}

#define Sigma0(a)(ROTR((a), 2) ^ ROTR((a), 13) ^ ROTR((a), 22))
#define Sigma1(a)(ROTR((a), 6) ^ ROTR((a), 11) ^ ROTR((a), 25))
#define sigma0(a)(ROTR((a), 7) ^ ROTR((a), 18) ^ ((a) >> 3))
#define sigma1(a)(ROTR((a), 17) ^ ROTR((a), 19) ^ ((a) >> 10))

#define Ch(a, b, c)(((a) & (b)) ^ ((~(a)) & (c)))
#define Maj(a, b, c)(((a) & (b)) ^ ((a) & (c)) ^ ((b) & (c)))

static inline unsigned int BigEndian(const uint8_t **c)
{
	unsigned int n = 0;

	n = (((unsigned int)(*((*c)++))) << 24);
	n |= ((unsigned int)(*((*c)++)) << 16);
	n |= ((unsigned int)(*((*c)++)) << 8);
	n |= ((unsigned int)(*((*c)++)));
	return n;
}

static void tc_sha256_compress(unsigned int *iv, const uint8_t *data)
{
	unsigned int a, b, c, d, e, f, g, h;
	unsigned int s0, s1;
	unsigned int t1, t2;
	unsigned int work_space[16];
	unsigned int n;
	unsigned int i;

	a = iv[0]; b = iv[1]; c = iv[2]; d = iv[3];
	e = iv[4]; f = iv[5]; g = iv[6]; h = iv[7];

	for (i = 0; i < 16; ++i) {
		n = BigEndian(&data);
		t1 = work_space[i] = n;
		t1 += h + Sigma1(e) + Ch(e, f, g) + k256[i];
		t2 = Sigma0(a) + Maj(a, b, c);
		h = g; g = f; f = e; e = d + t1;
		d = c; c = b; b = a; a = t1 + t2;
	}

	for ( ; i < 64; ++i) {
		s0 = work_space[(i+1)&0x0f];
		s0 = sigma0(s0);
		s1 = work_space[(i+14)&0x0f];
		s1 = sigma1(s1);

		t1 = work_space[i&0xf] += s0 + s1 + work_space[(i+9)&0xf];
		t1 += h + Sigma1(e) + Ch(e, f, g) + k256[i];
		t2 = Sigma0(a) + Maj(a, b, c);
		h = g; g = f; f = e; e = d + t1;
		d = c; c = b; b = a; a = t1 + t2;
	}

	iv[0] += a; iv[1] += b; iv[2] += c; iv[3] += d;
	iv[4] += e; iv[5] += f; iv[6] += g; iv[7] += h;
}
#endif // SRC_SQLMATH_BASE_C2


/*
repo https://github.com/microsoft/LightGBM/tree/v4.6.0
committed 2025-02-14T20:27:28Z
*/


/*
file https://github.com/microsoft/LightGBM/blob/v4.6.0/include/LightGBM/c_api.h
*/

#if defined(SRC_SQLMATH_BASE_C2)
/*!
 * \file c_api.h
 * \copyright Copyright (c) 2016 Microsoft Corporation. All rights reserved.
 *            Licensed under the MIT License. See LICENSE file in the project root for license information.
 * \note
 * To avoid type conversion on large data, the most of our exposed interface supports both float32 and float64,
 * except the following:
 * 1. gradient and Hessian;
 * 2. current score for training and validation data.
 * .
 * The reason is that they are called frequently, and the type conversion on them may be time-cost.
 */
#ifndef LIGHTGBM_C_API_H_
#define LIGHTGBM_C_API_H_

// hack-nodejs - inline header
// #include <LightGBM/arrow.h>
// hack-nodejs - inline header
// #include <LightGBM/export.h>

#ifdef __cplusplus
#include <cstdint>
#include <cstdio>
#include <cstring>
#else
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#endif


typedef void* DatasetHandle;  /*!< \brief Handle of dataset. */
typedef void* BoosterHandle;  /*!< \brief Handle of booster. */
typedef void* FastConfigHandle; /*!< \brief Handle of FastConfig. */
typedef void* ByteBufferHandle; /*!< \brief Handle of ByteBuffer. */

#define C_API_DTYPE_FLOAT32 (0)  /*!< \brief float32 (single precision float). */
#define C_API_DTYPE_FLOAT64 (1)  /*!< \brief float64 (double precision float). */
#define C_API_DTYPE_INT32   (2)  /*!< \brief int32. */
#define C_API_DTYPE_INT64   (3)  /*!< \brief int64. */

#define C_API_PREDICT_NORMAL     (0)  /*!< \brief Normal prediction, with transform (if needed). */
#define C_API_PREDICT_RAW_SCORE  (1)  /*!< \brief Predict raw score. */
#define C_API_PREDICT_LEAF_INDEX (2)  /*!< \brief Predict leaf index. */
#define C_API_PREDICT_CONTRIB    (3)  /*!< \brief Predict feature contributions (SHAP values). */

#define C_API_MATRIX_TYPE_CSR (0)  /*!< \brief CSR sparse matrix type. */
#define C_API_MATRIX_TYPE_CSC (1)  /*!< \brief CSC sparse matrix type. */

#define C_API_FEATURE_IMPORTANCE_SPLIT (0)  /*!< \brief Split type of feature importance. */
#define C_API_FEATURE_IMPORTANCE_GAIN  (1)  /*!< \brief Gain type of feature importance. */

/*!
 * \brief Get string message of the last error.
 * \return Error information
 */
LIGHTGBM_C_EXPORT const char* (*LGBM_GetLastError_t) ();
static LGBM_GetLastError_t LGBM_GetLastError = NULL;

/*!
 * \brief Dump all parameter names with their aliases to JSON.
 * \param buffer_len String buffer length, if ``buffer_len < out_len``, you should re-allocate buffer
 * \param[out] out_len Actual output length
 * \param[out] out_str JSON format string of parameters, should pre-allocate memory
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DumpParamAliases_t) (int64_t buffer_len,
                                            int64_t* out_len,
                                            char* out_str);
static LGBM_DumpParamAliases_t LGBM_DumpParamAliases = NULL;

/*!
 * \brief Register a callback function for log redirecting.
 * \param callback The callback function to register
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_RegisterLogCallback_t) (void (*callback)(const char*));
static LGBM_RegisterLogCallback_t LGBM_RegisterLogCallback = NULL;

/*!
 * \brief Get number of samples based on parameters and total number of rows of data.
 * \param num_total_row Number of total rows
 * \param parameters Additional parameters, namely, ``bin_construct_sample_cnt`` is used to calculate returned value
 * \param[out] out Number of samples. This value is used to pre-allocate memory to hold sample indices when calling ``LGBM_SampleIndices``
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_GetSampleCount_t) (int32_t num_total_row,
                                          const char* parameters,
                                          int* out);
static LGBM_GetSampleCount_t LGBM_GetSampleCount = NULL;

/*!
 * \brief Create sample indices for total number of rows.
 * \note
 * You should pre-allocate memory for ``out``, you can get its length by ``LGBM_GetSampleCount``.
 * \param num_total_row Number of total rows
 * \param parameters Additional parameters, namely, ``bin_construct_sample_cnt`` and ``data_random_seed`` are used to produce the output
 * \param[out] out Created indices, type is int32_t
 * \param[out] out_len Number of indices
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_SampleIndices_t) (int32_t num_total_row,
                                         const char* parameters,
                                         void* out,
                                         int32_t* out_len);
static LGBM_SampleIndices_t LGBM_SampleIndices = NULL;

/*!
 * \brief Get a ByteBuffer value at an index.
 * \param handle Handle of byte buffer to be read
 * \param index Index of value to return
 * \param[out] out_val Byte value at index to return
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_ByteBufferGetAt_t) (ByteBufferHandle handle, int32_t index, uint8_t* out_val);
static LGBM_ByteBufferGetAt_t LGBM_ByteBufferGetAt = NULL;

/*!
 * \brief Free space for byte buffer.
 * \param handle Handle of byte buffer to be freed
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_ByteBufferFree_t) (ByteBufferHandle handle);
static LGBM_ByteBufferFree_t LGBM_ByteBufferFree = NULL;

/* --- start Dataset interface */

/*!
 * \brief Load dataset from file (like LightGBM CLI version does).
 * \param filename The name of the file
 * \param parameters Additional parameters
 * \param reference Used to align bin mapper with other dataset, nullptr means isn't used
 * \param[out] out A loaded dataset
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetCreateFromFile_t) (const char* filename,
                                                 const char* parameters,
                                                 const DatasetHandle reference,
                                                 DatasetHandle* out);
static LGBM_DatasetCreateFromFile_t LGBM_DatasetCreateFromFile = NULL;

/*!
 * \brief Allocate the space for dataset and bucket feature bins according to sampled data.
 * \param sample_data Sampled data, grouped by the column
 * \param sample_indices Indices of sampled data
 * \param ncol Number of columns
 * \param num_per_col Size of each sampling column
 * \param num_sample_row Number of sampled rows
 * \param num_local_row Total number of rows local to machine
 * \param num_dist_row Number of total distributed rows
 * \param parameters Additional parameters
 * \param[out] out Created dataset
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetCreateFromSampledColumn_t) (double** sample_data,
                                                          int** sample_indices,
                                                          int32_t ncol,
                                                          const int* num_per_col,
                                                          int32_t num_sample_row,
                                                          int32_t num_local_row,
                                                          int64_t num_dist_row,
                                                          const char* parameters,
                                                          DatasetHandle* out);
static LGBM_DatasetCreateFromSampledColumn_t LGBM_DatasetCreateFromSampledColumn = NULL;

/*!
 * \brief Allocate the space for dataset and bucket feature bins according to reference dataset.
 * \param reference Used to align bin mapper with other dataset
 * \param num_total_row Number of total rows
 * \param[out] out Created dataset
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetCreateByReference_t) (const DatasetHandle reference,
                                                    int64_t num_total_row,
                                                    DatasetHandle* out);
static LGBM_DatasetCreateByReference_t LGBM_DatasetCreateByReference = NULL;

/*!
 * \brief Initialize the Dataset for streaming.
 * \param dataset Handle of dataset
 * \param has_weights Whether the dataset has Metadata weights
 * \param has_init_scores Whether the dataset has Metadata initial scores
 * \param has_queries Whether the dataset has Metadata queries/groups
 * \param nclasses Number of initial score classes
 * \param nthreads Number of external threads that will use the PushRows APIs
 * \param omp_max_threads Maximum number of OpenMP threads (-1 for default)
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetInitStreaming_t) (DatasetHandle dataset,
                                                int32_t has_weights,
                                                int32_t has_init_scores,
                                                int32_t has_queries,
                                                int32_t nclasses,
                                                int32_t nthreads,
                                                int32_t omp_max_threads);
static LGBM_DatasetInitStreaming_t LGBM_DatasetInitStreaming = NULL;

/*!
 * \brief Allocate the space for dataset and bucket feature bins according to serialized reference dataset.
 * \param ref_buffer A binary representation of the dataset schema (feature groups, bins, etc.)
 * \param ref_buffer_size The size of the reference array in bytes
 * \param num_row Number of total rows the dataset will contain
 * \param num_classes Number of classes (will be used only in case of multiclass and specifying initial scores)
 * \param parameters Additional parameters
 * \param[out] out Created dataset
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetCreateFromSerializedReference_t) (const void* ref_buffer,
                                                                int32_t ref_buffer_size,
                                                                int64_t num_row,
                                                                int32_t num_classes,
                                                                const char* parameters,
                                                                DatasetHandle* out);
static LGBM_DatasetCreateFromSerializedReference_t LGBM_DatasetCreateFromSerializedReference = NULL;

/*!
 * \brief Push data to existing dataset, if ``nrow + start_row == num_total_row``, will call ``dataset->FinishLoad``.
 * \param dataset Handle of dataset
 * \param data Pointer to the data space
 * \param data_type Type of ``data`` pointer, can be ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \param nrow Number of rows
 * \param ncol Number of columns
 * \param start_row Row start index
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetPushRows_t) (DatasetHandle dataset,
                                           const void* data,
                                           int data_type,
                                           int32_t nrow,
                                           int32_t ncol,
                                           int32_t start_row);
static LGBM_DatasetPushRows_t LGBM_DatasetPushRows = NULL;

/*!
 * \brief Push data to existing dataset.
 *        The general flow for a streaming scenario is:
 *        1. create Dataset "schema" (e.g. ``LGBM_DatasetCreateFromSampledColumn``)
 *        2. init them for thread-safe streaming (``LGBM_DatasetInitStreaming``)
 *        3. push data (``LGBM_DatasetPushRowsWithMetadata`` or ``LGBM_DatasetPushRowsByCSRWithMetadata``)
 *        4. call ``LGBM_DatasetMarkFinished``
 * \param dataset Handle of dataset
 * \param data Pointer to the data space
 * \param data_type Type of ``data`` pointer, can be ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \param nrow Number of rows
 * \param ncol Number of feature columns
 * \param start_row Row start index, i.e., the index at which to start inserting data
 * \param label Pointer to array with nrow labels
 * \param weight Optional pointer to array with nrow weights
 * \param init_score Optional pointer to array with nrow*nclasses initial scores, in column format
 * \param query Optional pointer to array with nrow query values
 * \param tid The id of the calling thread, from 0...N-1 threads
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetPushRowsWithMetadata_t) (DatasetHandle dataset,
                                                       const void* data,
                                                       int data_type,
                                                       int32_t nrow,
                                                       int32_t ncol,
                                                       int32_t start_row,
                                                       const float* label,
                                                       const float* weight,
                                                       const double* init_score,
                                                       const int32_t* query,
                                                       int32_t tid);
static LGBM_DatasetPushRowsWithMetadata_t LGBM_DatasetPushRowsWithMetadata = NULL;

/*!
 * \brief Push data to existing dataset, if ``nrow + start_row == num_total_row``, will call ``dataset->FinishLoad``.
 * \param dataset Handle of dataset
 * \param indptr Pointer to row headers
 * \param indptr_type Type of ``indptr``, can be ``C_API_DTYPE_INT32`` or ``C_API_DTYPE_INT64``
 * \param indices Pointer to column indices
 * \param data Pointer to the data space
 * \param data_type Type of ``data`` pointer, can be ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \param nindptr Number of rows in the matrix + 1
 * \param nelem Number of nonzero elements in the matrix
 * \param num_col Number of columns
 * \param start_row Row start index
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetPushRowsByCSR_t) (DatasetHandle dataset,
                                                const void* indptr,
                                                int indptr_type,
                                                const int32_t* indices,
                                                const void* data,
                                                int data_type,
                                                int64_t nindptr,
                                                int64_t nelem,
                                                int64_t num_col,
                                                int64_t start_row);
static LGBM_DatasetPushRowsByCSR_t LGBM_DatasetPushRowsByCSR = NULL;

/*!
 * \brief Push CSR data to existing dataset. (See ``LGBM_DatasetPushRowsWithMetadata`` for more details.)
 * \param dataset Handle of dataset
 * \param indptr Pointer to row headers
 * \param indptr_type Type of ``indptr``, can be ``C_API_DTYPE_INT32`` or ``C_API_DTYPE_INT64``
 * \param indices Pointer to column indices
 * \param data Pointer to the data space
 * \param data_type Type of ``data`` pointer, can be ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \param nindptr Number of rows in the matrix + 1
 * \param nelem Number of nonzero elements in the matrix
 * \param start_row Row start index
 * \param label Pointer to array with nindptr-1 labels
 * \param weight Optional pointer to array with nindptr-1 weights
 * \param init_score Optional pointer to array with (nindptr-1)*nclasses initial scores, in column format
 * \param query Optional pointer to array with nindptr-1 query values
 * \param tid The id of the calling thread, from 0...N-1 threads
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetPushRowsByCSRWithMetadata_t) (DatasetHandle dataset,
                                                            const void* indptr,
                                                            int indptr_type,
                                                            const int32_t* indices,
                                                            const void* data,
                                                            int data_type,
                                                            int64_t nindptr,
                                                            int64_t nelem,
                                                            int64_t start_row,
                                                            const float* label,
                                                            const float* weight,
                                                            const double* init_score,
                                                            const int32_t* query,
                                                            int32_t tid);
static LGBM_DatasetPushRowsByCSRWithMetadata_t LGBM_DatasetPushRowsByCSRWithMetadata = NULL;

/*!
 * \brief Set whether or not the Dataset waits for a manual MarkFinished call or calls FinishLoad on itself automatically.
 *        Set to 1 for streaming scenario, and use ``LGBM_DatasetMarkFinished`` to manually finish the Dataset.
 * \param dataset Handle of dataset
 * \param wait Whether to wait or not (1 or 0)
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetSetWaitForManualFinish_t) (DatasetHandle dataset, int wait);
static LGBM_DatasetSetWaitForManualFinish_t LGBM_DatasetSetWaitForManualFinish = NULL;

/*!
 * \brief Mark the Dataset as complete by calling ``dataset->FinishLoad``.
 * \param dataset Handle of dataset
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetMarkFinished_t) (DatasetHandle dataset);
static LGBM_DatasetMarkFinished_t LGBM_DatasetMarkFinished = NULL;

/*!
 * \brief Create a dataset from CSR format.
 * \param indptr Pointer to row headers
 * \param indptr_type Type of ``indptr``, can be ``C_API_DTYPE_INT32`` or ``C_API_DTYPE_INT64``
 * \param indices Pointer to column indices
 * \param data Pointer to the data space
 * \param data_type Type of ``data`` pointer, can be ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \param nindptr Number of rows in the matrix + 1
 * \param nelem Number of nonzero elements in the matrix
 * \param num_col Number of columns
 * \param parameters Additional parameters
 * \param reference Used to align bin mapper with other dataset, nullptr means isn't used
 * \param[out] out Created dataset
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetCreateFromCSR_t) (const void* indptr,
                                                int indptr_type,
                                                const int32_t* indices,
                                                const void* data,
                                                int data_type,
                                                int64_t nindptr,
                                                int64_t nelem,
                                                int64_t num_col,
                                                const char* parameters,
                                                const DatasetHandle reference,
                                                DatasetHandle* out);
static LGBM_DatasetCreateFromCSR_t LGBM_DatasetCreateFromCSR = NULL;

/*!
 * \brief Create a dataset from CSR format through callbacks.
 * \param get_row_funptr Pointer to ``std::function<void(int idx, std::vector<std::pair<int, double>>& ret)>``
 *                       (called for every row and expected to clear and fill ``ret``)
 * \param num_rows Number of rows
 * \param num_col Number of columns
 * \param parameters Additional parameters
 * \param reference Used to align bin mapper with other dataset, nullptr means isn't used
 * \param[out] out Created dataset
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetCreateFromCSRFunc_t) (void* get_row_funptr,
                                                    int num_rows,
                                                    int64_t num_col,
                                                    const char* parameters,
                                                    const DatasetHandle reference,
                                                    DatasetHandle* out);
static LGBM_DatasetCreateFromCSRFunc_t LGBM_DatasetCreateFromCSRFunc = NULL;

/*!
 * \brief Create a dataset from CSC format.
 * \param col_ptr Pointer to column headers
 * \param col_ptr_type Type of ``col_ptr``, can be ``C_API_DTYPE_INT32`` or ``C_API_DTYPE_INT64``
 * \param indices Pointer to row indices
 * \param data Pointer to the data space
 * \param data_type Type of ``data`` pointer, can be ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \param ncol_ptr Number of columns in the matrix + 1
 * \param nelem Number of nonzero elements in the matrix
 * \param num_row Number of rows
 * \param parameters Additional parameters
 * \param reference Used to align bin mapper with other dataset, nullptr means isn't used
 * \param[out] out Created dataset
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetCreateFromCSC_t) (const void* col_ptr,
                                                int col_ptr_type,
                                                const int32_t* indices,
                                                const void* data,
                                                int data_type,
                                                int64_t ncol_ptr,
                                                int64_t nelem,
                                                int64_t num_row,
                                                const char* parameters,
                                                const DatasetHandle reference,
                                                DatasetHandle* out);
static LGBM_DatasetCreateFromCSC_t LGBM_DatasetCreateFromCSC = NULL;

/*!
 * \brief Create dataset from dense matrix.
 * \param data Pointer to the data space
 * \param data_type Type of ``data`` pointer, can be ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \param nrow Number of rows
 * \param ncol Number of columns
 * \param is_row_major 1 for row-major, 0 for column-major
 * \param parameters Additional parameters
 * \param reference Used to align bin mapper with other dataset, nullptr means isn't used
 * \param[out] out Created dataset
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetCreateFromMat_t) (const void* data,
                                                int data_type,
                                                int32_t nrow,
                                                int32_t ncol,
                                                int is_row_major,
                                                const char* parameters,
                                                const DatasetHandle reference,
                                                DatasetHandle* out);
static LGBM_DatasetCreateFromMat_t LGBM_DatasetCreateFromMat = NULL;

/*!
 * \brief Create dataset from array of dense matrices.
 * \param nmat Number of dense matrices
 * \param data Pointer to the data space
 * \param data_type Type of ``data`` pointer, can be ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \param nrow Number of rows
 * \param ncol Number of columns
 * \param is_row_major Pointer to the data layouts. 1 for row-major, 0 for column-major
 * \param parameters Additional parameters
 * \param reference Used to align bin mapper with other dataset, nullptr means isn't used
 * \param[out] out Created dataset
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetCreateFromMats_t) (int32_t nmat,
                                                 const void** data,
                                                 int data_type,
                                                 int32_t* nrow,
                                                 int32_t ncol,
                                                 int* is_row_major,
                                                 const char* parameters,
                                                 const DatasetHandle reference,
                                                 DatasetHandle* out);
static LGBM_DatasetCreateFromMats_t LGBM_DatasetCreateFromMats = NULL;

/*!
 * \brief Create dataset from Arrow.
 * \param n_chunks The number of Arrow arrays passed to this function
 * \param chunks Pointer to the list of Arrow arrays
 * \param schema Pointer to the schema of all Arrow arrays
 * \param parameters Additional parameters
 * \param reference Used to align bin mapper with other dataset, nullptr means isn't used
 * \param[out] out Created dataset
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetCreateFromArrow_t) (int64_t n_chunks,
                                                  const ArrowArray* chunks,
                                                  const ArrowSchema* schema,
                                                  const char* parameters,
                                                  const DatasetHandle reference,
                                                  DatasetHandle *out);
static LGBM_DatasetCreateFromArrow_t LGBM_DatasetCreateFromArrow = NULL;

/*!
 * \brief Create subset of a data.
 * \param handle Handle of full dataset
 * \param used_row_indices Indices used in subset
 * \param num_used_row_indices Length of ``used_row_indices``
 * \param parameters Additional parameters
 * \param[out] out Subset of data
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetGetSubset_t) (const DatasetHandle handle,
                                            const int32_t* used_row_indices,
                                            int32_t num_used_row_indices,
                                            const char* parameters,
                                            DatasetHandle* out);
static LGBM_DatasetGetSubset_t LGBM_DatasetGetSubset = NULL;

/*!
 * \brief Save feature names to dataset.
 * \param handle Handle of dataset
 * \param feature_names Feature names
 * \param num_feature_names Number of feature names
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetSetFeatureNames_t) (DatasetHandle handle,
                                                  const char** feature_names,
                                                  int num_feature_names);
static LGBM_DatasetSetFeatureNames_t LGBM_DatasetSetFeatureNames = NULL;

/*!
 * \brief Get feature names of dataset.
 * \param handle Handle of dataset
 * \param len Number of ``char*`` pointers stored at ``out_strs``.
 *            If smaller than the max size, only this many strings are copied
 * \param[out] num_feature_names Number of feature names
 * \param buffer_len Size of pre-allocated strings.
 *                   Content is copied up to ``buffer_len - 1`` and null-terminated
 * \param[out] out_buffer_len String sizes required to do the full string copies
 * \param[out] feature_names Feature names, should pre-allocate memory
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetGetFeatureNames_t) (DatasetHandle handle,
                                                  const int len,
                                                  int* num_feature_names,
                                                  const size_t buffer_len,
                                                  size_t* out_buffer_len,
                                                  char** feature_names);
static LGBM_DatasetGetFeatureNames_t LGBM_DatasetGetFeatureNames = NULL;

/*!
 * \brief Free space for dataset.
 * \param handle Handle of dataset to be freed
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetFree_t) (DatasetHandle handle);
static LGBM_DatasetFree_t LGBM_DatasetFree = NULL;

/*!
 * \brief Save dataset to binary file.
 * \param handle Handle of dataset
 * \param filename The name of the file
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetSaveBinary_t) (DatasetHandle handle,
                                             const char* filename);
static LGBM_DatasetSaveBinary_t LGBM_DatasetSaveBinary = NULL;

/*!
 * \brief Create a dataset schema representation as a binary byte array (excluding data).
 * \param handle Handle of dataset
 * \param[out] out The output byte array
 * \param[out] out_len The length of the output byte array (returned for convenience)
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetSerializeReferenceToBinary_t) (DatasetHandle handle,
                                                             ByteBufferHandle* out,
                                                             int32_t* out_len);
static LGBM_DatasetSerializeReferenceToBinary_t LGBM_DatasetSerializeReferenceToBinary = NULL;

/*!
 * \brief Save dataset to text file, intended for debugging use only.
 * \param handle Handle of dataset
 * \param filename The name of the file
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetDumpText_t) (DatasetHandle handle,
                                           const char* filename);
static LGBM_DatasetDumpText_t LGBM_DatasetDumpText = NULL;

/*!
 * \brief Set vector to a content in info.
 * \note
 * - \a group only works for ``C_API_DTYPE_INT32``;
 * - \a label and \a weight only work for ``C_API_DTYPE_FLOAT32``;
 * - \a init_score only works for ``C_API_DTYPE_FLOAT64``.
 * \param handle Handle of dataset
 * \param field_name Field name, can be \a label, \a weight, \a init_score, \a group
 * \param field_data Pointer to data vector
 * \param num_element Number of elements in ``field_data``
 * \param type Type of ``field_data`` pointer, can be ``C_API_DTYPE_INT32``, ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetSetField_t) (DatasetHandle handle,
                                           const char* field_name,
                                           const void* field_data,
                                           int num_element,
                                           int type);
static LGBM_DatasetSetField_t LGBM_DatasetSetField = NULL;

/*!
 * \brief Set vector to a content in info.
 * \note
 * - \a group converts input datatype into ``int32``;
 * - \a label and \a weight convert input datatype into ``float32``;
 * - \a init_score converts input datatype into ``float64``.
 * \param handle Handle of dataset
 * \param field_name Field name, can be \a label, \a weight, \a init_score, \a group
 * \param n_chunks The number of Arrow arrays passed to this function
 * \param chunks Pointer to the list of Arrow arrays
 * \param schema Pointer to the schema of all Arrow arrays
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetSetFieldFromArrow_t) (DatasetHandle handle,
                                                    const char* field_name,
                                                    int64_t n_chunks,
                                                    const ArrowArray* chunks,
                                                    const ArrowSchema* schema);
static LGBM_DatasetSetFieldFromArrow_t LGBM_DatasetSetFieldFromArrow = NULL;

/*!
 * \brief Get info vector from dataset.
 * \param handle Handle of dataset
 * \param field_name Field name
 * \param[out] out_len Used to set result length
 * \param[out] out_ptr Pointer to the result
 * \param[out] out_type Type of result pointer, can be ``C_API_DTYPE_INT32``, ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetGetField_t) (DatasetHandle handle,
                                           const char* field_name,
                                           int* out_len,
                                           const void** out_ptr,
                                           int* out_type);
static LGBM_DatasetGetField_t LGBM_DatasetGetField = NULL;

/*!
 * \brief Raise errors for attempts to update dataset parameters.
 * \param old_parameters Current dataset parameters
 * \param new_parameters New dataset parameters
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetUpdateParamChecking_t) (const char* old_parameters,
                                                      const char* new_parameters);
static LGBM_DatasetUpdateParamChecking_t LGBM_DatasetUpdateParamChecking = NULL;

/*!
 * \brief Get number of data points.
 * \param handle Handle of dataset
 * \param[out] out The address to hold number of data points
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetGetNumData_t) (DatasetHandle handle,
                                             int* out);
static LGBM_DatasetGetNumData_t LGBM_DatasetGetNumData = NULL;

/*!
 * \brief Get number of features.
 * \param handle Handle of dataset
 * \param[out] out The address to hold number of features
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetGetNumFeature_t) (DatasetHandle handle,
                                                int* out);
static LGBM_DatasetGetNumFeature_t LGBM_DatasetGetNumFeature = NULL;

/*!
 * \brief Get number of bins for feature.
 * \param handle Handle of dataset
 * \param feature Index of the feature
 * \param[out] out The address to hold number of bins
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetGetFeatureNumBin_t) (DatasetHandle handle,
                                                   int feature,
                                                   int* out);
static LGBM_DatasetGetFeatureNumBin_t LGBM_DatasetGetFeatureNumBin = NULL;

/*!
 * \brief Add features from ``source`` to ``target``.
 * \param target The handle of the dataset to add features to
 * \param source The handle of the dataset to take features from
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_DatasetAddFeaturesFrom_t) (DatasetHandle target,
                                                  DatasetHandle source);
static LGBM_DatasetAddFeaturesFrom_t LGBM_DatasetAddFeaturesFrom = NULL;

/* --- start Booster interfaces */

/*!
* \brief Get int representing whether booster is fitting linear trees.
* \param handle Handle of booster
* \param[out] out The address to hold linear trees indicator
* \return 0 when succeed, -1 when failure happens
*/
LIGHTGBM_C_EXPORT int (*LGBM_BoosterGetLinear_t) (BoosterHandle handle, int* out);
static LGBM_BoosterGetLinear_t LGBM_BoosterGetLinear = NULL;

/*!
 * \brief Create a new boosting learner.
 * \param train_data Training dataset
 * \param parameters Parameters in format 'key1=value1 key2=value2'
 * \param[out] out Handle of created booster
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterCreate_t) (const DatasetHandle train_data,
                                         const char* parameters,
                                         BoosterHandle* out);
static LGBM_BoosterCreate_t LGBM_BoosterCreate = NULL;

/*!
 * \brief Load an existing booster from model file.
 * \param filename Filename of model
 * \param[out] out_num_iterations Number of iterations of this booster
 * \param[out] out Handle of created booster
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterCreateFromModelfile_t) (const char* filename,
                                                      int* out_num_iterations,
                                                      BoosterHandle* out);
static LGBM_BoosterCreateFromModelfile_t LGBM_BoosterCreateFromModelfile = NULL;

/*!
 * \brief Load an existing booster from string.
 * \param model_str Model string
 * \param[out] out_num_iterations Number of iterations of this booster
 * \param[out] out Handle of created booster
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterLoadModelFromString_t) (const char* model_str,
                                                      int* out_num_iterations,
                                                      BoosterHandle* out);
static LGBM_BoosterLoadModelFromString_t LGBM_BoosterLoadModelFromString = NULL;

/*!
 * \brief Get parameters as JSON string.
 * \param handle Handle of booster
 * \param buffer_len Allocated space for string
 * \param[out] out_len Actual size of string
 * \param[out] out_str JSON string containing parameters
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterGetLoadedParam_t) (BoosterHandle handle,
                                                 int64_t buffer_len,
                                                 int64_t* out_len,
                                                 char* out_str);
static LGBM_BoosterGetLoadedParam_t LGBM_BoosterGetLoadedParam = NULL;


/*!
 * \brief Free space for booster.
 * \param handle Handle of booster to be freed
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterFree_t) (BoosterHandle handle);
static LGBM_BoosterFree_t LGBM_BoosterFree = NULL;

/*!
 * \brief Shuffle models.
 * \param handle Handle of booster
 * \param start_iter The first iteration that will be shuffled
 * \param end_iter The last iteration that will be shuffled
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterShuffleModels_t) (BoosterHandle handle,
                                                int start_iter,
                                                int end_iter);
static LGBM_BoosterShuffleModels_t LGBM_BoosterShuffleModels = NULL;

/*!
 * \brief Merge model from ``other_handle`` into ``handle``.
 * \param handle Handle of booster, will merge another booster into this one
 * \param other_handle Other handle of booster
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterMerge_t) (BoosterHandle handle,
                                        BoosterHandle other_handle);
static LGBM_BoosterMerge_t LGBM_BoosterMerge = NULL;

/*!
 * \brief Add new validation data to booster.
 * \param handle Handle of booster
 * \param valid_data Validation dataset
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterAddValidData_t) (BoosterHandle handle,
                                               const DatasetHandle valid_data);
static LGBM_BoosterAddValidData_t LGBM_BoosterAddValidData = NULL;

/*!
 * \brief Reset training data for booster.
 * \param handle Handle of booster
 * \param train_data Training dataset
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterResetTrainingData_t) (BoosterHandle handle,
                                                    const DatasetHandle train_data);
static LGBM_BoosterResetTrainingData_t LGBM_BoosterResetTrainingData = NULL;

/*!
 * \brief Reset config for booster.
 * \param handle Handle of booster
 * \param parameters Parameters in format 'key1=value1 key2=value2'
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterResetParameter_t) (BoosterHandle handle,
                                                 const char* parameters);
static LGBM_BoosterResetParameter_t LGBM_BoosterResetParameter = NULL;

/*!
 * \brief Get number of classes.
 * \param handle Handle of booster
 * \param[out] out_len Number of classes
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterGetNumClasses_t) (BoosterHandle handle,
                                                int* out_len);
static LGBM_BoosterGetNumClasses_t LGBM_BoosterGetNumClasses = NULL;

/*!
 * \brief Update the model for one iteration.
 * \param handle Handle of booster
 * \param[out] is_finished 1 means the update was successfully finished (cannot split any more), 0 indicates failure
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterUpdateOneIter_t) (BoosterHandle handle,
                                                int* is_finished);
static LGBM_BoosterUpdateOneIter_t LGBM_BoosterUpdateOneIter = NULL;

/*!
 * \brief Refit the tree model using the new data (online learning).
 * \param handle Handle of booster
 * \param leaf_preds Pointer to predicted leaf indices
 * \param nrow Number of rows of ``leaf_preds``
 * \param ncol Number of columns of ``leaf_preds``
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterRefit_t) (BoosterHandle handle,
                                        const int32_t* leaf_preds,
                                        int32_t nrow,
                                        int32_t ncol);
static LGBM_BoosterRefit_t LGBM_BoosterRefit = NULL;

/*!
 * \brief Update the model by specifying gradient and Hessian directly
 *        (this can be used to support customized loss functions).
 * \note
 * The length of the arrays referenced by ``grad`` and ``hess`` must be equal to
 * ``num_class * num_train_data``, this is not verified by the library, the caller must ensure this.
 * \param handle Handle of booster
 * \param grad The first order derivative (gradient) statistics
 * \param hess The second order derivative (Hessian) statistics
 * \param[out] is_finished 1 means the update was successfully finished (cannot split any more), 0 indicates failure
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterUpdateOneIterCustom_t) (BoosterHandle handle,
                                                      const float* grad,
                                                      const float* hess,
                                                      int* is_finished);
static LGBM_BoosterUpdateOneIterCustom_t LGBM_BoosterUpdateOneIterCustom = NULL;

/*!
 * \brief Rollback one iteration.
 * \param handle Handle of booster
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterRollbackOneIter_t) (BoosterHandle handle);
static LGBM_BoosterRollbackOneIter_t LGBM_BoosterRollbackOneIter = NULL;

/*!
 * \brief Get index of the current boosting iteration.
 * \param handle Handle of booster
 * \param[out] out_iteration Index of the current boosting iteration
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterGetCurrentIteration_t) (BoosterHandle handle,
                                                      int* out_iteration);
static LGBM_BoosterGetCurrentIteration_t LGBM_BoosterGetCurrentIteration = NULL;

/*!
 * \brief Get number of trees per iteration.
 * \param handle Handle of booster
 * \param[out] out_tree_per_iteration Number of trees per iteration
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterNumModelPerIteration_t) (BoosterHandle handle,
                                                       int* out_tree_per_iteration);
static LGBM_BoosterNumModelPerIteration_t LGBM_BoosterNumModelPerIteration = NULL;

/*!
 * \brief Get number of weak sub-models.
 * \param handle Handle of booster
 * \param[out] out_models Number of weak sub-models
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterNumberOfTotalModel_t) (BoosterHandle handle,
                                                     int* out_models);
static LGBM_BoosterNumberOfTotalModel_t LGBM_BoosterNumberOfTotalModel = NULL;

/*!
 * \brief Get number of evaluation metrics.
 * \param handle Handle of booster
 * \param[out] out_len Total number of evaluation metrics
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterGetEvalCounts_t) (BoosterHandle handle,
                                                int* out_len);
static LGBM_BoosterGetEvalCounts_t LGBM_BoosterGetEvalCounts = NULL;

/*!
 * \brief Get names of evaluation metrics.
 * \param handle Handle of booster
 * \param len Number of ``char*`` pointers stored at ``out_strs``.
 *            If smaller than the max size, only this many strings are copied
 * \param[out] out_len Total number of evaluation metrics
 * \param buffer_len Size of pre-allocated strings.
 *                   Content is copied up to ``buffer_len - 1`` and null-terminated
 * \param[out] out_buffer_len String sizes required to do the full string copies
 * \param[out] out_strs Names of evaluation metrics, should pre-allocate memory
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterGetEvalNames_t) (BoosterHandle handle,
                                               const int len,
                                               int* out_len,
                                               const size_t buffer_len,
                                               size_t* out_buffer_len,
                                               char** out_strs);
static LGBM_BoosterGetEvalNames_t LGBM_BoosterGetEvalNames = NULL;

/*!
 * \brief Get names of features.
 * \param handle Handle of booster
 * \param len Number of ``char*`` pointers stored at ``out_strs``.
 *            If smaller than the max size, only this many strings are copied
 * \param[out] out_len Total number of features
 * \param buffer_len Size of pre-allocated strings.
 *                   Content is copied up to ``buffer_len - 1`` and null-terminated
 * \param[out] out_buffer_len String sizes required to do the full string copies
 * \param[out] out_strs Names of features, should pre-allocate memory
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterGetFeatureNames_t) (BoosterHandle handle,
                                                  const int len,
                                                  int* out_len,
                                                  const size_t buffer_len,
                                                  size_t* out_buffer_len,
                                                  char** out_strs);
static LGBM_BoosterGetFeatureNames_t LGBM_BoosterGetFeatureNames = NULL;

/*!
 * \brief Check that the feature names of the data match the ones used to train the booster.
 * \param handle Handle of booster
 * \param data_names Array with the feature names in the data
 * \param data_num_features Number of features in the data
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterValidateFeatureNames_t) (BoosterHandle handle,
                                                       const char** data_names,
                                                       int data_num_features);
static LGBM_BoosterValidateFeatureNames_t LGBM_BoosterValidateFeatureNames = NULL;

/*!
 * \brief Get number of features.
 * \param handle Handle of booster
 * \param[out] out_len Total number of features
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterGetNumFeature_t) (BoosterHandle handle,
                                                int* out_len);
static LGBM_BoosterGetNumFeature_t LGBM_BoosterGetNumFeature = NULL;

/*!
 * \brief Get evaluation for training data and validation data.
 * \note
 *   1. You should call ``LGBM_BoosterGetEvalNames`` first to get the names of evaluation metrics.
 *   2. You should pre-allocate memory for ``out_results``, you can get its length by ``LGBM_BoosterGetEvalCounts``.
 * \param handle Handle of booster
 * \param data_idx Index of data, 0: training data, 1: 1st validation data, 2: 2nd validation data and so on
 * \param[out] out_len Length of output result
 * \param[out] out_results Array with evaluation results
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterGetEval_t) (BoosterHandle handle,
                                          int data_idx,
                                          int* out_len,
                                          double* out_results);
static LGBM_BoosterGetEval_t LGBM_BoosterGetEval = NULL;

/*!
 * \brief Get number of predictions for training data and validation data
 *        (this can be used to support customized evaluation functions).
 * \param handle Handle of booster
 * \param data_idx Index of data, 0: training data, 1: 1st validation data, 2: 2nd validation data and so on
 * \param[out] out_len Number of predictions
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterGetNumPredict_t) (BoosterHandle handle,
                                                int data_idx,
                                                int64_t* out_len);
static LGBM_BoosterGetNumPredict_t LGBM_BoosterGetNumPredict = NULL;

/*!
 * \brief Get prediction for training data and validation data.
 * \note
 * You should pre-allocate memory for ``out_result``, its length is equal to ``num_class * num_data``.
 * \param handle Handle of booster
 * \param data_idx Index of data, 0: training data, 1: 1st validation data, 2: 2nd validation data and so on
 * \param[out] out_len Length of output result
 * \param[out] out_result Pointer to array with predictions
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterGetPredict_t) (BoosterHandle handle,
                                             int data_idx,
                                             int64_t* out_len,
                                             double* out_result);
static LGBM_BoosterGetPredict_t LGBM_BoosterGetPredict = NULL;

/*!
 * \brief Make prediction for file.
 * \param handle Handle of booster
 * \param data_filename Filename of file with data
 * \param data_has_header Whether file has header or not
 * \param predict_type What should be predicted
 *   - ``C_API_PREDICT_NORMAL``: normal prediction, with transform (if needed);
 *   - ``C_API_PREDICT_RAW_SCORE``: raw score;
 *   - ``C_API_PREDICT_LEAF_INDEX``: leaf index;
 *   - ``C_API_PREDICT_CONTRIB``: feature contributions (SHAP values)
 * \param start_iteration Start index of the iteration to predict
 * \param num_iteration Number of iterations for prediction, <= 0 means no limit
 * \param parameter Other parameters for prediction, e.g. early stopping for prediction
 * \param result_filename Filename of result file in which predictions will be written
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterPredictForFile_t) (BoosterHandle handle,
                                                 const char* data_filename,
                                                 int data_has_header,
                                                 int predict_type,
                                                 int start_iteration,
                                                 int num_iteration,
                                                 const char* parameter,
                                                 const char* result_filename);
static LGBM_BoosterPredictForFile_t LGBM_BoosterPredictForFile = NULL;

/*!
 * \brief Get number of predictions.
 * \param handle Handle of booster
 * \param num_row Number of rows
 * \param predict_type What should be predicted
 *   - ``C_API_PREDICT_NORMAL``: normal prediction, with transform (if needed);
 *   - ``C_API_PREDICT_RAW_SCORE``: raw score;
 *   - ``C_API_PREDICT_LEAF_INDEX``: leaf index;
 *   - ``C_API_PREDICT_CONTRIB``: feature contributions (SHAP values)
 * \param start_iteration Start index of the iteration to predict
 * \param num_iteration Number of iterations for prediction, <= 0 means no limit
 * \param[out] out_len Length of prediction
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterCalcNumPredict_t) (BoosterHandle handle,
                                                 int num_row,
                                                 int predict_type,
                                                 int start_iteration,
                                                 int num_iteration,
                                                 int64_t* out_len);
static LGBM_BoosterCalcNumPredict_t LGBM_BoosterCalcNumPredict = NULL;

/*!
 * \brief Release FastConfig object.
 *
 * \param fastConfig Handle to the FastConfig object acquired with a ``*FastInit()`` method.
 * \return 0 when it succeeds, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_FastConfigFree_t) (FastConfigHandle fastConfig);
static LGBM_FastConfigFree_t LGBM_FastConfigFree = NULL;

/*!
 * \brief Make prediction for a new dataset in CSR format.
 * \note
 * You should pre-allocate memory for ``out_result``:
 *   - for normal and raw score, its length is equal to ``num_class * num_data``;
 *   - for leaf index, its length is equal to ``num_class * num_data * num_iteration``;
 *   - for feature contributions, its length is equal to ``num_class * num_data * (num_feature + 1)``.
 * \param handle Handle of booster
 * \param indptr Pointer to row headers
 * \param indptr_type Type of ``indptr``, can be ``C_API_DTYPE_INT32`` or ``C_API_DTYPE_INT64``
 * \param indices Pointer to column indices
 * \param data Pointer to the data space
 * \param data_type Type of ``data`` pointer, can be ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \param nindptr Number of rows in the matrix + 1
 * \param nelem Number of nonzero elements in the matrix
 * \param num_col Number of columns
 * \param predict_type What should be predicted
 *   - ``C_API_PREDICT_NORMAL``: normal prediction, with transform (if needed);
 *   - ``C_API_PREDICT_RAW_SCORE``: raw score;
 *   - ``C_API_PREDICT_LEAF_INDEX``: leaf index;
 *   - ``C_API_PREDICT_CONTRIB``: feature contributions (SHAP values)
 * \param start_iteration Start index of the iteration to predict
 * \param num_iteration Number of iterations for prediction, <= 0 means no limit
 * \param parameter Other parameters for prediction, e.g. early stopping for prediction
 * \param[out] out_len Length of output result
 * \param[out] out_result Pointer to array with predictions
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterPredictForCSR_t) (BoosterHandle handle,
                                                const void* indptr,
                                                int indptr_type,
                                                const int32_t* indices,
                                                const void* data,
                                                int data_type,
                                                int64_t nindptr,
                                                int64_t nelem,
                                                int64_t num_col,
                                                int predict_type,
                                                int start_iteration,
                                                int num_iteration,
                                                const char* parameter,
                                                int64_t* out_len,
                                                double* out_result);
static LGBM_BoosterPredictForCSR_t LGBM_BoosterPredictForCSR = NULL;

/*!
 * \brief Make sparse prediction for a new dataset in CSR or CSC format. Currently only used for feature contributions.
 * \note
 * The outputs are pre-allocated, as they can vary for each invocation, but the shape should be the same:
 *   - for feature contributions, the shape of sparse matrix will be ``num_class * num_data * (num_feature + 1)``.
 * The output indptr_type for the sparse matrix will be the same as the given input indptr_type.
 * Call ``LGBM_BoosterFreePredictSparse`` to deallocate resources.
 * \param handle Handle of booster
 * \param indptr Pointer to row headers for CSR or column headers for CSC
 * \param indptr_type Type of ``indptr``, can be ``C_API_DTYPE_INT32`` or ``C_API_DTYPE_INT64``
 * \param indices Pointer to column indices for CSR or row indices for CSC
 * \param data Pointer to the data space
 * \param data_type Type of ``data`` pointer, can be ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \param nindptr Number of entries in ``indptr``
 * \param nelem Number of nonzero elements in the matrix
 * \param num_col_or_row Number of columns for CSR or number of rows for CSC
 * \param predict_type What should be predicted, only feature contributions supported currently
 *   - ``C_API_PREDICT_CONTRIB``: feature contributions (SHAP values)
 * \param start_iteration Start index of the iteration to predict
 * \param num_iteration Number of iterations for prediction, <= 0 means no limit
 * \param parameter Other parameters for prediction, e.g. early stopping for prediction
 * \param matrix_type Type of matrix input and output, can be ``C_API_MATRIX_TYPE_CSR`` or ``C_API_MATRIX_TYPE_CSC``
 * \param[out] out_len Length of output data and output indptr (pointer to an array with two entries where to write them)
 * \param[out] out_indptr Pointer to output row headers for CSR or column headers for CSC
 * \param[out] out_indices Pointer to sparse column indices for CSR or row indices for CSC
 * \param[out] out_data Pointer to sparse data space
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterPredictSparseOutput_t) (BoosterHandle handle,
                                                      const void* indptr,
                                                      int indptr_type,
                                                      const int32_t* indices,
                                                      const void* data,
                                                      int data_type,
                                                      int64_t nindptr,
                                                      int64_t nelem,
                                                      int64_t num_col_or_row,
                                                      int predict_type,
                                                      int start_iteration,
                                                      int num_iteration,
                                                      const char* parameter,
                                                      int matrix_type,
                                                      int64_t* out_len,
                                                      void** out_indptr,
                                                      int32_t** out_indices,
                                                      void** out_data);
static LGBM_BoosterPredictSparseOutput_t LGBM_BoosterPredictSparseOutput = NULL;

/*!
 * \brief Method corresponding to ``LGBM_BoosterPredictSparseOutput`` to free the allocated data.
 * \param indptr Pointer to output row headers or column headers to be deallocated
 * \param indices Pointer to sparse indices to be deallocated
 * \param data Pointer to sparse data space to be deallocated
 * \param indptr_type Type of ``indptr``, can be ``C_API_DTYPE_INT32`` or ``C_API_DTYPE_INT64``
 * \param data_type Type of ``data`` pointer, can be ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterFreePredictSparse_t) (void* indptr, int32_t* indices, void* data, int indptr_type, int data_type);
static LGBM_BoosterFreePredictSparse_t LGBM_BoosterFreePredictSparse = NULL;

/*!
 * \brief Make prediction for a new dataset in CSR format. This method re-uses the internal predictor structure
 *        from previous calls and is optimized for single row invocation.
 * \note
 * You should pre-allocate memory for ``out_result``:
 *   - for normal and raw score, its length is equal to ``num_class * num_data``;
 *   - for leaf index, its length is equal to ``num_class * num_data * num_iteration``;
 *   - for feature contributions, its length is equal to ``num_class * num_data * (num_feature + 1)``.
 * \param handle Handle of booster
 * \param indptr Pointer to row headers
 * \param indptr_type Type of ``indptr``, can be ``C_API_DTYPE_INT32`` or ``C_API_DTYPE_INT64``
 * \param indices Pointer to column indices
 * \param data Pointer to the data space
 * \param data_type Type of ``data`` pointer, can be ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \param nindptr Number of rows in the matrix + 1
 * \param nelem Number of nonzero elements in the matrix
 * \param num_col Number of columns
 * \param predict_type What should be predicted
 *   - ``C_API_PREDICT_NORMAL``: normal prediction, with transform (if needed);
 *   - ``C_API_PREDICT_RAW_SCORE``: raw score;
 *   - ``C_API_PREDICT_LEAF_INDEX``: leaf index;
 *   - ``C_API_PREDICT_CONTRIB``: feature contributions (SHAP values)
 * \param start_iteration Start index of the iteration to predict
 * \param num_iteration Number of iterations for prediction, <= 0 means no limit
 * \param parameter Other parameters for prediction, e.g. early stopping for prediction
 * \param[out] out_len Length of output result
 * \param[out] out_result Pointer to array with predictions
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterPredictForCSRSingleRow_t) (BoosterHandle handle,
                                                         const void* indptr,
                                                         int indptr_type,
                                                         const int32_t* indices,
                                                         const void* data,
                                                         int data_type,
                                                         int64_t nindptr,
                                                         int64_t nelem,
                                                         int64_t num_col,
                                                         int predict_type,
                                                         int start_iteration,
                                                         int num_iteration,
                                                         const char* parameter,
                                                         int64_t* out_len,
                                                         double* out_result);
static LGBM_BoosterPredictForCSRSingleRow_t LGBM_BoosterPredictForCSRSingleRow = NULL;

/*!
 * \brief Initialize and return a ``FastConfigHandle`` for use with ``LGBM_BoosterPredictForCSRSingleRowFast``.
 *
 * Release the ``FastConfig`` by passing its handle to ``LGBM_FastConfigFree`` when no longer needed.
 *
 * \param handle Booster handle
 * \param predict_type What should be predicted
 *   - ``C_API_PREDICT_NORMAL``: normal prediction, with transform (if needed);
 *   - ``C_API_PREDICT_RAW_SCORE``: raw score;
 *   - ``C_API_PREDICT_LEAF_INDEX``: leaf index;
 *   - ``C_API_PREDICT_CONTRIB``: feature contributions (SHAP values)
 * \param start_iteration Start index of the iteration to predict
 * \param num_iteration Number of iterations for prediction, <= 0 means no limit
 * \param data_type Type of ``data`` pointer, can be ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \param num_col Number of columns
 * \param parameter Other parameters for prediction, e.g. early stopping for prediction
 * \param[out] out_fastConfig FastConfig object with which you can call ``LGBM_BoosterPredictForCSRSingleRowFast``
 * \return 0 when it succeeds, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterPredictForCSRSingleRowFastInit_t) (BoosterHandle handle,
                                                                 const int predict_type,
                                                                 const int start_iteration,
                                                                 const int num_iteration,
                                                                 const int data_type,
                                                                 const int64_t num_col,
                                                                 const char* parameter,
                                                                 FastConfigHandle *out_fastConfig);
static LGBM_BoosterPredictForCSRSingleRowFastInit_t LGBM_BoosterPredictForCSRSingleRowFastInit = NULL;

/*!
 * \brief Faster variant of ``LGBM_BoosterPredictForCSRSingleRow``.
 *
 * Score single rows after setup with ``LGBM_BoosterPredictForCSRSingleRowFastInit``.
 *
 * By removing the setup steps from this call extra optimizations can be made like
 * initializing the config only once, instead of once per call.
 *
 * \note
 *   Setting up the number of threads is only done once at ``LGBM_BoosterPredictForCSRSingleRowFastInit``
 *   instead of at each prediction.
 *   If you use a different number of threads in other calls, you need to start the setup process over,
 *   or that number of threads will be used for these calls as well.
 *
 * \note
 * You should pre-allocate memory for ``out_result``:
 *   - for normal and raw score, its length is equal to ``num_class * num_data``;
 *   - for leaf index, its length is equal to ``num_class * num_data * num_iteration``;
 *   - for feature contributions, its length is equal to ``num_class * num_data * (num_feature + 1)``.
 *
 * \param fastConfig_handle FastConfig object handle returned by ``LGBM_BoosterPredictForCSRSingleRowFastInit``
 * \param indptr Pointer to row headers
 * \param indptr_type Type of ``indptr``, can be ``C_API_DTYPE_INT32`` or ``C_API_DTYPE_INT64``
 * \param indices Pointer to column indices
 * \param data Pointer to the data space
 * \param nindptr Number of rows in the matrix + 1
 * \param nelem Number of nonzero elements in the matrix
 * \param[out] out_len Length of output result
 * \param[out] out_result Pointer to array with predictions
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterPredictForCSRSingleRowFast_t) (FastConfigHandle fastConfig_handle,
                                                             const void* indptr,
                                                             const int indptr_type,
                                                             const int32_t* indices,
                                                             const void* data,
                                                             const int64_t nindptr,
                                                             const int64_t nelem,
                                                             int64_t* out_len,
                                                             double* out_result);
static LGBM_BoosterPredictForCSRSingleRowFast_t LGBM_BoosterPredictForCSRSingleRowFast = NULL;

/*!
 * \brief Make prediction for a new dataset in CSC format.
 * \note
 * You should pre-allocate memory for ``out_result``:
 *   - for normal and raw score, its length is equal to ``num_class * num_data``;
 *   - for leaf index, its length is equal to ``num_class * num_data * num_iteration``;
 *   - for feature contributions, its length is equal to ``num_class * num_data * (num_feature + 1)``.
 * \param handle Handle of booster
 * \param col_ptr Pointer to column headers
 * \param col_ptr_type Type of ``col_ptr``, can be ``C_API_DTYPE_INT32`` or ``C_API_DTYPE_INT64``
 * \param indices Pointer to row indices
 * \param data Pointer to the data space
 * \param data_type Type of ``data`` pointer, can be ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \param ncol_ptr Number of columns in the matrix + 1
 * \param nelem Number of nonzero elements in the matrix
 * \param num_row Number of rows
 * \param predict_type What should be predicted
 *   - ``C_API_PREDICT_NORMAL``: normal prediction, with transform (if needed);
 *   - ``C_API_PREDICT_RAW_SCORE``: raw score;
 *   - ``C_API_PREDICT_LEAF_INDEX``: leaf index;
 *   - ``C_API_PREDICT_CONTRIB``: feature contributions (SHAP values)
 * \param start_iteration Start index of the iteration to predict
 * \param num_iteration Number of iteration for prediction, <= 0 means no limit
 * \param parameter Other parameters for prediction, e.g. early stopping for prediction
 * \param[out] out_len Length of output result
 * \param[out] out_result Pointer to array with predictions
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterPredictForCSC_t) (BoosterHandle handle,
                                                const void* col_ptr,
                                                int col_ptr_type,
                                                const int32_t* indices,
                                                const void* data,
                                                int data_type,
                                                int64_t ncol_ptr,
                                                int64_t nelem,
                                                int64_t num_row,
                                                int predict_type,
                                                int start_iteration,
                                                int num_iteration,
                                                const char* parameter,
                                                int64_t* out_len,
                                                double* out_result);
static LGBM_BoosterPredictForCSC_t LGBM_BoosterPredictForCSC = NULL;

/*!
 * \brief Make prediction for a new dataset.
 * \note
 * You should pre-allocate memory for ``out_result``:
 *   - for normal and raw score, its length is equal to ``num_class * num_data``;
 *   - for leaf index, its length is equal to ``num_class * num_data * num_iteration``;
 *   - for feature contributions, its length is equal to ``num_class * num_data * (num_feature + 1)``.
 * \param handle Handle of booster
 * \param data Pointer to the data space
 * \param data_type Type of ``data`` pointer, can be ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \param nrow Number of rows
 * \param ncol Number of columns
 * \param is_row_major 1 for row-major, 0 for column-major
 * \param predict_type What should be predicted
 *   - ``C_API_PREDICT_NORMAL``: normal prediction, with transform (if needed);
 *   - ``C_API_PREDICT_RAW_SCORE``: raw score;
 *   - ``C_API_PREDICT_LEAF_INDEX``: leaf index;
 *   - ``C_API_PREDICT_CONTRIB``: feature contributions (SHAP values)
 * \param start_iteration Start index of the iteration to predict
 * \param num_iteration Number of iteration for prediction, <= 0 means no limit
 * \param parameter Other parameters for prediction, e.g. early stopping for prediction
 * \param[out] out_len Length of output result
 * \param[out] out_result Pointer to array with predictions
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterPredictForMat_t) (BoosterHandle handle,
                                                const void* data,
                                                int data_type,
                                                int32_t nrow,
                                                int32_t ncol,
                                                int is_row_major,
                                                int predict_type,
                                                int start_iteration,
                                                int num_iteration,
                                                const char* parameter,
                                                int64_t* out_len,
                                                double* out_result);
static LGBM_BoosterPredictForMat_t LGBM_BoosterPredictForMat = NULL;

/*!
 * \brief Make prediction for a new dataset. This method re-uses the internal predictor structure
 *        from previous calls and is optimized for single row invocation.
 * \note
 * You should pre-allocate memory for ``out_result``:
 *   - for normal and raw score, its length is equal to ``num_class * num_data``;
 *   - for leaf index, its length is equal to ``num_class * num_data * num_iteration``;
 *   - for feature contributions, its length is equal to ``num_class * num_data * (num_feature + 1)``.
 * \param handle Handle of booster
 * \param data Pointer to the data space
 * \param data_type Type of ``data`` pointer, can be ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \param ncol Number columns
 * \param is_row_major 1 for row-major, 0 for column-major
 * \param predict_type What should be predicted
 *   - ``C_API_PREDICT_NORMAL``: normal prediction, with transform (if needed);
 *   - ``C_API_PREDICT_RAW_SCORE``: raw score;
 *   - ``C_API_PREDICT_LEAF_INDEX``: leaf index;
 *   - ``C_API_PREDICT_CONTRIB``: feature contributions (SHAP values)
 * \param start_iteration Start index of the iteration to predict
 * \param num_iteration Number of iteration for prediction, <= 0 means no limit
 * \param parameter Other parameters for prediction, e.g. early stopping for prediction
 * \param[out] out_len Length of output result
 * \param[out] out_result Pointer to array with predictions
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterPredictForMatSingleRow_t) (BoosterHandle handle,
                                                         const void* data,
                                                         int data_type,
                                                         int ncol,
                                                         int is_row_major,
                                                         int predict_type,
                                                         int start_iteration,
                                                         int num_iteration,
                                                         const char* parameter,
                                                         int64_t* out_len,
                                                         double* out_result);
static LGBM_BoosterPredictForMatSingleRow_t LGBM_BoosterPredictForMatSingleRow = NULL;

/*!
 * \brief Initialize and return a ``FastConfigHandle`` for use with ``LGBM_BoosterPredictForMatSingleRowFast``.
 *
 * Release the ``FastConfig`` by passing its handle to ``LGBM_FastConfigFree`` when no longer needed.
 *
 * \param handle Booster handle
 * \param predict_type What should be predicted
 *   - ``C_API_PREDICT_NORMAL``: normal prediction, with transform (if needed);
 *   - ``C_API_PREDICT_RAW_SCORE``: raw score;
 *   - ``C_API_PREDICT_LEAF_INDEX``: leaf index;
 *   - ``C_API_PREDICT_CONTRIB``: feature contributions (SHAP values)
 * \param start_iteration Start index of the iteration to predict
 * \param num_iteration Number of iterations for prediction, <= 0 means no limit
 * \param data_type Type of ``data`` pointer, can be ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \param ncol Number of columns
 * \param parameter Other parameters for prediction, e.g. early stopping for prediction
 * \param[out] out_fastConfig FastConfig object with which you can call ``LGBM_BoosterPredictForMatSingleRowFast``
 * \return 0 when it succeeds, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterPredictForMatSingleRowFastInit_t) (BoosterHandle handle,
                                                                 const int predict_type,
                                                                 const int start_iteration,
                                                                 const int num_iteration,
                                                                 const int data_type,
                                                                 const int32_t ncol,
                                                                 const char* parameter,
                                                                 FastConfigHandle *out_fastConfig);
static LGBM_BoosterPredictForMatSingleRowFastInit_t LGBM_BoosterPredictForMatSingleRowFastInit = NULL;

/*!
 * \brief Faster variant of ``LGBM_BoosterPredictForMatSingleRow``.
 *
 * Score a single row after setup with ``LGBM_BoosterPredictForMatSingleRowFastInit``.
 *
 * By removing the setup steps from this call extra optimizations can be made like
 * initializing the config only once, instead of once per call.
 *
 * \note
 *   Setting up the number of threads is only done once at ``LGBM_BoosterPredictForMatSingleRowFastInit``
 *   instead of at each prediction.
 *   If you use a different number of threads in other calls, you need to start the setup process over,
 *   or that number of threads will be used for these calls as well.
 *
 * \param fastConfig_handle FastConfig object handle returned by ``LGBM_BoosterPredictForMatSingleRowFastInit``
 * \param data Single-row array data (no other way than row-major form).
 * \param[out] out_len Length of output result
 * \param[out] out_result Pointer to array with predictions
 * \return 0 when it succeeds, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterPredictForMatSingleRowFast_t) (FastConfigHandle fastConfig_handle,
                                                             const void* data,
                                                             int64_t* out_len,
                                                             double* out_result);
static LGBM_BoosterPredictForMatSingleRowFast_t LGBM_BoosterPredictForMatSingleRowFast = NULL;

/*!
 * \brief Make prediction for a new dataset presented in a form of array of pointers to rows.
 * \note
 * You should pre-allocate memory for ``out_result``:
 *   - for normal and raw score, its length is equal to ``num_class * num_data``;
 *   - for leaf index, its length is equal to ``num_class * num_data * num_iteration``;
 *   - for feature contributions, its length is equal to ``num_class * num_data * (num_feature + 1)``.
 * \param handle Handle of booster
 * \param data Pointer to the data space
 * \param data_type Type of ``data`` pointer, can be ``C_API_DTYPE_FLOAT32`` or ``C_API_DTYPE_FLOAT64``
 * \param nrow Number of rows
 * \param ncol Number columns
 * \param predict_type What should be predicted
 *   - ``C_API_PREDICT_NORMAL``: normal prediction, with transform (if needed);
 *   - ``C_API_PREDICT_RAW_SCORE``: raw score;
 *   - ``C_API_PREDICT_LEAF_INDEX``: leaf index;
 *   - ``C_API_PREDICT_CONTRIB``: feature contributions (SHAP values)
 * \param start_iteration Start index of the iteration to predict
 * \param num_iteration Number of iteration for prediction, <= 0 means no limit
 * \param parameter Other parameters for prediction, e.g. early stopping for prediction
 * \param[out] out_len Length of output result
 * \param[out] out_result Pointer to array with predictions
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterPredictForMats_t) (BoosterHandle handle,
                                                 const void** data,
                                                 int data_type,
                                                 int32_t nrow,
                                                 int32_t ncol,
                                                 int predict_type,
                                                 int start_iteration,
                                                 int num_iteration,
                                                 const char* parameter,
                                                 int64_t* out_len,
                                                 double* out_result);
static LGBM_BoosterPredictForMats_t LGBM_BoosterPredictForMats = NULL;

/*!
 * \brief Make prediction for a new dataset.
 * \note
 * You should pre-allocate memory for ``out_result``:
 *   - for normal and raw score, its length is equal to ``num_class * num_data``;
 *   - for leaf index, its length is equal to ``num_class * num_data * num_iteration``;
 *   - for feature contributions, its length is equal to ``num_class * num_data * (num_feature + 1)``.
 * \param handle Handle of booster
 * \param n_chunks The number of Arrow arrays passed to this function
 * \param chunks Pointer to the list of Arrow arrays
 * \param schema Pointer to the schema of all Arrow arrays
 * \param predict_type What should be predicted
 *   - ``C_API_PREDICT_NORMAL``: normal prediction, with transform (if needed);
 *   - ``C_API_PREDICT_RAW_SCORE``: raw score;
 *   - ``C_API_PREDICT_LEAF_INDEX``: leaf index;
 *   - ``C_API_PREDICT_CONTRIB``: feature contributions (SHAP values)
 * \param start_iteration Start index of the iteration to predict
 * \param num_iteration Number of iteration for prediction, <= 0 means no limit
 * \param parameter Other parameters for prediction, e.g. early stopping for prediction
 * \param[out] out_len Length of output result
 * \param[out] out_result Pointer to array with predictions
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterPredictForArrow_t) (BoosterHandle handle,
                                                  int64_t n_chunks,
                                                  const ArrowArray* chunks,
                                                  const ArrowSchema* schema,
                                                  int predict_type,
                                                  int start_iteration,
                                                  int num_iteration,
                                                  const char* parameter,
                                                  int64_t* out_len,
                                                  double* out_result);
static LGBM_BoosterPredictForArrow_t LGBM_BoosterPredictForArrow = NULL;

/*!
 * \brief Save model into file.
 * \param handle Handle of booster
 * \param start_iteration Start index of the iteration that should be saved
 * \param num_iteration Index of the iteration that should be saved, <= 0 means save all
 * \param feature_importance_type Type of feature importance, can be ``C_API_FEATURE_IMPORTANCE_SPLIT`` or ``C_API_FEATURE_IMPORTANCE_GAIN``
 * \param filename The name of the file
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterSaveModel_t) (BoosterHandle handle,
                                            int start_iteration,
                                            int num_iteration,
                                            int feature_importance_type,
                                            const char* filename);
static LGBM_BoosterSaveModel_t LGBM_BoosterSaveModel = NULL;

/*!
 * \brief Save model to string.
 * \param handle Handle of booster
 * \param start_iteration Start index of the iteration that should be saved
 * \param num_iteration Index of the iteration that should be saved, <= 0 means save all
 * \param feature_importance_type Type of feature importance, can be ``C_API_FEATURE_IMPORTANCE_SPLIT`` or ``C_API_FEATURE_IMPORTANCE_GAIN``
 * \param buffer_len String buffer length, if ``buffer_len < out_len``, you should re-allocate buffer
 * \param[out] out_len Actual output length
 * \param[out] out_str String of model, should pre-allocate memory
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterSaveModelToString_t) (BoosterHandle handle,
                                                    int start_iteration,
                                                    int num_iteration,
                                                    int feature_importance_type,
                                                    int64_t buffer_len,
                                                    int64_t* out_len,
                                                    char* out_str);
static LGBM_BoosterSaveModelToString_t LGBM_BoosterSaveModelToString = NULL;

/*!
 * \brief Dump model to JSON.
 * \param handle Handle of booster
 * \param start_iteration Start index of the iteration that should be dumped
 * \param num_iteration Index of the iteration that should be dumped, <= 0 means dump all
 * \param feature_importance_type Type of feature importance, can be ``C_API_FEATURE_IMPORTANCE_SPLIT`` or ``C_API_FEATURE_IMPORTANCE_GAIN``
 * \param buffer_len String buffer length, if ``buffer_len < out_len``, you should re-allocate buffer
 * \param[out] out_len Actual output length
 * \param[out] out_str JSON format string of model, should pre-allocate memory
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterDumpModel_t) (BoosterHandle handle,
                                            int start_iteration,
                                            int num_iteration,
                                            int feature_importance_type,
                                            int64_t buffer_len,
                                            int64_t* out_len,
                                            char* out_str);
static LGBM_BoosterDumpModel_t LGBM_BoosterDumpModel = NULL;

/*!
 * \brief Get leaf value.
 * \param handle Handle of booster
 * \param tree_idx Index of tree
 * \param leaf_idx Index of leaf
 * \param[out] out_val Output result from the specified leaf
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterGetLeafValue_t) (BoosterHandle handle,
                                               int tree_idx,
                                               int leaf_idx,
                                               double* out_val);
static LGBM_BoosterGetLeafValue_t LGBM_BoosterGetLeafValue = NULL;

/*!
 * \brief Set leaf value.
 * \param handle Handle of booster
 * \param tree_idx Index of tree
 * \param leaf_idx Index of leaf
 * \param val Leaf value
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterSetLeafValue_t) (BoosterHandle handle,
                                               int tree_idx,
                                               int leaf_idx,
                                               double val);
static LGBM_BoosterSetLeafValue_t LGBM_BoosterSetLeafValue = NULL;

/*!
 * \brief Get model feature importance.
 * \param handle Handle of booster
 * \param num_iteration Number of iterations for which feature importance is calculated, <= 0 means use all
 * \param importance_type Method of importance calculation:
 *   - ``C_API_FEATURE_IMPORTANCE_SPLIT``: result contains numbers of times the feature is used in a model;
 *   - ``C_API_FEATURE_IMPORTANCE_GAIN``: result contains total gains of splits which use the feature
 * \param[out] out_results Result array with feature importance
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterFeatureImportance_t) (BoosterHandle handle,
                                                    int num_iteration,
                                                    int importance_type,
                                                    double* out_results);
static LGBM_BoosterFeatureImportance_t LGBM_BoosterFeatureImportance = NULL;

/*!
 * \brief Get model upper bound value.
 * \param handle Handle of booster
 * \param[out] out_results Result pointing to max value
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterGetUpperBoundValue_t) (BoosterHandle handle,
                                                     double* out_results);
static LGBM_BoosterGetUpperBoundValue_t LGBM_BoosterGetUpperBoundValue = NULL;

/*!
 * \brief Get model lower bound value.
 * \param handle Handle of booster
 * \param[out] out_results Result pointing to min value
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_BoosterGetLowerBoundValue_t) (BoosterHandle handle,
                                                     double* out_results);
static LGBM_BoosterGetLowerBoundValue_t LGBM_BoosterGetLowerBoundValue = NULL;

/*!
 * \brief Initialize the network.
 * \param machines List of machines in format 'ip1:port1,ip2:port2'
 * \param local_listen_port TCP listen port for local machines
 * \param listen_time_out Socket time-out in minutes
 * \param num_machines Total number of machines
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_NetworkInit_t) (const char* machines,
                                       int local_listen_port,
                                       int listen_time_out,
                                       int num_machines);
static LGBM_NetworkInit_t LGBM_NetworkInit = NULL;

/*!
 * \brief Finalize the network.
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_NetworkFree_t) ();
static LGBM_NetworkFree_t LGBM_NetworkFree = NULL;

/*!
 * \brief Initialize the network with external collective functions.
 * \param num_machines Total number of machines
 * \param rank Rank of local machine
 * \param reduce_scatter_ext_fun The external reduce-scatter function
 * \param allgather_ext_fun The external allgather function
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_NetworkInitWithFunctions_t) (int num_machines,
                                                    int rank,
                                                    void* reduce_scatter_ext_fun,
                                                    void* allgather_ext_fun);
static LGBM_NetworkInitWithFunctions_t LGBM_NetworkInitWithFunctions = NULL;

/*!
 * \brief Set maximum number of threads used by LightGBM routines in this process.
 * \param num_threads maximum number of threads used by LightGBM. -1 means defaulting to omp_get_num_threads().
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_SetMaxThreads_t) (int num_threads);
static LGBM_SetMaxThreads_t LGBM_SetMaxThreads = NULL;

/*!
 * \brief Get current maximum number of threads used by LightGBM routines in this process.
 * \param[out] out current maximum number of threads used by LightGBM. -1 means defaulting to omp_get_num_threads().
 * \return 0 when succeed, -1 when failure happens
 */
LIGHTGBM_C_EXPORT int (*LGBM_GetMaxThreads_t) (int* out);
static LGBM_GetMaxThreads_t LGBM_GetMaxThreads = NULL;

#if !defined(__cplusplus) && (!defined(__STDC__) || (__STDC_VERSION__ < 199901L))
/*! \brief Inline specifier no-op in C using standards before C99. */
#define INLINE_FUNCTION
#else
/*! \brief Inline specifier. */
// hack-lightgbm - fix warning
#define INLINE_FUNCTION static inline
#endif

#if !defined(__cplusplus) && (!defined(__STDC__) || (__STDC_VERSION__ < 201112L))
/*! \brief Thread local specifier no-op in C using standards before C11. */
#define THREAD_LOCAL
#elif !defined(__cplusplus)
/*! \brief Thread local specifier. */
#define THREAD_LOCAL _Thread_local
#elif defined(_MSC_VER)
/*! \brief Thread local specifier. */
#define THREAD_LOCAL __declspec(thread)
#else
/*! \brief Thread local specifier. */
#define THREAD_LOCAL thread_local
#endif

/*!
 * \brief Handle of error message.
 * \return Error message
 */
static char* LastErrorMsg() { static THREAD_LOCAL char err_msg[512] = "Everything is fine"; return err_msg; }

#ifdef _MSC_VER
  #pragma warning(disable : 4996)
#endif
/*!
 * \brief Set string message of the last error.
 * \note
 * This will call unsafe ``sprintf`` when compiled using C standards before C99.
 * \param msg Error message
 */
INLINE_FUNCTION void LGBM_SetLastError(const char* msg) {
#if !defined(__cplusplus) && (!defined(__STDC__) || (__STDC_VERSION__ < 199901L))
  sprintf(LastErrorMsg(), "%s", msg);  /* NOLINT(runtime/printf) */
#else
  const int err_buf_len = 512;
  snprintf(LastErrorMsg(), err_buf_len, "%s", msg);
#endif
}

#endif  /* LIGHTGBM_C_API_H_ */


/*
file https://github.com/microsoft/LightGBM/blob/v4.6.0/include/LightGBM/c_api.h
*/
static void LGBM_dlsym() {
LGBM_DLSYM(LGBM_GetLastError);
LGBM_DLSYM(LGBM_DumpParamAliases);
LGBM_DLSYM(LGBM_RegisterLogCallback);
LGBM_DLSYM(LGBM_GetSampleCount);
LGBM_DLSYM(LGBM_SampleIndices);
LGBM_DLSYM(LGBM_ByteBufferGetAt);
LGBM_DLSYM(LGBM_ByteBufferFree);
LGBM_DLSYM(LGBM_DatasetCreateFromFile);
LGBM_DLSYM(LGBM_DatasetCreateFromSampledColumn);
LGBM_DLSYM(LGBM_DatasetCreateByReference);
LGBM_DLSYM(LGBM_DatasetInitStreaming);
LGBM_DLSYM(LGBM_DatasetCreateFromSerializedReference);
LGBM_DLSYM(LGBM_DatasetPushRows);
LGBM_DLSYM(LGBM_DatasetPushRowsWithMetadata);
LGBM_DLSYM(LGBM_DatasetPushRowsByCSR);
LGBM_DLSYM(LGBM_DatasetPushRowsByCSRWithMetadata);
LGBM_DLSYM(LGBM_DatasetSetWaitForManualFinish);
LGBM_DLSYM(LGBM_DatasetMarkFinished);
LGBM_DLSYM(LGBM_DatasetCreateFromCSR);
LGBM_DLSYM(LGBM_DatasetCreateFromCSRFunc);
LGBM_DLSYM(LGBM_DatasetCreateFromCSC);
LGBM_DLSYM(LGBM_DatasetCreateFromMat);
LGBM_DLSYM(LGBM_DatasetCreateFromMats);
LGBM_DLSYM(LGBM_DatasetCreateFromArrow);
LGBM_DLSYM(LGBM_DatasetGetSubset);
LGBM_DLSYM(LGBM_DatasetSetFeatureNames);
LGBM_DLSYM(LGBM_DatasetGetFeatureNames);
LGBM_DLSYM(LGBM_DatasetFree);
LGBM_DLSYM(LGBM_DatasetSaveBinary);
LGBM_DLSYM(LGBM_DatasetSerializeReferenceToBinary);
LGBM_DLSYM(LGBM_DatasetDumpText);
LGBM_DLSYM(LGBM_DatasetSetField);
LGBM_DLSYM(LGBM_DatasetSetFieldFromArrow);
LGBM_DLSYM(LGBM_DatasetGetField);
LGBM_DLSYM(LGBM_DatasetUpdateParamChecking);
LGBM_DLSYM(LGBM_DatasetGetNumData);
LGBM_DLSYM(LGBM_DatasetGetNumFeature);
LGBM_DLSYM(LGBM_DatasetGetFeatureNumBin);
LGBM_DLSYM(LGBM_DatasetAddFeaturesFrom);
LGBM_DLSYM(LGBM_BoosterGetLinear);
LGBM_DLSYM(LGBM_BoosterCreate);
LGBM_DLSYM(LGBM_BoosterCreateFromModelfile);
LGBM_DLSYM(LGBM_BoosterLoadModelFromString);
LGBM_DLSYM(LGBM_BoosterGetLoadedParam);
LGBM_DLSYM(LGBM_BoosterFree);
LGBM_DLSYM(LGBM_BoosterShuffleModels);
LGBM_DLSYM(LGBM_BoosterMerge);
LGBM_DLSYM(LGBM_BoosterAddValidData);
LGBM_DLSYM(LGBM_BoosterResetTrainingData);
LGBM_DLSYM(LGBM_BoosterResetParameter);
LGBM_DLSYM(LGBM_BoosterGetNumClasses);
LGBM_DLSYM(LGBM_BoosterUpdateOneIter);
LGBM_DLSYM(LGBM_BoosterRefit);
LGBM_DLSYM(LGBM_BoosterUpdateOneIterCustom);
LGBM_DLSYM(LGBM_BoosterRollbackOneIter);
LGBM_DLSYM(LGBM_BoosterGetCurrentIteration);
LGBM_DLSYM(LGBM_BoosterNumModelPerIteration);
LGBM_DLSYM(LGBM_BoosterNumberOfTotalModel);
LGBM_DLSYM(LGBM_BoosterGetEvalCounts);
LGBM_DLSYM(LGBM_BoosterGetEvalNames);
LGBM_DLSYM(LGBM_BoosterGetFeatureNames);
LGBM_DLSYM(LGBM_BoosterValidateFeatureNames);
LGBM_DLSYM(LGBM_BoosterGetNumFeature);
LGBM_DLSYM(LGBM_BoosterGetEval);
LGBM_DLSYM(LGBM_BoosterGetNumPredict);
LGBM_DLSYM(LGBM_BoosterGetPredict);
LGBM_DLSYM(LGBM_BoosterPredictForFile);
LGBM_DLSYM(LGBM_BoosterCalcNumPredict);
LGBM_DLSYM(LGBM_FastConfigFree);
LGBM_DLSYM(LGBM_BoosterPredictForCSR);
LGBM_DLSYM(LGBM_BoosterPredictSparseOutput);
LGBM_DLSYM(LGBM_BoosterFreePredictSparse);
LGBM_DLSYM(LGBM_BoosterPredictForCSRSingleRow);
LGBM_DLSYM(LGBM_BoosterPredictForCSRSingleRowFastInit);
LGBM_DLSYM(LGBM_BoosterPredictForCSRSingleRowFast);
LGBM_DLSYM(LGBM_BoosterPredictForCSC);
LGBM_DLSYM(LGBM_BoosterPredictForMat);
LGBM_DLSYM(LGBM_BoosterPredictForMatSingleRow);
LGBM_DLSYM(LGBM_BoosterPredictForMatSingleRowFastInit);
LGBM_DLSYM(LGBM_BoosterPredictForMatSingleRowFast);
LGBM_DLSYM(LGBM_BoosterPredictForMats);
LGBM_DLSYM(LGBM_BoosterPredictForArrow);
LGBM_DLSYM(LGBM_BoosterSaveModel);
LGBM_DLSYM(LGBM_BoosterSaveModelToString);
LGBM_DLSYM(LGBM_BoosterDumpModel);
LGBM_DLSYM(LGBM_BoosterGetLeafValue);
LGBM_DLSYM(LGBM_BoosterSetLeafValue);
LGBM_DLSYM(LGBM_BoosterFeatureImportance);
LGBM_DLSYM(LGBM_BoosterGetUpperBoundValue);
LGBM_DLSYM(LGBM_BoosterGetLowerBoundValue);
LGBM_DLSYM(LGBM_NetworkInit);
LGBM_DLSYM(LGBM_NetworkFree);
LGBM_DLSYM(LGBM_NetworkInitWithFunctions);
LGBM_DLSYM(LGBM_SetMaxThreads);
LGBM_DLSYM(LGBM_GetMaxThreads);
}
#endif // SRC_SQLMATH_BASE_C2


/*
repo https://github.com/nodejs/node/tree/v10.22.1
committed 2020-09-08T23:55:04Z
*/


/*
file https://github.com/nodejs/node/blob/v10.22.1/LICENSE
*/

#if defined(SRC_SQLMATH_NODEJS_C2)
/*
Node.js is licensed for use as follows:

"""
Copyright Node.js contributors. All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to
deal in the Software without restriction, including without limitation the
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.
"""

This license applies to parts of Node.js originating from the
https://github.com/joyent/node repository:

"""
Copyright Joyent, Inc. and other Node contributors. All rights reserved.
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to
deal in the Software without restriction, including without limitation the
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.
"""

The Node.js license applies to all parts of Node.js that are not externally
maintained libraries.

The externally maintained libraries used by Node.js are:

- Acorn, located at deps/acorn, is licensed as follows:
  """
    Copyright (C) 2012-2018 by various contributors (see AUTHORS)

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
  """

- c-ares, located at deps/cares, is licensed as follows:
  """
    Copyright (c) 2007 - 2018, Daniel Stenberg with many contributors, see AUTHORS
    file.

    Copyright 1998 by the Massachusetts Institute of Technology.

    Permission to use, copy, modify, and distribute this software and its
    documentation for any purpose and without fee is hereby granted, provided that
    the above copyright notice appear in all copies and that both that copyright
    notice and this permission notice appear in supporting documentation, and that
    the name of M.I.T. not be used in advertising or publicity pertaining to
    distribution of the software without specific, written prior permission.
    M.I.T. makes no representations about the suitability of this software for any
    purpose.  It is provided "as is" without express or implied warranty.
  """

- HTTP Parser, located at deps/http_parser, is licensed as follows:
  """
    Copyright Joyent, Inc. and other Node contributors.

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to
    deal in the Software without restriction, including without limitation the
    rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
    sell copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
    IN THE SOFTWARE.
  """

- ICU, located at deps/icu-small, is licensed as follows:
  """
    COPYRIGHT AND PERMISSION NOTICE (ICU 58 and later)

    Copyright © 1991-2019 Unicode, Inc. All rights reserved.
    Distributed under the Terms of Use in https://www.unicode.org/copyright.html.

    Permission is hereby granted, free of charge, to any person obtaining
    a copy of the Unicode data files and any associated documentation
    (the "Data Files") or Unicode software and any associated documentation
    (the "Software") to deal in the Data Files or Software
    without restriction, including without limitation the rights to use,
    copy, modify, merge, publish, distribute, and/or sell copies of
    the Data Files or Software, and to permit persons to whom the Data Files
    or Software are furnished to do so, provided that either
    (a) this copyright and permission notice appear with all copies
    of the Data Files or Software, or
    (b) this copyright and permission notice appear in associated
    Documentation.

    THE DATA FILES AND SOFTWARE ARE PROVIDED "AS IS", WITHOUT WARRANTY OF
    ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT OF THIRD PARTY RIGHTS.
    IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS INCLUDED IN THIS
    NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT OR CONSEQUENTIAL
    DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
    DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
    TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
    PERFORMANCE OF THE DATA FILES OR SOFTWARE.

    Except as contained in this notice, the name of a copyright holder
    shall not be used in advertising or otherwise to promote the sale,
    use or other dealings in these Data Files or Software without prior
    written authorization of the copyright holder.

    ---------------------

    Third-Party Software Licenses

    This section contains third-party software notices and/or additional
    terms for licensed third-party software components included within ICU
    libraries.

    1. ICU License - ICU 1.8.1 to ICU 57.1

    COPYRIGHT AND PERMISSION NOTICE

    Copyright (c) 1995-2016 International Business Machines Corporation and others
    All rights reserved.

    Permission is hereby granted, free of charge, to any person obtaining
    a copy of this software and associated documentation files (the
    "Software"), to deal in the Software without restriction, including
    without limitation the rights to use, copy, modify, merge, publish,
    distribute, and/or sell copies of the Software, and to permit persons
    to whom the Software is furnished to do so, provided that the above
    copyright notice(s) and this permission notice appear in all copies of
    the Software and that both the above copyright notice(s) and this
    permission notice appear in supporting documentation.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT
    OF THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
    HOLDERS INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY
    SPECIAL INDIRECT OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER
    RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF
    CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
    CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

    Except as contained in this notice, the name of a copyright holder
    shall not be used in advertising or otherwise to promote the sale, use
    or other dealings in this Software without prior written authorization
    of the copyright holder.

    All trademarks and registered trademarks mentioned herein are the
    property of their respective owners.

    2. Chinese/Japanese Word Break Dictionary Data (cjdict.txt)

     #     The Google Chrome software developed by Google is licensed under
     # the BSD license. Other software included in this distribution is
     # provided under other licenses, as set forth below.
     #
     #  The BSD License
     #  http://opensource.org/licenses/bsd-license.php
     #  Copyright (C) 2006-2008, Google Inc.
     #
     #  All rights reserved.
     #
     #  Redistribution and use in source and binary forms, with or without
     # modification, are permitted provided that the following conditions are met:
     #
     #  Redistributions of source code must retain the above copyright notice,
     # this list of conditions and the following disclaimer.
     #  Redistributions in binary form must reproduce the above
     # copyright notice, this list of conditions and the following
     # disclaimer in the documentation and/or other materials provided with
     # the distribution.
     #  Neither the name of  Google Inc. nor the names of its
     # contributors may be used to endorse or promote products derived from
     # this software without specific prior written permission.
     #
     #
     #  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
     # CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
     # INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
     # MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
     # DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
     # LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
     # CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
     # SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
     # BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
     # LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
     # NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
     # SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
     #
     #
     #  The word list in cjdict.txt are generated by combining three word lists
     # listed below with further processing for compound word breaking. The
     # frequency is generated with an iterative training against Google web
     # corpora.
     #
     #  * Libtabe (Chinese)
     #    - https://sourceforge.net/project/?group_id=1519
     #    - Its license terms and conditions are shown below.
     #
     #  * IPADIC (Japanese)
     #    - http://chasen.aist-nara.ac.jp/chasen/distribution.html
     #    - Its license terms and conditions are shown below.
     #
     #  ---------COPYING.libtabe ---- BEGIN--------------------
     #
     #  /\*
     #   * Copyright (c) 1999 TaBE Project.
     #   * Copyright (c) 1999 Pai-Hsiang Hsiao.
     #   * All rights reserved.
     #   *
     #   * Redistribution and use in source and binary forms, with or without
     #   * modification, are permitted provided that the following conditions
     #   * are met:
     #   *
     #   * . Redistributions of source code must retain the above copyright
     #   *   notice, this list of conditions and the following disclaimer.
     #   * . Redistributions in binary form must reproduce the above copyright
     #   *   notice, this list of conditions and the following disclaimer in
     #   *   the documentation and/or other materials provided with the
     #   *   distribution.
     #   * . Neither the name of the TaBE Project nor the names of its
     #   *   contributors may be used to endorse or promote products derived
     #   *   from this software without specific prior written permission.
     #   *
     #   * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
     #   * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
     #   * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
     #   * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
     #   * REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
     #   * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
     #   * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
     #   * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
     #   * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
     #   * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
     #   * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
     #   * OF THE POSSIBILITY OF SUCH DAMAGE.
     #   *\/
     #
     #  /\*
     #   * Copyright (c) 1999 Computer Systems and Communication Lab,
     #   *                    Institute of Information Science, Academia
     #       *                    Sinica. All rights reserved.
     #   *
     #   * Redistribution and use in source and binary forms, with or without
     #   * modification, are permitted provided that the following conditions
     #   * are met:
     #   *
     #   * . Redistributions of source code must retain the above copyright
     #   *   notice, this list of conditions and the following disclaimer.
     #   * . Redistributions in binary form must reproduce the above copyright
     #   *   notice, this list of conditions and the following disclaimer in
     #   *   the documentation and/or other materials provided with the
     #   *   distribution.
     #   * . Neither the name of the Computer Systems and Communication Lab
     #   *   nor the names of its contributors may be used to endorse or
     #   *   promote products derived from this software without specific
     #   *   prior written permission.
     #   *
     #   * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
     #   * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
     #   * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
     #   * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
     #   * REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
     #   * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
     #   * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
     #   * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
     #   * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
     #   * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
     #   * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
     #   * OF THE POSSIBILITY OF SUCH DAMAGE.
     #   *\/
     #
     #  Copyright 1996 Chih-Hao Tsai @ Beckman Institute,
     #      University of Illinois
     #  c-tsai4@uiuc.edu  http://casper.beckman.uiuc.edu/~c-tsai4
     #
     #  ---------------COPYING.libtabe-----END--------------------------------
     #
     #
     #  ---------------COPYING.ipadic-----BEGIN-------------------------------
     #
     #  Copyright 2000, 2001, 2002, 2003 Nara Institute of Science
     #  and Technology.  All Rights Reserved.
     #
     #  Use, reproduction, and distribution of this software is permitted.
     #  Any copy of this software, whether in its original form or modified,
     #  must include both the above copyright notice and the following
     #  paragraphs.
     #
     #  Nara Institute of Science and Technology (NAIST),
     #  the copyright holders, disclaims all warranties with regard to this
     #  software, including all implied warranties of merchantability and
     #  fitness, in no event shall NAIST be liable for
     #  any special, indirect or consequential damages or any damages
     #  whatsoever resulting from loss of use, data or profits, whether in an
     #  action of contract, negligence or other tortuous action, arising out
     #  of or in connection with the use or performance of this software.
     #
     #  A large portion of the dictionary entries
     #  originate from ICOT Free Software.  The following conditions for ICOT
     #  Free Software applies to the current dictionary as well.
     #
     #  Each User may also freely distribute the Program, whether in its
     #  original form or modified, to any third party or parties, PROVIDED
     #  that the provisions of Section 3 ("NO WARRANTY") will ALWAYS appear
     #  on, or be attached to, the Program, which is distributed substantially
     #  in the same form as set out herein and that such intended
     #  distribution, if actually made, will neither violate or otherwise
     #  contravene any of the laws and regulations of the countries having
     #  jurisdiction over the User or the intended distribution itself.
     #
     #  NO WARRANTY
     #
     #  The program was produced on an experimental basis in the course of the
     #  research and development conducted during the project and is provided
     #  to users as so produced on an experimental basis.  Accordingly, the
     #  program is provided without any warranty whatsoever, whether express,
     #  implied, statutory or otherwise.  The term "warranty" used herein
     #  includes, but is not limited to, any warranty of the quality,
     #  performance, merchantability and fitness for a particular purpose of
     #  the program and the nonexistence of any infringement or violation of
     #  any right of any third party.
     #
     #  Each user of the program will agree and understand, and be deemed to
     #  have agreed and understood, that there is no warranty whatsoever for
     #  the program and, accordingly, the entire risk arising from or
     #  otherwise connected with the program is assumed by the user.
     #
     #  Therefore, neither ICOT, the copyright holder, or any other
     #  organization that participated in or was otherwise related to the
     #  development of the program and their respective officials, directors,
     #  officers and other employees shall be held liable for any and all
     #  damages, including, without limitation, general, special, incidental
     #  and consequential damages, arising out of or otherwise in connection
     #  with the use or inability to use the program or any product, material
     #  or result produced or otherwise obtained by using the program,
     #  regardless of whether they have been advised of, or otherwise had
     #  knowledge of, the possibility of such damages at any time during the
     #  project or thereafter.  Each user will be deemed to have agreed to the
     #  foregoing by his or her commencement of use of the program.  The term
     #  "use" as used herein includes, but is not limited to, the use,
     #  modification, copying and distribution of the program and the
     #  production of secondary products from the program.
     #
     #  In the case where the program, whether in its original form or
     #  modified, was distributed or delivered to or received by a user from
     #  any person, organization or entity other than ICOT, unless it makes or
     #  grants independently of ICOT any specific warranty to the user in
     #  writing, such person, organization or entity, will also be exempted
     #  from and not be held liable to the user for any such damages as noted
     #  above as far as the program is concerned.
     #
     #  ---------------COPYING.ipadic-----END----------------------------------

    3. Lao Word Break Dictionary Data (laodict.txt)

     #  Copyright (c) 2013 International Business Machines Corporation
     #  and others. All Rights Reserved.
     #
     # Project: http://code.google.com/p/lao-dictionary/
     # Dictionary: http://lao-dictionary.googlecode.com/git/Lao-Dictionary.txt
     # License: http://lao-dictionary.googlecode.com/git/Lao-Dictionary-LICENSE.txt
     #              (copied below)
     #
     #  This file is derived from the above dictionary, with slight
     #  modifications.
     #  ----------------------------------------------------------------------
     #  Copyright (C) 2013 Brian Eugene Wilson, Robert Martin Campbell.
     #  All rights reserved.
     #
     #  Redistribution and use in source and binary forms, with or without
     #  modification,
     #  are permitted provided that the following conditions are met:
     #
     #
     # Redistributions of source code must retain the above copyright notice, this
     #  list of conditions and the following disclaimer. Redistributions in
     #  binary form must reproduce the above copyright notice, this list of
     #  conditions and the following disclaimer in the documentation and/or
     #  other materials provided with the distribution.
     #
     #
     # THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
     # "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
     # LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
     # FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
     # COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
     # INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
     # (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
     # SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
     # HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
     # STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
     # ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
     # OF THE POSSIBILITY OF SUCH DAMAGE.
     #  --------------------------------------------------------------------------

    4. Burmese Word Break Dictionary Data (burmesedict.txt)

     #  Copyright (c) 2014 International Business Machines Corporation
     #  and others. All Rights Reserved.
     #
     #  This list is part of a project hosted at:
     #    github.com/kanyawtech/myanmar-karen-word-lists
     #
     #  --------------------------------------------------------------------------
     #  Copyright (c) 2013, LeRoy Benjamin Sharon
     #  All rights reserved.
     #
     #  Redistribution and use in source and binary forms, with or without
     #  modification, are permitted provided that the following conditions
     #  are met: Redistributions of source code must retain the above
     #  copyright notice, this list of conditions and the following
     #  disclaimer.  Redistributions in binary form must reproduce the
     #  above copyright notice, this list of conditions and the following
     #  disclaimer in the documentation and/or other materials provided
     #  with the distribution.
     #
     #    Neither the name Myanmar Karen Word Lists, nor the names of its
     #    contributors may be used to endorse or promote products derived
     #    from this software without specific prior written permission.
     #
     #  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
     #  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
     #  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
     #  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
     #  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
     #  BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
     #  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
     #  TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
     #  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
     #  ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
     #  TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
     #  THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
     #  SUCH DAMAGE.
     #  --------------------------------------------------------------------------

    5. Time Zone Database

      ICU uses the public domain data and code derived from Time Zone
    Database for its time zone support. The ownership of the TZ database
    is explained in BCP 175: Procedure for Maintaining the Time Zone
    Database section 7.

     # 7.  Database Ownership
     #
     #    The TZ database itself is not an IETF Contribution or an IETF
     #    document.  Rather it is a pre-existing and regularly updated work
     #    that is in the public domain, and is intended to remain in the
     #    public domain.  Therefore, BCPs 78 [RFC5378] and 79 [RFC3979] do
     #    not apply to the TZ Database or contributions that individuals make
     #    to it.  Should any claims be made and substantiated against the TZ
     #    Database, the organization that is providing the IANA
     #    Considerations defined in this RFC, under the memorandum of
     #    understanding with the IETF, currently ICANN, may act in accordance
     #    with all competent court orders.  No ownership claims will be made
     #    by ICANN or the IETF Trust on the database or the code.  Any person
     #    making a contribution to the database or code waives all rights to
     #    future claims in that contribution or in the TZ Database.

    6. Google double-conversion

    Copyright 2006-2011, the V8 project authors. All rights reserved.
    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are
    met:

        * Redistributions of source code must retain the above copyright
          notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above
          copyright notice, this list of conditions and the following
          disclaimer in the documentation and/or other materials provided
          with the distribution.
        * Neither the name of Google Inc. nor the names of its
          contributors may be used to endorse or promote products derived
          from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
    A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
    OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  """

- libuv, located at deps/uv, is licensed as follows:
  """
    libuv is licensed for use as follows:

    ====
    Copyright (c) 2015-present libuv project contributors.

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to
    deal in the Software without restriction, including without limitation the
    rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
    sell copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
    IN THE SOFTWARE.
    ====

    This license applies to parts of libuv originating from the
    https://github.com/joyent/libuv repository:

    ====

    Copyright Joyent, Inc. and other Node contributors. All rights reserved.
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to
    deal in the Software without restriction, including without limitation the
    rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
    sell copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
    IN THE SOFTWARE.

    ====

    This license applies to all parts of libuv that are not externally
    maintained libraries.

    The externally maintained libraries used by libuv are:

      - tree.h (from FreeBSD), copyright Niels Provos. Two clause BSD license.

      - inet_pton and inet_ntop implementations, contained in src/inet.c, are
        copyright the Internet Systems Consortium, Inc., and licensed under the ISC
        license.

      - stdint-msvc2008.h (from msinttypes), copyright Alexander Chemeris. Three
        clause BSD license.

      - pthread-fixes.c, copyright Google Inc. and Sony Mobile Communications AB.
        Three clause BSD license.

      - android-ifaddrs.h, android-ifaddrs.c, copyright Berkeley Software Design
        Inc, Kenneth MacKay and Emergya (Cloud4all, FP7/2007-2013, grant agreement
        n° 289016). Three clause BSD license.
  """

- OpenSSL, located at deps/openssl, is licensed as follows:
  """
    Copyright (c) 1998-2018 The OpenSSL Project.  All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:

    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in
    the documentation and/or other materials provided with the
    distribution.

    3. All advertising materials mentioning features or use of this
    software must display the following acknowledgment:
    "This product includes software developed by the OpenSSL Project
    for use in the OpenSSL Toolkit. (http://www.openssl.org/)"

    4. The names "OpenSSL Toolkit" and "OpenSSL Project" must not be used to
    endorse or promote products derived from this software without
    prior written permission. For written permission, please contact
    openssl-core@openssl.org.

    5. Products derived from this software may not be called "OpenSSL"
    nor may "OpenSSL" appear in their names without prior written
    permission of the OpenSSL Project.

    6. Redistributions of any form whatsoever must retain the following
    acknowledgment:
    "This product includes software developed by the OpenSSL Project
    for use in the OpenSSL Toolkit (http://www.openssl.org/)"

    THIS SOFTWARE IS PROVIDED BY THE OpenSSL PROJECT ``AS IS'' AND ANY
    EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
    PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE OpenSSL PROJECT OR
    ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
    NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
    HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
    STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
    OF THE POSSIBILITY OF SUCH DAMAGE.
    ====================================================================

    This product includes cryptographic software written by Eric Young
    (eay@cryptsoft.com).  This product includes software written by Tim
    Hudson (tjh@cryptsoft.com).
  """

- Punycode.js, located at lib/punycode.js, is licensed as follows:
  """
    Copyright Mathias Bynens <https://mathiasbynens.be/>

    Permission is hereby granted, free of charge, to any person obtaining
    a copy of this software and associated documentation files (the
    "Software"), to deal in the Software without restriction, including
    without limitation the rights to use, copy, modify, merge, publish,
    distribute, sublicense, and/or sell copies of the Software, and to
    permit persons to whom the Software is furnished to do so, subject to
    the following conditions:

    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
    LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
    OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
    WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
  """

- V8, located at deps/v8, is licensed as follows:
  """
    This license applies to all parts of V8 that are not externally
    maintained libraries.  The externally maintained libraries used by V8
    are:

      - PCRE test suite, located in
        test/mjsunit/third_party/regexp-pcre/regexp-pcre.js.  This is based on the
        test suite from PCRE-7.3, which is copyrighted by the University
        of Cambridge and Google, Inc.  The copyright notice and license
        are embedded in regexp-pcre.js.

      - Layout tests, located in test/mjsunit/third_party/object-keys.  These are
        based on layout tests from webkit.org which are copyrighted by
        Apple Computer, Inc. and released under a 3-clause BSD license.

      - Strongtalk assembler, the basis of the files assembler-arm-inl.h,
        assembler-arm.cc, assembler-arm.h, assembler-ia32-inl.h,
        assembler-ia32.cc, assembler-ia32.h, assembler-x64-inl.h,
        assembler-x64.cc, assembler-x64.h, assembler-mips-inl.h,
        assembler-mips.cc, assembler-mips.h, assembler.cc and assembler.h.
        This code is copyrighted by Sun Microsystems Inc. and released
        under a 3-clause BSD license.

      - Valgrind client API header, located at third_party/valgrind/valgrind.h
        This is release under the BSD license.

      - antlr4 parser generator Cpp library located in third_party/antlr4
        This is release under the BSD license.

    These libraries have their own licenses; we recommend you read them,
    as their terms may differ from the terms below.

    Further license information can be found in LICENSE files located in
    sub-directories.

    Copyright 2014, the V8 project authors. All rights reserved.
    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are
    met:

        * Redistributions of source code must retain the above copyright
          notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above
          copyright notice, this list of conditions and the following
          disclaimer in the documentation and/or other materials provided
          with the distribution.
        * Neither the name of Google Inc. nor the names of its
          contributors may be used to endorse or promote products derived
          from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
    A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
    OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  """

- SipHash, located at deps/v8/src/third_party/siphash, is licensed as follows:
  """
    SipHash reference C implementation

    Copyright (c) 2016 Jean-Philippe Aumasson <jeanphilippe.aumasson@gmail.com>

    To the extent possible under law, the author(s) have dedicated all
    copyright and related and neighboring rights to this software to the public
    domain worldwide. This software is distributed without any warranty.
  """

- zlib, located at deps/zlib, is licensed as follows:
  """
    zlib.h -- interface of the 'zlib' general purpose compression library
    version 1.2.11, January 15th, 2017

    Copyright (C) 1995-2017 Jean-loup Gailly and Mark Adler

    This software is provided 'as-is', without any express or implied
    warranty.  In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.

    Jean-loup Gailly        Mark Adler
    jloup@gzip.org          madler@alumni.caltech.edu
  """

- npm, located at deps/npm, is licensed as follows:
  """
    The npm application
    Copyright (c) npm, Inc. and Contributors
    Licensed on the terms of The Artistic License 2.0

    Node package dependencies of the npm application
    Copyright (c) their respective copyright owners
    Licensed on their respective license terms

    The npm public registry at https://registry.npmjs.org
    and the npm website at https://www.npmjs.com
    Operated by npm, Inc.
    Use governed by terms published on https://www.npmjs.com

    "Node.js"
    Trademark Joyent, Inc., https://joyent.com
    Neither npm nor npm, Inc. are affiliated with Joyent, Inc.

    The Node.js application
    Project of Node Foundation, https://nodejs.org

    The npm Logo
    Copyright (c) Mathias Pettersson and Brian Hammond

    "Gubblebum Blocky" typeface
    Copyright (c) Tjarda Koster, https://jelloween.deviantart.com
    Used with permission

    --------

    The Artistic License 2.0

    Copyright (c) 2000-2006, The Perl Foundation.

    Everyone is permitted to copy and distribute verbatim copies
    of this license document, but changing it is not allowed.

    Preamble

    This license establishes the terms under which a given free software
    Package may be copied, modified, distributed, and/or redistributed.
    The intent is that the Copyright Holder maintains some artistic
    control over the development of that Package while still keeping the
    Package available as open source and free software.

    You are always permitted to make arrangements wholly outside of this
    license directly with the Copyright Holder of a given Package.  If the
    terms of this license do not permit the full use that you propose to
    make of the Package, you should contact the Copyright Holder and seek
    a different licensing arrangement.

    Definitions

        "Copyright Holder" means the individual(s) or organization(s)
        named in the copyright notice for the entire Package.

        "Contributor" means any party that has contributed code or other
        material to the Package, in accordance with the Copyright Holder's
        procedures.

        "You" and "your" means any person who would like to copy,
        distribute, or modify the Package.

        "Package" means the collection of files distributed by the
        Copyright Holder, and derivatives of that collection and/or of
        those files. A given Package may consist of either the Standard
        Version, or a Modified Version.

        "Distribute" means providing a copy of the Package or making it
        accessible to anyone else, or in the case of a company or
        organization, to others outside of your company or organization.

        "Distributor Fee" means any fee that you charge for Distributing
        this Package or providing support for this Package to another
        party.  It does not mean licensing fees.

        "Standard Version" refers to the Package if it has not been
        modified, or has been modified only in ways explicitly requested
        by the Copyright Holder.

        "Modified Version" means the Package, if it has been changed, and
        such changes were not explicitly requested by the Copyright
        Holder.

        "Original License" means this Artistic License as Distributed with
        the Standard Version of the Package, in its current version or as
        it may be modified by The Perl Foundation in the future.

        "Source" form means the source code, documentation source, and
        configuration files for the Package.

        "Compiled" form means the compiled bytecode, object code, binary,
        or any other form resulting from mechanical transformation or
        translation of the Source form.

    Permission for Use and Modification Without Distribution

    (1)  You are permitted to use the Standard Version and create and use
    Modified Versions for any purpose without restriction, provided that
    you do not Distribute the Modified Version.

    Permissions for Redistribution of the Standard Version

    (2)  You may Distribute verbatim copies of the Source form of the
    Standard Version of this Package in any medium without restriction,
    either gratis or for a Distributor Fee, provided that you duplicate
    all of the original copyright notices and associated disclaimers.  At
    your discretion, such verbatim copies may or may not include a
    Compiled form of the Package.

    (3)  You may apply any bug fixes, portability changes, and other
    modifications made available from the Copyright Holder.  The resulting
    Package will still be considered the Standard Version, and as such
    will be subject to the Original License.

    Distribution of Modified Versions of the Package as Source

    (4)  You may Distribute your Modified Version as Source (either gratis
    or for a Distributor Fee, and with or without a Compiled form of the
    Modified Version) provided that you clearly document how it differs
    from the Standard Version, including, but not limited to, documenting
    any non-standard features, executables, or modules, and provided that
    you do at least ONE of the following:

        (a)  make the Modified Version available to the Copyright Holder
        of the Standard Version, under the Original License, so that the
        Copyright Holder may include your modifications in the Standard
        Version.

        (b)  ensure that installation of your Modified Version does not
        prevent the user installing or running the Standard Version. In
        addition, the Modified Version must bear a name that is different
        from the name of the Standard Version.

        (c)  allow anyone who receives a copy of the Modified Version to
        make the Source form of the Modified Version available to others
        under

            (i)  the Original License or

            (ii)  a license that permits the licensee to freely copy,
            modify and redistribute the Modified Version using the same
            licensing terms that apply to the copy that the licensee
            received, and requires that the Source form of the Modified
            Version, and of any works derived from it, be made freely
            available in that license fees are prohibited but Distributor
            Fees are allowed.

    Distribution of Compiled Forms of the Standard Version
    or Modified Versions without the Source

    (5)  You may Distribute Compiled forms of the Standard Version without
    the Source, provided that you include complete instructions on how to
    get the Source of the Standard Version.  Such instructions must be
    valid at the time of your distribution.  If these instructions, at any
    time while you are carrying out such distribution, become invalid, you
    must provide new instructions on demand or cease further distribution.
    If you provide valid instructions or cease distribution within thirty
    days after you become aware that the instructions are invalid, then
    you do not forfeit any of your rights under this license.

    (6)  You may Distribute a Modified Version in Compiled form without
    the Source, provided that you comply with Section 4 with respect to
    the Source of the Modified Version.

    Aggregating or Linking the Package

    (7)  You may aggregate the Package (either the Standard Version or
    Modified Version) with other packages and Distribute the resulting
    aggregation provided that you do not charge a licensing fee for the
    Package.  Distributor Fees are permitted, and licensing fees for other
    components in the aggregation are permitted. The terms of this license
    apply to the use and Distribution of the Standard or Modified Versions
    as included in the aggregation.

    (8) You are permitted to link Modified and Standard Versions with
    other works, to embed the Package in a larger work of your own, or to
    build stand-alone binary or bytecode versions of applications that
    include the Package, and Distribute the result without restriction,
    provided the result does not expose a direct interface to the Package.

    Items That are Not Considered Part of a Modified Version

    (9) Works (including, but not limited to, modules and scripts) that
    merely extend or make use of the Package, do not, by themselves, cause
    the Package to be a Modified Version.  In addition, such works are not
    considered parts of the Package itself, and are not subject to the
    terms of this license.

    General Provisions

    (10)  Any use, modification, and distribution of the Standard or
    Modified Versions is governed by this Artistic License. By using,
    modifying or distributing the Package, you accept this license. Do not
    use, modify, or distribute the Package, if you do not accept this
    license.

    (11)  If your Modified Version has been derived from a Modified
    Version made by someone other than you, you are nevertheless required
    to ensure that your Modified Version complies with the requirements of
    this license.

    (12)  This license does not grant you the right to use any trademark,
    service mark, tradename, or logo of the Copyright Holder.

    (13)  This license includes the non-exclusive, worldwide,
    free-of-charge patent license to make, have made, use, offer to sell,
    sell, import and otherwise transfer the Package with respect to any
    patent claims licensable by the Copyright Holder that are necessarily
    infringed by the Package. If you institute patent litigation
    (including a cross-claim or counterclaim) against any party alleging
    that the Package constitutes direct or contributory patent
    infringement, then this Artistic License to you shall terminate on the
    date that such litigation is filed.

    (14)  Disclaimer of Warranty:
    THE PACKAGE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS "AS
    IS' AND WITHOUT ANY EXPRESS OR IMPLIED WARRANTIES. THE IMPLIED
    WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR
    NON-INFRINGEMENT ARE DISCLAIMED TO THE EXTENT PERMITTED BY YOUR LOCAL
    LAW. UNLESS REQUIRED BY LAW, NO COPYRIGHT HOLDER OR CONTRIBUTOR WILL
    BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, OR CONSEQUENTIAL
    DAMAGES ARISING IN ANY WAY OUT OF THE USE OF THE PACKAGE, EVEN IF
    ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

    --------
  """

- GYP, located at tools/gyp, is licensed as follows:
  """
    Copyright (c) 2009 Google Inc. All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are
    met:

       * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
       * Redistributions in binary form must reproduce the above
    copyright notice, this list of conditions and the following disclaimer
    in the documentation and/or other materials provided with the
    distribution.
       * Neither the name of Google Inc. nor the names of its
    contributors may be used to endorse or promote products derived from
    this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
    A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
    OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  """

- inspector_protocol, located at tools/inspector_protocol, is licensed as follows:
  """
    // Copyright 2016 The Chromium Authors. All rights reserved.
    //
    // Redistribution and use in source and binary forms, with or without
    // modification, are permitted provided that the following conditions are
    // met:
    //
    //    * Redistributions of source code must retain the above copyright
    // notice, this list of conditions and the following disclaimer.
    //    * Redistributions in binary form must reproduce the above
    // copyright notice, this list of conditions and the following disclaimer
    // in the documentation and/or other materials provided with the
    // distribution.
    //    * Neither the name of Google Inc. nor the names of its
    // contributors may be used to endorse or promote products derived from
    // this software without specific prior written permission.
    //
    // THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    // "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    // LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
    // A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
    // OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    // SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    // LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    // DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    // THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    // (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    // OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  """

- jinja2, located at tools/inspector_protocol/jinja2, is licensed as follows:
  """
    Copyright (c) 2009 by the Jinja Team, see AUTHORS for more details.

    Some rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are
    met:

        * Redistributions of source code must retain the above copyright
          notice, this list of conditions and the following disclaimer.

        * Redistributions in binary form must reproduce the above
          copyright notice, this list of conditions and the following
          disclaimer in the documentation and/or other materials provided
          with the distribution.

        * The names of the contributors may not be used to endorse or
          promote products derived from this software without specific
          prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
    A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
    OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  """

- markupsafe, located at tools/inspector_protocol/markupsafe, is licensed as follows:
  """
    Copyright (c) 2010 by Armin Ronacher and contributors.  See AUTHORS
    for more details.

    Some rights reserved.

    Redistribution and use in source and binary forms of the software as well
    as documentation, with or without modification, are permitted provided
    that the following conditions are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.

    * The names of the contributors may not be used to endorse or
      promote products derived from this software without specific
      prior written permission.

    THIS SOFTWARE AND DOCUMENTATION IS PROVIDED BY THE COPYRIGHT HOLDERS AND
    CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT
    NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
    A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
    OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
    EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
    PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
    PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
    LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
    NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE AND DOCUMENTATION, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
    DAMAGE.
  """

- cpplint.py, located at tools/cpplint.py, is licensed as follows:
  """
    Copyright (c) 2009 Google Inc. All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are
    met:

       * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
       * Redistributions in binary form must reproduce the above
    copyright notice, this list of conditions and the following disclaimer
    in the documentation and/or other materials provided with the
    distribution.
       * Neither the name of Google Inc. nor the names of its
    contributors may be used to endorse or promote products derived from
    this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
    A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
    OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  """

- ESLint, located at tools/node_modules/eslint, is licensed as follows:
  """
    Copyright JS Foundation and other contributors, https://js.foundation

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
  """

- babel-eslint, located at tools/node_modules/babel-eslint, is licensed as follows:
  """
    Copyright (c) 2014-2016 Sebastian McKenzie <sebmck@gmail.com>

    MIT License

    Permission is hereby granted, free of charge, to any person obtaining
    a copy of this software and associated documentation files (the
    "Software"), to deal in the Software without restriction, including
    without limitation the rights to use, copy, modify, merge, publish,
    distribute, sublicense, and/or sell copies of the Software, and to
    permit persons to whom the Software is furnished to do so, subject to
    the following conditions:

    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
    LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
    OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
    WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
  """

- gtest, located at test/cctest/gtest, is licensed as follows:
  """
    Copyright 2008, Google Inc.
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are
    met:

        * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above
    copyright notice, this list of conditions and the following disclaimer
    in the documentation and/or other materials provided with the
    distribution.
        * Neither the name of Google Inc. nor the names of its
    contributors may be used to endorse or promote products derived from
    this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
    A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
    OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  """

- nghttp2, located at deps/nghttp2, is licensed as follows:
  """
    The MIT License

    Copyright (c) 2012, 2014, 2015, 2016 Tatsuhiro Tsujikawa
    Copyright (c) 2012, 2014, 2015, 2016 nghttp2 contributors

    Permission is hereby granted, free of charge, to any person obtaining
    a copy of this software and associated documentation files (the
    "Software"), to deal in the Software without restriction, including
    without limitation the rights to use, copy, modify, merge, publish,
    distribute, sublicense, and/or sell copies of the Software, and to
    permit persons to whom the Software is furnished to do so, subject to
    the following conditions:

    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
    LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
    OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
    WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
  """

- node-inspect, located at deps/node-inspect, is licensed as follows:
  """
    Copyright Node.js contributors. All rights reserved.

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to
    deal in the Software without restriction, including without limitation the
    rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
    sell copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
    IN THE SOFTWARE.
  """

- large_pages, located at src/large_pages, is licensed as follows:
  """
     Copyright (C) 2018 Intel Corporation

     Permission is hereby granted, free of charge, to any person obtaining a copy
     of this software and associated documentation files (the "Software"),
     to deal in the Software without restriction, including without limitation
     the rights to use, copy, modify, merge, publish, distribute, sublicense,
     and/or sell copies of the Software, and to permit persons to whom
     the Software is furnished to do so, subject to the following conditions:

     The above copyright notice and this permission notice shall be included
     in all copies or substantial portions of the Software.

     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
     OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
     THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
     OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
     ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
     OR OTHER DEALINGS IN THE SOFTWARE.
  """

- brotli, located at deps/brotli, is licensed as follows:
  """
    Copyright (c) 2009, 2010, 2013-2016 by the Brotli Authors.

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
  """
*/


/*
file https://github.com/nodejs/node/blob/v10.22.1/src/node_api_types.h
*/
#ifndef SRC_NODE_API_TYPES_H_
#define SRC_NODE_API_TYPES_H_

#include <stddef.h>
#include <stdint.h>

#if !defined __cplusplus || (defined(_MSC_VER) && _MSC_VER < 1900)
    typedef uint16_t char16_t;
#endif

// JSVM API types are all opaque pointers for ABI stability
// typedef undefined structs instead of void* for compile time type safety
typedef struct napi_env__* napi_env;
typedef struct napi_value__* napi_value;
typedef struct napi_ref__* napi_ref;
typedef struct napi_handle_scope__* napi_handle_scope;
typedef struct napi_escapable_handle_scope__* napi_escapable_handle_scope;
typedef struct napi_callback_scope__* napi_callback_scope;
typedef struct napi_callback_info__* napi_callback_info;
typedef struct napi_async_context__* napi_async_context;
typedef struct napi_async_work__* napi_async_work;
typedef struct napi_deferred__* napi_deferred;
#if NAPI_VERSION >= 4
typedef struct napi_threadsafe_function__* napi_threadsafe_function;
#endif  // NAPI_VERSION >= 4

typedef enum {
  napi_default = 0,
  napi_writable = 1 << 0,
  napi_enumerable = 1 << 1,
  napi_configurable = 1 << 2,

  // Used with napi_define_class to distinguish static properties
  // from instance properties. Ignored by napi_define_properties.
  napi_static = 1 << 10,
} napi_property_attributes;

typedef enum {
  // ES6 types (corresponds to typeof)
  napi_undefined,
  napi_null,
  napi_boolean,
  napi_number,
  napi_string,
  napi_symbol,
  napi_object,
  napi_function,
  napi_external,
  napi_bigint,
} napi_valuetype;

typedef enum {
  napi_int8_array,
  napi_uint8_array,
  napi_uint8_clamped_array,
  napi_int16_array,
  napi_uint16_array,
  napi_int32_array,
  napi_uint32_array,
  napi_float32_array,
  napi_float64_array,
  napi_bigint64_array,
  napi_biguint64_array,
} napi_typedarray_type;

typedef enum {
  napi_ok,
  napi_invalid_arg,
  napi_object_expected,
  napi_string_expected,
  napi_name_expected,
  napi_function_expected,
  napi_number_expected,
  napi_boolean_expected,
  napi_array_expected,
  napi_generic_failure,
  napi_pending_exception,
  napi_cancelled,
  napi_escape_called_twice,
  napi_handle_scope_mismatch,
  napi_callback_scope_mismatch,
  napi_queue_full,
  napi_closing,
  napi_bigint_expected,
  napi_date_expected,
  napi_arraybuffer_expected,
  napi_detachable_arraybuffer_expected,
} napi_status;
// Note: when adding a new enum value to `napi_status`, please also update
// `const int last_status` in `napi_get_last_error_info()' definition,
// in file js_native_api_v8.cc. Please also update the definition of
// `napi_status` in doc/api/n-api.md to reflect the newly added value(s).

#if NAPI_VERSION >= 4
typedef enum {
  napi_tsfn_release,
  napi_tsfn_abort
} napi_threadsafe_function_release_mode;

typedef enum {
  napi_tsfn_nonblocking,
  napi_tsfn_blocking
} napi_threadsafe_function_call_mode;
#endif  // NAPI_VERSION >= 4

typedef napi_value (*napi_callback)(napi_env env,
                                    napi_callback_info info);
typedef void (*napi_finalize)(napi_env env,
                              void* finalize_data,
                              void* finalize_hint);
typedef void (*napi_async_execute_callback)(napi_env env,
                                            void* data);
typedef void (*napi_async_complete_callback)(napi_env env,
                                             napi_status status,
                                             void* data);

#if NAPI_VERSION >= 4
typedef void (*napi_threadsafe_function_call_js)(napi_env env,
                                                 napi_value js_callback,
                                                 void* context,
                                                 void* data);
#endif  // NAPI_VERSION >= 4

typedef struct {
  // One of utf8name or name should be NULL.
  const char* utf8name;
  napi_value name;

  napi_callback method;
  napi_callback getter;
  napi_callback setter;
  napi_value value;

  napi_property_attributes attributes;
  void* data;
} napi_property_descriptor;

typedef struct {
  const char* error_message;
  void* engine_reserved;
  uint32_t engine_error_code;
  napi_status error_code;
} napi_extended_error_info;

typedef struct {
  uint32_t major;
  uint32_t minor;
  uint32_t patch;
  const char* release;
} napi_node_version;

#if NAPI_VERSION >= 6
typedef enum {
  napi_key_include_prototypes,
  napi_key_own_only
} napi_key_collection_mode;

typedef enum {
  napi_key_all_properties = 0,
  napi_key_writable = 1,
  napi_key_enumerable = 1 << 1,
  napi_key_configurable = 1 << 2,
  napi_key_skip_strings = 1 << 3,
  napi_key_skip_symbols = 1 << 4
} napi_key_filter;

typedef enum {
  napi_key_keep_numbers,
  napi_key_numbers_to_strings
} napi_key_conversion;
#endif  // NAPI_VERSION >= 6

#endif  // SRC_NODE_API_TYPES_H_


/*
file https://github.com/nodejs/node/blob/v10.22.1/src/node_api.h
*/
#ifndef SRC_NODE_API_H_
#define SRC_NODE_API_H_

#include <stddef.h>
#include <stdbool.h>

#ifndef NAPI_VERSION
#ifdef NAPI_EXPERIMENTAL
// Use INT_MAX, this should only be consumed by the pre-processor anyway.
#define NAPI_VERSION 2147483647
#else
// The baseline version for N-API
#define NAPI_VERSION 6
#endif
#endif

// hack-nodejs - inline header
// #include "node_api_types.h"

struct uv_loop_s;  // Forward declaration.

#ifdef _WIN32
  #ifdef BUILDING_NODE_EXTENSION
    #ifdef EXTERNAL_NAPI
      // Building external N-API, or native module against external N-API
      #define NAPI_EXTERN /* nothing */
    #else
      // Building native module against node with built-in N-API
      #define NAPI_EXTERN __declspec(dllimport)
    #endif
  #else
    // Building node with built-in N-API
    #define NAPI_EXTERN __declspec(dllexport)
  #endif
#else
  #define NAPI_EXTERN /* nothing */
#endif

#ifdef _WIN32
# define NAPI_MODULE_EXPORT __declspec(dllexport)
#else
# define NAPI_MODULE_EXPORT __attribute__((visibility("default")))
#endif

#ifdef __GNUC__
#define NAPI_NO_RETURN __attribute__((noreturn))
#else
#define NAPI_NO_RETURN
#endif


typedef napi_value (*napi_addon_register_func)(napi_env env,
                                               napi_value exports);

typedef struct {
  int nm_version;
  unsigned int nm_flags;
  const char* nm_filename;
  napi_addon_register_func nm_register_func;
  const char* nm_modname;
  void* nm_priv;
  void* reserved[4];
} napi_module;

#define NAPI_MODULE_VERSION  1

#if defined(_MSC_VER)
#pragma section(".CRT$XCU", read)
#define NAPI_C_CTOR(fn)                                                     \
  static void __cdecl fn(void);                                             \
  __declspec(dllexport, allocate(".CRT$XCU")) void(__cdecl * fn##_)(void) = \
      fn;                                                                   \
  static void __cdecl fn(void)
#else
#define NAPI_C_CTOR(fn)                              \
  static void fn(void) __attribute__((constructor)); \
  static void fn(void)
#endif

#ifdef __cplusplus
#define EXTERN_C_START extern "C" {
#define EXTERN_C_END }
#else
#define EXTERN_C_START
#define EXTERN_C_END
#endif

#define NAPI_MODULE_X(modname, regfunc, priv, flags)                  \
  EXTERN_C_START                                                      \
    static napi_module _module =                                      \
    {                                                                 \
      NAPI_MODULE_VERSION,                                            \
      flags,                                                          \
      __FILE__,                                                       \
      regfunc,                                                        \
      #modname,                                                       \
      priv,                                                           \
      {0},                                                            \
    };                                                                \
    NAPI_C_CTOR(_register_ ## modname) {                              \
      napi_module_register(&_module);                                 \
    }                                                                 \
  EXTERN_C_END

#define NAPI_MODULE(modname, regfunc)                                 \
  NAPI_MODULE_X(modname, regfunc, NULL, 0)  // NOLINT (readability/null_usage)

#define NAPI_MODULE_INITIALIZER_BASE napi_register_module_v

#define NAPI_MODULE_INITIALIZER_X(base, version)                      \
    NAPI_MODULE_INITIALIZER_X_HELPER(base, version)
#define NAPI_MODULE_INITIALIZER_X_HELPER(base, version) base##version

#define NAPI_MODULE_INITIALIZER                                       \
  NAPI_MODULE_INITIALIZER_X(NAPI_MODULE_INITIALIZER_BASE,             \
      NAPI_MODULE_VERSION)

#define NAPI_MODULE_INIT()                                            \
  EXTERN_C_START                                                      \
  NAPI_MODULE_EXPORT napi_value                                       \
  NAPI_MODULE_INITIALIZER(napi_env env, napi_value exports);          \
  EXTERN_C_END                                                        \
  NAPI_MODULE(NODE_GYP_MODULE_NAME, NAPI_MODULE_INITIALIZER)          \
  napi_value NAPI_MODULE_INITIALIZER(napi_env env,                    \
                                     napi_value exports)

#define NAPI_AUTO_LENGTH SIZE_MAX

EXTERN_C_START

NAPI_EXTERN void napi_module_register(napi_module* mod);

NAPI_EXTERN napi_status
napi_get_last_error_info(napi_env env,
                         const napi_extended_error_info** result);

NAPI_EXTERN NAPI_NO_RETURN void napi_fatal_error(const char* location,
                                                 size_t location_len,
                                                 const char* message,
                                                 size_t message_len);

// Getters for defined singletons
NAPI_EXTERN napi_status napi_get_undefined(napi_env env, napi_value* result);
NAPI_EXTERN napi_status napi_get_null(napi_env env, napi_value* result);
NAPI_EXTERN napi_status napi_get_global(napi_env env, napi_value* result);
NAPI_EXTERN napi_status napi_get_boolean(napi_env env,
                                         bool value,
                                         napi_value* result);

// Methods to create Primitive types/Objects
NAPI_EXTERN napi_status napi_create_object(napi_env env, napi_value* result);
NAPI_EXTERN napi_status napi_create_array(napi_env env, napi_value* result);
NAPI_EXTERN napi_status napi_create_array_with_length(napi_env env,
                                                      size_t length,
                                                      napi_value* result);
NAPI_EXTERN napi_status napi_create_double(napi_env env,
                                           double value,
                                           napi_value* result);
NAPI_EXTERN napi_status napi_create_int32(napi_env env,
                                          int32_t value,
                                          napi_value* result);
NAPI_EXTERN napi_status napi_create_uint32(napi_env env,
                                           uint32_t value,
                                           napi_value* result);
NAPI_EXTERN napi_status napi_create_int64(napi_env env,
                                          int64_t value,
                                          napi_value* result);
NAPI_EXTERN napi_status napi_create_string_latin1(napi_env env,
                                                  const char* str,
                                                  size_t length,
                                                  napi_value* result);
NAPI_EXTERN napi_status napi_create_string_utf8(napi_env env,
                                                const char* str,
                                                size_t length,
                                                napi_value* result);
NAPI_EXTERN napi_status napi_create_string_utf16(napi_env env,
                                                 const char16_t* str,
                                                 size_t length,
                                                 napi_value* result);
NAPI_EXTERN napi_status napi_create_symbol(napi_env env,
                                           napi_value description,
                                           napi_value* result);
NAPI_EXTERN napi_status napi_create_function(napi_env env,
                                             const char* utf8name,
                                             size_t length,
                                             napi_callback cb,
                                             void* data,
                                             napi_value* result);
NAPI_EXTERN napi_status napi_create_error(napi_env env,
                                          napi_value code,
                                          napi_value msg,
                                          napi_value* result);
NAPI_EXTERN napi_status napi_create_type_error(napi_env env,
                                               napi_value code,
                                               napi_value msg,
                                               napi_value* result);
NAPI_EXTERN napi_status napi_create_range_error(napi_env env,
                                                napi_value code,
                                                napi_value msg,
                                                napi_value* result);

// Methods to get the native napi_value from Primitive type
NAPI_EXTERN napi_status napi_typeof(napi_env env,
                                    napi_value value,
                                    napi_valuetype* result);
NAPI_EXTERN napi_status napi_get_value_double(napi_env env,
                                              napi_value value,
                                              double* result);
NAPI_EXTERN napi_status napi_get_value_int32(napi_env env,
                                             napi_value value,
                                             int32_t* result);
NAPI_EXTERN napi_status napi_get_value_uint32(napi_env env,
                                              napi_value value,
                                              uint32_t* result);
NAPI_EXTERN napi_status napi_get_value_int64(napi_env env,
                                             napi_value value,
                                             int64_t* result);
NAPI_EXTERN napi_status napi_get_value_bool(napi_env env,
                                            napi_value value,
                                            bool* result);

// Copies LATIN-1 encoded bytes from a string into a buffer.
NAPI_EXTERN napi_status napi_get_value_string_latin1(napi_env env,
                                                     napi_value value,
                                                     char* buf,
                                                     size_t bufsize,
                                                     size_t* result);

// Copies UTF-8 encoded bytes from a string into a buffer.
NAPI_EXTERN napi_status napi_get_value_string_utf8(napi_env env,
                                                   napi_value value,
                                                   char* buf,
                                                   size_t bufsize,
                                                   size_t* result);

// Copies UTF-16 encoded bytes from a string into a buffer.
NAPI_EXTERN napi_status napi_get_value_string_utf16(napi_env env,
                                                    napi_value value,
                                                    char16_t* buf,
                                                    size_t bufsize,
                                                    size_t* result);

// Methods to coerce values
// These APIs may execute user scripts
NAPI_EXTERN napi_status napi_coerce_to_bool(napi_env env,
                                            napi_value value,
                                            napi_value* result);
NAPI_EXTERN napi_status napi_coerce_to_number(napi_env env,
                                              napi_value value,
                                              napi_value* result);
NAPI_EXTERN napi_status napi_coerce_to_object(napi_env env,
                                              napi_value value,
                                              napi_value* result);
NAPI_EXTERN napi_status napi_coerce_to_string(napi_env env,
                                              napi_value value,
                                              napi_value* result);

// Methods to work with Objects
NAPI_EXTERN napi_status napi_get_prototype(napi_env env,
                                           napi_value object,
                                           napi_value* result);
NAPI_EXTERN napi_status napi_get_property_names(napi_env env,
                                                napi_value object,
                                                napi_value* result);
NAPI_EXTERN napi_status napi_set_property(napi_env env,
                                          napi_value object,
                                          napi_value key,
                                          napi_value value);
NAPI_EXTERN napi_status napi_has_property(napi_env env,
                                          napi_value object,
                                          napi_value key,
                                          bool* result);
NAPI_EXTERN napi_status napi_get_property(napi_env env,
                                          napi_value object,
                                          napi_value key,
                                          napi_value* result);
NAPI_EXTERN napi_status napi_delete_property(napi_env env,
                                             napi_value object,
                                             napi_value key,
                                             bool* result);
NAPI_EXTERN napi_status napi_has_own_property(napi_env env,
                                              napi_value object,
                                              napi_value key,
                                              bool* result);
NAPI_EXTERN napi_status napi_set_named_property(napi_env env,
                                          napi_value object,
                                          const char* utf8name,
                                          napi_value value);
NAPI_EXTERN napi_status napi_has_named_property(napi_env env,
                                          napi_value object,
                                          const char* utf8name,
                                          bool* result);
NAPI_EXTERN napi_status napi_get_named_property(napi_env env,
                                          napi_value object,
                                          const char* utf8name,
                                          napi_value* result);
NAPI_EXTERN napi_status napi_set_element(napi_env env,
                                         napi_value object,
                                         uint32_t index,
                                         napi_value value);
NAPI_EXTERN napi_status napi_has_element(napi_env env,
                                         napi_value object,
                                         uint32_t index,
                                         bool* result);
NAPI_EXTERN napi_status napi_get_element(napi_env env,
                                         napi_value object,
                                         uint32_t index,
                                         napi_value* result);
NAPI_EXTERN napi_status napi_delete_element(napi_env env,
                                            napi_value object,
                                            uint32_t index,
                                            bool* result);
NAPI_EXTERN napi_status
napi_define_properties(napi_env env,
                       napi_value object,
                       size_t property_count,
                       const napi_property_descriptor* properties);

// Methods to work with Arrays
NAPI_EXTERN napi_status napi_is_array(napi_env env,
                                      napi_value value,
                                      bool* result);
NAPI_EXTERN napi_status napi_get_array_length(napi_env env,
                                              napi_value value,
                                              uint32_t* result);

// Methods to compare values
NAPI_EXTERN napi_status napi_strict_equals(napi_env env,
                                           napi_value lhs,
                                           napi_value rhs,
                                           bool* result);

// Methods to work with Functions
NAPI_EXTERN napi_status napi_call_function(napi_env env,
                                           napi_value recv,
                                           napi_value func,
                                           size_t argc,
                                           const napi_value* argv,
                                           napi_value* result);
NAPI_EXTERN napi_status napi_new_instance(napi_env env,
                                          napi_value constructor,
                                          size_t argc,
                                          const napi_value* argv,
                                          napi_value* result);
NAPI_EXTERN napi_status napi_instanceof(napi_env env,
                                        napi_value object,
                                        napi_value constructor,
                                        bool* result);

// Methods to work with napi_callbacks

// Gets all callback info in a single call. (Ugly, but faster.)
NAPI_EXTERN napi_status napi_get_cb_info(
    napi_env env,               // [in] NAPI environment handle
    napi_callback_info cbinfo,  // [in] Opaque callback-info handle
    size_t* argc,      // [in-out] Specifies the size of the provided argv array
                       // and receives the actual count of args.
    napi_value* argv,  // [out] Array of values
    napi_value* this_arg,  // [out] Receives the JS 'this' arg for the call
    void** data);          // [out] Receives the data pointer for the callback.

NAPI_EXTERN napi_status napi_get_new_target(napi_env env,
                                            napi_callback_info cbinfo,
                                            napi_value* result);
NAPI_EXTERN napi_status
napi_define_class(napi_env env,
                  const char* utf8name,
                  size_t length,
                  napi_callback constructor,
                  void* data,
                  size_t property_count,
                  const napi_property_descriptor* properties,
                  napi_value* result);

// Methods to work with external data objects
NAPI_EXTERN napi_status napi_wrap(napi_env env,
                                  napi_value js_object,
                                  void* native_object,
                                  napi_finalize finalize_cb,
                                  void* finalize_hint,
                                  napi_ref* result);
NAPI_EXTERN napi_status napi_unwrap(napi_env env,
                                    napi_value js_object,
                                    void** result);
NAPI_EXTERN napi_status napi_remove_wrap(napi_env env,
                                         napi_value js_object,
                                         void** result);
NAPI_EXTERN napi_status napi_create_external(napi_env env,
                                             void* data,
                                             napi_finalize finalize_cb,
                                             void* finalize_hint,
                                             napi_value* result);
NAPI_EXTERN napi_status napi_get_value_external(napi_env env,
                                                napi_value value,
                                                void** result);

// Methods to control object lifespan

// Set initial_refcount to 0 for a weak reference, >0 for a strong reference.
NAPI_EXTERN napi_status napi_create_reference(napi_env env,
                                              napi_value value,
                                              uint32_t initial_refcount,
                                              napi_ref* result);

// Deletes a reference. The referenced value is released, and may
// be GC'd unless there are other references to it.
NAPI_EXTERN napi_status napi_delete_reference(napi_env env, napi_ref ref);

// Increments the reference count, optionally returning the resulting count.
// After this call the  reference will be a strong reference because its
// refcount is >0, and the referenced object is effectively "pinned".
// Calling this when the refcount is 0 and the object is unavailable
// results in an error.
NAPI_EXTERN napi_status napi_reference_ref(napi_env env,
                                           napi_ref ref,
                                           uint32_t* result);

// Decrements the reference count, optionally returning the resulting count.
// If the result is 0 the reference is now weak and the object may be GC'd
// at any time if there are no other references. Calling this when the
// refcount is already 0 results in an error.
NAPI_EXTERN napi_status napi_reference_unref(napi_env env,
                                             napi_ref ref,
                                             uint32_t* result);

// Attempts to get a referenced value. If the reference is weak,
// the value might no longer be available, in that case the call
// is still successful but the result is NULL.
NAPI_EXTERN napi_status napi_get_reference_value(napi_env env,
                                                 napi_ref ref,
                                                 napi_value* result);

NAPI_EXTERN napi_status napi_open_handle_scope(napi_env env,
                                               napi_handle_scope* result);
NAPI_EXTERN napi_status napi_close_handle_scope(napi_env env,
                                                napi_handle_scope scope);
NAPI_EXTERN napi_status
napi_open_escapable_handle_scope(napi_env env,
                                 napi_escapable_handle_scope* result);
NAPI_EXTERN napi_status
napi_close_escapable_handle_scope(napi_env env,
                                  napi_escapable_handle_scope scope);

NAPI_EXTERN napi_status napi_escape_handle(napi_env env,
                                           napi_escapable_handle_scope scope,
                                           napi_value escapee,
                                           napi_value* result);

// Methods to support error handling
NAPI_EXTERN napi_status napi_throw(napi_env env, napi_value error);
NAPI_EXTERN napi_status napi_throw_error(napi_env env,
                                         const char* code,
                                         const char* msg);
NAPI_EXTERN napi_status napi_throw_type_error(napi_env env,
                                         const char* code,
                                         const char* msg);
NAPI_EXTERN napi_status napi_throw_range_error(napi_env env,
                                         const char* code,
                                         const char* msg);
NAPI_EXTERN napi_status napi_is_error(napi_env env,
                                      napi_value value,
                                      bool* result);

// Methods to support catching exceptions
NAPI_EXTERN napi_status napi_is_exception_pending(napi_env env, bool* result);
NAPI_EXTERN napi_status napi_get_and_clear_last_exception(napi_env env,
                                                          napi_value* result);

// Methods to provide node::Buffer functionality with napi types
NAPI_EXTERN napi_status napi_create_buffer(napi_env env,
                                           size_t length,
                                           void** data,
                                           napi_value* result);
NAPI_EXTERN napi_status napi_create_external_buffer(napi_env env,
                                                    size_t length,
                                                    void* data,
                                                    napi_finalize finalize_cb,
                                                    void* finalize_hint,
                                                    napi_value* result);
NAPI_EXTERN napi_status napi_create_buffer_copy(napi_env env,
                                                size_t length,
                                                const void* data,
                                                void** result_data,
                                                napi_value* result);
NAPI_EXTERN napi_status napi_is_buffer(napi_env env,
                                       napi_value value,
                                       bool* result);
NAPI_EXTERN napi_status napi_get_buffer_info(napi_env env,
                                             napi_value value,
                                             void** data,
                                             size_t* length);

// Methods to work with array buffers and typed arrays
NAPI_EXTERN napi_status napi_is_arraybuffer(napi_env env,
                                            napi_value value,
                                            bool* result);
NAPI_EXTERN napi_status napi_create_arraybuffer(napi_env env,
                                                size_t byte_length,
                                                void** data,
                                                napi_value* result);
NAPI_EXTERN napi_status
napi_create_external_arraybuffer(napi_env env,
                                 void* external_data,
                                 size_t byte_length,
                                 napi_finalize finalize_cb,
                                 void* finalize_hint,
                                 napi_value* result);
NAPI_EXTERN napi_status napi_get_arraybuffer_info(napi_env env,
                                                  napi_value arraybuffer,
                                                  void** data,
                                                  size_t* byte_length);
NAPI_EXTERN napi_status napi_is_typedarray(napi_env env,
                                           napi_value value,
                                           bool* result);
NAPI_EXTERN napi_status napi_create_typedarray(napi_env env,
                                               napi_typedarray_type type,
                                               size_t length,
                                               napi_value arraybuffer,
                                               size_t byte_offset,
                                               napi_value* result);
NAPI_EXTERN napi_status napi_get_typedarray_info(napi_env env,
                                                 napi_value typedarray,
                                                 napi_typedarray_type* type,
                                                 size_t* length,
                                                 void** data,
                                                 napi_value* arraybuffer,
                                                 size_t* byte_offset);

NAPI_EXTERN napi_status napi_create_dataview(napi_env env,
                                             size_t length,
                                             napi_value arraybuffer,
                                             size_t byte_offset,
                                             napi_value* result);
NAPI_EXTERN napi_status napi_is_dataview(napi_env env,
                                         napi_value value,
                                         bool* result);
NAPI_EXTERN napi_status napi_get_dataview_info(napi_env env,
                                               napi_value dataview,
                                               size_t* bytelength,
                                               void** data,
                                               napi_value* arraybuffer,
                                               size_t* byte_offset);

// Methods to manage simple async operations
NAPI_EXTERN
napi_status napi_create_async_work(napi_env env,
                                   napi_value async_resource,
                                   napi_value async_resource_name,
                                   napi_async_execute_callback execute,
                                   napi_async_complete_callback complete,
                                   void* data,
                                   napi_async_work* result);
NAPI_EXTERN napi_status napi_delete_async_work(napi_env env,
                                               napi_async_work work);
NAPI_EXTERN napi_status napi_queue_async_work(napi_env env,
                                              napi_async_work work);
NAPI_EXTERN napi_status napi_cancel_async_work(napi_env env,
                                               napi_async_work work);

// Methods for custom handling of async operations
NAPI_EXTERN napi_status napi_async_init(napi_env env,
                                        napi_value async_resource,
                                        napi_value async_resource_name,
                                        napi_async_context* result);

NAPI_EXTERN napi_status napi_async_destroy(napi_env env,
                                           napi_async_context async_context);

NAPI_EXTERN napi_status napi_make_callback(napi_env env,
                                           napi_async_context async_context,
                                           napi_value recv,
                                           napi_value func,
                                           size_t argc,
                                           const napi_value* argv,
                                           napi_value* result);

// version management
NAPI_EXTERN napi_status napi_get_version(napi_env env, uint32_t* result);

NAPI_EXTERN
napi_status napi_get_node_version(napi_env env,
                                  const napi_node_version** version);

// Promises
NAPI_EXTERN napi_status napi_create_promise(napi_env env,
                                            napi_deferred* deferred,
                                            napi_value* promise);
NAPI_EXTERN napi_status napi_resolve_deferred(napi_env env,
                                              napi_deferred deferred,
                                              napi_value resolution);
NAPI_EXTERN napi_status napi_reject_deferred(napi_env env,
                                             napi_deferred deferred,
                                             napi_value rejection);
NAPI_EXTERN napi_status napi_is_promise(napi_env env,
                                        napi_value promise,
                                        bool* is_promise);

// Memory management
NAPI_EXTERN napi_status napi_adjust_external_memory(napi_env env,
                                                    int64_t change_in_bytes,
                                                    int64_t* adjusted_value);

// Running a script
NAPI_EXTERN napi_status napi_run_script(napi_env env,
                                        napi_value script,
                                        napi_value* result);

#if NAPI_VERSION >= 2

// Return the current libuv event loop for a given environment
NAPI_EXTERN napi_status napi_get_uv_event_loop(napi_env env,
                                               struct uv_loop_s** loop);

#endif  // NAPI_VERSION >= 2

#if NAPI_VERSION >= 3

NAPI_EXTERN napi_status napi_open_callback_scope(napi_env env,
                                                 napi_value resource_object,
                                                 napi_async_context context,
                                                 napi_callback_scope* result);

NAPI_EXTERN napi_status napi_close_callback_scope(napi_env env,
                                                  napi_callback_scope scope);

NAPI_EXTERN napi_status napi_fatal_exception(napi_env env, napi_value err);

NAPI_EXTERN napi_status napi_add_env_cleanup_hook(napi_env env,
                                                  void (*fun)(void* arg),
                                                  void* arg);

NAPI_EXTERN napi_status napi_remove_env_cleanup_hook(napi_env env,
                                                     void (*fun)(void* arg),
                                                     void* arg);

#endif  // NAPI_VERSION >= 3

#if NAPI_VERSION >= 4

// Calling into JS from other threads
NAPI_EXTERN napi_status
napi_create_threadsafe_function(napi_env env,
                                napi_value func,
                                napi_value async_resource,
                                napi_value async_resource_name,
                                size_t max_queue_size,
                                size_t initial_thread_count,
                                void* thread_finalize_data,
                                napi_finalize thread_finalize_cb,
                                void* context,
                                napi_threadsafe_function_call_js call_js_cb,
                                napi_threadsafe_function* result);

NAPI_EXTERN napi_status
napi_get_threadsafe_function_context(napi_threadsafe_function func,
                                     void** result);

NAPI_EXTERN napi_status
napi_call_threadsafe_function(napi_threadsafe_function func,
                              void* data,
                              napi_threadsafe_function_call_mode is_blocking);

NAPI_EXTERN napi_status
napi_acquire_threadsafe_function(napi_threadsafe_function func);

NAPI_EXTERN napi_status
napi_release_threadsafe_function(napi_threadsafe_function func,
                                 napi_threadsafe_function_release_mode mode);

NAPI_EXTERN napi_status
napi_unref_threadsafe_function(napi_env env, napi_threadsafe_function func);

NAPI_EXTERN napi_status
napi_ref_threadsafe_function(napi_env env, napi_threadsafe_function func);

#endif  // NAPI_VERSION >= 4

#if NAPI_VERSION >= 5

// Dates
NAPI_EXTERN napi_status napi_create_date(napi_env env,
                                         double time,
                                         napi_value* result);

NAPI_EXTERN napi_status napi_is_date(napi_env env,
                                     napi_value value,
                                     bool* is_date);

NAPI_EXTERN napi_status napi_get_date_value(napi_env env,
                                            napi_value value,
                                            double* result);

// Add finalizer for pointer
NAPI_EXTERN napi_status napi_add_finalizer(napi_env env,
                                           napi_value js_object,
                                           void* native_object,
                                           napi_finalize finalize_cb,
                                           void* finalize_hint,
                                           napi_ref* result);

#endif  // NAPI_VERSION >= 5

#if NAPI_VERSION >= 6

// BigInt
NAPI_EXTERN napi_status napi_create_bigint_int64(napi_env env,
                                                 int64_t value,
                                                 napi_value* result);
NAPI_EXTERN napi_status napi_create_bigint_uint64(napi_env env,
                                                  uint64_t value,
                                                  napi_value* result);
NAPI_EXTERN napi_status napi_create_bigint_words(napi_env env,
                                                 int sign_bit,
                                                 size_t word_count,
                                                 const uint64_t* words,
                                                 napi_value* result);
NAPI_EXTERN napi_status napi_get_value_bigint_int64(napi_env env,
                                                    napi_value value,
                                                    int64_t* result,
                                                    bool* lossless);
NAPI_EXTERN napi_status napi_get_value_bigint_uint64(napi_env env,
                                                     napi_value value,
                                                     uint64_t* result,
                                                     bool* lossless);
NAPI_EXTERN napi_status napi_get_value_bigint_words(napi_env env,
                                                    napi_value value,
                                                    int* sign_bit,
                                                    size_t* word_count,
                                                    uint64_t* words);

// Instance data
NAPI_EXTERN napi_status napi_set_instance_data(napi_env env,
                                               void* data,
                                               napi_finalize finalize_cb,
                                               void* finalize_hint);

NAPI_EXTERN napi_status napi_get_instance_data(napi_env env,
                                               void** data);

// Object
NAPI_EXTERN napi_status
napi_get_all_property_names(napi_env env,
                            napi_value object,
                            napi_key_collection_mode key_mode,
                            napi_key_filter key_filter,
                            napi_key_conversion key_conversion,
                            napi_value* result);

#endif  // NAPI_VERSION >= 6

#ifdef NAPI_EXPERIMENTAL
// ArrayBuffer detaching
NAPI_EXTERN napi_status
napi_detach_arraybuffer(napi_env env,
                        napi_value arraybuffer);

NAPI_EXTERN napi_status
napi_is_detached_arraybuffer(napi_env env,
                             napi_value value,
                             bool* result);
#endif  // NAPI_EXPERIMENTAL

EXTERN_C_END

#endif  // SRC_NODE_API_H_
#endif // SRC_SQLMATH_NODEJS_C2


/*
file none
*/
/*jslint-enable*/


// *INDENT-ON*
#endif                          // SRC_SQLMATH_BASE_H3
