/** @file */

#include <fcntl.h>    // for fcntl()  (setting locks)
#include <unistd.h>   // for getpid()
#include <errno.h>

#include <stdio.h>    // for fileno()

#include "recno.h"

RECNO_ERROR locks_set_lock_direct(FILE *file, off_t offset, size_t length, int *errnum)
{
   RECNO_ERROR rval;

   *errnum = 0;
   
   int fd = fileno(file);
   struct flock fl;
   memset(&fl, 0, sizeof(fl));
   fl.l_type = F_WRLCK;
   fl.l_whence = SEEK_SET;
   fl.l_start = offset;
   fl.l_len = length;
   fl.l_pid = getpid();

   if (fcntl(fd, F_SETLK, &fl) == -1)
   {
      if (errno == EAGAIN || errno == EACCES)
         rval = RECNO_LOCK_FAILED;
      else
      {
         rval = RECNO_SYSTEM_ERROR;
         *errnum = errno;
      }
   }
   else
      rval = RECNO_SUCCESS;

   return rval;
}

RECNO_ERROR locks_release_lock_direct(FILE *file, off_t offset, size_t length, int *errnum)
{
   RECNO_ERROR rval;
   
   *errnum = 0;

   int fd = fileno(file);
   struct flock fl;
   memset(&fl, 0, sizeof(fl));
   fl.l_type = F_UNLCK;
   fl.l_whence = SEEK_SET;
   fl.l_start = offset;
   fl.l_len = length;
   fl.l_pid = getpid();

   if (fcntl(fd, F_SETLK, &fl) == -1)
   {
      rval = RECNO_SYSTEM_ERROR;
      *errnum = errno;
   }
   else
      rval = RECNO_SUCCESS;

   return rval;
}

RECNO_ERROR locks_set_lock(DB_HANDLE *handle, BLOC *bloc)
{
   return locks_set_lock(handle, bloc.offset, bloc.size, &handle->errnum);
}

RECNO_ERROR locks_release_lock(DB_HANDLE *handle, BLOC *bloc)
{
   return locks_release_lock(handle, bloc.offset, bloc.size, &handle->errnum);
}
