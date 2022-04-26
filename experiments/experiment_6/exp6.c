#include<stdio.h> 
#include<unistd.h> 
#include<stdlib.h> 
#include<pthread.h>

int shared = 1; 
pthread_mutex_t lock;

void* ThreadFunction1(void *num) 
{ 
    int x;
    printf("Acquiring lock (thread 1)...\n"); 
    pthread_mutex_lock(&lock);

    printf("Lock acquired by thread 1...\n"); 
    printf("Thread1 read : %d\n", shared);

    shared++;
    printf("Thread1 updates the variable to : %d\n", shared); 
    pthread_mutex_unlock(&lock);
    printf("Relinquishing lock (thread 1)...\n");
    return NULL;       
}

void* ThreadFunction2(void *num) 
{ 
    int x;
    printf("Acquiring lock (thread 2)...\n");
    pthread_mutex_lock(&lock); printf("Lock acquired by thread 2...\n"); 
    printf("Thread2 read : %d\n", shared); 
    shared--;
    printf("Thread 2 updates the variable to : %d\n", shared); 
    pthread_mutex_unlock(&lock);
    printf("Relinquishing lock (thread 2)...\n");
    return NULL;
}

int main(int argc, char *argv[]) 
{ 
    pthread_mutex_init(&lock, NULL); 
    pthread_t ptid1, ptid2;
    pthread_create(&ptid1, NULL, &ThreadFunction1, NULL);
    pthread_create(&ptid2, NULL, &ThreadFunction2, NULL);
    pthread_join(ptid1, NULL); pthread_join(ptid2, NULL);
    printf("The final updated value of shared variable is: %d\n", shared); pthread_exit(NULL);
    return (0);
}
