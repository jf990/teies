/* Buffer Memory Management */
extern struct bmm_cb {
   tint last_used;
   tint num_used;
   tint guard_count;  /* watchdog count to intercept code bugs */
   }bmmcb;   /* this is allocated in bmmstrt( ) */

#define BMM_GC 100; /* max # of tries before abort */

/* $extract */
struct virbh      *bmmab();   /* ( );
                              allocate a buffer, return header */

hword              bmmdump(); /* (*file_name);
                              dump buffers into a file for debugging */

hword              bmmfb();   /* (*bufhdr);
                              free the buffer */

hword              bmmlock(); /* (*bufhdr);
                              inc count on buffer */

byte              *bmmgba();  /* (*bufhdr);
                              gets actual starting address */

hword              bmmstat(); /* ( );
                              returns # buffers used at present*/

hword              bmmstrt(); /* ( );
                              start up subsystem*/

hword              bmmtest( ); /*       */


/* $endextract */
