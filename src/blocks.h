/** @file */

#ifndef BLOCKS_H
#define BLOCKS_H

#include <stdint.h>    // for uint32_t, etc
#include <stdio.h>
#include <fcntl.h>

typedef enum {
   RN_UNDEFINED = 0,
   RN_GENERIC,
   RN_DATA,
   RN_CHAIN,
   RN_TABLE,
   RN_FILE
} BTYPE;

typedef struct rn_block_location {
   off_t offset;
   off_t size;
} BLOC;

#define MAGIC_STRING "RCNO"

/**
 * Every block begins with this structure, which provides the minimum information
 * needed to anonymously manage the block.  Headers that may immediately follow this
 * may provide additional information for more specific roles.
 */
typedef struct rn_info_block {
   uint16_t type;        /**< BTYPE value signalling how to use the block. */
   uint16_t size_head;   /**< size of entire header;  alternatively used as the number of bytes */
                         /** to the block's payload. */
   uint32_t size_block;  /**< number of bytes in this block, including the head. */
   BLOC     next_block;  /**< where to get the next block */
} INFO_BLOCK;

/**
 * Head that may follow the BLOCK structure for the head of a chain.  This information
 * allows a chain to lengthen without walking the entire chain.
 *
 * Each block contains a reference that includes the offset and size of the next
 * block in a chain.  If the last block can be extended because it's the last block
 * in the file, the second-to-last block will need to update the *size* value of
 * its *next_block* member.  Having the offset to both blocks makes this possible
 * without having to either:
 * 1. visit each block to find the second-to-last block, or
 * 2. have each block contain a back reference in order to reverse from the
 *    last block.
 */
typedef struct rn_info_chain {
   off_t block_penultimate;  /**< offset to the second-to-last block */
   off_t block_last;         /**< offset to the last block */
} INFO_CHAIN;

/**
 * Head for block chain that incorporates a set of fixed-length records.
 *
 * Even variable-length records will have a fixed-length record root from which
 * the location of the variable-length data resides.
 */
typedef struct rn_info_table {
   uint32_t  rec_size;    /**< Size of fixed-length record, or 0 for variable-length record. */
   uint32_t  last_recno;  /**< last assigned, auto-incremented, record number. */
} INFO_TABLE;


/**
 * Head for file.  Follows other xxxx_INFO types
 */
typedef struct rn_info_file {
   char     magic[4];     /**< Will always be "RCNO" (but use MAGIC_STRING) */
   uint32_t chunk_size;   /**< Miniumum multiple of allocated file extensions. */
} INFO_FILE;

/**
 * HEAD types
 */

typedef struct rn_info_block HEAD_BLOCK;

typedef struct rn_head_chain {
   INFO_BLOCK info_block;
   INFO_CHAIN info_chain;
} HEAD_CHAIN;

typedef struct rn_head_table {
   INFO_BLOCK info_block;
   INFO_CHAIN info_chain;
   INFO_TABLE info_table;
} HEAD_TABLE;

typedef struct rn_head_file {
   INFO_BLOCK info_block;
   INFO_CHAIN info_chain;
   INFO_TABLE info_table;
   INFO_FILE  info_file;
} HEAD_FILE;

// Typedeffed to DB_HANDLE in recno.h
struct recno_handle;

uint16_t blocks_get_headsize(BTYPE type);
uint32_t blocks_convert_length(struct recno_handle *dbh, size_t min_length);



#endif
