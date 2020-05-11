#include "defs.h"
#include "pcomm.h"
#include "osk.h"
#include "str.h"
#include "debug.h"
#include "debugi.h"
#if (SYS_C == WATC13)
   #include <file.h>
#else
   #if (SYS_C == TURBOC15)
      #include "io.h"
      #include "fcntl.h"
   #endif
#endif
#include "teies.h"
#define MAX_DATA 254
#define IDLEN 8

struct construc *condata;
hword crc_tab(:CRC_TAB_SIZE:);
main( argc, argv )
   sint argc;
   byte *argv(::);
{
   sint rc = 0, fd = 0, i = 0;
   string idsal = S_NULL;
   byte *databuf = NULL,
        *tmpbuf = NULL,
        *filname = NULL;
   hword datalen = MAX_DATA,
        filelen = 0;

   #D_main(pc_main, pcomm)

   condata = NULL;



   if( argc == 1 )
   {
      printf("You forgot the args, dummy!\n");
      exit(1);
   }

   databuf = os_mema( MAX_DATA );

   rc = pc_on( databuf, &datalen);
   if( rc != 0) exit(rc);


   datalen = MAX_DATA;
   rc = pc_open(databuf, &datalen);
   if( (rc % 100) == 28)             /* if screen is already open   */
   {
      pc_close();
      rc = pc_open(databuf, &datalen);
   }

   if( rc != 0)       /* if there still is an error  then get out */
   {
      pc_off();
      exit(rc);
   }

  #if SYS_OS == CMS40
   rc = os_conn(CON_CLEAR, CON_DEVICE, NULL, NULL, 0);
  #endif

   if( *argv(:1:) == 'S' )         /* send */
   {
      for(i = 0;i < MAX_DATA;databuf(:i:) = i,i++);
      datalen = MAX_DATA;

      filname = "TMP PRT A";
      filelen = strlen( filname );


     #if 0
      fd = open( filname, O_RDONLY);
      datalen = read( fd, databuf, MAX_DATA );
      close( fd );
      #D_dump(databuf, datalen, 'Data being sent');
     #endif


      rc = pc_send(filname, filelen, databuf, datalen);
   }
   else  /* if option is to send */
   {
      filelen = 20;
      filname = os_mema( filelen );
      rc = pc_recv(filname, &filelen, databuf, &datalen);

#if 0
      if( rc == 0 )
      {
         #D_dump(filname, filelen, 'File recvd');
         #D_dump(databuf, datalen, 'Data recvd');
         fd = open(filname, O_WRONLY|O_APPEND|O_CREAT);
         write( fd, databuf, datalen);
         close( fd );
      }
#endif

      os_memd( filname );
   }  /* if sending or receiving */

#if 0
  #if SYS_OS == CMS40
   rc = os_conn(CON_CLEAR, CON_DEVICE,NULL, NULL, 0);
  #endif
#endif

      os_memd( databuf );

      rc = pc_close();
      rc = pc_off();

      #D_exit( rc )
}
