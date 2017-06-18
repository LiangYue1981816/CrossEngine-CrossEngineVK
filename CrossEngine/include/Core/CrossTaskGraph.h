#pragma once
#include "stdio.h"
#include "event.h"
#include "pthread.h"
#include <map>

#ifdef PLATFORM_WINDOWS
#include "windows.h"
#endif


class CTask
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


class CTaskGraph
{
public:
	CTaskGraph(int numThreads);
	virtual ~CTaskGraph(void);


public:
	void Task(CTask *pTask, void *pParams, event_t *pEventSignal, event_t *pEventWait = NULL);
	void Dispatch(void);
	void Wait(int msec);

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
	std::map<event_t*, CTask*> m_pTaskListHeads;
};
