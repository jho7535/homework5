/**
 * circularQ.c
 *
 * School of Computer Science, 
 * Chungbuk National University
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 4 //기호상수 MAX_QUEUE_SIZE를 4로 설정

typedef char element; //char형을 element로 재정의
typedef struct { 
	element queue[MAX_QUEUE_SIZE]; //크기가 MAX_QUEUE_SIZE인 element형 배열 queue 선언
	int front, rear; //int형 변수 front, rear 선언
}QueueType; //구조체 이름을 QueueType으로 재정의


QueueType *createQueue(); // 큐를 생성하는 함수
int freeQueue(QueueType *cQ); // 큐를 해제하는 함수
int isEmpty(QueueType *cQ); // 큐가 비어있는지 확인하는 함수
int isFull(QueueType *cQ); // 큐가 가득 찼는지 확인하는 함수
void enQueue(QueueType *cQ, element item); // 큐에 요소를 추가하는 함수
void deQueue(QueueType *cQ, element *item); // 큐에서 요소를 제거하는 함수
void printQ(QueueType *cQ); // 큐를 출력하는 함수
void debugQ(QueueType *cQ); // 큐의 상태를 출력하는 함수
element getElement(); // 사용자로부터 입력을 받는 함수

int main(void)
{
    printf("[----- [Jeong Hanul] [2020039076] -----]\n\n");


    QueueType *cQ = createQueue(); //큐 생성
    element data;

    char command;

    do {
        printf("\n-----------------------------------------------------\n");
        printf("                     Circular Q                   \n");
        printf("------------------------------------------------------\n");
        printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n"); //메뉴얼 출력
        printf("------------------------------------------------------\n");

        printf("Command = ");
        scanf(" %c", &command);

        switch (command) {
        case 'i': case 'I': // 데이터 삽입
            data = getElement();
            enQueue(cQ, data);
            break;
        case 'd': case 'D': // 데이터 삭제
            deQueue(cQ, &data);
            break;
        case 'p': case 'P': // 큐 출력
            printQ(cQ);
            break;
        case 'b': case 'B': // 큐의 상태 출력
            debugQ(cQ);
            break;
        case 'q': case 'Q': // 종료
            break;
        default: // 에러 메세지 출력
            printf("\n       >>>>>   Concentration!!   <<<<<     \n");
            break;
        }

    } while (command != 'q' && command != 'Q'); //입력값이 q, Q 일 때까지 반복

    freeQueue(cQ); //큐 해제

    return 1;
}

QueueType *createQueue()
{
	QueueType *cQ; //QueueType형 포인터 변수 cQ 생성
	cQ = (QueueType *)malloc(sizeof(QueueType)); // 동적 메모리 할당을 통해 QueueType 크기의 메모리 공간을 할당 받음
	cQ->front = 0; // front를 0으로 초기화
	cQ->rear = 0; // rear를 0으로 초기화
	return cQ; // 생성된 QueueType 포인터 반환
}

int freeQueue(QueueType *cQ)
{
    if(cQ == NULL) return 1; // cQ가 NULL인 경우 에러 처리
    free(cQ); // 동적으로 할당받은 메모리 공간을 해제
    return 1; // 정상적으로 해제되었음을 반환
}

element getElement()
{
	element item; //element형 변수 item 선언
	printf("Input element = "); // "Input element = " 출력
	scanf(" %c", &item); // 사용자로부터 입력받은 값을 item 변수에 저장
	return item; // item 반환
}

int isEmpty(QueueType *cQ)
{
	if (cQ->front == cQ->rear){ // front와 rear가 같으면 큐가 비어있는 상태
		printf("Circular Queue is empty!"); // "Circular Queue is empty!" 출력
		return 1; // 큐가 비어있음을 반환
	}
	else return 0; // 큐가 비어있지 않음을 반환
}

int isFull(QueueType *cQ)
{
	if (((cQ->rear+1)%MAX_QUEUE_SIZE) == cQ->front) { // rear의 다음 위치가 front와 같으면 큐가 가득 찬 상태
		printf(" Circular Queue is full!"); 
		return 1; // 큐가 가득 찼음을 반환
	}
	else return 0; // 큐가 가득 차지 않았음을 반환
}

void enQueue(QueueType *cQ, element item)
{
	if(isFull(cQ)) return; // 큐가 가득 찬 경우에는 enQueue 작업 수행하지 않고 종료
	else {
		cQ->rear = (cQ->rear + 1) % MAX_QUEUE_SIZE; // rear를 순환하면서 다음 위치로 이동
		cQ->queue[cQ->rear] = item; // 다음 위치에 item 값을 저장하여 enQueue 작업 수행.
	}
}

void deQueue(QueueType *cQ, element *item)
{
	if(isEmpty(cQ)) return; // 큐가 비어있는 경우에는 deQueue 작업 수행하지 않고 종료
	else {
		cQ->front = (cQ->front + 1)%MAX_QUEUE_SIZE; // front를 순환하면서 다음 위치로 이동
		*item = cQ->queue[cQ->front]; // 다음 위치의 값을 item에 저장하여 deQueue 작업 수행
		return;
	}
}

void printQ(QueueType *cQ)
{
	int i, first, last; //int형 변수 i, first, last 선언

	first = (cQ->front + 1)%MAX_QUEUE_SIZE; // front 다음 위치를 계산하여 first에 저장
	last = (cQ->rear + 1)%MAX_QUEUE_SIZE; // rear 다음 위치를 계산하여 last에 저장

	printf("Circular Queue : [");

	i = first;
	while(i != last) //i != last인 동안 반복
    {
		printf("%3c", cQ->queue[i]); // 원형 큐에서 first부터 last까지의 요소들을 출력
		i = (i+1)%MAX_QUEUE_SIZE; // 다음 위치로 이동
	}

	printf(" ]\n");
}

void debugQ(QueueType *cQ)
{

	printf("\n---DEBUG\n");
	for(int i = 0; i < MAX_QUEUE_SIZE; i++) //i를 0부터 MAX_QUEUE_SIZE보다 작을 때까지 1씩 증가하며 반복
	{
		if(i == cQ->front) {
			printf("  [%d] = front\n", i); // front 위치에 "front"라는 표시를 출력
			continue;
		}
		printf("  [%d] = %c\n", i, cQ->queue[i]); // 나머지 위치에 해당하는 요소들을 출력

	}
	//printQ(cQ);
	printf("front = %d, rear = %d\n", cQ->front, cQ->rear); // 현재 front와 rear의 값을 출력
}