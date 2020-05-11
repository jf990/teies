/*********************************************************************

  FORMI.H                   November 9, 1987                      JF


  Some Global, one-time structures needed to initialize Forms.

*********************************************************************/






/*  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  */
/*                                                                    */
/*      FORMS tables of markups, qualifiers, and global pointers      */
/*      NOTE: Form.h must be included before this to resolve symbols. */
/*  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  */




byte *units[] = {                   /* types of units can be specified */
           "INCH",
           "MILLIMETERS",
           "MM",
           "CENTIMETERS",
           "CM",
           "PICA",
           "CICERO",
           "CHARACTERS",
           "EMS",
           "DEVICE UNITS",
           "DU"
           };

byte *colors[] = {                              /* color chart for GKS */
           "BLUE",
           "RED",
           "PINK",
           "GREEN",
           "TURQUOISE",
           "YELLOW",
           "WHITE"
           };

byte *justifys[] = {                        /* types of justifications */
           "L",
           "R",
           "F",
           "C"
           };
