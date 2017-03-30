#undef  _DEBUG_NEW_REDEFINE_NEW
#define _DEBUG_NEW_REDEFINE_NEW 0
#include "debug_new.h"


#ifdef _DEBUG
void* operator new(size_t size, const char* file, int line)
{
	return alloc_mem(size, file, line, false);
}

void* operator new(size_t size) throw(std::bad_alloc)
{
	return alloc_mem(size, NULL, 0, false);
}

void* operator new(size_t size, const std::nothrow_t&) _NOEXCEPT
{
	return alloc_mem(size, NULL, 0, false);
}

void* operator new[](size_t size, const char* file, int line)
{
	return alloc_mem(size, file, line, true);
}

void* operator new[](size_t size) throw(std::bad_alloc)
{
	return alloc_mem(size, NULL, 0, true);
}

void* operator new[](size_t size, const std::nothrow_t&) _NOEXCEPT
{
	return alloc_mem(size, NULL, 0, true);
}

void operator delete(void* ptr, const char* file, int line) _NOEXCEPT
{
	free_pointer(ptr, NULL, false);
}

void operator delete(void* ptr) _NOEXCEPT
{
	free_pointer(ptr, NULL, false);
}

void operator delete(void* ptr, const std::nothrow_t&) _NOEXCEPT
{
	free_pointer(ptr, NULL, false);
}

void operator delete[](void* ptr, const char* file, int line) _NOEXCEPT
{
	free_pointer(ptr, NULL, true);
}

void operator delete[](void* ptr) _NOEXCEPT
{
	free_pointer(ptr, NULL, true);
}

void operator delete[](void* ptr, const std::nothrow_t&) _NOEXCEPT
{
	free_pointer(ptr, NULL, true);
}
#else
void* operator new(size_t size) throw(std::bad_alloc)
{
	return CrossEngine::Alloc(size, CrossEngine::MEMTYPE_HEAP);
}

void* operator new(size_t size, const std::nothrow_t&) _NOEXCEPT
{
	return CrossEngine::Alloc(size, CrossEngine::MEMTYPE_HEAP);
}

void* operator new[](size_t size) throw(std::bad_alloc)
{
	return CrossEngine::Alloc(size, CrossEngine::MEMTYPE_HEAP);
}

void* operator new[](size_t size, const std::nothrow_t&) _NOEXCEPT
{
	return CrossEngine::Alloc(size, CrossEngine::MEMTYPE_HEAP);
}

void operator delete(void* ptr) _NOEXCEPT
{
	CrossEngine::Free(ptr);
}

void operator delete(void* ptr, const std::nothrow_t&) _NOEXCEPT
{
	CrossEngine::Free(ptr);
}

void operator delete[](void* ptr) _NOEXCEPT
{
	CrossEngine::Free(ptr);
}

void operator delete[](void* ptr, const std::nothrow_t&) _NOEXCEPT
{
	CrossEngine::Free(ptr);
}
#endif
