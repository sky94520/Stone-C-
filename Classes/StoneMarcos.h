#ifndef __Stone_StoneMacros_H__
#define __Stone_StoneMacros_H__

#define NS_STONE_BEGIN namespace Stone{
#define NS_STONE_END }

#define USING_NS_STONE using namespace Stone;

#define STONE_SAFE_DELETE(p) do{ if(p) delete p; p=nullptr;}while(0)

#endif