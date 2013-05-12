#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

/* Constantes */
#define INFINITY 1000000000  

/* Estruturas */
typedef struct vertex{
	int val;
	int pair_G1;
	int dist;
	struct adj *adj_next;
} vertex;

typedef struct adj{
	int val;
	struct adj *adj_next;
} adj;



/* VARIAVEIS GLOBAIS */
vertex* nosMatrix;
int *q, *pair_G2;
int nStud, nJobs, noNIL, qMax;
int head = 0;
int tail = 0;

int min (int x, int y) {  
  return x<y ? x : y; // returns minimum of x and y  
}  
  
void enqueue (int x) { //add to queue  
	if(tail >= qMax-5){
		qMax *= qMax;
		q = (int*) realloc (q, qMax * sizeof(int));
	}
  q[tail] = x;  
  tail++; 
}  

int dequeue () {    //remove from queue  
  int x = q[head];  
  head++;
  return x;  
} 

void push(int index, adj* ptr){
	if(nosMatrix[index].adj_next == NULL){
		nosMatrix[index].adj_next = ptr;
	}
	else{
		ptr->adj_next = nosMatrix[index].adj_next;
		nosMatrix[index].adj_next = ptr;
	}

}

int BFS (){
		int v, u, n;
		adj* ptr;
		
    for(v = 0; v < nStud; v++){ // v in G1
        if (nosMatrix[v].pair_G1 == -1){
            nosMatrix[v].dist = 0;
            enqueue(v);
        }
        else{
            nosMatrix[v].dist = INFINITY;
        }
    }
    noNIL = INFINITY;
    //printf("entro aqui1\n");
    while (tail != head){
        v = dequeue();
        n = v;
        if (nosMatrix[v].dist < noNIL){
            for (ptr = nosMatrix[v].adj_next; ptr != NULL; ptr = ptr->adj_next){ // each u in Adj[v]
            		u = ptr->val;
            		//printf("entro aqui: %d\n", u);
                if (pair_G2[u] == -1){
                    if(noNIL == INFINITY){
                    noNIL = nosMatrix[v].dist + 1;
                    //enqueue(-1);
                		}
                }
                else{
                	if(nosMatrix[pair_G2[u]].dist == INFINITY){
                		nosMatrix[pair_G2[u]].dist = nosMatrix[v].dist + 1;
                		enqueue(pair_G2[u]);
                 	}
                }
            }
       	}
    }
    return (noNIL != INFINITY);
}

int DFS(int v){
	adj* ptr;
	int u;
	
    if (v != -1){
        for(ptr = nosMatrix[v].adj_next; ptr != NULL; ptr = ptr->adj_next){
        		u = ptr->val;
        		
            if (pair_G2[u] == -1){
            	if(noNIL == nosMatrix[v].dist + 1){
		          	if (DFS(pair_G2[u])){
		                pair_G2[u] = v;
		                nosMatrix[v].pair_G1 = u;
		                return 1;
		            }
              }
            }
            else{
             	if(nosMatrix[pair_G2[u]].dist == nosMatrix[v].dist + 1){
		            if (DFS(pair_G2[u])){
		              	pair_G2[u] = v;
		                nosMatrix[v].pair_G1 = u;
		                return 1;
		            }
              }
            }
        }    
        nosMatrix[v].dist = INFINITY;
        return 0;
    }
    return 1;
}

int H_K(){
    int matching, i;
    matching = 0;
  
    while (BFS()){
    		//printf("matching: %d", matching);
        for (i=0; i < nStud; i++){/* vertices em v1 */
            if (nosMatrix[i].pair_G1 == -1){
            		//printf("entrei dfs: %d", i);
                if (DFS(i)){
                    matching = matching + 1;
                }
            }
        }
   	}
    return matching;
}

 
 void printMatrix(){
	int i;
	adj* ptr;
	printf("----------\n");
    for (i=0; i < nStud; i++) {  
      for (ptr = nosMatrix[i].adj_next; ptr != NULL; ptr = ptr->adj_next) {  
       printf("%d ", ptr->val);  
      }  
     printf("\n");  
    }  
 }

int main(int argc, char** argv){
	adj* test;
	int nStudLst, nJobsLst, i, j;
	
	scanf("%d %d", &nStud, &nJobs);
	
	/* alloc the Nodes matrix */
	nosMatrix = (vertex *)malloc(nStud * sizeof(vertex));
	q = (int *)malloc(nStud * sizeof(int));
	qMax=nStud;
	pair_G2 = (int *)malloc(nJobs * sizeof(int));
	for(i = 0; i < nJobs; i++){ 
		pair_G2[i]=-1;
	}
	for(i = 0; i < nStud; i++){ 
		nosMatrix[i].val = i;
		nosMatrix[i].pair_G1 = -1;
		nosMatrix[i].dist = INFINITY;
		nosMatrix[i].adj_next = NULL;		
	}
	
	

	for(i = 0; i < nStud; i++){
		scanf("%d", &nStudLst);
		for(j = 0; j < nStudLst; j++){
			scanf("%d", &nJobsLst);
			
			test = (adj*) malloc (sizeof(adj));
			test->val = nJobsLst;
			test->adj_next = NULL;
			
			push(i, test);
		}
	}
	
//	printMatrix();
	
	printf("%d\n", H_K());
	
	
	/* free the memory */
	for(i = 0; i < nStud; i++){ 
		if(nosMatrix[i].adj_next != NULL){
			while(nosMatrix[i].adj_next->adj_next != NULL){
				test = nosMatrix[i].adj_next->adj_next;
				free(nosMatrix[i].adj_next);
				nosMatrix[i].adj_next = test;
			}
			free(nosMatrix[i].adj_next);
		}
	}
	free(nosMatrix);
	free(q);
	free(pair_G2);	
	
	return 0;
}
