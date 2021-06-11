#include "recno.h"
#include "file_ops.h"
#include "extra.h"

extern bool confirm_head_file(const HEAD_FILE *head_file);
extern bool confirm_head_handle(const HEAD_HANDLE *head_handle);

/**
 * Create a new recno database file.
 *
 * @param path     path and filename of file to be created
 * @param params   creation parameters
 * @param handle   [out] handle in which the newly-opened file can be accessed.
 */
EXPORT RECNO_ERROR recno_create(const char *path, CREATE_PARAMS *params, DB_HANDLE *handle)
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

   // Initialize temporary DB_HANDLE for writing as well as eventual return parameter
   uint16_t head_size = recno_get_headsize(RN_FILE);
   DB_HANDLE temp_handle =
      { file, 0, 0, 
        // Initialize DB_HANDLE
        {
           // Initialize DB_HANDLE::bloc
           { 0, head_size },
           // Initialize DB_HANDLE::head_file:
           { {RN_FILE, head_size, chunk_size },
             { 0, 0 },
             { params->rec_size, 0 },
             { MAGIC_STRING, chunk_size }
           }
        }
      };

   // Use error-checking function to write the file head
   if ((rval = recno_write_head(&temp_handle, &temp_handle.head_handle)))
      goto abandon_file;

   // Extension and head writing successful, prepare return parameters
   memcpy(handle, &temp_handle, sizeof(DB_HANDLE));
   goto exit_function;

  abandon_file:
   fclose(file);

  abandon_function:
  exit_function:
   return rval;
}

/**
 * Open an existing recno database file.

 * @param path     path and filename of recno database to be opened
 * @param flags    flags dictating options for opened file
 * @param handle   [out] handle in which the newly-opened file can be accessed.
 */
EXPORT RECNO_ERROR recno_open(const char *path, RECNO_FLAGS flags, DB_HANDLE *handle)
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

   // Read file head and confirm file type
   HEAD_HANDLE head_handle;
   if ((rval = recno_read_head_handle_direct(file, 0, RN_FILE, &head_handle, &handle->errnum)))
      goto abandon_file;

   if (!confirm_head_handle(&head_handle))
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

/**
 * Close an open database handle.
 *
 * @param handle        open handle to a recno database
 */
EXPORT RECNO_ERROR recno_close(DB_HANDLE *handle)
{
   if (handle->file)
   {
      fclose(handle->file);
      handle->file = NULL;
   }

   return RECNO_SUCCESS;
}

/**
 * Get a handle to a table chain, from which the table records can be accessed.
 *
 * @param handle        open handle to a recno database
 * @param table_name    name of the table to open. Use NULL for root table.
 * @param table_handle  [out] handle object to use for the table.
 */
EXPORT RECNO_ERROR recno_open_table(DB_HANDLE *handle, const char *table_name, HEAD_HANDLE *table_handle)
{
   RECNO_ERROR rval = RECNO_FAIL;
   
   if (!handle || !table_handle)
   {
      rval = RECNO_BAD_PARAMETER;
      goto abandon_function;
   }

   memset(table_handle, 0, sizeof(HEAD_HANDLE));

   if (table_name == NULL)
   {
      table_handle->bloc.offset = 0;
      table_handle->bloc.size = recno_get_headsize(RN_TABLE);
   }


  abandon_function:
   return rval;
}
