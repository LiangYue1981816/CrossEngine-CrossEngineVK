#include "event.h"

#ifdef WIN32
#include "windows.h"
#endif


#ifdef WIN32
static int gettimeofday(struct timeval *tp, void *tzp)
{
	time_t clock;
	struct tm tm;
	SYSTEMTIME wtm;
	GetLocalTime(&wtm);
	tm.tm_year = wtm.wYear - 1900;
	tm.tm_mon = wtm.wMonth - 1;
	tm.tm_mday = wtm.wDay;
	tm.tm_hour = wtm.wHour;
	tm.tm_min = wtm.wMinute;
	tm.tm_sec = wtm.wSecond;
	tm.tm_isdst = -1;
	clock = mktime(&tm);
	tp->tv_sec = (long)clock;
	tp->tv_usec = wtm.wMilliseconds * 1000;
	return 0;
}
#endif


void event_init(event_t *event, int set)
{
	event->count = set ? 0 : 1;
	pthread_cond_init(&event->cond, NULL);
	pthread_mutex_init(&event->mutex, NULL);
}

void event_destroy(event_t *event)
{
	pthread_cond_destroy(&event->cond);
	pthread_mutex_destroy(&event->mutex);
}

void event_reset(event_t *event)
{
	pthread_mutex_lock(&event->mutex);
	{
		event->count = 1;
	}
	pthread_mutex_unlock(&event->mutex);
}

void event_signal(event_t *event)
{
	pthread_mutex_lock(&event->mutex);
	{
		if (event->count > 0) {
			event->count--;

			if (event->count == 0) {
				pthread_cond_broadcast(&event->cond);
			}
		}
	}
	pthread_mutex_unlock(&event->mutex);
}

void event_unsignal(event_t *event)
{
	pthread_mutex_lock(&event->mutex);
	{
		event->count++;
	}
	pthread_mutex_unlock(&event->mutex);
}

void event_wait(event_t *event)
{
	pthread_mutex_lock(&event->mutex);
	{
		while (event->count) {
			pthread_cond_wait(&event->cond, &event->mutex);
		}
	}
	pthread_mutex_unlock(&event->mutex);
}

int event_wait_timeout(event_t *event, int msec)
{
	int rcode = NO_ERROR;

	pthread_mutex_lock(&event->mutex);
	{
		if (event->count) {
			int nsec;
			struct timeval now;
			struct timespec abstime;

			gettimeofday(&now, NULL);
			nsec = now.tv_usec * 1000 + (msec % 1000) * 1000000;
			abstime.tv_nsec = nsec % 1000000000;
			abstime.tv_sec = now.tv_sec + nsec / 1000000000 + msec / 1000;

			rcode = pthread_cond_timedwait(&event->cond, &event->mutex, &abstime);
		}
	}
	pthread_mutex_unlock(&event->mutex);

	return rcode;
}
