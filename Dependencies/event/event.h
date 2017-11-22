#pragma once
#include "pthread.h"


#ifdef __cplusplus
extern "C" {
#endif

struct event_t {
	int count;
	pthread_cond_t cond;
	pthread_mutex_t mutex;
};

extern void event_init(event_t *event, int set);
extern void event_destroy(event_t *event);
extern void event_reset(event_t *event, int init);
extern void event_signal(event_t *event);
extern int  event_is_signaled(event_t *event);
extern void event_wait(event_t *event);
extern int  event_wait_timeout(event_t *event, int msec);

#ifdef __cplusplus
}
#endif
