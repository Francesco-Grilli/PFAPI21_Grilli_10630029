#include <stdio.h>
#include <stdlib.h>


#define INFINITY 4294967295

struct node{
    unsigned int* arrayCost;
    unsigned int* arrayElem;
    unsigned int currentSize;
    unsigned int MaxSize;
};
typedef struct node* minHeap;


minHeap createMinHeap();
void addNewSpaceMinHeap(minHeap);
void insertElement(minHeap, unsigned int, unsigned int);
void minHeapFixUp(minHeap, unsigned int);
void minHeapSwapElement(minHeap, unsigned int, unsigned int);
void removeMinHeapElement(minHeap);
unsigned int* getMinHeapElement(minHeap);    //int[0]=key, int[1]=elem
int minHeapEmpty(minHeap);
void minHeapFixDown(minHeap, unsigned int);



int main() {
    printf("Hello, World!\n");
    unsigned int i = 4294967295;

    return 0;
}


minHeap createMinHeap() {
    minHeap heap = (minHeap) malloc(sizeof (struct node));
    heap->MaxSize = 16;
    heap->currentSize = 0;
    heap->arrayCost = (unsigned int*) malloc(sizeof (unsigned int)*(heap->MaxSize+1));
    heap->arrayElem = (unsigned int*) malloc(sizeof (unsigned int)*(heap->MaxSize+1));

    return heap;
}

void addNewSpaceMinHeap(minHeap heap){
    int newSize = heap->MaxSize*2;

    heap->arrayCost = (unsigned int*) realloc(heap->arrayCost, sizeof (unsigned int)*(newSize+1));
    heap->arrayElem = (unsigned int *) realloc(heap->arrayElem, sizeof(unsigned int) * (newSize + 1));
    heap->MaxSize = newSize;
}

void insertElement(minHeap heap, unsigned int key, unsigned int prevElem){
    if(heap->currentSize == heap->MaxSize)
        addNewSpaceMinHeap(heap);

    heap->arrayCost[heap->currentSize] = key;
    heap->arrayElem[++heap->currentSize] = prevElem;

    minHeapFixUp(heap, heap->currentSize);

}

void minHeapFixUp(minHeap heap, unsigned int pos) {
    while(pos>=1 && heap->arrayCost[pos/2] <= heap->arrayCost[pos]){
        minHeapSwapElement(heap, pos/2, pos);
        pos = pos/2;
    }
}

void minHeapSwapElement(minHeap heap, unsigned int f, unsigned int s) {
    unsigned int fatherCost = heap->arrayCost[f];
    unsigned int fatherElem = heap->arrayElem[f];
    heap->arrayCost[f] = heap->arrayCost[s];
    heap->arrayElem[f] = heap->arrayElem[s];
    heap->arrayCost[s] = fatherCost;
    heap->arrayElem[s] = fatherElem;
}

unsigned int* getMinHeapElement(minHeap heap) {
    unsigned int* data = (unsigned int*) malloc(sizeof (unsigned int)*2);

    if(!minHeapEmpty(heap)){
        data[0] = heap->arrayCost[1];
        data[1] = heap->arrayElem[1];
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

void minHeapFixDown(minHeap heap, unsigned int pos) {
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

