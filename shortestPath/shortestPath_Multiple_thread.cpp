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
#include <unistd.h>
#include <fstream>

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

// thread create count
int actual_thread_count = 0;
int plan_thread_count = 0;

// time tracing
char time_tracing_option;

//
ofstream outfile;


/************************************************************************
 
 strcut:        worker
 
 Description:   calculating threads for each dist[][]
 
 *************************************************************************/
void *worker(void *param){
    // thread count
    pthread_mutex_lock(&writeLock);
    actual_thread_count++;
    pthread_mutex_unlock(&writeLock);
    
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
            actual_thread_count++;
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

/************************************************************************
 
 strcut:        shortestPath
 
 Description:   shortest Path main functions
 
 *************************************************************************/
void shortestPath (){
    
    const clock_t begin_time = clock();
    
    for (int k = 0; k < N_nodes; k++) {
        arg_s value[N_nodes];
        // create N pthreads
        pthread_t *thread = (pthread_t *)malloc(N_nodes * sizeof(pthread_t));
        
        for (int i = 0; i < N_nodes; i++) {
            // set up arguments to be passed
            value[i].n = N_nodes;
            value[i].k = k;
            value[i].i = i;
            pthread_create((thread + i), NULL, worker, &value[i]);
        }
   
        // join pthreads
        for (int i = 0 ; i < N_nodes; i++) {
            plan_thread_count++;
            pthread_join(*(thread + i), NULL);
        }
        
        if (time_tracing_option == 'Y') {
            printf("\n\nAlready running %d times!:\n" , k);
            printf("Created threads %d !\n" , actual_thread_count);
            printf("Totoal Running time now is %lf ! \n" , float( clock () - begin_time ) /  CLOCKS_PER_SEC);
            // 向文件写入用户输入的数据
            outfile << "\n\n Already running %d times!: " << k << endl;
            outfile << "Total create " << actual_thread_count <<  " threads(actual)"  << endl;
            outfile << "Totoal Running time now is " << float( clock () - begin_time ) /  CLOCKS_PER_SEC << endl;
        }
        
    }
    
    total_time_of_running = float( clock () - begin_time ) /  CLOCKS_PER_SEC;

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
    // check if not print output
    printf("Chose whether print out whole distance matrix or not, Y for yes, N for no(Captial Letter)\n");
    char option;
    scanf("\n%c" , &option);
    
    printf("Enable time tracing? Y for yes, N for no(Captial Letter)\n");
    scanf("\n%c" , &time_tracing_option);
    
    initializeGraph(N_nodes, M_edges);                      // initialize graph
    signEdges(M_edges);                                     // get edges' input

    if (option == 'Y') {
        printf("INPUT:\n");
        printGraph();
    }

    outfile.open("/Users/WillJia/Desktop/IOS Lecture/Projects/shortestPath/shortestPath/file1.txt");

    // calculate shortest path
    shortestPath();
    
    
    if (option == 'Y') {
        for (int i = 0; i < N_nodes; i++) {
            printf("*****");
        }
        cout << endl;
        printf("OUTPUT:\n");
        printGraph(); printDist();
    }

    // 以写模式打开文件
    
    
    // 向文件写入用户输入的数据
    outfile << "\nTotal Time = " << total_time_of_running << endl;
    outfile << "Total create " << actual_thread_count <<  " threads(actual)"  << endl;
    outfile << "Total create " << plan_thread_count <<  " threads(plan)" << endl;
    

    printf("\nTotal Time = %f\nTotal create %d threads(actual)\nTotal create %d threads(plan) " , total_time_of_running , actual_thread_count, plan_thread_count);
    
    // 关闭打开的文件
    outfile.close();
    
//    sem_destroy(&writeLock);
//    sem_destroy(&readCountLock);
    pthread_mutex_destroy(&readCountLock);
    pthread_mutex_destroy(&writeLock);
    
    return 0;
}
