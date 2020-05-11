/* Machine Independent Integer store/retrieve */

/* $extract */

tint     mii_set();  /* (*pos,length,value) */

tint     mii_qry();  /* (*pos,length,test_value) */

tint     mii_ts ();  /* (*pos,length);
                        "Test Set", store a test integer */

tint     mii_tq ();  /* (*pos,length);
                        "Test Query", query the test integer */

/* $end-extract */

