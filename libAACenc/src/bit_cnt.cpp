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
   contents/description: Huffman Bitcounter & coder

******************************************************************************/

#include "bit_cnt.h"

#include "aacEnc_ram.h"

#define HI_LTAB(a) (a>>16)
#define LO_LTAB(a) (a & 0xffff)

/*****************************************************************************


    functionname: FDKaacEnc_count1_2_3_4_5_6_7_8_9_10_11
    description:  counts tables 1-11
    returns:
    input:        quantized spectrum
    output:       bitCount for tables 1-11

*****************************************************************************/

static void FDKaacEnc_count1_2_3_4_5_6_7_8_9_10_11(const SHORT *RESTRICT values,
                                         const INT  width,
                                         INT       *bitCount)
{

  INT i;
  INT bc1_2,bc3_4,bc5_6,bc7_8,bc9_10,bc11,sc;
  INT t0,t1,t2,t3;
  bc1_2=0;
  bc3_4=0;
  bc5_6=0;
  bc7_8=0;
  bc9_10=0;
  bc11=0;
  sc=0;

  for(i=0;i<width;i+=4){

    t0= values[i+0];
    t1= values[i+1];
    t2= values[i+2];
    t3= values[i+3];

    /* 1,2 */

    bc1_2+=FDKaacEnc_huff_ltab1_2[t0+1][t1+1][t2+1][t3+1];

    /* 5,6 */
    bc5_6+=FDKaacEnc_huff_ltab5_6[t0+4][t1+4];
    bc5_6+=FDKaacEnc_huff_ltab5_6[t2+4][t3+4];

    t0=fixp_abs(t0);
    t1=fixp_abs(t1);
    t2=fixp_abs(t2);
    t3=fixp_abs(t3);


    bc3_4+= FDKaacEnc_huff_ltab3_4[t0][t1][t2][t3];

    bc7_8+=FDKaacEnc_huff_ltab7_8[t0][t1];
    bc7_8+=FDKaacEnc_huff_ltab7_8[t2][t3];

    bc9_10+=FDKaacEnc_huff_ltab9_10[t0][t1];
    bc9_10+=FDKaacEnc_huff_ltab9_10[t2][t3];

    bc11+= (INT) FDKaacEnc_huff_ltab11[t0][t1];
    bc11+= (INT) FDKaacEnc_huff_ltab11[t2][t3];

    sc+=(t0>0)+(t1>0)+(t2>0)+(t3>0);
  }

  bitCount[1]=HI_LTAB(bc1_2);
  bitCount[2]=LO_LTAB(bc1_2);
  bitCount[3]=HI_LTAB(bc3_4)+sc;
  bitCount[4]=LO_LTAB(bc3_4)+sc;
  bitCount[5]=HI_LTAB(bc5_6);
  bitCount[6]=LO_LTAB(bc5_6);
  bitCount[7]=HI_LTAB(bc7_8)+sc;
  bitCount[8]=LO_LTAB(bc7_8)+sc;
  bitCount[9]=HI_LTAB(bc9_10)+sc;
  bitCount[10]=LO_LTAB(bc9_10)+sc;
  bitCount[11]=bc11+sc;

}


/*****************************************************************************

    functionname: FDKaacEnc_count3_4_5_6_7_8_9_10_11
    description:  counts tables 3-11
    returns:
    input:        quantized spectrum
    output:       bitCount for tables 3-11

*****************************************************************************/

static void FDKaacEnc_count3_4_5_6_7_8_9_10_11(const SHORT *RESTRICT values,
                                     const INT  width,
                                     INT       *bitCount)
{

  INT i;
  INT bc3_4,bc5_6,bc7_8,bc9_10,bc11,sc;
  INT t0,t1,t2,t3;

  bc3_4=0;
  bc5_6=0;
  bc7_8=0;
  bc9_10=0;
  bc11=0;
  sc=0;

  for(i=0;i<width;i+=4){

    t0= values[i+0];
    t1= values[i+1];
    t2= values[i+2];
    t3= values[i+3];

    bc5_6+=FDKaacEnc_huff_ltab5_6[t0+4][t1+4];
    bc5_6+=FDKaacEnc_huff_ltab5_6[t2+4][t3+4];

    t0=fixp_abs(t0);
    t1=fixp_abs(t1);
    t2=fixp_abs(t2);
    t3=fixp_abs(t3);

    bc3_4+= FDKaacEnc_huff_ltab3_4[t0][t1][t2][t3];

    bc7_8+=FDKaacEnc_huff_ltab7_8[t0][t1];
    bc7_8+=FDKaacEnc_huff_ltab7_8[t2][t3];

    bc9_10+=FDKaacEnc_huff_ltab9_10[t0][t1];
    bc9_10+=FDKaacEnc_huff_ltab9_10[t2][t3];

    bc11+= (INT) FDKaacEnc_huff_ltab11[t0][t1];
    bc11+= (INT) FDKaacEnc_huff_ltab11[t2][t3];

    sc+=(t0>0)+(t1>0)+(t2>0)+(t3>0);
  }

  bitCount[1]=INVALID_BITCOUNT;
  bitCount[2]=INVALID_BITCOUNT;
  bitCount[3]=HI_LTAB(bc3_4)+sc;
  bitCount[4]=LO_LTAB(bc3_4)+sc;
  bitCount[5]=HI_LTAB(bc5_6);
  bitCount[6]=LO_LTAB(bc5_6);
  bitCount[7]=HI_LTAB(bc7_8)+sc;
  bitCount[8]=LO_LTAB(bc7_8)+sc;
  bitCount[9]=HI_LTAB(bc9_10)+sc;
  bitCount[10]=LO_LTAB(bc9_10)+sc;
  bitCount[11]=bc11+sc;
}



/*****************************************************************************

    functionname: FDKaacEnc_count5_6_7_8_9_10_11
    description:  counts tables 5-11
    returns:
    input:        quantized spectrum
    output:       bitCount for tables 5-11

*****************************************************************************/


static void FDKaacEnc_count5_6_7_8_9_10_11(const SHORT *RESTRICT values,
                                 const INT  width,
                                 INT       *bitCount)
{

  INT i;
  INT bc5_6,bc7_8,bc9_10,bc11,sc;
  INT t0,t1;
  bc5_6=0;
  bc7_8=0;
  bc9_10=0;
  bc11=0;
  sc=0;

  for(i=0;i<width;i+=2){

    t0 = values[i+0];
    t1 = values[i+1];

    bc5_6+=FDKaacEnc_huff_ltab5_6[t0+4][t1+4];

    t0=fixp_abs(t0);
    t1=fixp_abs(t1);

    bc7_8+=FDKaacEnc_huff_ltab7_8[t0][t1];
    bc9_10+=FDKaacEnc_huff_ltab9_10[t0][t1];
    bc11+= (INT) FDKaacEnc_huff_ltab11[t0][t1];

    sc+=(t0>0)+(t1>0);
  }
  bitCount[1]=INVALID_BITCOUNT;
  bitCount[2]=INVALID_BITCOUNT;
  bitCount[3]=INVALID_BITCOUNT;
  bitCount[4]=INVALID_BITCOUNT;
  bitCount[5]=HI_LTAB(bc5_6);
  bitCount[6]=LO_LTAB(bc5_6);
  bitCount[7]=HI_LTAB(bc7_8)+sc;
  bitCount[8]=LO_LTAB(bc7_8)+sc;
  bitCount[9]=HI_LTAB(bc9_10)+sc;
  bitCount[10]=LO_LTAB(bc9_10)+sc;
  bitCount[11]=bc11+sc;

}


/*****************************************************************************

    functionname: FDKaacEnc_count7_8_9_10_11
    description:  counts tables 7-11
    returns:
    input:        quantized spectrum
    output:       bitCount for tables 7-11

*****************************************************************************/

static void FDKaacEnc_count7_8_9_10_11(const SHORT *RESTRICT values,
                             const INT  width,
                             INT       *bitCount)
{

  INT i;
  INT bc7_8,bc9_10,bc11,sc;
  INT t0,t1;

  bc7_8=0;
  bc9_10=0;
  bc11=0;
  sc=0;

  for(i=0;i<width;i+=2){
    t0=fixp_abs(values[i+0]);
    t1=fixp_abs(values[i+1]);

    bc7_8+=FDKaacEnc_huff_ltab7_8[t0][t1];
    bc9_10+=FDKaacEnc_huff_ltab9_10[t0][t1];
    bc11+= (INT) FDKaacEnc_huff_ltab11[t0][t1];
    sc+=(t0>0)+(t1>0);
  }

  bitCount[1]=INVALID_BITCOUNT;
  bitCount[2]=INVALID_BITCOUNT;
  bitCount[3]=INVALID_BITCOUNT;
  bitCount[4]=INVALID_BITCOUNT;
  bitCount[5]=INVALID_BITCOUNT;
  bitCount[6]=INVALID_BITCOUNT;
  bitCount[7]=HI_LTAB(bc7_8)+sc;
  bitCount[8]=LO_LTAB(bc7_8)+sc;
  bitCount[9]=HI_LTAB(bc9_10)+sc;
  bitCount[10]=LO_LTAB(bc9_10)+sc;
  bitCount[11]=bc11+sc;

}

/*****************************************************************************

    functionname: FDKaacEnc_count9_10_11
    description:  counts tables 9-11
    returns:
    input:        quantized spectrum
    output:       bitCount for tables 9-11

*****************************************************************************/



static void FDKaacEnc_count9_10_11(const SHORT *RESTRICT values,
                         const INT  width,
                         INT       *bitCount)
{

  INT i;
  INT bc9_10,bc11,sc;
  INT t0,t1;

  bc9_10=0;
  bc11=0;
  sc=0;

  for(i=0;i<width;i+=2){
    t0=fixp_abs(values[i+0]);
    t1=fixp_abs(values[i+1]);

    bc9_10+=FDKaacEnc_huff_ltab9_10[t0][t1];
    bc11+= (INT) FDKaacEnc_huff_ltab11[t0][t1];

    sc+=(t0>0)+(t1>0);
  }

  bitCount[1]=INVALID_BITCOUNT;
  bitCount[2]=INVALID_BITCOUNT;
  bitCount[3]=INVALID_BITCOUNT;
  bitCount[4]=INVALID_BITCOUNT;
  bitCount[5]=INVALID_BITCOUNT;
  bitCount[6]=INVALID_BITCOUNT;
  bitCount[7]=INVALID_BITCOUNT;
  bitCount[8]=INVALID_BITCOUNT;
  bitCount[9]=HI_LTAB(bc9_10)+sc;
  bitCount[10]=LO_LTAB(bc9_10)+sc;
  bitCount[11]=bc11+sc;

}

/*****************************************************************************

    functionname: FDKaacEnc_count11
    description:  counts table 11
    returns:
    input:        quantized spectrum
    output:       bitCount for table 11

*****************************************************************************/

static void FDKaacEnc_count11(const SHORT *RESTRICT values,
                    const INT  width,
                    INT        *bitCount)
{

  INT i;
  INT bc11,sc;
  INT t0,t1;

  bc11=0;
  sc=0;
  for(i=0;i<width;i+=2){
    t0=fixp_abs(values[i+0]);
    t1=fixp_abs(values[i+1]);
    bc11+= (INT) FDKaacEnc_huff_ltab11[t0][t1];
    sc+=(t0>0)+(t1>0);
  }

  bitCount[1]=INVALID_BITCOUNT;
  bitCount[2]=INVALID_BITCOUNT;
  bitCount[3]=INVALID_BITCOUNT;
  bitCount[4]=INVALID_BITCOUNT;
  bitCount[5]=INVALID_BITCOUNT;
  bitCount[6]=INVALID_BITCOUNT;
  bitCount[7]=INVALID_BITCOUNT;
  bitCount[8]=INVALID_BITCOUNT;
  bitCount[9]=INVALID_BITCOUNT;
  bitCount[10]=INVALID_BITCOUNT;
  bitCount[11]=bc11+sc;
}

/*****************************************************************************

    functionname: FDKaacEnc_countEsc
    description:  counts table 11 (with Esc)
    returns:
    input:        quantized spectrum
    output:       bitCount for tables 11 (with Esc)

*****************************************************************************/

static void FDKaacEnc_countEsc(const SHORT *RESTRICT values,
                     const INT  width,
                     INT       *RESTRICT bitCount)
{

  INT i;
  INT bc11,ec,sc;
  INT t0,t1,t00,t01;

  bc11=0;
  sc=0;
  ec=0;
  for(i=0;i<width;i+=2){
    t0=fixp_abs(values[i+0]);
    t1=fixp_abs(values[i+1]);

    sc+=(t0>0)+(t1>0);

    t00 = fixMin(t0,16);
    t01 = fixMin(t1,16);
    bc11+= (INT) FDKaacEnc_huff_ltab11[t00][t01];

    if(t0>=16){
      ec+=5;
      while((t0>>=1) >= 16)
        ec+=2;
    }

    if(t1>=16){
      ec+=5;
      while((t1>>=1) >= 16)
        ec+=2;
    }
  }

  for (i=0; i<11; i++)
    bitCount[i]=INVALID_BITCOUNT;

  bitCount[11]=bc11+sc+ec;
}


typedef void (*COUNT_FUNCTION)(const SHORT *RESTRICT values,
                               const INT  width,
                               INT       *RESTRICT bitCount);

static const COUNT_FUNCTION countFuncTable[CODE_BOOK_ESC_LAV+1] =
{

 FDKaacEnc_count1_2_3_4_5_6_7_8_9_10_11,  /* 0  */
 FDKaacEnc_count1_2_3_4_5_6_7_8_9_10_11,  /* 1  */
 FDKaacEnc_count3_4_5_6_7_8_9_10_11,      /* 2  */
 FDKaacEnc_count5_6_7_8_9_10_11,          /* 3  */
 FDKaacEnc_count5_6_7_8_9_10_11,          /* 4  */
 FDKaacEnc_count7_8_9_10_11,              /* 5  */
 FDKaacEnc_count7_8_9_10_11,              /* 6  */
 FDKaacEnc_count7_8_9_10_11,              /* 7  */
 FDKaacEnc_count9_10_11,                  /* 8  */
 FDKaacEnc_count9_10_11,                  /* 9  */
 FDKaacEnc_count9_10_11,                  /* 10 */
 FDKaacEnc_count9_10_11,                  /* 11 */
 FDKaacEnc_count9_10_11,                  /* 12 */
 FDKaacEnc_count11,                       /* 13 */
 FDKaacEnc_count11,                       /* 14 */
 FDKaacEnc_count11,                       /* 15 */
 FDKaacEnc_countEsc                       /* 16 */
};



INT    FDKaacEnc_bitCount(const SHORT *values,
                          const INT   width,
                          INT         maxVal,
                          INT        *bitCount)
{

  /*
    check if we can use codebook 0
  */

  if(maxVal == 0)
    bitCount[0] = 0;
  else
    bitCount[0] = INVALID_BITCOUNT;

  maxVal = fixMin(maxVal,(INT)CODE_BOOK_ESC_LAV);
  countFuncTable[maxVal](values,width,bitCount);
  return(0);
}




/*
  count difference between actual and zeroed lines
*/
INT FDKaacEnc_countValues(SHORT *RESTRICT values, INT width, INT codeBook)
{

  INT i,t0,t1,t2,t3,t00,t01;
  INT codeLength;
  INT signLength;
  INT bitCnt=0;

  switch(codeBook){
  case CODE_BOOK_ZERO_NO:
    break;

  case CODE_BOOK_1_NO:
    for(i=0; i<width; i+=4) {
      t0         = values[i+0];
      t1         = values[i+1];
      t2         = values[i+2];
      t3         = values[i+3];
      codeLength = HI_LTAB(FDKaacEnc_huff_ltab1_2[t0+1][t1+1][t2+1][t3+1]);
      bitCnt+= codeLength;
    }
    break;

  case CODE_BOOK_2_NO:
    for(i=0; i<width; i+=4) {
      t0         = values[i+0];
      t1         = values[i+1];
      t2         = values[i+2];
      t3         = values[i+3];
      codeLength = LO_LTAB(FDKaacEnc_huff_ltab1_2[t0+1][t1+1][t2+1][t3+1]);
      bitCnt+= codeLength;
    }
    break;

  case CODE_BOOK_3_NO:
    for(i=0; i<width; i+=4) {
      signLength=0;
      t0 = values[i+0];
      if(t0 != 0){
        signLength++;
        t0=fixp_abs(t0);
      }
      t1 = values[i+1];
      if(t1 != 0){
        signLength++;
        t1=fixp_abs(t1);
      }
      t2 = values[i+2];
      if(t2 != 0){
        signLength++;
        t2=fixp_abs(t2);
      }
      t3 = values[i+3];
      if(t3 != 0){
        signLength++;
        t3=fixp_abs(t3);
      }

      codeLength = HI_LTAB(FDKaacEnc_huff_ltab3_4[t0][t1][t2][t3]);
      bitCnt+=codeLength+signLength;
    }
    break;

  case CODE_BOOK_4_NO:
    for(i=0; i<width; i+=4) {
      signLength=0;
      t0 = values[i+0];
      if(t0 != 0){
        signLength++;
        t0=fixp_abs(t0);
      }
      t1 = values[i+1];
      if(t1 != 0){
        signLength++;
        t1=fixp_abs(t1);
      }
      t2 = values[i+2];
      if(t2 != 0){
        signLength++;
        t2=fixp_abs(t2);
      }
      t3 = values[i+3];
      if(t3 != 0){
        signLength++;
        t3=fixp_abs(t3);
      }
      codeLength = LO_LTAB(FDKaacEnc_huff_ltab3_4[t0][t1][t2][t3]);
      bitCnt+=codeLength+signLength;
    }
    break;

  case CODE_BOOK_5_NO:
    for(i=0; i<width; i+=2) {
      t0         = values[i+0];
      t1         = values[i+1];
      codeLength = HI_LTAB(FDKaacEnc_huff_ltab5_6[t0+4][t1+4]);
      bitCnt+=codeLength;
    }
    break;

  case CODE_BOOK_6_NO:
    for(i=0; i<width; i+=2) {
      t0         = values[i+0];
      t1         = values[i+1];
      codeLength = LO_LTAB(FDKaacEnc_huff_ltab5_6[t0+4][t1+4]);
      bitCnt+=codeLength;
    }
    break;

  case CODE_BOOK_7_NO:
    for(i=0; i<width; i+=2){
      signLength=0;
      t0 = values[i+0];
      if(t0 != 0){
        signLength++;
        t0=fixp_abs(t0);
      }

      t1 = values[i+1];
      if(t1 != 0){
        signLength++;
        t1=fixp_abs(t1);
      }
      codeLength = HI_LTAB(FDKaacEnc_huff_ltab7_8[t0][t1]);
      bitCnt+=codeLength +signLength;
    }
    break;

  case CODE_BOOK_8_NO:
    for(i=0; i<width; i+=2) {
      signLength=0;
      t0 = values[i+0];
      if(t0 != 0){
        signLength++;
        t0=fixp_abs(t0);
      }

      t1 = values[i+1];
      if(t1 != 0){
        signLength++;
        t1=fixp_abs(t1);
      }
      codeLength = LO_LTAB(FDKaacEnc_huff_ltab7_8[t0][t1]);
      bitCnt+=codeLength +signLength;
    }
    break;

  case CODE_BOOK_9_NO:
    for(i=0; i<width; i+=2) {
      signLength=0;
      t0 = values[i+0];
      if(t0 != 0){
        signLength++;
        t0=fixp_abs(t0);
      }
      t1 = values[i+1];
      if(t1 != 0){
        signLength++;
        t1=fixp_abs(t1);
      }
      codeLength = HI_LTAB(FDKaacEnc_huff_ltab9_10[t0][t1]);
      bitCnt+=codeLength +signLength;
    }
    break;

  case CODE_BOOK_10_NO:
    for(i=0; i<width; i+=2) {
      signLength=0;
      t0 = values[i+0];
      if(t0 != 0){
        signLength++;
        t0=fixp_abs(t0);
      }

      t1 = values[i+1];
      if(t1 != 0){
        signLength++;
        t1=fixp_abs(t1);
      }
      codeLength = LO_LTAB(FDKaacEnc_huff_ltab9_10[t0][t1]);
      bitCnt+=codeLength +signLength;
    }
    break;

  case CODE_BOOK_ESC_NO:
    for(i=0; i<width; i+=2) {
      signLength=0;
      t0 = values[i+0];
      if(t0 != 0){
        signLength++;
        t0=fixp_abs(t0);
      }
      t1 = values[i+1];
      if(t1 != 0){
        signLength++;
        t1=fixp_abs(t1);
      }
      t00 = fixMin(t0,16);
      t01 = fixMin(t1,16);

      codeLength = (INT) FDKaacEnc_huff_ltab11[t00][t01];
      bitCnt+=codeLength +signLength;
      if(t0 >=16){
        INT n,p;
        n=0;
        p=t0;
        while((p>>=1) >=16){
          bitCnt++;
          n++;
        }
        bitCnt+=(n+5);
      }
      if(t1 >=16){
        INT n,p;
        n=0;
        p=t1;
        while((p>>=1) >=16){
          bitCnt++;
          n++;
        }
        bitCnt+=(n+5);
      }
    }
    break;

  default:
    break;
  }

  return(bitCnt);
}



INT FDKaacEnc_codeValues(SHORT *RESTRICT values, INT width, INT codeBook,  HANDLE_FDK_BITSTREAM hBitstream)
{

  INT i,t0,t1,t2,t3,t00,t01;
  INT codeWord,codeLength;
  INT sign,signLength;

  switch(codeBook){
  case CODE_BOOK_ZERO_NO:
    break;

  case CODE_BOOK_1_NO:
    for(i=0; i<width; i+=4) {
      t0         = values[i+0]+1;
      t1         = values[i+1]+1;
      t2         = values[i+2]+1;
      t3         = values[i+3]+1;
      codeWord   = FDKaacEnc_huff_ctab1[t0][t1][t2][t3];
      codeLength = HI_LTAB(FDKaacEnc_huff_ltab1_2[t0][t1][t2][t3]);
      FDKwriteBits(hBitstream,codeWord,codeLength);
    }
    break;

  case CODE_BOOK_2_NO:
    for(i=0; i<width; i+=4) {
      t0         = values[i+0]+1;
      t1         = values[i+1]+1;
      t2         = values[i+2]+1;
      t3         = values[i+3]+1;
      codeWord   = FDKaacEnc_huff_ctab2[t0][t1][t2][t3];
      codeLength = LO_LTAB(FDKaacEnc_huff_ltab1_2[t0][t1][t2][t3]);
      FDKwriteBits(hBitstream,codeWord,codeLength);
    }
    break;

  case CODE_BOOK_3_NO:
    for(i=0; i<width; i+=4) {
      sign=0;
      signLength=0;
      t0 = values[i+0];
      if(t0 != 0){
        signLength++;
        sign<<=1;
        if(t0 < 0){
          sign|=1;
          t0=fixp_abs(t0);
        }
      }
      t1 = values[i+1];
      if(t1 != 0){
        signLength++;
        sign<<=1;
        if(t1 < 0){
          sign|=1;
          t1=fixp_abs(t1);
        }
      }
      t2 = values[i+2];
      if(t2 != 0){
        signLength++;
        sign<<=1;
        if(t2 < 0){
          sign|=1;
          t2=fixp_abs(t2);
        }
      }
      t3 = values[i+3];
      if(t3 != 0){
        signLength++;
        sign<<=1;
        if(t3 < 0){
          sign|=1;
          t3=fixp_abs(t3);
        }
      }

      codeWord   = FDKaacEnc_huff_ctab3[t0][t1][t2][t3];
      codeLength = HI_LTAB(FDKaacEnc_huff_ltab3_4[t0][t1][t2][t3]);
      FDKwriteBits(hBitstream,codeWord,codeLength);
      FDKwriteBits(hBitstream,sign,signLength);
    }
    break;

  case CODE_BOOK_4_NO:
    for(i=0; i<width; i+=4) {
      sign=0;
      signLength=0;
      t0 = values[i+0];
      if(t0 != 0){
        signLength++;
        sign<<=1;
        if(t0 < 0){
          sign|=1;
          t0=fixp_abs(t0);
        }
      }
      t1 = values[i+1];
      if(t1 != 0){
        signLength++;
        sign<<=1;
        if(t1 < 0){
          sign|=1;
          t1=fixp_abs(t1);
        }
      }
      t2 = values[i+2];
      if(t2 != 0){
        signLength++;
        sign<<=1;
        if(t2 < 0){
          sign|=1;
          t2=fixp_abs(t2);
        }
      }
      t3 = values[i+3];
      if(t3 != 0){
        signLength++;
        sign<<=1;
        if(t3 < 0){
          sign|=1;
          t3=fixp_abs(t3);
        }
      }
      codeWord   = FDKaacEnc_huff_ctab4[t0][t1][t2][t3];
      codeLength = LO_LTAB(FDKaacEnc_huff_ltab3_4[t0][t1][t2][t3]);
      FDKwriteBits(hBitstream,codeWord,codeLength);
      FDKwriteBits(hBitstream,sign,signLength);
    }
    break;

  case CODE_BOOK_5_NO:
    for(i=0; i<width; i+=2) {
      t0         = values[i+0]+4;
      t1         = values[i+1]+4;
      codeWord   = FDKaacEnc_huff_ctab5[t0][t1];
      codeLength = HI_LTAB(FDKaacEnc_huff_ltab5_6[t0][t1]);
      FDKwriteBits(hBitstream,codeWord,codeLength);
    }
    break;

  case CODE_BOOK_6_NO:
    for(i=0; i<width; i+=2) {
      t0         = values[i+0]+4;
      t1         = values[i+1]+4;
      codeWord   = FDKaacEnc_huff_ctab6[t0][t1];
      codeLength = LO_LTAB(FDKaacEnc_huff_ltab5_6[t0][t1]);
      FDKwriteBits(hBitstream,codeWord,codeLength);
    }
    break;

  case CODE_BOOK_7_NO:
    for(i=0; i<width; i+=2){
      sign=0;
      signLength=0;
      t0 = values[i+0];
      if(t0 != 0){
        signLength++;
        sign<<=1;
        if(t0 < 0){
          sign|=1;
          t0=fixp_abs(t0);
        }
      }

      t1 = values[i+1];
      if(t1 != 0){
        signLength++;
        sign<<=1;
        if(t1 < 0){
          sign|=1;
          t1=fixp_abs(t1);
        }
      }
      codeWord   = FDKaacEnc_huff_ctab7[t0][t1];
      codeLength = HI_LTAB(FDKaacEnc_huff_ltab7_8[t0][t1]);
      FDKwriteBits(hBitstream,codeWord,codeLength);
      FDKwriteBits(hBitstream,sign,signLength);
    }
    break;

  case CODE_BOOK_8_NO:
    for(i=0; i<width; i+=2) {
      sign=0;
      signLength=0;
      t0 = values[i+0];
      if(t0 != 0){
        signLength++;
        sign<<=1;
        if(t0 < 0){
          sign|=1;
          t0=fixp_abs(t0);
        }
      }

      t1 = values[i+1];
      if(t1 != 0){
        signLength++;
        sign<<=1;
        if(t1 < 0){
          sign|=1;
          t1=fixp_abs(t1);
        }
      }
      codeWord   = FDKaacEnc_huff_ctab8[t0][t1];
      codeLength = LO_LTAB(FDKaacEnc_huff_ltab7_8[t0][t1]);
      FDKwriteBits(hBitstream,codeWord,codeLength);
      FDKwriteBits(hBitstream,sign,signLength);
    }
    break;

  case CODE_BOOK_9_NO:
    for(i=0; i<width; i+=2) {
      sign=0;
      signLength=0;
      t0 = values[i+0];
      if(t0 != 0){
        signLength++;
        sign<<=1;
        if(t0 < 0){
          sign|=1;
          t0=fixp_abs(t0);
        }
      }
      t1 = values[i+1];
      if(t1 != 0){
        signLength++;
        sign<<=1;
        if(t1 < 0){
          sign|=1;
          t1=fixp_abs(t1);
        }
      }
      codeWord   = FDKaacEnc_huff_ctab9[t0][t1];
      codeLength = HI_LTAB(FDKaacEnc_huff_ltab9_10[t0][t1]);
      FDKwriteBits(hBitstream,codeWord,codeLength);
      FDKwriteBits(hBitstream,sign,signLength);
    }
    break;

  case CODE_BOOK_10_NO:
    for(i=0; i<width; i+=2) {
      sign=0;
      signLength=0;
      t0 = values[i+0];
      if(t0 != 0){
        signLength++;
        sign<<=1;
        if(t0 < 0){
          sign|=1;
          t0=fixp_abs(t0);
        }
      }

      t1 = values[i+1];
      if(t1 != 0){
        signLength++;
        sign<<=1;
        if(t1 < 0){
          sign|=1;
          t1=fixp_abs(t1);
        }
      }
      codeWord   = FDKaacEnc_huff_ctab10[t0][t1];
      codeLength = LO_LTAB(FDKaacEnc_huff_ltab9_10[t0][t1]);
      FDKwriteBits(hBitstream,codeWord,codeLength);
      FDKwriteBits(hBitstream,sign,signLength);
    }
    break;

  case CODE_BOOK_ESC_NO:
    for(i=0; i<width; i+=2) {
      sign=0;
      signLength=0;
      t0 = values[i+0];
      if(t0 != 0){
        signLength++;
        sign<<=1;
        if(t0 < 0){
          sign|=1;
          t0=fixp_abs(t0);
        }
      }
      t1 = values[i+1];
      if(t1 != 0){
        signLength++;
        sign<<=1;
        if(t1 < 0){
          sign|=1;
          t1=fixp_abs(t1);
        }
      }
      t00 = fixMin(t0,16);
      t01 = fixMin(t1,16);

      codeWord   = FDKaacEnc_huff_ctab11[t00][t01];
      codeLength = (INT) FDKaacEnc_huff_ltab11[t00][t01];
      FDKwriteBits(hBitstream,codeWord,codeLength);
      FDKwriteBits(hBitstream,sign,signLength);
      if(t0 >=16){
        INT n,p;
        n=0;
        p=t0;
        while((p>>=1) >=16){
          FDKwriteBits(hBitstream,1,1);
          n++;
        }
        FDKwriteBits(hBitstream,0,1);
        FDKwriteBits(hBitstream,t0-(1<<(n+4)),n+4);
      }
      if(t1 >=16){
        INT n,p;
        n=0;
        p=t1;
        while((p>>=1) >=16){
          FDKwriteBits(hBitstream,1,1);
          n++;
        }
        FDKwriteBits(hBitstream,0,1);
        FDKwriteBits(hBitstream,t1-(1<<(n+4)),n+4);
      }
    }
    break;

  default:
    break;
  }
  return(0);
}

INT FDKaacEnc_codeScalefactorDelta(INT delta, HANDLE_FDK_BITSTREAM hBitstream)
{
  INT codeWord,codeLength;

  if(fixp_abs(delta) >CODE_BOOK_SCF_LAV)
    return(1);

  codeWord   = FDKaacEnc_huff_ctabscf[delta+CODE_BOOK_SCF_LAV];
  codeLength = (INT)FDKaacEnc_huff_ltabscf[delta+CODE_BOOK_SCF_LAV];
  FDKwriteBits(hBitstream,codeWord,codeLength);
  return(0);
}



