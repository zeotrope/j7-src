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
/* Control word formation                                                  */

#include "j.h"


#define NCNTRLW   21


A conctrw(C c,I n,C*s){A z;C *t;I m;
 t=memchr(s,'_',n);
 t++; m=n-1-(I)(t-s);
 ASSERT(vnm(m,t),EVILNAME);
 z=ctrw(c,m,t);
 R z;
}

static C *cntrl[NCNTRLW]={
 "if."    , "do."    , "end."   , "else."  , "elseif.",
 "while." , "whilst.", "for."   , "try."   , "catch." ,
 "catchd.", "catcht.", "select.", "case."  , "fcase." ,
 "assert.", "break." , "throw." , "for"    , "goto" ,
 "label"
};

static C idcntrl[NCNTRLW]={
 CIF,     CDO,     CEND,    CELSE, CELSEIF,
 CWHILE,  CWHILST, CFOR,    CTRY,  CCATCH,
 CCATCHD, CCATCHT, CSELECT, CCASE, CFCASE,
 CASSERT, CBREAK,  CTHROW,  CFORI, CGOTO,
 CLABEL
};

C xspellin(I n,C*s){A z;C d,*t;I i,l,m;
 d=s[n-1]; m=n;
 if((t=memchr(s,'_',n))) m=t-s;
 if(!m||d!=CESC1) R 0;
 for(i=0;i<NCNTRLW;i++){
  t=cntrl[i]; l=strlen(t);
  if(l!=m) continue;
  else if(!strncmp(s,t,m)) break;
 }
 if(i==NCNTRLW) R 0;
 R idcntrl[i];
}
