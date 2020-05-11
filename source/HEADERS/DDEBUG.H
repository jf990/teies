#define d_proc 0x01
#define d_err 0x02
#define d_detail 0x04
#define d_diag 0x08
/* should return a 1 or a zero */
#define bit(x,y) (((x) & (y)) || ((x) & (y)))
#if (PROD_SYS)
;
#else
extern printf();
extern sint (*D_pr_d)();
#define D_pr (*D_pr_d)
extern hword D_lvl;
#endif
