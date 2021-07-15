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
bool moreThenK=false;

headList createList(ul nn);
void addElemToList(headList head, ubig distance, ul elem);
minHeap createMinHeap(ul nn);
void addMinHeapElement(minHeap tHeap, headList list);
void minHeapFixUp(minHeap tHeap, ul pos);
void minHeapSwapElement(minHeap tHeap, ul f, ul s);
headList getMinHeapElement(minHeap tHeap);
void removeMinHeap(minHeap tHeap);
void minHeapFixDown(minHeap tHeap, ul pos);
bool emptyMinHeap(minHeap tHeap);

maxHeap createMaxHeap(ul nk);
void addMaxHeapElement(maxHeap heap, ubig d, ul id);
void maxHeapFixUp(maxHeap heap, ul pos);
void maxHeapSwapElement(maxHeap heap, ul f, ul s);
void maxHeapInsertRemove(maxHeap heap, ubig d, ul id);
void printMaxHeap(maxHeap heap);

ubig djkRun(minHeap tHeap, headList* list, ul nn);
void maxHeapFixDown(maxHeap heap, ul pos);
void addGraph(ul nn, ul nk);
void clearAll(ul nn);
void initializeAll(ul nn, ul nk);
void buildMaxHeap(maxHeap heap, ul nk);
void addFastMaxHeapElement(maxHeap heap, ubig d, ul id);

int main() {

    if (scanf("%u %u", &numberNode, &numberTopK)) {

        initializeAll(numberNode, numberTopK);

        char c[14];

        while (!feof(stdin)) {

            if (scanf("%s", c)) {

                if (feof(stdin))
                    return 0;

                if (strcmp(c, "AggiungiGrafo") == 0)
                    addGraph(numberNode, numberTopK);
                else
                    printMaxHeap(mHeap);
            }
        }

        return 0;
    }
    else
        return -1;

}

void initializeAll(ul nn, ul nk) {
    list = (headList*) malloc(sizeof (headList)*nn);
    ul i;
    for(i=0; i<nn; i++)
        list[i]=createList(nn);
    heap = createMinHeap(nn);
    mHeap = createMaxHeap(nk);
}

void addGraph(ul nn, ul nk) {

    ubig d = djkRun(heap, list, nn);

    if(currentId < nk)
        addFastMaxHeapElement(mHeap, d, currentId);
    else if(moreThenK) {
        if (d < mHeap->distance[1])
            maxHeapInsertRemove(mHeap, d, currentId);
    }
    else
        buildMaxHeap(mHeap, nk);

    currentId++;

}


headList createList(ul nn){

    headList u = (headList) malloc(sizeof (struct h));
    u->elemArr = (ul*) malloc(sizeof (ul) * nn);
    u->distanceArr = (ubig*) malloc(sizeof (ubig)*nn);
    u->currentSize=0;
    u->wasInHeap=0;
    return u;

}

void addElemToList(headList head, ubig distance, ul elem) {

    head->distanceArr[head->currentSize] = distance;
    head->elemArr[head->currentSize] = elem;
    head->currentSize++;

}

minHeap createMinHeap(ul nn) {
    minHeap u = (minHeap) malloc(sizeof (struct l));
    u->head = (headList*) malloc(sizeof (headList)*(nn+1));
    u->currentSize=0;
    return u;
}

void addMinHeapElement(minHeap tHeap, headList list) {
    tHeap->currentSize++;
    tHeap->head[tHeap->currentSize] = list;
    list->inHeap=true;
    list->wasInHeap=true;
    list->heapPosition=tHeap->currentSize;

    minHeapFixUp(tHeap, tHeap->currentSize);
}

void minHeapFixUp(minHeap tHeap, ul pos) {
    while(pos>1 && tHeap->head[pos]->distance < tHeap->head[pos/2]->distance){
        minHeapSwapElement(tHeap, pos, pos/2);
        pos = pos/2;
    }
}

void minHeapSwapElement(minHeap tHeap, ul f, ul s) {

    headList father = tHeap->head[f];
    tHeap->head[f] = tHeap->head[s];
    tHeap->head[f]->heapPosition = f;
    tHeap->head[s] = father;
    tHeap->head[s]->heapPosition = s;

}

headList getMinHeapElement(minHeap tHeap) {


        headList u = tHeap->head[1];
        u->inHeap=false;
        removeMinHeap(tHeap);
        return u;

}

void removeMinHeap(minHeap tHeap) {

    if(tHeap->currentSize>1)
        minHeapSwapElement(tHeap, 1, tHeap->currentSize);
    tHeap->currentSize--;
    minHeapFixDown(tHeap, 1);

}

void minHeapFixDown(minHeap tHeap, ul pos) {

    while(pos*2 <= tHeap->currentSize){
        ul sPos = pos * 2;
        if(sPos<tHeap->currentSize){
            if(tHeap->head[sPos+1]->distance < tHeap->head[sPos]->distance){
                sPos++;
            }
        }
        if(tHeap->head[pos]->distance < tHeap->head[sPos]->distance) {
            return; //no more element to swap, father is already minimum
        }

        minHeapSwapElement(tHeap, pos, sPos);
        pos = sPos;

    }

}


ubig djkRun(minHeap tHeap, headList* tList, ul nn){

    ul i, j;
    ubig value;
    ubig start;
    bool allEquals=true;
    bool allZero=true;

    if(scanf("%lu,", &start));
    for(i=1; i<nn; i++){

        if(scanf("%lu,", &value));
        if(value>0) {
            addElemToList(tList[0], value, i);
            allZero=false;
        }
        if(value!=start)
            allEquals=false;

    }

    if(allZero){
        for(i=1; i<nn; i++) {
            for (j = 0; j < nn; j++) {
                if(scanf("%lu,", &value));
            }
        }
        clearAll(nn);
        return 0;

    }

    tList[0]->distance=0;
    addMinHeapElement(tHeap, tList[0]);

    for(i=1; i<nn; i++){

        for(j=0; j<nn; j++){

            if(scanf("%lu,", &value));
            if(value>0 && i!=j)
                addElemToList(tList[i], value, j);
            if(value!=start)
                allEquals=false;

        }

        tList[i]->distance=INF;
    }

    if(allEquals){
        clearAll(nn);
        return value*(nn-1);
    }

    ubig sum=0;

    do{
        headList u = getMinHeapElement(tHeap);

        if(u->distance<INF) {
            sum += u->distance;
        }

        for(i=0; i<u->currentSize; i++){
            if(!tList[u->elemArr[i]]->wasInHeap){
                tList[u->elemArr[i]]->distance=u->distance + u->distanceArr[i];
                addMinHeapElement(tHeap, tList[u->elemArr[i]]);
            }
            else if(tList[u->elemArr[i]]->inHeap){
                ubig alt = u->distance + u->distanceArr[i];
                if(tList[u->elemArr[i]]->distance > alt){
                    tList[u->elemArr[i]]->distance=alt;
                    minHeapFixUp(tHeap, tList[u->elemArr[i]]->heapPosition);
                }
            }

        }
    }while(tHeap->currentSize>0);

    clearAll(nn);
    return sum;

}

void clearAll(ul nn) {

    ul i;
    for(i=0; i<nn; i++){
        list[i]->currentSize=0;
        list[i]->wasInHeap=0;
    }
    heap->currentSize=0;

}

maxHeap createMaxHeap(ul nk) {
    maxHeap u = (maxHeap) malloc(sizeof (struct m));
    u->currentSize=0;
    u->distance = (ubig*) malloc(sizeof(ubig)*(nk+1));
    u->graphId = (ul*) malloc(sizeof (ul) * (nk + 1));

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

    heap->distance[1] = d;
    heap->graphId[1] = id;
    maxHeapFixDown(heap, 1);

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
    for(s=1; s<heap->currentSize; s++){
        printf("%u ", heap->graphId[s]);
    }
    printf("%u", heap->graphId[heap->currentSize]);
    printf("\n");
}


bool emptyMinHeap(minHeap heap) {
    if(heap->currentSize>0)
        return false;

    return true;
}

void buildMaxHeap(maxHeap heap, ul nk) {
    ul i;

    for(i=(nk+1)/2; i>0; i--){
        maxHeapFixDown(heap, i);
    }
    moreThenK=true;
}

void addFastMaxHeapElement(maxHeap heap, ubig d, ul id) {
    heap->currentSize++;
    heap->distance[heap->currentSize] = d;
    heap->graphId[heap->currentSize] = id;
}

