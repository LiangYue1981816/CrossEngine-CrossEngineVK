#include "thread.h"

#ifdef WIN32
#include "windows.h"
#endif


static void* ThreadWorker(void *param)
{
	THREAD_PARAM *pThreadParam = (THREAD_PARAM *)param;
	THREAD_CLUSTER *pThreadCluster = (THREAD_CLUSTER *)pThreadParam->pThreadCluster;

	while (TRUE) {
		event_wait(&pThreadCluster->eventDispatch);
		{
			if (event_wait_timeout(&pThreadCluster->eventExit, 0) == NO_ERROR) {
				break;
			}

			event_signal(&pThreadCluster->eventReady);
			event_wait(&pThreadCluster->eventReady);

			(*pThreadCluster->ThreadFunc)(pThreadParam->param);
		}
		event_reset(&pThreadCluster->eventDispatch);
		event_signal(&pThreadCluster->eventFinish);
	}

	return NULL;
}

void CreateThreads(THREAD_CLUSTER *pThreadCluster, void *(*ThreadFunc)(void *), void **params)
{
	pThreadCluster->ThreadFunc = ThreadFunc;

	event_init(&pThreadCluster->eventExit, 0);
	event_init(&pThreadCluster->eventReady, 1);
	event_init(&pThreadCluster->eventFinish, 1);
	event_init(&pThreadCluster->eventDispatch, 0);

	for (int index = 0; index < THREAD_COUNT; index++) {
		pThreadCluster->params[index].param = params[index];
		pThreadCluster->params[index].pThreadCluster = pThreadCluster;
		pthread_create(&pThreadCluster->threads[index], NULL, ThreadWorker, &pThreadCluster->params[index]);
	}
}

void DestroyThreads(THREAD_CLUSTER *pThreadCluster)
{
	event_signal(&pThreadCluster->eventExit);
	event_signal(&pThreadCluster->eventDispatch);

	for (int index = 0; index < THREAD_COUNT; index++) {
		pthread_join(pThreadCluster->threads[index], NULL);
	}

	event_destroy(&pThreadCluster->eventExit);
	event_destroy(&pThreadCluster->eventReady);
	event_destroy(&pThreadCluster->eventFinish);
	event_destroy(&pThreadCluster->eventDispatch);
}

void DispatchThreads(THREAD_CLUSTER *pThreadCluster)
{
	for (int index = 0; index < THREAD_COUNT; index++) {
		event_unsignal(&pThreadCluster->eventReady);
		event_unsignal(&pThreadCluster->eventFinish);
	}

	event_signal(&pThreadCluster->eventDispatch);
}

void WaitThreads(THREAD_CLUSTER *pThreadCluster)
{
	event_wait(&pThreadCluster->eventFinish);
}
