#include <stdio.h>
#include <stdlib.h>


#define INFINITY 4294967295

typedef unsigned int uint;


struct h{
    uint distance;
    uint currentPosition;
    uint maxPosition;
    uint* distanceArr;
    uint* elemArr;
    uint minHeapPosition;
};
typedef struct h* headList;

struct minHeapNode{
    headList *head;
    uint currentSize;
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

/**
 * FUNCTION TO CREATE MIN HEAP AND ADJACENCY LIST FOR DIJKSTRA ALGORITHM
 */
void addElementToList(headList list, uint distance, uint elem);
headList createList();
minHeap createMinHeap();
void addNewElementMinHeap(minHeap heap, headList head);
void minHeapFixUp(minHeap heap, uint pos);
void minHeapSwapElement(minHeap heap, uint f, uint s);
void removeMinHeapElement(minHeap heap);
headList getMinHeapElement(minHeap heap);
int emptyMinHeap(minHeap heap);
void minHeapFixDown(minHeap heap, uint pos);
uint djkRun2();
void printTest(minHeap heap);
void addNewSpaceList(headList list);

/**
 * FUNCTION TO CREATE MAX HEAP AND TOPK VALUE
 */
maxHeap createMaxHeap();
void removeMaxHeapElement(maxHeap heap);
void maxHeapFixDown(maxHeap heap, uint pos);
void addNewElementMaxHeap(maxHeap heap, uint key, uint prevElem);
void maxHeapFixUp(maxHeap heap, uint pos);
void maxHeapSwapElement(maxHeap heap, uint f, uint s);
int emptyMaxHeap(maxHeap heap);
void maxHeapInsertDelete(maxHeap heap, uint key, uint prevElem);

void addGraph();
void showTopK();

int compareString(const char* string1, const char* string2);


int main(){
    if (scanf("%u %u", &rowLength, &maxNumberTopGraph)){}
    topKHeap = createMaxHeap();

    while(!feof(stdin)){
        char command[14];
        if(scanf("%s", command)){}

        if(feof(stdin))
            return 0;
        if(compareString(command, "AggiungiGrafo")){
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

void minHeapFixUp(minHeap heap, uint pos) {
    while(pos>1 && heap->head[pos]->distance < heap->head[pos/2]->distance){
        minHeapSwapElement(heap, pos / 2, pos);
        pos = pos/2;
    }
}

void minHeapSwapElement(minHeap heap, uint f, uint s) {
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

void minHeapFixDown(minHeap heap, uint pos) {
    while(pos*2 <= heap->currentSize){
        uint nPos = pos*2;
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

uint djkRun2() {

    minHeap heap;
    heap = createMinHeap();
    headList adjList[rowLength];
    int zeroFirstRow=0;

    adjList[0] = createList();
    adjList[0]->distance=0;
    for(int i=0; i<rowLength; i++){
        uint d;
        if(scanf("%u,", &d)){}
        if(d>0 && i!=0){
            addElementToList(adjList[0], d, i);
            zeroFirstRow=1;
        }
    }

    if(zeroFirstRow==0){
        uint d;
        for(int i=1; i<rowLength; i++){
            for(int j=0; j<rowLength; j++){
                if(scanf("%u,", &d)){}
            }
        }
        return 0;
    }
    else
        addNewElementMinHeap(heap, adjList[0]);

    for(int i=1; i<rowLength; i++){
        adjList[i] = createList();
        adjList[i]->distance = INFINITY;

        for(int j=0; j<rowLength; j++){
            uint d;
            if(scanf("%u,", &d)){}

            if(i!=j && j!=0 && d>0){
                addElementToList(adjList[i], d, j);
            }
        }
        addNewElementMinHeap(heap, adjList[i]);
    }



    uint sum=0;
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
    list->distanceArr = (uint*) malloc(sizeof (uint)*(list->maxPosition+1));
    list->elemArr = (uint*) malloc(sizeof (uint)*(list->maxPosition+1));

    return list;
}

void addElementToList(headList list, uint distance, uint elem) {
    if(list->currentPosition >= list->maxPosition)
        addNewSpaceList(list);

    list->currentPosition++;
    list->distanceArr[list->currentPosition] = distance;
    list->elemArr[list->currentPosition] = elem;

}

void addNewSpaceList(headList list){
    uint max = list->maxPosition*2;
    list->elemArr = (uint*) realloc(list->elemArr, (sizeof (uint))*(max+1));
    list->distanceArr = (uint*) realloc(list->distanceArr, (sizeof (uint)*(max+1)));
    list->maxPosition = max;
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
    if(!emptyMaxHeap(heap)){
        maxHeapSwapElement(heap, 1, heap->currentSize);
        heap->currentSize--;
        maxHeapFixDown(heap, 1);
    }
}

void maxHeapFixDown(maxHeap heap, uint pos) {
    while(pos*2 <= heap->currentSize){
        uint nPos = pos*2;
        if(nPos < heap->currentSize && (heap->arrayCost[nPos+1] > heap->arrayCost[nPos] || (heap->arrayCost[nPos + 1] == heap->arrayCost[nPos] && heap->arrayElem[nPos + 1] > heap->arrayElem[nPos])))
            nPos++; //if the right child is bigger, going to the right child

        if(heap->arrayCost[pos] > heap->arrayCost[nPos] || (heap->arrayCost[pos] == heap->arrayCost[nPos] && heap->arrayElem[pos] > heap->arrayElem[nPos])){
            break;      //have already swapped everything
        }

        maxHeapSwapElement(heap, pos, nPos);
        pos = nPos;
    }
}

void addNewElementMaxHeap(maxHeap heap, uint key, uint prevElem) {

    heap->currentSize++;
    heap->arrayCost[heap->currentSize] = key;
    heap->arrayElem[heap->currentSize] = prevElem;

    maxHeapFixUp(heap, heap->currentSize);
}

void maxHeapFixUp(maxHeap heap, uint pos) {
    while(pos>1 && (heap->arrayCost[pos] > heap->arrayCost[pos/2] || (heap->arrayCost[pos] == heap->arrayCost[pos/2] && heap->arrayElem[pos] > heap->arrayElem[pos/2]))){
        maxHeapSwapElement(heap, pos / 2, pos);
        pos = pos/2;
    }
}

void addGraph() {
    uint key = djkRun2();
    if(currentNumberTopGraph < maxNumberTopGraph){
        addNewElementMaxHeap(topKHeap, key, currentNumberTopGraph);
    }
    else{
        if(key < topKHeap->arrayCost[1]) {
            maxHeapInsertDelete(topKHeap, key, currentNumberTopGraph);
        }
    }
    currentNumberTopGraph++;
}

void showTopK() {
    for(int i=1; i <= topKHeap->currentSize; i++)
        printf("%u ", topKHeap->arrayElem[i]);
    printf("\n");
}

void maxHeapSwapElement(maxHeap heap, uint f, uint s) {
    uint elemF = heap->arrayElem[f];
    uint costF = heap->arrayCost[f];
    heap->arrayElem[f] = heap->arrayElem[s];
    heap->arrayCost[f] = heap->arrayCost[s];
    heap->arrayElem[s] = elemF;
    heap->arrayCost[s] = costF;
}

int emptyMaxHeap(maxHeap heap) {
    return heap->currentSize<=0;
}

void maxHeapInsertDelete(maxHeap heap, uint key, uint prevElem) {

    heap->arrayCost[1] = key;
    heap->arrayElem[1] = prevElem;
    maxHeapFixDown(heap, 1);

}
