#include "recno.h"
#include "file_ops.h"


EXPORT RECNO_ERROR recno_create(const char *path, CREATE_PARAMS *params, RHANDLE *handle)
{
   RECNO_ERROR rval = RECNO_FAIL;

   if (!path || !params || !handle)
   {
      rval = RECNO_BAD_PARAMETER;
      goto abandon_function;
   }

   FILE *file = fopen(path, "w+b");
   if (!file)
   {
      rval = RECNO_SYSTEM_ERROR;
      handle->errnum = errno;
      goto abandon_function;
   }

   uint32_t chunk_size = params->chunk_size;
   if (!chunk_size)
      chunk_size = 4096;

   if ((rval = recno_extend_file_direct(file, chunk_size, &handle->errnum)))
      goto abandon_file;

   HEAD_FILE head_file = { {RN_FILE, recno_get_headsize(RN_FILE), chunk_size },
                           { 0, 0 },
                           { params->rec_size, 0 },
                           { "RCNO", chunk_size } };

   if ((rval = recno_write_data_direct(file, 0, &head_file, sizeof(HEAD_FILE), &handle->errnum)))
      goto abandon_file;

   memcpy(&handle->head_handle.head_file, &head_file, sizeof(HEAD_FILE));
   handle->file = file;
   rval = RECNO_SUCCESS;
   goto exit_function;

  abandon_file:
   fclose(file);

  abandon_function:
  exit_function:
   return rval;
}

EXPORT RECNO_ERROR recno_open(const char *path, RECNO_FLAGS flags, RHANDLE *handle)
{
   RECNO_ERROR rval = RECNO_FAIL;

   const char *mode = flags & RECNO_READONLY ? "rb" : "r+b";

   FILE *file = fopen(path, mode);
   if (!file)
   {
      rval = RECNO_SYSTEM_ERROR;
      handle->errnum = errno;
      goto abandon_function;
   }

   HEAD_FILE *head_file = &handle->head_handle.head_file;

   if ((rval = recno_read_head_handle(file, 0, RN_FILE, &handle->head_handle, &handle->errnum)))
      goto abandon_file;

   if (strncmp(head_file->info_file.magic, "RCNO", 4))
   {
      rval = RECNO_INVALID_RECNO_FILE;
      goto abandon_file;
   }

   handle->file = file;
   rval = RECNO_SUCCESS;
   goto exit_function;

  abandon_file:
   fclose(file);

  abandon_function:
  exit_function:
   return rval;
}


EXPORT RECNO_ERROR recno_close(RHANDLE *handle)
{
   if (handle->file)
   {
      fclose(handle->file);
      handle->file = NULL;
   }

   return RECNO_SUCCESS;
}
