/************************************************************************

    <DOC>

    <NAME>         a_crntf(OCB *act_ocb, sint part_num, hword act_type,
                                                       sint attach_type)

    <LEVEL>        Activity

    <DATE>         March 12, 1989

    <AUTHOR>       Tanmay S. Kumar

    <GLOBAL>       None.

    <PARAM>        atch_ocb   OCB *   INPUT  The ocb of the object to
                                             which the activity will be
                                             attached.
                   modifier   string  INPUT
                   level      hword   INPUT
                   result     OCB **  IN/OUT

    <OUTPUT>       Sint.

    <PURPOSE>

    <CALLS>

    <ERRORS>

    <LIMITS>       None.

    <MODS>         None.

    <END>

    </DOC>

************************************************************************/

#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"
#include "obj_ntf.h"
#include "activity.h"
#include "isv.h"

sint  a_crntf( act_ocb, part_num, act_type, attach_type)

OCB     *act_ocb;
sint    part_num;
hword   act_type;
sint    attach_type;

{

   OCB          *params            =            NIL;

   sint         rc                 =              0,
                total_sec          =              0,
                marker             =              1;

   byte         f_byte             =           0x00,
                l_byte             =           0x00,
                e_byte             =           0x00,
                s_byte             =           0x00;

   string       modifier           =         S_NULL,
                modifier1          =         S_NULL,
                id                 =         S_NULL,
                for_line           =         S_NULL,
                label              =         S_NULL,
                sec_num            =         S_NULL;

   hword        level              = AUTOMATIC_CREATION;


   s_sitos(&sec_num, part_num);
   s_init(&for_line, " for ");
   obj_find(act_ocb, "ACTIVITY.ID", &id);
   if (attach_type == MAIL_OBJ)
   {
      isv_get(&label, ISV_MMSG, 1);
   }
   else if (attach_type == COMMENT_OBJ)
   {
      isv_get(&label, ISV_CC, 1);
   }
   s_trim(&id, 'B');
   s_cat(&id, label, id, 0, 0);
   s_cat(&id, for_line, id, 0, 0);

   obj_find(act_ocb, "ACTIVITY.PART_COUNT", &total_sec);
   rc = obj_find(act_ocb, "ACTIVITY.NOTIFS", &f_byte);

   if ( f_byte & NTF_FIRST )
   {
      /* then send a notification saying that the first section was */
      /* read or that a first response was made or that a first     */
      /* selection was made and set the marker to 1                 */
      switch (act_type)
      {
         case ISV_READ:
                       s_init(&modifier, " read section ");
                       s_init(&modifier1, " as the first section ");
                       break;

         case ISV_QUESTION:
                       s_init(&modifier, " made response ");
                       s_init(&modifier1, " as the first response ");
                       break;

         case ISV_SELECTIO:
                       s_init(&modifier, " made selection ");
                       s_init(&modifier1, " as the first selection ");
                       break;

         default:      break;
      }
      s_cat(&modifier, modifier, sec_num, 0, 0);
      s_cat(&modifier, modifier, modifier1, 0, 0);
      s_cat(&modifier, modifier, id, 0, 0);

      rc = cr_ntf(act_ocb, modifier, level, &params);
   }
   else if ( f_byte  & NTF_LAST )
   {
      /* then send a notification that the last section was read */
      /* or that the maximum number of responses was made or that*/
      /* the last maximum number of selections was made and set  */
      /* the marker                                              */
      switch (act_type)
      {
         case READ: s_init(&modifier, " read last section ");
                    break;

         case QUESTION:
                       s_init(&modifier, " reached the response\
                                         limit by making response ");
                       break;

         case SELECTION:
                       s_init(&modifier, " reached the selection\
                                         limit by making selection ");
                       break;

         default:      break;
      }
      s_cat(&modifier, modifier, sec_num, 0, 0);
      s_cat(&modifier, modifier, id, 0, 0);

      rc = cr_ntf(act_ocb, modifier, level, &params);
   }
   else
   {
      if ( f_byte & NTF_EACH )
      {
         /* then send a notification saying that a specific section was*/
         /* read or that a response number x was made or that a        */
         /* specific selection was made and set the marker to that #   */
         switch (act_type)
         {
            case READ:    s_init(&modifier, " read section ");
                          break;

            case QUESTION:
                          s_init(&modifier, " made response ");
                          break;

            case SELECTION:
                          s_init(&modifier, " made selection ");
                          break;

            default:      break;
         }
         s_cat(&modifier, modifier, sec_num, 0, 0);
         s_cat(&modifier, modifier, id, 0, 0);

         rc= cr_ntf(act_ocb, modifier, level, &params);
      }
   }
   s_drop(&modifier);
   s_drop(&modifier1);
   s_drop(&for_line);
   s_drop(&id);
   s_drop(&sec_num);

   return(rc);
}
