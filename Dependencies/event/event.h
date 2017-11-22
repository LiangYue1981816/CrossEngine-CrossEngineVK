#pragma once
#include "pthread.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	int count;
	pthread_cond_t cond;
	pthread_mutex_t mutex;
} event_t;

void event_init(event_t *event, int set);
void event_destroy(event_t *event);
void event_reset(event_t *event);
void event_signal(event_t *event);
void event_unsignal(event_t *event);
void event_wait(event_t *event);
int  event_wait_timeout(event_t *event, int msec);

#ifdef __cplusplus
}
#endif
