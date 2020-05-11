/*+-------------------------------------------------------------------+
  |  <Doc>                                                            |
  |  <NAME>    obj_names Header                                       |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    March 25, 1988                                         |
  |  <LEVEL>   OBJ                                                    |
  |  <PURPOSE> Definitions for general purpose object - actions       |
  |            functions for use in non-state operations. All fun-    |
  |            tions follow the standard obj-act calling convention.  |
  |  </Doc>                                                           |
  +-------------------------------------------------------------------+*/
                               /*  $MAKER-PROTO            $EXTRACT    */
                               /*                                      */
extern sint gt_name( OCB * ocb, string modifier, hword level, string * params);
                               /* Return the name given an OCB.        */
                               /* Params:                              */
                               /*                                      */
                               /*      OCB     *ocb;  ocb of user      */
                               /*      string   mod;  not used         */
                               /*      hword    level; one of:         */
#define    FULL_TEIES_NAME                        1
#define    FULL_NAME_ONLY                         2
#define    NICKNAME_AND_NUMBER                    3
#define    NICKNAME_ONLY                          4
#define    SNI_KEY_FIRST_NAME                     5
#define    SNI_KEY_LAST_NAME                      6
                               /*                                      */
                               /*      string  *result;  pointer to    */
                               /*                        result.       */
                               /* $ENDEXTRACT                          */

