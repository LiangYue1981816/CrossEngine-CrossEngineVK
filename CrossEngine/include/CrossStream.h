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

#pragma once
#include "CrossEngine.h"


namespace CrossEngine {

	class CROSS_EXPORT CStream
	{
	public:
		typedef enum {
			SEEK_MODE_CUR = SEEK_CUR,
			SEEK_MODE_END = SEEK_END,
			SEEK_MODE_SET = SEEK_SET
		} SEEK_MODE;


	public:
		CStream(void);
		virtual ~CStream(void);


	public:
		BOOL IsValid(void) const;

		void Init(void);
		void Free(void);
		void FreeData(void);

		BOOL Alloc(size_t size);
		BOOL SetStream(BYTE *pAddress, size_t size);

	public:
		BOOL CopyFrom(const CStream *pStream);
		BOOL LoadFromFile(const char *szFileName);
		BOOL LoadFromPack(const char *szPackName, const char *szFileName);
		BOOL LoadFromPack(ZZIP_DIR *pPack, const char *szFileName);

		BOOL Reload(void);

	public:
		BOOL SetName(const char *szName);
		const char* GetName(void) const;
		DWORD GetHashName(void) const;

		BOOL SetFileName(const char *szFileName);
		const char* GetFileName(void) const;

		BOOL SetPackName(const char *szPackName);
		const char* GetPackName(void) const;

		BOOL SetPack(ZZIP_DIR *pPack);
		ZZIP_DIR* GetPack(void) const;

	public:
		size_t GetFullSize(void) const;
		size_t GetFreeSize(void) const;
		size_t GetCurrentPosition(void) const;

		void* GetAddress(void) const;
		void* GetCurrentAddress(void) const;

	public:
		size_t Read(void *pBuffer, size_t size, size_t count);
		size_t ReadString(char *szString, size_t size = UINT_MAX);

		BYTE ReadBYTE(void);
		WORD ReadWORD(void);
		DWORD ReadDWORD(void);
		float ReadFLOAT(void);

		BOOL Seek(size_t offset, SEEK_MODE mode);
		BOOL IsEOF(void) const;


	protected:
		DWORD m_dwName;
		char m_szName[_MAX_STRING];

		ZZIP_DIR *m_pPack;
		char m_szPackName[_MAX_STRING];
		char m_szFileName[_MAX_STRING];

	protected:
		BOOL m_bAlloced;
		BYTE *m_pAddress;

		size_t m_size;
		size_t m_position;
	};

}
