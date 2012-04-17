/***************************  Fraunhofer IIS FDK Tools  **********************

                        (C) Copyright Fraunhofer IIS (2006)
                               All Rights Reserved

    Please be advised that this software and/or program delivery is
    Confidential Information of Fraunhofer and subject to and covered by the

    Fraunhofer IIS Software Evaluation Agreement
    between Google Inc. and  Fraunhofer
    effective and in full force since March 1, 2012.

    You may use this software and/or program only under the terms and
    conditions described in the above mentioned Fraunhofer IIS Software
    Evaluation Agreement. Any other and/or further use requires a separate agreement.


   $Id$
   Author(s):
   Description: fixed point intrinsics

   This software and/or program is protected by copyright law and international
   treaties. Any reproduction or distribution of this software and/or program,
   or any portion of it, may result in severe civil and criminal penalties, and
   will be prosecuted to the maximum extent possible under law.

******************************************************************************/
#if defined(__mips__)

#if (__GNUC__) && defined(__mips__)	/* cppp replaced: elif */
/* MIPS GCC based compiler */

#define FUNCTION_fixmuldiv2_DD

#define FUNCTION_fixmuldiv2BitExact_DD
#define fixmuldiv2BitExact_DD(a,b) fixmuldiv2_DD(a,b)

inline INT fixmuldiv2_DD (const INT a, const INT b)
{
  INT result ;

  asm ("mult %1,%2;\n"
	     : "=hi" (result)
	     : "d" (a), "r" (b)
             : "lo");

  return result ;
}

#endif /* (__GNUC__) && defined(__mips__) */

#endif /* __mips__ */

