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

	CGLES3DescriptorSetManager::CGLES3DescriptorSetManager(CGLES3Device *pDevice)
		: m_pDevice(pDevice)
	{
		pthread_mutex_init(&m_mutex, NULL);
	}

	CGLES3DescriptorSetManager::~CGLES3DescriptorSetManager(void)
	{
		pthread_mutex_destroy(&m_mutex);
	}

	int CGLES3DescriptorSetManager::Create(void)
	{
		return NO_ERROR;
	}

	void CGLES3DescriptorSetManager::Destroy(void)
	{
		for (auto &itDescriptorSet : m_pDescriptorSets) {
			SAFE_DELETE(itDescriptorSet.second);
		}

		m_pDescriptorSets.clear();
	}

	CGfxDescriptorSetPtr CGLES3DescriptorSetManager::AllocDescriptorSet(const CGLES3DescriptorSetLayout *pSetLayout)
	{
		CGLES3DescriptorSet *pDescriptorSet = SAFE_NEW CGLES3DescriptorSet(m_pDevice, this, const_cast<CGLES3DescriptorSetLayout *>(pSetLayout));
		{
			mutex_autolock mutex(m_mutex);
			m_pDescriptorSets[pDescriptorSet] = pDescriptorSet;
		}
		return CGfxDescriptorSetPtr(pDescriptorSet);
	}

	void CGLES3DescriptorSetManager::FreeDescriptorSet(CGLES3DescriptorSet *pDescriptorSet)
	{
		if (pDescriptorSet) {
			mutex_autolock mutex(m_mutex);
			m_pDescriptorSets.erase(pDescriptorSet);
			SAFE_DELETE(pDescriptorSet);
		}
	}

	void CGLES3DescriptorSetManager::DumpLog(const char *szTitle) const
	{
		LOGI("%s\n", szTitle);
		LOGI("*** %d objects found\n", m_pDescriptorSets.size());
		LOGI("\n");
	}

}
