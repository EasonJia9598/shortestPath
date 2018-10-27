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
//total_time_of_running
double total_time_of_running = 0;
//total time of calculating during threads join
double total_time_of_calculating = 0;
//total time of creating threads during pthread create
double total_time_of_creating_threads = 0;
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

//
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

        if (graph[i][k] != 0 &&  graph[k][j] != 0 && dist[i][k] + dist[k][j] < dist[i][j]) {
            //acquire write lock
            sem_wait(&writeLock);
            dist[i][j] = dist[i][k] + dist[k][j];
            //release write lock
            sem_post(&writeLock);
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
        // create struct
        arg_s value[N_nodes];
        // time of beginning of iteration
        const clock_t begin_time_iteration = clock();
        // create threads
        for (int i = 0; i < N_nodes; i++) {
            // set up arguments to be passed
            value[i].n = N_nodes;
            value[i].k = k;
            value[i].i = i;
            pthread_create((thread + i), NULL, worker, &value[i]);
        }
        // totoal time of creating threads
        total_time_of_creating_threads += float( clock () - begin_time_iteration ) /  (CLOCKS_PER_SEC * 10);
        // start computing time in join()
        const clock_t begin_time_joining_iteration = clock();
        
        // join pthreads
        for (int i = 0 ; i < N_nodes; i++) {
            plan_thread_count++;
            pthread_join(*(thread + i), NULL);
        }
        // compute all time value
        total_time_of_calculating += float( clock () - begin_time_joining_iteration ) /  (CLOCKS_PER_SEC * 10);
        total_time_of_running += float( clock () - begin_time_iteration ) /  (CLOCKS_PER_SEC * 10);
        
        // print if user chose to print time tracing
        if (time_tracing_option == 'Y') {
            printf("\n\nAlready running %d times!:\n" , k + 1);
            printf("Created threads %d !\n" , actual_thread_count);
            printf("Totoal Running time now is %lf ! \n" , total_time_of_running);
            printf("Totoal calculating time now is %lf ! \n" , total_time_of_calculating);
            printf("Totoal creating threads time now is %lf ! \n" , total_time_of_creating_threads);
            // print if user chose to write every tracing into file
            if (write_into_file) {
                outfile << "\n\n Already running " << k + 1 << " times!: " << endl;
                outfile << "Total create " << actual_thread_count <<  " threads(actual)"  << endl;
                outfile << "Totoal Running time now is " << total_time_of_running << endl;
                outfile << "Totoal calculating time now is " << total_time_of_calculating << endl;
                
            }
            
        }
        
    }
    
}




int main(int argc, const char * argv[]) {
    
    // initialize sem lock
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
    write_into_file = (file_writing == 'Y') ? 1 : 0;
    
    // get data from user
    initializeGraph(N_nodes, M_edges);                      // initialize graph
    signEdges(M_edges);                                     // get edges' input
    
    // print whole matrix let user choose
    if (option == 'Y') {
        printf("INPUT:\n");
        printGraph();
    }


    // open file if user want to write everything into file
    // because if this is true, the total will become larger.
    if (write_into_file) {
        outfile.open("/Users/WillJia/Desktop/IOS Lecture/Projects/shortestPath/shortestPath_Multiple_thread_without_readLock/file.txt");
    }
    
    // calculate shortest path in threads
    shortestPath();
    
    // still matrix print out
    if (option == 'Y') {
        for (int i = 0; i < N_nodes; i++) {
            printf("*****");
        }
        cout << endl;
        printf("OUTPUT:\n");
        printGraph(); printDist();
    }
    
    // if user don't want to write every tracing into file
    // still open file after calculating and write the final result into file
    if (!write_into_file) {
        outfile.open("/Users/WillJia/Desktop/IOS Lecture/Projects/shortestPath/shortestPath_Multiple_thread_without_readLock/file.txt");
    }
    
    // write into file
    outfile << "\nTotal running Time = " << total_time_of_running << endl;
    outfile << "\nTotal calcuating Time = " << total_time_of_calculating << endl;
    outfile << "Total create = " << plan_thread_count <<  " threads(plan)" << endl;
    
    // get final result
    printf("\nTotal running Time = %f\nTotal calculating Time = %f\nTotal create threads time %f\nTotal create %d threads(plan) " , total_time_of_running , total_time_of_calculating, total_time_of_creating_threads, plan_thread_count);
    
    
    // close file
    outfile.close();
    
    // destroy lock
    sem_destroy(&writeLock);
    sem_destroy(&readCountLock);
    
    return 0;
}
