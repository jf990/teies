/******************************************************************

   NAME.       SKIMAIN.C

   LEVEL.      SKI

   AUTHOR.     Pamela Cham

   DATE.       August 31, 1989

   PARAM.

   PURPOSE.    Test routine for SKI_ADD, SKI_FIND, and SKI_DEL

   CALLS.      ski_add(), ski_find(), ski_del()

******************************************************************/

#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "teies.h"
#include "ski3.h"

main ()
{
   string primary=S_NULL;
   string secondary=S_NULL;
   OID *oid=NIL;
   sint status=0;
   sint retcode=0;

   #D_main (skimain, SKI);
   t_init (S_NULL, S_NULL, 0);
   #D_master (ON);
   #D_say ("BEGIN SKIMAIN ");

   #D_say ('***     TESTING SKI_ADD ROUTINE     ***');

   /*  test by passing null primary & secondary keys                   */
   retcode = ski_add (primary, secondary, oid, status);

   /*  test by passing null oid                                        */
   oid = NIL;
   s_init (&primary, "PERSON");
   s_init (&secondary, "NAME");
   status = 10;
   retcode = ski_add (primary, secondary, oid, status);

   /*  test by passing just a single (primary) key                     */
   s_init (&primary, "PERSON");
   secondary=S_NULL;
   oid = (OID *) os_mema (sizeof (OID));
   oid_new (oid);
   status = 10;
   retcode = ski_add (primary, secondary, oid, status);
   /* test by passing valid but random values                          */
   s_init (&primary, "PERSON");
   s_init (&secondary, "NAME");
   oid_new (oid);
   status = 10;
   retcode = ski_add (primary, secondary, oid, status);

   s_init (&primary, "PERSON");
   s_init (&secondary, "ADDR");
   oid_new (oid);
   status = 20;
   retcode = ski_add (primary, secondary, oid, status);

   s_init (&primary, "BOOK");
   s_init (&secondary, "TITLE");
   oid_new (oid);
   status = 30;
   retcode = ski_add (primary, secondary, oid, status);

                                       /* add this entry the 2nd time  */
   s_init (&primary, "PERSON");
   s_init (&secondary, "ADDR");
   oid_new (oid);
   status = 20;
   retcode = ski_add (primary, secondary, oid, status);

   s_init (&primary, "ITEM");
   s_init (&secondary, "COST");
   oid_new (oid);
   status = 10;
   retcode = ski_add (primary, secondary, oid, status);


   #D_say ('***     TESTING SKI_FIND ROUTINE      ***');

   /* test by giving null search keys                                  */
   oid = (OID *) os_mema (sizeof(OID));
   primary = S_NULL;
   secondary = S_NULL;
   retcode = ski_find (primary, secondary, &oid, &status);
   #D_oid ('oid');
   #D_show ('status=%d','status');

   /* test by giving null oid                                          */
   s_init (&primary, "BOOK");
   s_init (&secondary, "TITLE");
   oid = NIL;
   retcode = ski_find (primary, secondary, &oid, &status);

   /* test by giving single key                                        */
   s_init (&primary, "BOOK");
   secondary = S_NULL;
   oid = (OID *) os_mema (sizeof(OID));
   retcode = ski_find (primary, secondary, &oid, &status);

   /* test by giving valid input parameters at random                  */
   s_init (&primary, "BOOK");
   s_init (&secondary, "TITLE");
   retcode = ski_find (primary, secondary, &oid, &status);
   #D_oid ('oid');
   #D_show ('status=%d','status');

   s_init (&primary, "PERSON");
   s_init (&secondary, "ADDR");
   retcode = ski_find (primary, secondary, &oid, &status);
   #D_oid ('oid');
   #D_show ('status=%d','status');

#D_set('D5DMS',ON);
#D_set('D2OBJ_REP',ON);                /* try adding another entry     */
#D_set('D2OBJ_GET',ON);
   s_init (&primary, "FILE");
   s_init (&secondary, "ATTR");
   oid_new (oid);
   status = 30;
   retcode = ski_add (primary, secondary, oid, status);
#D_set('D2OBJ_GET',OFF);
#D_set('D2OBJ_REP',OFF);
#D_set('D5DMS',OFF);

   s_init (&primary, "ITEM");
   s_init (&secondary, "COST");
   retcode = ski_find (primary, secondary, &oid, &status);
   #D_oid ('oid');
   #D_show ('status=%d','status');

   s_init (&primary, "PERSON");        /* not in SKI           *********/
   s_init (&secondary, "ID");
   oid = (OID *) os_mema (sizeof(OID));
   retcode = ski_find (primary, secondary, &oid, &status);


   s_init (&primary, "PERSON");
   secondary = S_NULL;
   oid = (OID *) os_mema (sizeof(OID));
   retcode = ski_find (primary, secondary, &oid, &status);
   #D_oid ('oid');
   #D_show ('status=%d','status');

   s_init (&primary, "PERSON");
   s_init (&secondary, "NAME");
   retcode = ski_find (primary, secondary, &oid, &status);
   #D_oid ('oid');
   #D_show ('status=%d','status');

   s_init (&primary, "FILE");
   s_init (&secondary, "ATTR");
   retcode = ski_find (primary, secondary, &oid, &status);
   #D_oid ('oid');
   #D_show ('status=%d','status');


   #D_say ('***     TESTING SKI_DEL ROUTINE      ***');

   s_init (&primary, "ITEM");
   s_init (&secondary, "COST");
#D_set('D5DMS',ON);
#D_set('D2OBJ_RMV',ON);
#D_set('D2OBJ_GET',ON);
   retcode = ski_del (primary, secondary);
#D_set('D2OBJ_GET',OFF);
#D_set('D2OBJ_RMV',OFF);
#D_set('D5DMS',OFF);
   oid = (OID *) os_mema (sizeof (OID));     /* see if deleted already */
   retcode = ski_find (primary, secondary, &oid, &status);

   s_init (&primary, "FILE");
   s_init (&secondary, "ATTR");
   retcode = ski_find (primary, secondary, &oid, &status);
   #D_oid ('oid');
   #D_show ('status=%d','status');

   s_init (&primary, "BOOK");                /* not in SKI     *********/
   s_init (&secondary, "SUBJECT");
#D_set('D5DMS',ON);
#D_set('D2OBJ_RMV',ON);
#D_set('D2OBJ_GET',ON);
   retcode = ski_del (primary, secondary);
#D_set('D2OBJ_GET',OFF);
#D_set('D2OBJ_RMV',OFF);
#D_set('D5DMS',OFF);

   s_init (&primary, "PERSON");
   secondary = S_NULL;
#D_set('D5DMS',ON);
#D_set('D2OBJ_RMV',ON);
#D_set('D2OBJ_GET',ON);
   retcode = ski_del (primary, secondary);
#D_set('D2OBJ_GET',OFF);
#D_set('D2OBJ_RMV',OFF);
#D_set('D5DMS',OFF);
   oid = (OID *) os_mema (sizeof(OID));      /* see if deleted already */
   retcode = ski_find (primary, secondary, &oid, &status);

   s_init (&primary, "FILE");
   s_init (&secondary, "ATTR");
   retcode = ski_find (primary, secondary, &oid, &status);
   #D_oid ('oid');
   #D_show ('status=%d','status');

                                           /* look for this old entry */
   s_init (&primary, "PERSON");
   s_init (&secondary, "NAME");
   retcode = ski_find (primary, secondary, &oid, &status);
   #D_oid ('oid');
   #D_show ('status=%d','status');

#D_set('D5DMS',ON);
#D_set('D2OBJ_GET',ON);
   s_init (&primary, "FILE");
   s_init (&secondary, "ATTR");
   retcode = ski_find (primary, secondary, &oid, &status);
#D_set('D2OBJ_GET',OFF);
#D_set('D5DMS',OFF);
   #D_oid ('oid');
   #D_show ('status=%d','status');

   /* add entries again */
   #D_say ('*** Testing ADD and FIND again ***');
   s_init (&primary, "PERSON");
   s_init (&secondary, "AGE");
   oid_new (oid);
   status = 10;
   retcode = ski_add (primary, secondary, oid, status);

   s_init (&primary, "PERSON");
   secondary=S_NULL;
   oid_new (oid);
   status = 10;
   retcode = ski_add (primary, secondary, oid, status);

   s_init (&primary, "PERSON");
   s_init (&secondary, "STAT");
   oid_new (oid);
   status = 10;
   retcode = ski_add (primary, secondary, oid, status);

   /* look for the added entries */
   s_init (&primary, "PERSON");
   s_init (&secondary, "STAT");
   retcode = ski_find (primary, secondary, &oid, &status);
   #D_oid ('oid');
   #D_show ('status=%d','status');

   s_init (&primary, "PERSON");
   s_init (&secondary, "AGE");
   retcode = ski_find (primary, secondary, &oid, &status);
   #D_oid ('oid');
   #D_show ('status=%d','status');

   s_init (&primary, "PERSON");
   secondary = S_NULL;
   retcode = ski_find (primary, secondary, &oid, &status);
   #D_oid ('oid');
   #D_show ('status=%d','status');

/* #D_master (OFF);   */
   t_shutdn();
/* #D_master (ON);    */
   #D_leave ('retcode','');
}
