#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>

#define THREAD_NUM 6

sem_t empty;
sem_t full;
pthread_mutex_t storageEntry;
int n = 10;
int storage[10];
int producerPosition = 0;
int consumerPosition = 0;

void* producer(void* args)
{
	int *nr = (int *)args;
	while (1)
	{
		int product = rand() % 100;
		sem_wait(&empty);
		pthread_mutex_lock(&storageEntry);
		storage[producerPosition] = product;
		producerPosition = (producerPosition + 1) % n;
		printf("Produced element %d by producer %d\n", product, *nr);
		pthread_mutex_unlock(&storageEntry);
		sem_post(&full);
	}
}

void* consumer(void* args)
{
	int *nr = (int *)args;
	while(1)
	{
		int product;
		sem_wait(&full);
		pthread_mutex_lock(&storageEntry);
		product = storage[consumerPosition];
		consumerPosition = (consumerPosition + 1) % n;
		printf("consumed element %d by consumer %d\n", product, *nr);
		pthread_mutex_unlock(&storageEntry);
		sem_post(&empty);
	}
}

int main(int argc, char* argv[])
{
	srand(time(NULL));
	pthread_t threads[THREAD_NUM];
	pthread_mutex_init(&storageEntry, NULL);
	sem_init(&empty, 0, n);
	sem_init(&full, 0, 0);
	int i;
	int tmp[THREAD_NUM/2];
	for(i = 0; i < THREAD_NUM/2; i++)
	{
		tmp[i] = i+1;
	}
	for(i = 0; i < THREAD_NUM; i++)
	{
		if(i % 2 == 0)
		{
			pthread_create(&threads[i], NULL, &producer, &tmp[i/2]);
		}
		else
		{
			pthread_create(&threads[i], NULL, &consumer, &tmp[i/2]);
		}
	}
	for(i = 0; i < THREAD_NUM; i++)
	{
		pthread_join(threads[i], NULL);
	}
	
	sem_destroy(&empty);
	sem_destroy(&full);
	pthread_mutex_destroy(&storageEntry);
	
	return 0;
}










