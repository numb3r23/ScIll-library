#include "CRTPingPong.hpp"

#include "SFilterEngine.hpp"

using namespace SciIllLib;

SciIllLib::CRTPingPong::CRTPingPong()
: mIdx(0)
{
}

void SciIllLib::CRTPingPong::createRenderTargets(int width, int height)
{
  mTexturesPingPong[0] = createRenderTexture(width, height);
  mTexturesPingPong[1] = createRenderTexture(width, height);
}

ATexture& SciIllLib::CRTPingPong::operator[](int idx)
{
  return mTexturesPingPong[idx % 2];
}

void SciIllLib::CRTPingPong::setIndex(int idx)
{
  mIdx = idx % 2;
}

ATexture& SciIllLib::CRTPingPong::getPreviousTarget()
{
  return mTexturesPingPong[(mIdx + 1) % 2];
}

ATexture& SciIllLib::CRTPingPong::getCurrentTarget()
{
  return mTexturesPingPong[mIdx];
}

void SciIllLib::CRTPingPong::tick()
{
  mIdx = (mIdx + 1) % 2;
}

ATexture SciIllLib::CRTPingPong::createRenderTexture(int width, int height)
{
  GLuint tex = 0;
  SFilterEngine::ReGenerateTextureSize(&tex, GL_FLOAT, width, height);
  return ATexture(tex, width, height);
}
