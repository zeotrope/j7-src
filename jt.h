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
/* Type Definitions                                                        */


#define U unsigned

typedef char   B;
typedef char   C;
typedef U char UC;
typedef short  S;
typedef long   I;
typedef double D;
typedef struct {D re,im;} Z;
typedef struct AST {I t,c,n,r,s[1];} *A;
typedef struct {A k,v;} K;
typedef struct {A name,val;} SY;
typedef A(*AF)();
typedef void(*SF)();

/* Layout of type A                                                        */
/*                                                                         */
/* t      Type.  One of the defined constants below.                       */
/* c      Reference count.                                                 */
/* n      Number of elements in the ravel of the array.                    */
/* r      Rank.                                                            */
/* AS(s)  Pointer to the first element of shape vector.                    */
/* AV(a)  Pointer to the first array element.                              */

#define AT(a)           ((a)->t)
#define AC(a)           ((a)->c)
#define AN(a)           ((a)->n)
#define AR(a)           ((a)->r)
#define AH              4L              /* # header words in A             */
#define AS(a)           ((a)->s)

#if (SYS_DOUBLE)
#define AV(a)           ((I*)(a)+AH+AR(a)+(1&AR(a)))
#else
#define AV(a)           ((I*)(a)+AH+AR(a)) /* (AH+AR(a)+(I*)(a)) bombs SGI */
#endif

#define BAV(a)          ((B*)AV(a))
#define CAV(a)          ((C*)AV(a))
#define DAV(a)          ((D*)AV(a))
#define ZAV(a)          ((Z*)AV(a))
#define AAV(a)          ((A*)AV(a))
#define VAV(f)          ((V*)AV(f))
#define SYAV(a)         ((SY*)AV(a))
#define ID(f)           (VAV(f)->id)
#define CTRWID(c)       (*CAV(c))
#define CTRWNM(c)       (AN(c)>1?(C*)(1+CAV(c)):0)

#define BOOL            (1L<<0)    /* B    boolean                 */
#define CHAR            (1L<<1)    /* C    literal (character)     */
#define INT             (1L<<2)    /* I    integer                 */
#define FL              (1L<<3)    /* D    floating point (double) */
#define CMPX            (1L<<4)    /* Z    complex                 */
#define BOX             (1L<<5)    /* A    boxed                   */
#define BOXK            (1L<<6)    /* K    box with key            */
#define VERB            (1L<<7)    /* V    verb                    */
#define ADV             (1L<<8)    /* V    adverb                  */
#define CONJ            (1L<<9)    /* V    conjunction             */
#define NAME            (1L<<10)   /* C    pronoun                 */
#define LPAR            (1L<<11)   /* I    left  parenthesis       */
#define RPAR            (1L<<12)   /* I    right parenthesis       */
#define ASGN            (1L<<13)   /* I    assignment              */
#define MARK            (1L<<14)   /* I    end-of-stack marker     */
#define SYMB            (1L<<15)   /* SY   symbol table            */
#define CTRW            (1L<<16)   /* CTR  control word            */

#define ANY             -1L
#define NUMERIC         (BOOL|INT|FL|CMPX)
#define NOUN            (NUMERIC|CHAR|BOX|BOXK)
#define FUNC            (VERB|ADV|CONJ)
#define CAVN            (NOUN|FUNC)
#define IS1BYTE         (BOOL|CHAR|NAME)


#if (SYS_DOUBLE)
#define WP(t,n,r)       (1+AH+r+(SZT(t,n)+!!(IS1BYTE&t)+(SZI-1))/SZI)
#else
#define WP(t,n,r)       (  AH+r+(SZT(t,n)+!!(IS1BYTE&t)+(SZI-1))/SZI)
#endif

    /* for use by the session manager  */
typedef I SI;

typedef void* Ptr;

typedef struct {I type;AF f1,f2;UC mr,lr,rr,inv;} P;
typedef struct {AF f1,f2;A f,g,h,s;I fl;UC mr,lr,rr;C id;} V;
                                        /* fl - flags                      */
#define VGERL           1L              /* gerund left  argument           */
#define VGERR           2L              /* gerund right argument           */

typedef struct {SF f;I cv;} VA2;
typedef struct {C id,bf;VA2 fcv[6];} VA;

typedef enum {                          /* bf - boolean function           */
 V0000, V0001, V0010, V0011, V0100, V0101, V0110, V0111,
 V1000, V1001, V1010, V1011, V1100, V1101, V1110, V1111
} BOOLF;
                                        /* cv - control vector             */
#define VBB             1L              /* argument type B                 */
#define VII             2L              /* argument type I                 */
#define VDD             4L              /* argument type D                 */
#define VZZ             8L              /* argument type Z                 */
#define VB              16L             /* result   type B                 */
#define VI              32L             /* result   type I                 */
#define VD              64L             /* result   type D                 */
#define VZ              128L            /* result   type Z                 */
#define VRD             256L            /* convert result to D             */
#define VRI             512L            /* convert result to I             */
#define VRJ             1024L           /* convert result to I from Z      */
#define VASS            2048L           /* associative                     */
