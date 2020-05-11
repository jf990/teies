/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
<DOC>
<NAME>                             F_PARSE.C
<LEVEL>                            FORMS.PARSER
<AUTHOR>                           Heidi E. Harting, Nestor Voronka,
                                   John L. Foster, Stephen Muccione
<DATE>                             July 29, 1987

<PARAM>             form_id     a pointer to the form that is to be
                                parsed.

<OUTPUT>                        sint

<GLOBAL>
         TM_master->current  : current process record

<PURPOSE>
          This routine will get a line of text from the form indicated by
          form_id.  The text will then be searched for an open mark up
          tag.  If one is found, the closing mark up tag will be searched
          for in the same line of text.  If it is found, the text between
          the two tags is sent to FEXE_CMD.C for further pprocessing.  If
          the close tag is not found on this line of text, consecutive
          lines of text will be read from the item containing the form
          until the close tag is found.  At that time, all of the text
          between the tags will be pasted to FEXE_CMD.C.

          If no open tag is found in the string of text, the string is
          sent to FARG_VAR.C and is searched for variable names.

<CALLS>   os_mema.c  s_init.c  s_upper.c  s_first.c  s_copy.c
          s_trim.c  s_len.c  s_cat.c  fexe_cmd.c  farg_var.c
</DOC>
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "defs.h"
#include "str.h"
#include "debug.h"
#include "osk.h"
#include "bse.h"
#include "tm.h"
#include "form_i.h"

sint f_parse (ocb, ocd)

OCB          *ocb;
OCD          *ocd;

{
   struct  form_process * f_global = NIL;
   struct  f_fmat_s     * f_fmat   = NIL;
   struct  f_prof_s     * f_prof   = NIL;
   struct  f_prof_s     * f_deft   = NIL;
   string  line                    = S_NULL;
   string  line2                   = S_NULL;
   string  line_save               = S_NULL;
   string  blank                   = S_NULL;
   string  mup_tmpl                = S_NULL;
   string  var_tmpl                = S_NULL;
   string  deli_str                = S_NULL;
   string  spill_str               = S_NULL;
   string  tmp                     = S_NULL;
   string  garbage                 = S_NULL;
   string  * str_vec               = NIL;
   sint    rc                      = 0;
   sint    omp                     = 0;
   sint    var                     = 0;
   sint    amount_rendered         = 0;
   boolean eof                     = FALSE;
   boolean end_tag                 = FALSE;
   boolean did_render              = FALSE;
   char    end_mkup                = '\0';
   char    first_ch                = '\0';


   f_global    = (struct form_process *) TM_master->current->forms;

   f_prof      = (struct f_prof_s   * ) f_global->f_prof;
   f_deft      = (struct f_prof_s   * ) f_global->f_profdf;
   f_fmat      = (struct f_fmat_s   * ) f_global->f_fmat;

   s_init (& mup_tmpl," < @$n@+.>. @-.>. @~.. @n@+,=>,@?,=,@\"n,@-,>,.");
   s_init (& var_tmpl," & @n@-.;. ");
   s_valloc (& str_vec, 35 );


   eof=FALSE;
   while (  ! eof  )
   {

      if ( s_len(line) < 1 )
      {
         if (  (f_fmat->just == N_JUST)  &&   did_render   )
         {
            f_render (f_fmat, line, F_RENDER_CR, NIL);
         }
         if (obj_grec((OCB *) ocb, (OCD *) ocd, &line) == S_NULL)
         {
            eof = TRUE;
         }
         did_render = FALSE;
      }
      omp=s_first(f_prof->open,line,0,0);
      var=s_first(f_prof->bvar,line,0,0);
      if (((omp < var) && (omp != 0)) || ((omp != 0 ) && (var == 0)))
      {
         if (omp > 1)
         {
            s_clip (&tmp,&line,0,omp-1);
            if (f_fmat->just != N_JUST)
            {
               f_render (f_fmat, tmp, F_RENDER_CR, NIL);
            }
            else
            {
               f_render (f_fmat, tmp, F_RENDER_NCR, NIL);
            }
            did_render = TRUE;
         }
         rc=s_copy  (&line_save, line, 0, 0);
         rc=s_parse (&line,mup_tmpl,deli_str,str_vec,&spill_str);
         s_copy (&line,spill_str,0,0);
         if (rc == S_PARSE_CE)
         {
            if (fexe_cmd (str_vec, f_prof, f_fmat, f_deft) != 0)
            {
               s_clip (&line, &line_save, 0, 1);
               if (f_fmat->just != N_JUST)
               {
                  f_render (f_fmat, line, F_RENDER_CR, NIL);
               } else
               {
                  f_render (f_fmat, line, F_RENDER_NCR, NIL);
               }
               did_render = TRUE;
               s_copy (&line, line_save, 0, 0);
            }
         } else
         {
            if (f_fmat->just != N_JUST)
            {
               if (obj_grec ((OCB *) ocb, (OCD *) ocd,&line2) == S_NULL)
               {
                  eof = TRUE;
               }
               s_cat ( & line, line_save, line2, 0, 0 );
            } else
            {
               s_clip (&line, &line_save, 0, 1);
               f_render (f_fmat, line, F_RENDER_NCR, NIL);
               did_render = TRUE;
               s_copy (&line, line_save, 0, 0);
            }
         }
      } else
      if (((var < omp) && (var != 0)) || ((var != 0 ) && (omp == 0)))
      {
         if (var > 1)
         {
            s_clip (&tmp,&line,0,var-1);
            if (f_fmat->just != N_JUST)
            {
               f_render (f_fmat, tmp, F_RENDER_CR, NIL);
            } else
            {
               f_render (f_fmat, tmp, F_RENDER_NCR, NIL);
            }
            did_render = TRUE;
         }
         rc=s_copy  (&line_save,line,0,0);
         rc=s_parse (&line,var_tmpl,deli_str,str_vec,&spill_str);
         s_copy (&line,spill_str,0,0);
         if (rc == S_PARSE_CE)
         {
            did_render = TRUE;       /* TWEEK - SM 11/14/89 */
            if (f_sfield (str_vec [1], f_prof, f_fmat) != 0)
            {
               s_clip (&line, &line_save, 0, 1);
               if (f_fmat->just != N_JUST)
               {
                  f_render (f_fmat, line, F_RENDER_CR, NIL);
               } else
               {
                  f_render (f_fmat, line, F_RENDER_NCR, NIL);
               }
               did_render = TRUE;
               s_copy (&line, line_save, 0, 0);
            }
         } else
         {
            if (f_fmat->just != N_JUST)
            {
               if (obj_grec ((OCB *) ocb, (OCD *) ocd,&line2) == S_NULL)
               {
                  eof = TRUE;
               }
               s_cat ( & line, line_save, line2, 0, 0 );
            } else
            {
               s_clip (&line, &line_save, 0, 1);
               f_render (f_fmat, line, F_RENDER_NCR, NIL);
               did_render = TRUE;
               s_copy (&line, line_save, 0, 0);
            }
         }
      } else if ((omp == 0) && (var == 0))
      {
         f_render (f_fmat, line, F_RENDER_NCR, NIL);
         if( s_len( line ) < 1 )
         {

            did_render = TRUE ;

         }
         else
         {

            did_render = TRUE;

         }
         s_nulout (&line);

      }
   }

   s_drop( &  line          );
   s_drop( &  line2         );
   s_drop( &  line_save     );
   s_drop( &  blank         );
   s_drop( &  mup_tmpl      );
   s_drop( &  var_tmpl      );
   s_drop( &  deli_str      );
   s_drop( &  spill_str     );
   s_drop( &  tmp           );
   s_drop( &  garbage       );
   s_vdrop(&  str_vec       );

   return(0);
}
