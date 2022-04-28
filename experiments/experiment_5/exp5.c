#include<stdio.h> 
#include<unistd.h> 
#include<stdlib.h> 
#include<signal.h> 
#include<pthread.h>

int data[200];
void * ThreadFunction(void *num) 
{ 
    data[1] = 0;
    data[2] = 1;
    int n = *((int *)num); 
    if (n > 2) 
    {
        for (int i = 3; i <= n; i++) 
        {
            data[i] = data[i - 1] + data[i - 2];
        } 
    }
    
    return NULL;
}   

int main(int argc, char *argv[]) 
{
    if (argc != 2) 
    {
        printf("ERROR!! INCORRECT PARAMETERS \n");
    }

    int n = atoi(argv[1]);
    pthread_t ptid;
    printf("Creating fibonacci series till %d th element! \n", n);
    pthread_create(&ptid, NULL, &ThreadFunction, &n);
    pthread_join(ptid, NULL);
    printf("The series is :\n");
    for (int i = 1; i <= n; i++) 
    { 
        printf("%d ", data[i]);
    }
    printf("\n"); 
    pthread_exit(NULL); 
    return (0);
}