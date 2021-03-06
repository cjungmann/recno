/** @file */

#ifndef EXTRA_H
#define EXTRA_H

#include <stdbool.h>
#include <assert.h>
#include "blocks.h"   // for MAGIC_STRING

#define INLINE static inline

INLINE HEAD_HANDLE* head_handle_from_db_handle(DB_HANDLE *h)   { return &h->head_handle; }
INLINE FILE* file_from_db_handle(DB_HANDLE *h)    { return h->file; }

INLINE HEAD_FILE *hh_header(HEAD_HANDLE *hh)      { return &hh->header; }
INLINE BTYPE hh_type(HEAD_HANDLE *hh)             { return hh->header.info_block.type; }

INLINE bool hh_is_chain_block(HEAD_HANDLE *hh)    { return hh_type(hh) >= RN_CHAIN; }
INLINE bool hh_is_table_block(HEAD_HANDLE *hh)    { return hh_type(hh) >= RN_TABLE; }
INLINE bool hh_is_file_block(HEAD_HANDLE *hh)     { return hh_type(hh) >= RN_FILE; }

INLINE INFO_BLOCK *hh_info_block(HEAD_HANDLE *hh) { return &hh->header.info_block; }
INLINE INFO_CHAIN *hh_info_chain(HEAD_HANDLE *hh) { return &hh->header.info_chain; }
INLINE INFO_TABLE *hh_info_table(HEAD_HANDLE *hh) { return &hh->header.info_table; }
INLINE INFO_FILE  *hh_info_file(HEAD_HANDLE *hh)  { return &hh->header.info_file; }

INLINE bool hh_file_good_magic(HEAD_HANDLE *hh)
{
   return ! strncmp(hh->header.info_file.magic, MAGIC_STRING, 4);
}

INLINE uint16_t hh_size_head(HEAD_HANDLE *hh) { return hh_info_block(hh)->size_head; }
INLINE uint32_t hh_size_block(HEAD_HANDLE *hh) { return hh_info_block(hh)->size_block; }
INLINE BLOC *hh_next_block(HEAD_HANDLE *hh) { return &hh_info_block(hh)->next_block; }

INLINE off_t hh_block_penultimate(HEAD_HANDLE *hh)
{
   assert(hh_is_chain_block(hh));
   return hh_info_chain(hh)->block_penultimate;
}

INLINE off_t hh_block_last(HEAD_HANDLE *hh)
{
   assert(hh_is_chain_block(hh));
   return hh_info_chain(hh)->block_last;
}

INLINE uint32_t hh_rec_size(HEAD_HANDLE *hh)
{
   assert(hh_is_table_block(hh));
   return hh_info_table(hh)->rec_size;
}

INLINE uint32_t hh_last_recno(HEAD_HANDLE *hh)
{
   assert(hh_is_table_block(hh));
   return hh_info_table(hh)->last_recno;
}

INLINE uint32_t hh_chunk_size(HEAD_HANDLE *hh)
{
   assert(hh_is_file_block(hh));
   return hh_info_file(hh)->chunk_size;
}

#endif
