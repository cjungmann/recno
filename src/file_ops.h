#ifndef HEAD_HANDLE_H
#define HEAD_HANDLE_H

#include "recno.h"

RECNO_ERROR recno_extend_file_direct(FILE *file, uint32_t length, int *errnum);
RECNO_ERROR recno_write_data_direct(FILE *file,
                                    off_t offset,
                                    const void *data,
                                    size_t length,
                                    int *errnum);

RECNO_ERROR recno_extend_file(RHANDLE *handle, uint32_t length);


RECNO_ERROR recno_read_head_handle(FILE *file,
                                   off_t offset,
                                   BTYPE type,
                                   HEAD_HANDLE *handle,
                                   int *errnum);

RECNO_ERROR recno_get_read_head_handle(RHANDLE *rhandle, off_t offset, BTYPE type, HEAD_HANDLE *handle);
   


#endif
