/** @file */
#include "blocks.h"

uint16_t recno_get_headsize(BTYPE type)
{
   switch(type)
   {
      case RN_FILE:
         return sizeof(HEAD_FILE);
      case RN_TABLE:
         return sizeof(HEAD_TABLE);
      case RN_CHAIN:
         return sizeof(HEAD_CHAIN);
      default:
         return sizeof(HEAD_BLOCK);
   }
}
