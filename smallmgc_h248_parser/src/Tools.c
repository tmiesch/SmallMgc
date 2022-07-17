/********************************************************************
  Except for a few trivial modifications, the source code for the
  CRC32 cyclic redundancy check is freely available in RFC 3309.
  <http://www.ietf.org/rfc.html>

Full Copyright Statement

   Copyright (C) The Internet Society (2002).  All Rights Reserved.

   This document and translations of it may be copied and furnished to
   others, and derivative works that comment on or otherwise explain it
   or assist in its implementation may be prepared, copied, published
   and distributed, in whole or in part, without restriction of any
   kind, provided that the above copyright notice and this paragraph are
   included on all such copies and derivative works.  However, this
   document itself may not be modified in any way, such as by removing
   the copyright notice or references to the Internet Society or other
   Internet organizations, except as needed for the purpose of
   developing Internet standards in which case the procedures for
   copyrights defined in the Internet Standards process must be
   followed, or as required to translate it into languages other than
   English.

   The limited permissions granted above are perpetual and will not be
   revoked by the Internet Society or its successors or assigns.

   This document and the information contained herein is provided on an
   "AS IS" basis and THE INTERNET SOCIETY AND THE INTERNET ENGINEERING
   TASK FORCE DISCLAIMS ALL WARRANTIES, EXPRESS OR IMPLIED, INCLUDING
   BUT NOT LIMITED TO ANY WARRANTY THAT THE USE OF THE INFORMATION
   HEREIN WILL NOT INFRINGE ANY RIGHTS OR ANY IMPLIED WARRANTIES OF
   MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.

    author(modifications only): Lowell Thomas
            lowell@coasttocoastresearch.com
            http://www.coasttocoastresearch.com

   created: 8/25/2007 10:18
  filename: D:\Projects\Apgv5_0\ApgLib\Tools.c
   purpose: Compute the 32-bit cyclic redundancy check (hash) value
            of an arbitrary-length string of bytes.

*********************************************************************/
/********************************************************************
      RFC Copyright Notice

  This file contains a modified version of code and/or
  syntax grammar published in:

      RFC 3309
      Copyright (C) The Internet Society (2002)
      <http://www.ietf.org/rfc.html>
*********************************************************************/

#include "Apg.h"

//////////////////////////////////////////////////////////////////////////
// CYCLIC REDUNDANCY CHECK
// cut and paste from RFC 3309
//  - with slight editing
//////////////////////////////////////////////////////////////////////////
#define CRC32C(c,d) (c=(c>>8)^crc_c[(c^(d))&0xFF])
ulong  crc_c[256] =
{
  0x00000000L, 0xF26B8303L, 0xE13B70F7L, 0x1350F3F4L,
    0xC79A971FL, 0x35F1141CL, 0x26A1E7E8L, 0xD4CA64EBL,
    0x8AD958CFL, 0x78B2DBCCL, 0x6BE22838L, 0x9989AB3BL,
    0x4D43CFD0L, 0xBF284CD3L, 0xAC78BF27L, 0x5E133C24L,
    0x105EC76FL, 0xE235446CL, 0xF165B798L, 0x030E349BL,
    0xD7C45070L, 0x25AFD373L, 0x36FF2087L, 0xC494A384L,
    0x9A879FA0L, 0x68EC1CA3L, 0x7BBCEF57L, 0x89D76C54L,
    0x5D1D08BFL, 0xAF768BBCL, 0xBC267848L, 0x4E4DFB4BL,
    0x20BD8EDEL, 0xD2D60DDDL, 0xC186FE29L, 0x33ED7D2AL,
    0xE72719C1L, 0x154C9AC2L, 0x061C6936L, 0xF477EA35L,
    0xAA64D611L, 0x580F5512L, 0x4B5FA6E6L, 0xB93425E5L,
    0x6DFE410EL, 0x9F95C20DL, 0x8CC531F9L, 0x7EAEB2FAL,
    0x30E349B1L, 0xC288CAB2L, 0xD1D83946L, 0x23B3BA45L,
    0xF779DEAEL, 0x05125DADL, 0x1642AE59L, 0xE4292D5AL,
    0xBA3A117EL, 0x4851927DL, 0x5B016189L, 0xA96AE28AL,
    0x7DA08661L, 0x8FCB0562L, 0x9C9BF696L, 0x6EF07595L,
    0x417B1DBCL, 0xB3109EBFL, 0xA0406D4BL, 0x522BEE48L,
    0x86E18AA3L, 0x748A09A0L, 0x67DAFA54L, 0x95B17957L,
    0xCBA24573L, 0x39C9C670L, 0x2A993584L, 0xD8F2B687L,
    0x0C38D26CL, 0xFE53516FL, 0xED03A29BL, 0x1F682198L,
    0x5125DAD3L, 0xA34E59D0L, 0xB01EAA24L, 0x42752927L,
    0x96BF4DCCL, 0x64D4CECFL, 0x77843D3BL, 0x85EFBE38L,
    0xDBFC821CL, 0x2997011FL, 0x3AC7F2EBL, 0xC8AC71E8L,
    0x1C661503L, 0xEE0D9600L, 0xFD5D65F4L, 0x0F36E6F7L,
    0x61C69362L, 0x93AD1061L, 0x80FDE395L, 0x72966096L,
    0xA65C047DL, 0x5437877EL, 0x4767748AL, 0xB50CF789L,
    0xEB1FCBADL, 0x197448AEL, 0x0A24BB5AL, 0xF84F3859L,
    0x2C855CB2L, 0xDEEEDFB1L, 0xCDBE2C45L, 0x3FD5AF46L,
    0x7198540DL, 0x83F3D70EL, 0x90A324FAL, 0x62C8A7F9L,
    0xB602C312L, 0x44694011L, 0x5739B3E5L, 0xA55230E6L,
    0xFB410CC2L, 0x092A8FC1L, 0x1A7A7C35L, 0xE811FF36L,
    0x3CDB9BDDL, 0xCEB018DEL, 0xDDE0EB2AL, 0x2F8B6829L,
    0x82F63B78L, 0x709DB87BL, 0x63CD4B8FL, 0x91A6C88CL,
    0x456CAC67L, 0xB7072F64L, 0xA457DC90L, 0x563C5F93L,
    0x082F63B7L, 0xFA44E0B4L, 0xE9141340L, 0x1B7F9043L,
    0xCFB5F4A8L, 0x3DDE77ABL, 0x2E8E845FL, 0xDCE5075CL,
    0x92A8FC17L, 0x60C37F14L, 0x73938CE0L, 0x81F80FE3L,
    0x55326B08L, 0xA759E80BL, 0xB4091BFFL, 0x466298FCL,
    0x1871A4D8L, 0xEA1A27DBL, 0xF94AD42FL, 0x0B21572CL,
    0xDFEB33C7L, 0x2D80B0C4L, 0x3ED04330L, 0xCCBBC033L,
    0xA24BB5A6L, 0x502036A5L, 0x4370C551L, 0xB11B4652L,
    0x65D122B9L, 0x97BAA1BAL, 0x84EA524EL, 0x7681D14DL,
    0x2892ED69L, 0xDAF96E6AL, 0xC9A99D9EL, 0x3BC21E9DL,
    0xEF087A76L, 0x1D63F975L, 0x0E330A81L, 0xFC588982L,
    0xB21572C9L, 0x407EF1CAL, 0x532E023EL, 0xA145813DL,
    0x758FE5D6L, 0x87E466D5L, 0x94B49521L, 0x66DF1622L,
    0x38CC2A06L, 0xCAA7A905L, 0xD9F75AF1L, 0x2B9CD9F2L,
    0xFF56BD19L, 0x0D3D3E1AL, 0x1E6DCDEEL, 0xEC064EEDL,
    0xC38D26C4L, 0x31E6A5C7L, 0x22B65633L, 0xD0DDD530L,
    0x0417B1DBL, 0xF67C32D8L, 0xE52CC12CL, 0x1747422FL,
    0x49547E0BL, 0xBB3FFD08L, 0xA86F0EFCL, 0x5A048DFFL,
    0x8ECEE914L, 0x7CA56A17L, 0x6FF599E3L, 0x9D9E1AE0L,
    0xD3D3E1ABL, 0x21B862A8L, 0x32E8915CL, 0xC083125FL,
    0x144976B4L, 0xE622F5B7L, 0xF5720643L, 0x07198540L,
    0x590AB964L, 0xAB613A67L, 0xB831C993L, 0x4A5A4A90L,
    0x9E902E7BL, 0x6CFBAD78L, 0x7FAB5E8CL, 0x8DC0DD8FL,
    0xE330A81AL, 0x115B2B19L, 0x020BD8EDL, 0xF0605BEEL,
    0x24AA3F05L, 0xD6C1BC06L, 0xC5914FF2L, 0x37FACCF1L,
    0x69E9F0D5L, 0x9B8273D6L, 0x88D28022L, 0x7AB90321L,
    0xAE7367CAL, 0x5C18E4C9L, 0x4F48173DL, 0xBD23943EL,
    0xF36E6F75L, 0x0105EC76L, 0x12551F82L, 0xE03E9C81L,
    0x34F4F86AL, 0xC69F7B69L, 0xD5CF889DL, 0x27A40B9EL,
    0x79B737BAL, 0x8BDCB4B9L, 0x988C474DL, 0x6AE7C44EL,
    0xBE2DA0A5L, 0x4C4623A6L, 0x5F16D052L, 0xAD7D5351L,
};

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: Computes the cyclic redundancy check (hash)
// of a string of bytes.
//
// FUNCTION: ulong ulCRC32
//
//      ARG: 1) uchar *buffer
//              : pointer to the first byte in the string
//
//      ARG: 2) ulong length
//              : number of bytes in the string
//
//   RETURN: the computed value
//
//////////////////////////////////////////////////////////////////////////
ulong ulCRC32(uchar *buffer, ulong length)
{
  ulong i;
  ulong crc32 = ~0L;
  for (i = 0; i < length; i++){
    CRC32C(crc32, buffer[i]);
  }

  return crc32;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: Computes the case insensitive cyclic redundancy check (hash)
// of a string of ASCII characters.
//
// Same as ulCRC32() except that bytes in the range of ASCII alphabetic
// characters are converted to lower case. All alphabetic strings
// will yield the same result regardless of case.
// e.g. This, THIS and ThIs will yield the same hash value
//
// FUNCTION: ulong ulcrc32
//
//      ARG: 1) uchar *buffer
//              : pointer to the first character in the string
//
//      ARG: 2) ulong length
//              : number of characters in the string
//
//   RETURN: the computed value
//
//////////////////////////////////////////////////////////////////////////
ulong ulcrc32(uchar *buffer, ulong length)
{
  ulong i;
  uchar ucChar;
  ulong crc32 = ~0L;
  for (i = 0; i < length; i++)
  {
    ucChar = buffer[i];
    if(ucChar >= 65 && ucChar <= 90)
    {
      // convert to lower case
      ucChar = ucChar + 32;
    }
    CRC32C(crc32, ucChar);
  }

  return crc32;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: Initialize the computation of a cyclic redundancy check (hash)
// in which the string of bytes is in a non-contiguous set of blocks.
// A "chained" computation will compute the hash of the string in a series
// of computations on the individual blocks yield the same result as if
// the blocks had been chained together in one contiguous block.
//
// FUNCTION: ulong ulChainedCRC32Init
//
//   RETURN: initial hash value
//
//////////////////////////////////////////////////////////////////////////
ulong ulChainedCRC32Init()
{
  return ~0L;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: Compute the cyclic redundancy check (hash) value of one
// block from a series of one or more chained blocks.
//
// FUNCTION: ulong ulChainedCRC32
//
//      ARG: 1) ulong ulPrevValue
//              : the has value from a previous call to ulChainedCRC32()
//
//      ARG: 2) uchar *buffer
//              : pointer to a string of bytes
//
//      ARG: 3) ulong length
//              : number of bytes in the string
//
//   RETURN: the hash result for the string consisting of the previous
//           string chained to this block.
//
//////////////////////////////////////////////////////////////////////////
ulong ulChainedCRC32(ulong ulPrevValue, uchar *buffer, ulong length)
{
  ulong i;
  if(buffer)
  {
    for (i = 0; i < length; i++)
    {
      CRC32C(ulPrevValue, buffer[i]);
    }
  }

  return ulPrevValue;
}

//////////////////////////////////////////////////////////////////////////
// DESCRIPTION: Case-insensitive: compares two strings.
// The two strings are compared, character for character.
// NOTE: stricmp() is non-ANSI and therefore, not used
//
// FUNCTION: int apg_stricmp
//
//      ARG: 1) const char* cpLhs
//              : pointer to a string
//
//      ARG: 2) const char* cpRhs
//              : pointer to a string
//
//   RETURN: 0 if the two strings are case-insensitive equal
//           -1 if cpLhs is "less than" cpRhs
//            1 if cpLhs is "greater than" cpRhs
//
//           0 includes the case that both string pointers are empty
//             a NULL pointer is considered and empty string
//////////////////////////////////////////////////////////////////////////
int apg_stricmp(const char* cpLhs, const char* cpRhs)
{
  ulong ulLhsEmpty = APG_FALSE;
  ulong ulRhsEmpty = APG_FALSE;

  // handle any empty strings
  if(cpLhs == NULL || *cpLhs == 0){ulLhsEmpty = APG_TRUE;}
  if(cpRhs == NULL || *cpRhs == 0){ulRhsEmpty = APG_TRUE;}
  if(ulLhsEmpty && ulRhsEmpty){return 0;}
  if(ulLhsEmpty){return -1;}
  if(ulRhsEmpty){return 1;}

  // compare the strings
  while(APG_TRUE)
  {
    char cLhs = *cpLhs;
    char cRhs = *cpRhs;
    if(cLhs >= 65 && cLhs <= 90){cLhs += 32;}
    if(cRhs >= 65 && cRhs <= 90){cRhs += 32;}
    if(cLhs < cRhs){return -1;}
    if(cLhs > cRhs){return 1;}
    if(cLhs == 0){return 0;}
    cpLhs++;
    cpRhs++;
  }
}
