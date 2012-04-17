/*****************************  MPEG-4 AAC Decoder  **************************

                        (C) Copyright Fraunhofer IIS (2004)
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
   Author(s):   Josef Hoepfl
   Description:

   This software and/or program is protected by copyright law and international
   treaties. Any reproduction or distribution of this software and/or program,
   or any portion of it, may result in severe civil and criminal penalties, and
   will be prosecuted to the maximum extent possible under law.

******************************************************************************/

#ifndef AACDECODER_H
#define AACDECODER_H

#include "common_fix.h"

#include "FDK_bitstream.h"

#include "channel.h"

#include "tpdec_lib.h"
#include "FDK_audio.h"

#include "block.h"

#include "genericStds.h"


#include "sbrdecoder.h"


#include "aacdec_drc.h"

 #include "pcmutils_lib.h"


/* Capabilities flags */
#define CAPF_AAC_LC           0x00000001
#define CAPF_AAC_LD           0x00000002
#define CAPF_AAC_SCAL         0x00000004
#define CAPF_AAC_ER           0x00000008
#define CAPF_AAC_480          0x00000010
#define CAPF_AAC_512          0x00000020
#define CAPF_AAC_960          0x00000040
#define CAPF_AAC_1024         0x00000080
#define CAPF_AAC_HCR          0x00000100
#define CAPF_AAC_VCB11        0x00000200
#define CAPF_AAC_RVLC         0x00000400
#define CAPF_AAC_MPEG4        0x00000800 /* PNS */
#define CAPF_AAC_DRC          0x00001000
#define CAPF_AAC_CONCEAL      0x00002000
#define CAPF_AAC_DRM_BSFORMAT 0x00004000
#define CAPF_AAC_BSAC         0x00008000

typedef struct AAC_DECODER_INSTANCE *HANDLE_AACDECODER;


enum
{
  L = 0,
  R = 1
};

typedef struct {
    unsigned char *buffer;
    int bufferSize;
    int offset[8];
    int nrElements;
} CAncData;

typedef enum {
  NOT_DEFINED = -1,
  MODE_HQ     =  0,
  MODE_LP     =  1
} QMF_MODE;

typedef struct {
  int        bsDelay;
} SBR_PARAMS;


/* AAC decoder (opaque toward userland) struct declaration */
struct AAC_DECODER_INSTANCE {
  INT                   aacChannels;                 /*!< Amount of AAC decoder channels allocated.        */
  INT                   ascChannels;                 /*!< Amount of AAC decoder channels signalled in ASC. */
  INT                   blockNumber;                 /*!< frame counter                                    */

  INT                   nrOfLayers;

  INT                   outputInterleaved;           /*!< PCM output format (interleaved/none interleaved). */

  HANDLE_TRANSPORTDEC   hInput;                      /*!< Transport layer handle. */

  SamplingRateInfo      samplingRateInfo;            /*!< Sampling Rate information table */

  UCHAR                 frameOK;                     /*!< Will be unset if a consistency check, e.g. CRC etc. fails */

  UINT                  flags;                       /*!< Flags for internal decoder use. DO NOT USE self::streaminfo::flags ! */

  MP4_ELEMENT_ID        elements[7]; /*!< Table where the element Id's are listed          */
  UCHAR                 elTags[7];   /*!< Table where the elements id Tags are listed      */
  UCHAR                 chMapping[(6)];   /*!< Table of MPEG canonical order to bitstream channel order mapping. */

  AUDIO_CHANNEL_TYPE    channelType[(6)];    /*!< Audio channel type of each output audio channel (from 0 upto numChannels).           */
  UCHAR                 channelIndices[(6)]; /*!< Audio channel index for each output audio channel (from 0 upto numChannels).         */
                                                             /* See ISO/IEC 13818-7:2005(E), 8.5.3.2 Explicit channel mapping using a program_config_element() */


  const UCHAR         (*channelOutputMapping)[8];    /*!< Table for MPEG canonical order to output channel order mapping. */


  CProgramConfig                pce;
  CStreamInfo                   streamInfo;         /*!< pointer to StreamInfo data (read from the bitstream) */
  CAacDecoderChannelInfo       *pAacDecoderChannelInfo[(6)];       /*!< Temporal channel memory */
  CAacDecoderStaticChannelInfo *pAacDecoderStaticChannelInfo[(6)]; /*!< Persistent channel memory */

  CAacDecoderCommonData         aacCommonData;             /*!< Temporal shared data for all channels hooked into pAacDecoderChannelInfo */

  CConcealParams                concealCommonData;
  INT   concealChannels;


  HANDLE_SBRDECODER   hSbrDecoder;                   /*!< SBR decoder handle.                        */
  UCHAR               sbrEnabled;                    /*!< flag to store if SBR has been detected     */
  UCHAR               sbrEnabledPrev;                /*!< flag to store if SBR has been detected from previous frame */
  UCHAR               psPossible;                    /*!< flag to store if PS is possible            */
  SBR_PARAMS          sbrParams;                     /*!< struct to store all sbr parameters         */

  QMF_MODE   qmfModeCurr;                            /*!< The current QMF mode                       */
  QMF_MODE   qmfModeUser;                            /*!< The QMF mode requested by the library user */

  HANDLE_AAC_DRC  hDrcInfo;                          /*!< handle to DRC data structure               */


  CAncData      ancData;                             /*!< structure to handle ancillary data         */

  HANDLE_PCM_DOWNMIX  hPcmUtils;                     /*!< privat data for the PCM utils.             */

};


#define AAC_DEBUG_EXTHLP "\
--- AAC-Core ---\n\
    0x00010000 Header data\n\
    0x00020000 CRC data\n\
    0x00040000 Channel info\n\
    0x00080000 Section data\n\
    0x00100000 Scalefactor data\n\
    0x00200000 Pulse data\n\
    0x00400000 Tns data\n\
    0x00800000 Quantized spectrum\n\
    0x01000000 Requantized spectrum\n\
    0x02000000 Time output\n\
    0x04000000 Fatal errors\n\
    0x08000000 Buffer fullness\n\
    0x10000000 Average bitrate\n\
    0x20000000 Synchronization\n\
    0x40000000 Concealment\n\
    0x7FFF0000 all AAC-Core-Info\n\
"

/**
 * \brief Synchronise QMF mode for all modules using QMF data.
 * \param self decoder handle
 */
void CAacDecoder_SyncQmfMode(HANDLE_AACDECODER self);

/**
 * \brief Signal a bit stream interruption to the decoder
 * \param self decoder handle
 */
void CAacDecoder_SignalInterruption(HANDLE_AACDECODER self);

/*!
  \brief Initialize ancillary buffer

  \ancData Pointer to ancillary data structure
  \buffer Pointer to (external) anc data buffer
  \size Size of the buffer pointed on by buffer

  \return  Error code
*/
AAC_DECODER_ERROR CAacDecoder_AncDataInit(CAncData *ancData, unsigned char *buffer, int size);

/*!
  \brief Get one ancillary data element

  \ancData Pointer to ancillary data structure
  \index Index of the anc data element to get
  \ptr Pointer to a buffer receiving a pointer to the requested anc data element
  \size Pointer to a buffer receiving the length of the requested anc data element

  \return  Error code
*/
AAC_DECODER_ERROR CAacDecoder_AncDataGet(CAncData *ancData, int index, unsigned char **ptr, int *size);


/* initialization of aac decoder */
LINKSPEC_H HANDLE_AACDECODER CAacDecoder_Open(TRANSPORT_TYPE bsFormat);

/* Initialization of stream-info elements */
LINKSPEC_H AAC_DECODER_ERROR CAacDecoder_Init(HANDLE_AACDECODER self,
                                              const CSAudioSpecificConfig *asc);

/*!
  \brief Decodes one aac frame

  The function decodes one aac frame. The decoding of coupling channel
  elements are not supported. The transport layer might signal, that the
  data of the current frame is invalid, e.g. as a result of a packet
  loss in streaming mode.
  The bitstream position of transportDec_GetBitstream(self->hInput) must
  be exactly the end of the access unit, including all byte alignment bits.
  For this purpose, the variable auStartAnchor is used.

  \return  error status
*/
LINKSPEC_H AAC_DECODER_ERROR CAacDecoder_DecodeFrame(
        HANDLE_AACDECODER self,
        const UINT flags,
        INT_PCM *pTimeData,
        const INT  timeDataSize,
        const INT interleaved
        );

/* Destroy aac decoder */
LINKSPEC_H void CAacDecoder_Close ( HANDLE_AACDECODER self );

/* get streaminfo handle from decoder */
LINKSPEC_H CStreamInfo* CAacDecoder_GetStreamInfo ( HANDLE_AACDECODER self );


#endif /* #ifndef AACDECODER_H */
