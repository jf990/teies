/*********************************************************************
  <DOC>
  <Name>     TM.H   Transaction Manager
  <Date>     November 5, 1986
  <Author>   JF
  <Level>    TM
  <Purpose>
  This module provides external data constants and structures for the
  programmer calling upon the functionality of the TM. Most important
  is the Global TM Process Table and Master Process Control Block.

  Refer to the section on the TM level in the Specification Document.
  </Doc>
*********************************************************************/

#ifndef TM_H_INCLUDED

#define TM_H_INCLUDED 1


#ifndef OBJ_H_INCLUDED
#include "obj.h"
#endif

#ifndef HASH_H_INCLUDED
#include "hash.h"
#endif

#ifndef VARS_H_INCLUDED
#include "vars.h"
#endif

#ifndef NUMBER_OF_ODTs
#include "tm_comm.h"
#endif

#ifndef TASK_COMPLETE
#include "tm_x.h"
#endif

#ifndef TM_INTERNAL_ERROR
#include "tm_errs.h"                                  /* $MAKER-IGNORE */
#endif

#ifndef MEM_H_INCLUDED
#include "mem.h"
#endif

#ifndef EXC_H_INCLUDED
#include "exc.h"
#endif

#ifndef ISF_H_INCLUDED
#include "isf.h"
#endif

#ifndef TM_PF_H_INCLUDED
#include "tm_pf.h"
#endif

#ifndef FORM_PF_H_INCLUDED
#include "form_pf.h"
#endif



#endif
