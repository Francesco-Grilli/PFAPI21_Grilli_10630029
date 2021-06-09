#include <stdio.h>
#include <stdlib.h>


#define INFINITY 4294967295

typedef unsigned int ubig;

struct h{
    ubig distance;
    ubig currentPosition;
    ubig maxPosition;
    ubig* distanceArr;
    ubig* elemArr;
    ubig minHeapPosition;
};
typedef struct h* headList;

struct minHeapNode{
    headList *head;
    ubig currentSize;
};
typedef struct minHeapNode* minHeap;

struct maxHeapNode{
    ubig* arrayCost;
    ubig* arrayElem;
    ubig currentSize;
    ubig maxSize;
};
typedef struct maxHeapNode* maxHeap;

ubig rowLength;
ubig maxNumberTopGraph;
ubig currentNumberTopGraph=0;
maxHeap topKHeap;

/**
 * FUNCTION TO CREATE MIN HEAP AND ADJACENCY LIST FOR DIJKSTRA ALGORITHM
 */
void addElementToList(headList list, ubig distance, ubig elem);
headList createList();
minHeap createMinHeap();
void addNewElementMinHeap(minHeap heap, headList head);
void minHeapFixUp(minHeap heap, ubig pos);
void minHeapSwapElement(minHeap heap, ubig f, ubig s);
void removeMinHeapElement(minHeap heap);
headList getMinHeapElement(minHeap heap);
int emptyMinHeap(minHeap heap);
void minHeapFixDown(minHeap heap, ubig pos);
ubig djkRun2();
void printTest(minHeap heap);
void addNewSpaceList(headList list);

/**
 * FUNCTION TO CREATE MAX HEAP AND TOPK VALUE
 */
maxHeap createMaxHeap();
void removeMaxHeapElement(maxHeap heap);
void maxHeapFixDown(maxHeap heap, ubig pos);
void addNewElementMaxHeap(maxHeap heap, ubig key, ubig prevElem);
void maxHeapFixUp(maxHeap heap, ubig pos);
void maxHeapSwapElement(maxHeap heap, ubig f, ubig s);
int emptyMaxHeap(maxHeap heap);
void maxHeapInsertDelete(maxHeap heap, ubig key, ubig prevElem);

void addGraph();
void showTopK();

int compareString(const char* string1, const char* string2);


int main(){
    if (scanf("%u %u", &rowLength, &maxNumberTopGraph)){}
    topKHeap = createMaxHeap();
    char s[14] = "AggiungiGrafo";

    while(!feof(stdin)){
        char command[14];
        if(scanf("%s", command)){}

        if(feof(stdin))
            return 0;
        if(compareString(command, s)){
            addGraph();
        }
        else
            showTopK();
    }


    return 0;
}


minHeap createMinHeap() {
    minHeap heap = (minHeap) malloc(sizeof(struct minHeapNode));
    heap->currentSize = 0;
    heap->head = (headList *) malloc(sizeof(headList) * (rowLength + 1));

    return heap;
}


void addNewElementMinHeap(minHeap heap, headList head){

    heap->currentSize++;
    heap->head[heap->currentSize] = head;
    heap->head[heap->currentSize]->minHeapPosition=heap->currentSize;

    minHeapFixUp(heap, heap->currentSize);

}

void minHeapFixUp(minHeap heap, ubig pos) {
    while(pos>1 && heap->head[pos]->distance < heap->head[pos/2]->distance){
        minHeapSwapElement(heap, pos / 2, pos);
        pos = pos/2;
    }
}

void minHeapSwapElement(minHeap heap, ubig f, ubig s) {
    headList father = heap->head[f];
    heap->head[f] = heap->head[s];
    heap->head[f]->minHeapPosition = f;
    heap->head[s] = father;
    heap->head[s]->minHeapPosition = s;
}

headList getMinHeapElement(minHeap heap) {

    if(!emptyMinHeap(heap)){
        headList h = heap->head[1];
        removeMinHeapElement(heap);
        return h;
    }

    return NULL;

}

int emptyMinHeap(minHeap heap) {
    return heap->currentSize<=0;
}

void removeMinHeapElement(minHeap heap) {
    if(!emptyMinHeap(heap)){
        minHeapSwapElement(heap, 1, heap->currentSize);
        heap->currentSize--;
        minHeapFixDown(heap, 1);
    }
}

void minHeapFixDown(minHeap heap, ubig pos) {
    while(pos*2 <= heap->currentSize){
        ubig nPos = pos * 2;
        if(nPos < heap->currentSize && heap->head[nPos+1]->distance < heap->head[nPos]->distance)
            nPos++;    //going to the right child
        if(heap->head[pos]->distance <= heap->head[nPos]->distance)
            break;      //have already swapped everything

        minHeapSwapElement(heap, pos, nPos);
        pos = nPos;
    }
}

int compareString(const char *string1, const char *string2) {

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

ubig djkRun2() {

    minHeap heap;
    heap = createMinHeap();
    headList adjList[rowLength];
    int zeroFirstRow=0;
    int allEquals=0;
    ubig elemEquals;

    adjList[0] = createList();
    adjList[0]->distance=0;
    if (scanf("%u,", &elemEquals)){}
    for(int i=1; i<rowLength; i++){
        ubig d;
        if(scanf("%u,", &d)){}
        if(d>0){
            addElementToList(adjList[0], d, i);
            zeroFirstRow=1;
        }
        if(d!=elemEquals)
            allEquals=1;
    }

    if(zeroFirstRow==0){
        ubig d;
        for(int i=1; i<rowLength; i++){
            for(int j=0; j<rowLength; j++){
                if(scanf("%u,", &d)){}
            }
        }

        free(adjList[0]->elemArr);
        free(adjList[0]->distanceArr);
        free(adjList[0]);
        free(heap->head);
        free(heap);

        return 0;
    }
    else
        addNewElementMinHeap(heap, adjList[0]);

    for(int i=1; i<rowLength; i++){
        adjList[i] = createList();
        adjList[i]->distance = INFINITY;

        for(int j=0; j<rowLength; j++){
            ubig d;
            if(scanf("%u,", &d)){}

            if(i!=j && j!=0 && d>0){
                addElementToList(adjList[i], d, j);
            }
            if(d!=elemEquals)
                allEquals=1;
        }
        addNewElementMinHeap(heap, adjList[i]);
    }

    if(allEquals==0){
        ubig c = elemEquals * (rowLength - 1);
        for(int i=0; i<rowLength; i++){
            free(adjList[i]->elemArr);
            free(adjList[i]->distanceArr);
            free(adjList[i]);
        }
        free(heap->head);
        free(heap);
        return c;
    }

    ubig sum=0;
    while(!emptyMinHeap(heap)){
        headList u = getMinHeapElement(heap);
        if(u->distance<INFINITY)
            sum +=u->distance;
        for(int i=1; i<=u->currentPosition; i++){
            if(adjList[u->elemArr[i]]->distance > (u->distanceArr[i] + u->distance) && u->distance<INFINITY){
                adjList[u->elemArr[i]]->distance = (u->distanceArr[i] + u->distance);
                minHeapFixUp(heap, adjList[u->elemArr[i]]->minHeapPosition);
            }
        }
    }

    for(int i=0; i<rowLength; i++){
        free(adjList[i]->elemArr);
        free(adjList[i]->distanceArr);
        free(adjList[i]);
    }
    free(heap->head);
    free(heap);

    return sum;
}

headList createList() {

    headList list = (headList) malloc(sizeof(struct h));
    list->currentPosition = 0;
    list->maxPosition = 2;
    list->distanceArr = (ubig*) malloc(sizeof (ubig) * (list->maxPosition + 1));
    list->elemArr = (ubig*) malloc(sizeof (ubig) * (list->maxPosition + 1));

    return list;
}

void addElementToList(headList list, ubig distance, ubig elem) {
    if(list->currentPosition >= list->maxPosition)
        addNewSpaceList(list);

    list->currentPosition++;
    list->distanceArr[list->currentPosition] = distance;
    list->elemArr[list->currentPosition] = elem;

}

void addNewSpaceList(headList list){
    ubig max = list->maxPosition * 2;
    list->elemArr = (ubig*) realloc(list->elemArr, (sizeof (ubig)) * (max + 1));
    list->distanceArr = (ubig*) realloc(list->distanceArr, (sizeof (ubig) * (max + 1)));
    list->maxPosition = max;
}

maxHeap createMaxHeap() {
    maxHeap heap = (maxHeap) malloc(sizeof (struct maxHeapNode));
    heap->maxSize = maxNumberTopGraph;
    heap->currentSize = 0;
    heap->arrayCost = (ubig*) malloc(sizeof (ubig) * (maxNumberTopGraph + 1));
    heap->arrayElem = (ubig*) malloc(sizeof (ubig) * (maxNumberTopGraph + 1));

    return heap;
}

void removeMaxHeapElement(maxHeap heap) {
    if(!emptyMaxHeap(heap)){
        maxHeapSwapElement(heap, 1, heap->currentSize);
        heap->currentSize--;
        maxHeapFixDown(heap, 1);
    }
}

void maxHeapFixDown(maxHeap heap, ubig pos) {
    while(pos*2 <= heap->currentSize){
        ubig nPos = pos * 2;
        if(nPos < heap->currentSize && (heap->arrayCost[nPos+1] > heap->arrayCost[nPos] || (heap->arrayCost[nPos + 1] == heap->arrayCost[nPos] && heap->arrayElem[nPos + 1] > heap->arrayElem[nPos]))) {
            nPos++; //if the right child is bigger, going to the right child
        }

        if(heap->arrayCost[pos] > heap->arrayCost[nPos] || (heap->arrayCost[pos] == heap->arrayCost[nPos] && heap->arrayElem[pos] > heap->arrayElem[nPos])){
            break;      //have already swapped everything
        }

        maxHeapSwapElement(heap, pos, nPos);
        pos = nPos;
    }
}

void addNewElementMaxHeap(maxHeap heap, ubig key, ubig prevElem) {

    if(heap->currentSize <= heap->maxSize) {
        heap->currentSize++;
        heap->arrayCost[heap->currentSize] = key;
        heap->arrayElem[heap->currentSize] = prevElem;

        maxHeapFixUp(heap, heap->currentSize);
    }
}

void maxHeapFixUp(maxHeap heap, ubig pos) {
    while(pos>1 && (heap->arrayCost[pos] > heap->arrayCost[pos/2] || (heap->arrayCost[pos] == heap->arrayCost[pos/2] && heap->arrayElem[pos] > heap->arrayElem[pos/2]))){
        ubig s = pos / 2;
        maxHeapSwapElement(heap, pos, s);
        pos = pos/2;
    }
}

void addGraph() {
    ubig key = djkRun2();
    printf("%u", key);
    if(currentNumberTopGraph >= maxNumberTopGraph){
        if(key < topKHeap->arrayCost[1]) {
            maxHeapInsertDelete(topKHeap, key, currentNumberTopGraph);
        }
    }
    else{
        addNewElementMaxHeap(topKHeap, key, currentNumberTopGraph);
    }
    currentNumberTopGraph++;
}

void showTopK() {
    for(int i=1; i <= topKHeap->currentSize; i++)
        printf("%u ", topKHeap->arrayElem[i]);
    printf("\n");
}

void maxHeapSwapElement(maxHeap heap, ubig f, ubig s) {
    ubig elemF = heap->arrayElem[f];
    ubig costF = heap->arrayCost[f];
    heap->arrayElem[f] = heap->arrayElem[s];
    heap->arrayCost[f] = heap->arrayCost[s];
    heap->arrayElem[s] = elemF;
    heap->arrayCost[s] = costF;
}

int emptyMaxHeap(maxHeap heap) {
    return heap->currentSize<=0;
}

void maxHeapInsertDelete(maxHeap heap, ubig key, ubig prevElem) {

    heap->arrayCost[1] = key;
    heap->arrayElem[1] = prevElem;
    maxHeapFixDown(heap, 1);

}
