/*+--------------------------------------------------------------------+
 | <DOC>                                                              |
 | <NAME>    VARS.H                                                   |
 | <LEVEL>   TM                                                       |
 | <AUTHOR>  John L. Foster                                           |
 | <DATE>    November 20, 1987                                        |
 | <PURPOSE> This header file defines the routines for manipulating   |
 |           user interface variables.  User Interface procedures     |
 |           and Object Actions use these variables for data storage, |
 |           data conversion, user input and Forms interfacing.       |
 |                                                                    |
 | <MODS>  23_Aug_1989    Samir Chopra  Rewrote to include definitions|
 |                                      of routines to handle stemmed |
 |                                      variables. Hash tables are    |
 |                                      used as storage mechanism. For|
 |                                      definition of structures used |
 |                                      refer to Hash.h . All data    |
 |                                      types for interface variables |
 |                                      are now supported.            |
 | </DOC>                                                             |
 +--------------------------------------------------------------------+*/

#ifndef VARS_H_INCLUDED

#define VARS_H_INCLUDED 1

#ifndef TM_H_INCLUDED
#error  VARS.H requires TM.H
#endif

#ifndef DATAPRIM_H_INCLUDED
#include "dataprim.h"
#endif

#include "var_x.h"

#endif
