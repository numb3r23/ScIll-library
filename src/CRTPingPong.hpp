#ifndef CRTPINGPONG_H
#define CRTPINGPONG_H

#include "stdafx.h"

#include "ATexture.hpp"

namespace SciIllLib {
  
  class CRTPingPong
  {
  public:
    CRTPingPong();
    
    void createRenderTargets(int width, int height);
    ATexture& operator[](int idx);

    void setIndex(int idx);
    
    ATexture& getPreviousTarget();
    ATexture& getCurrentTarget();
    
    void tick();
    
    static ATexture createRenderTexture(int width, int height);
    
  private:
    int mIdx;
    ATexture mTexturesPingPong[2];
  };
}
#endif // CRTPINGPONG_H
