/****************************************************************************
Copyright (c) 2017 LiangYue.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sub license, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "_CrossEngine.h"


namespace CrossEngine {

	void* _malloc(size_t size)
	{
		ASSERT(size > 0);

		void *ptr = NULL;

#if defined _PLATFORM_WINDOWS_

		ptr = _aligned_malloc(size, 16);

#elif defined _PLATFORM_ANDROID_

		ptr = memalign(16, size);

#else

		ptr = malloc(size);

#endif

		if (ptr == NULL) {
			LOGE("Error: malloc system memory failed.\n");
		}

		return ptr;
	}

	void _free(void *ptr)
	{
		ASSERT(ptr);

#if defined _PLATFORM_WINDOWS_

		_aligned_free(ptr);

#else

		free(ptr);

#endif
	}

}
