/** @file */

#ifndef RECNO_H
#define RECNO_H

#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "blocks.h"

#define EXPORT __attribute__((visibility("default")))

typedef enum {
   RECNO_CREATE = 1,
   RECNO_READONLY
} RECNO_FLAGS;

typedef struct recno_create_params {
   uint32_t rec_size;
   uint32_t chunk_size;
} CREATE_PARAMS;

typedef enum {
   RECNO_SUCCESS = 0,
   RECNO_FAIL,
   RECNO_SYSTEM_ERROR,
   RECNO_INVALID_RECNO_FILE,
   RECNO_BAD_PARAMETER,
   RECNO_MISSING_FHEAD,
   RECNO_FILE_ALREADY_OPEN,
   RECNO_FILE_NOT_OPEN,
   RECNO_EXTINCT_RECORD,
   RECNO_ATTEMPT_TO_ORPHAN_BLOCK,
   RECNO_REACHED_END_OF_BLOCK_CHAIN,
   RECNO_LOCK_FAILED,
   RECNO_UNLOCK_FAILED,
   RECNO_LOCK_READ_FAILED,
   RECNO_UNLOCK_WRITE_FAILED,
   RECNO_INCOMPLETE_READ,
   RECNO_INCOMPLETE_WRITE,
   RECNO_INVALID_BLOCK_SIZE,
   RECNO_INVALID_BLOCK_LOCATION,
   RECNO_INVALID_HEAD_FILE,
   RECNO_ERROR_LIMIT
} RECNO_ERROR;


/**
 * Combination head data and the file location where it was found
 */
typedef struct rn_head_handle {
   HEAD_FILE head_file;   /**< largest head struct to ensure it fits */
   BLOC      bloc;        /**< file location of the block head */
} HEAD_HANDLE;

typedef struct recno_handle {
   FILE        *file;
   uint32_t    padding;
   int         errnum;
   HEAD_HANDLE head_handle;
} RHANDLE;


const char *recno_strerror(RECNO_ERROR err, RHANDLE *handle);  // in "error.c"

RECNO_ERROR recno_create(const char *path, CREATE_PARAMS *params, RHANDLE *handle);
RECNO_ERROR recno_open(const char *path, RECNO_FLAGS flags, RHANDLE *handle);
RECNO_ERROR recno_close(RHANDLE *handle);


#endif
