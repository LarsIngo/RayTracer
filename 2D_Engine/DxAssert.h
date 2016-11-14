#pragma once

#ifdef NDEBUG
#define DxAssert(x, y) x
#else
#include <assert.h>
#define DxAssert(x, y) assert(x == y)
#endif // NDEBUG

