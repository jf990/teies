/* $extract $maker-proto */

hword               dmsted(hword lfd, struct dms_cb *dmscb
                          , hword how_many, hword sav_len);
                    /* delete elements in file from dmscb */


hword               dmstev(hword lfd,struct dms_cb *dmscb,
                          hword how_many, hword sav_len);
                              /*check file <=> dmscb */

hword               dmstew(int fd,struct dms_cb *dmscb
                          ,hword how_many, hword sav_len);
                              /*file -> dmscb */

/* test routines */
/* test hier routines */
hword               dmsth1(struct dms_cb *dmscb, byte *path
                           , byte *data);

hword               dmsth2(struct dms_cb *dmscb,byte *path
                          ,byte *data);

hword               dmsth3(struct dms_cb *dmscb,byte *path,byte *data);

hword               dmsth4(struct dms_cb *dmscb
                          , byte *pathh1, byte *datah1
                          , byte *pathh2, byte *datah2
                          , byte *pathh3, byte *datah3);


/* test repeating segments */
hword               dmstr1(struct dms_cb *dmscb,byte *path,byte *data);

hword               dmstr2(struct dms_cb *dmscb,byte *path,byte *data);

hword               dmstr3(struct dms_cb *dmscb,byte *path1, byte *data1
                          , byte *path2, byte *data2
                          , byte *path3, byte *data3);

hword               dmstr4(struct dms_cb *dmscb
                          , byte *pathr1, byte *datar1
                          , byte *pathr2, byte *datar2
                          , byte *pathr3a, byte *datar3a
                          , byte *pathr3b, byte *path3b
                          , byte *pathr3c, byte *datar3c);

hword               dmstr5( hword lfd);

/* test index segments */
hword               dmstj1();

hword               dmstj2();

hword               dmstj3(hword lfd);

hword               dmstj4(hword lfd);

hword               dmstj5(hword lfd);

hword               dmstj6(hword lfd);

hword               dmstj7(hword lfd);

hword               dmstj8(hword lfd);

hword               dmstj9(hword lfd);


/* general test routines */
hword               dmstg1(hword lfd);

hword               dmstg2();

hword               dmstg3(hword lfd);
                    /* fill up the database on purpose with one
                    huge index, test disk full recovery on repl */

hword               dmstg4(hword lfd); /* test journaling */

hword               dmstg5(hword lfd);
                    /* test dms_qnb, dms_qfb, dms_comp, dms_sync */

hword               dmstg6(hword lfd);
/* test sending invalid path combinations */


/* test movement routines */

hword               dmstm1(hword lfd);

hword               dmstm2(hword lfd, hword num_rec, byte *filename
                          ,byte *pack_point, byte *loop4start);


/* pack test functions */

hword               dmstp(hword lfd);

hword               dmstp1(hword lfd, byte *infile1, byte *infile2);

hword               dmstp2(hword lfd, byte *infile1);

hword               dmstp3(hword lfd, byte *infile1, byte *infile2
                          , byte *infile3, byte *path_to_move);

/* remove test functions */

hword               dmstv(void); /* main dmstv test routine */

hword               dmstv1(hword lfd, byte *infile, hword incount
                          , byte *deletion_path
                          , byte *outfile, hword outcount);
                    /* allocate dmscb, fill with infile, delete
                       using given path, result should be same as
                       outfile */

hword               dmstv2(hword lfd, byte *infile, byte *rmfile
                          ,byte *infile2, byte *shouldbefile);
                    /* create 'infile', delete 'rmfile', then
                       add 'infile2', result should be 'shouldbefile' */

hword               dmstv3(hword lfd, byte *infile1, byte *infile2
                    , byte *del_path, byte *final_file
                    , boolean close_after_create);
                    /* create 'infile1' and 'infile2', delete stuff
                    from infile2, pack infile1, unpack into infile2,
                    results should be 'final_file' */


hword               dmstv4(hword lfd, hword num2try);



hword               dmstc1(hword lfd, byte *infile);
                    /* test dms_copy */

hword               dmstd1(hword lfd,byte *infile,hword incount
                          , byte *del_path, byte *outfile
                          , hword outcount);

hword               dmstd2(hword lfd, byte *infile);




hword               dmstl1(hword lfd, byte *infile);
                    /* test hier to see if link counts are going ok */


hword               dmstl2(hword lfd, byte *infile);
                    /* test index to see if link counts are going ok */

hword               dmstl3(hword lfd, byte *infile);
                    /* test index to see if link counts are going ok */

hword               dmstl4(hword lfd, byte *infile, hword max2check);
                    /* test index, sgj key repl check */

hword               dmstl5(hword lfd, byte *infile);
                    /* read in amt, close, dic_vlc */


hword               dmstest (); /* $main */


hword               dmstk1(hword lfd, byte *infile);








hword               ccomp   (byte *from, byte *to,hword len);
/* $end-extract */




