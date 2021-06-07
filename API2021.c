#include <stdio.h>
#include <stdlib.h>


#define INFINITY 4294967295

typedef unsigned int uint;

struct adjNode{
    uint distance;
    uint elem;
    struct adjNode* next;
};
typedef struct adjNode* listNode;

struct h{
    uint distance;
    listNode point;
};
typedef struct h* headList;

struct minHeapNode{
    headList *head;
    uint currentSize;
    uint maxSize;
};
typedef struct minHeapNode* minHeap;

uint rowLength;
uint maxNumberTopGraph;
uint currentNumberTopGraph=0;


minHeap createMinHeap();
void addNewElementMinHeap(minHeap heap, headList head);
void minHeapFixUp(minHeap heap, uint pos);
void minHeapSwapElement(minHeap heap, uint f, uint s);
void removeMinHeapElement(minHeap heap);
headList getMinHeapElement(minHeap heap);
int emptyMinHeap(minHeap heap);
void minHeapFixDown(minHeap heap, uint pos);
int compareString(char* string1, char* string2);
uint djkRun2();
void printTest(minHeap heap);


int main(){
    scanf("%u %u", &rowLength, &maxNumberTopGraph);
    djkRun2();

    return 0;
}


minHeap createMinHeap() {
    minHeap heap = (minHeap) malloc(sizeof(struct minHeapNode));
    heap->maxSize = rowLength;
    heap->currentSize = 0;
    heap->head = (headList*) malloc(sizeof(headList) * rowLength);

    return heap;
}


void addNewElementMinHeap(minHeap heap, headList head){

    heap->currentSize++;
    heap->head[heap->currentSize] = head;

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
    heap->head[s] = father;
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
        int nPos = pos*2;
        if(nPos < heap->currentSize && heap->head[nPos+1]->distance < heap->head[nPos]->distance)
            nPos++;    //going to the right child
        if(heap->head[pos]->distance <= heap->head[nPos]->distance)
            break;      //have already swapped everything

        minHeapSwapElement(heap, pos, nPos);
        pos = nPos;
    }
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

uint djkRun2() {

    minHeap heap = createMinHeap();
    headList adjList[rowLength];

    for(int i=0; i<rowLength; i++){
        adjList[i] = (headList) malloc(sizeof (headList));
        adjList[i]->point = (listNode) malloc(sizeof (listNode));
        adjList[i]->point = NULL;
        adjList[i]->distance=INFINITY;

        for(int j=0; j<rowLength; j++){
            if(i!=j && j!=0){
                uint d;
                scanf("%u,", &d);
                listNode toAdd = (listNode) malloc(sizeof (listNode));
                toAdd->next=adjList[i]->point;
                adjList[i]->point = toAdd;
                toAdd->distance = d;
                toAdd->elem = j;
            }
            else {
                uint d;
                scanf("%u,",&d);
            }
        }

        addNewElementMinHeap(heap, adjList[i]);
    }

    printTest(heap);

    /*for(int i=1; i<rowLength; i++){
        if(matrix[0][i] > 0){
            addNewElementMinHeap(heap, matrix[0][i], i);
        }
    }

    while (counter < rowLength && !emptyHeap(heap)) {
        uint* arr = getMinHeapElement(heap);
        uint distance = arr[0];
        uint elem = arr[1];
        free(arr);

        while(visitedNode[elem] == 1 && !emptyHeap(heap)){
            arr = getMinHeapElement(heap);
            distance = arr[0];
            elem = arr[1];
            free(arr);
        }
        if(!emptyHeap(heap)) {


            //adding a fewer cost to the distanceNode array
            visitedNode[elem] = 1;
            distanceNode[elem] = distance;
            counter++;

            for (int i = 1; i < rowLength; i++) {
                if (visitedNode[i] != 1 && matrix[elem][i] > 0) {
                    addNewElementMinHeap(heap, matrix[elem][i] + distance, i);
                }
            }
        }
    }
    uint sum =0;
    for (int i = 1; i < rowLength; i++) {
        if(distanceNode[i]==INFINITY)
            distanceNode[i]=0;
        sum = sum + distanceNode[i];
    }

    free(heap->arrayCost);
    free(heap->arrayElem);
    free(heap);

    return sum;*/
}

void printTest(minHeap heap) {
    for(int i=1; i<heap->currentSize; i++){
        printf("Key: %u\n", heap->head[i]->distance);
    }
    printf("\n");
}
