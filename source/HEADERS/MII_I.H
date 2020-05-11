/* Machine Independent Integer store/retrieve.
   The functions mii_ts and mii_tq will store and test an integer.
   The function mii_qry will take the result of mii_ts and will
   perform a byte conversion depending on how the byte is stored
   on the local machine.


   $extract */

tint     mii_set();  /* (*pos,length,value)
                        Set a machine independent integer */

tint     mii_qry();  /* (*pos,length,*test_value)
                        Get the value of a machine independent int */

tint     mii_ts ();  /* (*pos,length);
                        "Test Set", store a test integer */

tint     mii_tq ();  /* (*pos,length, test_value);
                        "Test Query", query the test integer */

hword    miitest();  /* $main */


/* $end-extract */

