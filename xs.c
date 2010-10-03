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
/* Scripts                                                                 */

#include "j.h"
#include "x.h"
#include "d.h"

C*         scad=0;     /* script ptr to start of next line */
C*         scpad=0;    /* script ptr to start of previous line */
C*         sczad=0;    /* script ptr to end of script */
static A   lastout;    /* previous outfile value           */
FILE*      outfile;    /* output file handle               */
B          tostdout=1; /* 1 if output to stdout            */

static FILE*sopen(w)A w;{A x;FILE*z;
 if(outfile)fclose(outfile);
 outfile=0; lastout=w;
 RZ(w);
 ASSERT(BOX&AT(w),EVDOMAIN);
 ASSERT(!AR(w),EVRANK);
 x=*AAV(w);
 ASSERT(CHAR&AT(x),EVDOMAIN);
 z=outfile=fopen(CAV(x),SAPPEND);
 ASSERT(z,EVFACE);
 R z;
}

static A line(a,w,tso)A a,w;B tso;{A x,xl;B xt=tostdout;C *p,*sp,*spp,*spz;
  DC ds;I ln,old;
 RZ(w);
 ASSERT(CHAR&AT(w),EVDOMAIN);
 sp=scad; spz=sczad; spp=scpad;
 scad=CAV(w); sczad=scad+AN(w); scpad=scad;
 ln=0;
 xl=lastout; sopen(a);
 RZ(ds=debadd(DCSCRP));
 tostdout=tso;
 old=tbase+ttop;
 while(1)
 {
   ds->ln=ln++; 	
   jerr=0; x=jgets("   ");
   if(!jerr&&!x)break;
   if(jerr)x=mtv; else immex(x);
   tpop(old);
 }
 scad=sp; scpad=spp; sczad=spz; tostdout=xt; sopen(xl);
 debz();
 R mtv;
}

static A linf(a,w,tso)A a,w;B tso;{A x;
 RZ(w);
 if(!(BOX&AT(w)&&AN(w)&&(x=*AAV(w),!AN(x)&&1==AR(x))))R line(a,jfread(w),tso);
 tostdout=tso; sopen(a); lastout=0;
 R mtv;
}

F1(script1 ){F1RANK(  0,script1, 0); R linf(0L,w,1);}

F2(script2 ){F2RANK(0,0,script2, 0); R linf(a ,w,1);}

F1(sscript1){F1RANK(  0,sscript1,0); R linf(0L,w,0);}

F2(sscript2){F2RANK(0,0,sscript2,0); R linf(a ,w,0);}

F1(line1   ){F1RANK(  1,line1,   0); R line(0L,w,1);}

F2(line2   ){F2RANK(0,1,line2,   0); R line(a ,w,1);}

F1(sline1  ){F1RANK(  1,sline1,  0); R line(0L,w,0);}

F2(sline2  ){F2RANK(0,1,sline2,  0); R line(a ,w,0);}
