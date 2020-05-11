/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    DATAPRIM Header                                        |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    September 6, 1989                                      |
  |  <LEVEL>   OBJ / VARS                                             |
  |  <PURPOSE> Define data type constants for data independence in the|
  |            variables and object components.                       |
  |  <MODS>                                                           |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/


#ifndef DATAPRIM_H_INCLUDED

#define DATAPRIM_H_INCLUDED 1

#define       OC_LOWER_LIMIT 1    /*  Minimum data definition type.    */
#define       OC_BYTE    1        /* TEIES type byte                   */
#define       OC_INT     2        /* Integer number 4 byte ordered     */
#define       OC_REAL    3        /* Real - Floating point number      */
#define       OC_STR     4        /* String (TEIES style)              */
#define       OC_D_T     5        /* Date and Time (TEIES style)       */
#define       OC_PRIV    6        /* Privilege mask (6 bytes)          */
#define       OC_OID     7        /* Object Identifier                 */
#define       OC_HWRD    8        /* Half Word (16bit) number          */
#define       OC_INHR    9        /* OID Inheritance link              */
#define       OC_ISV    10        /* ISV hword reference is implied    */
#define       OC_VAR    11        /* TM level variable reference.      */
#define       OC_GFD    12        /* Generic File descriptor (OS File) */
#define       OC_HASH   13        /* Hash Table Reference              */
#define       OC_CSTR   14        /* C string (NIL byte at end.)       */
#define       OC_UPPER_LIMIT 14   /*  Maximum data definition type.    */

#endif
