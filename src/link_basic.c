#include "recno.h"

#include <stdio.h>
#include <readargs.h>

const char *file_create = NULL;
const char *file_open = NULL;

raAction actions[] = {
   { 'c', "create", "Create new database", &ra_string_agent, &file_create },
   { 'o', "open", "Open existing database", &ra_string_agent, &file_open }
};
   
void test_file_create(const char *filepath)
{
   CREATE_PARAMS cparams = { 0, 4096 };

   RHANDLE handle;
   RECNO_ERROR err = recno_create(filepath, &cparams, &handle);
   if (err)
      printf("Failed to create %s (%s)\n", filepath, recno_strerror(err, &handle));
   else
   {
      printf("Success!\n");
      recno_close(&handle);
   }
}

void test_file_open(const char *filepath)
{
   RHANDLE handle;
   RECNO_ERROR err = recno_open(filepath, 0, &handle);
   if (err)
      printf("Failed to open %s (%s)\n", filepath, recno_strerror(err, &handle));
   else
   {
      printf("Success!\n");
      recno_close(&handle);
   }
      
}

int main(int argc, const char **argv)
{
   ra_set_scene(argv, argc, actions, ACTS_COUNT(actions));

   if (ra_process_arguments())
   {
      if (file_create)
         test_file_create(file_create);
      else if (file_open)
         test_file_open(file_open);
      else
         printf("Nothing to do.\n");
   }



   return 0;
}
