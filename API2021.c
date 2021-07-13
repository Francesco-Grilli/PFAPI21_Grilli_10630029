#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

typedef unsigned long int ubig;
typedef unsigned int ul;

const unsigned long  int INF = 4294967295;

struct h {
    ubig distance;
    ul* elemArr;
    ubig* distanceArr;
    ul heapPosition;
    ul currentSize;
    short int inHeap;
    short int wasInHeap;
};
typedef struct h* headList;

struct l {
    headList* head;
    ul currentSize;
};
typedef struct l* minHeap;

struct m {
    ul currentSize;
    ubig* distance;
    ul* graphId;
};
typedef struct m* maxHeap;

ul numberNode;
ul numberTopK;
ul currentId=0;
maxHeap mHeap;
headList* list;
minHeap heap;

headList createList();
void addElemToList(headList head, ubig distance, ul elem);
minHeap createMinHeap();
void addMinHeapElement(minHeap heap, headList list);
void minHeapFixUp(minHeap heap, ul pos);
void minHeapSwapElement(minHeap heap, ul f, ul s);
headList getMinHeapElement(minHeap heap);
short int minHeapNotEmpty(minHeap heap);
void removeMinHeap(minHeap heap);
void minHeapFixDown(minHeap heap, ul pos);

maxHeap createMaxHeap();
void addMaxHeapElement(maxHeap heap, ubig d, ul id);
void maxHeapFixUp(maxHeap heap, ul pos);
void maxHeapSwapElement(maxHeap heap, ul f, ul s);
void maxHeapInsertRemove(maxHeap heap, ubig d, ul id);
void printMaxHeap(maxHeap heap);

ubig djkRun();

void maxHeapFixDown(maxHeap heap, ul pos);

void addGraph();

void clearAll();

void initializeAll();

int main() {

    if (scanf("%u %u", &numberNode, &numberTopK)) {

        initializeAll();

        while (!feof(stdin)) {

            char c[14];
            if (scanf("%s", c)) {

                if (feof(stdin))
                    return 0;

                if (strcmp(c, "AggiungiGrafo") == 0)
                    addGraph();
                else if (strcmp(c, "TopK") == 0)
                    printMaxHeap(mHeap);
            }
            else
                return -1;
        }

        return 0;
    }
    else
        return -1;

}

void initializeAll() {
    list = (headList*) malloc(sizeof (headList)*numberNode);
    ul i;
    for(i=0; i<numberNode; i++)
        list[i]=createList();
    heap = createMinHeap();
    mHeap = createMaxHeap();
}

void addGraph() {

    ubig d = djkRun();
    if(currentId < numberTopK){
        addMaxHeapElement(mHeap, d, currentId);
    }
    else
        maxHeapInsertRemove(mHeap, d, currentId);
    currentId++;

}


headList createList(){

    headList u = (headList) malloc(sizeof (struct h));
    u->elemArr = (ul*) malloc(sizeof (ul) * numberNode);
    u->distanceArr = (ubig*) malloc(sizeof (ubig)*numberNode);
    u->currentSize=0;
    u->wasInHeap=0;
    return u;

}

void addElemToList(headList head, ubig distance, ul elem) {

    head->distanceArr[head->currentSize] = distance;
    head->elemArr[head->currentSize] = elem;
    head->currentSize++;

}

minHeap createMinHeap() {
    minHeap u = (minHeap) malloc(sizeof (struct l));
    u->head = (headList*) malloc(sizeof (headList)*(numberNode+1));
    u->currentSize=0;
    return u;
}

void addMinHeapElement(minHeap heap, headList list) {
    heap->currentSize++;
    heap->head[heap->currentSize] = list;
    list->inHeap=1;
    list->wasInHeap=1;
    list->heapPosition=heap->currentSize;

    minHeapFixUp(heap, heap->currentSize);
}

void minHeapFixUp(minHeap heap, ul pos) {
    while(pos>1 && heap->head[pos]->distance < heap->head[pos/2]->distance){
        minHeapSwapElement(heap, pos, pos/2);
        pos = pos/2;
    }
}

void minHeapSwapElement(minHeap heap, ul f, ul s) {

    headList father = heap->head[f];
    heap->head[f] = heap->head[s];
    heap->head[f]->heapPosition = f;
    heap->head[s] = father;
    heap->head[s]->heapPosition = s;

}

headList getMinHeapElement(minHeap heap) {

    if(minHeapNotEmpty(heap)==1){

        headList u = heap->head[1];
        u->inHeap=0;
        removeMinHeap(heap);
        return u;

    }

    return NULL;

}

void removeMinHeap(minHeap heap) {

    if(minHeapNotEmpty(heap)==1){
        if(heap->currentSize>1)
            minHeapSwapElement(heap, 1, heap->currentSize);
        heap->currentSize--;
        minHeapFixDown(heap, 1);
    }

}

void minHeapFixDown(minHeap heap, ul pos) {

    while(pos*2 <= heap->currentSize){
        ul sPos = pos * 2;
        if(sPos<heap->currentSize){
            if(heap->head[sPos+1]->distance < heap->head[sPos]->distance){
                sPos++;
            }
        }
        if(heap->head[pos]->distance < heap->head[sPos]->distance) {
            return; //no more element to swap, father is already minimum
        }

        minHeapSwapElement(heap, pos, sPos);
        pos = sPos;

    }

}

short int minHeapNotEmpty(minHeap heap) {
    if(heap->currentSize<=0)
        return 0;
    else
        return 1;
}


ubig djkRun(){

    bool seen[numberNode];
    ul i, j;
    ubig d;

    for(i=0; i<numberNode; i++){

        if(scanf("%lu,", &d));
        for(j=1; j<numberNode; j++){
            if(scanf("%lu,", &d));
            if(d>0 && i!=j)
                addElemToList(list[i], d, j);
        }
        seen[i]=false;
    }
    list[0]->distance=0;
    addMinHeapElement(heap, list[0]);


}

void clearAll() {

    ul i;
    for(i=0; i<numberNode; i++){
        list[i]->currentSize=0;
        list[i]->wasInHeap=0;
    }
    heap->currentSize=0;

}

maxHeap createMaxHeap() {
    maxHeap u = (maxHeap) malloc(sizeof (struct m));
    u->currentSize=0;
    u->distance = (ubig*) malloc(sizeof(ubig)*(numberTopK+1));
    u->graphId = (ul*) malloc(sizeof (ul) * (numberTopK + 1));

    return u;
}

void addMaxHeapElement(maxHeap heap, ubig d, ul id) {

    heap->currentSize++;
    heap->distance[heap->currentSize] = d;
    heap->graphId[heap->currentSize] = id;
    maxHeapFixUp(heap, heap->currentSize);

}

void maxHeapFixUp(maxHeap heap, ul pos) {

    while(pos>1){
        ul sPos = pos / 2;
        if((heap->distance[pos] > heap->distance[pos/2]) || (heap->distance[pos]==heap->distance[pos/2] && heap->graphId[pos] > heap->graphId[pos/2])){
            maxHeapSwapElement(heap, pos, sPos);
        }
        pos = sPos;
    }

}

void maxHeapSwapElement(maxHeap heap, ul f, ul s) {

    ubig fDistance = heap->distance[f];
    ul fId = heap->graphId[f];
    heap->distance[f] = heap->distance[s];
    heap->graphId[f] = heap->graphId[s];
    heap->distance[s] = fDistance;
    heap->graphId[s] = fId;

}

void maxHeapInsertRemove(maxHeap heap, ubig d, ul id) {

    if(heap->distance[1]>d) {
        heap->distance[1] = d;
        heap->graphId[1] = id;
        maxHeapFixDown(heap, 1);
    }

}

void maxHeapFixDown(maxHeap heap, ul pos) {

    while(pos*2 <= heap->currentSize){
        ul sPos = pos * 2;
        if(sPos < heap->currentSize){
            if((heap->distance[sPos+1] > heap->distance[sPos]) || (heap->distance[sPos+1] == heap->distance[sPos] && heap->graphId[sPos+1] > heap->graphId[sPos])){
                sPos++;
            }
        }
        if((heap->distance[pos] > heap->distance[sPos]) || (heap->distance[pos] == heap->distance[sPos] && heap->graphId[pos] > heap->graphId[sPos])){
            return;
        }
        maxHeapSwapElement(heap, pos, sPos);
        pos = sPos;

    }

}

void printMaxHeap(maxHeap heap) {
    ul s;
    for(s=1; s<=heap->currentSize; s++){
        printf("%u", heap->graphId[s]);
        if(s<heap->currentSize)
            printf(" ");
    }
    printf("\n");
}
