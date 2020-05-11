/* $extract */

/* test routines */
/* test hier routines */
hword               dmsth1(); /* (*dmscb,path,data); */

hword               dmsth2(); /* (*dmscb,path,data); */

hword               dmsth3(); /* (*dmscb,path,data); */

hword               dmsth4(); /* (*dmscb,pathh1,datah1,pathh2,datah2,
                                 pathh3,datah3); */


/* test repeating segments */
hword               dmstr1(); /* (*dmscb,path,data); */

hword               dmstr2(); /* (*dmscb,path,data); */

hword               dmstr3(); /* (*dmscb,path1,data1,
                                 path2,data2,path3,data3); */

hword               dmstr4(); /* (*dmscb,pathr1,datar1,pathr2,datar2,
                    pathr3a,datar3a,pathr3b,datar3b,pathr3c,datar3c); */

hword               dmstr5(); /* (lfd); */

/* test index segments */
hword               dmstj1();

hword               dmstj2();

hword               dmstj3(); /* (lfd ) */

hword               dmstj4(); /* (lfd ) */

hword               dmstj5(); /* (lfd ) */

hword               dmstj6(); /* (lfd); */

/* general test routines */
hword               dmstg1(); /* (lfd); */

hword               dmstg2(); /* (alt_primary_lfd); */

/* test movement routines */

hword               dmstm1(); /* (lfd) */

hword               dmstm2(); /* (lfd,num_rec,filename); */

/* test pack */
hword               dmstp1(); /*(lfd,infile1,infile2); */



hword               dmstv1(); /* (lfd); remove */

hword               dmstd1(); /* (lfd,infile,incount,del_path,outfile,
                                 outcount); "Delete" */

hword               dmstest (); /* $main */


hword               ccomp   ();
/* $end-extract */




