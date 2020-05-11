/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    var_query.c                                            |
  |                                                                   |
  |  <AUTHOR>  Samir Chopra                                           |
  |                                                                   |
  |  <DATE>    September 6, 1989                                      |
  |                                                                   |
  |  <LEVEL>   VAR                                                    |
  |                                                                   |
  |  <GLOBAL>  * none *                                               |
  |                                                                   |
  |  <PARAM>   name   string   INPUT   Name to query                  |
  |            what_to_query   INPUT   Self explanatory               |
  |                                                                   |
  |  <PURPOSE> Return a sint indicating either data length in bytes   |
  |            or data type depending upon what was asked for.        |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |                                                                   |
  |  <CALLS>   var_lfnd(),                                            |
  |                                                                   |
  |  <ERRORS>  None                                                   |
  |                                                                   |
  |  <LIMITS>  None known.                                            |
  |                                                                   |
  |  <MODS>                                                           |
  |                                                                   |
  |  </DOC>                                                           |
  |                                                                   |
  +-------------------------------------------------------------------+*/


#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "vars.h"
#include "hash.h"

sint   var_query( name, what_to_query )

string           name;
sint     what_to_query;
{

   CELL    *hash_ptr         = NIL;
   sint    rc                =   0;



   hash_ptr = var_lfnd( name );
   if( hash_ptr != NIL )
   {
      switch ( what_to_query )
      {
         case  VARIABLE_QUERY_LENGTH  :

            switch(hash_ptr->type)
            {
               case OC_STR:
               case OC_GFD:
               case OC_VAR:
               {
                  rc = s_len((string)(hash_ptr->contents));
                  break;
               }
               case OC_BYTE:
               {
                  rc = 1;
                  break;
               }
               case  OC_INT:
               case  OC_REAL:
               {
                  rc = 4;
                  break;
               }
               case  OC_D_T:
               {
                  rc = sizeof(datetime);
                  break;
               }
               case  OC_PRIV:
               {
                  rc = sizeof(PRIVS);
                  break;
               }
               case  OC_OID:
               case  OC_INHR:
               {
                  rc = sizeof(OID);
                  break;
               }
               case  OC_HWRD:
               case  OC_ISV:
               {
                  rc = 2;
                  break;
               }
            }
            break;

         case VARIABLE_QUERY_TYPE  :

            rc = hash_ptr->type ;
            break;

         case VARIABLE_QUERY_EXISTANCE :

            if ( var_lfnd( name ) != NIL )
            {

               rc = TRUE ;

             }
             else
             {

                 rc = FALSE ;

             }
             break;

         default :

            rc = 0;

     }
  }
   return(rc);
}
