/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|   :NAME.      PRIV.H                                              |*/
/*|                                                                   |*/
/*|   :AUTHOR.    John Foster, Samir Chopra, Pamela Cham              |*/
/*|                                                                   |*/
/*|   :DATE.      February 10, 1989                                   |*/
/*|                                                                   |*/
/*|   :LEVEL.     PRV                                                 |*/
/*|                                                                   |*/
/*|   :PURPOSE.   Privilege management structures and definitions.    |*/
/*|               Routines are in PRIV_X.H                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#ifndef PRIV_H_INCLUDED
#define PRIV_H_INCLUDED


struct      prv_map                       /* o o o o o o o o o o o o o */
{                                         /*                           */
            byte one;                     /*                           */
            byte two;                     /*        Define what a      */
            byte three;                   /*     privilege structure   */
            byte four;                    /*             is            */
            byte five;                    /*                           */
            byte six;                     /*                           */
};                                        /*                           */
                                          /*                           */
typedef     struct prv_map    PRIVS;      /* o o o o o o o o o o o o o */



/* $EXTRACT $MAKER-PROTO */

/*+------------------------------------------------------------------+*/
/*| Actual definition of the PRIVS structure may be found in         |*/
/*| OBJ_COMM.h.                                                      |*/
/*+------------------------------------------------------------------+*/
/*| Declarations of PRV level bit manipulation routines.             |*/
/*+------------------------------------------------------------------+*/


/*+-------------------------------------------------------------------+*/
/*| Prv Set :  Sets on the privilege bit{s} {indicated by the         |*/
/*| privilege key, prv_key} in the privilege structure pointed to by  |*/
/*| the privilege pointer, prv.  Any bits that are already set on are |*/
/*| left on.                                                          |*/
/*+-------------------------------------------------------------------+*/

extern sint prv_set (PRIVS *prv, hword prv_key);

/*+-------------------------------------------------------------------+*/
/*| Prv Reset :  Sets off the privilege bit{s} {indicated by the      |*/
/*| privilege key, prv_key} in the privilege structure pointed to by  |*/
/*| the privilege pointer, prv.  If the bit is already off, it is left|*/
/*| off.                                                              |*/
/*+-------------------------------------------------------------------+*/

extern sint prv_rset (PRIVS *prv, hword prv_key);

/*+-------------------------------------------------------------------+*/
/*| Prv Compare :  Checks the equality {in bytewise fashion} of the   |*/
/*| privilege structures pointed to by prv1 and prv2.  This routine   |*/
/*| returns TRUE for equal structures, and FALSE, otherwise.          |*/
/*+-------------------------------------------------------------------+*/

extern boolean prv_cmp (PRIVS *prv1, PRIVS *prv2);

/*+-------------------------------------------------------------------+*/
/*| Prv Test :  Returns TRUE if all bits in the mask, prv_key, are set|*/
/*| on in the privilege structure pointed to by prv.  If any bits     |*/
/*| specified in prv_key are not set on, the value returned is FALSE. |*/
/*+-------------------------------------------------------------------+*/

extern boolean prv_test (PRIVS *prv, hword prv_key);

/*+-------------------------------------------------------------------+*/
/*| Prv Mask :  Performs a bitwise operation on two privilege         |*/
/*| structures pointed to by prv_1 and prv_2, respectively and        |*/
/*| stores the result in a third privilege structure pointed to by    |*/
/*| *prv_result.                                                      |*/
/*+-------------------------------------------------------------------+*/

extern sint prv_mask (PRIVS **prv_result, PRIVS *prv_1, PRIVS *prv_2,
                      sint opcode);

/*+-------------------------------------------------------------------+*/
/*| Prv ISV :  Matches an input string of a role to an entry in the   |*/
/*| ISV table and calls prv_map to carry out bitmap settings.         |*/
/*+-------------------------------------------------------------------+*/

extern sint prv_isv (string phrase, PRIVS **prv_1);

/*+-----------------------------------------------------------------+*/
/*| Prv_map : takes as input the isv_key corresponding to a role    |*/
/*| and returns the bitmap setting of the role in question.         |*/
/*+-----------------------------------------------------------------+*/

extern sint prv_map (sint isv_key, PRIVS *priv_ptr);

/* $ENDEXTRACT */

/*+-------------------------------------------------------------------+*/
/*| Definitions of PRV level error codes follow.  Exception level for |*/
/*| the PRV level is set as 26.                                       |*/
/*+-------------------------------------------------------------------+*/

#define PRV_BAD_OPCODE                       26550
#define PRV_UNDEFINED_PRIVILEGE              26551
#define PRV_UNDEFINED_ROLE                   26552
#define BAD_POINTER_TO_PRIVS                 26553
#define ISV_KEY_OUT_OF_RANGE                 26554

/*+-------------------------------------------------------------------+*/
/*| Definitions of PRV level bitwise operators:                       |*/
/*+-------------------------------------------------------------------+*/

#define PRV_OR    1
#define PRV_AND   2
#define PRV_MAX   3
#define PRV_MIN   4
#define PRV_XOR   5

/*+-------------------------------------------------------------------+*/
/*| The following are specific bit map settings for privileges.  In   |*/
/*| order to allow for expansion of existing privileges, space has    |*/
/*| allocated for forty-eight bits in the PRIVS structure.            |*/
/*+-------------------------------------------------------------------+*/

#define PRV_REFERENCE   0x0501
#define PRV_USE         0x0502
#define PRV_READ        0x0504
#define PRV_COPY        0x0508
#define PRV_CREATE      0x0510
#define PRV_AUTHOR      0x0520
#define PRV_ABSTRACT    0x0540
#define PRV_REPLY       0x0580
#define PRV_APPEND      0x0401
#define PRV_SOLICIT     0x0402
#define PRV_RENDER      0x0404
#define PRV_APPROVE     0x0408
#define PRV_ASSIGN      0x0410
#define PRV_TAKE        0x0420
#define PRV_DELETE      0x0440
#define PRV_BURN        0x0480
#define PRV_PERFORM     0x0301
#define PRV_INDEXER     0x0302
#define PRV_JOIN        0x0304
#define PRV_MODIFY      0x0308
#define PRV_SPONSOR     0x0310
#define PRV_OWNER       0x0320
#define PRV_SYS         0x0340
#define PRV_TARGET      0x0380

/*+-------------------------------------------------------------------+*/
/*| The following four bits are reserved for definition within a      |*/
/*| specific group communication application.  Their interpretation   |*/
/*| will be different for different group communication structures.   |*/
/*+-------------------------------------------------------------------+*/

#define PRV_ARBITRARY_ONE     0x0201
#define PRV_ARBITRARY_TWO     0x0202
#define PRV_ARBITRARY_THREE   0x0204
#define PRV_ARBITRARY_FOUR    0x0208

#endif
