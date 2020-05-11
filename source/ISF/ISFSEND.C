/* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                  TEST.C

:LEVEL.                 MAIN

:AUTHOR.                Sal Johar

:DATE.                  Aug 8, 1987

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
#include "str.h"
#include "osk_i.h"
#include "osk.h"
#include "debug.h"
#include "debugi.h"
#include "exc.h"
#include "vdi.h"
#include "gks.h"
#include "ibm3270.H"
#include "ti.h"
#include "isf_i.h"
#include "isf.h"
#include "teies.h"

extern void prntmenu();
extern void get_vmid();
extern void get_data();
extern void clr_bottom();
extern void clr_line();
extern char get_choice();

#define SPARE 0
#define WID 1
#define DEVICE 1
#define RATIO 1
#define TOPLINE 24.0
#define MSGLINE 2.0 * RATIO           /* also defined in isferrs()  */
#define VMIDLINE 4.0 * RATIO
#define INP_LINE 2

/*        struct scv_xstr *SCVExtrn;
*/
main()
{

/* extern struct scv_xstr *SCVExtrn;   */
string vmid = S_NULL;
sint   pid = 0;
string msgsnd = S_NULL,
       msgrec = S_NULL;
real  xcord = 20.0,
      ycord = 0.0;
sint tmpint = 0;
sint choice = 0,
     rc = 0;
sint  maxcon = 0;
string tmpstr = S_NULL,
       qrystr = S_NULL,
       tmpstr2 = S_NULL;
VDIdevice *devptr = NIL;

#D_main( isfsend, ISCF)
   SCVExtrn  = NIL;
   ti_init();

/* devptr = v_intab(DEVICE);
   xcord = devptr->width;
   ycord = devptr->height;
   gs_wind(DEVICE, 0, xcord, 0, ycord);
   gs_view(DEVICE, 0, xcord, 0, ycord);
   gsview_pri(DEVICE ,0,0);
   g_seltrn( DEVICE);
  */
/*for (choice = -1; choice != 0; (choice = get_choice())) */

   choice = -1;
   do
   {
      #D_show( 'choice = %d', choice)

      if ( choice != -1)
         choice = get_choice();

      clr_bottom();
      xcord = 20.0;
      ycord = MSGLINE;

      switch ( choice )
      {
         case -1  :
                     prntmenu();
                     choice = -2;
                     break;
         case CLEAR:
         case PA2:
                     #D_say('trying to redraw screen')
/*                   g_redraw(WID);
*/
                     prntmenu();
                     break;
         case PF1 :
         case PF13:
                     rc = isf_star(&maxcon);

                     if (rc != 0)
                     {
                        isferrs( rc );
                     }
                     else
                     {
                        s_init( &tmpstr," Maximum connection possible:");
                        g_ttext( xcord, ycord, tmpstr);
                        xcord = xcord + s_len( tmpstr ) + 1.0;
                        s_sitos( &tmpstr, maxcon );
                        g_ttext( xcord, ycord, tmpstr);
                     }
                     break;
         case PF2 :
         case PF14:
                     rc = isf_end();
                     if ( rc != 0 )
                     {
                        isferrs( rc );
                     }
                     else
                     {
                        s_init( &tmpstr, "System down successfull");
                        g_ttext( xcord, ycord, tmpstr);
                     }
                     break;

         case PF3 :
         case PF15:
                     get_vmid( &vmid );

                     rc = isf_open( vmid, SPARE, &pid);
                     clr_line( VMIDLINE );
                     if ( rc != 0)
                     {
                        isferrs( rc );
                     }
                     else
                     {
                        s_init( &tmpstr,"Connection established with:");
                        s_cat( &tmpstr, tmpstr, vmid, 0, 0);
                        s_trim( &tmpstr, 'T');
                        s_init( &tmpstr2, "       Pid:");
                        s_cat( &tmpstr, tmpstr, tmpstr2, 0, 0);
                        s_drop( &tmpstr2);
                        g_ttext( xcord, ycord, tmpstr);
                        xcord = xcord + s_len( tmpstr ) + 1.0;
                        s_sitos( &tmpstr, pid);
                        g_ttext( xcord, ycord, tmpstr);
                     }
                     break;
         case PF4 :
         case PF16:
                     get_vmid( &vmid );
                     rc = isf_clos( vmid, 0 );
                     clr_line( VMIDLINE );
                     if ( rc != 0 )
                     {
                        isferrs( rc );
                     }
                     else
                     {
                        s_init(&tmpstr,"Connection closed successfully\
                                        with:");
                        s_cat( &tmpstr, tmpstr, vmid, 0, 0);
                        g_ttext( xcord, ycord, tmpstr);
                     }
                     break;
         case PF5 :
         case PF17:
                     get_vmid( &vmid );
                     get_data( &msgsnd );
                     rc = isf_send( vmid, SPARE, msgsnd);
                     clr_line( VMIDLINE );
                     clr_line( MSGLINE );
                     if ( rc != 0 )
                     {
                        isferrs( rc );
                     }
                     else
                     {
                        s_init( &tmpstr,"Message sent successfully to ");
                        s_cat( &tmpstr, tmpstr, vmid, 0, 0);
                        g_ttext( xcord, ycord, tmpstr);
                     }
/*                   s_drop( &msgsnd );         */
                     break;
         case PF6 :
         case PF18:
                     get_vmid( &vmid );
                     rc = isf_recv( vmid, SPARE, &msgrec );
                     clr_line( VMIDLINE );
                     if ( rc != 0 )
                     {
                        isferrs( rc );
                     }
                     else
                     {
                        s_init(&tmpstr,"Message received from ");
                        s_cat( &tmpstr, tmpstr, vmid, 0, 0);
                        ycord = VMIDLINE;
                        g_text( xcord, ycord, (byte *) tmpstr);
                        ycord = MSGLINE;
                        g_ttext( xcord, ycord, msgrec);
                     }
                     s_drop( &msgrec );
                     break;

         case PF7 :
         case PF19:
                     s_drop( &tmpstr );
                     tmpstr = S_NULL;
                     rc = isf_qry(vmid, NEW_CONNECTION,&tmpint, &qrystr);
                     if (rc != 0)
                     {
                        isferrs( rc);
                     }
                     else
                     {
                        if (tmpstr==S_NULL) /* if there was NO new conn*/
                        {
                           s_init(&tmpstr,"NO connection was accepted.");
                        }
                        else
                        {
                           s_copy( &vmid, qrystr, 0, 0);
                           s_init(&tmpstr,"Connection established by ");
                           s_cat( &tmpstr, tmpstr, vmid, 0, 0);
                        }
                        g_ttext(xcord, ycord, tmpstr);
                     }      /* if rc  <> 0   */
                     break;

         case PF8 :
         case PF20:
                     s_drop( &tmpstr);
                     tmpint = 0;
                     rc = isf_qry(vmid, NEW_SOMETHING, &tmpint, &qrystr);
                     if (rc != 0)
                     {
                        isferrs( rc );
                     }
                     else
                     {
                        s_copy( &vmid, qrystr, 0, 0);
                        switch ( tmpint )
                        {
                           case NEW_MESSAGE :
                              s_init( &tmpstr, "Message pending from ");
                              s_cat( &tmpstr, tmpstr, vmid, 0, 0);
                              break;

                           case NEW_CONNECTION :
                              s_init( &tmpstr, "Conn established by ");
                              s_cat( &tmpstr, tmpstr, vmid, 0, 0);
                              break;

                           case NEW_DISCONNECTION :
                              s_init( &tmpstr, "Dis-connected by ");
                              s_cat( &tmpstr, tmpstr, vmid, 0, 0);
                              break;

                           default :
                            s_init(&tmpstr,"Oops..something went wrong!")
                                                                        ;
                              break;
                        }
                        g_ttext(xcord, ycord, tmpstr);

                     }
                     break;

         case PF10:
         case PF22:
                     scv_dump();
                     s_init( &tmpstr, " Structures dumped in to file");
                     g_ttext( xcord, ycord, tmpstr);
                     break;
         case PF11:
         case PF23:
                     rc = scv__off();
                     if ( rc != 0)
                     {
                        isferrs( rc );
                     }
                     else
                     {
                        s_init(&tmpstr,"System forced OFF.");
                        g_ttext( xcord, ycord, tmpstr);
                     }
                     break;

         case PF12 :
         case PF24 :
                     s_init( &tmpstr, "HAVE A NICE DAY!!");
                     g_ttext( xcord, ycord, tmpstr);
                     choice = -9;
                     break;
         default  :
                     s_init( &tmpstr, "Invalid request");
                     g_ttext( xcord, ycord, tmpstr);
      }  /* switch */
   }  while (choice != -9);

   gdct_wk(WID);
   gcls_wk(WID);

   gcls_gks();

   #D_exit( 0 )

}  /* main */

void prntmenu()
{
#define YLINE(tmpy) (TOPLINE - tmpy) * RATIO

   real tmpx = 23.0,
        tmpy = 0.0;
   real x = 20.0,
        y = TOPLINE * RATIO;
   string tmpstr = S_NULL;

   #D_begin( prntmenu, ISCF)

   s_init( &tmpstr, "ISCF  Test Routine Menu");
   tmpy = YLINE(2.0);
   g_ttext( tmpx, tmpy, tmpstr);

   s_init( &tmpstr, "PF 1     Bring system up");
   tmpx = x;
   tmpy = YLINE(4.0);
   g_ttext( tmpx, tmpy, tmpstr);

   s_init( &tmpstr, "PF 2     Bring system down");
   tmpy = YLINE(5.0);
   g_ttext( tmpx, tmpy, tmpstr);

   s_init( &tmpstr, "PF 3     Establish connection with another system");
   tmpy = YLINE(6.0);
   g_ttext( tmpx, tmpy, tmpstr);

   s_init( &tmpstr, "PF 4     Dis-connect with another system");
   tmpy = YLINE(7.0);
   g_ttext( tmpx, tmpy, tmpstr);

   s_init( &tmpstr, "PF 5     Send message to another system");
   tmpy = YLINE(8.0);
   g_ttext( tmpx, tmpy, tmpstr);

   s_init( &tmpstr, "PF 6     Receive message from another system");
   tmpy = YLINE(9.0);
   g_ttext( tmpx, tmpy, tmpstr);

   s_init( &tmpstr, "PF 7     Query for a new connection");
   tmpy = YLINE(11.0);
   g_ttext( tmpx, tmpy, tmpstr);

   s_init( &tmpstr, "PF 8     Query for new connection or message");
   tmpy = YLINE(12.0);
   g_ttext( tmpx, tmpy, tmpstr);

   s_init( &tmpstr, "PF *     Not yet available");
   tmpy = YLINE(13.0);
   g_ttext( tmpx, tmpy, tmpstr);

   s_init( &tmpstr, "PF10     Dump of iucv structures");
   tmpy = YLINE(15.0);
   g_ttext(tmpx, tmpy, tmpstr);

   s_init( &tmpstr, "PF11     FORCE system shut-down");
   tmpy = YLINE(16.0);
   g_ttext( tmpx, tmpy, tmpstr);

   s_init( &tmpstr, "PF12    Exit from this test program");
   tmpy = YLINE(17.0);
   g_ttext( tmpx, tmpy, tmpstr);


   s_init( &tmpstr, "______________________________________________");
   tmpx = 18.0;
   tmpy = YLINE(18.0);
   g_ttext( tmpx, tmpy, tmpstr);

   s_drop( &tmpstr);

   #D_void( );
}   /* prntmenu()  */

void get_vmid( vmid )
   string *vmid;
{
   real x = 20.0,
        y = VMIDLINE;
   string tmpstr = S_NULL;
   area echoarea;
   sint status = 0;

   #D_begin( get_vmid, ISCF)

   clr_line(y);

   s_init( &tmpstr, "VMID: ");
   g_ttext( x, y, tmpstr);

   x = 30.0;
/* if ( *vmid == S_NULL)
   {
   } */
      s_init( vmid, "        ");

   echoarea.xmin = x;
   echoarea.xmax = x + 8.0;
   echoarea.ymin = y;
   echoarea.ymax = y;
   g_str_ini( WID, WID, *vmid, 1, &echoarea, S_NULL);
   grq_str( WID, WID, &status, *vmid);

   s_upper( *vmid, 0, 0);
   #D_str( *vmid, 'VMID read in');
   s_drop( &tmpstr );
   #D_void(  );
}

void get_data( datstr)
   string *datstr;
{
   real x = 20.0,
        y = MSGLINE;
   string tmpstr = S_NULL;
   sint status = 0;
   area echoarea;

   #D_begin( get_data, ISCF)

   clr_line( y );

   s_init( &tmpstr, "DATA:");
   g_ttext( x, y, tmpstr);

   echoarea.xmin = x + 5.0;
   echoarea.xmax = x + 50.0;
   echoarea.ymin = y;
   echoarea.ymax = y;
   s_init( datstr,"123456789A123456789B123456789C123456789D123456789E");
   g_str_ini( WID, WID, *datstr, 1, &echoarea, S_NULL);
   grq_str( WID, WID, &status, *datstr);

   #D_str(*datstr, 'Data read in')
   s_drop( &tmpstr );
   #D_void( );
}

void clr_bottom()
{
   real x = 0.0,
        y = 9.0 * RATIO;

   #D_begin( clr_bottom, ISCF)

/* for (; y > 1.0 ; y = y - 1.0)
   {
      clr_line( y );
   }
*/
   y = MSGLINE;
   clr_line( y );
   y = VMIDLINE;
   clr_line( y );

   #D_void( );
}

void clr_line( linnum )
   real linnum;
{
   string tmpstr = S_NULL,
          blank = S_NULL;
   real x = 0.0;

   #D_begin( clr_line, ISCF)

   s_init(&blank,"                                           \
                                     ");
/* s_init( &tmpstr, " ");
   s_dup( &blank, tmpstr, 0, 0, 50);    */

      g_ttext( x, linnum, blank);

   s_drop( &blank );
/* s_drop( &tmpstr );     */

   #D_void( )
}

char get_choice()
{

   char ch;

   #D_begin( get_choice, ISCF)

   vrq_chr(DEVICE, &ch);

   #D_return( ch );
}
