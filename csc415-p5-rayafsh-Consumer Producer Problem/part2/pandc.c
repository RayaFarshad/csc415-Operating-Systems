#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>

//Define a global buffer
int *buffer;
size_t array_size;
int *producerArray;
int *consumerArray;
// pthread_t *tid_p;
// pthread_t *tid_c;
int in, out;
int global_counter;
int producerIndex=0;
int consumerIndex =0;
int numOfBuffers, numProducerThreads, numConsumerThreads, numOfProducerItem, numOfConsumerItem, pTime, cTime;
int overConsume;
int temp;

// int *id_c, *id_p;
//define semaphores
sem_t empty;
sem_t full;
pthread_mutex_t mutex;
//functions to be implemented
void *producer(void *arg);
void *consumer(void *arg);
int enqueue_item(int item);
int dequeue_item();
void timestamp();
int match(int *producerArray, int *consumerArray);
struct ThreadArgs
{
    pthread_t tid;
    int id;
};
int main(int argc, char** argv) {
  //our program must accept 6 command arguments
  if (argc < 7){
    printf("Invalid number of arguments!\n");
    return 1;
  }
  //N is number of buffers to maintain
  numOfBuffers = atoi(argv[1]);
  //P is the number of producer threads
  numProducerThreads = atoi(argv[2]);
  //C is the number of consumer Threads
  numConsumerThreads = atoi(argv[3]);
  //X is number of items each producer thread will produce
  numOfProducerItem = atoi(argv[4]);
  overConsume = (numProducerThreads * numOfProducerItem)%numConsumerThreads;
  //num of items each consumer thread will produced
  numOfConsumerItem = ((numProducerThreads * numOfProducerItem)/numConsumerThreads);
  //Ptime is the how long insecond each consumer thread will sleep after consuming an item.
  pTime = atoi(argv[5]);
  //Ctime is how long in second each consumer thread will sleep after consuming an item.
  cTime = atoi(argv[6]);

  printf("Number of Buffers :   %d\n"
  "Number of Producer :   %d\nNumber of Consumers :   %d\n"
  "Number of items produced by each producer :   %d\n"
  "Number of items consumed by each consumer :   %d\n"
  "Number of overConsume :   %d\n"
  "Ptime :   %d\nCtime :   %d\n",
  numOfBuffers, numProducerThreads, numConsumerThreads, numOfProducerItem,numOfConsumerItem, overConsume, pTime, cTime);
  //Producer sleep time and consumer sleep time should be grater that 0
  if (pTime < 0){
    perror("The ptime can't be negative number\n");
    return 1;
  }
  if (cTime < 0){
    perror ("the ctime can't be a negative number\n");
  }
  //Initialize buffer
  array_size = numOfBuffers;
  buffer = malloc(sizeof(int)*array_size);
  //Initialize arrays to compare consumer with producer items
  producerArray = malloc(sizeof(int)*(numProducerThreads*numOfProducerItem));
  consumerArray = malloc(sizeof(int)*(numProducerThreads*numOfProducerItem));
  //Initialize semaphore
  sem_init(&full, 0, 0);
  sem_init(&empty, 0, array_size);
  pthread_mutex_init(&mutex, NULL);
  //run time
  timestamp();
  time_t begin = time(NULL);

  //initialize the threads
  pthread_t *tid_1 = (pthread_t*)malloc(sizeof(pthread_t) * numProducerThreads);
  pthread_t *tid_2 = (pthread_t*)malloc(sizeof(pthread_t) * numConsumerThreads);
  //Create Threads
  struct ThreadArgs pThread[numProducerThreads];
  for(int i = 0; i < numProducerThreads; i++){
    pThread[i].id = i;
    int ret_val = pthread_create(&pThread[i].tid, NULL, producer, (void *)&pThread[i]);
    if(ret_val < 0)
    {
        perror("Error creating thread..");
        return -2;
    }
  }
  struct ThreadArgs cThread[numConsumerThreads];
  for(int j = 0; j < numConsumerThreads; j++){
    cThread[j].id = j;
    int ret_val = pthread_create(&cThread[j].tid, NULL, consumer, (void *)&cThread[j]);
    if(ret_val < 0)
    {
        perror("Error creating thread..");
        return -2;
    }
    // if (overConsume != 0){
    //   cThread[numConsumerThreads-1].flag = 1;
    // }
  }
  for(int i = 0; i < numProducerThreads; i++)
  {
      int ret_val = pthread_join(pThread[i].tid, NULL);
      if(ret_val)
      {
          perror("Error joining thread: ");
          return -3;
      }
  }
  for(int j = 0; j < numConsumerThreads; j++)
  {
      int ret_val = pthread_join(cThread[j].tid, NULL);
      if(ret_val)
      {
          perror("Error joining thread: ");
          return -3;
      }
  }

  printf("Producer Array: ");
  for(int i = 0 ; i < numOfProducerItem * numProducerThreads; i++) {
      printf("%d, ", producerArray[i]);
  }
  printf("\nConsumer Array: ");
  for(int i = 0 ; i < numOfProducerItem * numProducerThreads; i++) {
      printf("%d, ", consumerArray[i]);
  }
  if(match(producerArray, consumerArray) == 1){
    printf("\nTwo arrays are match!\n");
  }
  time_t end = time(NULL);
  printf("\nTotal Runtime: %ld seconds\n", end - begin);
//destroy semaphores and heap memories
sem_destroy(&full);
sem_destroy(&empty);
pthread_mutex_destroy(&mutex);
free(producerArray);
free(consumerArray);
free(buffer);
return 0;
}
int match(int *producerArray, int *consumerArray){
  for(int i =0; i<numOfProducerItem*numProducerThreads;i++){
    if (producerArray[i] == consumerArray[i]){
      return 1;
    }

  }
  return 0;
}

void timestamp() {
    time_t ltime; /* calendar time */
    ltime = time(NULL); /* get current cal time */
    printf("\nTimestamp: %s\n", asctime(localtime(&ltime)));
}
/*
 * Function to remove item.
 * Item removed is returned
 */
 int dequeue_item()
 {
   int consume;
   consume = buffer[out];
   out = (out +1)%array_size;
 return consume;
 }
/*
 * Function to add item.
 * Item added is returned.
 * It is up to you to determine
 * how to use the ruturn value.
 * If you decide to not use it, then ignore
 * the return value, do not change the
 * return type to void.
 */
int enqueue_item(int item){
  buffer[in] = item;
  in = (in + 1)%array_size;
return item;
}
//produce items
void *producer(void *arg){

    int counter = 0, product;
    struct ThreadArgs* args = (struct ThreadArgs*)arg;
  //  printf("tid %d and Number of produced items %d\n",args->id, numOfProducerItem );
    while(counter < numOfProducerItem) {
        //lock
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        //critical section
        product = global_counter++;
        printf("%d was produced by producer -> %d\n", enqueue_item(product), args->id);
        producerArray[producerIndex++] = product;

        counter++;
        //sleep


        //unlock
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
        sleep(pTime);
        //printf("        %d producer %d\n", args->id, product);

    }
    pthread_exit(0);
}
//consume items
void *consumer(void *arg) {
    int counter = 0;int consume;
    struct ThreadArgs* args = (struct ThreadArgs*)arg;

  //  printf("  consumer thread id number %d -number of consumer item conumed %d\n", args->id, numOfConsumerItem);

  //  printf("tid %d and Number of consumed items %d\n",args->id, numOfConsumerItem );
    while(args->id == 0   && counter < numOfConsumerItem + overConsume ||
      (args->id !=0 && counter < numOfConsumerItem )
    ) {
        //printf(" %d - %d\n", args->id, numOfConsumerItem);
        //lock
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        consume = dequeue_item();
        printf("%d was consumed by consumer -> %d\n", consume, args->id);
        consumerArray[consumerIndex++] = consume;
        counter++;
        //sleep

        //unlock


        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
        sleep(cTime);
        //printf("        %d consumer %d\n", args->id, consume);



    }
    pthread_exit(0);
}
