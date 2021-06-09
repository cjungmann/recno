/** @file */

#include "file_ops.h"
#include "stdio.h"

RECNO_ERROR recno_extend_file_direct(FILE *file, uint32_t length, int *errnum)
{
   RECNO_ERROR rval = RECNO_SUCCESS;

   *errnum = 0;

   if (fseek(file, length-1, SEEK_END)
       || ! (fwrite("\0", 1, 1, file)==1 || !(errno = ferror(file))))
   {
      rval = RECNO_SYSTEM_ERROR;
      *errnum = errno;
      goto abandon_function;
   }

   // *errnum = fflush(file);

  abandon_function:
   return rval;
}

RECNO_ERROR recno_write_data_direct(FILE *file,
                                    off_t offset,
                                    const void *data,
                                    size_t length,
                                    int *errnum)
{
   *errnum = 0;

   if (fseek(file, offset, SEEK_SET)
       || ! (fwrite(data, length, 1, file)==1 || !(errno = ferror(file))))
   {
      *errnum = errno;
      return RECNO_SYSTEM_ERROR;
   }
   else
      return RECNO_SUCCESS;
}
 

RECNO_ERROR recno_read_head_handle(FILE *file,
                                   off_t offset,
                                   BTYPE type,
                                   HEAD_HANDLE *handle,
                                   int *errnum)
{
   RECNO_ERROR rval = RECNO_FAIL;
   *errnum = 0;

   size_t head_size = recno_get_headsize(type);

   // Discard previous contents
   memset(handle, 0, sizeof(HEAD_HANDLE));

   if (fseek(file, offset, SEEK_SET)
       || ! (fread(handle, head_size, 1, file)==1 || !(errno = ferror(file))))
   {
      rval = RECNO_SYSTEM_ERROR;
      *errnum = errno;
      goto abandon_function;
   }

   handle->bloc.offset = offset;
   handle->bloc.size = head_size;

   rval = RECNO_SUCCESS;

  abandon_function:
   return rval;
}


RECNO_ERROR recno_get_head_handle(RHANDLE *rhandle, off_t offset, BTYPE type, HEAD_HANDLE *handle)
{
   return recno_read_head_handle(rhandle->file, offset, type, handle, &rhandle->errnum);
}

