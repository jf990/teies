/****************************************************************************
   <Doc>
   <Function> lstrdup
   <Author>   John L. Foster
   <Date>     June 20, 1991
   <Purpose>  Duplicate a string, allocating new memory
   <Comments> Duplicate string must be deallocated at some time by calling
               MemFree().
   </Doc>
****************************************************************************/
#include <klTools.h>


HANDLE   FAR PASCAL lstrdup ( LPSTR original )
{

   LPSTR  lpresult = NULL ;
   HANDLE hresult  = NULLHANDLE ;
   int    length   = 0 ;


   length  = lstrlen ( original ) ;
   hresult = MemHandle ( EXH_MEMFLAGS , length + 1 ) ;
   if ( hresult != NULLHANDLE )
   {

      lpresult = MemLock ( EXH_MEMFLAGS , hresult ) ;
      if ( lpresult != NULL )
      {

         while ( * original != NULLCHAR )
         {

            * lpresult = * original ;
            original ++ ;
            lpresult ++ ;

         }
         MemUnlock ( EXH_MEMFLAGS , hresult ) ;

      }
   }
   return ( hresult ) ;

}
