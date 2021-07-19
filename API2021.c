#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#ifdef __clang__
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#elif __GNUC__
#pragma GCC diagnostic ignored "-Wunused-result"
#endif

typedef unsigned long long  ubig;
typedef unsigned long ul;

const unsigned long INF = 4294967295;

struct h {
    ubig distance;
    ul* elemArr;
    ubig* distanceArr;
    ul heapPosition;
    ul currentSize;
    bool inHeap;
    bool wasInHeap;
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
void removeMinHeap(minHeap heap);
void minHeapFixDown(minHeap heap, ul pos);

maxHeap createMaxHeap();
void addMaxHeapElement(ubig d, ul id);
void maxHeapFixUp(ul pos);
void maxHeapSwapElement(ul f, ul s);
void maxHeapInsertRemove(ubig d, ul id);
void printMaxHeap();

ubig djkRun();

void maxHeapFixDown(ul pos);

void addGraph();

void clearAll();

void initializeAll();
void freeAll();

int main() {

    char c[14];
    scanf("%lu %lu", &numberNode, &numberTopK);
    initializeAll();

    while (!feof(stdin)) {

        scanf("%s\n", c);

        if(!strcmp(c, "AggiungiGrafo")){
            addGraph();
        }
        else if(!strcmp(c, "TopK")) {
            printMaxHeap();
        }
    }
    freeAll();
    return 0;

}

void freeAll(){
    ul i;
    free(heap->head);
    free(heap);
    free(mHeap->distance);
    free(mHeap->graphId);
    free(mHeap);
    for(i=0; i<numberNode; i++){
        free(list[i]->elemArr);
        free(list[i]->distanceArr);
        free(list[i]);
    }
    free(list);
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
    if(currentId < numberTopK)
        addMaxHeapElement(d, currentId);
    else if(d < mHeap->distance[1])
        maxHeapInsertRemove(d, currentId);

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
    list->inHeap=true;
    list->wasInHeap=true;
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

    if(heap->currentSize>0){

        headList u = heap->head[1];
        u->inHeap=false;
        removeMinHeap(heap);
        return u;

    }

    return NULL;

}

void removeMinHeap(minHeap heap) {

    if(heap->currentSize>0){
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

ubig djkRun(){

    ul i, j;
    ubig value;
    ul nn = numberNode;

    for(i=0; i<nn; i++){
        for(j=0; j<nn-1; j++){
            scanf("%llu,", &value);
            if(value>0)
                addElemToList(list[i], value, j);
        }
        scanf("%llu", &value);
        if(value>0)
            addElemToList(list[i], value, nn-1);
    }
    list[0]->distance=0;
    addMinHeapElement(heap, list[0]);

    ubig sum=0;

    do{
        headList u = getMinHeapElement(heap);

        if(u->distance<INF)
            sum += u->distance;

        for(i=0; i<u->currentSize; i++){
            if(!list[u->elemArr[i]]->wasInHeap){
                list[u->elemArr[i]]->distance=u->distance + u->distanceArr[i];
                addMinHeapElement(heap, list[u->elemArr[i]]);
            }
            else if(list[u->elemArr[i]]->inHeap){
                ubig alt = u->distance + u->distanceArr[i];
                if(list[u->elemArr[i]]->distance > alt){
                    list[u->elemArr[i]]->distance=alt;
                    minHeapFixUp(heap, list[u->elemArr[i]]->heapPosition);
                }
            }
        }
    }while(heap->currentSize>0);

    clearAll();
    return sum;

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

void addMaxHeapElement(ubig d, ul id) {

    mHeap->currentSize++;
    mHeap->distance[mHeap->currentSize] = d;
    mHeap->graphId[mHeap->currentSize] = id;
    maxHeapFixUp(mHeap->currentSize);

}

void maxHeapFixUp(ul pos) {

    while(pos>1){
        ul sPos = pos / 2;
        if((mHeap->distance[pos] > mHeap->distance[pos/2]) || (mHeap->distance[pos]==mHeap->distance[pos/2] && mHeap->graphId[pos] > mHeap->graphId[pos/2])){
            maxHeapSwapElement(pos, sPos);
        }
        pos = sPos;
    }

}

void maxHeapSwapElement(ul f, ul s) {

    ubig fDistance = mHeap->distance[f];
    ul fId = mHeap->graphId[f];
    mHeap->distance[f] = mHeap->distance[s];
    mHeap->graphId[f] = mHeap->graphId[s];
    mHeap->distance[s] = fDistance;
    mHeap->graphId[s] = fId;

}

void maxHeapInsertRemove(ubig d, ul id) {

    mHeap->distance[1] = d;
    mHeap->graphId[1] = id;
    maxHeapFixDown(1);

}

void maxHeapFixDown(ul pos) {

    while(pos*2 <= mHeap->currentSize){
        ul sPos = pos * 2;
        if(sPos < mHeap->currentSize){
            if((mHeap->distance[sPos+1] > mHeap->distance[sPos]) || (mHeap->distance[sPos+1] == mHeap->distance[sPos] && mHeap->graphId[sPos+1] > mHeap->graphId[sPos])){
                sPos++;
            }
        }
        if((mHeap->distance[pos] > mHeap->distance[sPos]) || (mHeap->distance[pos] == mHeap->distance[sPos] && mHeap->graphId[pos] > mHeap->graphId[sPos])){
            return;
        }
        maxHeapSwapElement(pos, sPos);
        pos = sPos;

    }

}

void printMaxHeap() {
    ul s;
    for(s=1; s<mHeap->currentSize; s++){
        printf("%lu ", mHeap->graphId[s]);
    }
    printf("%lu", mHeap->graphId[mHeap->currentSize]);
    printf("\n");
}

