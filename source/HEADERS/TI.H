/*********************************************************************
<NAME>    TI.H
<LEVEL>   TI
<DATE>    July 27, 1987
<AUTHOR>  Kenneth Liang

<PURPOSE> Definitions for the Terminal Interface layer of TEIES
<MODS>    Nov-07-88    Added GKS and VDI includes
          Mar 07 89    Prototying and header file standards added.

</Doc>
*********************************************************************/

#ifndef TI_H_INCLUDED

#define  TI_H_INCLUDED  1
#define  NUMOFTYPES     4

struct   _conv
{                                   /***********************************/
    sint      type;                 /* Workstation type                */
    string    rname;                /* Real device name used by OS     */
    string    lname;                /* Logical workstation name set by */
                                    /*   GKS user                      */
    sint      category;             /* Workstation category            */
};                                  /***********************************/


extern   struct    _conv    *convtab;  /* Workstation name conversion table*/

/* $EXTRACT     $MAKER-PROTO    */

extern   sint  ti_init( void );
                                   /***********************************/
                                   /* Initialize Terminal Interface   */
                                   /* ------------------------------- */
extern   sint  tini_wtct( void );
                                   /* ------------------------------- */
                                   /* Initialize workstation type     */
                                   /* conversion table                */
                                   /***********************************/



extern   sint  ti_end( void );
                                   /***********************************/
                                   /* Shut down Terminal Interface    */
                                   /* ------------------------------- */




/* $ENDEXTRACT */

#ifndef VDI_H_INCLUDED
#include "vdi.h"
#endif

#ifndef GKS_H_INCLUDED
#include "gks.h"
#endif

#endif
