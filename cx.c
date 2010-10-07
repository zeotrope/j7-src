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
/*  h  4-element vector of boxes                                           */
/*       0  vector of boxed tokens excluding labels for f                  */
/*       1  2-column matrix of (label) link (line#) for f                  */
/*       2  vector of boxed tokens excluding labels for g                  */
/*       3  2-column matrix of (label) link (line#) for g                  */

#include "j.h"
#include "a.h"
#include "d.h"

static I ensuite(w)A w;{
 RZ(w&&AN(w)&&w!=mark);
 if(!(INT&AT(w)))RZ(w=cvt(INT,w));
 ASSERT(1>=AR(w)&&INT&AT(w),EVDEFN);
 R 1+*AV(w);
}

static I nline;

static F1(label){A*v;I k;
 RZ(w);
 v=AAV(w); k=i0(v[1]); RZ(symbis(*v,apv(nline-k,k,1L),local));
 R one;
}

static DF2(xd){PROLOG;DECLFG;A f,*line,loc=local,name,seq,*v,z=0;B b;DC dv;
  I i=0,n,old;
 b=a&&w&&VERB&AT(self); v=b+b+AAV(sv->h);
 f=v[0]; line=AAV(f); n=nline=AN(f); ASSERT(n,EVDOMAIN);
 GA(local,SYMB,twprimes[0],1,0);
 if(AN(v[1]))RZ(rank1ex(v[1],0L,1L,label));
 symbis(scnm(CALPHA),a,local);
 symbis(scnm(COMEGA),w,local);
 name=scnm(CGOTO); seq=ii(f);
 RZ(dv=debadd(DCDEFN)); dv->p=sv->s; drun=0;
 old=tbase+ttop;
 ra(self);
 do{
  tpop(old);
  symbis(name,behead(seq),local);
  dv->ln=i; dv->n=1+(0!=a);
  z=parse(ca(line[i]));
  if(!debugb&&!z) break;
  seq=srd(name,local);
  i=ensuite(seq)-1;
 } while(0<=i&&i<n);
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
 n=AN(t); tv=AAV(t);
 RZ(y=reshape(sc(2*n),jot)); yv=AAV(y);
 for(i=0;i<n;++i){
  s=tv[i]; sv=AAV(s); lab=sv[1];
  if(7<=AN(s)&&NAME&AT(lab)&&RPAR&AT(sv[2])){
   yv[j++]=lab; RZ(yv[j++]=sc(i)); sv[2]=mark; RZ(tv[i]=drop(two,s));
 }}
 R link(t,box(reshape(v2(j/2,2L),y)));
}

F2(colon){A h,hu,hv,m,l,u=a,v=w;B b,c,d;I an,at,un,ut,vn,vt;C*p,*q,*r,*x,*z;
 RZ(a&&w);
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
 RZ(hu=hv=append(jot,jot));
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
 switch(an&&NUMERIC&at?i0(a):0){
  case 1: R fdef(CCOLON, ADV,  xadv,0L,  a,v,h,  0L, 0L,0L,0L);
  case 2: R fdef(CCOLON, CONJ, 0L,xconj, a,v,h,  0L, 0L,0L,0L);
  case 3: {
   w=un&&vn ? append(u,append(ravel(scc(CESC2)),v)) : u;
   R fdef(CCOLON, VERB, b?xn1:xv1,d?xn2:xv2, a,w,append(hu,hv), 0L,
          b?RMAX:mr(u),d?RMAX:lr(v),d?RMAX:rr(v));
  }
  case 4: R fdef(CCOLON, VERB, 0L,d?xn2:xv2, a,u,append(hv,hu), 0L,
                 0L, d?RMAX:lr(v),d?RMAX:rr(v));
  default: ASSERT(0,EVDOMAIN);
 }
 R mtv;
}
