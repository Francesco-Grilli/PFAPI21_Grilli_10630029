#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const unsigned long int INFINITY = 4294967295;

struct h{
    unsigned long int distance;
    unsigned long int currentPosition;
    unsigned long int* distanceArr;
    unsigned long int* elemArr;
    unsigned long int minHeapPosition;
    short int inMinHeap;
};
typedef struct h* headList;

struct minHeapNode{
    headList *head;
    unsigned long int currentSize;
};
typedef struct minHeapNode* minHeap;

struct maxHeapNode{
    unsigned long int* arrayCost;
    unsigned long int* graphId;
    unsigned long int currentSize;
};
typedef struct maxHeapNode* maxHeap;

unsigned long int rowLength;
unsigned long int maxNumberTopGraph;
unsigned long int graphId;
maxHeap topKHeap;

/**
 * FUNCTION TO CREATE MIN HEAP AND ADJACENCY LIST FOR DIJKSTRA ALGORITHM
 */
void addElementToList(headList list, unsigned long int distance, unsigned long int elem);
headList createList();
minHeap createMinHeap();
void addNewElementMinHeap(minHeap heap, headList head);
void minHeapFixUp(minHeap heap, unsigned long int pos);
void minHeapSwapElement(minHeap heap, unsigned long int f, unsigned long int s);
void removeMinHeapElement(minHeap heap);
headList getMinHeapElement(minHeap heap);
int emptyMinHeap(minHeap heap);
void minHeapFixDown(minHeap heap, unsigned long int pos);
unsigned long int djkRun();

/**
 * FUNCTION TO CREATE MAX HEAP AND TOPK VALUE
 */
maxHeap createMaxHeap();
void maxHeapFixDown(maxHeap heap, unsigned long int pos);
void addNewElementMaxHeap(maxHeap heap, unsigned long int key, unsigned long int id);
void maxHeapFixUp(maxHeap heap, unsigned long int pos);
void maxHeapSwapElement(maxHeap heap, unsigned long int f, unsigned long int s);
void maxHeapInsertDelete(maxHeap heap, unsigned long int key, unsigned long int prevElem);

void addGraph();
void showTopK();

int main(){

    if(scanf("%lu %lu", &rowLength, &maxNumberTopGraph));
    topKHeap = createMaxHeap();

    while(!feof(stdin)) {
        char* command = (char *) malloc(sizeof (char)*14);
        if (scanf("%s", command)==1) {
            if (strcmp(command, "AggiungiGrafo") == 0) {
                addGraph();
            } else if (strcmp(command, "TopK") == 0) {
                showTopK();
            } else {
                free(command);
                return 0;
            }
        }
        free(command);
    }
    return 0;

}

minHeap createMinHeap(){
    minHeap heap = (minHeap) malloc(sizeof (struct minHeapNode));
    heap->currentSize =0;
    heap->head = (headList *) malloc(sizeof(headList) * (rowLength +1));

    return heap;

}

void addNewElementMinHeap(minHeap heap, headList head){

    heap->currentSize++;
    heap->head[heap->currentSize] = head;
    heap->head[heap->currentSize]->minHeapPosition = heap->currentSize;
    heap->head[heap->currentSize]->inMinHeap=1;

    minHeapFixUp(heap, heap->currentSize);

}

void minHeapFixUp(minHeap heap, unsigned long int pos){

    while(pos>1 && heap->head[pos]->distance < heap->head[pos/2]->distance){
        minHeapSwapElement(heap, (pos/2), pos);
        pos = pos/2;
    }

}

void minHeapSwapElement(minHeap heap, unsigned long int f, unsigned long int s){

    headList father = heap->head[f];
    heap->head[f] = heap->head[s];
    heap->head[f]->minHeapPosition = f;
    heap->head[s] = father;
    heap->head[s]->minHeapPosition = s;

}

headList getMinHeapElement(minHeap heap){

    if(!emptyMinHeap(heap)){
        headList h = heap->head[1];
        h->inMinHeap=0;
        removeMinHeapElement(heap);
        return h;
    }

    return NULL;

}

int emptyMinHeap(minHeap heap){
    return heap->currentSize<=0;
}

void removeMinHeapElement(minHeap heap){

    if(!emptyMinHeap(heap)){
        if(heap->currentSize>1)
            minHeapSwapElement(heap, 1, heap->currentSize);
        heap->currentSize--;
        minHeapFixDown(heap, 1);
    }

}

void minHeapFixDown(minHeap heap, unsigned long int pos){

    while(pos*2 <= heap->currentSize){
        unsigned long int nPos = pos*2;
        if(nPos < heap->currentSize && heap->head[nPos+1]->distance < heap->head[nPos]->distance){
            if(heap->head[nPos+1]->distance < heap->head[nPos]->distance)
                nPos++;
        }
        if(heap->head[pos]->distance <= heap->head[nPos]->distance){
            break;
        }

        minHeapSwapElement(heap, pos, nPos);
        pos = nPos;
    }

}

unsigned long int djkRun(){

    minHeap heap = createMinHeap();
    headList* adjList = (headList*) malloc(sizeof (headList)*rowLength);

    unsigned long int i, j;

    adjList[0] = createList();
    adjList[0]->distance = 0;
    for(i=0; i<rowLength; i++){
        unsigned long int d;
        if(scanf("%lu,", &d));
        if(d>0){
            addElementToList(adjList[0], d, i);
        }
    }
    addNewElementMinHeap(heap, adjList[0]);
    for(i=1; i<rowLength; i++){
        adjList[i] = createList();
        adjList[i]->distance=INFINITY;
        for(j=0; j<rowLength; j++){
            unsigned long int d;
            if(scanf("%lu,", &d));
            if(d>0){
                addElementToList(adjList[i], d, j);
            }
        }
        addNewElementMinHeap(heap, adjList[i]);
    }


    unsigned long int sum=0;

    while(!emptyMinHeap(heap)){

        headList u = getMinHeapElement(heap);
        if(u==NULL)
            exit(0);
        if(u->distance < INFINITY )
            sum += u->distance;

        for(i=1; i<=u->currentPosition; i++){
            //controllo anche se l'elemento che sto andando a modificare la testa sia ancora nel min heap altrimenti non lo modifico poichè non ha senso
            if(u->distance<INFINITY && (adjList[u->elemArr[i]]->distance > (u->distanceArr[i] + u->distance)) && (adjList[u->elemArr[i]]->inMinHeap==1)){
                adjList[u->elemArr[i]]->distance = (u->distanceArr[i] + u->distance);
                minHeapFixUp(heap, adjList[u->elemArr[i]]->minHeapPosition);
            }
        }

    }

    for(i=0; i<rowLength; i++){
        free(adjList[i]->elemArr);
        free(adjList[i]->distanceArr);
        free(adjList[i]);
    }
    free(adjList);
    free(heap->head);
    free(heap);

    return sum;

}

headList createList(){

    headList list = (headList) malloc(sizeof (struct h));
    list->currentPosition =0;
    list->distanceArr = (unsigned long int*) malloc(sizeof (unsigned long int) * (rowLength+1));
    list->elemArr = (unsigned long int*) malloc(sizeof (unsigned long int)*(rowLength+1));

    return list;

}

void addElementToList(headList list, unsigned long int distance, unsigned long int elem){

    list->currentPosition++;
    list->distanceArr[list->currentPosition] = distance;
    list->elemArr[list->currentPosition] = elem;

}

maxHeap createMaxHeap(){

    maxHeap heap = (maxHeap) malloc(sizeof (struct maxHeapNode));
    heap->currentSize=0;
    heap->arrayCost = (unsigned long int*) malloc (sizeof (unsigned long int)* (maxNumberTopGraph+1));
    heap->graphId = (unsigned long int*) malloc(sizeof (unsigned long int)*(maxNumberTopGraph+1));

    return heap;

}

void maxHeapFixDown(maxHeap heap, unsigned long int pos){

    while((pos*2) <= heap->currentSize){
        unsigned long int nPos = pos*2;
        if(nPos < heap->currentSize){
            if((heap->arrayCost[nPos+1] > heap->arrayCost[nPos]) || ((heap->arrayCost[nPos+1] == heap->arrayCost[nPos]) && (heap->graphId[nPos+1] > heap->graphId[nPos])))
                nPos++;
        }
        if((heap->arrayCost[pos] > heap->arrayCost[nPos]) || (heap->arrayCost[pos] == heap->arrayCost[nPos] && heap->graphId[pos] > heap->graphId[nPos])){
            break;
        }

        maxHeapSwapElement(heap, pos, nPos);
        pos = nPos;
    }

}

void addNewElementMaxHeap(maxHeap heap, unsigned long int key, unsigned long int id){

    heap->currentSize++;
    heap->arrayCost[heap->currentSize] = key;
    heap->graphId[heap->currentSize] = id;

    maxHeapFixUp(heap, heap->currentSize);

}

void maxHeapFixUp(maxHeap heap, unsigned long int pos){

    while(pos > 1 && (heap->arrayCost[pos] > heap->arrayCost[pos/2] || (heap->arrayCost[pos] == heap->arrayCost[pos/2] && heap->graphId[pos] > heap->graphId[pos/2]))){
        unsigned long int s = pos/2;
        maxHeapSwapElement(heap, pos, s);
        pos = pos/2;
    }

}

void addGraph(){

    unsigned long int key = djkRun();
    if(graphId >= maxNumberTopGraph){
        if(key < topKHeap->arrayCost[1]){
            maxHeapInsertDelete(topKHeap, key, graphId);
        }
    }
    else{
        addNewElementMaxHeap(topKHeap, key, graphId);
    }
    graphId++;

}

void showTopK(){
    unsigned long int i;
    for(i=1; i <= topKHeap->currentSize; i++){
        printf("%lu ", topKHeap->graphId[i]);
    }
    printf("\n");

}

void maxHeapSwapElement(maxHeap heap, unsigned long int f, unsigned long int s){

    unsigned long int elemF = heap->graphId[f];
    unsigned long int costF = heap->arrayCost[f];
    heap->graphId[f] = heap->graphId[s];
    heap->arrayCost[f] = heap->arrayCost[s];
    heap->arrayCost[s] = costF;
    heap->graphId[s] = elemF;

}


void maxHeapInsertDelete(maxHeap heap, unsigned long int key, unsigned long int prevElem){

    heap->arrayCost[1] = key;
    heap->graphId[1] = prevElem;
    maxHeapFixDown(heap, 1);

}
