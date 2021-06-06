#include <stdio.h>
#include <stdlib.h>


#define INFINITY 4294967295

typedef unsigned int uint;

struct node{
    uint* arrayCost;
    uint* arrayElem;
    uint currentSize;
    uint MaxSize;
};
typedef struct node* myHeap;

uint rowLength;
uint graphId =0;
uint maxNumberTopGraph;
uint currentNumberTopGraph=0;
char* topK = "TopK";
myHeap maxHeap;


myHeap createMinHeap();
void addNewSpaceMinHeap(myHeap heap);
void addNewElementMinHeap(myHeap heap, uint key, uint prevElem);
void minHeapFixUp(myHeap heap, uint pos);
void heapSwapElement(myHeap heap, uint f, uint s);
void removeMinHeapElement(myHeap heap);
uint* getMinHeapElement(myHeap heap);    //int[0]=key, int[1]=elem
int emptyHeap(myHeap heap);
void minHeapFixDown(myHeap heap, uint pos);
void printHeap(myHeap heap);
uint djkRun();
int compareString(char* string1, char* string2);

myHeap createMaxHeap();
void removeMaxHeapElement(myHeap heap);
void maxHeapFixDown(myHeap heap, uint pos);
void addNewElementMaxHeap(myHeap heap, uint key, uint prevElem);
void maxHeapFixUp(myHeap heap, uint pos);

void addGraph();
void showTopK();


int main() {
    scanf("%u %u", &rowLength, &maxNumberTopGraph);
    maxHeap = createMaxHeap();
    while(1){
        char command[14];
        scanf("%s", command);

        if(compareString(command, "AggiungiGrafo")){
            addGraph();
        }
        else if(compareString(command, "TopK")){
            showTopK();
        }
        else
            break;
    }
    return 0;
}


myHeap createMinHeap() {
    myHeap heap = (myHeap) malloc(sizeof (struct node));
    heap->MaxSize = 16;
    heap->currentSize = 0;
    heap->arrayCost = (uint*) malloc(sizeof (uint)*(heap->MaxSize+1));
    heap->arrayElem = (uint*) malloc(sizeof (uint)*(heap->MaxSize+1));

    return heap;
}

void addNewSpaceMinHeap(myHeap heap){
    int newSize = heap->MaxSize*2;

    heap->arrayCost = (uint*) realloc(heap->arrayCost, sizeof (uint)*(newSize+1));
    heap->arrayElem = (uint *) realloc(heap->arrayElem, sizeof(uint) * (newSize + 1));
    heap->MaxSize = newSize;
}

void addNewElementMinHeap(myHeap heap, uint key, uint prevElem){
    if(heap->currentSize == heap->MaxSize)
        addNewSpaceMinHeap(heap);

    heap->currentSize++;
    heap->arrayCost[heap->currentSize] = key;
    heap->arrayElem[heap->currentSize] = prevElem;

    minHeapFixUp(heap, heap->currentSize);

}

void minHeapFixUp(myHeap heap, uint pos) {
    while(pos>1 && heap->arrayCost[pos] < heap->arrayCost[pos/2]){
        heapSwapElement(heap, pos / 2, pos);
        pos = pos/2;
    }
}

void heapSwapElement(myHeap heap, uint f, uint s) {
    uint fatherCost = heap->arrayCost[f];
    uint fatherElem = heap->arrayElem[f];
    heap->arrayCost[f] = heap->arrayCost[s];
    heap->arrayElem[f] = heap->arrayElem[s];
    heap->arrayCost[s] = fatherCost;
    heap->arrayElem[s] = fatherElem;
}

uint* getMinHeapElement(myHeap heap) {
    uint* data = (uint*) malloc(sizeof (uint)*2);

    if(!emptyHeap(heap)){
        data[0] = heap->arrayCost[1];
        data[1] = heap->arrayElem[1];
        removeMinHeapElement(heap);
        return data;
    }

    return NULL;

}

int emptyHeap(myHeap heap) {
    return heap->currentSize<=0;
}

void removeMinHeapElement(myHeap heap) {
    if(!emptyHeap(heap)){
        heapSwapElement(heap, 1, heap->currentSize);
        heap->currentSize--;
        minHeapFixDown(heap, 1);
    }
}

void minHeapFixDown(myHeap heap, uint pos) {
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

void printHeap(myHeap heap) {
    for(int i=1; i<=heap->currentSize; i++)
        printf("Key: %u Elem: %u", heap->arrayCost[i], heap->arrayElem[i]);
    printf("\n");
}

uint djkRun() {

    uint visitedNode[rowLength];
    uint distanceNode[rowLength];
    uint counter = 1;
    uint matrix[rowLength][rowLength];

    myHeap heap = createMinHeap();

    for(int i=0; i<rowLength; i++){
        distanceNode[i]=INFINITY;
        visitedNode[i]=0;
        for(int j=0; j<rowLength; j++){
            scanf("%u,", &matrix[i][j]);
        }
    }

    distanceNode[0] = 0;
    visitedNode[0] = 1;

    for(int i=0; i<rowLength; i++){
        if(matrix[0][i] > 0){
            addNewElementMinHeap(heap, matrix[0][i], i);
        }
    }

    while (counter < rowLength && !emptyHeap(heap)) {
        uint* arr = getMinHeapElement(heap);
        uint distance = arr[0];
        uint elem = arr[1];
        free(arr);

        while(visitedNode[elem]==1){
            arr = getMinHeapElement(heap);
            distance = arr[0];
            elem = arr[1];
            free(arr);
        }

        //adding a fewer cost to the distanceNode array
        visitedNode[elem] = 1;
        distanceNode[elem] = distance;
        counter++;

        for(int i=0; i<rowLength; i++){
            if(visitedNode[i]!=1 && matrix[elem][i]>0){
                addNewElementMinHeap(heap, matrix[elem][i] + distance, i);
            }
        }
    }
    uint sum =0;
    for (int i = 0; i < rowLength; ++i) {
        if(distanceNode[i]==INFINITY)
            distanceNode[i]=0;
        sum = sum + distanceNode[i];
    }

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

myHeap createMaxHeap() {
    myHeap heap = (myHeap) malloc(sizeof (struct node));
    heap->MaxSize = maxNumberTopGraph;
    heap->currentSize = 0;
    heap->arrayCost = (uint*) malloc(sizeof (uint)*(heap->MaxSize+1));
    heap->arrayElem = (uint*) malloc(sizeof (uint)*(heap->MaxSize+1));

    return heap;
}

void removeMaxHeapElement(myHeap heap) {
    if(!emptyHeap(heap)){
        heapSwapElement(heap, 1, heap->currentSize);
        heap->currentSize--;
        maxHeapFixDown(heap, 1);
    }
}

void maxHeapFixDown(myHeap heap, uint pos) {
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

void addNewElementMaxHeap(myHeap heap, uint key, uint prevElem) {
    if(heap->currentSize == heap->MaxSize)
        addNewSpaceMinHeap(heap);   //should never be verified

    heap->currentSize++;
    heap->arrayCost[heap->currentSize] = key;
    heap->arrayElem[heap->currentSize] = prevElem;

    maxHeapFixUp(heap, heap->currentSize);
}

void maxHeapFixUp(myHeap heap, uint pos) {
    while(pos>1 && (heap->arrayCost[pos] > heap->arrayCost[pos/2] || (heap->arrayCost[pos] == heap->arrayCost[pos/2] && heap->arrayElem[pos] > heap->arrayElem[pos/2]))){
        heapSwapElement(heap, pos / 2, pos);
        pos = pos/2;
    }
}

void addGraph() {
    uint key = djkRun();
    if(currentNumberTopGraph < maxNumberTopGraph){
        addNewElementMaxHeap(maxHeap, key, currentNumberTopGraph);
    }
    else{
        if(key < maxHeap->arrayCost[maxHeap->currentSize]) {
            removeMaxHeapElement(maxHeap);
            addNewElementMaxHeap(maxHeap, key, currentNumberTopGraph);
        }
    }
    currentNumberTopGraph++;
}

void showTopK() {
    for(int i=1; i<=maxHeap->currentSize; i++)
        printf("%u ", maxHeap->arrayElem[i]);
    printf("\n");
}


