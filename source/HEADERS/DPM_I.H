/* Database Performance Monitor */

extern struct dpm_cb{

   /* VIR statistic information */
   tint blkacc;    /* total number of block accesses */
   tint blkhit;    /* # of block access which were in memory */
   tint lckblkcur; /* current # blocks locked in memory */
   tint lckblkmax; /* maximum # blocks locked in memory */
   tint blkchnmax; /* maximum # block chain ops needed to find blk */
   tint blkchnsum; /* total # block chain operations done */

   /* STL statistic information */
   tint stlclssum; /* total # stl close operations */
   tint stlnbsum;  /* total # nb blks on all closes */
   tint stllusum;  /* total # lu blks on all closes */
   tint stlnbmax;  /* max # nb blocks on a close */
   tint stllumax;  /* max # lu blocks on a close */
   } dpmcb;



/* $extract */

hword        dpmstart(); /* ( ); Startup/initialize */











/* $end-extract */



















