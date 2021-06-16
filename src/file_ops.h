/** @file */

#ifndef HEAD_HANDLE_H
#define HEAD_HANDLE_H

RECNO_ERROR fiops_extend_file_direct(FILE *file, uint32_t length, BLOC *bloc, int *errnum);

RECNO_ERROR fiops_write_data_direct(FILE *file,
                                    const void *data,
                                    off_t offset,
                                    size_t length,
                                    int *errnum);

RECNO_ERROR fiops_read_data_direct(FILE *file,
                                   void *data,
                                   off_t offset,
                                   size_t length,
                                   int *errnum);

RECNO_ERROR fiops_read_head_handle_direct(FILE *file,
                                          off_t offset,
                                          BTYPE type,
                                          HEAD_HANDLE *head_handle,
                                          int *errnum);

RECNO_ERROR fiops_handle_initialize_head(DB_HANDLE *handle,
                                         off_t offset,
                                         BTYPE type,
                                         HEAD_HANDLE *head_handle);

RECNO_ERROR fiops_write_head(DB_HANDLE *db_handle, const HEAD_HANDLE *head_handle);
RECNO_ERROR fiops_update_head(DB_HANDLE *db_handle, HEAD_HANDLE *head_handle);

RECNO_ERROR fiops_lock_head_and_read(DB_HANDLE *db_handle, HEAD_HANDLE *head_handle);
RECNO_ERROR fiops_write_head_and_unlock(DB_HANDLE *db_handle, HEAD_HANDLE *head_handle);
RECNO_ERROR fiops_unlock_head(DB_HANDLE *db_handle, HEAD_HANDLE *head_handle);


#endif
