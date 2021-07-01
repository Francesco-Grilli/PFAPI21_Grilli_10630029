#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef unsigned long long int ubig;
typedef unsigned long int ulong;

const unsigned long long int INF = 1844674407370955161;

struct h {
    ubig distance;
    ulong* elemArr;
    ubig* distanceArr;
    ulong heapPosition;
    ulong currentSize;
    short int inHeap;
};
typedef struct h* headList;

struct l {
    headList* head;
    ulong currentSize;
};
typedef struct l* minHeap;

struct m {
    ulong currentSize;
    ubig* distance;
    ulong* graphId;
};
typedef struct m* maxHeap;

ulong numberNode;
ulong numberTopK;
ulong currentId=0;
maxHeap mHeap;

headList createList();
void addElemToList(headList head, ubig distance, ulong elem);
minHeap createMinHeap();
void addMinHeapElement(minHeap heap, headList list);
void minHeapFixUp(minHeap heap, ulong pos);
void minHeapSwapElement(minHeap heap, ulong f, ulong s);
headList getMinHeapElement(minHeap heap);
int minHeapNotEmpty(minHeap heap);
void removeMinHeap(minHeap heap);
void minHeapFixDown(minHeap heap, ulong pos);

maxHeap createMaxHeap();
void addMaxHeapElement(maxHeap heap, ubig d, ulong id);
void maxHeapFixUp(maxHeap heap, ulong pos);
void maxHeapSwapElement(maxHeap heap, ulong f, ulong s);
void maxHeapInsertRemove(maxHeap heap, ubig d, ulong id);
void printMaxHeap(maxHeap heap);

ubig djkRun();

void maxHeapFixDown(maxHeap heap, ulong pos);

void addGraph();

int main() {

    if (scanf("%lu %lu", &numberNode, &numberTopK)) {
        mHeap = createMaxHeap();

        while (!feof(stdin)) {

            char c[14];
            if (scanf("%s", c));

            if (feof(stdin))
                return 0;

            if (strcmp(c, "AggiungiGrafo") == 0)
                addGraph();
            else if (strcmp(c, "TopK") == 0)
                printMaxHeap(mHeap);
        }

        return 0;
    }
    else
        return -1;

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
    u->elemArr = (ulong*) malloc(sizeof (ulong) * numberNode);
    u->distanceArr = (ubig*) malloc(sizeof (ubig)*numberNode);
    u->currentSize=0;
    return u;

}

void addElemToList(headList head, ubig distance, ulong elem) {

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
    list->heapPosition=heap->currentSize;

    minHeapFixUp(heap, heap->currentSize);
}

void minHeapFixUp(minHeap heap, ulong pos) {
    while(pos>1 && heap->head[pos]->distance < heap->head[pos/2]->distance){
        minHeapSwapElement(heap, pos, pos/2);
        pos = pos/2;
    }
}

void minHeapSwapElement(minHeap heap, ulong f, ulong s) {

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

void minHeapFixDown(minHeap heap, ulong pos) {

    while(pos*2 <= heap->currentSize){
        ulong sPos = pos * 2;
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

int minHeapNotEmpty(minHeap heap) {
    if(heap->currentSize<=0)
        return 0;
    else
        return 1;
}

ubig djkRun() {

    headList* list = (headList*) malloc(sizeof (headList)*numberNode);
    minHeap heap = createMinHeap();

    ulong i, j;

    list[0] = createList();
    list[0]->distance=0;
    for(i=0; i<numberNode; i++){
        ubig d;
        if(scanf("%llu,", &d)) {
            if (d > 0)
                addElemToList(list[0], d, i);
        }
        else
            return -1;
    }
    addMinHeapElement(heap, list[0]);

    for(i=1; i<numberNode; i++){
        list[i] = createList();
        list[i]->distance=INF;
        for(j=0; j<numberNode; j++){
            ubig d;
            if(scanf("%llu,", &d)) {
                if (d > 0)
                    addElemToList(list[i], d, j);
            }
            else
                return -1;
        }
        addMinHeapElement(heap, list[i]);
    }

    ubig sum=0;

    while(minHeapNotEmpty(heap)==1){

        headList u = getMinHeapElement(heap);
        if(u->distance< INF)
            sum = sum + u->distance;

        for(i=0; i<u->currentSize; i++){
            if(list[u->elemArr[i]]->distance > (u->distance + u->distanceArr[i]) && list[u->elemArr[i]]->inHeap==1) {
                list[u->elemArr[i]]->distance = (u->distance + u->distanceArr[i]);
                minHeapFixUp(heap, list[u->elemArr[i]]->heapPosition);
            }
        }

    }

    for(i=0; i<numberNode; i++){
        free(list[i]->elemArr);
        free(list[i]->distanceArr);
        free(list[i]);
    }
    free(list);
    free(heap->head);
    free(heap);

    return sum;

}

maxHeap createMaxHeap() {
    maxHeap u = (maxHeap) malloc(sizeof (struct m));
    u->currentSize=0;
    u->distance = (ubig*) malloc(sizeof(ubig)*(numberTopK+1));
    u->graphId = (ulong*) malloc(sizeof (ulong) * (numberTopK + 1));

    return u;
}

void addMaxHeapElement(maxHeap heap, ubig d, ulong id) {

    heap->currentSize++;
    heap->distance[heap->currentSize] = d;
    heap->graphId[heap->currentSize] = id;
    maxHeapFixUp(heap, heap->currentSize);

}

void maxHeapFixUp(maxHeap heap, ulong pos) {

    while(pos>1){
        ulong sPos = pos / 2;
        if((heap->distance[pos] > heap->distance[pos/2]) || (heap->distance[pos]==heap->distance[pos/2] && heap->graphId[pos] > heap->graphId[pos/2])){
            maxHeapSwapElement(heap, pos, sPos);
        }
        pos = sPos;
    }

}

void maxHeapSwapElement(maxHeap heap, ulong f, ulong s) {

    ubig fDistance = heap->distance[f];
    ulong fId = heap->graphId[f];
    heap->distance[f] = heap->distance[s];
    heap->graphId[f] = heap->graphId[s];
    heap->distance[s] = fDistance;
    heap->graphId[s] = fId;

}

void maxHeapInsertRemove(maxHeap heap, ubig d, ulong id) {

    if(heap->distance[1]>d) {
        heap->distance[1] = d;
        heap->graphId[1] = id;
        maxHeapFixDown(heap, 1);
    }

}

void maxHeapFixDown(maxHeap heap, ulong pos) {

    while(pos*2 <= heap->currentSize){
        ulong sPos = pos * 2;
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
    ulong s;
    for(s=1; s<=heap->currentSize; s++){
        printf("%lu ", heap->graphId[s]);
    }
    printf("\n");
}


