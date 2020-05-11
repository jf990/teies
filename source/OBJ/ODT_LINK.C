/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    odt_link .c                                            |
  |                                                                   |
  |  :AUTHOR.  John L. Foster                                         |
  |                                                                   |
  |  :DATE.    June 01, 1988                                          |
  |                                                                   |
  |  :LEVEL.   OBJ                                                    |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.   odt_start   ODT *      INPUT  Pointer to the beginning |
  |                                          of an ODT to link.       |
  |                                                                   |
  |  :PURPOSE. Go through an ODT and link its inheritance OCDs to     |
  |            their proper ODTs.  This assumes that all inherited    |
  |            ODTs are already loaded.  The linkage works as follows:|
  |                                                                   |
  |              1) Locate an OCD of type OC_INHR (inheritance);      |
  |                                                                   |
  |              2) In the privs field will be the half word object   |
  |                 type of the object to link.  Call ODT_PTR() to    |
  |                 get a pointer to this ODT.                        |
  |                                                                   |
  |              3) Overwrite the priv field with the resulting ODT   |
  |                 pointer.                                          |
  |                                                                   |
  |              4) Contine to step (1) until there are no more       |
  |                 links to establish.                               |
  |                                                                   |
  |  :OUTPUT.  sint:  return 0 if there were no ODTs to link or if all|
  |                   links were sucessful.  Otherwise the 5 digit    |
  |                   error code is returned.                         |
  |                                                                   |
  |  :CALLS.   odt_ptr()                                              |
  |                                                                   |
  |  :ERRORS.  None known.                                            |
  |                                                                   |
  |  :LIMITS.  None known.                                            |
  |                                                                   |
  |  :END.                                                            |
  |                                                                   |
  +-------------------------------------------------------------------+*/


#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"



sint   odt_link( odt_start )

ODT             *odt_start;

{

sint      rc              =    0,
          j               =    1;
ODT      *pos             =  NIL,
         *link_reference  =  NIL,
        **link_pointer    =  NIL;
hword    *object_type     =  NIL;






while( odt_start->offset <= j )
{

   if( (pos = odt_start + j)->type == OC_INHR )
   {


      object_type = (hword *) &(pos->privs.one);
      link_reference = odt_ptr( *object_type );

      if( ODT_POINTER_OK( link_reference ) )
      {

         link_pointer = (ODT **) &(pos->privs.three);
        *link_pointer = link_reference;

      }
      else
      {

         rc = NO_INHERITANCE_POINTER_TO_FOLLOW;

      }
   }
}


   return(rc);

}
