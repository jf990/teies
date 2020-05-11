/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    t_nxtfld()                                             |
  |                                                                   |
  |  :AUTHOR.  John L. Foster                                         |
  |                                                                   |
  |  :DATE.    August 11, 1988                                        |
  |                                                                   |
  |  :LEVEL.   TM                                                     |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.   field_name            string *         OUTPUT          |
  |                                                                   |
  |                Where to return the name of the next modified      |
  |                field of the current formed state.                 |
  |                                                                   |
  |  :PARAM.   field_contents        string *                         |
  |                                                                   |
  |                Where to return the contents of the next modified  |
  |                field of the current formed state.                 |
  |                                                                   |
  |  :PARAM.   length_of_contents    sint   *                         |
  |                                                                   |
  |                Where to return how long the contents are of the   |
  |                field (even if it is a string field).              |
  |                                                                   |
  |  :PARAM.   type_of_contents      sint   *                         |
  |                                                                   |
  |                Where to return the data type of the contents data |
  |                of the field.  Refer to OBJ.H for data type def-   |
  |                initions.                                          |
  |                                                                   |
  |  :PARAM.   field_type            sint   *                         |
  |                                                                   |
  |                Where to return the field type of the field.  It   |
  |                is either (1) if it is a TEIES interface variable  |
  |                field or (2) if it is a direct object reference.   |
  |                                                                   |
  |  :PURPOSE. This routine will return the next field in the current |
  |            form that has been modified by the user.  Two things   |
  |            to remember:                                           |
  |                                                                   |
  |            1) After this routine returns the field is no longer   |
  |               considered modified;                                |
  |                                                                   |
  |            2) All fields are reset to not modified before the     |
  |               form is displayed on the screen.  Therefore all     |
  |               modified fields are only tagged if they were in fact|
  |               edited by the user the *last* time the form was     |
  |               displayed.  It matters not if this routine, or any  |
  |               other, accessed or processed the fields.            |
  |                                                                   |
  |  :OUTPUT.  sint    0 indicates a modified field is returned;      |
  |                    NO_MORE_MODIFIED_FIELDS indicates just that.   |
  |                                                                   |
  |  :CALLS.                                                          |
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
#include "tm.h"
#include "form.h"



sint   t_nxtfld( field_name, field_contents, length_of_contents,
                 type_of_contents, field_type                       )

string          *field_name,
                *field_contents;

sint            *length_of_contents,
                *type_of_contents,
                *field_type;


{





   return(0);

}
