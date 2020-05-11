/*********************************************************************/
/*   THIS PRODUCT CONTAINS "RESTRICTED MATERIALS OF IBM"             */
/*   5798-FAL (C) COPYRIGHT IBM CORPORATION 1987, 1988               */
/*   ALL RIGHTS RESERVED                                             */
/*   LICENSED MATERIALS - PROPERTY OF IBM                            */
/*   SEE COPYRIGHT INSTRUCTIONS, G120-2083                           */
/*********************************************************************/

#include <XlibInt.h>
#include <stdio.h>
#include <cmallcl.h>
#include <socket.h>
#include <select.h>
#include <strings.h>
#include <un.h>
#include <uio.h>

#define WAITforever 60            /* Timeout waiting for connection */
#define ALLnotes 0xffffffff       /* Used in handle note request    */
#define MAXTCP    8192            /* Maximum data xfer/tcp call     */
#define MAXSOCK     32            /* Only support 32 sockets        */
#define CONN_AVAIL  -1            /* To mark connection available   */
#define IUCV_BSIZE  40            /* Size of iucv buffer (getnextnote) */
#define XLATE_FILENAME    "STANDARD TCPXLBIN"
#define XLATE_HEADER      "*TCP/IP translate tables"


#define onexit_t _onext

static BeginTCP();
static TCPmsg();
static ReadXlate();
static clear_reads();
char   *getenv();
static onexit_t Close_Socks();

/*
**  TCPIP for VM requires us to use an environment number for most
**  calls. The following int provides a place to keep that.
*/
int TCP_env = 0;                  /* TCPIP environment number   */
/*
** This array will hold TCPIP connection numbers and connection
** status information, such as socket type, etc.
**
** For the application program, we will supply an index into
** this array as the "socket" number.
*/
static struct {
       int  number;          /* TCPIP for VM connection number  */
       int  type;            /* Either SOCK_STREAM or DGRAM     */
       struct sockaddr_in to;/* Save connect values here        */
       int  tolen;           /* Save connect tolen value here   */
       int  foraddr;         /* For UDP select() temp values    */
       int  forport;
       int  datalen;
       int  read_status;     /* Values defined below            */
       union {
          char buf[4];       /* lookaside buffer for TCP select */
          char *bp;          /* buffer pointer for UDP select   */
       } b;
} connection[MAXSOCK];

/*
** read_status field values
*/
#define READ_NONE        0
#define READ_OUTSTANDING 1
#define READ_DONE        2

static int    glue_fds = 0;       /* Number descriptors in use      */
static struct hostent  cms_host;  /* structure for gethostX() calls */
static int    cms_IPaddr;         /* static for IP address          */
static char   cms_IPname[256];    /* static for IP name             */
static onexit_t TCP_onexit = NULL;/* Save on exit value here        */
/*
** Char pointers in haddrs because we cannot assume net address
** fits in an int, although everyone does assume this...
*/
static char   *cms_haddrs[2];     /* static for return to caller    */

static int Sock_Debug = 0;

socket(af, type, protocol)
int af, type, protocol;
{
    int  i;

    if (af != AF_INET)
       return(-1);  /* We can only talk to internet address family */
    if ( (type != SOCK_STREAM) && (type != SOCK_DGRAM) )
       return(-1);  /* We only support TCP and UDP connections     */
    if (TCP_env == 0)
       BeginTCP();
/*
** Search for an unused connection
*/
    for (i = 0;(connection[i].number != CONN_AVAIL) && (i < MAXSOCK);++i)
        ;
    if (i == MAXSOCK)   /* No more sockets available            */
       return(-1);
    ++glue_fds;         /* Handed out another "file descriptor' */
    connection[i].type = type;  /* Record type of connection    */
    connection[i].read_status = READ_NONE; /* And no buffered rd*/
    connection[i].b.bp = NULL;  /* No lookaside buffer          */
    return(i);          /* Pass back a socket handle            */
}

connect(s, name, namelen)
int s;
struct sockaddr_in *name;
int namelen;
{
    int  connstat = ACTIVE;
    int  timeout  = WAITforever;
    int  connptr  = 0;
    int  lcladdr, lclport, rmtaddr, rmtport;
    int  retcode;

    if (connection[s].type == SOCK_STREAM) {  /* TCP type socket */
       lcladdr = UNSPECIFIEDaddress;
       lclport = UNSPECIFIEDport;
       rmtaddr = name->sin_addr.s_addr;
       rmtport = name->sin_port;
       if (Sock_Debug) {
        fprintf(stderr, "connect: parms: s: %d namelen: %d\n",
                s, namelen);
        fprintf(stderr, "  sockaddr: sin_family: %d\n", name->sin_family);
        fprintf(stderr, "  sockaddr: sin_port: %d\n", rmtport);
        fprintf(stderr, "  sockaddr: sin_addr: %04x\n", rmtaddr);
        fprintf(stderr, "  connect: Trying to open TCPIP connection.\n");
       }
       inter(ctcpwatopen, &TCP_env, &connstat, &timeout,
                   &lcladdr, &lclport, &rmtaddr, &rmtport,
                   &connptr, &connection[s].number, &retcode);
       if (retcode) {
           connection[s].number = CONN_AVAIL;
           if (Sock_Debug)
              fprintf(stderr, "connect, watopen retcode = %d\n", retcode);
           if (retcode == TIMEOUTopen)
              return(-1);
           else
              TCPmsg(retcode);     /* Display msg if failure */
       }
    } else {       /* SOCK_DGRAM */
       memcpy(&connection[s].to, name, namelen);
       connection[s].tolen = namelen;
    }

    return(0);
}

inet_addr(cp)
char *cp;
{
    int  retcode;
    int  len;
    char buf[256];

    if (TCP_env == 0)
       BeginTCP();
    strcpy(buf, cp);
    len = strlen(cp);
    inter(chostnumber, &TCP_env, buf, &len, &retcode);

    if (Sock_Debug) {
       fprintf(stderr, "inet_addr returning %04x as net address\n",
                retcode);
    }

    return(retcode);
}

struct hostent *gethostbyname(name)
char *name;
{
    int  i;

    if (TCP_env == 0)
       BeginTCP();
    i = strlen(name);
/*
**  Get the IP number corresponding to the name
*/
    inter(chostnumber, &TCP_env, name, &i, &cms_IPaddr);
    if (cms_IPaddr) {
/*
**  Now get the official name corresponding to the number
*/
         inter(choststring, &TCP_env, &cms_IPaddr, &cms_IPname, &i);
         cms_host.h_name = cms_IPname;
         cms_host.h_aliases = NULL;
         cms_host.h_addrtype = AF_INET;
         cms_host.h_length = 4;
         cms_host.h_addr_list = cms_haddrs;
         cms_haddrs[0] = (char *) &cms_IPaddr;
         cms_haddrs[1] = (char *) 0;
         return(&cms_host);
    } else {
       return(NULL);
    }
}

static BeginTCP()
{
    int  retcode;
    int  i, len;
    int  notemask = ALLnotes;
    char tcpip[8];
    char *lcp;

/*
**  Check to see if we should talk to different TCPIP VM.
*/
    if ((lcp = getenv("TCPIP")) != NULL) {
       memset(tcpip, ' ', sizeof(tcpip));
       strncpy(tcpip, lcp, (len = strlen(lcp)) > 8 ? 8 : len);
       inter(ctcpnamechange, &TCP_env, tcpip);
    }
    inter(cbegintcpip, &TCP_env, &retcode);
    if (retcode)
       TCPmsg(retcode);     /* Display msg if failure         */
    if (Sock_Debug)
       fprintf(stderr, "BeginTCP: Beginning tcpip...env: %d\n", TCP_env);
    inter(chandle, &TCP_env, &notemask, &retcode);
    if (retcode)
       TCPmsg(retcode);     /* Display msg if failure         */
    ReadXlate();            /* Read ASCII/EBCDIC xlate tables */
    for (i = 0; i < MAXSOCK; ++i)
         connection[i].number = CONN_AVAIL;   /* Mark not in use      */
    if (TCP_onexit == NULL) {
       TCP_onexit = (onexit_t) onexit(Close_Socks);
    }
    return;
}

static onexit_t Close_Socks()
{
    int  i;

    if (TCP_env) {   /* If the environment still seems active */
       for (i = 0; i < MAXSOCK; ++i) {
           if (connection[i].number != CONN_AVAIL)
              close(i);         /* Close down the connections    */
       }
    }
    return(TCP_onexit);      /* Return next function in chain */
}

close(fd)
int fd;
{
    int  retcode = 0;

    if (Sock_Debug) {
       fprintf(stderr, "close: socket: %d number %d\n",
                        fd, connection[fd].number);
    }
    if (connection[fd].number != CONN_AVAIL) {
       if (connection[fd].type == SOCK_STREAM) {
          inter(ctcpclose, &TCP_env, &connection[fd].number, &retcode);
       } else if (connection[fd].type == SOCK_DGRAM) {
          inter(cudpclose, &TCP_env, &connection[fd].number, &retcode);
          if (connection[fd].b.bp != NULL) {
             free(connection[fd].b.bp);
          }
          connection[fd].b.bp = NULL;
       } else {
          return(-1);
       }
    }
    if (retcode)
       TCPmsg(retcode);             /* Display msg if failure          */
    connection[fd].number = CONN_AVAIL;/* Indicate connection available*/
    if (--glue_fds == 0) {  /* No more file descriptors in use */
       inter(cendtcpip, &TCP_env);
       TCP_env = 0;
    }
    return(0);
}


read(fd, cp, len)
int fd;
char *cp;
int len;
{
    int  bytesread, retcode;
    int  i, tlen;
    char *ncp;

    if (Sock_Debug) {
       fprintf(stderr, "read: socket: %d, buffer: %04x, len: %d\n",
               fd, cp, len);
       fprintf(stderr, "read_status: %d\n", connection[fd].read_status);
    }
    ncp = cp;                              /* Set up read pointer */
    tlen = len;                            /* Set up read length  */
    if (connection[fd].read_status == READ_OUTSTANDING) {
       if ((retcode = clear_reads(1, fd)) < 0)
          return(-1);        /* I/O Error on socket */
    }
    if (connection[fd].read_status == READ_DONE) {
         *cp = connection[fd].b.buf[0];
         ++ncp;                            /* One byte read, and   */
         --tlen;                           /* ... one less to read */
         connection[fd].read_status = READ_NONE; /* Reset status */
    }
    inter(ctcpwatreceive, &TCP_env, &connection[fd].number, &ncp, &tlen,
             &bytesread, &retcode);
    if (retcode) {
       return(0);                 /* Indicate close of this socket   */
    }
    if (ncp != cp) ++bytesread;   /* Read one byte thru select logic */
    if (Sock_Debug)
       fprintf(stderr, "read: bytesread %d\n", bytesread);
    return(bytesread);
}

write(fd, cp, len)
int fd;
char *cp;
int len;
{
    int  push, urgent, retcode;
    int  sublen, templen;

    push   = 1;         /* Not really URGENT to get it there */
    urgent = 0;         /* Not really URGENT to get it there */
    if (Sock_Debug) {
       fprintf(stderr,
              "write: socket: %d, buffer: %04x, len: %d, opode: %d\n",
              fd, cp, len, *cp);
    }
    templen = len;
    while (templen) {
       sublen = (templen >= MAXTCP) ? MAXTCP : templen;
       do {
          inter(ctcpsend, &TCP_env, &connection[fd].number, &cp, &sublen,
                &push, &urgent, &retcode);
       } while (retcode == NObufferSPACE);
       if (retcode) {
          errno = EPIPE;       /* And the connection is broken */
          return(-1);          /* Indicate I/O error to caller */
       }
       templen -= sublen;
       cp += sublen;
    }
    return(len);
}

ioctl(fd, rqst, bytes)
int fd;
int rqst;
int *bytes;
{
    int  timeout, unacked, connstat;
    int  lcladdr, lclport, rmtaddr, rmtport;
    int  retcode;

    if (rqst != FIONREAD)    /* We only handle FIONREAD ioctl */
       return(-1);
    timeout = WAITforever;
    if (Sock_Debug)
       fprintf(stderr, "ioctl: socket: %d, rqst: %d\n", fd, rqst);
    if (connection[fd].read_status == READ_OUTSTANDING) {
       if ((retcode = clear_reads(0, -1)) < 0)
          return(-1);        /* I/O Error on socket */
    }
    inter(ctcpstatus, &TCP_env, &connection[fd].number, &timeout, bytes,
              &unacked, &connstat,
              &lcladdr, &lclport, &rmtaddr, &rmtport,
              &retcode);
    if (retcode) {
       errno = EPIPE;       /* And the connection is broken     */
       return(-1);          /* Return with I/O error indication */
    }
    if (connection[fd].read_status == READ_DONE)
         ++*bytes;
    if (Sock_Debug)
       fprintf(stderr, "ioctl: btoread: %d\n", *bytes);

    return(0);
}

select(numfds, readfds, writefds, excp, tv)
int  numfds;
int *readfds;
int *writefds;
int *excp;
struct timeval *tv;
{
    int  i;
    int  num = 0;
    int  newfds = 0;
    int  mask;
    int  timerpt;
    int  notemask = TIMERexpired;
    int  retcode;
    int  wait = 1;
    int  stayinloop = 1;
    int  waittime;
    int  notetype;
    int  con_num;
    int  urgbytes, urgspan;
    int  bytesdel, lasturg, push, amtspc, newstat, reas, datm;
    int  assctmr, rupt, datalen, foraddr, forport;
    char iucv_buffer[IUCV_BSIZE];
    char *iucvbf = iucv_buffer;

    int timeout, btoread, unacked, connstat;
    int lcladdr, lclport, rmtaddr, rmtport;
    int statusrc;
    char *bp;

    if ( (tv != NULL)  &&
         (tv->tv_sec == 0) && (tv->tv_usec == 0) ) {   /* Poll */
         mask = 0x01;
         for (i = 0; i < numfds; ++i) {
             if (mask & *readfds) {
                inter(ctcpstatus, &TCP_env, &connection[i].number,
                      &timeout, &btoread, &unacked, &connstat,
                      &lcladdr, &lclport, &rmtaddr, &rmtport, &statusrc);
                if (btoread) {
                   ++num;
                   newfds |= mask;
                }
             }
             mask <<= 1;
         }
    } else {                          /* Timed select or wait forever */
/*
** For TCP:
** What we will do here is set a timer as requested
** and then do 1 byte tcpreceives. When TCPIP has data
** available for one of the connections with a one byte
** read, it will send a DATAdelievered note to us, as all
** of the data we requested has then been received.
**
** For UDP:
** A similar strategy is performed, except we need to read the
** entire datagram and acquire a temp buffer for that purpose.
**
** Then we loop, waiting for notes. If the timer pops, it is
** obvious what to do... return to caller...
** If we get a DATAdelievered note, then fix up the readfds
** mask and return to the caller.
*/
         if (tv == NULL)
              waittime = 999999;
         else
              waittime = tv->tv_sec;
         if (Sock_Debug)
            fprintf(stderr, "Select: waittime = %d\n", waittime);
         inter(ccreatetimer, &TCP_env, &timerpt);
         inter(csettimer, &TCP_env, &timerpt, &waittime, &waittime);
         mask = 0x01;
         datalen = 1;        /* Read ONE byte */
         for (i = 0; i < numfds; ++i) {
             if (mask & *readfds) {  /* Put out a one byte read */
                if (connection[i].type == SOCK_STREAM) {
                   bp = connection[i].b.buf;
                   inter(ctcpreceive, &TCP_env, &connection[i].number,
                         &bp, &datalen, &retcode);
                } else {
                   if (connection[i].b.bp == NULL) { /* Have buf yet  */
                      connection[i].b.bp = malloc(MAXTCP);
                      if (connection[i].b.bp == NULL) /* Still no?    */
                         TCPmsg(VIRTUALmemoryTOOsmall);
                      inter(cudpreceive, &TCP_env, &connection[i].number,
                            &connection[i].b.bp, &retcode);
                   }
                   inter(ctcpreceive, &TCP_env, &connection[i].number,
                         &bp, &datalen, &retcode);
                }
                connection[i].read_status = READ_OUTSTANDING;
                if (Sock_Debug) {
                   fprintf(stderr,
                   "Set up read on connection number %d, retcode = %d\n",
                   i, retcode);
                }
             }
             mask <<= 1;
         }
         while (stayinloop) {
            inter(cgetnextnote, &TCP_env, &wait, &notetype,
                  &con_num, &urgbytes, &urgspan, &bytesdel,
                  &lasturg, &push, &amtspc, &newstat, &reas, &datm,
                  &assctmr, &rupt, &datalen, &foraddr, &forport, &iucvbf,
                  &retcode);
            if (Sock_Debug) {
               fprintf(stderr, "select: rc = %d Next note type is %08x\n",
                       retcode, notetype);
            }
            if (retcode) {
               errno = EPIPE;      /* The connection is broken        */
               return(-1);         /* Indicate I/O error to caller    */
            }
            if (notetype == DATAdelivered) {
               mask = 0x01;
               for (i = 0; i < numfds; ++i) {
                   if (con_num == connection[i].number) {
                      connection[i].read_status = READ_DONE;
                      newfds |= mask;
                      ++num;
                   }
                   mask <<= 1;
               }
               stayinloop = 0;
            } else if (notetype == TIMERexpired) {
               stayinloop = 0;
            } else if (notetype == CONNECTIONstateCHANGED) {
               stayinloop = 0;
               num = -1;            /* Indicate I/O error to caller */
               errno = EPIPE;       /* And the connection is broken */
            } else if (notetype == UDPdatagramDELIVERED) {
               mask = 0x01;
               for (i = 0; i < numfds; ++i) {
                   if (con_num == connection[i].number) {
                      connection[i].read_status = READ_DONE;
                      connection[i].foraddr = foraddr;
                      connection[i].forport = forport;
                      connection[i].datalen = datalen;
                      newfds |= mask;
                      ++num;
                   }
                   mask <<= 1;
               }
               stayinloop = 0;
            }
         }
         inter(cdestroytimer, &TCP_env, &timerpt);
    }
    *readfds = newfds;

    if (Sock_Debug)
       fprintf(stderr, "select returning %d\n", num);
    return(num);
}

writev(fd, iov, num)
int fd;
struct iovec *iov;
int num;
{
    int  i, total;
    int  rc;

    total = 0;
    for (i = 0; i < num; ++i) {
        if (iov->iov_len) {
           rc = write(fd, iov->iov_base, iov->iov_len);
           total += rc;
           if (rc != iov->iov_len)
              return(total);
        }
        ++iov;
    }
    return(total);
}

readv(fd, iov, num)
int fd;
struct iovec *iov;
int num;
{
    int  i, total;
    int  rc;

    total = 0;
    for (i = 0; i < num; ++i) {
        if (iov->iov_len) {
           rc = read(fd, iov->iov_base, iov->iov_len);
           total += rc;
           if (rc != iov->iov_len)
              return(total);
        }
        ++iov;
    }
    return(total);
}

send(s, buf, len, flags)
int      s;
char     *buf;
int      len, flags;
{
    int  rc;

    rc = sendto(s, buf, len, flags,
                connection[s].to, connection[s].tolen);
    return(rc);
}

sendto(s, msg, len, flags, to, tolen)
int      s;
char     *msg;
int      len, flags;
struct sockaddr_in *to;
int      tolen;
{
    int  lcladdr, lclport, rmtaddr, rmtport;
    int  push, urgent;
    int  retcode;

    rmtaddr = to->sin_addr.s_addr;
    rmtport = to->sin_port;
    if (Sock_Debug) {
       fprintf(stderr, "sendto socket: %d, buffer: %04x, len: %d\n",
               s, msg, len);
       fprintf(stderr, " sockaddr: sin_family: %d\n", to->sin_family);
       fprintf(stderr, " sockaddr: sin_port: %d\n", rmtport);
       fprintf(stderr, " sockaddr: sin_addr: %04x\n", rmtaddr);
    }
    if (connection[s].number == CONN_AVAIL) {
    if (Sock_Debug)
       fprintf(stderr, " connect: Trying to open UDP connection.\n");
       lcladdr = UNSPECIFIEDaddress;
       lclport = UNSPECIFIEDport;
       inter(cudpopen, &TCP_env, &lcladdr, &lclport,
                       &connection[s].number, &retcode);
       if (retcode)
          TCPmsg(retcode);     /* Display msg if failure */
       if (Sock_Debug) {
          fprintf(stderr, " connect: Back with open connection!\n"); ;
          fprintf(stderr, " connect returning %d\n",
                  connection[s].number);
       }
    }

    inter(cudpwatsend, &TCP_env, &connection[s].number,
                       &rmtaddr, &rmtport, &msg, &len, &retcode);
    if (retcode)
       TCPmsg(retcode);     /* Display msg if failure */
    return(len);
}

recv(s, buf, len, flags)
int      s;
char     *buf;
int      len, flags;
{
    int  rc;

    rc = recvfrom(s, buf, len, flags,
                  connection[s].to, connection[s].tolen);
    return(rc);
}

recvfrom(s, buf, len, flags, from, fromlen)
int      s;
char     *buf;
int      len, flags;
struct sockaddr_in *from;
int      fromlen;
{
    int  lcladdr, lclport, rmtaddr, rmtport;
    int  retcode;
    int  notemask = UDPdatagramDELIVERED;
    int  wait = 1;
    int  notetype;
    int  con_num;
    int  urgbytes, urgspan;
    int  bytesdel, lasturg, push, amtspc, newstat, reas, datm;
    int  assctmr, rupt;
    char iucv_buffer[IUCV_BSIZE];
    char *iucvbf = iucv_buffer;

    if (Sock_Debug) {
       fprintf(stderr, "recvfrom socket: %d, buffer: %04x, len: %d\n",
               s, buf, len);
    }
    if (connection[s].number == CONN_AVAIL) {
    if (Sock_Debug)
       fprintf(stderr, " connect: Trying to open UDP connection.\n");
       lcladdr = UNSPECIFIEDaddress;
       lclport = UNSPECIFIEDport;
       inter(cudpopen, &TCP_env, &lcladdr, &lclport,
                       &connection[s].number, &retcode);
       if (retcode)
           TCPmsg(retcode);     /* Display msg if failure */
       if (Sock_Debug) {
          fprintf(stderr, " connect: Back with open connection!\n"); ;
          fprintf(stderr, " connect returning %d\n",
                  connection[s].number);
       }
    }

    if (connection[s].read_status == READ_OUTSTANDING) {
       if ((retcode = clear_reads(1, s)) < 0)
          return(-1);        /* I/O Error on socket */
    }
    if (connection[s].read_status == READ_DONE) { /* In lookaside   */
         memcpy(buf, connection[s].b.bp, connection[s].datalen);
         connection[s].read_status = READ_NONE; /* Reset status     */
         return(len = connection[s].datalen);   /* Return to caller */
    }

    inter(cudpreceive, &TCP_env, &connection[s].number,
                       &buf, &retcode);
    if (retcode)
       TCPmsg(retcode);     /* Display msg if failure */
/*
** Now wait for notification that we have data back!
*/
    inter(chandle, &TCP_env, &notemask, &retcode);
    inter(cgetnextnote, &TCP_env, &wait, &notetype,
                        &con_num, &urgbytes, &urgspan, &bytesdel,
                        &lasturg, &push, &amtspc, &newstat, &reas, &datm,
                        &assctmr, &rupt,
                        &len, &from->sin_addr.s_addr, &from->sin_port,
                        &iucvbf, &retcode);
    inter(cunhandle, &TCP_env, &notemask, &retcode);
    if (Sock_Debug) {
       fprintf(stderr, " received, buffer: %04x, len: %d\n", buf, len);
       fprintf(stderr, " sockaddr: sin_addr: %08x\n",
               from->sin_addr.s_addr);
       fprintf(stderr, " sockaddr: sin_port: %d\n", from->sin_port);
    }
    return(len);
}

static TCPmsg(rc)
int rc;
{
    int  retlen;
    char buf[256];

    inter(csaycalre, &TCP_env, &rc, buf, &retlen);
    fprintf(stderr, buf);    /* Produce TCPIP error message  */
    inter(cendtcpip, &TCP_env); /* Stop TCPIP now...         */
    TCP_env = 0;             /* Avoid on_exit error messages */
    exit(rc);
}

static ReadXlate()
{
    FILE *fp;
    struct {
         char header[sizeof(XLATE_HEADER) - 1];
         char atoe[256];
         char etoa[256];
    } xlate;
    char *cp;

    if ((cp = getenv("X-XLATE")) == NULL)
         cp = XLATE_FILENAME;
    if ( (fp = fopen(cp, "rb")) == NULL) {
       fprintf(stderr, "Translate file %s not found\n", cp);
    } else {
        fread(&xlate, sizeof(xlate), 1, fp);
        if (strncmp(xlate.header, XLATE_HEADER, strlen(XLATE_HEADER))) {
           fprintf(stderr, "Format error in EBCDIC/ASCII translate file header\n");
        } else {
           memcpy(asciitoebcdic, xlate.atoe, 256);
           memcpy(ebcdictoascii, xlate.etoa, 256);
        }
    }
    fclose(fp);
    return;
}

static clear_reads(wait, fd)
int wait;     /* For ioctl, nowait; for read, wait */
int fd;       /* When waiting, wait for read on this connection */
{
    int  i;
    int  bytesread, retcode;
    int  notetype;
    int  con_num;
    int  urgbytes, urgspan;
    int  bytesdel, lasturg, push, amtspc, newstat, reas, datm;
    int  assctmr, rupt, datalen, foraddr, forport;
    char iucv_buffer[IUCV_BSIZE];
    char *iucvbf = iucv_buffer;

    do {
       inter(cgetnextnote, &TCP_env, &wait, &notetype,
                  &con_num, &urgbytes, &urgspan, &bytesdel,
                  &lasturg, &push, &amtspc, &newstat, &reas, &datm,
                  &assctmr, &rupt, &datalen, &foraddr, &forport, &iucvbf,
                  &retcode);
       if (Sock_Debug) {
          fprintf(stderr,
                 "Clear_reads: rc = %d Next note type is %08x\n",
                  retcode, notetype);
       }
       if (notetype == DATAdelivered) {
          for (i = 0; i < MAXSOCK; ++i) {
              if (con_num == connection[i].number) {
                 connection[i].read_status = READ_DONE;
              }
              if (i == fd) {
                 return(0);
              }
          }
       } else if (notetype == CONNECTIONstateCHANGED) {
              errno = EPIPE;      /* Signify I/O error */
              return(-1);
       }
    } while (retcode != NOoutstandingNOTIFICATIONS);
    return(0);
}

tcp_debug(onoff)
int onoff;
{
    Sock_Debug = onoff;
}
