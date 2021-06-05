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
typedef struct node* minHeap;

uint rowLength;
uint graphId =0;
uint numberTopGraph;


minHeap createMinHeap();
void addNewSpaceMinHeap(minHeap heap);
void addNewElement(minHeap heap, uint key, uint prevElem);
void minHeapFixUp(minHeap heap, uint pos);
void minHeapSwapElement(minHeap heap, uint f, uint s);
void removeMinHeapElement(minHeap heap);
uint* getMinHeapElement(minHeap heap);    //int[0]=key, int[1]=elem
int minHeapEmpty(minHeap heap);
void minHeapFixDown(minHeap heap, uint pos);
void printHeap(minHeap heap);
uint djkRun();
void setup();



int main() {

    system("pause");
    return 0;
}


minHeap createMinHeap() {
    minHeap heap = (minHeap) malloc(sizeof (struct node));
    heap->MaxSize = 16;
    heap->currentSize = 0;
    heap->arrayCost = (uint*) malloc(sizeof (uint)*(heap->MaxSize+1));
    heap->arrayElem = (uint*) malloc(sizeof (uint)*(heap->MaxSize+1));

    return heap;
}

void addNewSpaceMinHeap(minHeap heap){
    int newSize = heap->MaxSize*2;

    heap->arrayCost = (uint*) realloc(heap->arrayCost, sizeof (uint)*(newSize+1));
    heap->arrayElem = (uint *) realloc(heap->arrayElem, sizeof(uint) * (newSize + 1));
    heap->MaxSize = newSize;
}

void addNewElement(minHeap heap, uint key, uint prevElem){
    if(heap->currentSize == heap->MaxSize)
        addNewSpaceMinHeap(heap);

    heap->currentSize++;
    heap->arrayCost[heap->currentSize] = key;
    heap->arrayElem[heap->currentSize] = prevElem;

    minHeapFixUp(heap, heap->currentSize);

}

void minHeapFixUp(minHeap heap, uint pos) {
    while(pos>1 && heap->arrayCost[pos] < heap->arrayCost[pos/2]){
        minHeapSwapElement(heap, pos/2, pos);
        pos = pos/2;
    }
}

void minHeapSwapElement(minHeap heap, uint f, uint s) {
    uint fatherCost = heap->arrayCost[f];
    uint fatherElem = heap->arrayElem[f];
    heap->arrayCost[f] = heap->arrayCost[s];
    heap->arrayElem[f] = heap->arrayElem[s];
    heap->arrayCost[s] = fatherCost;
    heap->arrayElem[s] = fatherElem;
}

uint* getMinHeapElement(minHeap heap) {
    uint* data = (uint*) malloc(sizeof (uint)*2);

    if(!minHeapEmpty(heap)){
        data[0] = heap->arrayCost[1];
        data[1] = heap->arrayElem[1];
        removeMinHeapElement(heap);
        return data;
    }

    return NULL;

}

int minHeapEmpty(minHeap heap) {
    return heap->currentSize<=0;
}

void removeMinHeapElement(minHeap heap) {
    if(!minHeapEmpty(heap)){
        minHeapSwapElement(heap, 1, heap->currentSize);
        heap->currentSize--;
        minHeapFixDown(heap, 1);
    }
}

void minHeapFixDown(minHeap heap, uint pos) {
    while(pos*2 <= heap->currentSize){
        int nPos = pos*2;
        if(nPos < heap->currentSize && heap->arrayCost[nPos+1] < heap->arrayCost[nPos])
            nPos++;    //going to the right child
        if(heap->arrayCost[pos] <= heap->arrayCost[nPos])
            break;      //have already swapped everything

        minHeapSwapElement(heap, pos, nPos);
        pos = nPos;
    }
}

void printHeap(minHeap heap) {
    for(int i=1; i<=heap->currentSize; i++)
        printf("%u ", heap->arrayCost[i]);
}

uint djkRun() {

    uint visitedNode[rowLength];
    uint distanceNode[rowLength];
    uint counter = 1;
    printf("Number of element: ");
    scanf("%u", &rowLength);
    uint matrix[rowLength][rowLength];

    minHeap heap = createMinHeap();

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
            addNewElement(heap, matrix[0][i], i);
        }
    }

    while (counter < rowLength && !minHeapEmpty(heap)) {
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
                addNewElement(heap, matrix[elem][i]+distance, i);
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

void setup() {

}

