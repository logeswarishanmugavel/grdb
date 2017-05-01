#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "cli.h"
#include "graph.h"
#include "tuple.h"
#define MAX 9999
int vcnt;

/*--------------------------------------------------------------------------------------*/
//		Dijkstra's Algorithm for the grdb database 
/*--------------------------------------------------------------------------------------*/


/*--------------------------------------------------
 * reverse_string
 *
 * This function reverses the given string
 *
 * @param (char*) input string
 *
 * @return (char*)
 --------------------------------------------------*/

char* reverse_string(char *path){
	
	//Declaring Variables
	int i = 0;
    int j = strlen(path) -1;
    char temp;

    //Exchanging the character positions within the string
    while(i<=j){
		temp = path[i];
		path[i] = path[j];
		path[j] = temp;
		i++;
		j--;
    }

    return path;
}

/*--------------------------------------------------
 * print_result
 *
 * This function prints the output of the algorithm
 *
 * @param (graph_t) the selected graph
 * @param (int) the source vertex
 * @param (int[]) holds the distance from source
 * @param (int[]) holds previous node of the vertex
 *				   in the path
 *
 * @return (void)
 --------------------------------------------------*/

void print_result(graph_t g, int source, int dist[vcnt], int prev[vcnt]){
	
	//Declaring Variables
	int vid1, next;
	vertex_t u;
	char *rem = (char*)calloc(255, sizeof(char));
	char *path = (char*)calloc(255, sizeof(char));

	printf("\n--------------------------------------------------------------------------------------");
	printf("\nShortest Path to all vertices from the source %d: ", source);
	printf("\n--------------------------------------");
	printf("\nVertex\tDistance\tPath");
	printf("\n--------------------------------------");

	//Printing the output
	for(u = g->v; u!=NULL; u = u->next){
		vid1 = u->id;
			//Vertex and corresponding distance from the source
			printf("\n%d\t%d\t",vid1,dist[vid1]);

			//the path
			sprintf(path, "%d", vid1);
			next = prev[vid1];
            do{
            	if(next!=source){
            		sprintf(rem, "%d", next);
            		strcat(path,"-");
            		strcat(path,rem);
            	}
            	next = prev[next];
            }while(next!=source);
           	sprintf(rem, "%d", next);
            strcat(path,"-");
            strcat(path,rem);
           
            printf("\t%s",reverse_string(path));
	}
	printf("\n");
}

/*--------------------------------------------------
 * run_dijkstra
 *
 * This function executes the algorithm and 
   calculates the shortest path between source and 
   other vertices
 *
 * @param (int[][]) the cost matrix
 * @param (graph_t) the selected graph
 * @param (int) the source vertex
 *
 * @return (void)
 --------------------------------------------------*/

void run_dijkstra(int G[vcnt][vcnt], graph_t g, int source){
	
	//Declaring Variables
	int dist[vcnt], prev[vcnt], visited[vcnt];
	int count, minDist, nextNode;
	int vid1, vid2;
	vertex_t u,v;

	//Initialize the distance vector from the direct path if present
	for(u = g->v; u!=NULL; u = u->next){
		vid1 = u->id;
		dist[vid1] = G[source][vid1];
		prev[vid1] = source;
		visited[vid1] = 0;
	}

	//the properties of source vertex are set
	dist[source] = 0;
	visited[source] = 1;
	count = 1;

	while(count < vcnt-1){

		//Finding the minimum distance vertex
		minDist = MAX;
		for(u = g->v; u != NULL; u = u->next){
			vid1 = u->id;
			if(dist[vid1] < minDist && !visited[vid1]){
				minDist = dist[vid1];
				nextNode = vid1;
			}
		}

		//Updating the distance value of all the adjacent vertices
		//For every adjacent vertex vid2, if sum of distance value of vid1 (from source) and weight of edge vid1-vid2, 
		//is less than the distance value of vid2, then update the distance value of vid2.
		visited[nextNode] = 1;
		for(v = g->v; v != NULL; v = v->next){
			vid2 = v->id;
			if(!visited[vid2]){
				if(minDist + G[nextNode][vid2] < dist[vid2]){
					dist[vid2] = minDist+G[nextNode][vid2];
					prev[vid2] = nextNode;
				}
			}
		}

		count++;
	}

	//Printing the result
	print_result(g, source, dist, prev);
}

/*--------------------------------------------------
 * cli_dijkstra
 *
 * This function initializes the required parameters
   and calls the run function
 *
 * @param (char*) the commandline prompt
 * @param (int*) 
 *
 * @return (void)
 --------------------------------------------------*/

void cli_dijkstra(char *cmdline, int *pos){
	printf("\n--------------------------------------------------------------------------------------");
	printf("\n                         Applying Dijkstra's Algorithm: ");
	printf("\n--------------------------------------------------------------------------------------");

	//To check if atleast one graph is available
	assert (graphs != NULL);

	//Declaring Variables;
	graph_t g;
	vertex_t u,v;
	edge_t e;
	int e_weight = 0;
	int vid1, vid2;

	//For all the graphs
	for(g = graphs; g!=NULL; g = g->next){
		if(g == current){
			
			//Printing the selected graph
			printf("\nSelected graph:\n");
			graph_print(g,1);
			
			//Counting the number of vertices in the graph
			for(v = g->v; v!=NULL; v = v->next){
				vcnt++;
			}

			//Initializing the cost matrix;
			int G[vcnt][vcnt];
			for(u = g->v; u!=NULL; u = u->next){
				vid1 = u->id;
				for(v = g->v; v!=NULL; v = v->next){
					vid2 = v->id;
					G[vid1][vid2] = MAX;
				}
			}

			//Updating the cost matrix with the edge weights			
			for(e = g->e; e!=NULL; e = e->next){
				if (e->tuple != NULL && 1)
				{
	 				e_weight = get_tuple_weight(e->tuple, g->el);
			    	vid1 = e->id1;
			    	vid2 = e->id2;
			    	if(cmdline[6]-'0' == 0){
			    		G[vid1][vid2] = e_weight;
			    		G[vid2][vid1] = e_weight;
			    	}
			    	else if(cmdline[6]-'0' == 1){
			    		G[vid1][vid2] = e_weight;
			    	}
				}
			}

			//Executing the algorithm
			run_dijkstra(G,g,cmdline[4]-'0');
		}
	}
}