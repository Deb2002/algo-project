#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

int ind = 0;                    // index for the edges

struct edge{                    // edges -> roads b/w two vertices
    int a;
    int b;
    int index;
    int weight;
    int visited;
    int no_bin;
};

struct adj_lst_node{            // adjacency list representation 
    int data;  
    struct adj_lst_node* next;
};

struct vertex{                  // vertices -> intersection points of the roads
    int total_edges;
    int last_bin;
    int index;
    struct adj_lst_node* head;
    struct adj_lst_node* current;
}; 

struct edge* new_edge(int a, int b, int weight){    // returns an edge b/w vertices[a] and vertices[b] with weight
    struct edge* temp = (struct edge*)malloc(sizeof(struct edge));
    temp->a = a;
    temp->b = b;
    temp->index = ind;
    temp->weight = weight;
    temp->no_bin = 0;
    temp->visited = 0;
    return temp;
}

struct adj_lst_node* new_adj_lst_node(){  // adj list implementation
    struct adj_lst_node* temp = (struct adj_lst_node*)malloc(sizeof(struct adj_lst_node));
    temp->data = ind;
    temp->next = NULL;
    return temp;
}

struct edge* add_edge( int a, int b, int weight, struct vertex* vertices){  // adds an edge b/w vertices[a] and vertices[b]
    struct edge* temp = new_edge(a, b, weight);
    struct adj_lst_node* adj_node1 = new_adj_lst_node();
    
    if(vertices[a].head==NULL){
        vertices[a].head = adj_node1;
        vertices[a].current = adj_node1;
    }else{
        vertices[a].current->next = adj_node1;
        vertices[a].current = vertices[a].current->next;
    }
    vertices[a].total_edges++;
    
    struct adj_lst_node* adj_node2 = new_adj_lst_node();
    if(vertices[b].head==NULL){
        vertices[b].head = adj_node2;
        vertices[b].current = adj_node2;
    }else{
        vertices[b].current->next = adj_node2;
        vertices[b].current = vertices[b].current->next;
    }
    vertices[b].total_edges++;
  
    return temp;
}

struct queue{               // array implementation of queue
    int size, front, rear;
    int capacity;
    int* keys;
};

struct queue* new_queue(int V){     // creates and initializes a queue containing indices and returns it
    struct queue* q = (struct queue*)malloc(sizeof(struct queue));
    q->capacity = V;
    q->size = 0;
    q->front = 0;
    q->rear = q->capacity-1;
    q->keys = (int*)malloc(V*sizeof(int));
    for(int i=0; i<V; i++)
        q->keys[i]=-1;
        
    return q;
}

void enqueue(struct queue* q, int s){   // enqueue index s in queue q
    if(q->size == q->capacity)
        return;
        
    q->rear = (q->rear + 1)%q->capacity;
    q->keys[q->rear] = s;
    q->size++;
}

int dequeue(struct queue* q){       // removes the element with the most priority and returns it
    if(q->size==0)
        return INT_MAX;
        
    int ans = q->keys[q->front];
    q->front = (q->front + 1)%q->capacity;
    q->size--;
    return ans;
}

int geta(struct edge* e, int x)
{
  if(e->a == x)  return e->b;
  else  return e->a;
}

int maximum(int a, int b){      // utility function return max(a, b)
    if(a>b)
        return a;
    else
        return b;
}


int BFS(int s, struct vertex* vertices, int V, int k, int total, struct edge** edges){
    
    // greedy choice for s, atart bfs with the vertex which has 
    // maximum number of edges attached
    int visited[V];
    for(int i=0; i<V; i++)
        visited[i] = 0;

    struct queue* q = new_queue(V);

    visited[s]=1;
    vertices[s].last_bin = 0;
    total++;
    enqueue(q, s);

    while (q->size != 0)
    {
        s = dequeue(q);

        struct adj_lst_node* node = vertices[s].head;
        while (node!=NULL)
        {
            struct edge* e = edges[node->data];
            node = node->next;
            int weight = e->weight;
            int n = geta(e, s);
            int lastBin;
            if(!e->visited){                         //if the edge was not visited
                e->visited = 1;
                int dis = vertices[s].last_bin;

                if(weight>=(k-dis)){
                    weight-=(k-dis);
                    total += 1 + weight/k;
                    lastBin = weight - k*(weight/k);
                    e->no_bin = 1+weight/k;
                    if(dis!=0)  vertices[s].last_bin = maximum(dis, k-dis);
                }
                else{
                    if(weight>dis && dis!=0)  vertices[s].last_bin = weight;
                    lastBin = weight + dis;
                }
                if(vertices[n].last_bin==-1)   vertices[n].last_bin = lastBin;
                else if(vertices[n].last_bin==0 && lastBin==0){
                    total--;                    //this is because if there is already a bin present on that vertex and we are trying to install another hence we will decrease 1 from it
                }
                else if(lastBin==0)     vertices[n].last_bin = 0;       //if we are installing a bin on that vertex
                else if(vertices[n].last_bin!=0 && lastBin!=0){   
                  if(vertices[n].last_bin> lastBin)
                  vertices[n].last_bin = vertices[n].last_bin;
                  else
                    vertices[n].last_bin = lastBin;
                }


            }
            if (!visited[n])
            {
                visited[n] = 1;
                enqueue(q, n);
            }
        }
    }
    return total;
}


int main(){
    int V, E, k;                        // V: #vertices, E: #edges, k: maximum distance
    printf("Enter the number of points of intersection: ");
    scanf("%d", &V);
    printf("Enter the number of roads: ");
    scanf("%d", &E);
    printf("Enter the maximum allowable distance b/w successive dustbins: ");
    scanf("%d", &k);
    
    struct vertex* vertices = (struct vertex*)malloc(V*sizeof(struct vertex));
    
    struct edge** edges = (struct edge**)malloc(E*sizeof(struct edge*));
    for(int i=0; i<V; i++){
        vertices[i].index = i;
        vertices[i].last_bin = -1;
        vertices[i].total_edges = 0;
        vertices[i].head = NULL;
        vertices[i].current = vertices[i].head;
      
    }
    printf("\nEnter the %d roads as the start index, end index, length:\n", E);
    for(int i=0; i<E; i++){
        int a,b, w;
        scanf("%d %d %d", &a, &b, &w);
        edges[i] = add_edge(a,b,w,vertices);
        ind++;
    }
    
    int max = 0, maxi = -1;
  
        for(int i=0;i<V;i++){
            if(max<vertices[i].total_edges){
                max = vertices[i].total_edges;
                maxi = i;
            }
        }
  
        int total = BFS(maxi, vertices, V, k, 0, edges);
  
        for(int i=0;i<E;i++){
            printf("Dustbin installed on the Edge connecting %d and %d are %d\n",edges[i]->a, edges[i]->b, edges[i]->no_bin);
        }
  
        printf("Total number of bins installed are %d",total);
    
    
}
