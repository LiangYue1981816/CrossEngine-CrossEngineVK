#pragma once
#include "event.h"
#include "pthread.h"


#ifdef __cplusplus
extern "C" {
#endif

#define THREAD_COUNT 4

typedef struct {
	void *param;
	void *pThreadCluster;
} THREAD_PARAM;

typedef struct {
	void *(*ThreadFunc)(void *);

	event_t eventExit;
	event_t eventReady;
	event_t eventFinish;
	event_t eventDispatch;

	pthread_t threads[THREAD_COUNT];
	THREAD_PARAM params[THREAD_COUNT];
} THREAD_CLUSTER;

void CreateThreads(THREAD_CLUSTER *pThreadCluster, void *(*ThreadFunc)(void *), void **params);
void DestroyThreads(THREAD_CLUSTER *pThreadCluster);
void DispatchThreads(THREAD_CLUSTER *pThreadCluster);
void WaitThreads(THREAD_CLUSTER *pThreadCluster);

#ifdef __cplusplus
}
#endif
