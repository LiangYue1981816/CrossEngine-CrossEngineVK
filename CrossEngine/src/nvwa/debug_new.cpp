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
 * @file  debug_new.cpp
 *
 * Implementation of debug versions of new and delete to check leakage.
 *
 * @date  2013-12-31
 */

#include <new>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))
#include <alloca.h>
#endif
#ifdef _WIN32
#include <malloc.h>
#endif
#include "debug_new.h"
#include "fast_mutex.h"
#include "static_assert.h"

#if !_FAST_MUTEX_CHECK_INITIALIZATION && !defined(_NOTHREADS)
#error "_FAST_MUTEX_CHECK_INITIALIZATION not set: check_leaks may not work"
#endif

/**
 * @def _DEBUG_NEW_ALIGNMENT
 *
 * The alignment requirement of allocated memory blocks.  It must be a
 * power of two.
 */
#ifndef _DEBUG_NEW_ALIGNMENT
#define _DEBUG_NEW_ALIGNMENT 16
#endif

/**
 * @def _DEBUG_NEW_ERROR_ACTION
 *
 * The action to take when an error occurs.  The default behaviour is to
 * call \e abort, unless \c _DEBUG_NEW_ERROR_CRASH is defined, in which
 * case a segmentation fault will be triggered instead (which can be
 * useful on platforms like Windows that do not generate a core dump
 * when \e abort is called).
 */
#ifndef _DEBUG_NEW_ERROR_ACTION
#ifndef _DEBUG_NEW_ERROR_CRASH
#define _DEBUG_NEW_ERROR_ACTION abort()
#else
#define _DEBUG_NEW_ERROR_ACTION do { *((char*)0) = 0; abort(); } while (0)
#endif
#endif

/**
 * @def _DEBUG_NEW_TAILCHECK
 *
 * Macro to indicate whether a writing-past-end check will be performed.
 * Define it to a positive integer as the number of padding bytes at the
 * end of a memory block for checking.
 */
#ifndef _DEBUG_NEW_TAILCHECK
#define _DEBUG_NEW_TAILCHECK 0
#endif

/**
 * @def _DEBUG_NEW_TAILCHECK_CHAR
 *
 * Value of the padding bytes at the end of a memory block.
 */
#ifndef _DEBUG_NEW_TAILCHECK_CHAR
#define _DEBUG_NEW_TAILCHECK_CHAR 0xCC
#endif

 /**
 * Gets the aligned value of memory block size.
 */
#define ALIGN(s) (((s) + _DEBUG_NEW_ALIGNMENT - 1) & ~(_DEBUG_NEW_ALIGNMENT - 1))

/**
 * Structure to store the position information where \c new occurs.
 */
struct new_ptr_list_t
{
    new_ptr_list_t* next;       ///< Pointer to the next memory block
    new_ptr_list_t* prev;       ///< Pointer to the previous memory block
    size_t          size;       ///< Size of the memory block
    union
    {
    const char*     file;       ///< Pointer to the file name of the caller
    void*           addr;       ///< Address of the caller to \e new
    };
    unsigned        line   :31; ///< Line number of the caller; or \c 0
    unsigned        is_array:1; ///< Non-zero iff <em>new[]</em> is used
    unsigned        magic;      ///< Magic number for error detection
};

/**
 * Definition of the constant magic number used for error detection.
 */
static const unsigned DEBUG_NEW_MAGIC = 0x4442474E;

/**
 * The extra memory allocated by <code>operator new</code>.
 */
static const int ALIGNED_LIST_ITEM_SIZE = ALIGN(sizeof(new_ptr_list_t));

/**
 * List of all new'd pointers.
 */
static new_ptr_list_t new_ptr_list = {
    &new_ptr_list,
    &new_ptr_list,
    0,
    {
        NULL
    },
    0,
    0,
    DEBUG_NEW_MAGIC
};

/**
 * The mutex guard to protect simultaneous access to the pointer list.
 */
static fast_mutex new_ptr_lock;

/**
 * Total memory allocated in bytes.
 */
static size_t total_mem_alloc = 0;

/**
 * Prints the position information of a memory operation point.  When \c
 * _DEBUG_NEW_USE_ADDR2LINE is defined to a non-zero value, this
 * function will try to convert a given caller address to file/line
 * information with \e addr2line.
 *
 * @param ptr   source file name if \e line is non-zero; caller address
 *              otherwise
 * @param line  source line number if non-zero; indication that \e ptr
 *              is the caller address otherwise
 */
static void print_position(const void* ptr, int line)
{
    if (line != 0)          // Is file/line information present?
    {
        LOGI("%s:%d", (const char*)ptr, line);
    }
    else if (ptr != NULL)   // Is caller address present?
    {
        LOGI("%p", ptr);
    }
    else                    // No information is present
    {
        LOGI("<Unknown>");
    }
}

#if _DEBUG_NEW_TAILCHECK
/**
 * Checks whether the padding bytes at the end of a memory block is
 * tampered with.
 *
 * @param ptr  pointer to a new_ptr_list_t struct
 * @return     \c true if the padding bytes are untouched; \c false
 *             otherwise
 */
static bool check_tail(new_ptr_list_t* ptr)
{
    const unsigned char* const tail_ptr = (unsigned char*)ptr +
                            ALIGNED_LIST_ITEM_SIZE + ptr->size;
    for (int i = 0; i < _DEBUG_NEW_TAILCHECK; ++i)
        if (tail_ptr[i] != _DEBUG_NEW_TAILCHECK_CHAR)
            return false;
    return true;
}
#endif

/**
 * Allocates memory and initializes control data.
 *
 * @param size      size of the required memory block
 * @param file      null-terminated string of the file name
 * @param line      line number
 * @param is_array  boolean value whether this is an array operation
 * @return          pointer to the user-requested memory area; \c NULL
 *                  if memory allocation is not successful
 */
CROSS_EXPORT void* alloc_mem(size_t size, const char* file, int line, bool is_array)
{
    assert(line >= 0);
    STATIC_ASSERT((_DEBUG_NEW_ALIGNMENT & (_DEBUG_NEW_ALIGNMENT - 1)) == 0, Alignment_must_be_power_of_two);
    STATIC_ASSERT(_DEBUG_NEW_TAILCHECK >= 0, Invalid_tail_check_length);
    size_t s = size + ALIGNED_LIST_ITEM_SIZE + _DEBUG_NEW_TAILCHECK;
	new_ptr_list_t* ptr = (new_ptr_list_t*)CrossEngine::Alloc(s, CrossEngine::MEMTYPE_HEAP);
    if (ptr == NULL)
    {
        LOGI("Out of memory when allocating %lu bytes\n", (unsigned long)size);
        _DEBUG_NEW_ERROR_ACTION;
    }
    void* usr_ptr = (char*)ptr + ALIGNED_LIST_ITEM_SIZE;
    ptr->file = file;
    ptr->line = line;
    ptr->is_array = is_array;
    ptr->size = size;
    ptr->magic = DEBUG_NEW_MAGIC;
    {
        fast_mutex_autolock lock(new_ptr_lock);
        ptr->prev = new_ptr_list.prev;
        ptr->next = &new_ptr_list;
        new_ptr_list.prev->next = ptr;
        new_ptr_list.prev = ptr;
    }
#if _DEBUG_NEW_TAILCHECK
    memset((char*)usr_ptr + size, _DEBUG_NEW_TAILCHECK_CHAR,
                                  _DEBUG_NEW_TAILCHECK);
#endif
    total_mem_alloc += size;
    return usr_ptr;
}

/**
 * Frees memory and adjusts pointers.
 *
 * @param usr_ptr   pointer to the previously allocated memory
 * @param addr      pointer to the caller
 * @param is_array  flag indicating whether it is invoked by a
 *                  <code>delete[]</code> call
 */
CROSS_EXPORT void free_pointer(void* usr_ptr, void* addr, bool is_array)
{
    if (usr_ptr == NULL)
        return;
    new_ptr_list_t* ptr = (new_ptr_list_t*)((char*)usr_ptr - ALIGNED_LIST_ITEM_SIZE);
    if (ptr->magic != DEBUG_NEW_MAGIC)
    {
        {
            LOGI("delete%s: invalid pointer %p (", is_array ? "[]" : "", usr_ptr);
            print_position(addr, 0);
            LOGI(")\n");
        }
        check_mem_corruption();
        _DEBUG_NEW_ERROR_ACTION;
    }
    if (is_array != ptr->is_array)
    {
        const char* msg;
        if (is_array)
            msg = "delete[] after new";
        else
            msg = "delete after new[]";
        LOGI("%s: pointer %p (size %lu)\n\tat ", msg, (char*)ptr + ALIGNED_LIST_ITEM_SIZE, (unsigned long)ptr->size);
        print_position(addr, 0);
        LOGI("\n\toriginally allocated at ");
        if (ptr->line != 0)
            print_position(ptr->file, ptr->line);
        else
            print_position(ptr->addr, ptr->line);
        LOGI("\n");
        _DEBUG_NEW_ERROR_ACTION;
    }
#if _DEBUG_NEW_TAILCHECK
    if (!check_tail(ptr))
    {
        check_mem_corruption();
        _DEBUG_NEW_ERROR_ACTION;
    }
#endif
    {
        fast_mutex_autolock lock(new_ptr_lock);
        total_mem_alloc -= ptr->size;
        ptr->magic = 0;
        ptr->prev->next = ptr->next;
        ptr->next->prev = ptr->prev;
    }
	CrossEngine::Free(ptr);
    return;
}

/**
 * Dump memory objects.
 */
CROSS_EXPORT int dump_memory_objects()
{
	int object_cnt = 0;
	size_t total_size = 0;
	
    fast_mutex_autolock lock_ptr(new_ptr_lock);
	for (new_ptr_list_t* ptr = new_ptr_list.next; ptr != &new_ptr_list; ptr = ptr->next)
    {
        const char* const usr_ptr = (char*)ptr + ALIGNED_LIST_ITEM_SIZE;
        if (ptr->magic != DEBUG_NEW_MAGIC)
        {
            LOGI("warning: heap data corrupt near %p\n", usr_ptr);
        }
#if _DEBUG_NEW_TAILCHECK
        if (!check_tail(ptr))
        {
            LOGI("warning: overwritten past end of object at %p\n", usr_ptr);
        }
#endif
        LOGI("object at %p (size %lu, ", usr_ptr, (unsigned long)ptr->size);
        if (ptr->line != 0)
            print_position(ptr->file, ptr->line);
        else
            print_position(ptr->addr, ptr->line);
        LOGI(")\n");
        
		object_cnt++;
		total_size += ptr->size;
    }

    LOGI("*** %d objects found, total size %d\n", object_cnt, total_size);
    return object_cnt;
}

/**
 * Checks for heap corruption.
 *
 * @return  zero if no problem is found; the number of found memory
 *          corruptions otherwise
 */
CROSS_EXPORT int check_mem_corruption()
{
    int corrupt_cnt = 0;
    fast_mutex_autolock lock_ptr(new_ptr_lock);
    LOGI("*** Checking for memory corruption: START\n");
    for (new_ptr_list_t* ptr = new_ptr_list.next; ptr != &new_ptr_list; ptr = ptr->next)
    {
        const char* const usr_ptr = (char*)ptr + ALIGNED_LIST_ITEM_SIZE;
        if (ptr->magic == DEBUG_NEW_MAGIC
#if _DEBUG_NEW_TAILCHECK
                && check_tail(ptr)
#endif
                )
            continue;
#if _DEBUG_NEW_TAILCHECK
        if (ptr->magic != DEBUG_NEW_MAGIC)
        {
#endif
            LOGI("Heap data corrupt near %p (size %lu, ", usr_ptr, (unsigned long)ptr->size);
#if _DEBUG_NEW_TAILCHECK
        }
        else
        {
            LOGI("Overwritten past end of object at %p (size %lu, ", usr_ptr, (unsigned long)ptr->size);
        }
#endif
        if (ptr->line != 0)
            print_position(ptr->file, ptr->line);
        else
            print_position(ptr->addr, ptr->line);
        LOGI(")\n");
        ++corrupt_cnt;
    }
    LOGI("*** Checking for memory corruption: %d FOUND\n", corrupt_cnt);
    return corrupt_cnt;
}
