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

    if(minHeapNotEmpty(heap)==1){

        headList u = heap->head[1];
        u->inHeap=false;
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

/*ubig djkRun() {

    ul i, j;
    int allEquals=1;
    int allZero=1;

    list[0]->distance=0;
    ubig value;
    if(scanf("%lu,", &value));

    if(scanf("%lu,", &value)){
        if (value > 0) {
            addElemToList(list[0], value, 1);
            allZero=0;
        }
    }

    for(i=2; i<numberNode; i++){
        ubig d;
        if(scanf("%lu,", &d)) {
            if(d!=value)
                allEquals=0;
            if (d > 0) {
                addElemToList(list[0], d, i);
                allZero=0;
            }
        }
        else
            return -1;
    }

    if(allZero==1){
        for(i=1; i<numberNode; i++){
            for(j=0; j<numberNode; j++){
                if(scanf("%lu,", &value));
            }
        }
        clearAll();
        return 0;
    }
    else{
        addMinHeapElement(heap, list[0]);
    }

    for(i=1; i<numberNode; i++){
        list[i]->distance=INF;
        for(j=0; j<numberNode; j++){
            ubig d;
            if(scanf("%lu,", &d)) {
                if(d!=value)
                    allEquals=0;

                if (d > 0 && j!=0 && j!=i)
                    addElemToList(list[i], d, j);
            }
            else
                return -1;
        }
    }
    if(allEquals==1){
        clearAll();
        return value*(numberNode-1);
    }

    ul k=0;
    i=0;
    while(i<numberNode){
        for(k=0; k<numberNode; k++) {
            for (j = 0; j < list[k]->currentSize; j++) {
                if (list[list[k]->elemArr[j]]->inHeap == 0) {
                    addMinHeapElement(heap, list[list[k]->elemArr[j]]);
                    i++;
                    if (i >= numberNode)
                        break;
                }
            }
        }
        break;
    }

    ubig sum=0;

    while(heap->currentSize>0){

        headList u = getMinHeapElement(heap);
        if(u->distance< INF)
            sum = sum + u->distance;

        for(i=0; i<u->currentSize; i++){
                ubig alt = u->distance + u->distanceArr[i];
                if(list[u->elemArr[i]]->inHeap==1 && list[u->elemArr[i]]->distance > alt) {
                    list[u->elemArr[i]]->distance = alt;
                    minHeapFixUp(heap, list[u->elemArr[i]]->heapPosition);
                }
            }
    }

    clearAll();

    return sum;

}*/

ubig djkRun(){

    ul i, j;
    ubig value;
    ubig start;
    bool allEquals=true;
    bool allZero=true;

    if(scanf("%lu,", &start));
    for(i=1; i<numberNode; i++){

        if(scanf("%lu,", &value));
        if(value>0) {
            addElemToList(list[0], value, i);
            allZero=false;
        }
        if(value!=start)
            allEquals=false;

    }

    if(allZero){
        for(i=1; i<numberNode; i++) {
            for (j = 0; j < numberNode; j++) {
                if(scanf("%lu,", &value));
            }
        }
        clearAll();
        return 0;

    }

    list[0]->distance=0;
    addMinHeapElement(heap, list[0]);

    for(i=1; i<numberNode; i++){

        for(j=0; j<numberNode; j++){

            if(scanf("%lu,", &value));
            if(value>0 && i!=j)
                addElemToList(list[i], value, j);
            if(value!=start)
                allEquals=false;

        }

        list[i]->distance=INF;
    }

    if(allEquals){
        clearAll();
        return value*(numberNode-1);
    }

    ubig sum=0;

    do{
        headList u = getMinHeapElement(heap);

        if(u->distance<INF)
            sum+=u->distance;

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
    for(s=1; s<heap->currentSize; s++){
        printf("%u ", heap->graphId[s]);
    }
    printf("%u", heap->graphId[heap->currentSize]);
    printf("\n");
}