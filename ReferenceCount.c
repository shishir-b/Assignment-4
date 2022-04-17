// Name : Shishir Bhalerao
// Enroll no: BT20CSE023
// Software Lab -II Assignment-4 
// Garbage Collection through Reference Count.
#include<stdio.h>
#include<stdlib.h>
#define MAX_MEM 1024
#define N 11
int FREE_MEM;
int adj_Matrix[N+1][N+1]={0};

typedef struct node
{
    int node_vertex;
    struct node *next;
}Node;

typedef struct graph_node
{
    int total_vertex;
    int rfc[N];
    Node **adjacency_lists;
}Graph;

Node *createNode(int v)
{
    Node *newNode = malloc(sizeof(Node));
    newNode->node_vertex = v;
    newNode->next = NULL;
    return newNode;
}

void allocate_first_fit_heap(int id,int size_to_alloc,Node* g_ptr,int rfc);
void deallocate_heap(int id);

typedef struct allocate_heap_node_tag
{
    int id;
    int size;
    int starting_point;
    int free;
    int end_point;
    int rfc;
    Node* node;
    struct allocate_heap_node_tag* prev;
    struct allocate_heap_node_tag* next;
}allocate_heap;

allocate_heap* allocate_head;

typedef struct free_heap_node_tag
{
    int size;
    int starting_point;
    int end_point;
    int free;
    struct free_heap_node_tag* prev;
    struct free_heap_node_tag* next;
}free_heap;

free_heap* free_head;

free_heap *make_free_heap_node(int starting,int size);
allocate_heap *make_allocated_node(int starting ,int size,int id);
void free_node_add(free_heap *temp_free_ptr);
allocate_heap* allocate_node_add(free_heap *lptr,int size_to_alloc,int id,Node* g_ptr,int rfc);

Graph *createAGraph(int vertices)
{
    Graph *graph = malloc(sizeof(Graph));
    graph->total_vertex = vertices;

    graph->adjacency_lists = malloc(vertices * sizeof(Node *));

    for (int i = 0; i < vertices; i++)
    {
        graph->adjacency_lists[i] = NULL;
        graph->rfc[i] = 0;
    }
    return graph;
}

void addEdge(Graph *graph, int s, int d)
{
    Node *newNode = createNode(d);
    newNode->next = graph->adjacency_lists[s];
    graph->adjacency_lists[s] = newNode;
    adj_Matrix[s][d]=1;
    graph->rfc[d]++;
}

void printGraph(Graph *graph)
{
    int v;
    printf("-----------------------------------------------------------------------------------------------------\n");
    printf("The Adjacency List is : \n");
    printf("-----------------------------------------------------------------------------------------------------\n");
    for (v = 0; v < graph->total_vertex; v++)
    {
        Node *temp = graph->adjacency_lists[v];
        if(temp!=NULL || graph->rfc[v]>0){
        printf("Node %d RFC is %d with address %p. : ", v, graph->rfc[v], graph->adjacency_lists[v]);
        while (temp)
        {
            printf("%d -> ", temp->node_vertex);
            temp = temp->next;
        }
        printf("\n");
        }
    }
    printf("-----------------------------------------------------------------------------------------------------\n");
    printf("The Adjacency Matrix is :\n");
    printf("-----------------------------------------------------------------------------------------------------\n");
    printf("\t");
    for (int i = 1; i <= N; i++)
    {
        printf("%d ",i);
    }
    printf("\n\n");
    for (int i = 1; i <= N; i++)
    {
        printf("%d\t",i);
        for (int j = 1; j <= N; j++)
        {
            printf("%d ",adj_Matrix[i][j]);
        }
        printf("\n");
    }
    
}

void print_allocate(){
    allocate_heap *aptr;
    aptr=allocate_head;
    if(aptr==NULL){
        printf("Allocated Heap Memory is Empty.\n");
    }
    else{
        printf("Allocated Heap:\n");
        printf("ProcessID\tMemoryStart\tMemoryEnd\tSize\tRfc\tAddress\n");
        while(aptr!=NULL){
            aptr->end_point=aptr->starting_point+aptr->size;
            printf("%d\t\t%d\t\t%d\t\t%d\t%d\t%p\t",aptr->id,aptr->starting_point,aptr->end_point,aptr->size,aptr->rfc,aptr->node);
            Node *temp = aptr->node;
            while (temp)
            {
                printf("%d -> ", temp->node_vertex);
                temp = temp->next;
            }
            printf("\n");
            aptr = aptr->next;
        }
        printf("\n");
    }
}

void print_free(){
    free_heap *free_ptr;
    free_ptr=free_head;
    int total_free=0;
    printf("Free Heap Memory:\n");
    printf("MemoryStart\tMemoryEnd\tMemory Size\n");
    while(free_ptr!=NULL){
        free_ptr->end_point=free_ptr->starting_point+free_ptr->size;
        total_free+=free_ptr->size;
        printf("%d\t\t%d\t\t%d\n",free_ptr->starting_point,free_ptr->end_point,free_ptr->size);
        free_ptr = free_ptr->next;
    }
    FREE_MEM = total_free;
    printf("\nTotal Memory Free (Bytes)\t%d.",total_free);
    printf("\n");
}
void garbage_print(){
    printf("Garbage Nodes that are no longer required.\n");
    printf("-----------------------------------------------------------------------------------------------------\n");
    allocate_heap *aptr;
    aptr=allocate_head;
    printf("MemoryStart\tMemoryEnd\tMemory Size\n");
    while(aptr!=NULL){
        aptr->end_point=aptr->starting_point+aptr->size;
        if(aptr->rfc==0){
            printf("%d\t\t%d\t\t%d\t\t%d\t%d\t%p\t",aptr->id,aptr->starting_point,aptr->end_point,aptr->size,aptr->rfc,aptr->node);
            Node *temp = aptr->node;
            while (temp)
            {
                printf("%d -> ", temp->node_vertex);
                temp = temp->next;
            }
            printf("\n");
        }
        aptr = aptr->next;
    }
    printf("-----------------------------------------------------------------------------------------------------");
    printf("\n");
}
int main()
{
    Graph *graph = createAGraph(N);
    // Start Root either 5 or 1
    graph->rfc[5]=1;
    graph->rfc[1]=1;
    addEdge(graph, 5, 1);
    addEdge(graph, 7, 1);
    addEdge(graph, 7, 8);
    addEdge(graph, 3, 8);
    addEdge(graph, 3, 10);
    addEdge(graph, 8, 9);
    addEdge(graph, 1, 2);
    addEdge(graph, 1, 9);
    addEdge(graph, 1, 10);
    printGraph(graph);
    allocate_head=NULL;
    free_head=make_free_heap_node(0,MAX_MEM);
    for (int i = 0; i < N; i++)
    {
        Node *temp = graph->adjacency_lists[i];
        if(temp!=NULL || graph->rfc[i]>0){
            allocate_first_fit_heap(i,10,graph->adjacency_lists[i],graph->rfc[i]);
        }
    }
    printf("-----------------------------------------------------------------------------------------------------\n");
    print_allocate();
    int flag=1;
    int arr[N]={0};
    int gc_calls=0;
    while (flag)
    {
        flag=0;
        allocate_heap *aptr=allocate_head;
        while(aptr!=NULL){
            if(aptr->rfc==0 && arr[aptr->id]==0){
                flag=1;
                arr[aptr->id]=1;
                Node *temp = aptr->node;
                while (temp)
                {
                    int to_del=temp->node_vertex;
                    allocate_heap * ttptr=allocate_head;
                    while (ttptr)
                    {
                        if(ttptr->id==to_del){
                            ttptr->rfc--;
                        }
                        ttptr=ttptr->next;
                    }
                    temp = temp->next;
                }
            }
            aptr = aptr->next;
        }
        printf("-----------------------------------------------------------------------------------------------------\n");
        printf("After %d GC calls: \n",++gc_calls);
        printf("-----------------------------------------------------------------------------------------------------\n");
        print_allocate();
    }
    printf("-----------------------------------------------------------------------------------------------------\n\n");
    garbage_print();
    printf("\n\n");
    print_free();
    int free_before=FREE_MEM;
    printf("-----------------------------------------------------------------------------------------------------\n");
    printf("After Deleting the Garbage Nodes with Rfc set 0:\n");
    printf("-----------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < N; i++)
    {
        if(arr[i]==1){
            deallocate_heap(i);
        }
    }
    print_allocate();
    printf("-----------------------------------------------------------------------------------------------------\n");
    printf("After Merging all the free blocks together.\n");
    printf("-----------------------------------------------------------------------------------------------------\n");
    print_free();
    printf("-----------------------------------------------------------------------------------------------------\n");
    printf("Total Memory Freed After garbage Collection is %d Bytes.\n",FREE_MEM-free_before);
    printf("-----------------------------------------------------------------------------------------------------\n");
    return 0;
}

allocate_heap *make_allocated_node(int starting ,int size,int id){
    allocate_heap *temp;
    temp= (allocate_heap*)(malloc(sizeof(allocate_heap)));
    temp->starting_point = starting;
    temp->size=size;
    temp->end_point=temp->starting_point+temp->size;
    temp->id=id;
    temp->free=0;
    temp->next=NULL;
    temp->prev=NULL;
    return temp;
}

free_heap *make_free_heap_node(int starting,int size){
    free_heap *temp;
    temp= (free_heap*)(malloc(sizeof(free_heap)));
    temp->starting_point = starting;
    temp->size = size;
    temp->end_point=temp->starting_point+temp->size;
    if(temp->end_point>MAX_MEM) temp->end_point=MAX_MEM;
    temp->free=0;
    temp->next=NULL;
    temp->prev=NULL;
    return temp;
}
allocate_heap* allocate_node_add(free_heap *lptr,int size_to_alloc,int id,Node* g_ptr,int rfc){
    allocate_heap *allocate_ptr;
    int size=8; //8bytes for both node and it refernces
    Node* temp_gptr=g_ptr;
    while (temp_gptr)
    {
        size+=8;
        temp_gptr=temp_gptr->next;
    }
    size_to_alloc=size;
    allocate_ptr = make_allocated_node(lptr->starting_point,size_to_alloc,id);
    allocate_ptr->node=g_ptr;
    allocate_ptr->rfc=rfc;
    free(lptr);
    allocate_heap* lptr_allocated=allocate_head;
    if(lptr_allocated==NULL){
        allocate_head=allocate_ptr;
        return allocate_ptr;
    }
    
    if(lptr_allocated==allocate_head ){
        if(allocate_ptr->starting_point < lptr_allocated->starting_point){   
            lptr_allocated->prev=allocate_ptr;
            allocate_ptr->next=lptr_allocated;
            allocate_head=allocate_ptr;
            return allocate_ptr;
        }
    }
 
    int condition=(allocate_ptr->starting_point)>(lptr_allocated->starting_point);
    while (lptr_allocated->next!=NULL && condition)
    {
        lptr_allocated=lptr_allocated->next;
        condition=(allocate_ptr->starting_point)>(lptr_allocated->starting_point);
    }

    condition=(allocate_ptr->starting_point)>(lptr_allocated->starting_point);
    if(lptr_allocated->next==NULL && condition){
        if (allocate_ptr!=NULL)
        {  
            allocate_ptr->next=NULL;
            allocate_ptr->prev=lptr_allocated;
        }
        lptr_allocated->next=allocate_ptr;
        return allocate_ptr;
    }
    
    lptr_allocated->prev->next=allocate_ptr;

    if(allocate_ptr!=NULL){
        allocate_ptr->next=lptr_allocated;
        allocate_ptr->prev=lptr_allocated->prev;
    }
    
    lptr_allocated->prev=allocate_ptr;

    return allocate_ptr;
}
void free_node_add(free_heap *temp_free_ptr){
    if(temp_free_ptr==NULL) return ;
    free_heap *lptr;
    lptr=free_head;
    int condition=lptr->starting_point<temp_free_ptr->starting_point;
    while (condition && lptr->next!=NULL && lptr)
    {
        lptr=lptr->next;
        condition=lptr->starting_point<temp_free_ptr->starting_point;
    }
    condition=lptr->starting_point<temp_free_ptr->starting_point;
    if(condition && lptr->next==NULL && lptr){
        temp_free_ptr->prev=lptr;
        if(lptr!=NULL){
            lptr->next=temp_free_ptr;
        }
        return ;
    }

    if(lptr!=NULL && lptr==free_head){
        temp_free_ptr->next=lptr;
        free_head=temp_free_ptr;
        if(lptr!=NULL){
            lptr->prev=temp_free_ptr;
        }
        return ;  
    } 

    if(lptr!=NULL){
        temp_free_ptr->prev=lptr->prev;
        if(lptr!=NULL){
            lptr->prev->next=temp_free_ptr;
            lptr->prev=temp_free_ptr;
        }
        temp_free_ptr->next=lptr;
    }
}
void allocate_first_fit_heap(int id,int size_to_alloc, Node* g_ptr,int rfc){
    allocate_heap *aptr;
    aptr=allocate_head;
    while (aptr!=NULL)
    {
        if(aptr->id==id){
            int input;
            printf("Same id allocation found\nChoose 1)To Overwrite 2)Cancel\n");
            scanf("%d",&input);
            if(input==1) deallocate_heap(id);
            else return;
            aptr->next=NULL;
        }
        aptr=aptr->next;
    } 
    int size=8; //8bytes for both node and it refernces
    Node* temp_gptr=g_ptr;
    while (temp_gptr)
    {
        size+=8;
        temp_gptr=temp_gptr->next;
    }
    size_to_alloc=size;
    free_heap *lptr;
    lptr = free_head;
    while (lptr!=NULL && !(lptr->size>size_to_alloc))
    {
        lptr = lptr->next;
    }
    if(lptr==NULL){
        printf("Error No more free space available.\n");
        return ;
    }
    int last_point = lptr->starting_point + size_to_alloc;

    if(lptr->next==NULL && last_point>MAX_MEM){
        printf("Error No more free space available.\n");
        return ;
    }

    free_heap *free_ptr;
    free_ptr = make_free_heap_node(lptr->starting_point+size_to_alloc,size_to_alloc);

    if(lptr!=free_head){
        lptr->prev->next = free_ptr;
        if(free_ptr!=NULL){
            free_ptr->next=lptr->next;
            free_ptr->prev=lptr->prev;
        }
        if(lptr->next!=NULL) {
            lptr->next->prev=free_ptr;
        }
        int temp_size=lptr->size-size_to_alloc;
        free_ptr->size=temp_size;
    }
    else{
        if(free_ptr!=NULL){
            free_ptr->next = free_head->next;
        }
        free_head=free_ptr;
        int temp_size=lptr->size-size_to_alloc;
        free_ptr->size=temp_size;
    }  
    
    allocate_heap *allocate_ptr=allocate_node_add(lptr,size_to_alloc,id,g_ptr,rfc);
    //printf("Allocation Done from %d to %d.\n",allocate_ptr->starting_point,allocate_ptr->starting_point+allocate_ptr->size);
    
}

void deallocate_heap(int id){
    allocate_heap *nptr;
    nptr=allocate_head;
    if(nptr==NULL){
        printf("This id is not present.\n");
        return ;
    }
    free_heap *temp_free_ptr;
    while (nptr!=NULL && nptr->id!=id)
    {
        nptr=nptr->next;
        if(nptr==NULL || allocate_head==NULL){
            printf("This id is not present.\n");
            return ;
        }
    }

    temp_free_ptr = make_free_heap_node(nptr->starting_point,nptr->size);
    if(free_head==NULL){
        free_head = temp_free_ptr;
        return;
    }

    if (allocate_head!=nptr)
    {
        nptr->prev->next=nptr->next;
    }
    else{
        allocate_head=nptr->next;
    }

    if(nptr->next!=NULL){
        nptr->next->prev=nptr->prev;
    }

    free_node_add(temp_free_ptr);

    if(temp_free_ptr==NULL){
        return ;
    }

    if(temp_free_ptr!=NULL){
        if(temp_free_ptr->next!=NULL && temp_free_ptr->next->starting_point==temp_free_ptr->starting_point+temp_free_ptr->size){
            int s=temp_free_ptr->next->size;
            temp_free_ptr->next=temp_free_ptr->next->next;
            if(temp_free_ptr->next!=NULL){
                temp_free_ptr->next->prev=temp_free_ptr;
            }
            temp_free_ptr->size=s+temp_free_ptr->size;
        }
    }

    if(temp_free_ptr!=NULL){
        if(temp_free_ptr->prev!=NULL && temp_free_ptr->starting_point==temp_free_ptr->prev->starting_point+temp_free_ptr->prev->size){
            int s=temp_free_ptr->size;
            temp_free_ptr->prev->next=temp_free_ptr->next;
            if(temp_free_ptr->next!=NULL){
                temp_free_ptr->next->prev=temp_free_ptr->prev;
            }
            temp_free_ptr->prev->size=s+temp_free_ptr->prev->size;
            temp_free_ptr=temp_free_ptr->prev;
        }
    }
    return ;
}