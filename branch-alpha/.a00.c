// copyright

// LINT_C_FILE

/*
cmd.exe /C .a00.bat
*/

#define SIZEOF_BLOB_MAX         1000000000

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./sqlmath_base.h"

/**
 * @brief Compresses a string and writes the gzipped output to a file.
 * @param src_buf The string to compress.
 * @param output_filename The name of the file to write to.
 * @return int Returns 0 on success, or a non-zero zlib error code on failure.
 */
int compress_string_to_file(
    const uint8_t * src_buf,
    const char *output_filename
) {
    // declare var
    z_stream strm = { 0 };
    int errcode = Z_OK;
    uint8_t *gzip_buf = NULL;
    // init argv


    const int src_len = (int) strlen((char *) src_buf);
    //
    FILE *file = NULL;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    errcode =
        deflateInit2(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 15 + 16, 8,
        Z_DEFAULT_STRATEGY);
    if (errcode != Z_OK) {
        fprintf(stderr, "Error initializing deflate stream: %d\n", errcode);
        return errcode;
    }
    // uLong compressBound(
    //     uLong sourceLen
    // );
    int gzip_len = (int) compressBound(src_len) + 18;
    gzip_buf = (uint8_t *) malloc(gzip_len);
    if (!gzip_buf) {
        fprintf(stderr, "Memory allocation failed.\n");
        errcode = Z_MEM_ERROR;
        goto cleanup;
    }


    strm.avail_in = (uLong) src_len;
    strm.next_in = (uint8_t *) src_buf;
    strm.avail_out = (uLong) gzip_len;
    strm.next_out = gzip_buf;
    // int deflate(
    //     z_streamp strm,
    //     int flush
    // );
    errcode = deflate(&strm, Z_FINISH);




    if (errcode != Z_STREAM_END) {
        fprintf(stderr, "Compression failed with error code: %d\n", errcode);
        goto cleanup;
    }



    gzip_len = gzip_len - strm.avail_out;
    errcode = fopen_s(&file, output_filename, "wb");
    //!! file = fopen(output_filename, "wb");
    //!! if (!file) {
    //!! fprintf(stderr, "Failed to open file %s\n", output_filename);
    //!! errcode = Z_ERRNO;
    //!! goto cleanup;
    //!! }
    size_t bytes_written = fwrite(gzip_buf, 1, gzip_len, file);
    if (bytes_written != gzip_len) {
        fprintf(stderr, "Error writing to file. Wrote %zu of %lu bytes.\n",
            bytes_written, gzip_len);
        errcode = Z_ERRNO;
        goto cleanup;
    }
    printf("Successfully compressed string to file: %s\n", output_filename);
    errcode = Z_OK;
  cleanup:
    if (file) {
        fclose(file);
    }
    if (gzip_buf) {
        free(gzip_buf);
    }
    deflateEnd(&strm);
    return errcode;
}





/**
 * @brief Decompresses a gzipped buffer and returns the decompressed data.
 * @param gzip_buf The gzip_buf buffer.
 * @param gzip_len The size of the gzip_buf.
 * @param output_size A pointer to a size_t to store the size of the decompressed data.
 * @return uint8_t* A newly allocated buffer containing the decompressed data, or NULL on failure.
 */
uint8_t *decompress_buffer(
    uint8_t * gzip_buf,
    size_t gzip_len,
    size_t * output_size
) {
    z_stream strm = { 0 };
    uint8_t *src_buf = NULL;
    int src_len = 4096;
    int errcode = Z_OK;


    // int inflateInit2(
    //     z_streamp strm,
    //     int windowBits
    // );
    strm.avail_in = (uLong) gzip_len;
    strm.next_in = gzip_buf;


    errcode = inflateInit2(&strm, 15 + 16);
    if (errcode != Z_OK) {
        fprintf(stderr, "Error initializing inflate stream: %d\n", errcode);
        return NULL;
    }


    // Allocate the initial output buffer
    src_buf = (uint8_t *) malloc(src_len);
    if (!src_buf) {
        fprintf(stderr, "Memory allocation failed.\n");
        errcode = Z_MEM_ERROR;
        goto cleanup;
    }
    do {
        // Set up the output buffer for the next decompression step.
        // `total_out` is the total bytes decompressed so far.
        strm.avail_out = (uLong) src_len - strm.total_out;
        strm.next_out = src_buf + strm.total_out;
        errcode = inflate(&strm, Z_NO_FLUSH);
        if (errcode < 0 && errcode != Z_STREAM_END) {
            fprintf(stderr, "Decompression failed with error code: %d\n",
                errcode);
            goto cleanup;
        }
        // If the output buffer is full, reallocate to double its capacity.
        if (strm.avail_out == 0 && errcode != Z_STREAM_END) {
            src_len = src_len * 2;
            if (SIZEOF_BLOB_MAX < src_len) {    // Check for overflow
                fprintf(stderr, "src_len overflow.\n");
                errcode = Z_MEM_ERROR;
                goto cleanup;
            }
            src_buf = (uint8_t *) realloc(src_buf, src_len);
            if (!src_buf) {
                fprintf(stderr, "Memory reallocation failed.\n");
                errcode = Z_MEM_ERROR;
                goto cleanup;
            }
        }
    } while (errcode != Z_STREAM_END);
    // `total_out` now holds the exact size of the decompressed data.
    *output_size = strm.total_out;
    errcode = Z_OK;
  cleanup:
    inflateEnd(&strm);
    if (errcode != Z_OK) {
        if (src_buf) {
            free(src_buf);
        }
        return NULL;
    }
    //!! // Final realloc to trim the buffer to the exact size.
    //!! // This prevents memory waste from over-allocation.
    //!! uint8_t *tmp = (uint8_t *) realloc(src_buf, strm.total_out);
    //!! if (tmp) {
    //!! src_buf = tmp;
    //!! } else {
    //!! // realloc failed. We will return the over-allocated buffer
    //!! // and a warning, as it is better than a NULL result.
    //!! fprintf(stderr,
    //!! "Warning: Final buffer trim failed,"
    //!! " returning over-allocated buffer.\n");
    //!! }
    return src_buf;
}




/**
 * @brief The main function, which serves as the program's entry point.
 * It handles command-line arguments to choose between compression and decompression.
 */
int main(
    int argc,
    char *argv[]
) {
    if (argc < 3) {
        fprintf(stderr, "Usage:\n");
        fprintf(stderr, "  %s -c \"<string>\" <output_file.gz>\n", argv[0]);
        fprintf(stderr, "  %s -d <input_file.gz>\n", argv[0]);
        return 1;
    }
    const char *operation = argv[1];
    if (strcmp(operation, "-c") == 0) {
        if (argc != 4) {
            fprintf(stderr,
                "Usage for compression: %s -c \"<string>\" <output_file.gz>\n",
                argv[0]);
            return 1;
        }
        const uint8_t *data_to_compress = (uint8_t *) argv[2];
        const char *output_file = argv[3];
        int result = compress_string_to_file(data_to_compress, output_file);
        return (result == Z_OK) ? 0 : 1;
    } else if (strcmp(operation, "-d") == 0) {
        if (argc != 3) {
            fprintf(stderr,
                "Usage for decompression: %s -d <input_file.gz>\n", argv[0]);
            return 1;
        }
        const char *input_file = argv[2];
        FILE *file = NULL;
        uint8_t *gzip_buf = NULL;
        long file_size = 0;     // NOLINT
        int result = Z_ERRNO;
        uint8_t *src_buf = NULL;
        size_t decompressed_size = 0;

        fopen_s(&file, input_file, "rb");
        //!! errcode = fopen_s(&file, output_filename, "wb");
        //!! file = fopen(input_file, "rb");
        //!! if (!file) {
        //!! fprintf(stderr, "Failed to open file %s\n", input_file);
        //!! return 1;
        //!! }
        fseek(file, 0, SEEK_END);
        file_size = ftell(file);
        rewind(file);
        gzip_buf = (uint8_t *) malloc(file_size);
        if (!gzip_buf) {
            fprintf(stderr, "Memory allocation failed.\n");
            result = Z_MEM_ERROR;
            goto decompress_cleanup;
        }
        size_t bytes_read = fread(gzip_buf, 1, file_size, file);
        if (bytes_read != file_size) {
            fprintf(stderr, "Error reading file.\n");
            result = Z_ERRNO;
            goto decompress_cleanup;
        } else {
            src_buf =
                decompress_buffer(gzip_buf, file_size, &decompressed_size);
            if (src_buf) {
                // For demonstration, print the src_buf and its size
                printf("src_buf (%zu bytes):\n", decompressed_size);
                fwrite(src_buf, 1, decompressed_size, stdout);
                printf("\n");
                result = Z_OK;
            } else {
                fprintf(stderr, "Decompression failed.\n");
                result = Z_ERRNO;
            }
        }
      decompress_cleanup:
        if (file) {
            fclose(file);
        }
        if (gzip_buf) {
            free(gzip_buf);
        }
        if (src_buf) {
            free(src_buf);
        }
        return (result == Z_OK) ? 0 : 1;
    } else {
        fprintf(stderr,
            "Invalid operation. Use -c for compress or -d for decompress.\n");
        return 1;
    }
}
