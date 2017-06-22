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

	CTask::CTask(void)
		: m_pTaskParams(NULL)
		, m_pTaskEventSignal(NULL)

		, pNext(NULL)
	{

	}

	CTask::~CTask(void)
	{

	}

	void CTask::SetTaskSignal(void)
	{
		if (m_pTaskEventSignal) {
			event_signal(m_pTaskEventSignal);
		}
	}

	void CTask::SetTaskEventSignal(event_t *pEventSignal)
	{
		if (m_pTaskEventSignal = pEventSignal) {
			event_reset(m_pTaskEventSignal, 0);
		}
	}

	void CTask::SetTaskParams(void *pParams)
	{
		m_pTaskParams = pParams;
	}

	void* CTask::GetTaskParams(void) const
	{
		return m_pTaskParams;
	}



	CTaskGraph::CTaskGraph(int numThreads)
		: m_threads(NULL)
		, m_numThreads(numThreads)
	{
		event_init(&m_eventExit, 1);
		event_init(&m_eventReady, 0);
		event_init(&m_eventFinish, 0);
		event_init(&m_eventDispatch, 1);
		pthread_mutex_init(&m_mutexTaskList, NULL);

		m_threads = (pthread_t *)malloc(sizeof(pthread_t) * m_numThreads);
		for (int index = 0; index < m_numThreads; index++) {
			pthread_create(&m_threads[index], NULL, TaskThread, this);
		}
	}

	CTaskGraph::~CTaskGraph(void)
	{
		event_signal(&m_eventExit);
		event_signal(&m_eventDispatch);

		for (int index = 0; index < m_numThreads; index++) {
			pthread_join(m_threads[index], NULL);
		}
		free(m_threads);

		event_destroy(&m_eventExit);
		event_destroy(&m_eventReady);
		event_destroy(&m_eventFinish);
		event_destroy(&m_eventDispatch);
		pthread_mutex_destroy(&m_mutexTaskList);
	}

	int CTaskGraph::GetThreadCount(void) const
	{
		return m_numThreads;
	}

	void CTaskGraph::Task(CTask *pTask, void *pParams, event_t *pEventSignal, event_t *pEventWait)
	{
		pTask->SetTaskParams(pParams);
		pTask->SetTaskEventSignal(pEventSignal);

		pthread_mutex_lock(&m_mutexTaskList);
		{
			pTask->pNext = m_pTaskListHeads[pEventWait];
			m_pTaskListHeads[pEventWait] = pTask;
		}
		pthread_mutex_unlock(&m_mutexTaskList);
	}

	void CTaskGraph::Dispatch(void)
	{
		for (int index = 0; index < m_numThreads; index++) {
			event_reset(&m_eventReady, 0);
			event_reset(&m_eventFinish, 0);
		}

		event_signal(&m_eventDispatch);
	}

	void CTaskGraph::Wait(int msec)
	{
		event_wait_timeout(&m_eventFinish, msec);
	}

	void* CTaskGraph::TaskThread(void *pParams)
	{
		BOOL bFinish = TRUE;
		CTask *pTask = NULL;
		CTaskGraph *pTaskGraph = (CTaskGraph *)pParams;

		while (TRUE) {
			event_wait(&pTaskGraph->m_eventDispatch);
			{
				// Check if the thread needs to exit
				if (event_wait_timeout(&pTaskGraph->m_eventExit, 0) == NO_ERROR) {
					break;
				}

				// Waiting for all threads to wake up
				event_signal(&pTaskGraph->m_eventReady);
				event_wait(&pTaskGraph->m_eventReady);

				// Run tasks
				while (TRUE) {
					pTask = NULL;
					bFinish = TRUE;

					pthread_mutex_lock(&pTaskGraph->m_mutexTaskList);
					{
						for (auto &itTask : pTaskGraph->m_pTaskListHeads) {
							if (itTask.first == NULL || event_wait_timeout(itTask.first, 0) == NO_ERROR) {
								pTask = itTask.second;

								if (itTask.second->pNext) {
									itTask.second = itTask.second->pNext;
								}
								else {
									pTaskGraph->m_pTaskListHeads.erase(itTask.first);
								}

								break;
							}
						}

						bFinish = pTaskGraph->m_pTaskListHeads.empty() ? TRUE : FALSE;
					}
					pthread_mutex_unlock(&pTaskGraph->m_mutexTaskList);

					if (pTask) {
						pTask->TaskFunc(pTask->GetTaskParams());
						pTask->SetTaskSignal();
					}

					if (bFinish) {
						break;
					}
				}
			}
			event_reset(&pTaskGraph->m_eventDispatch, 1);
			event_signal(&pTaskGraph->m_eventFinish);
		}

		return NULL;
	}

}
