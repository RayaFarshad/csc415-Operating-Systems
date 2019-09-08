#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <ctype.h>

/**
 * THESE DEFINE VALUES CANNOT BE CHANGED.
 * DOING SO WILL CAUSE POINTS TO BE DEDUCTED
 * FROM YOUR GRADE (15 points)
 */
 /** BEGIN VALUES THAT CANNOT BE CHANGED */
#define MAX_THREADS 16
#define MAX_ITERATIONS 40
/** END VALUES THAT CANNOT BE CHANGED */


/**
 * use this struct as a parameter for the function
 * nanosleep.
 * For exmaple : nanosleep(&ts, NULL);
 */
struct timespec ts = {0, 123456};
struct ThreadArgs
{
    pthread_t tid;
    int id;
};
static int global = 0;
// this is the thread function
void*
adder_threads(void * arg)
{
    struct ThreadArgs* args = (struct ThreadArgs*)arg;
    int i;
    for(i =0; i <= MAX_ITERATIONS;i++)
    {
        int temp;
        temp = global;
        temp = temp + 3;
        nanosleep(&ts, NULL);
        global = temp;
        printf("Current Value written to Global Variables by ADDER thread id: tid %d is val %d\n", args->id, temp);
    }

    return NULL;
}

void*
subtracter_threads(void * arg)
{
    struct ThreadArgs* args = (struct ThreadArgs*)arg;
    int i;
    for(i =0; i <= MAX_ITERATIONS;i++)
    {


        int temp;
        temp = global;
        temp = temp - 3;
        global = temp;
        printf("Current Value written to Global Variables by SUBTRACTER thread id: tid %d is val %d\n", args->id, temp);


    }

    return NULL;
}
int
main(int argc, char const *argv[])
{
    struct ThreadArgs thread_info[MAX_THREADS];
    int i, ret_val;
    for(i=0; i < MAX_THREADS;i++)
    {
      thread_info[i].id = i+1;
      if(i % 2 == 0){
        ret_val = pthread_create(&thread_info[i].tid, NULL, adder_threads, (void *)&thread_info[i]);

        if(ret_val < 0)
        {
            perror("Error creating thread..");
            return -2;
        }
    }else{
      ret_val = pthread_create(&thread_info[i].tid, NULL, subtracter_threads, (void *)&thread_info[i]);
      if(ret_val < 0)
      {
          perror("Error creating thread..");
          return -2;
      }
    }

   }

    for(i = 0; i < MAX_THREADS; i++)
    {
        ret_val = pthread_join(thread_info[i].tid, NULL);
        if(ret_val)
        {
            perror("Error joining thread: ");
            return -3;
        }
    }
    printf("Main Has Joined All Threads %d \n", global);
    return 0;
}
