.c.obj :
    DosCl $* Str

str.lib       :                         \
                     sv_alloc.obj       \
                     sv_free.obj        \
                     sv_clear.obj

sv_alloc.obj  :      sv_alloc.c

sv_clear.obj  :      sv_clear.c

sv_free.obj   :      sv_free.c

