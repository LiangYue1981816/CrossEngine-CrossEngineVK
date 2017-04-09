// -*- Mode: C++; tab-width: 4; c-basic-offset: 4; indent-tabs-mode: nil -*-
// vim:tabstop=4:shiftwidth=4:expandtab:

/*
 * Copyright (C) 2004-2013 Wu Yongwei <adah at users dot sourceforge dot net>
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any
 * damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute
 * it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must
 *    not claim that you wrote the original software.  If you use this
 *    software in a product, an acknowledgement in the product
 *    documentation would be appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must
 *    not be misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source
 *    distribution.
 *
 * This file is part of Stones of Nvwa:
 *      http://sourceforge.net/projects/nvwa
 *
 */

/**
 * @file  debug_new.h
 *
 * Header file for checking leaks caused by unmatched new/delete.
 *
 * @date  2013-10-06
 */

#ifndef NVWA_DEBUG_NEW_H
#define NVWA_DEBUG_NEW_H

#include <new>
#include <stdio.h>
#include "CrossEngine.h"

#ifdef _MSC_VER
#pragma warning(disable: 4074)
#pragma warning(disable: 4290)
#if _MSC_VER >= 1400
#pragma warning(disable: 4996)
#endif
#pragma init_seg(compiler)
#endif

 /* Prototypes */
CROSS_EXPORT void* alloc_mem(size_t size, const char* file, int line, bool is_array);
CROSS_EXPORT void free_pointer(void* usr_ptr, void* addr, bool is_array);
CROSS_EXPORT int check_leaks();
CROSS_EXPORT int check_mem_corruption();

/* Special allocation/deallocation functions in the global scope */
#ifdef _DEBUG
#ifndef _DEBUG_NEW_REDEFINE_NEW
#define _DEBUG_NEW_REDEFINE_NEW 1
#endif
void* operator new(size_t size, const char* file, int line);
void* operator new(size_t size) throw(std::bad_alloc);
void* operator new(size_t size, const std::nothrow_t&) _NOEXCEPT;
void* operator new[](size_t size, const char* file, int line);
void* operator new[](size_t size) throw(std::bad_alloc);
void* operator new[](size_t size, const std::nothrow_t&) _NOEXCEPT;
void operator delete(void* ptr, const char* file, int line) _NOEXCEPT;
void operator delete(void* ptr) _NOEXCEPT;
void operator delete(void* ptr, const std::nothrow_t&) _NOEXCEPT;
void operator delete[](void* ptr, const char* file, int line) _NOEXCEPT;
void operator delete[](void* ptr) _NOEXCEPT;
void operator delete[](void* ptr, const std::nothrow_t&) _NOEXCEPT;
#else
void* operator new(size_t size) throw(std::bad_alloc);
void* operator new(size_t size, const std::nothrow_t&) _NOEXCEPT;
void* operator new[](size_t size) throw(std::bad_alloc);
void* operator new[](size_t size, const std::nothrow_t&) _NOEXCEPT;
void operator delete(void* ptr) _NOEXCEPT;
void operator delete(void* ptr, const std::nothrow_t&) _NOEXCEPT;
void operator delete[](void* ptr) _NOEXCEPT;
void operator delete[](void* ptr, const std::nothrow_t&) _NOEXCEPT;
#endif

/**
 * @def DEBUG_NEW
 *
 * Macro to catch file/line information on allocation.  If
 * #_DEBUG_NEW_REDEFINE_NEW is \c 0, one can use this macro directly;
 * otherwise \c new will be defined to it, and one must use \c new
 * instead.
 */
#define DEBUG_NEW new(__FILE__, __LINE__)

# if _DEBUG_NEW_REDEFINE_NEW
#   define new DEBUG_NEW
# endif

#endif // NVWA_DEBUG_NEW_H
