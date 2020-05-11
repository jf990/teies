/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    Sched.H                                                |
  |                                                                   |
  |  :AUTHOR.  John L. Foster                                         |
  |                                                                   |
  |  :DATE.    October 9, 1987                                        |
  |                                                                   |
  |  :LEVEL.   TM - Transaction Manager Scheduler                     |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PURPOSE. Internal scheduler structures and definitions.         |
  |                                                                   |
  |  :END.                                                            |
  |                                                                   |
  +-------------------------------------------------------------------+*/


extern struct _SE_
       {

          tint      pid;
          byte      priority;
          datetime  dt_sched;
          datetime  dt_start;
          sint      revol_pos;
          sint      spare;

       };

typedef  struct _SE_   SE;
