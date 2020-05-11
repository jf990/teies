/*+-------------------------------------------------------------------+
  |  <Doc>                                                            |
  |  <NAME>    OBJ_X H                                                |
  |  <AUTHOR>  JF                                                     |
  |  <DATE>    March 1, 1989                                          |
  |  <LEVEL>   OBJ                                                    |
  |  <PURPOSE> Define all the routines that make up the object level  |
  |            internal procedures.  Object/Actions are defined in    |
  |            OBJ_PRIM.H.                                            |
  |  </Doc>                                                           |
  +-------------------------------------------------------------------+*/


/* $EXTRACT    $MAKER-PROTO                                            */

/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
/* Routines that work with the Object Definition Tables {ODT}          */
/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */

extern ODT   * odt_find ( OCB * ocbptr, OCD * ocd ) ;

                                    /*Given and OCD and OCB, find the  */
                                    /*pointer to the OCD's entry in the*/
                                    /*ODT. Parameters are:             */
                                    /* struct OCB ocb;                 */
                                    /* struct OCD ocd;                 */
                                    /*                                 */
                                    /*                                 */

extern ODT  * odt_ptr  ( hword obj_type ) ;

                                    /*Locate the ODT for a given Object*/
                                    /*type. If the ODT cannot be found,*/
                                    /*load it from the database. Args: */
                                    /*   hword object_type;            */
                                    /*                                 */

extern sint odt_make ( OCB * ocb, OCD * ocd ) ;

                          /*Build and save and ODT from source text    */

extern sint odt_init ( void ) ;

                          /*Used by odt_make{} to initialize tables.   */

extern sint odt_dini ( void ) ;

                          /*Used by odt_make{} to deallocate tables.   */

extern sint odt_add( ODT **odt, sint *n, string str, byte lvl, hword off,
                       hword nxt, hword prv, hword type, PRIVS * priv ) ;

                          /*Add 1 element to a new ODT.                */

extern sint odt_disp ( odtrecord * odt_entry ) ;

                          /*Print out an ODT.                          */

extern ODT *odt_load ( hword object_type ) ;

                          /*Load an ODT from object or OS file.        */

extern sint odt_link ( ODT * odt_start ) ;

                          /*Resolve ODT inheritance links.             */

extern sint odt_save ( odtrecord * odt, string name ) ;

                          /*Save an ODT as an object or OS file.       */

extern sint odt_sgln ( ODT * current, hword obj_type ) ;

                          /*Compute a segment length from the odtentry.*/

extern sint odt_len  ( ODT * odt_entry, hword obj_type ) ;

                          /*Given a pointer to an ODT entry, compute  */
                          /*the length {sizeof} that components data. */
                          /*Returns the length. Args are:             */
                          /*  struct ODT *odt_entry;                  */
                          /*  hword      obj_type;                    */

extern boolean odt_comp( ODT * component_entry, string string_to_check );

                          /*Given a pointer to an ODT entry, compare a*/
                          /*string to see if it matches.              */
                          /*Returns TRUE if they match.  Args are:    */
                          /*  struct ODT *odt_entry;                  */
                          /*  string      string_to_check;            */

extern sint    odt_drop( hword object_type );

                          /*Given the type of object release its re-  */
                          /*spective ODT from memory.                 */

/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
/* Routines that work on Object Component Descriptors {OCDs}           */
/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */

extern sint ocd_ini  ( OCB * ocb, OCB * ocd ) ;

                          /*Initialize an object component to default */
                          /*contents.  Parameters are:                */
                          /* struct OCB *ocb;                         */
                          /* struct OCD *ocd;                         */
                          /*                                          */

extern sint ocd_dbp  ( OCB * ocb, OCD * ocd, string * db_path ) ;

                          /*Compute the DataBasePath {DBP} for a given*/
                          /*OCD. Parameters are: {returns error}      */
                          /* struct OCB *ocb;                         */
                          /* struct OCD *ocd;                         */
                          /* string     *dbp;                         */
                          /*                                          */

extern sint ocd_next ( OCB * ocb, OCD ** ocd, void * data ) ;

                          /*REturn the next OCD and associated data   */
                          /*from the current OCD in the object.       */
                          /*     Parameters are: {returns error}      */
                          /* OCB        *ocb;                         */
                          /* OCD       **ocd;                         */
                          /* byte       *data;                        */
                          /*                                          */

extern sint ocd_prev ( OCB * ocb, OCD ** ocd, void * data ) ;

                          /*Return the previous OCD & associated data */
                          /*from the current OCD in the object.       */
                          /*     Parameters are: {returns error}      */
                          /* OCB        *ocb;                         */
                          /* OCD       **ocd;                         */
                          /* byte       *data;                        */
                          /*                                          */

extern ODT *  ocd_root ( OCB * ocb, OCD * ocd ) ;

                          /*Return the root ODT for the OCD provided. */

extern sint ocd_len  ( OCB * object, OCD  * component );

                          /*Compute the length of the data for a given*/
                          /*OCD. Parameters are: {returns sint length}*/
                          /* OCB        * object;                     */
                          /* OCD        * component;                  */
                          /*                                          */

extern boolean ocd_isvar( ODT * odtptr, hword obj_type );
#define    ocd_is_variable_length                          ocd_isvar

                           /*Determine if the Component is a variable  */
                          /*length string or segment.  Args are:      */
                          /* struct ODT *odt_entry;                   */
                          /* hword      obj_type;                     */
                          /*                                          */

extern sint ocd_sgln ( ODT * current, hword obj_type ) ;

                          /*Compute the length of the segment the OCD */
                          /*is a parent of       {returns sint length}*/
                          /* struct ODT *odt_entry;                   */
                          /* hword      obj_type;                     */
                          /*                                          */

extern boolean ocd_key( odtrecord * ocd_ptr );

                          /*Determine if the ocd given belongs to a   */
                          /*key of an indexed segment.  Parameters are*/
                          /*    odtrecord    *ocd_ptr;                */
                          /*                                          */

extern sint ocd_dcd  ( OCD * ocd_ptr ) ;

                          /*Takes an OCD name and tries to decipher   */
                          /*possible index key and bias information if*/
                          /*it exists in the OCD string. Results are  */
                          /*placed in ocd->name, ocd->key, ocd->bias. */
                          /*    struct OCD *ocd;                      */
                          /*                                          */
                          /*Returns 0 unless there is something wrong */
                          /*with the OCD name or ocd structure. DOES  */
                          /*NOT VALIDATE THE OCD !!                   */
                          /*                                          */
                          /*                                          */

extern sint ocd_ctos ( OCB * ocb, OCD * ocd, string * buffer ) ;

                          /*OCD Component TO String conversion. Takes */
                          /*any component and converts it into a TEIES*/
                          /*string.  Returns error. Args are:         */
                          /*  struct OCB *ocb;                        */
                          /*  struct OCD *ocd;                        */
                          /*  string     *results;                    */
                          /*                                          */

extern sint ocd_stoc ( OCB * ocb, OCD * ocd, string data ) ;

                          /*Convert a string to an OCD component and  */
                          /*replace it in the object requested.       */
                          /*         Returns error. Args are:         */
                          /*  struct OCB *ocb;                        */
                          /*  struct OCD *ocd;                        */
                          /*  string      data;                       */
                          /*                                          */

extern OCD * ocd_new ( OCD ** ocd_ptr, byte * component ) ;

                          /* Allocate a new OCD structure given       */
                          /*a pointer to a current OCD. If NIL, create*/
                          /*it, else clear it out for the new one.    */
                          /*Return a pointer to the new struct {as wel*/
                          /*as update the argument}. Args are:        */
                          /*    struct **ocd;                         */
                          /*    byte    *name;                        */
                          /*                                          */
                          /*                                          */
extern OCD  * ocd_drp ( OCD ** ocd_ptr ) ;

                          /* deallocate an OCD structure, Args        */
                          /*are:                                      */
                          /*  struct OCD **ocd;                       */
                          /*                                          */
                          /*A NIL pointer should be returned as well  */
                          /*as the argument ptr being NILed out.      */
                          /*                                          */

extern sint ocd_bse  ( OCB * ocb, OCD * ocd, OCB * gfd ) ;

                          /* Convert local file to BSE object contents*/
                          /*and place resulting segments in the OCD in*/
                          /*the object specified by OCB.              */
                          /* Args are:                                */
                          /*   struct OCB *ocb;                       */
                          /*   struct OCD *ocd;                       */
                          /*   string      local_file_name;           */

extern sint ocd_cont ( OCB * ocb, OCD * ocd, OCB * gfd ) ;

                          /* Determine where the contents of the obj- */
                          /* ect are, in the object of linked to a    */
                          /* contents object.  Calls ocd_bse once     */
                          /* everything is found.                     */

extern sint ocd_gfd  ( OCB * ocb, OCD * ocd, OCB * gfd ) ;

                          /* Convert object components to OS File     */
                          /* format.  {Copy from object to file}.     */

extern sint ocd_pack ( string * packet, OCD * ocd ) ;

                          /* Pack an OCD into a single string of the  */
                          /* format:  OFFSET|OCD->NAME|OCD->KEY       */
                          /* Args are:                                */
                          /*                                          */
                          /*   string     *packet;                    */
                          /*   OCD        *ocd;                       */

extern sint ocd_unpack( string packet, OCD ** ocd );

                          /* Unpack a string packet formatted by ocd_ */
                          /* pack into an OCD.                        */
                          /* Args are:                                */
                          /*                                          */
                          /*   string      packet;                    */
                          /*   OCD       **ocd;                       */

extern sint ocd_send( OCB * object, OCD * component, string * packet,
                                                         hword * flags );

                          /* Pack an OCD into DMS_PAK{} format.       */
                          /* Args are:                                */
                          /*                                          */
                          /*   OCB        *object;                    */
                          /*   OCD        *component;                 */
                          /*   string     *packet;                    */
                          /*   hword      *flags;                     */
                          /*                                          */
                          /* Returns std OBJ error code.              */
                          /*            Flags are defined as:         */

#define OCD_SEND_START              ( (hword)  0 )
#define OCD_SEND_FIRST_AND_LAST     ( (hword) 91 )
#define OCD_SEND_FIRST_OF_MANY      ( (hword) 92 )
#define OCD_SEND_LAST_OF_MANY       ( (hword) 93 )
#define OCD_SEND_ONE_OF_MANY        ( (hword) 94 )
#define OCD_SEND_SEQUENCE_ERROR     ( (hword) 95 )

extern sint ocd_recv( OCB * object, string packet );

                          /* Unpack a packet of DMS_PACK{} format in  */
                          /* to an object.                            */
                          /* Args are:                                */
                          /*                                          */
                          /*   OCB        *object;                    */
                          /*   string      packet;                    */
                          /*                                          */
                          /* Returns std OBJ error code.              */

/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
/* Routines that operate on Object IDentifiers {OIDs}                 */
/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */


extern sint oid_new  ( OID * oid ) ;

                          /*Allocate and create a new Object ID. Args:*/
                          /*  struct OID *oid;                        */
                          /*                                          */
                          /*Returns error. If *oid is NIL then it will*/
                          /*be allocated, else overwritten.           */
                          /*                                          */

extern sint oid_key  ( OCD * ocd, OID * oid ) ;

                          /* Move an OID into an OCD key field        */

extern sint oid_merge( OID ** target_oid_array,
                       sint * length_of_target_oid_array,
                       OID *  source_oid_array,
                       sint   length_of_source_oid_array,
                       sint   limit,                       sint flags );

                          /*Given two arrays containing a list of OIDs*/
                          /*merge them together and eliminate any in- */
                          /*tersection between the two arrays.  The   */
                          /*result is stored in the first array.  The */
                          /*function returns any result code.         */

extern sint oid_sort( OID ** array_to_sort, OCD * component,
                                      sint length_of_array, sint flags );

                          /*Sort an OID array given an OCD to sort by.*/
                          /*The OID array will be sorted by the data  */
                          /*of each Component.                        */

/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
/* Routines that operate on the System OID Index {SOIDI}              */
/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */


extern sint soidi_gt( OID *oid, hword *lfd, tint *goid, hword *obj_type);

                          /*                                          */

extern sint soidi_ad( OID * oid, hword lfd, tint goid, hword obj_type );

                          /*                                          */

extern sint soidi_de( OID * oid ) ;

                          /*                                          */
/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
/* Routines that work on the Principle System Index {PSI}             */
/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */

extern sint psi_add  ( hword obj_obj, string obj_name ) ;

                          /*Add an object into the PSI                */
                          /*                                          */

extern sint psi_del  ( string system_id ) ;

                          /* Remove a PSI entry given its ID.         */
                          /*                                          */

/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
/* Routines that work on the System Mail Index { SMI }                */
/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */

extern sint smi_add( OID * mail_oid, string mail_id, string mail_label,
                                                          hword flags ) ;

                          /*Add a mail item to the SMI keyed by the   */
                          /*mail id and mail label {provided}. Returns*/
                          /*the number of mail items in the index or a*/
                          /*object level error code if negative. Args:*/
                          /*   mail_oid           OID *               */
                          /*   mail_id            string              */
                          /*   mail_label         string              */

extern sint smi_get( OID **mail_oid, string mail_id, string mail_label );

                          /*Locate a mail OID in the SMI given either */
                          /*the ID key or the Label key.              */
                          /*   mail_oid           OID **              */
                          /*   mail_id            string              */
                          /*   mail_label         string              */
                          /*Returns 0 if found else obj level error.  */

extern sint smi_qry  ( hword flag ) ;

                          /*Return the number of mail items in the SMI*/
                          /*   * no arguments *                       */



/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
/* Routines that operate on Object Control Blocks {OCBs}              */
/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */

extern OCB * ocb_inta( OID * oid, hword mode ) ;

                          /*Locate the OCB requested by passing the   */
                          /*pointer to an OID.  If -1, return the last*/
                          /*OCB in the list.  If mode is non-zero then*/
                          /*request a specific open mode.             */

extern OCB *   ocb_gfd( string * gfd_name, hword mode ) ;

                          /*Locate the OCB requested by passing the   */
                          /*GFD name.  If -1, return the last OCB in  */
                          /*the table.        If mode is non-zero then*/
                          /*request a specific open mode.             */

extern sint    ocb_drp  ( OCB * ocb ) ;

                          /*Deallocate OCB requested and release all  */
                          /*associated memory, then remove from table.*/
                          /*Arg:    struct * OCB                      */
                          /*                                          */

extern OCB   * ocb_all( void );

                          /* Allocate an object control block and     */
                          /*link it into the OCB table. No args.      */
                          /*Returns a pointer to an empty OCB.        */
                          /*                                          */

extern sint    ocb_segflush(  OCB * object, byte request_flag  );

#define   OCB_CACHE_FLUSH            17
#define   OCB_CACHE_CLEAR             0

                          /* Write the segment cache to the DMS.      */
                          /* FLUSH only writes to the dms and leaves  */
                          /* memory and flags set, CLEAR writes,      */
                          /* clears and deallocates everything.       */

extern sint    ocb_setcache(  OCB  * object,
                              byte * segment, sint  segment_length );

                          /* Set up the segment cache for the object  */
                          /* and component requested.  If the segment */
                          /* pointer is NIL but a length is given, the*/
                          /* cache will still be allocated but no data*/
                          /* copied into it.                          */



/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
/* Routines that operate on Objects given their OCB                   */
/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */


extern OCB *obj_all  ( hword obj_type, byte location, OID * oid ) ;

#define obj_allocate obj_all
                          /* Allocate a new object, declare it either */
                          /* local or remote, and return the OCB of   */
                          /* the new object.                          */

extern OCB *obj_opn  ( OID * oid, hword mode ) ;

#define obj_open obj_opn
                          /* Given an OID, open the object and return */
                          /* the OCB.                                 */

extern OCB *  obj_reco ( OID * object_id );

#define obj_recover obj_reco
                                    /* If the OID is in the deletion  */
                                    /* queue, remove it, restore its  */
                                    /* links, and return it as an open*/
                                    /* object.                        */

extern OCB *  obj_own  ( OCB * ocb ) ;

                                    /* Return the owner of the object */
extern OID ** obj_lnks ( OCB * ocb ) ;

                                    /* Return all links of the object */

extern sint obj_alk( OCB * ocb, byte * component, OCB * ocb_to_link,
                                                      hword link_type ) ;

                                    /* Add a link to a specific com-  */
                                    /* ponent of an object under the  */
                                    /* %LINKS array.             Args:*/
                                    /*                                */
                                    /* OCB *    ocb_to_link;          */
                                    /* String   component;            */
                                    /* OCB      ocb_to_link_to;       */
                                    /* hword    link_type;            */
                                    /*                                */
                                    /*           Returns Sint result  */
                                    /*           code.                */
                                    /*                                */

extern sint obj_glk( OCB * ocb, string component, OID ** oid_to_return,
                  sint link_pos, hword link_type, hword * object_type ) ;

                                    /* Get a link from a specific com-*/
                                    /* ponent of an object under the  */
                                    /* %LINKS array.             Args:*/
                                    /*                                */
                                    /* OCB *    ocb_to_get_from;      */
                                    /* String   component;            */
                                    /* OID **   oid_to_return;        */
                                    /* hword    type_of_link;         */


#define ANY_TYPE_OF_LINK       ( (hword) 0 )
#define STANDARD_LINK          ( (hword) 1 )
#define UNRESTRICTED_LINK      ( (hword) 1 )
#define PRIVILEGE_LINK         ( (hword) 2 )
#define ACCESS_LINK            ( (hword) 3 )
#define TICKET_LINK            ( (hword) 4 )
#define OWNER_LINK             ( (hword) 5 )
#define RECIPIENT_LINK         ( (hword) 6 )
#define AUTHOR_LINK            ( (hword) 7 )
#define COURTESY_COPY_LINK     ( (hword) 8 )
#define BLIND_COPY_LINK        ( (hword) 9 )
#define RESTRICTED_LINK        ( (hword)10 )
#define REVERSE_LINK           ( (hword)11 )

                                    /* hword *  object_type;          */
                                    /*                                */
                                    /*           Returns Sint result  */
                                    /*           code.                */
                                    /*                                */

extern sint obj_gaol( OCB * object, byte * component, sint * oid_array,
                            sint * oid_array_size, hword type_of_link ) ;

                                    /* Get all links from a specific  */
                                    /* component link index of a given*/
                                    /* link type.                Args:*/
                                    /*                                */
                                    /* OCB *    ocb_to_get_from;      */
                                    /* String   component;            */
                                    /* OID ***  oid_array_to_return;  */
                                    /* sint *   size_of_array;        */
                                    /* hword    type_of_link;         */

extern sint obj_rlk( OCB * ocb, string component, OID * oid_to_remove ) ;
                                    /* Remove a specific link under   */
                                    /* the %LINKS array.         Args:*/
                                    /*                                */
                                    /* OCB *    ocb_to_remove_link;   */
                                    /* String   component;            */
                                    /* OID *    oid_of_link;          */
                                    /*                                */
                                    /*           Returns Sint result  */
                                    /*           code.                */
                                    /*                                */

extern struct dms_cb *obj_dms  ( hword lfd, tint goid ) ;

                                    /*                                */

extern sint obj_fmt( hword lfd, sint number_of_blocks ) ;

                                    /*  lfd, lfd-blk-size             */

extern sint obj_next ( OCB * cocb, OCD * ocd, void ** dataptr,
                        sint offset, hword what_to_get, sint * length ) ;
                          /* Locate the next OCD and associated data  */
                          /* given a current OCD.  Params are:        */
                          /*    OCB     *ocb;                         */
                          /*    OCD     *ocd;                         */
                          /*    byte   **data;                        */
                          /*    sint     offset;                      */
                          /*    hword    what_to_get;                 */
                          /*    sint     len_of_data;                 */

extern sint obj_pack ( string * senddata, OCB * ocb, OCD * ocd ) ;

                          /* Build a packet out of the contents of an */
                          /* object.                                  */
                          /*    string  *packet;                      */
                          /*    OCB     *ocb;                         */
                          /*    OCD     *ocd;                         */

extern sint obj_unpack( OCB ** ocb, string data );

                          /* Replace or create an object from a packet*/
                          /* object.                                  */
                          /*    string  *packet;                      */
                          /*    OCB     *ocb;                         */

extern hword obj_type( OID * oid_to_check ) ;

                          /*Given an OID return the type of object.   */
                          /*    OID     *oid_to_check;                */

extern sint  obj_beg  ( hword lfd ) ;

                          /*Start the object level and DMS on LFD 1.  */

extern sint  obj_ini  ( void ) ;
                          /*Define the Object level global structures */

extern sint  obj_tini( void ) ;
                          /*Define the table of objects for lookups   */

extern hword obj_inta( string object_name ) ;

#define      obj_name_in_table               obj_inta
                          /*Determine is an object is in the table.   */
                          /* Argument is a string with the name to    */
                          /* verify, return is halfword object type or*/
                          /* 0 if not found.                          */

extern string obj_name( hword object_type );

                          /*Determine is an object is in the table    */
                          /*given an object type return the object    */
                          /*name string.  Do not drop the resulting   */
                          /*string unless you s_copy{} it first.      */

extern sint  obj_end  ( void ) ;
                          /* Shut down the object level and deallocate*/
                          /* all data structures, call dms_end{}.     */

extern sint  obj_sync ( void ) ;
                          /* Synchronize the data base by closeing all*/
                          /* open objects and writing out the PSI and */
                          /* SOIDI.  The PSI and SOIDI are not closed.*/
                          /* No parameters and always returns 0.      */

extern sint  obj_cls  ( OCB * ocb ) ;

#define      obj_close                  obj_cls
                          /* Close and object opened with obj_opn{}.  */
                          /* Args are:                                */
                          /*               OCB   *ocb;                */
                          /*               hword  mode;               */
                          /*                                          */
                          /* Returns sint return code, 0 if closed ok */
                          /* or std obj level code if exception exists*/
                          /* Mode should be either OBJ_UPDATE when the*/
                          /* object is open for update, OBJ_READ to   */
                          /* not commit any updates, or OBJ_FLUSH to  */
                          /* commit updates and leave the object open */
                          /* for further work.                        */

extern sint  obj_rep  ( OCB * ocb, OCD * ocd, void * buffer, byte flag );

#define      obj_replace       obj_rep
#define  obj_replace_cache( ocb )  obj_rep( ocb, NIL, NIL, SEG_UPDATE )
                          /* Replace an object component.  Arguments: */
                          /*                                          */
                          /*   OCB   *object_control_block;           */
                          /*   OCD   *component;                      */
                          /*   byte  *data;                           */
                          /*   sint   flags;                          */
                          /*                                          */
                          /* Flags currently are SEG_HOLD which means */
                          /* this segment will receive more replace-  */
                          /* ments so hold it in memory until         */
                          /* SEG_UPDATE is requested.  SEG_HOLD is    */
                          /* potentially dangerous in that it is lost */
                          /* if ANY operation on that object that uses*/
                          /* a differnt segment will throw it away.   */
                          /* SEG_UPDATE guarentees it get stored prop-*/
                          /* erly.                                    */

extern sint obj_upd  ( OCB * ocb, byte * ocdname, void * data ) ;
#define     obj_update  obj_upd

                          /*Update an object given OCB, component and */
                          /*data.  Returns obj_rep{} return code.     */

extern sint obj_copy ( OCB * destination_ocb,
                       OCD * destination_ocd,
                       OCB * source_ocb,
                       OCD * source_ocd       ) ;

                          /* Copy one object or portion of an object  */
                          /* to another.  Parameters are:             */
                          /*                                          */
                          /* Returns a standard object level result   */
                          /* code or 0 if everything performed ok.    */

extern boolean   obj_comp ( OCB * destination_ocb,
                            OCD * destination_ocd,
                            OCB * source_ocb,
                            OCD * source_ocd       ) ;

                          /* Compare the data of one component of an  */
                          /* object to that of another.  Heterogeneous*/
                          /* components may be compared.  Returns TRUE*/
                          /* if the two components are binary equal,  */
                          /* FALSE otherwise.                         */

#define     obj_compare    obj_comp

extern boolean   obj_isnull( OCB * object,
                             OCD * component  );

                          /* Return TRUE if the data of the component */
                          /* is NULL.  NULL is the case when the data */
                          /* is all NULL bytes or the segment contain-*/
                          /* ing the component does not exist.        */

extern boolean   obj_iskey( OCB * object,
                            OCD * component  );

                          /* Return TRUE if the component if the object*/
                          /* is a member of the index key.             */


extern sint obj_get  ( OCB * ocb, OCD * ocd, void * buffer ) ;

                          /*  Return the data requested by the        */
                          /*  component of the specified object.      */

extern sint obj_find ( OCB * ocb, byte * ocdname, void * data ) ;

                          /*Find data associated to a OCD name. Return*/
                          /*obj_get{} return code.  Params are:       */
                          /*   OCB *ocb;                              */
                          /*   byte *ocdname;                         */
                          /*   byte *data_buffer;                     */
                          /*                                          */
                          /*                                          */

extern sint obj_enc  ( string * packet, OCB * ocb, OCD * ocd ) ;

                          /*                                          */

extern sint obj_dec( string packet, OCB * ocb, hword * obj_type,
                       OID ** oid, boolean ok_to_link, string * result );

                          /*                                          */

extern sint obj_lnk  ( OCB * ocb1, OCB * ocb2, OCD * ocd1, OCD * ocd2 ) ;

                          /*                                          */
                          /*                                          */
extern sint obj_del  ( OID * oid ) ;

#define obj_delete obj_del
                          /* Takes oid pointer and attempts to delete */
                          /* the object if it is not open and it is   */
                          /* in the local SOIDI.  Returns result code.*/
                          /*                                          */

extern sint obj_rmv  ( OCB * ocb, OCD * ocd ) ;

#define obj_remove obj_rmv
                          /* Takes OCD, OCD, and LOCATION to delete,  */
                          /* if OCD is NIL then the whole object is   */
                          /* deleted. If OCD addresses a segment, the */
                          /* whole segment is removed. If OCD is a com*/
                          /* ponent then that component is NULLed out.*/
                          /* If the object is a remote object then the*/
                          /* request is passed on to the MasterServer.*/
                          /*                                          */
                          /*                                          */

extern sint obj_move  ( void );

                          /*                                          */

extern sint obj_unlink( OCB * ocb1, OCB * ocb2, OCD * ocd1 );

                          /*                                          */
                          /*                                          */

extern string obj_grec( OCB * ocb, OCD * ocd, string * line );

#define obj_get_record obj_grec
                          /*Given an OCB, OCD and buffer returns the  */
                          /*  next available record of text.  Assumes */
                          /*  the OCD points to a contents type of    */
                          /*  segments.  Returns a string pointer as  */
                          /*  well as fills in the buffer parameter.  */
                          /*  Returns S_NULL if no more records.      */
                          /*                                          */

extern sint obj_sysndx( hword system_index_type, OCD * object_component,
           void * data, sint  data_length, hword flag, void  ** result );

                          /*  Update or retrieve a component in a     */
                          /*  system index on another server if the   */
                          /*  invoker has the proper privilege to do  */
                          /*  so. Args are:                           */
                          /*                                          */
                          /*    hword    object_type;                 */
                          /*    OCD     *ocd;                         */
                          /*    void    *data;                        */
                          /*    sint     data_length;                 */
                          /*    hword    flag;                        */
                          /*    void   **result;                      */
                          /*                                          */
                          /*  Flag can be either UPDATE_SYSTEM_INDEX  */
                          /*  or LOCATE_IN_SYSTEM_INDEX {tm.h}.       */
                          /*                                          */

extern sint obj_item( string   list_of_items,
                      hword    level,
                      OID   ** oid,
                      OCB   ** list_ocb,
                      string * list_of_bad_items,
                      sint   * number_of_bad_items,
                      hword    default_object_type                    );

                          /*  Convert a string to an object ID.       */
                          /*  Levels are:                             */
                          /*                                          */


#define    OBJ_NEED_ONE_OID    ((hword) 1)
#define    OBJ_NEED_MANY_OID   ((hword) 2)

#define obj_count obj_coun
extern sint obj_coun ( OCB * ocb, OCD * ocd );

                          /*  Return the number of segments under a   */
                          /*  component in an object.                 */

#define obj_sdeli obj_sdel
extern sint obj_sdel ( OCB      ** object,
                       hword       object_type,
                       hword       level,
                       OID      ** object_id,
                       datetime ** date_of_deletion,
                       OID      ** who_deleted
                     );

                          /*  Return information in the SDELI based on*/
                          /*  level request.                          */


#define    SDELI_FIND_BY_OBJECT_TYPE  ((hword) 0 )
#define    SDELI_FIND_OID_ONLY        ((hword) 1 )
#define    SDELI_FIND_BY_DELETOR      ((hword) 2 )
#define    SDELI_FIND_BY_DATE         ((hword) 3 )
#define    SDELI_ADD_TO_INDEX         ((hword) 4 )
#define    SDELI_REMOVE_FROM_INDEX    ((hword) 5 )
#define    SDELI_REMOVE_ALL_ENTRIES   ((hword) 6 )
#define    SDELI_QUERY_QUEUE_SIZE     ((hword) 7 )





extern sint obj_key ( OCD * component, void * key, sint length_of_key );

#define obj_set_key obj_key
                          /*  Convert data pointed to by the generic  */
                          /*  pointer KEY in to a key for the OCD     */
                          /*  specified.                              */


extern sint obj_cmod( OCB * object,  hword mode );

#define obj_change_mode obj_cmod

                          /*  Change the mode of an open object.      */

extern sint   obj_import( OCB * target, OCB * input );

                          /* Import a textual description of an object
                             as input in user level format and commands
                             and fill in the target object.           */


extern sint   obj_export( OCB * target, OCB * input );

                          /* Export an objects contents to a file to
                             contain a textual description of the object.
                             Can be read back into the database with
                             obj_import{}                             */

extern sint   obj_gtseg( OCB *, OCD *, string * );

#define obj_get_segment   obj_gtse

                          /* Get a DMS segment out of an object as    */
                          /* opposed to an individual component.  The */
                          /* length of the segment is returned as the */
                          /* s_len{} of the string.                   */

extern sint   obj_rpseg( OCB *, OCD *, string );

#define obj_replace_segment  obj_rpseg

                          /* Replace a DMS segment in an object as    */
                          /* opposed to an individual component.  The */
                          /* length of the segment is specified by the*/
                          /* s_len{} of the string.                   */

extern sint   obj_sndseg( OCB *object, OCD *component, string server );

#define obj_send_segment  obj_sndseg

                          /* Packet a segment and send it to another  */
                          /* server {who will call rcvseg}            */


extern sint   obj_rcvseg( string server, string packet );

#define obj_receive_segment  obj_rcvseg

                          /* Replace a DMS segment in an object that  */
                          /* was sent over from another server.       */

extern sint   obj_qns( OCB * object, byte * component );

#define obj_query_number_of_segments obj_qns

                          /* Return the number of segments stored in  */
                          /* a given segment of an object.            */

extern sint   obj_pos( OCB * object, OCD * component );

#define obj_segment_position         obj_pos

                          /* Return the relative segment position from*/
                          /* the beginning of the segment level.      */


extern sint   obj_id( OID * object, string * item_id );

#define object_identifier obj_id

                          /* Return the item id of an object given the*/
                          /* object's OID.                            */


extern sint obj_dlst ( void ) ;

                          /* TEST ROUTINE to dump OBJ Control Structs */


/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
/* The following files are part of the high level object level        */
/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */

extern sint perform  ( string object, string action, byte * args ) ;
extern sint cr_sys   ( void ) ;


/*                                                         $ENDEXTRACT */
