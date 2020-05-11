/************************************************************************
*|                                                                     |*
*|  :NAME.         sgi_d.h                                             |*
*|                                                                     |*
*|  :LEVEL.        SeGment Index.                                      |*
*|                                                                     |*
*|  :DATE.         10-28-1986.                                         |*
*|                                                                     |*
*|  :AUTHOR.       Fen-Chung Kung.                                     |*
*|                                                                     |*
*|  :GLOBAL.       none.                                               |*
*|                                                                     |*
*|  :PARAM.        none.                                               |*
*|                                                                     |*
*|  :OUTPUT.       none.                                               |*
*|                                                                     |*
*|  :PURPOSE.      This is the external header file that provide all   |*
*|                 necessary routine call.                             |*
*|                                                                     |*
*|  :CALLS.        sgi_dbg(),     internal debugging call for indexes. |*
*|  :CALLS.        sgi_eva(),     evaluate performance of indexes.     |*
*|  :CALLS.        sgi_uti(),     evaluate utilization of indexes.     |*
*|  :CALLS.        sgi_bkp(),     perform the replace operation.       |*
*|  :CALLS.        sgi_bkm(),     call for memory allocation.          |*
*|  :CALLS.        sgidel(),      call for memory allocation.          |*
*|  :CALLS.        sgi_del(),     call for memory allocation.          |*
*|  :CALLS.        sgi_pr() .     call for memory allocation.          |*
*|                                                                     |*
*|  :ERRORS.       none.                                               |*
*|                                                                     |*
*|  :LIMITS.       none.                                               |*
*|                                                                     |*
*|  :MODS.         none.                                               |*
*|                                                                     |*
*|  :END.                                                              |*
*|                                                                     |*
************************************************************************/

/*---------------------------------------------------------------------*/
extern sint   blkcount;
extern struct statis
{
       sint   DC   ;          /* deletion counter                      */
       hword  LEVEL;          /* Tree levels                           */
       hword  ACCESS;         /* frequency of disk accessing           */
       hword  SPLIT;          /* spliting                              */
       tint   TOTAL;
       hword  COMPARE ;       /* key's comparision in a segment        */
       tint   MAX_KEYS;       /* total keys existing in the tree       */
       tint   MAX_NODE;       /* total nodes existing in the tree      */
       real   UTILIZAT;       /* utilization of the tree               */
}      *stat;
/*---------------------------------------------------------------------*/
/*$extract*/                     /* starting of MAKER */
extern sint   sgi_dbg(),         /* block counter start.               */

              sgi_eva(),         /* evaluation of index segment        */

              sgi_uti(),         /* utilization evaluation             */

              sgi_bkp(),         /* block counter increase.            */

              sgi_bkm(),         /* block counter decrease.            */

              sgidel(),          /* block counter decrease.            */

              sgi_del(),         /* block counter decrease.            */

              sgi_pr();          /* print a index segment.             */

extern hword  sgi_move(),        /* move to next key item if del='1'   */

              sgi_back();        /* back to previous one. if del = '1' */

extern struct item *sgi_item();  /* alloca. an item and initiali.  it  */

 /* $endextract */
