#include <stdio.h>
#include <stdlib.h>


#define INFINITY 4294967295

typedef unsigned int uint;

struct adjNode{
    uint distance;
    uint elem;
    struct adjNode* next;
};
typedef struct adjNode* listNode;

struct h{
    uint distance;
    listNode point;
};
typedef struct h* headList;

struct minHeapNode{
    headList *head;
    uint currentSize;
    uint maxSize;
};
typedef struct minHeapNode* minHeap;

struct maxHeapNode{
    uint* arrayCost;
    uint* arrayElem;
    uint currentSize;
    uint maxSize;
};
typedef struct maxHeapNode* maxHeap;

uint rowLength;
uint maxNumberTopGraph;
uint currentNumberTopGraph=0;
maxHeap topKHeap;


minHeap createMinHeap();
void addNewSpaceMinHeap(maxHeap heap);
void addNewElementMinHeap(minHeap heap, headList head);
void minHeapFixUp(minHeap heap, uint pos);
void minHeapSwapElement(minHeap heap, uint f, uint s);
void removeMinHeapElement(maxHeap heap);
headList getMinHeapElement(minHeap heap);
int emptyMinHeap(minHeap heap);
void minHeapFixDown(maxHeap heap, uint pos);
void printHeap(maxHeap heap);
uint djkRun();
int compareString(char* string1, char* string2);

maxHeap createMaxHeap();
void removeMaxHeapElement(maxHeap heap);
void maxHeapFixDown(maxHeap heap, uint pos);
void addNewElementMaxHeap(maxHeap heap, uint key, uint prevElem);
void maxHeapFixUp(maxHeap heap, uint pos);

void addGraph();
void showTopK();


int main() {
    scanf("%u %u", &rowLength, &maxNumberTopGraph);
    topKHeap = createMaxHeap();
    while(1){
        char command[14];
        scanf("%s", command);

        if(feof(stdin))
            return 0;
        if(compareString(command, "AggiungiGrafo")){
            addGraph();
        }
        else if(compareString(command, "TopK")){
            showTopK();
        }
    }
    return 0;
}


minHeap createMinHeap() {
    minHeap heap = (minHeap) malloc(sizeof(struct minHeapNode));
    heap->maxSize = rowLength;
    heap->currentSize = 0;
    heap->head = (headList *) (headList) malloc(sizeof(headList) * rowLength);

    return heap;
}


void addNewElementMinHeap(minHeap heap, headList head){

    heap->currentSize++;
    heap->head[heap->currentSize] = head;

    minHeapFixUp(heap, heap->currentSize);

}

void minHeapFixUp(minHeap heap, uint pos) {
    while(pos>1 && heap->head[pos]->distance < heap->head[pos/2]->distance){
        minHeapSwapElement(heap, pos / 2, pos);
        pos = pos/2;
    }
}

void heapSwapElement(minHeap heap, uint f, uint s) {
    headList father = heap->head[f];
    heap->head[f] = heap->head[s];
    heap->head[s] = father;
}

headList getMinHeapElement(minHeap heap) {

    if(!emptyMinHeap(heap)){
        headList h = heap->head[1];
        removeMinHeapElement(heap);
        return h;
    }

    return NULL;

}

int emptyHeap(minHeap heap) {
    return heap->currentSize<=0;
}//arrivato qui

void removeMinHeapElement(maxHeap heap) {
    if(!emptyHeap(heap)){
        heapSwapElement(heap, 1, heap->currentSize);
        heap->currentSize--;
        minHeapFixDown(heap, 1);
    }
}

void minHeapFixDown(maxHeap heap, uint pos) {
    while(pos*2 <= heap->currentSize){
        int nPos = pos*2;
        if(nPos < heap->currentSize && heap->arrayCost[nPos+1] < heap->arrayCost[nPos])
            nPos++;    //going to the right child
        if(heap->arrayCost[pos] <= heap->arrayCost[nPos])
            break;      //have already swapped everything

        heapSwapElement(heap, pos, nPos);
        pos = nPos;
    }
}

void printHeap(maxHeap heap) {
    for(int i=1; i<=heap->currentSize; i++)
        printf("Key: %u Elem: %u", heap->arrayCost[i], heap->arrayElem[i]);
    printf("\n");
}

uint djkRun() {

    uint visitedNode[rowLength];
    uint distanceNode[rowLength];
    uint counter = 1;
    uint matrix[rowLength][rowLength];

    maxHeap heap = createMinHeap();

    for(int i=0; i<rowLength; i++){
        distanceNode[i]=INFINITY;
        visitedNode[i]=0;
        for(int j=0; j<rowLength; j++){
            scanf("%u,", &matrix[i][j]);
        }
    }

    distanceNode[0] = 0;
    visitedNode[0] = 1;

    for(int i=1; i<rowLength; i++){
        if(matrix[0][i] > 0){
            addNewElementMinHeap(heap, matrix[0][i], i);
        }
    }

    while (counter < rowLength && !emptyHeap(heap)) {
        uint* arr = getMinHeapElement(heap);
        uint distance = arr[0];
        uint elem = arr[1];
        free(arr);

        while(visitedNode[elem] == 1 && !emptyHeap(heap)){
            arr = getMinHeapElement(heap);
            distance = arr[0];
            elem = arr[1];
            free(arr);
        }
        if(!emptyHeap(heap)) {


            //adding a fewer cost to the distanceNode array
            visitedNode[elem] = 1;
            distanceNode[elem] = distance;
            counter++;

            for (int i = 1; i < rowLength; i++) {
                if (visitedNode[i] != 1 && matrix[elem][i] > 0) {
                    addNewElementMinHeap(heap, matrix[elem][i] + distance, i);
                }
            }
        }
    }
    uint sum =0;
    for (int i = 1; i < rowLength; i++) {
        if(distanceNode[i]==INFINITY)
            distanceNode[i]=0;
        sum = sum + distanceNode[i];
    }

    free(heap->arrayCost);
    free(heap->arrayElem);
    free(heap);

    return sum;
}

int compareString(char *string1, char *string2) {
    int i=0;
    int ret = 0;

    while(string1[i] == string2[i] && ret == 0 ){
        if(string1[i] == '\0' || string2[i] == '\0'){
            ret = 1;
        }
        i++;
    }

    return ret;
}

maxHeap createMaxHeap() {
    maxHeap heap = (maxHeap) malloc(sizeof (struct maxHeapNode));
    heap->maxSize = maxNumberTopGraph;
    heap->currentSize = 0;
    heap->arrayCost = (uint*) malloc(sizeof (uint)*(heap->maxSize + 1));
    heap->arrayElem = (uint*) malloc(sizeof (uint)*(heap->maxSize + 1));

    return heap;
}

void removeMaxHeapElement(maxHeap heap) {
    if(!emptyHeap(heap)){
        heapSwapElement(heap, 1, heap->currentSize);
        heap->currentSize--;
        maxHeapFixDown(heap, 1);
    }
}

void maxHeapFixDown(maxHeap heap, uint pos) {
    while(pos*2 <= heap->currentSize){
        int nPos = pos*2;
        if(nPos < heap->currentSize && heap->arrayCost[nPos+1] > heap->arrayCost[nPos])
            nPos++; //if the right child is bigger, going to the right child
        else if(heap->arrayCost[nPos + 1] == heap->arrayCost[nPos] && heap->arrayElem[nPos + 1] > heap->arrayElem[nPos])
            nPos++;
        if(heap->arrayCost[pos] > heap->arrayCost[nPos]){
            break;      //have already swapped everything
        }
        else if(heap->arrayCost[pos] == heap->arrayCost[nPos] && heap->arrayElem[pos] > heap->arrayElem[nPos])
            break;

        heapSwapElement(heap, pos, nPos);
        pos = nPos;
    }
}

void addNewElementMaxHeap(maxHeap heap, uint key, uint prevElem) {
    if(heap->currentSize == heap->maxSize)
        addNewSpaceMinHeap(heap);   //should never be verified

    heap->currentSize++;
    heap->arrayCost[heap->currentSize] = key;
    heap->arrayElem[heap->currentSize] = prevElem;

    maxHeapFixUp(heap, heap->currentSize);
}

void maxHeapFixUp(maxHeap heap, uint pos) {
    while(pos>1 && (heap->arrayCost[pos] > heap->arrayCost[pos/2] || (heap->arrayCost[pos] == heap->arrayCost[pos/2] && heap->arrayElem[pos] > heap->arrayElem[pos/2]))){
        heapSwapElement(heap, pos / 2, pos);
        pos = pos/2;
    }
}

void addGraph() {
    uint key = djkRun();
    if(currentNumberTopGraph < maxNumberTopGraph){
        addNewElementMaxHeap(topKHeap, key, currentNumberTopGraph);
    }
    else{
        if(key < topKHeap->arrayCost[1]) {
            removeMaxHeapElement(topKHeap);
            addNewElementMaxHeap(topKHeap, key, currentNumberTopGraph);
        }
    }
    currentNumberTopGraph++;
}

void showTopK() {
    for(int i=1; i <= topKHeap->currentSize; i++)
        printf("%u ", topKHeap->arrayElem[i]);
    printf("\n");
}

uint djkRun2() {

    minHeap heap = createMinHeap();
    headList adjList[rowLength];

    for(int i=0; i<rowLength; i++){
        adjList[i] = (headList) malloc(sizeof (headList));
        adjList[i]->point = NULL;
        adjList[i]->distance=INFINITY;

        for(int j=0; j<rowLength; j++){
            if(i!=j && j!=0){
                uint d;
                scanf("%u,", &d);
                listNode toAdd = (listNode) malloc(sizeof (listNode));
                toAdd->next=adjList[i]->point;
                adjList[i]->point = toAdd;
                toAdd->distance = d;
                toAdd->elem = j;
            }
            else {
                uint d;
                scanf("%u,",&d);
            }
        }
    }

    distanceNode[0] = 0;
    visitedNode[0] = 1;

    for(int i=1; i<rowLength; i++){
        if(matrix[0][i] > 0){
            addNewElementMinHeap(heap, matrix[0][i], i);
        }
    }

    while (counter < rowLength && !emptyHeap(heap)) {
        uint* arr = getMinHeapElement(heap);
        uint distance = arr[0];
        uint elem = arr[1];
        free(arr);

        while(visitedNode[elem] == 1 && !emptyHeap(heap)){
            arr = getMinHeapElement(heap);
            distance = arr[0];
            elem = arr[1];
            free(arr);
        }
        if(!emptyHeap(heap)) {


            //adding a fewer cost to the distanceNode array
            visitedNode[elem] = 1;
            distanceNode[elem] = distance;
            counter++;

            for (int i = 1; i < rowLength; i++) {
                if (visitedNode[i] != 1 && matrix[elem][i] > 0) {
                    addNewElementMinHeap(heap, matrix[elem][i] + distance, i);
                }
            }
        }
    }
    uint sum =0;
    for (int i = 1; i < rowLength; i++) {
        if(distanceNode[i]==INFINITY)
            distanceNode[i]=0;
        sum = sum + distanceNode[i];
    }

    free(heap->arrayCost);
    free(heap->arrayElem);
    free(heap);

    return sum;
}


