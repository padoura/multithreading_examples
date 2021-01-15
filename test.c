#include <pthread.h>
#include "match_making.h"
#include <assert.h>
#include <stdio.h>

#define NUM_OF_THREADS 10

void *make_match_wrapper(void *vargp){
    pthread_t *retVal = (pthread_t *)vargp;
    *retVal = make_match();
    // pthread_t pair = make_match();
    // fprintf(stderr, "tid == %ld paired with tid == %ld \n", pthread_self(), *retVal);
    return NULL;
}

int retVal_is_valid(pthread_t *thread_id, pthread_t *retVal, int index){
    for (int i=0;i<NUM_OF_THREADS;i++){
        // fprintf(stderr, "tid == %ld paired with tid == %ld \n", thread_id[i], retVal[index]);
        if (thread_id[i] == retVal[index]){
            // fprintf(stderr, "i == %d paired with index == %d \n", i, index);
            if (i != index)
                return 1;
            else
                return 0;
        }
    }
    return 0;
}

void test_match_making(){
    pthread_t thread_id[NUM_OF_THREADS];
    pthread_t retVal[NUM_OF_THREADS];

    for (int i=0; i<NUM_OF_THREADS; i++){
        pthread_create(&thread_id[i], NULL, make_match_wrapper, &retVal[i]);
        // pthread_create(&thread_id[i], NULL, &make_match_wrapper, NULL);
    }

    for (int i=0; i<NUM_OF_THREADS; i++){
        pthread_join(thread_id[i], NULL);
    }
    
    for (int i=0; i<NUM_OF_THREADS; i++){
        assert(retVal_is_valid(thread_id, retVal, i));
    }
}

int main(int argc, char *argv[]){
    test_match_making();
}