/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

   if (len < 0)                   /* Check for negative length         */
   {
      s_nulout(target);
      #D_leave (S_BAD_LEN, 'length is negative') ;
   }

   if (beg == 0)                  /* Make sure beginning is 1          */
      beg = 1 ;

   ulene = s_len (end) ;          /* Get used length of source2        */
   if (len == 0)                  /* If len=0 search entire string     */
      len = ulene ;

   if (beg < 0)                   /* If beginning is negative compute  */
      beg = ulene + beg + 1 ;     /*   the positive value              */

   if (len > ulene - beg + 1 )    /* If length past end indicated cut  */
      len =  ulene - beg + 1 ;

   #D_show ('beg == %d len == %d','beg, len') ;

   if ( (beg <= 0) ||             /* Check for too negative beginning  */
      (beg > ulene) )             /* or beginning is past end          */
      len = 0 ;

   ulens = s_len(start);
   ulent = ulens + len ;

   if ( *target == start )        /* Start & target are identical!     */
      new_tar = FALSE;
   else
      s_drop(target);             /* Not identical                     */
   rc = s_alloc (target, ulent) ;
   if ( rc < 0 )
       #D_leave(rc,'s_alloc told s_cat it had an allocation error')

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
