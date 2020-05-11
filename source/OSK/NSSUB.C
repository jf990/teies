/*********************************************************************/ 00010000
/*   THIS PRODUCT CONTAINS "RESTRICTED MATERIALS OF IBM"             */ 00020000
/*   5798-FAL (C) COPYRIGHT IBM CORPORATION 1987, 1988               */ 00030000
/*   ALL RIGHTS RESERVED                                             */ 00040000
/*   LICENSED MATERIALS - PROPERTY OF IBM                            */ 00050000
/*   SEE COPYRIGHT INSTRUCTIONS, G120-2083                           */ 00060000
/*********************************************************************/ 00070000
                                                                        00080000
#include <stdio.h>                                                      00090000
#include <ctype.h>                                                      00100000
#include "cmc2wis.h"                                                    00110000
#include "cmolver.h"                                                    00120000
                                                                        00130000
#define NSINIT 99            /* name server init entry to nsbldcm   */  00140000
                                                                        00150000
extern nsdbsql();            /* access sql data base               */   00160000
extern prtmsg();             /* print the message                  */   00170000
extern int DEBUG;                                                       00180000
extern char NULL;   /* defined in cmsysde c                       */    00190000
extern char xff;   /* tell nsdbsql to free the sql ptr and data bfr*/   00200000
extern char *ns003;                                                     00210000
extern char *msgptr;         /* pointer to the message text        */   00220000
extern int textlen;          /* length of the message text         */   00230000
extern char *soans;             /* array of rr soa, ns records     */   00240000
extern struct rrecord *soanspt; /* soa, ns rr struct array ptr     */   00250000
                                                                        00260000
/* We will go to the nsbldcm() pgm which will build and         */      00270000
/* execute the initilize sql command. That command is...        */      00280000
/*   SELECT * FROM TCP WHERE (TYPE=2 OR TYPE=6) ORDER BY        */      00290000
/*   NAME ASC, TYPE ASC                                         */      00300000
/*                                                              */      00310000
soansinit(hostcase)                                                     00320000
  char *hostcase;                                                       00330000
{                                                                       00340000
int hmresult;    /* num of soa, ns recds returned from sql fetch */     00350000
char  ocode;        /* opcode from header                           */  00360000
int rc;                            /* subroutine return code        */  00370000
char **pt;                                                              00380000
char **p0;                                                              00390000
char **initptr;     /* ptr to soa, ns recds from init attempt       */  00400000
char *datapt;                                                           00410000
                                                                        00420000
short namelen;      /* length of the qname field from sql fetch     */  00430000
short datalen;      /* length of the rdata field from sql fetch     */  00440000
short length;                                                           00450000
short type;         /* type                                         */  00460000
short class;        /* class                                        */  00470000
                                                                        00480000
char *questions;    /* array of questions structure                 */  00490000
char *answers;      /* array of answers structure                   */  00500000
char *auth;         /* array of authority sturcture                 */  00510000
char *addl;         /* array of additional structures               */  00520000
                                                                        00530000
     ocode = NSINIT;         /* indicate is initilize entry     */      00540000
     rc = nsbldcm(&ocode,                                               00550000
                   questions,   /* not used for this call       */      00560000
                   answers,     /* not used for this call       */      00570000
                   auth,        /* not used for this call       */      00580000
                   addl,        /* not used for this call       */      00590000
                   &p0,         /* where to put ptr to ptrs bfr */      00600000
                   &hostcase);  /* not used for this call       */      00610000
    if (rc != COK)                                                      00620000
       {                        /* build of init sql cmnd failed*/      00630000
       printf("ns: Build of initilize sql cmnd rc = %d \n");            00640000
       fflush(stdout);                                                  00650000
/* Must quit here since can't fetch the authority recds         */      00660000
       return(rc);              /* error in name server         */      00670000
       }                        /* build of init sql comnd faild*/      00680000
    initptr = p0;               /* so we may free it later      */      00690000
    hmresult = countem(p0);     /* count num of ptrs in ptr buf */      00700000
    if(DEBUG)                                                           00710000
        printf("hmresult = %d\n",hmresult);                             00720000
/*  The number of pointers in the data buffer exists in hmresult*/      00730000
    if (hmresult == 0)          /* was nothing fetched           */     00740000
       {                        /* begin nothing was fetched     */     00750000
 /*    msgptr = ns003;  */      /* point to the message text     */     00760000
 /*    textlen = strlen(ns003)*//* length of the msg             */     00770000
 /*    prtmsg(msgptr,textlen,DEBUG);*/                                  00780000
       printf("NS: No SOA or NS records found.\n");                     00790000
       return(hmresult);        /* finished                      */     00800000
       }                        /* end nothing was fetched       */     00810000
/* All the SOA and NS records have been fetched. We will now     */     00820000
/* place the recds in a structure for future refrence.           */     00830000
      soans = (char *)mycalloc(hmresult,sizeof(struct rrecord));        00840000
      soanspt = (struct rrecord *)soans;   /* array of rrd struct*/     00850000
     for(;;)                                                            00860000
     {                            /* for all pointers            */     00870000
     for (pt=p0+1;*pt;pt++,soanspt++)                                   00880000
     {                                                                  00890000
/* This length is the total length. If a column was defined as   */     00900000
/* varchar in the sql db then an additional length preceeds that */     00910000
/* field.                                                        */     00920000
      length = *((short *)*pt);   /* total length of data fetched*/     00930000
      datapt = *pt+2;             /* length preceeds data        */     00940000
/*  Place the data from the SQL fetch into the rrecord structure */     00950000
/* This length is the length of the varchar column qname.        */     00960000
      cmwmove(&namelen,datapt,2); /* length of the qname field   */     00970000
      nullit(datapt+2,&soanspt->rr_name,namelen); /* add null trm*/     00980000
      if (strncmp(soanspt->rr_name,"..",2) == 0)                        00990000
         nullit("*",&soanspt->rr_name,1);                               01000000
      datapt += namelen + 2;      /* point to the type field     */     01010000
      cmwmove(&type,datapt,2);    /*                             */     01020000
      soanspt->rr_type = type;    /* from short to int in rr stru*/     01030000
      datapt += 2;                /* point to the class field    */     01040000
      cmwmove(&class,datapt,2);   /*                             */     01050000
      soanspt->rr_class = class;  /* from short to int in rr stru*/     01060000
      datapt += 2;                /* point to the ttl field      */     01070000
      cmwmove(&soanspt->rr_ttl,datapt,4); /* ttl is an integer   */     01080000
      datapt += 4;                /* length of rdata is next     */     01090000
      cmwmove(&datalen,datapt,2); /* length of the rdata field   */     01100000
      nullit(datapt+2,&soanspt->rr_rdata,datalen); /*add null ter*/     01110000
     }                                                                  01120000
     if (*p0 == 0) break;         /* zero says no more buffers   */     01130000
     p0 = (char **)*p0;           /* next buffer of pointers     */     01140000
     }                            /* for all pointers            */     01150000
soafini:                                                                01160000
/* Go back to the pgm that did the sql fetch to free the buffers */     01170000
/* acquired for our fetch.                                       */     01180000
    inter(nsdbsql,&xff,&initptr); /* free sql buffers            */     01190000
    return(hmresult);                                                   01200000
}                                                                       01210000
