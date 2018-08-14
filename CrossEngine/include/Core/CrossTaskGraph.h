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

	class CROSS_EXPORT CTask
	{
		friend class CTaskGraph;


	public:
		CTask(void);
		virtual ~CTask(void);


	public:
		virtual void TaskFunc(void *pParams) = 0;

	private:
		inline void SetTaskSignal(void);
		inline void SetTaskEventSignal(event_t *pEventSignal);

		inline void SetTaskParams(void *pParams);
		inline void* GetTaskParams(void) const;


	private:
		void *m_pTaskParams;
		event_t *m_pTaskEventSignal;

	private:
		CTask *pNext;
	};


	class CROSS_EXPORT CTaskGraph
	{
	public:
		CTaskGraph(int numThreads);
		virtual ~CTaskGraph(void);


	public:
		void Task(CTask *pTask, void *pParams, event_t *pEventSignal, event_t *pEventWait = NULL);
		void Dispatch(void);
		void Wait(void);

	private:
		static void* TaskThread(void *pParams);


	private:
		int m_numThreads;
		pthread_t *m_threads;

		event_t m_eventExit;
		event_t m_eventReady;
		event_t m_eventFinish;
		event_t m_eventDispatch;

	private:
		pthread_mutex_t m_mutexTaskList;
		eastl::map<event_t*, CTask*> m_pTaskListHeads;
		eastl::map<event_t*, event_t*> m_pTaskListDependence;
	};

}
