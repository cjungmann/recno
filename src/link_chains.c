#include "recno.h"
#include "chains.h"

#include <stdio.h>

void test_augment(void)
{
   CREATE_PARAMS cparams = { 0, 4096 };
   const char *filepath = "augment.db";

   DB_HANDLE handle;
   RECNO_ERROR err = recno_create(filepath, &cparams, &handle);
   if (err)
      printf("Failed to create %s (%s)\n", filepath, recno_strerror(err, &handle));
   else
   {
      printf("Success opening %s.\n", filepath);

      HEAD_HANDLE new_block_head;
      if ((err = chains_chain_augment(&handle, &handle.head_handle, &new_block_head)))
         printf("Failed to extend the file block chain (%s).\n", recno_strerror(err, &handle));
      else
         printf("Success extending the file block chain.\n");

      recno_close(&handle);
   }
}

int main(int argc, const char **argv)
{
   test_augment();
   return 0;
}
