#include <stdlib.h>

void* alloc_mem(size_t size, const char* file, int line, bool is_array);

void* operator new[] (size_t size, const char* name, int flags, unsigned debugFlags, const char* file, int line)
{
	return alloc_mem(size, file, line, true);
//	return malloc(size);
}

void* operator new[](size_t size, size_t alignment, size_t alignmentOffset, const char* name, int flags, unsigned debugFlags, const char* file, int line)
{
	return alloc_mem(size, file, line, true);
//	return _aligned_offset_malloc(size, alignment, alignmentOffset);
}
