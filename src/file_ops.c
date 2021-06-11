/** @file */

#include <stdio.h>

#include "recno.h"
#include "blocks.h"
#include "file_ops.h"
#include "extra.h"

/**
 * Extend a file by a give number of bytes, called by informed function.
 *
 * No safeguards about appropriate sizes or anything, this function just
 * extends the file, setting application-standard error messaging.
 * 
 * @param file    an open file stream
 * @param length  number of bytes to extend the file
 * @param errnum  [out] set to system *errno* in the case of a system error.
 *
 * @return appropriate RECNO_ERROR value to pass on to ultimate calling function.
 */
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

/**
 * Write data to file, returning system error according to library convention.
 *
 * @param file     pointer to an open file stream
 * @param data     pointer to buffer containing data to write
 * @param offset   offset in file where data should be written
 * @param length   length of data to be written
 * @param errnum   [out] pointer to variable into which errno can be copied
 *                 if the system calls result in failure.
 *
 * @return appropriate RECNO_ERROR value to pass on to ultimate calling function.
 */
RECNO_ERROR recno_write_data_direct(FILE *file,
                                    const void *data,
                                    off_t offset,
                                    size_t length,
                                    int *errnum)
{
   *errnum = 0;

   if (fseek(file, offset, SEEK_SET)
       || ! (fwrite(data, length, 1, file)==1 || !(*errnum = ferror(file))))
      return RECNO_SYSTEM_ERROR;
   else
      return RECNO_SUCCESS;
}

/**
 * Write data to file, returning system error according to library convention.
 *
 * @param file     pointer to an open file stream
 * @param data     pointer to buffer into which data will be written 
 * @param offset   offset in file from which the data should be read
 * @param length   length of data to be read
 * @param errnum   [out] pointer to variable into which errno can be copied
 *                 if the system calls result in failure.
 *
 * @return appropriate RECNO_ERROR value to pass on to ultimate calling function.
 */
RECNO_ERROR recno_read_data_direct(FILE *file,
                                   void *data,
                                   off_t offset,
                                   size_t length,
                                   int *errnum)
{
   *errnum = 0;
   if (fseek(file, offset, SEEK_SET)
       || ! (fread(data, length, 1, file)==1 || !(*errnum = ferror(file))))
      return RECNO_SYSTEM_ERROR;
   else
      return RECNO_SUCCESS;
}


/**
 * Reads a header without a DB_HANDLE, directly called when opening a file.
 *
 * @param file        pointer to an open file stream
 * @param offset      offset in file from which the data will be read
 * @param type        type of head to be read (determines how long the head is)
 * @param head_handle [out] pointer to variable into which the data will be written
 * @param errnum      [out] pointer to variable into which errno can be copied
 *                    if the system calls result in failure.
 *
 * @return appropriate RECNO_ERROR value to pass on to ultimate calling function.
 */
RECNO_ERROR recno_read_head_handle_direct(FILE *file,
                                          off_t offset,
                                          BTYPE type,
                                          HEAD_HANDLE *head_handle,
                                          int *errnum)
{
   RECNO_ERROR rval;
   
   uint16_t head_size = recno_get_headsize(type);

   HEAD_HANDLE temp_handle = { { offset, head_size } };

   if (!(rval = recno_read_data_direct(file,
                                       &temp_handle.head_file,
                                       offset,
                                       head_size,
                                       errnum)))
   {
      memcpy(head_handle, &temp_handle, sizeof(HEAD_HANDLE));
   }

   return rval;
}

/**
 * Initialize a HEAD_HANDLE from specific file location.
 *
 * @param handle      pointer to open handle
 * @param offset      file location where head begins
 * @param type        block type used to determine header size
 * @param head_handle [out] pointer to variable into which the data will be written
 *
 * @return appropriate RECNO_ERROR value to pass on to ultimate calling function.
 */
RECNO_ERROR recno_handle_initialize_head(DB_HANDLE *handle,
                                         off_t offset,
                                         BTYPE type,
                                         HEAD_HANDLE *head_handle)
{
   return recno_read_head_handle_direct(handle->file, offset, type, head_handle, &handle->errnum);
}

/**
 * Convenient function for updating changed head information
 *
 * @param db_handle   pointer to an open handle
 * @param head_handle pointer to variable from which the data will be written to file
 *
 * @return appropriate RECNO_ERROR value to pass on to ultimate calling function.
 */
RECNO_ERROR recno_write_head(DB_HANDLE *db_handle, const HEAD_HANDLE *head_handle)
{
   return recno_write_data_direct(db_handle->file,
                                  &head_handle->head_file,
                                  head_handle->bloc.offset,
                                  head_handle->bloc.size,
                                  &db_handle->errnum);
}
/**
 * Convenient function for update a head variable from current file contents
 *
 * @param db_handle   pointer to an open handle
 * @param head_handle pointer to variable into which the file data will be copied
 *
 * @return appropriate RECNO_ERROR value to pass on to ultimate calling function.
 */
RECNO_ERROR recno_update_head(DB_HANDLE *db_handle, HEAD_HANDLE *head_handle)
{
   return recno_read_data_direct(db_handle->file,
                                 &head_handle->head_file,
                                 head_handle->bloc.offset,
                                 head_handle->bloc.size,
                                 &db_handle->errnum);
}


