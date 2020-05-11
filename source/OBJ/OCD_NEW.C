/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    ocd_new                                                |
  |                                                                   |
  |  :AUTHOR.  JF                                                     |
  |                                                                   |
  |  :DATE.    November 30, 1987                                      |
  |                                                                   |
  |  :LEVEL.   OBJ                                                    |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.   *ocd_ptr  struct OCD   INOUT  Pointer to the OCD that  |
  |                                          will get the new OCD.    |
  |                                                                   |
  |  :PARAM.   *component byte        INPUT  Pointer to an initialize |
  |                                          string for the OCD name. |
  |                                                                   |
  |  :PURPOSE. Allocate a new Object Component Descriptor.  If the    |
  |            pointer is NIL the new allocate new memory.  Otherwise |
  |            the variable is assumed to be a valid pointer to an    |
  |            already existing OCD that is to be cleared out.        |
  |                                                                   |
  |  :OUTPUT.  * struct OCD                                           |
  |                                                                   |
  |  :CALLS.   os_mema().                                             |
  |                                                                   |
  |  :ERRORS.  0 if we cannot make sense out of the pointer.          |
  |                                                                   |
  |  :LIMITS.  None known.                                            |
  |                                                                   |
  |  :END.                                                            |
  |                                                                   |
  +-------------------------------------------------------------------+*/

#include "defs.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "debug.h"




OCD       *ocd_new ( ocd_ptr, component )


OCD       **ocd_ptr;
byte       *component;


{

OCD       *new_component = NIL;


if( DATA_POINTER_OK( component ) )
{


}
new_component = *ocd_ptr;
if( new_component == NIL )
{

    new_component = (ocdrecord *)  os_mema ( sizeof( ocdrecord ) );

}
else
{

    s_drop( &(new_component->name) );
    s_drop( &(new_component->key)  );

}

new_component->name = S_NULL;
new_component->key  = S_NULL;
new_component->bias =      0;
new_component->num_keys =  0;

if( component != NIL )
{

   s_init( &(new_component->name), component );

}
if( ocd_ptr != NIL )
{

   *ocd_ptr = new_component;

}
   return(new_component);

}
