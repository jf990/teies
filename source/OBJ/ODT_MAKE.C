/*+------------------------------------------------------------------+*/
/*|                                                                  |*/
/*|  :NAME.      odt_make ()                                         |*/
/*|                                                                  |*/
/*|  :LEVEL.     OBJ                                                 |*/
/*|                                                                  |*/
/*|  :GLOBAL.    odt_type_rec                                        |*/
/*|                                                                  |*/
/*|  :PARAM.     ocd                                                 |*/
/*|                                                                  |*/
/*|  :PURPOSE.   To create an ODT table from input in text file form |*/
/*|                                                                  |*/
/*|  :OUTPUT.    ocd                                                 |*/
/*|                                                                  |*/
/*|  :CALLS.     s_init                                              |*/
/*|                                                                  |*/
/*|  :ERRORS.                                                        |*/
/*|                                                                  |*/
/*|  :LIMITS.    IF a segment becomes a Phantom segment when a       |*/
/*|              segment appears in a segment containing non-segment |*/
/*|              data, the last non-segment field CANNOT be a string.|*/
/*|              If it is a string, it will be the last non-segment  |*/
/*|              in the segment yet we will not know that and as a   |*/
/*|              result the String length computation will not be    |*/
/*|              done.                                               |*/
/*|                                                                  |*/
/*|  :MODS.     19 Jan 88    Steve Muccione  Preliminary version     |*/
/*|             14 Jul 88    JF              Improved debugging,     |*/
/*|                                          structure allocation and|*/
/*|                                          initialization.         |*/
/*|  :END.                                                           |*/
/*|                                                                  |*/
/*+------------------------------------------------------------------+*/

#include "defs.h"
#include "isv.h"
#include "str.h"
#include "str_i_i.h"
#include "osk.h"
#include "obj.h"
#include "debug.h"



#define ODT_WORK_TABLE_SIZE    199
#define MAX_LEVEL              16





sint    odt_make (  ocb,  ocd  )

OCB                *ocb;
OCD                *ocd;



{


struct wrk_def {

   string  name;
   sint    level;
   sint    type;
   PRIVS   priv;
   sint    len;
   boolean key;
   sint    offs;
   sint    prev;
   sint    next;

};



   ODT    *odt_fill  =   NULL;
   ODT    *odt_top   =   NULL;
   string *parse_vec =    NIL;
   string p_template = S_NULL;
   string p_delim    = S_NULL;
   string p_spill    = S_NULL;
   string type       = S_NULL;
   string isv_str    = S_NULL;
   string priv_str   = S_NULL;
   string key        = S_NULL;
   string len_str    = S_NULL;
   string obj_str    = S_NULL;
   string left_b     = S_NULL;
   string rght_b     = S_NULL;
   string privs      = S_NULL;
   string obj_name   = S_NULL;
   string inp_str    = S_NULL;
   sint   cntr            = 0;
   sint   cntr2           = 0;
   sint   cntr3           = 0;
   sint   cntr4           = 0;
   sint   current_level   = 0;
   sint   num_inhr        = 0;
   sint   extr            = 0;
   sint   rc              = 0;
   sint   n               = 0;
   sint   level_zero      = 0;
   sint   obj_type        = 0;
   sint   obj_isv         = 0;
   sint   top          [17];
   sint   skip         [17];
   sint   ofs_skip     [17];
   PRIVS  priv_def     [17];

   struct wrk_def *wrk_ary = NIL;





odt_init();

wrk_ary = (struct wrk_def *)
                 os_mema (sizeof(struct wrk_def) * ODT_WORK_TABLE_SIZE );

for ( cntr=0; cntr < ODT_WORK_TABLE_SIZE; cntr++ )
{

   ( wrk_ary + cntr )->name = S_NULL;

}


s_valloc( &parse_vec, 10 );

for ( cntr=0; cntr < 17; cntr++ )
{

   os_memn( &( priv_def[ cntr ] ), sizeof( PRIVS ) );

}

s_init ( &p_template, " @n @~.. @n@?;=; @n@?:,::;.");

s_init ( &p_delim,  ";*"      );
s_init ( &isv_str,  "ISV"     );
s_init ( &type,     "TYPE"    );
s_init ( &priv_str, "PRIV"    );
s_init ( &key,      "KEY"     );
s_init ( &len_str,  "LEN"     );
s_init ( &obj_str,  "OBJTYPE" );
s_init ( &left_b,   "{"       );
s_init ( &rght_b,   "}"       );
s_init ( &inp_str,  "."       );


cntr          = 1;
current_level = 0;


while ( obj_grec( ocb, ocd, &inp_str ))
{

   s_parse ( &inp_str, p_template, p_delim, parse_vec, &p_spill );
   for ( cntr2=1; cntr2 <= s_len ((string) parse_vec); cntr2++)
   {

      s_upper ( *(parse_vec+cntr2), 0, 0);
      s_trim (  parse_vec+cntr2,    'B' );

   }
   if (s_len ((string) parse_vec) > 0)
   {

      s_copy ( &obj_name, *(parse_vec+1), 1, 0   );
      os_memn( &(wrk_ary->priv), sizeof( PRIVS ) );
      for ( cntr3=2; (cntr3 < s_len ((string) parse_vec)); cntr3++ )
      {

         if( s_comp (*(parse_vec+cntr3),priv_str,0,0,'M',&extr) == 0 )
         {

            cntr3++;
            s_nulout (&privs);
            s_pad( (parse_vec+cntr3),*(parse_vec+cntr3),12, '0', 'L' );
            s_sxtos (&privs,*(parse_vec+cntr3),0,0);
            for (cntr4=0;cntr4<sizeof (PRIVS);cntr4++)
            {
               s_stob (((byte *)&(wrk_ary->priv))+cntr4,
                       privs,(cntr4+1));
            }
            break;
         }
      }
      priv_def[0] = wrk_ary->priv;
      for (cntr3=2;(cntr3 < s_len ((string) parse_vec));cntr3++)
      {
         if (
            s_comp (*(parse_vec+cntr3),type,0,0,'M',&extr) == 0
            )
         {
            s_stosi(&obj_type,*(parse_vec+cntr3+1),0,0);
            break;
         }
      }
      for (cntr3=2;(cntr3 < s_len ((string) parse_vec));cntr3++)
      {
         if (
            s_comp (*(parse_vec+cntr3),isv_str,0,0,'M',&extr) == 0
            )
         {
            s_stosi(&obj_isv,*(parse_vec+cntr3+1),0,0);
            break;
         }
      }
      break;
   }
}

while ( obj_grec( ocb, ocd, &inp_str ))
{

   if (inp_str != S_NULL)
   {

      s_nulout (parse_vec+1);
      s_parse (&inp_str,p_template,p_delim,parse_vec,&p_spill);


      for (cntr2=1;cntr2 <= s_len ((string) parse_vec);cntr2++)
      {
         s_upper (*(parse_vec+cntr2),0,0);
         s_trim (parse_vec+cntr2,'B');
      }
      if (s_comp (*(parse_vec+1),left_b,0,0,'M',&extr) == 0)
      {
         current_level++;
      } else if (s_comp (*(parse_vec+1),rght_b,0,0,'M',&extr) == 0)
         {
            current_level--;
         } else
      for (cntr2=2;(cntr2 < s_len ((string) parse_vec));cntr2++)
      {
         if (s_comp (*(parse_vec+cntr2),type,0,0,'M',&extr) == 0)
         {
            if (
                ((wrk_ary+cntr-1)->type == OC_STR) &&
                ((wrk_ary+cntr-1)->level > current_level)
               )
            {
               rc=s_init (&((wrk_ary+cntr)->name),"     ");
               (wrk_ary+cntr)->level = (wrk_ary+cntr-1)->level;
               (wrk_ary+cntr)->len   = 0;
               (wrk_ary+cntr)->type  = OC_STR;
               (wrk_ary+cntr)->prev  = 1;  /* preset prev as a flag */
               (wrk_ary+cntr)->next  = 1;  /* not to include record */
               (wrk_ary+cntr)->key   = FALSE;
               (wrk_ary+cntr)->priv  = (wrk_ary+cntr-1)->priv;
               cntr++;
            }
            rc=s_copy (&((wrk_ary+cntr)->name),*(parse_vec+1),0,0);
            cntr2++;
            (wrk_ary+cntr)->len=0;
            (wrk_ary+cntr)->type=0;
            for (cntr3=0;(cntr3<=ODT_TYPES);cntr3++)
            {
               if (
                   s_comp((odt_t_r[cntr3].name),
                          *(parse_vec+cntr2),0,0,'M',&extr)==0
                  )
               {

                  (wrk_ary+cntr)->len  = odt_t_r[ cntr3 ].length;
                  (wrk_ary+cntr)->type = odt_t_r[ cntr3 ].type  ;
                  if (odt_t_r[cntr3].type == OC_STR)
                  {
                     (wrk_ary+cntr)->len = 20;  /* Default STR Length  */
                     for ( cntr4=2;
                          (cntr4 < s_len((string) parse_vec));
                           cntr4++
                         )
                     {
                        if (
                            s_comp(*(parse_vec+cntr4),
                                    len_str,0,0,'M',&extr)==0
                           )
                        {
                           s_stosi ( & ((wrk_ary+cntr)->len),
                                       *(parse_vec+cntr4+1), 0, 0   );
                           (wrk_ary+cntr)->len =
                                   s_true_al((wrk_ary+cntr)->len);
                           break;
                        }
                     }
                  }
                  break;
               }
            }
            for (cntr3=0;(cntr3<6);cntr3++);
            {
               *((byte *)&((wrk_ary+cntr)->priv)+cntr3) = '\0';
            }
            (wrk_ary+cntr)->priv = priv_def[current_level];
            for (cntr3=2;(cntr3 < s_len ((string) parse_vec));cntr3++)
            {
               if (
                  s_comp (*(parse_vec+cntr3),priv_str,0,0,'M',&extr) == 0
                  )
               {
                  cntr3++;
                  s_nulout (&privs);
                  s_pad((parse_vec+cntr3),*(parse_vec+cntr3),12
                           ,'0','L');
                  s_sxtos (&privs,*(parse_vec+cntr3),0,0);
                  for (cntr4=0;cntr4<sizeof (PRIVS);cntr4++)
                  {
                     s_stob (((byte *)&((wrk_ary+cntr)->priv))+cntr4,
                             privs,(cntr4+1));
                  }
                  break;
               }
            }
            priv_def[current_level+1] = (wrk_ary+cntr)->priv;
            (wrk_ary+cntr)->level = current_level;
            (wrk_ary+cntr)->prev  = 0;
            (wrk_ary+cntr)->next  = 0;
            (wrk_ary+cntr)->key   = FALSE;
            for (cntr3=2;(cntr3 <= s_len ((string) parse_vec));cntr3++)
            {
               if (s_comp (*(parse_vec+cntr3),key,0,0,'M',&extr) == 0)
               {
                  (wrk_ary+cntr)->key   = TRUE;
                  break;
               }
            }
            for (cntr3=2;(cntr3 <= s_len ((string) parse_vec));cntr3++)
            {
               if ((s_comp (*(parse_vec+cntr3),obj_str,
                           0,0,'M',&extr) == 0) )
               {
                  extr=obj_inta (*(parse_vec+cntr3+1));
                  *((hword *)&((wrk_ary+cntr)->priv.five))=(hword) extr;
                  num_inhr++;
                  break;
               }
            }
            cntr++;
            break;
         }
      }
   }
}
if ((wrk_ary+cntr-1)->type == OC_STR)
{
   rc=s_init (&((wrk_ary+cntr)->name),"    ");
   (wrk_ary+cntr)->level = (wrk_ary+cntr-1)->level;
   (wrk_ary+cntr)->len   = 0;
   (wrk_ary+cntr)->type  = OC_STR;
   (wrk_ary+cntr)->prev  = 1;  /* preset prev as a flag */
   (wrk_ary+cntr)->next  = 1;  /* not to include record */
   (wrk_ary+cntr)->key   = FALSE;
   (wrk_ary+cntr)->priv  = (wrk_ary+cntr-1)->priv;
   cntr++;
}
(wrk_ary)->offs=0;
(wrk_ary)->level=-1;
(wrk_ary+cntr+1)->level=-1;
for (cntr2=0;cntr2 < MAX_LEVEL;cntr2++)
{
   top  [cntr2] = 0;
   skip [cntr2] = 0;
}
cntr--;




for (cntr2 = 1;cntr2 < 17;cntr2++)
{
   ofs_skip[cntr2] = 0;
}
for (cntr2 = 1;cntr2 <= cntr;cntr2++)
{
   if ((wrk_ary+cntr2)->key)
   {
      (wrk_ary+cntr2)->len = 0;
   }
   if ((wrk_ary+cntr2)->level > (wrk_ary+cntr2-1)->level)
   {
      ofs_skip[(wrk_ary+cntr2)->level-1] = cntr2-1;
      (wrk_ary+cntr2)->offs = 0;
   }
   else if ((wrk_ary+cntr2)->level < (wrk_ary+cntr2-1)->level)
   {
         (wrk_ary+cntr2)->offs =
           (wrk_ary+ofs_skip[(wrk_ary+cntr2)->level])->offs+
           (wrk_ary+ofs_skip[(wrk_ary+cntr2)->level])->len;
         ofs_skip[(wrk_ary+cntr2)->level] = 0;
   }
   else
   {
            (wrk_ary+cntr2)->offs = (wrk_ary+cntr2-1)->offs+
                                    (wrk_ary+cntr2-1)->len;
            if( (wrk_ary+cntr2)->type >= OC_SEG )
            {

               (wrk_ary+cntr2)->offs = 0;

            }
         }
}
for (cntr2=1;cntr2 <= cntr;cntr2++)
{
   if ((wrk_ary+cntr2)->level > (wrk_ary+cntr2-1)->level)
   {
      top  [(wrk_ary+cntr2)->level]   = cntr2;
      skip [(wrk_ary+cntr2-1)->level] = cntr2 -1;
   } else if ((wrk_ary+cntr2)->level < (wrk_ary+cntr2-1)->level)
      {
         for (cntr3=MAX_LEVEL;
              (cntr3 > (wrk_ary+cntr2)->level);
              cntr3--
             )
         {
            if (top [cntr3] > 0)
            {
               if (skip [cntr3] > 0)
               {
                  (wrk_ary+top [cntr3])->prev = skip[cntr3];
                  (wrk_ary+skip[cntr3])->next = top [cntr3];
               } else
                  {
                     (wrk_ary+top [cntr3])->prev = cntr2 -1;
                     (wrk_ary+cntr2-1)->next = top [cntr3];
                  }
               skip [cntr3] = 0;
               top  [cntr3] = 0;
            }
            if (cntr3 < 0)
            {
               break;
            }
         }
         if (skip[(wrk_ary+cntr2)->level] > 0)
         {
            (wrk_ary+skip[(wrk_ary+cntr2)->level])->next = cntr2;
            (wrk_ary+cntr2)->prev = skip [(wrk_ary+cntr2)->level];
            skip [(wrk_ary+cntr2)->level] = 0;
         }
      } else if ((wrk_ary+cntr2)->prev == 1)
         {
            (wrk_ary+cntr2-1)->next = top [(wrk_ary+cntr2-1)->level];
            (wrk_ary+top[(wrk_ary+cntr2-1)->level])->prev = cntr2-1;
            top [(wrk_ary+cntr2-1)->level] = 0;
            skip[(wrk_ary+cntr2-1)->level] = 0;

         } else
            {
               (wrk_ary+cntr2)->prev = cntr2-1;
               (wrk_ary+cntr2-1)->next = cntr2;
            }

}

for (cntr3=MAX_LEVEL;(cntr3 >= 0);cntr3--)
{
   if (top [cntr3] > 0)
   {
      if (skip [cntr3] > 0)
      {
         (wrk_ary+top [cntr3])->prev = skip[cntr3];
         (wrk_ary+skip[cntr3])->next = top [cntr3];
      } else
         {
            (wrk_ary+top [cntr3])->prev = cntr2 -1;
            (wrk_ary+cntr2-1)->next = top [cntr3];
         }
      skip [cntr3] = 0;
      top  [cntr3] = 0;
   }
   if (cntr3 < 0)
   {
      break;
   }
}

#if  PROD_SYS

   ;

#else


   for (  cntr2 = 1;  cntr2 <= cntr;  cntr2++  )
   {


   }

#endif


odt_top  = (ODT *) os_mema (sizeof (odtrecord)*(cntr+3));
odt_fill = odt_top;
n        = 0;

odt_add ( & odt_fill,
          & n, obj_name, 0, 0, 0, 0, USER_OBJ, & (wrk_ary->priv) );

for (cntr2=1;cntr2 <= cntr;cntr2++)
{

   odt_add (& odt_fill, & n,
              (wrk_ary+cntr2)->name,(wrk_ary+cntr2)->level,
              (wrk_ary+cntr2)->offs,(wrk_ary+cntr2)->next,
              (wrk_ary+cntr2)->prev,(wrk_ary+cntr2)->type,
            & ((wrk_ary+cntr2)->priv)
           );
   if ((wrk_ary+cntr2)->level == 0)
   {
      level_zero++;
   }
}
odt_lvl (odt_top) = level_zero;
odt_off (odt_top) = n-1;
odt_nxto(odt_top) = (hword) obj_isv;
odt_prvo(odt_top) = (hword) num_inhr;
odt_type(odt_top) = (hword) obj_type;


rc = odt_save ( odt_top, obj_name );

/*
 |
 |   Object Definition Table is built and saved.  Now deallocate
 |   everything we needed.
 |
 */


for (  cntr=0;  cntr < ODT_WORK_TABLE_SIZE;  cntr++  )
{

      s_drop ( &(wrk_ary+cntr)->name );

}

   s_drop (  &p_template);
   s_drop (  &p_delim   );
   s_drop (  &p_spill   );
   s_drop (  &type      );
   s_drop (  &priv_str  );
   s_drop (  &key       );
   s_drop (  &len_str   );
   s_drop (  &left_b    );
   s_drop (  &rght_b    );
   s_drop (  &privs     );
   s_drop (  &obj_name  );

   os_memd(  wrk_ary    );
   s_vdrop(  &parse_vec );

   odt_dini();


   return(rc);

}

