/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    var_tst.c                                              |
  |                                                                   |
  |  :AUTHOR.  John L. Foster                                         |
  |                                                                   |
  |  :DATE.    November 21, 1987                                      |
  |                                                                   |
  |  :LEVEL.   VAR                                                    |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.                                                          |
  |                                                                   |
  |  :PURPOSE. Test the variable level routines.                      |
  |                                                                   |
  |  :OUTPUT.  Always return 0.                                       |
  |                                                                   |
  |  :CALLS.                                                          |
  |                                                                   |
  |  :ERRORS.  None known.                                            |
  |                                                                   |
  |  :LIMITS.  As of this run, var_drp() does not do anything and the |
  |            variables are in a linear linked list (ie not sorted). |
  |  :END.                                                            |
  |                                                                   |
  +-------------------------------------------------------------------+*/


#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "vars.h"
#include "hash.h"
#include "debugi.h"
#include "teies.h"



main()


{

CELL     *hash_ptr = NIL;
CELL     **tab_ptr = NIL;

byte     *some_ptr = NIL;
OID       *oid_ptr = NIL;
sint      *num_ptr = NIL;
PRIVS     *prv_ptr = NIL;
OID       *test_ptr = NIL;
sint       tester  = 5;
string   a = S_NULL;
string   b = S_NULL;
string   c = S_NULL;
string   d = S_NULL;
string   e = S_NULL;
string   f = S_NULL;
string   g = S_NULL;
string   h = S_NULL;

hword test_key = 0x0104;
string  aa = S_NULL;
string  bb = S_NULL;
string  cc = S_NULL;
string  dd = S_NULL;
string  ee = S_NULL;

string str1 = S_NULL;
string str2 = S_NULL;
string str3 = S_NULL;
string str4 = S_NULL;
string str5 = S_NULL;


#D_main(  var_tst, VAR  );



s_init( &a, "Samir.Akron" );
s_init( &b, "Samir.Boston" );
s_init( &c, "Steve.Chicago" );
s_init( &d, "Steve.Denver" );
s_init( &e, "Steve.Eastwood" );
s_init( &f, "Fanny.Eastwood" );
s_init( &g, "George.Rochester" );
s_init( &h, "Hillary.Linden" );

s_init( &aa, "A variable a" );
s_init( &bb, "B variable b" );
s_init( &cc, "C variable c" );
s_init( &dd, "D variable d" );
s_init( &ee, "E variable e" );


t_tmini();


prv_ptr =  (PRIVS*) os_mema(sizeof(PRIVS));
os_memn(prv_ptr ,sizeof(PRIVS));
hash_ptr =  (CELL*) os_mema(sizeof(CELL));
os_memn(hash_ptr,sizeof(CELL));
test_ptr =  (OID*) os_mema(sizeof(OID));
os_memn(test_ptr ,sizeof(OID));
test_ptr->one = 4;
var_beg();


#D_show('TM_master->current->vars = 0x%x','TM_master->current->vars');
#D_show('*(TM_master->current->vars) = 0x%x',\
'*(TM_master->current->vars)');


var_set( a, aa);
var_set( b, bb);
var_set( c, cc);
var_set( d, dd);
var_set( e, ee);
prv_set (prv_ptr, test_key);

#if 0
var_assign( f,num_ptr, OC_INT);
#endif
var_assign( h,test_ptr, OC_OID);

var_assign( g,prv_ptr, OC_PRIV);
#D_priv('prv_ptr');

str1 = var_get ( a);
#D_show ( 'str1= 0x%x', 'str1');
#D_mdump( 'str1',16);

str2 = var_get( b ) ;
#D_show( 'str2=0x%x', 'str2');
#D_mdump( 'str2',16);

str3 = var_get( c )  ;
#D_show( 'str3= 0x%x', 'str3');
#D_mdump( 'str3',16);

str4 = var_get( d )   ;
#D_show( 'str4=0x%x', 'str4');
#D_mdump( 'str4',16);

str5 = var_get( e )    ;
#D_show( 'str5=0x%x','str5');
#D_mdump( 'str5',16);


var_clr( b );

var_drp( e );
var_drp( a );
var_drp( d );


os_memd(prv_ptr);
os_memd(hash_ptr);
os_memd(test_ptr);
s_drop( &a );
s_drop( &b );
s_drop( &c );
s_drop( &d );
s_drop( &e );
s_drop( &f );
s_drop( &g );
s_drop( &h );
s_drop( &aa );
s_drop( &bb );
s_drop( &cc );
s_drop( &dd );
s_drop( &ee );

t_shutdn();
#if 0
var_end(TM_master->current->vars);
#endif
#D_return( 0 );

}
