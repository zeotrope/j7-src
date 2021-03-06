/* ----------------------------------------------------------------------- */
/* J-Source Version 7 - COPYRIGHT 1993 Iverson Software Inc.               */
/* 33 Major Street, Toronto, Ontario, Canada, M5S 2K9, (416) 925 6096      */
/*                                                                         */
/* J-Source is provided "as is" without warranty of any kind.              */
/*                                                                         */
/* J-Source Version 7 license agreement:  You may use, copy, and           */
/* modify the source.  You have a non-exclusive, royalty-free right        */
/* to redistribute source and executable files.                            */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* Memory Management Utilities                                             */

#include "j.h"

#define FREE            free
#define MALLOC          (A)malloc
#define CALLOC          (A)calloc
#define NMEM            LONG_MAX

I          bytes;            /* bytes currently in used                    */
I          maxbytes;         /* "bytes" high-water mark of "bytes"         */  
I          totbytes;         /* bytes (re-)allocated since session start   */

I          tbase= -NTSTACK;  /* index of current frame                     */
static A*  tstack;           /* data portion of current frame              */
static A   tstacka;          /* current frame                              */
I          ttop=NTSTACK;     /* stack top (index into tstack[])            */

static A   traverse();

static F1(fr){
 RZ(w);
 if(--AC(w))R zero;
 bytes-=SZA(AT(w),AN(w),AR(w));
 FREE(w);
 R one;
}

static A cma(I n,I m){A z; I k=n*m;
 z=CALLOC(n,m);
 RZ(breaker());
 ASSERT(z,EVWSFULL);
 bytes+=k; totbytes+=k; maxbytes=MAX(bytes,maxbytes);
 R z;
}

static A ma(I m){A z;
 z=MALLOC(m);
 RZ(breaker());
 ASSERT(z,EVWSFULL);
 bytes+=m; totbytes+=m; maxbytes=MAX(bytes,maxbytes);
 R z;
}

static A traverse(A w,AF f){I n,*u;
 RZ(w);
 n=AN(w); u=AV(w);
 switch(AT(w)){
  case BOXK: n=n+n;
  case BOX:  {A *v=(A*) u; DO(n, f(*v++));} break;
  case VERB:
  case ADV:
  case CONJ: {V *v=(V*) u; f(v->f); f(v->g); f(v->h); f(v->s);} break;
  case SYMB: {SY*v=(SY*)u; DO(n, f(v->name); f(v->val); ++v);}
 }
 R one;
}

F1(fa){traverse(w,fa); R fr(w);}

F1(ra){RZ(w); traverse(w,ra); ++AC(w); R w;}


static A tg(void){A t=tstacka,z;
 RZ(z=ma(SZA(BOX,NTSTACK,1L)));
 AT(z)=BOX; AC(z)=AR(z)=1; AN(z)=*AS(z)=NTSTACK;
 tstacka=z; tstack=AAV(tstacka); tbase+=NTSTACK; ttop=1;
 *tstack=t;
 R z;
}

static A tf(void){A t=tstacka;
 tstacka=*tstack; tstack=AAV(tstacka); tbase-=NTSTACK; ttop=NTSTACK;
 R fr(t);
}

F1(tpush){
 RZ(w);
 traverse(w,tpush);
 if(ttop>=NTSTACK)RZ(tg());
 tstack[ttop]=w;
 ++ttop;
 R w;
}

I tpop(I old){while(old<tbase+ttop)1<ttop?fr(tstack[--ttop]):tf(); R old;}

A gc(A w,I old){ra(w); tpop(old); R tpush(w);}

void gc3(A x,A y,A z,I old){
 if(x)ra(x);    if(y)ra(y);    if(z)ra(z);
 tpop(old);
 if(x)tpush(x); if(y)tpush(y); if(z)tpush(z);
}


A ga(I t,I n,I r,I *s){A z;I m;
 ASSERT(r<=RMAX,EVLIMIT);
 RZ(z=cma(WP(t,n,r),SZI));
 AC(z)=1; AN(z)=n; AR(z)=r;
 AT(z)=0; tpush(z); AT(z)=t;
 if(1==r)*AS(z)=n; else if(r&&s)ICPY(AS(z),s,r);
 R z;
}

F1(ca){A z; RZ(w); GA(z,AT(w),AN(w),AR(w),AS(w)); MC(AV(z),AV(w),SZT(AT(w),AN(w))); R z;}

F1(car){I n;
 RZ(w=ca(w));
 n=AN(w);
 switch(AT(w)){
  case BOXK:
   n=n+n;
  case BOX:
   {A*v=AAV(w); DO(n, RZ(*v=car(*v)); ++v);}
   break;
  case VERB: case ADV: case CONJ:
   {V*v=VAV(w);
    if(v->f)RZ(v->f=car(v->f));
    if(v->g)RZ(v->g=car(v->g));
    if(v->h)RZ(v->h=car(v->h));
 }}
 R w;
}

B spc(){ A z; RZ(z=MALLOC(1000));  FREE(z); R 1; }
