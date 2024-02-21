#ifndef FCompressorH
#define FCompressorH

#include "bz2\bzlib.h"
#include <assert.h>

#define FCOMPRESSOR_TYPE_STORE 0
#define FCOMPRESSOR_TYPE_BZ2   1

class FCompressor {
//   bz_stream strm_C;
   bz_stream strm_D;
public:
//   bool CompressBlock(char** ppDestBuf, unsigned* pDestLen, char* pSrcBuf, unsigned SrcLen);
   bool DecompressBlock(char** ppDestBuf, unsigned* pDestLen, char* pSrcBuf);

   bool Initialize(void);
   bool Release(void);

   FCompressor(){};
   ~FCompressor()
   {
      Release();
   };
};

#endif
