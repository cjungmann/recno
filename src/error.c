#include "recno.h"

#include <errno.h>
#include <stdio.h>
#include <fcntl.h>


const char *recno_error_strings[] = {
   "No error",
   "Generic Failure",
   "System Error",
   "Invalid Recno Database File",
   "Bad Parameter",
   "Missing FHEAD Parameter",
   "Attempted to Open Already Opened Database",
   "File Not Open",
   "Extinct Record",
   "Attempt to Orphan Block",
   "Reached End of Block Chain",
   "Lock Failed",
   "Unlock Failed",
   "Lock Read Failed",
   "Unlock Write Failed",
   "Incomplete Read",
   "Incomplete Write",
   "Invalid Block Size",
   "Invalid Block Location",
   "Invalid File Head"
};

EXPORT const char *recno_strerror(RECNO_ERROR err, RHANDLE *handle)
{
   if (err == RECNO_SYSTEM_ERROR && handle)
      return strerror(handle->errnum);
   else
      return recno_error_strings[err];
}
