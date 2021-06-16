/** @file */

#include "recno.h"
#include "extra.h"
#include "blocks.h"
#include "file_ops.h"
#include "chains.h"

RECNO_ERROR chains_chain_walk(DB_HANDLE *handle,
                              HEAD_HANDLE *head_chain,
                              chain_walker walker,
                              void *closure)
{
   RECNO_ERROR rval = RECNO_FAIL;

   return rval;
}


EXPORT RECNO_ERROR chains_chain_augment(DB_HANDLE *handle,
                                        HEAD_HANDLE *head_chain,
                                        size_t min_length,
                                        HEAD_HANDLE *new_block)
{
   RECNO_ERROR rval = RECNO_FAIL;

   if ((rval = fiops_lock_head_and_read(handle, head_chain)))
      goto abandon_function;

   uint32_t required_block_size = blocks_convert_length(handle, min_length);
   BLOC bloc;

   if ((rval=fiops_extend_file_direct(handle->file, required_block_size, &bloc, &handle->errnum)))
      goto abandon_lock;

   off_t off_last = hh_block_last(head_chain);

   printf("The offset of the last block is %lu.\n", off_last);

  abandon_lock:
   fiops_unlock_head(handle, head_chain);

  abandon_function:
   return rval;
}
