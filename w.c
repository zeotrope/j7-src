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
/* Spelling and Word Formation                                             */

#include "j.h"
#include "d.h"


#define SS              0    /* space                           */
#define SX              1    /* base                            */
#define SA              2    /* alphanumeric                    */
#define SN              3    /* N                               */
#define SNB             4    /* NB                              */
#define SNZ             5    /* NB.                             */
#define S9              6    /* numeric                         */
#define SQ              7    /* quote                           */
#define SQQ             8    /* even quotes                     */
#define SZ              9    /* trailing comment                */

#define EI              1    /* emit (b,i-1); b=.i              */
#define EN              2    /* b=.i                            */

typedef struct {C new,effect;} ST;

static ST state[10][9]={
/*SS */ {{SX,EN},{SS,0 },{SA,EN},{SN,EN},{SA,EN},{S9,EN},{SX,EN},{SX,EN},{SQ,EN}},
/*SX */ {{SX,EI},{SS,EI},{SA,EI},{SN,EI},{SA,EI},{S9,EI},{SX,0 },{SX,0 },{SQ,EI}},
/*SA */ {{SX,EI},{SS,EI},{SA,0 },{SA,0 },{SA,0 },{SA,0 },{SX,0 },{SX,0 },{SQ,EI}},
/*SN */ {{SX,EI},{SS,EI},{SA,0 },{SA,0 },{SNB,0},{SA,0 },{SX,0 },{SX,0 },{SQ,EI}},
/*SNB*/ {{SX,EI},{SS,EI},{SA,0 },{SA,0 },{SA,0 },{SA,0 },{SNZ,0},{SX,0 },{SQ,EI}},
/*SNZ*/ {{SZ,0 },{SZ,0 },{SZ,0 },{SZ,0 },{SZ,0 },{SZ,0 },{SX,0 },{SX,0 },{SZ,0 }},
/*S9 */ {{SX,EI},{SS,EI},{S9,0 },{S9,0 },{S9,0 },{S9,0 },{S9,0 },{SX,0 },{SQ,EI}},
/*SQ */ {{SQ,0 },{SQ,0 },{SQ,0 },{SQ,0 },{SQ,0 },{SQ,0 },{SQ,0 },{SQ,0 },{SQQ,0}},
/*SQQ*/ {{SX,EI},{SS,EI},{SA,EI},{SN,EI},{SA,EI},{S9,EI},{SX,EI},{SX,EI},{SQ,0 }},
/*SZ */ {{SZ,0 },{SZ,0 },{SZ,0 },{SZ,0 },{SZ,0 },{SZ,0 },{SZ,0 },{SZ,0 },{SZ,0 }}
};
/*         CX      CS      CA      CN      CB      C9      CD      CC      CQ   */

static F1(wordil){A z;C e,nv,s,t;I b,i,m,n,*x,xb,xe;ST p;UC*v;
 RZ(w);
 nv=0; s=SS;
 n=AN(w); v=(UC*)AV(w); GA(z,INT,1+n+n,1,0); x=1+AV(z);
 for(i=0;i<n;++i){
  p=state[s][wtype[v[i]]]; e=p.effect;
  if(e==EI){
   t&=s==S9;
   if(t){if(!nv){nv=1; xb=b;} xe=i;}
   else{if(nv){nv=0; *x++=xb; *x++=xe-xb;} *x++=b; *x++=i-b;}
  }
  s=p.new;
  if(e){b=i; t=s==S9;}
 }
 if(s==SQ){jsignalx(EVOPENQ,w,-1L); debug(); R 0;}
 t&=s==S9;
 if(t){*x++=xb=nv?xb:b; *x++=n-xb;}
 else{if(nv){*x++=xb; *x++=xe-xb;} if(s!=SS){*x++=b; *x++=n-b;}}
 m=x-AV(z); *AV(z)=s==SZ||s==SNZ?-m/2:m/2;
 R z;
}

F1(words){A t,*x,z;C*s;I k,n,*y;
 F1RANK(1,words,0);
 ASSERT(CHAR&AT(w)||!AN(w),EVDOMAIN);
 RZ(t=wordil(w));
 s=CAV(w); y=AV(t); n=*y++; n=0>n?-n:n;
 GA(z,BOX,n,1,0); x=AAV(z);
 DO(n, k=*y++; *x++=str(*y++,s+k));
 R z;
}


static A conname(I n,C*s){A z;
 ASSERT(vnm(n,s),EVILNAME);
 GA(z,NAME,n,1,0); MC(AV(z),s,n);
 R z;
}

static A constr(I n,C*s){A z;C b,c,p,*t,*x;I m=0;
 p=0; t=s; DO(n-2, c=*++t; b=c==CQUOTE; if(!b||p)m++;    p=b&&!p);
 GA(z,CHAR,m,1!=m,0); x=CAV(z);
 p=0; t=s; DO(n-2, c=*++t; b=c==CQUOTE; if(!b||p)*x++=c; p=b&&!p);
 R z;
}

static F2(enstack){A t,*x,z;C c,d,e,p,*s,*wi;I i,n,*u,wl;
 RZ(a&&w);
 s=CAV(w); u=AV(a); n=*u++; n=0>n?-(1+n):n;
 GA(z,BOX,5+n,1,0); x=AAV(z); *x++=mark;
 for(i=0;i<n;i++){
  wi=s+*u++; wl=*u++;
  c=e=*wi; p=ctype[c];
  if(1<wl){
   d=*(wi+wl-1);
   if(p!=C9&&d==CESC1||d==CESC2){
    e=spellin(wl,wi);
    if(!e&&p==CA&&d==CESC1&&explcin) e=xspellin(wl,wi);
    printf("%d\n",(UC)e);
  }}
  if(!e||e==C1||127<(UC)c){jsignalx(EVSPELL,w,wi-s); debug(); R 0;}
  RZ(t=ds(e));
  if(AT(t)&MARK){switch(p){
   case CA: RZ(*x++=conname(wl,wi)); break;
   case CS:
   case C9: RZ(*x++=connum(wl,wi));  break;
   case CQ: RZ(*x++=constr(wl,wi));  break;
   default: ASSERT(0,EVDOMAIN);
  }}else{*x++=AT(t)&CTRW?conctrw(e,wl,wi):t;}
 }
 *x++=mark; *x++=mark; *x++=mark; *x++=mark;
 R z;
}

F1(tokens){R enstack(wordil(w),w);}


static C spell[6][66]={
 {'=',     '<',     '>',     '_',     '+',     '*',     '-',     '%',
  '^',     '$',     '~',     '|',     '.',     ':',     ',',     ';',
  '#',     '@',     '/',     CBSLASH, '[',     ']',     '{',     '}',
  '`',     CQQ,     '&',     '!',     '?',     'a',     'A',     'b',
  'C',     'd',     'D',     'e',     'E',     'f',     'H',     'i',
  'I',     'j',     'L',     'M',     'o',     'p',     'q',     'r',
  's',     'S',     't',     'T',     'u',     'x',     'y',     '0',
  '1',     '2',     '3',     '4',     '5',     '6',     '7',     '8',
  '9',     0},

 {CASGN,   CFLOOR,  CCEIL,   1,       COR,     CAND,    CNOT,    CDOMINO,
  CLOG,    CSPARS,  CNUB,    CREV,    CDOTDOT, COBVERSE,CCOMDOT, CCUT,
  CBASE,   CATDOT,  CSLDOT,  CBSDOT,  1,       1,       CTAKE,   CDROP,
  1,       CEXEC,   CUNDER,  CFIT,    CQRYDOT, CALP,    CATOMIC, CBOOL,
  CCYCLE,  CDERIVF, CDDOT,   CEPS,    CEBAR,   CFIX,    CHYPGM,  CIOTA,
  CINDCS,  CJDOT,   CLVLOF,  CMEMO,   CCIRCLE, CPOLY,   1,       CRDOT,
  1,       1,       CTAYC,   CTAYN,   1,       1,       1,       1,
  1,       1,       1,       1,       1,       1,       1,       1,
  1,       0},   /* 2: CESC1 */

 {CGASGN,  CLE,     CGE,     CUSCO,   CNOR,    CNAND,   CMATCH,  CROOT,
  CPOWOP,  CSELF,   CNE,     CCANT,   CDOTCO,  CADVERSE,CLAMIN,  CWORDS,
  CABASE,  CATCO,   CGRADE,  CDGRADE, CLBKCO,  1,       CTAIL,   CCTAIL,
  CGRCO,   CTHORN,  CAMPCO,  CIBEAM,  1,       CACE,    1,       1,
  1,       1,       CDCOL,   1,       1,       1,       1,       CSTEPS,
  1,       1,       CLVLAT,  1,       1,       CPRIME,  CPRIMF,  1,
  CSYMBL,  CSPRD,   CWTAY,   1,       CUNICD,  CEXTPR,  1,       CZERO,
  CONE,    CTWO,    CTHREE,  CFOUR,   CFIVE,   CSIX,    CSEVEN,  CEIGHT,
  CNINE,   0},   /* 2: CESC2 */

 {1,       1,       1,       1,       1,       1,       1,       1,
  1,       1,       1,       1,       1,       1,       1,       1,
  1,       1,       1,       1,       1,       1,       1,       1,
  1,       1,       1,       1,       1,       1,       1,       1,
  1,       1,       1,       1,       1,       1,       1,       1,
  1,       1,       1,       1,       1,       CPOLYD,  1,       1,
  1,       1,       1,       1,       1,       1,       1,       1,
  1,       1,       1,       1,       1,       1,       1,       1,
  1,       0},   /* 3: CESC1 CESC1 */

 {1,       1,       1,       1,       1,       1,       1,       1,
  1,       1,       1,       1,       1,       1,       1,       1,
  1,       1,       1,       1,       1,       1,       1,       1,
  1,       1,       CUNDRCO, 1,       1,       1,       1,       1,
  1,       1,       1,       1,       1,       1,       1,       1,
  1,       1,       1,       1,       1,       1,       1,       1,
  1,       1,       1,       1,       1,       1,       1,       1,
  1,       1,       1,       1,       1,       1,       1,       1,
  1,       0},   /* 3: CESC1 CESC2 */

 {1,       1,       1,       1,       1,       1,       1,       1,
  1,       1,       1,       1,       1,       1,       1,       1,
  1,       1,       1,       1,       1,       1,       CFETCH,  1,
  1,       1,       1,       1,       1,       1,       1,       1,
  1,       1,       1,       1,       1,       1,       1,       1,
  1,       1,       1,       1,       1,       1,       1,       1,
  1,       1,       1,       1,       1,       1,       1,       1,
  1,       1,       1,       1,       1,       1,       1,       1,
  1,       0}    /* 3: CESC2 CESC2 */
};

C spellin(I n,C*s){C d,p,q,*t;I j;
 switch(n){
  case 1:
   R *s;
  case 2: {
   d=*(s+1); j=d==CESC1?1:d==CESC2?2:0;
   R j&&(t=(C*)strchr(spell[0],*s)) ? spell[j][t-spell[0]] : 0;
  }
  case 3: {
   p=*s; q=*(1+s); d=*(2+s);
   j=(q==CESC1&&d==CESC1)?3:(q==CESC1&&d==CESC2)?4:(q==CESC2&&d==CESC2)?5:0;
   R j&&(t=(C*)strchr(spell[0],*s)) ? spell[j][t-spell[0]] : 0;
  }
  default: R 0;
}}

void spellit(C c,C*s){C*q;I i,k;
 s[1]=0;
 switch(c){
  case CHOOK:  s[0]='2'; break;
  case CFORK:  s[0]='3'; break;
  case CADVF:  s[0]='4'; break;
  case CHOOKO: s[0]='5'; break;
  case CFORKO: s[0]='6'; break;
  default:
   if(0<=c&&c<=127) s[0]=c;
   else{
    for(i=1;i<=5;i++){
     if((q=(C*)strchr(spell[i],c))){k=q-spell[i]; s[0]=spell[0][k]; break;}
    }
    switch(i){
     case 1: {s[1]=CESC1; break;}
     case 2: {s[1]=CESC2; break;}
     case 3: {s[1]=CESC1; s[2]=CESC1; break;}
     case 4: {s[1]=CESC1; s[2]=CESC2; break;}
     case 5: {s[1]=CESC2; s[2]=CESC2; break;}
    }
 }}}   /* spell out ID c in s */

A spellout(C c){C s[3]; spellit(c,s); R str(s[2]?3L:s[1]?2L:1L,s);}
