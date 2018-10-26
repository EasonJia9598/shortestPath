//
//  main.cpp
//  shortestPath_Singal_thread
//
//  Created by WillJia on 2018-10-25.
//  Copyright © 2018 WillJia. All rights reserved.
//

#include <iostream>
#include "graph.h"
#include <semaphore.h>

// struct for matrix info
struct arg_s{
    int n;
    int i;
    int k;
};
//total_time_of_running total_time_of_running
double total_time_of_running = 0;

// semaphore lock
//sem_t readCountLock;
//sem_t writeLock;
// mutex lock
pthread_mutex_t readCountLock;
pthread_mutex_t writeLock;





void *worker(void *param){
    // get *param as struct and convert to additionMatrix
    arg_s *arg = (arg_s*)param;
    int i = arg->i;
    int k = arg->k;
    for (int j = 0; j < arg->n; j++) {
        //acquire read lock
//        sem_wait(&readCountLock);
        pthread_mutex_lock(&readCountLock);
        
        if (graph[i][k] != 0 &&  graph[k][j] != 0 && dist[i][k] + dist[k][j] < dist[i][j]) {
            //release read lock
//            sem_post(&readCountLock);
            pthread_mutex_unlock(&readCountLock);

            //acquire write lock
//            sem_wait(&writeLock);
            pthread_mutex_lock(&writeLock);

            dist[i][j] = dist[i][k] + dist[k][j];
            
            //release write lock
//            sem_post(&writeLock);
            pthread_mutex_unlock(&writeLock);

        }else{
            //release read lock
//            sem_post(&readCountLock);
            pthread_mutex_unlock(&readCountLock);

        }
    }
    
    pthread_exit(NULL);
}


void shortestPath (){
    
    for (int k = 0; k < N_nodes; k++) {
        
        // create N pthreads
        pthread_t *thread = (pthread_t *)malloc(N_nodes * sizeof(pthread_t));
        arg_s value[N_nodes];

        for (int i = 0; i < N_nodes; i++) {
            // set up arguments to be passed
            value[i].n = N_nodes;
            value[i].k = k;
            value[i].i = i;
            pthread_create((thread + i), NULL, worker, &value[i]);
        }
        
        const clock_t begin_time = clock();
        // join pthreads
        for (int i = 0 ; i < N_nodes; i++) {
            pthread_join(*(thread + i), NULL);
        }
        
        total_time_of_running += float( clock () - begin_time ) /  CLOCKS_PER_SEC;

    }
    
}




int main(int argc, const char * argv[]) {
    
    // initialize lock
//    sem_init(&readCountLock, 0, 1);
//    sem_init(&writeLock, 0, 1);
    pthread_mutex_init(&readCountLock,NULL);
    pthread_mutex_init(&writeLock,NULL);
    
    // enter N and M
    printf("Enter N and M (N for n nodes, M for m edges)\n");
    scanf("%d %d", &N_nodes, &M_edges);
    initializeGraph(N_nodes, M_edges);                      // initialize graph
    signEdges(M_edges);                                     // get edges' input
    printf("INPUT:\n");
    printGraph();                                           // print out input
    
    // calculate shortest path
    shortestPath();
    
    
    // print out result
    for (int i = 0; i < N_nodes; i++) {
        printf("*****");
    }
    cout << endl;
    printf("OUTPUT:\n");
    printGraph(); printDist();
    printf("Total Time = %f\n" , total_time_of_running);
    
//    sem_destroy(&writeLock);
//    sem_destroy(&readCountLock);
    pthread_mutex_destroy(&readCountLock);
    pthread_mutex_destroy(&writeLock);
    
    return 0;
}
