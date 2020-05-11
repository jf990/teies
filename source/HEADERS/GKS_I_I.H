/*********************************************************************


:NAME.    GKS_I_I.H
:LEVEL.   GKS internal
:DATE.    June 23, 1987
:AUTHOR.  Kenneth Liang

:PURPOSE.

    This header file contains the definitions of GKS macro's and
    declarations of the GKS routines used internally.


:MODS.

:END.

*********************************************************************/




/*------------------------------------------------------------------+
 |                                                                  |
 |  GKS internal macro's                                            |
 |                                                                  |
 +------------------------------------------------------------------*/


#define  g_emplst( head ) ( (head) == NIL )





/*------------------------------------------------------------------+
 |                                                                  |
 |  Declarations of GKS internal routines.                          |
 |                                                                  |
 +------------------------------------------------------------------*/


/* $EXTRACT */

                                       /* -------------------------- */
extern   sint      g_clip()       ;    /* Clip a line using the      */
                                       /* current window.            */
                                       /*   real *x, *y, *len        */
                                       /*   real xdir, ydir          */
                                       /* -------------------------- */
extern   sint      *g_ftrans()    ;    /* Returns a pointer to the   */
                                       /* transformation specified   */
                                       /*   sint transnum            */
                                       /* -------------------------- */
extern   sint      *g_fndwk()     ;    /* Test to see if specified   */
                                       /* workstation is in a list.  */
                                       /* It returns pointer to the  */
                                       /* workstation state list (an */
                                       /* in virtual device table)   */
                                       /* for true or NIL for false. */
                                       /*   struct wk_lst  *wklst    */
                                       /*   sint           work_id   */
                                       /* -------------------------- */
extern   sint      *g_addwk()      ;   /* Add a workstation id to a  */
                                       /* list of workstations point-*/
                                       /* ed by a pointer.           */
                                       /*   struct wk_lst **wklst    */
                                       /*   sint           work_id   */
                                       /* -------------------------- */
extern   sint      g_rmvwk()      ;    /* Remove a workstation from a*/
                                       /* list of workstations.      */
                                       /*   struct wk_lst **wklst    */
                                       /*   sint           work_id   */
                                       /* -------------------------- */
extern   sint      *g_find  ()    ;    /* Test to see if specified   */
                                       /* element     is in a list.  */
                                       /* It returns pointer to the  */
                                       /* element structure for true */
                                       /* or NIL for false.          */
                                       /*   struct list    *head     */
                                       /*   sint           id        */
                                       /* -------------------------- */
extern   sint      *g_add()        ;   /* Add an new element to a    */
                                       /* linked list pointed by     */
                                       /*   struct list   **head     */
                                       /*   sint           id        */
                                       /* -------------------------- */
extern   sint      g_remove()     ;    /* Remove an element from a   */
                                       /* linked list.               */
                                       /*   struct list    **head    */
                                       /*   sint           id        */
                                       /* -------------------------- */
extern   sint      *g_fndsg ()    ;    /* Test to see if specified   */
                                       /* segment     is in a list.  */
                                       /* It returns pointer to the  */
                                       /* segment SL        for true */
                                       /* or NIL for false.          */
                                       /*   struct wk_lst  *head     */
                                       /*   sint           id        */
                                       /* -------------------------- */
extern   sint      *g_addsg()      ;   /* Add a new segment to the   */
                                       /* set of segments in use.    */
                                       /*   struct wk_lst **head     */
                                       /*   sint           id        */
                                       /* -------------------------- */
extern   sint      g_rmvsg()      ;    /* Remove a segment from the  */
                                       /* set of segments in use.    */
                                       /*   struct wk_lst  **head    */
                                       /*   sint           id        */
                                       /* -------------------------- */
extern   sint      g_transform()  ;    /* Transform a pointer in NDC */
                                       /* to another pointer in NDC  */
                                       /* using a given transforma-  */
                                       /* tion matrix.               */
                                       /*   real oldx, oldy          */
                                       /*   real *newx, *newy        */
                                       /*   matrix *mtx              */
                                       /* -------------------------- */
extern   sint      g_dispseg ()   ;    /* Display a segment of a     */
                                       /* workstation.               */
                                       /*   wk_lst *wk               */
                                       /*   seg_sl *seg              */
                                       /* -------------------------- */
extern   sint      g_dispprim()   ;    /* Display a primitive on the */
                                       /* display surface of a       */
                                       /* workstation.               */
                                       /*   wk_lst *wk               */
                                       /*   prim *prim               */
                                       /*   matrix *mtx              */
                                       /* -------------------------- */
extern   sint      g_disptext()   ;    /* Display a TEIES on the     */
                                       /* display surface of a       */
                                       /* workstation.               */
                                       /*   wk_lst *wk               */
                                       /*   text *text               */
                                       /*   matrix *mtx              */
                                       /* -------------------------- */
extern   sint      g_dispfld ()   ;    /* Display an input or output */
                                       /* field. If it is an input   */
                                       /* field, a new input field   */
                                       /* is added to the list of    */
                                       /* active input fields of the */
                                       /* specified workstation.     */
                                       /*   wk_lst *wk               */
                                       /*   field  *fld              */
                                       /*   matrix *mtx              */
                                       /* -------------------------- */
extern   sint      g_newprim ()   ;    /* Create a primitive and     */
                                       /* store it in the currently  */
                                       /* open segment.              */
                                       /*   prim   **prims           */
                                       /*   sint   type              */
                                       /*   sint   len               */
                                       /*   real   *x, *y            */
                                       /*   byte   *data             */
                                       /* -------------------------- */


/* $ENDEXTRACT */
