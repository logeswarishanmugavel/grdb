#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "graph.h"
# define MAX 20

void cli_dijkstra(char cmdline, int pos){
	printf("Applying Dijkstra's Algorithm to the selected graph\n");
	
	//Check if atleast one graph is present
	assert (graphs != NULL);

	//initialization
	graph_t g;
	vertex_t v;
	edge_t e;
	int vcnt = 0;
	int ecnt = 0;

	//Run the algorithm for all the graphs
	for(g = graphs; g!=NULL; g = g->next){
		if(g == current){
			//graph_print(g,1);
			for(v = g->v; v!=NULL; v = v->next){
				//vertex_print(v);
				vcnt++;
			}
			//printf("%d\n", vcnt);
			for(e = g->e; e!=NULL; e = e->next){
				//edge_print(e);
				ecnt++;
			}
			//printf("%d\n", ecnt);
		}
	}
}