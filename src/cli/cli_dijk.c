#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "cli.h"
#include "graph.h"
#include "tuple.h"
# define MAX 20

typedef int bool;
#define true 1
#define false 0

struct AdjListNode
{
    int dest;
    int weight;
    struct AdjListNode* next;
};

struct AdjList
{
    struct AdjListNode *head;  // pointer to head node of list
};

struct AdjList* array;

struct AdjListNode* newAdjListNode(int dest, int weight)
{
    struct AdjListNode* newNode =
            (struct AdjListNode*) malloc(sizeof(struct AdjListNode));
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}


struct MinHeapNode
{
    int  v;
    int dist;
};

struct MinHeap
{
    int size;      // Number of heap nodes present currently
    int capacity;  // Capacity of min heap
    int *pos;     // This is needed for decreaseKey()
    struct MinHeapNode **array;
};

struct MinHeapNode* newMinHeapNode(int v, int dist)
{
    struct MinHeapNode* minHeapNode =
           (struct MinHeapNode*) malloc(sizeof(struct MinHeapNode));
    minHeapNode->v = v;
    minHeapNode->dist = dist;
    return minHeapNode;
}

struct MinHeap* createMinHeap(int capacity)
{
    struct MinHeap* minHeap =
         (struct MinHeap*) malloc(sizeof(struct MinHeap));
    minHeap->pos = (int *)malloc(capacity * sizeof(int));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array =
         (struct MinHeapNode**) malloc(capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}

void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b)
{
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

void minHeapify(struct MinHeap* minHeap, int idx)
{
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;
 
    if (left < minHeap->size &&
        minHeap->array[left]->dist < minHeap->array[smallest]->dist )
      smallest = left;
 
    if (right < minHeap->size &&
        minHeap->array[right]->dist < minHeap->array[smallest]->dist )
      smallest = right;
 
    if (smallest != idx)
    {
        // The nodes to be swapped in min heap
        struct MinHeapNode *smallestNode = minHeap->array[smallest];
        struct MinHeapNode *idxNode = minHeap->array[idx];
 
        // Swap positions
        minHeap->pos[smallestNode->v] = idx;
        minHeap->pos[idxNode->v] = smallest;
 
        // Swap nodes
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
 
        minHeapify(minHeap, smallest);
    }
}

int isEmpty(struct MinHeap* minHeap)
{
    return minHeap->size == 0;
}

struct MinHeapNode* extractMin(struct MinHeap* minHeap)
{
    if (isEmpty(minHeap))
        return NULL;
 
    // Store the root node
    struct MinHeapNode* root = minHeap->array[0];
 
    // Replace root node with last node
    struct MinHeapNode* lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;
 
    // Update position of last node
    minHeap->pos[root->v] = minHeap->size-1;
    minHeap->pos[lastNode->v] = 0;
 
    // Reduce heap size and heapify root
    --minHeap->size;
    minHeapify(minHeap, 0);
 
    return root;
}

void decreaseKey(struct MinHeap* minHeap, int v, int dist)
{
    // Get the index of v in  heap array
    int i = minHeap->pos[v];
 
    // Get the node and update its dist value
    minHeap->array[i]->dist = dist;
 
    // Travel up while the complete tree is not hepified.
    // This is a O(Logn) loop
    while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist)
    {
        // Swap this node with its parent
        minHeap->pos[minHeap->array[i]->v] = (i-1)/2;
        minHeap->pos[minHeap->array[(i-1)/2]->v] = i;
        swapMinHeapNode(&minHeap->array[i],  &minHeap->array[(i - 1) / 2]);
 
        // move to parent index
        i = (i - 1) / 2;
    }
}

bool isInMinHeap(struct MinHeap *minHeap, int v)
{
   if (minHeap->pos[v] < minHeap->size)
     return true;
   return false;
}

void printArr(int dist[], int n)
{
    printf("Vertex   Distance from Source\n");
    for (int i = 0; i < n; ++i)
        printf("%d \t\t %d\n", i, dist[i]);
}

void run_dijkstra(graph_t g, int source, int vcnt){
	
	int dist[vcnt];

	struct MinHeap* minHeap = createMinHeap(vcnt);
	for(int v = 0; v < vcnt; v++){
		dist[v] = INT_MAX;
		minHeap->array[v] = newMinHeapNode(v, dist[v]);
		minHeap->pos[v] = v;
	}
	minHeap->array[source] = newMinHeapNode(source,dist[source]);
	minHeap->pos[source] = source;
	dist[source] = 0;
	decreaseKey(minHeap, source, dist[source]);
	minHeap->size = vcnt;
	printf("test1\n");
	while(!isEmpty(minHeap))
	{
		printf("test2\n");
		struct MinHeapNode* minHeapNode = extractMin(minHeap);
		int u = minHeapNode->v;
		printf("test3\n");
		struct AdjListNode* list = array[u].head;
		printf("%d\n",u);
		while(list != NULL){
			int v = list->dest;
			printf("test4\n");
			printf("%d\n",v);
			if(isInMinHeap(minHeap,v) && (dist[u] != INT_MAX) && (list->weight + dist[u] < dist[v])){
				printf("test5\n");
				dist[v] = dist[u] + list->weight;
				decreaseKey(minHeap, v, dist[v]);
				printf("test6\n");
			}
			list = list->next;
			printf("test7\n");
		}
		printf("test8\n");
	}
	printArr(dist,vcnt);
}

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
	int e_weight = 0;
	int id1 = 0;
	int id2 = 0;
	int vid;

	//Run the algorithm for all the graphs
	for(g = graphs; g!=NULL; g = g->next){
		if(g == current){
			//graph_print(g,1);
			array = (struct AdjList*) malloc(vcnt*sizeof(struct AdjList));
			for(v = g->v; v!=NULL; v = v->next){
				//vertex_print(v);
				vid = v->id;
				//printf("%d\n",vid);
				vcnt++;
				array[vid].head = NULL;
			}
			//printf("%d\n", vcnt);
			for(e = g->e; e!=NULL; e = e->next){
				//edge_print(e);
				ecnt++;
				if (e->tuple != NULL && 1)
				{
	 				e_weight = get_tuple_weight(e->tuple, g->el);
			    	id1 = e->id1;
			    	id2 = e->id2;
			    	//printf("%d\n",id1);
			    	//printf("%d\n",id2);
			    	
			    	struct AdjListNode* newNode = newAdjListNode(id2, e_weight);
    				newNode->next = array[id1].head;
    				array[id1].head = newNode;
    				
    				newNode = newAdjListNode(id1,e_weight);
    				newNode->next = array[id2].head;
    				array[id2].head = newNode;
				}
			}
		}
		//printf("%d\n", ecnt);
		printf("sixthtest\n");
		run_dijkstra(g,0,vcnt);
		printf("seventhtest\n");
	}
}