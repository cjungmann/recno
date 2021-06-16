/** @file */

#ifndef LOCKS_H
#define LOCKS_H

RECNO_ERROR locks_set_lock_direct(FILE *file, off_t offset, size_t length, int *errnum);
RECNO_ERROR locks_release_lock_direct(FILE *file, off_t offset, size_t length, int *errnum);

RECNO_ERROR locks_set_lock(DB_HANDLE *handle, BLOC *bloc);
RECNO_ERROR locks_release_lock(DB_HANDLE *handle, BLOC *bloc);

#endif

