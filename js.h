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
/* SYS_ and friends                                                        */


#ifndef SYS

#define SYS_AMIGA           1L              /* DICE                        */
#define SYS_MIPS            2L              /* GCC                         */
#define SYS_PC              4L              /* Turbo C                     */
#define SYS_PCWIN           8L              /* Watcom C 386                */
#define SYS_PC386           16L             /* Watcom C 386                */
#define SYS_SGI             32L             /* GCC                         */
#define SYS_SUN             64L             /* GCC                         */
#define SYS_386IX           128L            /* Interactive C               */

/* ! causes compile error until you chose appropriate target system */

#define SYS                 SYS_386IX       /* pick one of the above */

#define SYS_SESM            0               /* stdin/stdout session manager */

/* use following define for SYS_SESM if linking with
   ISI compatible session manager for the target system */

/*
#define SYS_SESM            (SYS_PC | SYS_PCWIN | SYS_PC386)
*/

/* Set SYS_SESM as 1 if linked with ISI compatible session manager.
   ISI compatible session managers are available for 
   SYS_ARCHIMEDES, SYS_MACINTOSH, SYS_PC, SYS_PCWIN, and SYS_PC386. */

#define SYS_ANSILIB         (SYS_AMIGA|SYS_PC|SYS_PCWIN|SYS_PC386|SYS_386IX)
					    
#define SYS_UNIX            (SYS_MIPS|SYS_SGI|SYS_SUN)

#define SYS_LILENDIAN       (SYS_PC|SYS_PCWIN|SYS_PC386|SYS_386IX)

#define SYS_DOUBLE          0    /* "double" requires doubleword alignment */
#define SYS_GETTOD          0    /* gettimeofday() is available            */

#endif
