/** @file */

#ifndef EXTRA_H
#define EXTRA_H

#include <stdbool.h>
#include "blocks.h"   // for MAGIC_STRING

inline HEAD_HANDLE* head_handle_from_db_handle(DB_HANDLE *h)   { return &h->head_handle; }
inline FILE* file_from_db_handle(DB_HANDLE *h)          { return h->file; }

inline const HEAD_HANDLE* const_head_handle_from_db_handle(const DB_HANDLE *h)
{
   return &h->head_handle;
}

inline bool confirm_head_file(const HEAD_FILE *head_file)
{
   return head_file
      && head_file->info_block.type == RN_FILE
      && head_file->info_block.size_head == recno_get_headsize(RN_FILE)
      && !strncmp(head_file->info_file.magic, MAGIC_STRING, 4);
}

inline bool confirm_head_handle(const HEAD_HANDLE *head_handle)
{
   return confirm_head_file(&head_handle->head_file);
}


#endif
