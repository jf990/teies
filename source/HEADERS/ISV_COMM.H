/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| :NAME.     ISV_COMM H                                             |*/
/*|                                                                   |*/
/*| :LEVEL.    ISV                                                    |*/
/*|                                                                   |*/
/*| :GLOBAL.                                                          |*/
/*|                                                                   |*/
/*| :PARAM.                                                           |*/
/*|                                                                   |*/
/*| :PURPOSE.  Contains definitions used internally by ISV and        |*/
/*|            externally by functions using ISV's.  Included by      |*/
/*|            both ISV.H and ISV_I.H                                 |*/
/*|                                                                   |*/
/*| :OUTPUT.                                                          |*/
/*|                                                                   |*/
/*| :CALLS.                                                           |*/
/*|                                                                   |*/
/*| :ERRORS.                                                          |*/
/*|                                                                   |*/
/*| :LIMITS.                                                          |*/
/*|                                                                   |*/
/*| :MODS.     15_Feb_89   K.J.Walsh   Implemented.                   |*/
/*|                                                                   |*/
/*| :MODS.     08_Feb_89   K.J.Walsh   Standardized.                  |*/
/*|                                                                   |*/
/*| :END.                                                             |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#ifndef ISV_COMM_H_INCLUDED
#define ISV_COMM_H_INCLUDED

typedef  sint isv;

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|   Following are error return codes for isv_init() and isv_add().  |*/
/*|   isv_init() calls isv_add() after checking to make sure the      |*/
/*|   default was not already added.                                  |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#define  ISV_FNF              -16512   /* File Not Found               */
#define  ISV_FILE_NOT_FOUND   ISV_FNF
#define  ISVFILENOTFOUND      ISV_FNF

#define  ISV_IFC              -16513   /* Invalid Format/Corrupt ISV   */
#define  ISV_INVALID_FORMAT   ISV_IFC
#define  ISV_INVALIDFORMAT    ISV_IFC
#define  ISV_INVALID          ISV_IFC
#define  ISV_CORRUPT          ISV_IFC

#define  ISV_WAR              -16514   /* isv_init() Was Already Run   */
#define  ISV_INIT_WAS_RUN     ISV_WAR

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|   Following are error return codes for isv_rem()                  |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#define  ISV_NIM              -16522   /* ISV Not In Memory            */
#define  ISV_NOT_IN_MEM       ISV_WAR
#define  ISV_NOTINMEM         ISV_WAR
#define  ISV_NOT_IN_MEMORY    ISV_WAR
#define  ISV_NOTINMEMORY      ISV_WAR

#define  ISV_DIC              -16523   /* Default ISV Cannot be removed*/
#define  ISV_CANNOT_REM_DEF   ISV_DIC

#define  ISV_BNG              -16524   /* Bad (file) Name Given        */
#define  ISV_BAD_FD           ISV_BNG
#define  ISV_BAD_FNAME        ISV_BNG
#define  ISV_BAD_FILE         ISV_BNG

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|   Following are error return codes for isv_lkup()                 |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#define  ISV_ITC              -16531   /* Invalid Tag Construction     */
#define  ISV_BAD_TAG          ISV_ITC
#define  ISV_INVALID_TAG      ISV_ITC

#define  ISV_TTL              -16532   /* Tag Too Long                 */
#define  ISV_TAG_TOO_LONG     ISV_TTL

#define  ISV_TNF              -16533   /* Tag Not Found                */
#define  ISV_TAG_NOT_FOUND    ISV_TNF

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|   Following are error return codes for isv_get()                  |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#define  ISV_NF               -16543   /* Not Found in any loaded ISV  */
#define  ISV_NOT_FOUND        ISV_NF

#define  ISV_NL               -16542   /* No Loaded ISV found          */
#define  ISV_NOT_LOADED       ISV_NL

#define  ISV_IOB              -16541   /* Index Out of Bounds          */
#define  ISV_BAD_INDEX        ISV_IOB
#define  ISV_INDEX_BAD        ISV_IOB
#define  ISV_OUTOFBOUNDS      ISV_IOB
#define  ISV_OUT_OF_BOUNDS    ISV_IOB

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|   This "function" is used to determine if an ISV returns an error |*/
/*|   code:                                                           |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#define  ISV_BAD_RC(rc) ( rc < 0 )     /* Negative RC means error      */

#endif
