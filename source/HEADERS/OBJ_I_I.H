/*+-------------------------------------------------------------------+
  |  <Doc>                                                            |
  |  <NAME>    OBJ_I_I H                                              |
  |  <AUTHOR>  Nestor Voronka                                         |
  |  <DATE>    June  8, 1987                                          |
  |  <LEVEL>   Object                                                 |
  |  <PURPOSE> This header contains the actual internal header info.  |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * The following macro returns the pointer to the Object Definition    *
 * Table given the pointer to the Object Control Block.                *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define ocb_odt(ocb_entry)     ((ocb_entry)->odt)


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * The following macros return the fields of an ODT record when passed *
 * the pointer to the record in 'odt_entry'.                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define odt_name(odt_entry)    ((odt_entry)->name)
#define odt_lvl(odt_entry)     ((odt_entry)->level)
#define odt_off(odt_entry)     ((odt_entry)->offset)
#define odt_nxto(odt_entry)    ((odt_entry)->offnxt)
#define odt_prvo(odt_entry)    ((odt_entry)->offprv)
#define odt_type(odt_entry)    ((odt_entry)->type)


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * The following macro will return the record number of the ODT record *
 * if it is passed the pointer to the ODT table in 'odt' and the       *
 * pointer to the ODT record in 'odt_entry'.                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define ocd_num(odt,odt_entry)   ((odt_entry)-(odt))
