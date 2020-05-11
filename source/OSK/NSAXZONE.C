/*********************************************************************/ 00010000
/*   THIS PRODUCT CONTAINS "RESTRICTED MATERIALS OF IBM"             */ 00020000
/*   5798-FAL (C) COPYRIGHT IBM CORPORATION 1987, 1988               */ 00030000
/*   ALL RIGHTS RESERVED                                             */ 00040000
/*   LICENSED MATERIALS - PROPERTY OF IBM                            */ 00050000
/*   SEE COPYRIGHT INSTRUCTIONS, G120-2083                           */ 00060000
/*********************************************************************/ 00070000
                                                                        00080000
#define MAXTABLENAME 18                                                 00090000
#define THRESHOLD 16          /* K min amount of storage required    */ 00100000
                                                                        00110000
#include <stdio.h>                                                      00120000
#include <ctype.h>                                                      00130000
#include "cmolver.h"                                                    00140000
#include "cmc2wis.h"                                                    00150000
                                                                        00160000
extern char NULL;   /* defined in cmsysde c                       */    00170000
extern int DEBUG;                                                       00180000
extern int AXFRDEBUG;                                                   00190000
extern nsdbsql();            /* access sql data base               */   00200000
extern int timeout;                                                     00210000
extern struct qsection *questions;                                      00220000
extern struct res_header header;                                        00230000
extern char *answers;         /* array of answers structure          */ 00240000
extern char *auth;            /* array of authority sturcture        */ 00250000
extern char *addl;         /* array of additional structures         */ 00260000
extern int conn;                    /* virtual circuit connection no.*/ 00270000
static int hmaxfr;           /* the number of zones to be transferred*/ 00280000
static int hmtcp;            /* 0 no tcp table defined; 1 tcp defined*/ 00290000
extern char xff;     /* tell nsdbsql to free the sql ptr and data bfr*/ 00300000
                                                                        00310000
struct axfrblk                                                          00320000
    {                                                                   00330000
      char domain[50];                                                  00340000
      char remotens[50];                                                00350000
      int current;                                                      00360000
      int xfer;                  /* 1- zone transfer required */        00370000
      int serial;                                                       00380000
      int refresh;                                                      00390000
      int retry;                                                        00400000
      char sqlt0[MAXTABLENAME];                                         00410000
      char sqlt1[MAXTABLENAME];                                         00420000
      struct qsection *question;                                        00430000
      struct axfrblk *qnext;                                            00440000
  } axfrblk;                                                            00450000
static struct axfrblk *axfrfirst, *axfrlast, *axfrptr,*axfrlp,*axfrcur; 00460000
                                                                        00470000
axfraddq(domain,remotens,sqltable)                                      00480000
char *domain;                                                           00490000
char *remotens;                                                         00500000
char *sqltable;                                                         00510000
{                                                                       00520000
 axfrptr = (struct axfrblk *)mymalloc(sizeof(axfrblk));                 00530000
 if (axfrfirst == NULL){                                                00540000
    axfrfirst = axfrptr;                                                00550000
    axfrcur = axfrptr;                                                  00560000
    axfrlast = axfrptr;                                                 00570000
    }                                                                   00580000
 else                                                                   00590000
   axfrlast->qnext = axfrptr;                                           00600000
 axfrlast = axfrptr;                                                    00610000
 axfrlast->qnext = ((struct axfrblk *)NULL);                            00620000
 strcpy(axfrlast->domain, domain);                                      00630000
 strcpy(axfrlast->remotens, remotens);                                  00640000
 header.reh_flags[0] = QUERY;                                           00650000
 header.reh_flags[0] = (header.reh_flags[0] << 3);                      00660000
 header.reh_flags[0] |= REH_RD;              /* recursion desired */    00670000
 ldoctet(header.reh_qdcount) = 1;            /* quest hdr count*/       00680000
                                                                        00690000
 strcpy(axfrlast->sqlt0, sqltable);                                     00700000
 strcat(axfrlast->sqlt0, "0");                                          00710000
 strcpy(axfrlast->sqlt1, sqltable);                                     00720000
 strcat(axfrlast->sqlt1, "1");                                          00730000
                                                                        00740000
 axfrlast->refresh = 0;                                                 00750000
 axfrlast->xfer = 1;                                                    00760000
                                                                        00770000
 hmaxfr++;                                                              00780000
}                                                                       00790000
                                                                        00800000
axfrtimer(axfrtimr,rdconn)                                              00810000
int axfrtimr;                                                           00820000
int *rdconn;                 /* recursion desired conn num    */        00830000
{                                                                       00840000
int rc = -1;                                                            00850000
int stor_avail;                    /* amount of virtual storage left*/  00860000
                                                                        00870000
/* Decrement the refresh period for each zone.                      */  00880000
 for (axfrptr = axfrfirst; axfrptr != NULL; axfrptr = axfrptr->qnext)   00890000
       axfrptr -> refresh -= axfrtimr;                                  00900000
                                                                        00910000
/* In round robin style find first zone needing refreshing          */  00920000
 if (axfrcur == NULL) axfrcur = axfrfirst;                              00930000
 for (axfrptr = axfrcur; axfrptr != NULL && rc != COK;                  00940000
      axfrptr = axfrptr->qnext)                                         00950000
   if (axfrptr ->refresh <= 0) {                                        00960000
     axfrptr->refresh = axfrptr->retry;  /* if request fails, retry */  00970000
     axfrptr->question = (struct qsection *)                            00980000
          mycalloc(1,sizeof(struct qsection));                          00990000
     axfrptr->question->qs_qname = (char *)                             01000000
          mymalloc(strlen(axfrptr->domain));                            01010000
     questions = axfrptr -> question;                                   01020000
     strcpy(axfrptr->question->qs_qname, axfrptr ->domain);             01030000
     if (axfrptr->xfer == 0)/* before transfering a zone get the SOA*/  01040000
         axfrptr->question->qs_qtype = TY_SOA;                          01050000
     else                                                               01060000
        axfrptr->question->qs_qtype = TY_AXFR;                          01070000
     axfrptr->question->qs_qclass = TY_A;                               01080000
     printquest(axfrptr->question);                                     01090000
     header.reh_flags[0] = QUERY;                                       01100000
     header.reh_flags[0] = (header.reh_flags[0] << 3);                  01110000
     header.reh_flags[0] |= REH_RD;       /* recursion desired */       01120000
     ldoctet(header.reh_qdcount) = 1;     /* quest hdr count*/          01130000
     stor_avail = qstor();            /* unused virtual storage     */  01140000
     if (stor_avail >= THRESHOLD) {                                     01150000
       *rdconn = doaopen(axfrptr->remotens);/* open conn to remote */   01160000
        if (rdconn < COK)                                               01170000
           printf("ns: couldn't open connection to remote ns.\n");      01180000
        else                                                            01190000
          rc = COK;                                                     01200000
     }                                                                  01210000
   }                                                                    01220000
  return(rc);                                                           01230000
}                                                                       01240000
                                                                        01250000
axfrprtq()                                                              01260000
{                                                                       01270000
 for (axfrptr = axfrfirst; axfrptr != NULL; axfrptr = axfrptr->qnext)   01280000
   printf("%s %s\n",axfrptr->domain,axfrptr->sqlt0);                    01290000
                                                                        01300000
}                                                                       01310000
                                                                        01320000
axfrrcv(conn, wholebuf, wholeptr, wholeend,state,zoneptr)               01330000
register int conn;      /* TCP or UDP connection number */              01340000
char *wholebuf;         /* Ptr to buffer containing part of message     01350000
                           read so far */                               01360000
char **wholeptr;                                                        01370000
                                                                        01380000
register char *wholeend;/* Pointer to end of wholebuf storage alloc */  01390000
int state;                                                              01400000
char **zoneptr;         /* Ptr to axfrblk: table name, current table */ 01410000
{                                                                       01420000
extern char *strtok();                                                  01430000
int axfrexit, rc;                                                       01440000
struct rrecord *rp;     /* The rrecord structure to fill in */          01450000
register struct qsection *qp;                                           01460000
int howarive;           /* 0 if via udp, 1 if via tcp, 2 if iucv   */   01470000
int ns_or_resolver;     /* 0 if called by resolver, 1 if name servr*/   01480000
int rrlength;                                                           01490000
char **p0;                                                              01500000
char  ocode;        /* opcode from header                           */  01510000
extern char hostcase??(5??); /* hostnamecase startup parm value     */  01520000
char sqltable[50] = "TCP";                                              01530000
char *token;                                                            01540000
int token_ctr;                                                          01550000
int serial;                                                             01560000
         axfrexit = 0;                                                  01570000
         howarive = 1;                                                  01580000
         ns_or_resolver = 1;                                            01590000
         ocode = INSERT;      /* indicate is inserting data   */        01600000
         qp = (struct qsection *) mymalloc(sizeof(struct qsection));    01610000
         rp = (struct rrecord *) mymalloc(sizeof(struct rrecord));      01620000
                                                                        01630000
        if (state == TY_AXFR)                                           01640000
          {                                                             01650000
           *wholeptr += 2;                                              01660000
           *wholeptr += sizeof(struct res_header);                      01670000
           if(readquest(conn, wholebuf, wholeptr,                       01680000
                wholeend, timeout, qp, asciitoebcdic,                   01690000
                howarive,ns_or_resolver) != COK) {                      01700000
              printf("Error in readquest\n");                           01710000
              }                                                         01720000
           for (axfrptr = axfrfirst; axfrptr != NULL;                   01730000
               axfrptr = axfrptr->qnext) {                              01740000
       if (DEBUG)                                                       01750000
          printf("----%s %s----\n",axfrptr->domain, qp->qs_qname);      01760000
               if (strcmp(axfrptr->domain, qp->qs_qname) == 0)          01770000
                 *zoneptr = (char *)axfrptr;                            01780000
               else                                                     01790000
                   printf("not equal\n");                               01800000
              }                                                         01810000
           printquest(qp);                                              01820000
                                                                        01830000
           axfrptr = (struct axfrblk *)*zoneptr;                        01840000
           if (axfrptr->current == 0)                                   01850000
              strcpy(sqltable,axfrptr->sqlt1);                          01860000
           else                                                         01870000
              strcpy(sqltable,axfrptr->sqlt0);                          01880000
                                                                        01890000
 /*      Get the SOA record for this zone                            */ 01900000
           if(readrr(conn, wholebuf, wholeptr,                          01910000
                  wholeend, timeout, rp, asciitoebcdic,                 01920000
                  howarive,ns_or_resolver) != COK) {                    01930000
               printf("Error in readrr\n");                             01940000
              }                                                         01950000
                                                                        01960000
 /*  If this is an SOA query then verify serial number               */ 01970000
           if (qp->qs_qtype == TY_SOA) {                                01980000
              for (token_ctr = 1, token = strtok(rp->rr_rdata," ,");    01990000
                  token != NULL, token_ctr != 3;                        02000000
                  token = strtok(NULL," ,"), token_ctr++);              02010000
              printf("token %s\n",token);                               02020000
              sscanf(token,"%d",&serial);                               02030000
           printf("token %d axfrptr serial %d",serial,axfrptr->serial); 02040000
              if (serial == axfrptr->serial) {                          02050000
                 printf("NSAXZONE: Resettng refresh value.\n");         02060000
                 token = strtok(NULL," ,");                             02070000
                 sscanf(token,"%d",&axfrptr->refresh);                  02080000
               }                                                        02090000
              else {                                                    02100000
                 axfrptr->xfer = 1;             /* New zone required */ 02110000
                 axfrptr->refresh = 0;  /* reset refresh value = now */ 02120000
              }                                                         02130000
              axfrexit = 1;                                             02140000
              return(axfrexit);                                         02150000
           }                                                            02160000
                                                                        02170000
 /*  This is an AXFR request, delete the table and insert the SOA RR */ 02180000
           ocode = DELETE;              /* delete sql table contents */ 02190000
           rc = nsbldcm(&ocode,                                         02200000
              questions, answers, auth, /* not used for this call   */  02210000
              addl, &p0, &hostcase,     /* not used for this call   */  02220000
              sqltable);                /* the list of tables       */  02230000
                                                                        02240000
         ocode = INSERT;      /* indicate is inserting data   */        02250000
           rc = nsbldcm(&ocode,                                         02260000
                   questions,   /* not used for this call       */      02270000
                   answers,     /* not used for this call       */      02280000
                   auth,        /* not used for this call       */      02290000
                   rp,          /* pointer to the SOA rr        */      02300000
                   &p0,         /* should not get any results   */      02310000
                   &hostcase,   /* not used for this call       */      02320000
                   sqltable);   /* sql table being used.        */      02330000
           if (rc != COK)                                               02340000
             {                  /* build of init sql cmnd failed*/      02350000
              printf("ns: Add resource record to db \n");               02360000
              printrr(rp);                                              02370000
             }                                                          02380000
                                                                        02390000
                                                                        02400000
         }/* if TY_AXFR */                                              02410000
                                                                        02420000
        axfrptr = (struct axfrblk *)*zoneptr;                           02430000
       if (axfrptr->current == 0)                                       02440000
          strcpy(sqltable,axfrptr->sqlt1);                              02450000
       else                                                             02460000
          strcpy(sqltable,axfrptr->sqlt0);                              02470000
                                                                        02480000
      rrlength = ldoctet(*wholeptr);                                    02490000
      while ((*wholeptr < wholeend - 1) && (axfrexit == 0)              02500000
            && (rrlength < (wholeend-*wholeptr-1))) {                   02510000
   /*  printf("rrlength %d %d\n",rrlength,wholeend -*wholeptr); */      02520000
       if (rrlength <= (wholeend - *wholeptr-1))/*rr wraps to next buf*/02530000
           {                                                            02540000
            *wholeptr += 2;                                             02550000
            *wholeptr += sizeof(struct res_header);                     02560000
            if ((rc = readrr(conn, wholebuf, wholeptr,                  02570000
                  wholeend, timeout, rp, asciitoebcdic,                 02580000
                  howarive,ns_or_resolver)) != COK) {                   02590000
               printf("Error in readrr\n");                             02600000
              }                                                         02610000
            if ((rp->rr_type == TY_SOA)                                 02620000
                && (strcmp(rp->rr_name, axfrptr->domain) == 0)){        02630000
               printf("Got final AXFR\n");                              02640000
               token = strtok(rp->rr_rdata," ,");                       02650000
               token_ctr = 1;                                           02660000
               while (token != NULL){                                   02670000
                   token = strtok(NULL," ,");                           02680000
                   switch (++token_ctr){                                02690000
                       case 3:                                          02700000
                           sscanf(token,"%d",&axfrptr->serial);         02710000
                           break;                                       02720000
                       case 4:                                          02730000
                           sscanf(token,"%d",&axfrptr->refresh);        02740000
                           break;                                       02750000
                       case 5:                                          02760000
                           sscanf(token,"%d",&axfrptr->retry);          02770000
                           break;                                       02780000
                       default:                                         02790000
                           break;                                       02800000
                      }                                                 02810000
               }                                                        02820000
              printf("New SOA: serial %d refresh %d\n",axfrptr->serial, 02830000
                        axfrptr->refresh);                              02840000
               axfrexit = 1;                                            02850000
               axfrptr = (struct axfrblk *)*zoneptr;                    02860000
               axfrptr->xfer = 0;                                       02870000
               ocode = STATS;        /* update statistics on table*/    02880000
               rc = nsbldcm(&ocode,                                     02890000
                  questions, answers, auth, addl, &p0, &hostcase,       02900000
                  sqltable);             /* the table name.          */ 02910000
               if (axfrptr->current == 0)      /* flip flop tables */   02920000
                 {                                                      02930000
                  axfrptr->current = 1 ;                                02940000
                 }                                                      02950000
               else                                                     02960000
                 {                                                      02970000
                  axfrptr->current = 0;                                 02980000
                 }                                                      02990000
              }                                                         03000000
            else                                                        03010000
              {                                                         03020000
               rc = nsbldcm(&ocode,                                     03030000
                   questions,   /* not used for this call       */      03040000
                   answers,     /* not used for this call       */      03050000
                   auth,        /* not used for this call       */      03060000
                   rp,          /* pointer to the resource rec  */      03070000
                   &p0,         /* should not get any results   */      03080000
                   &hostcase,   /* not used for this call       */      03090000
                   sqltable);   /* sql table being used.        */      03100000
               if (rc != COK){                                          03110000
                  printf("ns: Error adding resource record to db \n");  03120000
                  axfrexit = 1;                                         03130000
                 }                                                      03140000
             myfree(rp->rr_name);                                       03150000
             myfree(rp->rr_rdata);                                      03160000
            }                                                           03170000
           }/* end if */                                                03180000
        rrlength = ldoctet(*wholeptr);                                  03190000
        } /* end while */                                               03200000
       myfree(qp);                                                      03210000
       myfree(rp);                                                      03220000
       rc = 0;                                                          03230000
       if (axfrptr->xfer == 0)                                          03240000
            axfrview();                /*drop then create view */       03250000
       else                                                             03260000
       if (*wholeptr > wholeend) {                                      03270000
          axfrexit = 1;                                                 03280000
          printf("error wholeptr > wholeend\n");                        03290000
         }/* end if */                                                  03300000
       else                                                             03310000
       if (*wholeptr == wholeend){                                      03320000
          rc = 0;                                                       03330000
          printf("wholeptr == wholeend\n");                             03340000
       /* memcpy(wholebuf,*wholeptr,rc); */                             03350000
         }/* end if */                                                  03360000
       else                                                             03370000
       if (*wholeptr == wholeend-1){                                    03380000
          rc = 1;                                                       03390000
          printf("wholeptr == wholeend-1\n");                           03400000
          memcpy(wholebuf,*wholeptr,rc);                                03410000
         }/* end if */                                                  03420000
       else                                                             03430000
       if (rrlength != 0) {                                             03440000
          rc = wholeend - *wholeptr;                                    03450000
          memcpy(wholebuf,*wholeptr,rc);                                03460000
         }/* end if */                                                  03470000
       else                                                             03480000
          printf("axfrrcv else clause \n");                             03490000
                                                                        03500000
       *wholeptr = wholebuf + rc;                                       03510000
   return(axfrexit);                                                    03520000
}                                                                       03530000
                                                                        03540000
 axfrview()      /* drop and then create a view called tcpip */         03550000
{                                                                       03560000
char *tcpview;                                                          03570000
char sqltable[MAXTABLENAME] = "TCP";                                    03580000
int rc;                                                                 03590000
char  ocode;        /* opcode from header                           */  03600000
char **p0;                                                              03610000
int totalsize;                                                          03620000
                                                                        03630000
   ocode = DROP;                  /* delete tcp view              */    03640000
   rc = nsbldcm(&ocode,                                                 03650000
        questions, answers, auth, /* not used for this call       */    03660000
        addl, &p0, &hostcase,     /* not used for this call       */    03670000
        sqltable);               /* the list of tables           */     03680000
if (DEBUG)                                                              03690000
   printf("hmtcp %d \n",hmtcp);                                         03700000
if (hmtcp == 1){              /* The SQL TCP table exists           */  03710000
   totalsize = hmaxfr + 1;                                              03720000
   tcpview = (char *)mycalloc(totalsize,sizeof(MAXTABLENAME));          03730000
   strcpy(tcpview,"TCP ");                                              03740000
   if (axfrfirst ->current == 0)                                        03750000
      strcat(tcpview,axfrfirst->sqlt0);                                 03760000
   else                                                                 03770000
      strcat(tcpview,axfrfirst->sqlt1);                                 03780000
}                                                                       03790000
else{                                                                   03800000
   tcpview = (char *)mycalloc(hmaxfr,sizeof(MAXTABLENAME));             03810000
   if (axfrfirst ->current == 0)                                        03820000
      strcpy(tcpview,axfrfirst->sqlt0);                                 03830000
   else                                                                 03840000
      strcpy(tcpview,axfrfirst->sqlt1);                                 03850000
}                                                                       03860000
                                                                        03870000
for (axfrptr =axfrfirst->qnext;axfrptr!=NULL; axfrptr = axfrptr->qnext) 03880000
    {                                                                   03890000
     if (axfrptr ->current == 0)                                        03900000
        strcat(tcpview,axfrptr->sqlt0);                                 03910000
     else                                                               03920000
        strcat(tcpview,axfrptr->sqlt1);                                 03930000
     }                                                                  03940000
                                                                        03950000
   if (DEBUG)                                                           03960000
      printf("create tcpview: %s \n",tcpview);                          03970000
   ocode = CREATE;                /* create tcp view              */    03980000
   rc = nsbldcm(&ocode,                                                 03990000
        questions, answers, auth, /* not used for this call       */    04000000
        addl, &p0, &hostcase,     /* not used for this call       */    04010000
        tcpview);                 /* the list of tables           */    04020000
 if (DEBUG)                                                             04030000
    printf("after create view \n");                                     04040000
                                                                        04050000
 myfree(tcpview);                                                       04060000
 if (DEBUG)                                                             04070000
   printf("after freeing tcpview \n");                                  04080000
                                                                        04090000
}                                                                       04100000
                                                                        04110000
 axfrvalid()               /* Validate the AXFR domains + tables */     04120000
{                                                                       04130000
int rc, length;                                                         04140000
char *token;                                                            04150000
char *datapt;                                                           04160000
int hmresult;   /* stor number of resulting records from sql table */   04170000
char  ocode;        /* opcode from header                           */  04180000
char **p0, **sqlptr;                                                    04190000
char table[MAXTABLENAME];                                               04200000
                                                                        04210000
rc = COK;                                                               04220000
for (axfrptr = axfrfirst; axfrptr != NULL; axfrptr = axfrptr->qnext)    04230000
     if (strcmp(axfrptr->remotens,"") == 0) {                           04240000
      printf("Undefined remote NS for zone %s\n",axfrptr->domain);      04250000
      rc = CABNORMALcondition;                                          04260000
     }                                                                  04270000
for (axfrptr = axfrfirst; axfrptr != NULL; axfrptr = axfrptr->qnext)    04280000
  for (axfrlp  = axfrptr->qnext; axfrlp!= NULL; axfrlp = axfrlp->qnext) 04290000
   if (strcmp(axfrptr->sqlt0,axfrlp->sqlt0) == 0) {                     04300000
      printf("Duplicate SQL table name %s\n",axfrptr->sqlt0);           04310000
      rc = CABNORMALcondition;                                          04320000
     }                                                                  04330000
                                                                        04340000
/* Determine if a TCP table exists.                               */    04350000
   strcpy(table,"TCP");                                                 04360000
   ocode = EXIST;                 /* Does TCP table exist?        */    04370000
   rc = nsbldcm(&ocode,                                                 04380000
        questions, answers, auth, /* not used for this call       */    04390000
        addl, &p0, &hostcase,     /* not used for this call       */    04400000
        table);                   /* not used for this call       */    04410000
   if (rc != COK)                                                       04420000
       printf("ns: Can't get the TCP view rc = %d \n",rc);              04430000
                                                                        04440000
    sqlptr = p0;              /* so can be freed later          */      04450000
    hmtcp = countem(p0);      /* count num of ptrs in ptrs buf  */      04460000
    if (hmtcp != 0)                                                     04470000
      inter(nsdbsql,&xff,&sqlptr); /* free sql buffers           */     04480000
                                                                        04490000
/* Get the list of tables which make up the view tcp.             */    04500000
   ocode = TCPVIEW;               /* get the tcp view command     */    04510000
   rc = nsbldcm(&ocode,                                                 04520000
        questions, answers, auth, /* not used for this call       */    04530000
        addl, &p0, &hostcase,     /* not used for this call       */    04540000
        table);                   /* not used for this call       */    04550000
   if (rc != COK)                                                       04560000
       printf("ns: Can't get the TCP view rc = %d \n",rc);              04570000
                                                                        04580000
    sqlptr = p0;        /* so can be freed later          */            04590000
    hmresult = countem(p0);   /* count num of ptrs in ptrs buf  */      04600000
    p0 = sqlptr;                                                        04610000
    if (hmresult == 0){        /*  TCPIP view not defined        */     04620000
       if (hmtcp != 0){        /* IF TCP table exists, create view */   04630000
          strcpy(table,"TCP");                                          04640000
          ocode = CREATE;        /* create tcp view              */     04650000
          rc = nsbldcm(&ocode,                                          04660000
            questions, answers, auth, /* not used for this call       */04670000
            addl, &p0, &hostcase,     /* not used for this call       */04680000
            table);                   /* the name TCP                 */04690000
          inter(nsdbsql,&xff,&sqlptr);  /* free sql buffers           */04700000
        }                                                               04710000
    }                                                                   04720000
    else {                                                              04730000
      p0++;                                                             04740000
      datapt = *p0+4;               /* length preceeds data        */   04750000
      printf("viewtext %s\n",datapt);                                   04760000
                                                                        04770000
      token = strtok(datapt," ;");   /* parse the viewtext          */  04780000
      while (token != NULL) {       /* Initialize CURRENT          */   04790000
         for (axfrptr = axfrfirst; axfrptr != NULL;                     04800000
               axfrptr = axfrptr->qnext)                                04810000
            if (strcmp(token,axfrptr->sqlt0) == 0)                      04820000
               axfrptr->current = 0;                                    04830000
            else                                                        04840000
            if (strcmp(token,axfrptr->sqlt1) == 0)                      04850000
               axfrptr->current = 1;                                    04860000
        token = strtok(NULL," ;");                                      04870000
      }                                                                 04880000
                                                                        04890000
    inter(nsdbsql,&xff,&sqlptr);  /* free sql buffers           */      04900000
 }                                                                      04910000
 return(rc);                                                            04920000
}                                                                       04930000
                                                                        04940000
                                                                        04950000
 checkview()            /* Running with SQL make sure view defined */   04960000
{                                                                       04970000
int rc;                                                                 04980000
int hmresult;   /* stor number of resulting records from sql table */   04990000
char  ocode;        /* opcode from header                           */  05000000
char **p0, **sqlptr;                                                    05010000
char tcpview[MAXTABLENAME];                                             05020000
                                                                        05030000
/* Determine if a TCPIP view exists.                               */   05040000
   strcpy(tcpview,"TCPIP");                                             05050000
   ocode = EXIST;                 /* Does TCP table exist?        */    05060000
   rc = nsbldcm(&ocode,                                                 05070000
        questions, answers, auth, /* not used for this call       */    05080000
        addl, &p0, &hostcase,     /* not used for this call       */    05090000
        tcpview);                 /* determine if view defined.   */    05100000
   if (rc != COK)                                                       05110000
       printf("ns: Can't get the TCPIP view rc = %d \n",rc);            05120000
   else {                                                               05130000
     sqlptr = p0;                 /* so can be freed later          */  05140000
     hmresult = countem(p0);      /* count num of ptrs in ptrs buf  */  05150000
     if (DEBUG)                                                         05160000
        printf("hmresult = %d\n",hmresult);                             05170000
   }                                                                    05180000
   if (hmresult != 0)             /* TCPIP view defined             */  05190000
     return(COK);                                                       05200000
   else {                         /* No TCPIP view defined          */  05210000
     strcpy(tcpview,"TCP");       /* Determine if a TCP table exists.*/ 05220000
     rc = nsbldcm(&ocode,                                               05230000
        questions, answers, auth, /* not used for this call       */    05240000
        addl, &p0, &hostcase,     /* not used for this call       */    05250000
        tcpview);                 /* determine if view defined.   */    05260000
     if (rc != COK)                                                     05270000
         printf("ns: Can't get the TCP table rc = %d \n",rc);           05280000
     else {                                                             05290000
       sqlptr = p0;               /* so can be freed later          */  05300000
       hmresult = countem(p0);      /* count num of ptrs in ptrs buf  */05310000
       if (DEBUG)                                                       05320000
          printf("hmresult = %d\n",hmresult);                           05330000
     }                                                                  05340000
   }                                                                    05350000
   if ((hmresult == 0) &&          /* No TCPIP view + No TCP table   */ 05360000
       (axfrfirst == NULL)) {                                           05370000
       printf("ns: TCP table not defined; No zone defined.\n");         05380000
       rc = -1;                                                         05390000
   }                                                                    05400000
   else {                                                               05410000
     if (hmresult != 0)             /* No TCPIP view + No TCP table   */05420000
        strcpy(tcpview,"TCP");                                          05430000
     else                                                               05440000
        strcpy(tcpview,axfrfirst->sqlt0);                               05450000
     ocode = CREATE;                /* create tcp view              */  05460000
     rc = nsbldcm(&ocode,                                               05470000
          questions, answers, auth, /* not used for this call       */  05480000
          addl, &p0, &hostcase,     /* not used for this call       */  05490000
          tcpview);                 /* the list of tables           */  05500000
   }                                                                    05510000
}                                                                       05520000
                                                                        05530000
                                                                        05540000
                                                                        05550000
 axfrfreeq()            /* Release all storage associated with AXFR*/   05560000
{                                                                       05570000
 for (axfrptr = axfrfirst; axfrptr != NULL; axfrptr = axfrfirst)        05580000
     {                                                                  05590000
        myfree(axfrptr->question->qs_qname);                            05600000
        myfree(axfrptr -> question);                                    05610000
        axfrfirst = axfrptr -> qnext;                                   05620000
        myfree(axfrptr);                                                05630000
     }                                                                  05640000
}                                                                       05650000
                                                                        05660000
/* Return the table name to query                             */        05670000
axfrqtable(dname,tname)                                                 05680000
char *dname;                    /* domain name                */        05690000
char *tname;                    /* table name                 */        05700000
{                                                                       05710000
int pos;                                                                05720000
int savpos;                                                             05730000
                                                                        05740000
 savpos = 0;                                                            05750000
                                                                        05760000
 printf("axfrqtable %s \n",dname);                                      05770000
 strcpy(tname,"TCP");          /* default is the original tcp table */  05780000
 for (axfrptr = axfrfirst; axfrptr != NULL; axfrptr = axfrptr->qnext){  05790000
    pos = strindx(dname,axfrptr->domain);                               05800000
    if (((savpos ==0) && (pos > 0)) ||                                  05810000
        ((pos < savpos) && (pos > 0))){                                 05820000
       if (axfrptr ->current == 0)                                      05830000
          strcpy(tname,axfrptr->sqlt0);                                 05840000
       else                                                             05850000
          strcpy(tname,axfrptr->sqlt1);                                 05860000
       savpos = pos;                                                    05870000
    }/* endif */                                                        05880000
 printf("axfrqtable %s %s \n",dname,tname);                             05890000
 }/* end for */                                                         05900000
}                                                                       05910000
/* Return the starting position of string2 in string1         */        05920000
strindx(str1,str2)                                                      05930000
char *str1;    /* scan string */                                        05940000
char *str2;    /* search object */                                      05950000
{                                                                       05960000
int ictr;                                                               05970000
char *tmp1;                                                             05980000
int found = 0;                                                          05990000
                                                                        06000000
ictr = 0;               /* substring not found */                       06010000
                                                                        06020000
if (strlen(str1) < strlen(str2))                                        06030000
    return(ictr);                                                       06040000
                                                                        06050000
for (tmp1 = str1, ictr= 1, found = 0; tmp1 != NULL && !found;           06060000
          ictr++,*tmp1++){                                              06070000
     printf("tmp1 %s str2 %s\n",tmp1,str2);                             06080000
    if (strcmp(tmp1,str2) == 0)                                         06090000
       found = 1;                                                       06100000
   }                                                                    06110000
return(ictr);                                                           06120000
}                                                                       06130000
