/** @file */

#include "recno.h"
#include "blocks.h"
#include "file_ops.h"

uint16_t recno_get_headsize(BTYPE type)
{
   switch(type)
   {
      case RN_FILE:
         return sizeof(HEAD_FILE);
      case RN_TABLE:
         return sizeof(HEAD_TABLE);
      case RN_CHAIN:
         return sizeof(HEAD_CHAIN);
      default:
         return sizeof(HEAD_BLOCK);
   }
}

/**
 * 
RECNO_ERROR recno_read_block_head(DB_HANDLE *handle, HEAD_HANDLE *header)
{
   RECNO_ERROR rval = RECNO_FAIL;
   if (!handle || !handle->file || !header || !header->bloc.offset)
   {
      rval = RECNO_BAD_VALUE;
      goto abandon_function;
   }

   rval = recno_read_data_direct(handle->file,
                                 &header->head_file,
                                 header->bloc.offset,
                                 header->bloc.size,
                                 &handle->errnum);

   if (rval)
      goto abandon_function;


  abandon_function:
  exit_function:
   return rval;
}
 */
