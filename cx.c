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
/* Conjunction:  Explicit Definition : and Associates                      */
/*                                                                         */
/* Left:                                                                   */
/*  0   Noun                                                               */
/*  1   Adverb             m,u,x,y                                         */
/*  2   Conjunction        n,m,u,v,x,y                                     */
/*  3   Monad              y                                               */
/*  4   Dyad               x,y                                             */
/*  13  Explicit to tacit  x,y                                             */
/*                                                                         */
/* Right:                                                                  */
/*  0       Read from stdin until a lone ')' is encountered on a line      */
/*  string  Use string contents as argument to :                           */
/*                                                                         */
/* Usage of the f,g,h fields of :-defined verbs:                           */
/*  f  character matrix of  left argument to :                             */
/*  g  character matrix of right argument to :                             */
/*  h  2-element vector of boxes                                           */
/*       0  Monadic definition                                             */
/*       1  Dyadic definition                                              */

#include "j.h"
#include "a.h"
#include "d.h"

static I nline;


static DF2(xd){PROLOG;DECLFG;A f,*line,loc=local,name,seq,z=0;B b;DC dv;
  I i=0,n,old;
 b=a&&w; f=*(b+AAV(sv->h));
 line=AAV(f); n=nline=AN(f); ASSERT(n,EVDOMAIN);
 GA(local,SYMB,twprimes[0],1,0);
 symbis(scnm(CALPHA),a,local);
 symbis(scnm(COMEGA),w,local);
 RZ(dv=debadd(DCDEFN)); dv->p=sv->s; drun=0;
 old=tbase+ttop;
 ra(self);
 for(;0<=i&&i<n;i++){
  tpop(old);
  dv->ln=i; dv->n=1+(0!=a);
  z=parse(ca(line[i]));
  if(!debugb&&!z) break;
 }
 if(debugb&&!z){z=tpush(qpopres); qpopres=0;}
 z=car(z); ++AC(local); fa(local); local=loc;
 asgn=0;
 fa(self);
 debz();
 if(!z) jsignal(EVRESULT);
 EPILOG(z);
}

static CS1(xv1, f1(w,fs))

static CS2(xv2, g2(a,w,gs))

static DF1(xn1){RZ(   w); R vn(xd(0L,w,self));}

static DF2(xn2){RZ(a&&w); R vn(xd(a ,w,self));}

static DF1(xadv ){RZ(w);    R xd(w,0L,self);}

static DF2(xconj){RZ(a&&w); R xd(a,w, self);}

static F1(preparse){A lab,s,*sv,t,*tv,y,*yv;I i,j=0,n;
 RZ(w);
 RZ(t=every(BOX&AT(w)?w:rank1ex(w,0L,1L,box),tokens));
 R box(t);
}

F2(colon){A h,hu,hv,m,l,u=a,v=w;B b,c,d;I an,at,un,ut,vn,vt;C*p,*q,*r,*x,*z;
 RZ(a&&w);
 if(AT(a)&VERB&&AT(w)&VERB){
  R fdef(CCOLON, VERB, xv1,xv2, a,w,append(a,w), 0L, mr(u),lr(v),rr(v));
 }
 if(!AR(w)&&NUMERIC&AT(w)&&0==i0(w)){
   v=mtc;
   while(1){
     l=jgets("");
     if(jerr) R 0;
     if(!l) break;
     p=CAV(l);
     if(')'==*p){while(' '==*++p);if(!*p)break;}
     v=append(append(v,l),cnl);
 }}
 if(!AR(a)&&NUMERIC&AT(a)&&0==i0(a))R v;
 ASSERT(CHAR&AT(v)&&2>=AR(v),EVDOMAIN);
 if(2==AR(v))v=ravel(stitch(v,scc(CNL)));
 if(!(AN(v)&&CNL==cl(v)))v=append(v,scc(CNL));
 q=p=CAV(v); r=x=z=p+AN(v);
 while(p<z){
  if(':'!=*p){while(CNL!=*p)++p; ++p; continue;}
  r=p;
  while(' '==*++p);
  if(CNL==*p){x=r; break;}
  while(CNL!=*p)++p;
  ++p;
 }
 while(r<z && CNL!=*r)++r;
 if(r<z)++r;
 u=take(sc((I)(x-q)),v);
 v=drop(sc((I)(r-q)),v);
 if(AN(u)&&CHAR&AT(u)&&1>=AR(u)&&strchr(CAV(u),CNL)){
  if(!(AN(u)&&CNL==cl(u))) u=append(u,scc(CNL));
  h=cut(ds(CBOX),sc(-2L));
  u=df1(u,h);
 }
 if(AN(v)&&CHAR&AT(v)&&1>=AR(v)&&strchr(CAV(v),CNL)){
  if(!(AN(v)&&CNL==cl(v))) v=append(v,scc(CNL));
  h=cut(ds(CBOX),sc(-2L));
  v=df1(v,h);
 }
 an=AN(a); at=AT(a); c=NOUN&at;
 un=AN(u); ut=AT(u); b=NOUN&ut;
 vn=AN(v); vt=AT(v); d=NOUN&vt;
 if(an&&NUMERIC&at?13==i0(a):0)R vtrans(w);
 RZ(hu=hv=ca(ace));
 if(b){
  ASSERT(AR(u)<=1+(CHAR==ut),EVRANK);
  if(un){
   ASSERT(ut&BOX+CHAR,EVDOMAIN);
   RZ(hu=preparse(u));
   RZ(u= ut&BOX ? ope(rankle(u)) : 2>AR(u) ? lamin1(rankle(u)) : u);
 }}
 if(!vn||vt&BOX+CHAR){
  ASSERT(AR(v)<=1+(CHAR==vt),EVRANK);
  if(vn){
   ASSERT(vt&BOX+CHAR,EVDOMAIN);
   RZ(hv=preparse(v));
   RZ(v= vt&BOX ? ope(rankle(v)) : 2>AR(v) ? lamin1(rankle(v)) : v);
 }}
 w=un&&vn ? append(u,append(ravel(scc(CCOLON)),v)) : u;
 switch(an&&NUMERIC&at?i0(a):0){
  case 1: R fdef(CCOLON, ADV,  xadv,0L,  a,w,append(hu,hv), 0L, 0L,0L,0L);
  case 2: R fdef(CCOLON, CONJ, 0L,xconj, a,w,append(hv,hu), 0L, 0L,0L,0L);
  case 3: R fdef(CCOLON, VERB, xn1,xn2,  a,w,append(hu,hv), 0L, RMAX,RMAX,RMAX);
  case 4: R fdef(CCOLON, VERB, 0L,xn2,   a,w,append(hv,hu), 0L, 0L,RMAX,RMAX);
  default: ASSERT(0,EVDOMAIN);
 }
 R mtv;
}
