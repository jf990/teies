/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                  ISF_X H

:LEVEL.                 ISCF

:AUTHOR.                Sal Johar

:DATE.                  02/11/89

:PURPOSE.

:MODS.
                        Moved into this header file all ISF routines


* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*
$extract
$MAKER-PROTO */

extern   sint      isf_star(
         sint      *maxcon
                             );
#define isf_start isf_star

extern   sint      isf_end(
         void
                             );

extern   sint      isf_open(
         string    id,
         sint      level,
         sint      *pid
                             );

extern   sint      isf_clos(
         string    id,
         sint      level
                             );
#define isf_close isf_clos

extern   sint      isf_send(
         string    id,
         sint      level,
         string    buf
                             );

extern   sint      isf_recv(
         string    id,
         sint      level,
         string    *buf
                             );
#define isf_receive isf_recv

extern   sint      isf_qry(
         string    id,
         sint      flags,
         sint      *int_data,
         string    *str_data
                             );
#define isf_query isf_qry

extern   void      isferrs(
         sint      rc
                             );

/*
extern void isfsend(   $main */

