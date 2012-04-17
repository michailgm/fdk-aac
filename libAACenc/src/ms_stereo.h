/******************************** MPEG Audio Encoder **************************

                     (C) Copyright Fraunhofer IIS (1999)
                               All Rights Reserved

    Please be advised that this software and/or program delivery is
    Confidential Information of Fraunhofer and subject to and covered by the

    Fraunhofer IIS Software Evaluation Agreement
    between Google Inc. and  Fraunhofer
    effective and in full force since March 1, 2012.

    You may use this software and/or program only under the terms and
    conditions described in the above mentioned Fraunhofer IIS Software
    Evaluation Agreement. Any other and/or further use requires a separate agreement.


   This software and/or program is protected by copyright law and international
   treaties. Any reproduction or distribution of this software and/or program,
   or any portion of it, may result in severe civil and criminal penalties, and
   will be prosecuted to the maximum extent possible under law.

   $Id$
   Initial author:       M.Werner
   contents/description: MS stereo processing

******************************************************************************/
#ifndef __MS_STEREO_H__
#define __MS_STEREO_H__


#include "interface.h"

void FDKaacEnc_MsStereoProcessing(PSY_DATA   *RESTRICT psyData[(2)],
                        PSY_OUT_CHANNEL* psyOutChannel[2],
                        const INT  *isBook,
                        INT        *msDigest,       /* output */
                        INT        *msMask,         /* output */
                        const INT   sfbCnt,
                        const INT   sfbPerGroup,
                        const INT   maxSfbPerGroup,
                        const INT  *sfbOffset);

#endif
