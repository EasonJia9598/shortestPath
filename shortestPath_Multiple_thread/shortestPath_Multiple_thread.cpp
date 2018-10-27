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
sem_t readCountLock;
sem_t writeLock;

// thread create count
int actual_thread_count = 0;
int plan_thread_count = 0;

// time tracing
char time_tracing_option;

//
ofstream outfile;

//
int write_into_file = 0;


/************************************************************************
 
 strcut:        worker
 
 Description:   calculating threads for each dist[][]
 
 *************************************************************************/
void *worker(void *param){
    
    // get *param as struct and convert to additionMatrix
    arg_s *arg = (arg_s*)param;
    int i = arg->i;
    int k = arg->k;
    for (int j = 0; j < arg->n; j++) {
        //acquire read lock
        sem_wait(&readCountLock);
        if (graph[i][k] != 0 &&  graph[k][j] != 0 && dist[i][k] + dist[k][j] < dist[i][j]) {
            //release read lock
            sem_post(&readCountLock);
            
            //acquire write lock
            sem_wait(&writeLock);
            dist[i][j] = dist[i][k] + dist[k][j];
            
            //release write lock
            sem_post(&writeLock);
            
        }else{
            //release read lock
            sem_post(&readCountLock);
            
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
    // create N pthreads
    pthread_t *thread = (pthread_t *)malloc(N_nodes * sizeof(pthread_t));
    
    for (int k = 0; k < N_nodes; k++) {
        // generating struct
        arg_s value[N_nodes];

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
        
        // print time tracing info
        if (time_tracing_option == 'Y') {
            printf("\n\nAlready running %d times!:\n" , k);
            printf("Totoal Running time now is %lf ! \n" , float( clock () - begin_time ) /  (CLOCKS_PER_SEC * 10));
            
            // write data into file
            if (write_into_file) {
                outfile << "\n\n Already running " << k << " times!: " << endl;
                outfile << "Totoal Running time now is " << float( clock () - begin_time ) /  (CLOCKS_PER_SEC * 10) << endl;
            }
        }
        
    }
    
    total_time_of_running = float( clock () - begin_time ) /  (CLOCKS_PER_SEC * 10);

}




int main(int argc, const char * argv[]) {
    
    // initialize lock
    sem_init(&readCountLock, 0, 1);
    sem_init(&writeLock, 0, 1);
    
    // enter N and M
    printf("Enter N and M (N for n nodes, M for m edges)\n");
    scanf("%d %d", &N_nodes, &M_edges);
    // check if not print output
    printf("Chose whether print out whole distance matrix or not, Y for yes, N for no(Captial Letter)\n");
    char option;
    scanf("\n%c" , &option);
    
    printf("Enable time tracing? Y for yes, N for no(Captial Letter)\n");
    scanf("\n%c" , &time_tracing_option);
    
    char file_writing;
    printf("Enable write report into txt file? Y for yes, N for no(Captial Letter)\n");
    scanf("\n%c" , &file_writing);
    // check value
    write_into_file = (file_writing == 'Y') ? 1 : 0;
    
    initializeGraph(N_nodes, M_edges);                      // initialize graph
    signEdges(M_edges);                                     // get edges' input

    if (option == 'Y') {
        printf("INPUT:\n");
        printGraph();
    }

    if (write_into_file) {
        outfile.open("/Users/WillJia/Desktop/IOS Lecture/Projects/shortestPath/shortestPath_Singal_thread/file.txt");
    }

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

    // open file for writing reslut if user don't want to write every thing into file
    if (!write_into_file) {
        outfile.open("/Users/WillJia/Desktop/IOS Lecture/Projects/shortestPath/shortestPath_Singal_thread/file.txt");
    }
    
    // write into file
    outfile << "\nTotal Time = " << total_time_of_running << endl;
    

    printf("\nTotal Time = %f\n " , total_time_of_running );
    
    // close file
    outfile.close();
    
    sem_destroy(&writeLock);
    sem_destroy(&readCountLock);
    
    return 0;
}
