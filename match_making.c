#include <pthread.h>

#define NOTHREAD -1

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // mutex
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER; // condition variable
static pthread_t next_tid1 = NOTHREAD; // first thread of the pair
static pthread_t next_tid2 = NOTHREAD; // second thread of the pair
static unsigned int refcount = 0; // 1 if one of next_tid1/next_tid2 is occupied and not returned yet, 2 if both are occupied and not returned yet

void refcount_decrement(){
    refcount--;
    if (refcount == 0){
        next_tid1 = NOTHREAD;
        next_tid2 = NOTHREAD;
    }
}

pthread_t make_match(){
    pthread_t tid = pthread_self();
    pthread_t pair_tid = NOTHREAD;
    
    pthread_mutex_lock(&mutex);

    while(!(next_tid1 == tid && next_tid2 != NOTHREAD) && !(next_tid2 == tid && next_tid1 != NOTHREAD)){
        if (next_tid1 == NOTHREAD && next_tid2 != tid){ // fill the first spot if you are not the second
            next_tid1 = tid;
            refcount++;
            pthread_cond_broadcast(&cond);
        }else if (next_tid2 == NOTHREAD && next_tid1 != tid){ // fill the second spot if you are not the first
            next_tid2 = tid;
            refcount++;
            pthread_cond_broadcast(&cond);
        }else{ // wait for your pair
            pthread_cond_wait(&cond, &mutex);
        }
    }

    pair_tid = next_tid1 == tid ? next_tid2 : next_tid1;
    refcount_decrement();

    pthread_cond_broadcast(&cond);

    pthread_mutex_unlock(&mutex);

    return pair_tid;
};