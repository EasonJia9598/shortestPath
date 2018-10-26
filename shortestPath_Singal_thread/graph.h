//
//  graph.h
//  shortestPath_Singal_thread
//
//  Created by WillJia on 2018-10-25.
//  Copyright © 2018 WillJia. All rights reserved.
//

#ifndef graph_h
#define graph_h

#include <iostream>
#include <pthread.h>
#include <math.h>
#include <time.h>

using namespace std;

// 2D arrays
int **graph;
int **dist;

// arrays info
int N_nodes, M_edges;


/************************************************************************
 
 strcut:        initializeGraph
 
 Description:   initialize Graph
 
 *************************************************************************/
void initializeGraph( int N_nodes, int M_edges){
    // create space for Adjacency matrix
    graph = (int**)malloc(N_nodes * sizeof(int*));
    for (int i = 0; i < N_nodes; i++) {
        graph[i] = (int*)malloc(N_nodes * sizeof(int));
    }
    
    // create space for dist matrix
    dist = (int**)malloc(N_nodes * sizeof(int*));
    for (int i = 0; i < N_nodes; i++) {
        dist[i] = (int*)malloc(N_nodes * sizeof(int));
    }
    
    // initialize graph
    for (int i = 0; i < N_nodes; i++) {
        for (int j = 0; j < N_nodes; j++) {
            graph[i][j] = 0;
            dist[i][j] = (i == j ? 0 : INT_MAX);
        }
    }
    
}

void signEdges(int M_edges){
    printf("Enter edges\n");
    // enter edges
    int vi = 0, uj = 0, k = 0;
    for (int i = 0; i < M_edges; i++) {
        scanf("%d %d %d", &vi , &uj, &k);
        // undirected graph
        graph[vi - 1][uj - 1] = 1;
        graph[uj - 1][vi - 1] = 1;
        dist[vi - 1][uj - 1]  = k;
        dist[uj - 1][vi - 1]  = k;
    }
}

void printHeader(){
    printf("%5s" , " ");
    for (int i = 1; i <= N_nodes; i++) {
        printf("%-4d " , i);
    }
    cout << endl;
    for (int i = 0; i <= N_nodes; i++) {
        printf("****");
    }
    cout << endl;
}

void printGraph(){
    printf("\nAdjacency Matrix\n");
    printHeader();
    for (int i = 0; i < N_nodes ; i++) {
        printf("%-2d| " , i + 1);
        for (int j = 0 ; j < N_nodes; j++) {
            printf("%-4d " , graph[i][j]);
        }
        printf("\n");
    }
}

void printDist(){
    printf("\nDistance Matrix\n");
    printHeader();
    for (int i = 0; i < N_nodes ; i++) {
        printf("%-2d| " , i + 1);
        for (int j = 0 ; j < N_nodes; j++) {
            (dist[i][j] == INT_MAX) ? printf("INF ") : printf("%-4d " , dist[i][j]);
        }
        printf("\n");
    }
}


#endif /* graph_h */
