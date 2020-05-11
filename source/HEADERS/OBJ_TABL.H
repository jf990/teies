/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    obj_table header                                       |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    June 03, 1988                                          |
  |  <LEVEL>   OBJ                                                    |
  |                                                                   |
  |  <PURPOSE> Allocate a table of Object Names and their associated  |
  |            data values.                                           |
  |  <MODS>                                                           |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/

extern struct _OBJ_REF {

                          string    object_name;
                          sint      isv_key;
                          hword     object_type;
                          hword     spare;

                       }
                       objtable[ ];

