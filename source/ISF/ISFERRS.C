/* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                  ISFERRS C

:LEVEL.                 ISCF

:AUTHOR.                Sal Johar

:DATE.                  Oct 22, 1987

:INPUT.

:OUTPUT.                rc:   0 if all OK, else error

:INPUT_GLOBALS.         -none-

:OUTPUT_GLOBALS.        -none-

:PURPOSE.

:CALLS.                 -none-

:ERRORS.

:LIMITS.

:MODS.

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "isf.h"
#include "isf_i.h"

#define RATIO 1
#define MSGLINE 2.0 * RATIO             /* also defined in isfsend()   */

extern void clr_line();
extern sint g_ttext();

void isferrs( rc )
   int rc;
{
   real x = 0.0,
        y = MSGLINE;
   string tmpstr = S_NULL,
         tmpstr2 = S_NULL;
   byte  *e = NIL,
         *s = NIL,
         *e1 = NIL,
         *s1 = NIL;
   int  errnum = 0,
        subnum = 0;

   #D_begin( isferrs, ISCF)

   clr_line( y );

   s_init( &tmpstr, "*** Error(");
   s_sitos( &tmpstr2, rc);
   s_cat(&tmpstr,tmpstr,tmpstr2,0,0);

   subnum = (rc % 1000) / 100;
   errnum = rc % 100;


   switch ( subnum )
   {
      case 1:
    s =")INIT:";
#define SUB_ERR 100
    switch ( errnum )
    {
case 4: e="HNDIUCV:SET: System already initialized; Hit PF11 to \
                                                       force it off.";
        break;
case 36: e ="HNDIUCV:SET: Error while attempting to read VM directory.";
        break;
case 51: e ="DMSFREE: Insufficent storage space.";
         break;
case 52: e ="DMSFREE/DMSFRET: User storage pointer destroyed.";
         break;
case 53: e ="DMSFREE/DMSFRET: Nucleus storage pointer destroyed.";
         break;
case 54: e ="DMSFREE: Invalid size was requested.";
         break;
case 55: e ="DMSFRET: Invalid size was requested.";
         break;
case 56: e ="DMSFRET: Space never allocated by DMSFREE.";
         break;
case 57: e ="DMSFRET: Pointer is not double-word aligned.";
         break;
case 59: e ="DMSFREE/DMSFRET: Some sort of memory management error.";
         break;
case ERR_ALREADY_INIT :       /*  65  */
         e ="System is already initialized.";
         break;
    }
    break;
 case 2:
    s =")OFF:";
#undef  SUB_ERR
#define SUB_ERR 200
    switch ( errnum )
    {
case  8: e ="HNDIUCV:CLR: System was never initialized; Hit PF1.";
          break;
case 52: e ="DMSFREE/DMSFRET: User storage pointer destroyed.";
          break;
case 53: e ="DMSFREE/DMSFRET: Nucleus storage pointer destroyed.";
          break;
case 55: e ="DMSFRET: Invalid size was requested.";
          break;
case 56: e ="DMSFRET: Space never allocated by DMSFREE.";
          break;
case 57: e ="DMSFRET: Pointer is not double-word aligned.";
          break;
case 59: e ="DMSFREE/DMSFRET: Some sort of memory management error.";
          break;
case ERR_NEVER_INIT :      /*  65  */
         e ="System is not initilized yet.";
          break;
case ERR_NEVER_INIT + 1 :  /*  66  */



         e ="OFF:System already initalized; hit PF2 to shutdown.";
          break;

    }
    break;
 case 3:
    s =")OPEN:";
#undef  SUB_ERR
#define SUB_ERR 300
    switch ( errnum )
    {
case 11: e ="Other user is not logged on.";
          break;
case 12: e ="Other user has not initialized his system to handle IUCV.";
          break;
case 13: e ="Exceeded number of connection possible.";
          break;
case 14: e ="Other user exceeded number of possible connection.";
          break;
case 15: e ="No authorization found to communicate to the other user.";
          break;
case ERR_CONN_BUT_SEVER :  /*  62  */
         e ="Already conn but other sys dis-conn. close conn 1st.";
          break;
case ERR_ALREADY_CONN :   /*  61  */
case ERR_CONN_EXIST :     /*  64  */
         e ="Already connected to the other user.";
          break;
case ERR_NEVER_INIT:  /*  65  */
         e ="System is not initilized yet.";
          break;
case ERR_SELF_CONN :    /*  68  */
         e ="Can not connect to yourself.";
          break;
case ERR_CON_TIME_OUT :  /*  81  */
         e ="Open times out.";

    }
    break;
 case 4:
    s =")CLOSE:";
#undef  SUB_ERR
#define SUB_ERR 400
    switch ( errnum )
    {
case  1: e ="Invalid path id specified.";
          break;
case  8: e ="CMSIUCV:SEVER: System was never initialized; Hit PF1.";
          break;
case 12: e ="Program does not own the path.";
          break;
case 52: e ="DMSFREE/DMSFRET: User storage pointer destroyed.";
          break;
case 53: e ="DMSFREE/DMSFRET: Nucleus storage pointer destroyed.";
          break;
case 55: e ="DMSFRET: Invalid size was requested.";
          break;
case 56: e ="DMSFRET: Space never allocated by DMSFREE.";
          break;
case 57: e ="DMSFRET: Pointer is not double-word aligned.";
          break;
case 59: e ="DMSFREE/DMSFRET: Some sort of memory management error.";
          break;
case ERR_NEVER_INIT:  /*  65  */
         e ="System is not initilized yet.";
          break;
case ERR_NO_PATH :    /*  66  */
case ERR_SELF_CONN :    /*  68  */
         e ="Could not find path id to dis-connect.";
          break;
    }
    break;
 case 5:
    s =")SEND:";
#undef  SUB_ERR
#define SUB_ERR 500
    switch ( errnum )
    {
case 01: e ="Invalid Pathid.";
          break;
case 03: e ="Message number limit exceeded.";
          break;
case 51: e ="DMSFREE: Insufficent storage space.";
          break;
case 52: e ="DMSFREE/DMSFRET: User storage pointer destroyed.";
          break;
case 53: e ="DMSFREE/DMSFRET: Nucleus storage pointer destroyed.";
          break;
case 54: e ="DMSFREE: Invalid size was requested.";
          break;
case 55: e ="DMSFRET: Invalid size was requested.";
          break;
case 56: e ="DMSFRET: Space never allocated by DMSFREE.";
          break;
case 57: e ="DMSFRET: Pointer is not double-word aligned.";
          break;
case 59: e ="DMSFREE/DMSFRET: Some sort of memory management error.";
          break;
case ERR_CONN_BUT_SEVER :  /*  62  */
         e ="Connected but other sys dis-conn. close conn 1st.";
          break;
case ERR_NEVER_INIT:  /*  65  */
         e ="System is not initilized yet.";
          break;
case ERR_NO_PATH :    /*  66  */
case ERR_SELF_CONN :    /*  68  */
         e ="Could not find path-id to send message.";
          break;
    }
    break;
 case 6:
    s =")RECV:";
#undef  SUB_ERR
#define SUB_ERR 600
    switch ( errnum )
    {
case 01: e ="Invalid Pathid.";
          break;
case 05: e ="Receive buffer too short to accept message in.";
          break;
case 06: e ="Problems accessing senders data.";
          break;
case 07: e ="Problems accessing senders data.";
          break;
case  9: e ="Message purged by the other user.";
          break;
case 52: e ="DMSFREE/DMSFRET: User storage pointer destroyed.";
          break;
case 53: e ="DMSFREE/DMSFRET: Nucleus storage pointer destroyed.";
          break;
case 55: e ="DMSFRET: Invalid size was requested.";
          break;
case 56: e ="DMSFRET: Space never allocated by DMSFREE.";
          break;
case 57: e ="DMSFRET: Pointer is not double-word aligned.";
          break;
case 59: e ="DMSFREE/DMSFRET: Some sort of memory management error.";
          break;
case ERR_IUCV_RECV :  /*  63  */
         e ="IUCV says there is no message to receive.";
          break;
case ERR_NEVER_INIT:  /*  65  */
         e ="System is not initilized yet.";
          break;
case ERR_NO_PATH :    /*  66  */
case ERR_SELF_CONN :    /*  68  */
         e ="Could not find path-id to receive message.";
          break;
case ERR_NO_MSG :     /*  67  */
         e ="No message waiting to be received.";
          break;
    }  /* end switch errnum */
    break;
  case 7:
    s =")QRY:";
#undef  SUB_ERR
#define SUB_ERR 700
    switch ( errnum )
    {
case ERR_ALREADY_CONN :  /*  61  */
case ERR_CONN_EXIST   :  /*  64  */
         e ="Already connected to the other system.";
          break;
case ERR_CONN_BUT_SEVER :/*  62  */
         e ="Connected but connection has been severed by other.";
          break;
case ERR_NEVER_INIT:  /*  65  */
         e ="System is not initilized yet.";
          break;
case ERR_NO_PATH :    /*  66  */
         e="Could not find vmid in tables.";
          break;
case ERR_NO_MSG :     /*  67  */
         e ="No message pending.";
          break;
case ERR_SELF_CONN :  /*  68  */
         e ="Can't interact with self!!";
          break;
case ERR_NO_NEW_CONN :/*  70  */
         e ="No New connections accepted.";
          break;
case ERR_NO_NEW_DSC : /*  71  */
         e ="No New dis-connections.";
          break;
case ERR_NEW_MSG_BUT_DISC :   /*  72  */
         e ="New message but connection has been severed.";
          break;
case ERR_NEW_CONN_BUT_DISC :  /*  73  */
         e ="New connection but already has been severed.";
          break;
case ERR_MSG_BUT_NO_CONN :    /*  74  */
         e ="New message but can't find from who.";
          break;
case ERR_NO_MATCHING_PID : /*  79  */
         e ="No matching id was found for pid.";
          break;
case ERR_FUNC_NOT_DEF :   /*  80  */
         e ="Invalid query option.";
          break;
    }
    break;

    default:
           s = ")****:";

   }  /* switch  subnum */

#D_say('Out of switch')
   s1 = os_mema( strlen( s ) );
   strcpy( s1, s);
   s_init(&tmpstr2, s1);
   s_cat(&tmpstr,tmpstr,tmpstr2,0,0);
   if ( e1 == NULL)
      e1 = "Error explaination not found";
   e1 = os_mema( strlen( e ) );
   strcpy( e1, e);
   s_init(&tmpstr2, e1);
   s_cat(&tmpstr, tmpstr, tmpstr2, 0, 0);
   g_ttext( x, y, tmpstr);

   os_memd( s1 );
   os_memd( e1 );

   s_drop( &tmpstr );
   s_drop( &tmpstr2 );

   #D_void();
}
