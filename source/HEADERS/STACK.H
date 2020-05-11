/*********************************************************************
  <DOC>
  <Name>       Stack Header
  <Level>      TM
  <Date>       February 13, 1989
  <Author>     John L. Foster
  <Purpose>
  This header file defines functions and structures for the generalized
  stack handler.   This stack can handle any data type as it is only
  concerned about the size of stacked elements.  However, all stacked
  items on a given stack must be of the same size.
  </Doc>
*********************************************************************/
#ifndef STACK_H_INCLUDED

#define STACK_H_INCLUDED 1




                                   /***********************************/
extern struct _stack_              /*                                 */
       {                           /*                                 */
                                   /*                                 */
         sint   size;              /*Size of 1 stack entry in bytes   */
         sint   tot_el;            /*Max elements allowed in stack.   */
         byte  *base;              /*Pointer to first stack element.  */
         sint   used;              /*Total number currently stacked.  */
         byte  *tos;               /*Pointer to top of stack.         */
         sint   error;             /*Error code of last stack operatio*/
                                   /*                                 */
       };                          /*                                 */
                                   /*                                 */
typedef struct _stack_ STACK;      /*                        $EXTRACT */
                                   /*             $MAKER-PROTO        */
                                   /***********************************/

                                   /*Initialize a new Stack, rc = Base*/
                                   /*  sint SIZE; sint element_SIZE;  */
extern   STACK     *stk_ini(  sint stack_size, sint element_size );

extern   sint       stk_psh( STACK * base, byte * element );
                                   /*Push a new element on top, rc=err*/
                                   /* STACK *BASE; byte *element;     */
                                   /*                                 */
                                   /*You pass a pointer to the element*/
                                   /*    to be pushed.                */
                                   /*                                 */

extern   byte      *stk_pop( STACK * base );
                                   /*Pop top element off the stack,   */
                                   /*  rc = byte *element;            */
                                   /* STACK *BASE;                    */
                                   /*                                 */
                                   /*NOTE: YOU MUST DEALLOCATE THE    */
                                   /*----  POINTER THAT IS RETURNED   */
                                   /*      WHEN YOU ARE DONE WITH THE */
                                   /*      DATA!                      */
                                   /*                                 */

extern   byte      *stk_get( STACK * base );
                                   /*Get top element off the stack,   */
                                   /*  rc = byte *element;            */
                                   /* STACK *BASE;                    */
                                   /*                                 */
                                   /*NOTE: YOU MUST DEALLOCATE THE    */
                                   /*----  POINTER THAT IS RETURNED   */
                                   /*      WHEN YOU ARE DONE WITH THE */
                                   /*      DATA!                      */
                                   /*                                 */

extern   sint       stk_end( STACK * base );
                                   /*Deallocate the stack.STACK *BASE;*/
                                   /*  rc = err;                      */
                                   /*                                 */

extern   sint       stk_qry( STACK * base );
                                   /*Query number of elements stacked.*/
                                   /*  rc = sint SIZE;                */
                                   /* STACK *BASE;                    */
                                   /*                                 */

extern   sint       stk_put( STACK * base, byte * element, sint loc );
                                   /*Push an element at specific loc. */
                                   /*  rc = sint error;               */
                                   /* STACK *BASE; byte *element;     */
                                   /*  sint loc;, where loc is        */
                                   /*  -1 = FIFO, 0 = LIFO,           */
                                   /*  >0 = location from bottom      */
                                   /* <-1 = location from top         */
                                   /*                                 */
#define STACK_PUSH_FIFO     -1
#define STACK_PUCH_LIFO      0


extern   sint       stk_rot( STACK * base,  sint loc );
                                   /*Rotate stack, taking LOC and     */
                                   /*  placing it on top and pushing  */
                                   /*  elements between LOC and top   */
                                   /*  down 1.                        */
                                   /*    rc = err;  STACK *base;      */
                                   /*               sint   loc;       */
                                   /*                                 */

extern   sint       stk_dsp( STACK * base );
                                   /*Dump out the stack contents      */
                                   /*    rc = err; STACK *base;       */
                                   /*                                 */

extern   sint       stk_dup( STACK * base );
                                   /*Duplicate the top stack element  */
                                   /*    rc = err; STACK *base;       */
                                   /*                                 */
                                   /*                      $ENDEXTRACT*/
#endif
