//
//  main.cpp
//  shortestPath_Multiple_thread
//
//  Created by WillJia on 2018-10-25.
//  Copyright © 2018 WillJia. All rights reserved.
//

#include <iostream>
#include "graph.h"
//total_time_of_running total_time_of_running
double total_time_of_running_trace = 0;

/************************************************************************
 
 strcut:        shortestPath
 
 Description:   shortest Path main functions
 
 *************************************************************************/
void *shortestPath(void *param){
    for (int k = 0 ; k < N_nodes; k++) {
        printf("K %d\n" , k);
    const clock_t begin_t = clock();
      for (int i = 0 ; i < N_nodes; i++) {
        for (int j = 0 ; j < N_nodes; j++) {
            if (graph[i][k] != 0 &&  graph[k][j] != 0 && dist[i][k] + dist[k][j] < dist[i][j]) {
                dist[i][j] = dist[i][k] + dist[k][j];
            }
        }
      }
        total_time_of_running_trace += float( clock () - begin_t ) /  CLOCKS_PER_SEC;
        printf("iteration time %f \n" ,total_time_of_running_trace );

    }
    
    pthread_exit(NULL);
}


int main(int argc, const char * argv[]) {
    double total_time_of_running;
    // enter N and M
    printf("Enter N and M (N for n nodes, M for m edges)\n");
    scanf("%d %d", &N_nodes, &M_edges);
    printf("Chose whether print out whole distance matrix or not, Y for yes, N for no(Captial Letter)\n");
    char option;
    scanf("\n%c" , &option);
    
    initializeGraph(N_nodes, M_edges);
    signEdges(M_edges);
    
    if (option == 'Y') {
        printf("INPUT:\n");
        printGraph();
    }
    
    pthread_t cal;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    
    const clock_t begin_time = clock();
    
    // start to create 5 pthread with specific functions
    pthread_create(&cal, &attr, shortestPath, NULL);
    
    pthread_join(cal, NULL);
    total_time_of_running = float( clock () - begin_time ) /  CLOCKS_PER_SEC;

    if (option == 'Y') {
        for (int i = 0; i < N_nodes; i++) {
            printf("*****");
        }
        
        cout << endl;
        printf("OUTPUT:\n");
        printGraph(); printDist();
    }


    printf("Total Time = %f\n" , total_time_of_running);
    
    return 0;
}
