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
#define SYS_PCWIN           4L              /* Watcom C 386                */
#define SYS_SGI             8L              /* GCC                         */
#define SYS_SUN             16L             /* GCC                         */
#define SYS_386LX           32L             /* GCC                         */

/* ! causes compile error until you chose appropriate target system */

#define SYS                 SYS_386LX       /* pick one of the above */

#define SYS_SESM            0               /* stdin/stdout session manager */

/* use following define for SYS_SESM if linking with
   ISI compatible session manager for the target system */

/*
#define SYS_SESM            (SYS_PCWIN)
*/

/* Set SYS_SESM as 1 if linked with ISI compatible session manager.
   ISI compatible session managers are available for 
   SYS_ARCHIMEDES, SYS_MACINTOSH, SYS_PC, SYS_PCWIN, and SYS_PC386. */

#define SYS_ANSILIB         (SYS_AMIGA|SYS_PCWIN|SYS_386LX)
					    
#define SYS_UNIX            (SYS_MIPS|SYS_SGI|SYS_SUN)

#define SYS_LILENDIAN       (SYS_PCWIN|SYS_386LX)

#define SYS_DOUBLE          0    /* "double" requires doubleword alignment */

#endif
