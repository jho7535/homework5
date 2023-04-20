/**
 * postfix.c
 *
 * School of Computer Science,
 * Chungbuk National University
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define MAX_STACK_SIZE 10
#define MAX_EXPRESSION_SIZE 20
/* stack 내에서 우선순위는 내림차순, lparen = 0 가장 낮음 */

typedef enum
{
    lparen = 0, /* ( 왼쪽 괄호 */
    rparen = 9, /* ) 오른쪽 괄호*/
    times = 7,  /* * 곱셈 */
    divide = 6, /* / 나눗셈 */
    plus = 5,   /* + 덧셈 */
    minus = 4,  /* - 뺄셈 */
    operand = 1 /* 피연산자 */
} precedence;

char infixExp[MAX_EXPRESSION_SIZE];   // 중위 표기법으로 표현된 수식을 저장하는 배열
char postfixExp[MAX_EXPRESSION_SIZE]; // 후위 표기법으로 변환된 수식을 저장하는 배열
char postfixStack[MAX_STACK_SIZE];    // 중간 계산을 위해 사용되는 후위 표기법 변환 과정에서의 스택
int evalStack[MAX_STACK_SIZE];        // 후위 표기법으로 변환된 수식을 계산하는 과정에서의 스택
int postfixStackTop = -1;             // 후위 표기법 변환 과정에서의 스택의 탑(top)을 나타내는 인덱스
int evalStackTop = -1;                // 후위 표기법으로 변환된 수식을 계산하는 과정에서의 스택의 탑(top)을 나타내는 인덱스
int evalResult = 0;                   // 수식을 계산한 결과를 저장하는 변수

void postfixPush(char x);         // postfixStack에 원소를 push
char postfixPop();                // postfixStack에서 원소를 pop
void evalPush(int x);             // evalStack에 원소를 push
int evalPop();                    // evalStack에서 원소를 pop
void getInfix();                  // infixExp를 입력받음
precedence getToken(char symbol); // 연산자의 우선순위를 반환
precedence getPriority(char x);   // 연산자의 우선순위를 반환
void charCat(char *c);            // 문자를 postfixExp에 추가
void toPostfix();                 // infixExp를 postfixExp로 변환
void debug();                     // postfixExp, postfixStack, evalStack 상태 출력
void reset();                     // postfixExp, postfixStack, evalStack, evalResult 초기화
void evaluation();                // postfixExp를 계산하여 evalResult에 저장

int main()
{
    char command; // 사용자가 입력한 명령어를 저장하는 변수
    do
    {
        printf("----------------------------------------------------------------\n");
        printf(" Infix to Postfix, then Evaluation\n");
        printf("----------------------------------------------------------------\n");
        printf(" Infix=i, Postfix=p, Eval=e, Debug=d, Reset=r,Quit=q \n");
        printf("----------------------------------------------------------------\n");
        printf("Command = ");
        scanf(" %c", &command); // 사용자로부터 명령어를 입력받음
        switch (command)
        {
        case 'i':
        case 'I':
            getInfix(); // 사용자로부터 infix 표현식을 입력받음
            break;
        case 'p':
        case 'P':
            toPostfix(); // infix 표현식을 postfix 표현식으로 변환
            break;
        case 'e':
        case 'E':
            evaluation(); // postfix 표현식을 평가하여 결과 출력
            break;
        case 'd':
        case 'D':
            debug(); // 디버그 정보 출력
            break;
        case 'r':
        case 'R':
            reset(); // 모든 변수와 스택을 초기화
            break;
        case 'q':
        case 'Q':
            break;
        default:
            printf("\n >>>>> Concentration!! <<<<<\n"); // 잘못된 명령어 입력 시 오류 메시지 출력
            break;
        }
    } while (command != 'q' && command != 'Q'); // q 또는 Q가 입력될 때까지 반복
    return 1;
}

void postfixPush(char x)
{
    postfixStack[++postfixStackTop] = x; // postfix 스택에 요소를 push
}

char postfixPop()
{
    char x;
    if (postfixStackTop == -1)
        return '\0'; // postfix 스택이 비어있을 때 '\0' 반환
    else
    {
        x = postfixStack[postfixStackTop--]; // postfix 스택에서 요소를 pop
    }
    return x;
}

void evalPush(int x)
{
    evalStack[++evalStackTop] = x; // eval 스택에 요소를 push
}

int evalPop()
{
    if (evalStackTop == -1)
        return -1; // eval 스택이 비어있을 때 -1 반환
    else
        return evalStack[evalStackTop--]; // eval 스택에서 요소를 pop
}

/**
 * infix expression을 입력받는다.
 * infixExp에는 입력된 값을 저장한다.
 */
void getInfix()
{
    printf("Type the expression >>> ");
    scanf("%s", infixExp); // 사용자로부터 infix 표현식 입력받음
}

precedence getToken(char symbol)
{
    switch (symbol)
    {
    case '(':
        return lparen;
    case ')':
        return rparen;
    case '+':
        return plus;
    case '-':
        return minus;
    case '/':
        return divide;
    case '*':
        return times;
    default:
        return operand;
    }
}

precedence getPriority(char x)
{
    return getToken(x); // getToken() 함수 호출하여 우선순위 반환
}
/**
 * 문자하나를 전달받아, postfixExp에 추가
 */
void charCat(char *c)
{
    if (postfixExp == '\0')        // 만약 postfixExp가 비어있으면
        strncpy(postfixExp, c, 1); // c를 postfixExp에 복사
    else
        strncat(postfixExp, c, 1); // 아니면 c를 postfixExp에 이어붙임
}

/**
 * infixExp의 문자를 하나씩 읽어가면서 stack을 이용하여 postfix로 변경한다.
 * 변경된 postfix는 postFixExp에 저장된다.
 */
void toPostfix()
{
    /* infixExp의 문자 하나씩을 읽기위한 포인터 */
    char *exp = infixExp;
    char x; /* 문자하나를 임시로 저장하기 위한 변수 */
    /* exp를 증가시켜가면서, 문자를 읽고 postfix로 변경 */
    while (*exp != '\0')
    {
        if (getPriority(*exp) == operand) // 현재 문자가 operand(피연산자)인 경우
        {
            x = *exp;
            charCat(&x); // 현재 문자를 postfixExp에 추가
        }
        else if (getPriority(*exp) == lparen) // 현재 문자가 lparen(왼쪽 괄호)인 경우
        {
            postfixPush(*exp); // 현재 문자를 postfixStack에 푸시
        }
        else if (getPriority(*exp) == rparen) // 현재 문자가 rparen(오른쪽 괄호)인 경우
        {
            while ((x = postfixPop()) != '(') // '('를 만날 때까지 postfixStack에서 팝하고, 각각을 postfixExp에 추가
            {
                charCat(&x);
            }
        }
        else // 현재 문자가 연산자인 경우
        {
            while (getPriority(postfixStack[postfixStackTop]) >= getPriority(*exp)) // postfixStack의 top에 있는 연산자의 우선순위가 현재 연산자보다 크거나 같을 때까지 반복
            {
                x = postfixPop(); // postfixStack에서 팝한 연산자를 x에 저장
                charCat(&x);      // x를 postfixExp에 추가
            }
            postfixPush(*exp); // 현재 연산자를 postfixStack에 푸시
        }
        exp++; // 다음 문자로 이동
    }
    while (postfixStackTop != -1) // postfixStack에 남아있는 연산자들을 모두 postfixExp에 추가
    {
        x = postfixPop();
        charCat(&x);
    }
}

void debug()
{
    printf("\n---DEBUG\n");
    printf("infixExp = %s\n", infixExp);      // 중위 표기식
    printf("postExp = %s\n", postfixExp);     // 후위 표기식
    printf("eval result = %d\n", evalResult); // 계산 결과
    printf("postfixStack : ");
    for (int i = 0; i < MAX_STACK_SIZE; i++)
        printf("%c ", postfixStack[i]); // 후위 표기식 스택
    printf("\n");
}

void reset()
{
    infixExp[0] = '\0';   // 중위 표기식 초기화
    postfixExp[0] = '\0'; // 후위 표기식 초기화
    for (int i = 0; i < MAX_STACK_SIZE; i++)
        postfixStack[i] = '\0'; // 후위 표기식 스택 초기화

    postfixStackTop = -1; // 후위 표기식 스택의 top을 -1로 초기화
    evalStackTop = -1;    // 계산 스택의 top을 -1로 초기화
    evalResult = 0;       // 계산 결과 초기화
}

void evaluation()
{
    int opr1, opr2, i;
    int length = strlen(postfixExp);
    char symbol;
    evalStackTop = -1; // 계산 스택의 top을 -1로 초기화
    for (i = 0; i < length; i++)
    {
        symbol = postfixExp[i];
        if (getToken(symbol) == operand) // 피연산자인 경우
        {
            evalPush(symbol - '0'); // 스택에 push
        }
        else // 연산자인 경우
        {
            opr2 = evalPop(); // 스택에서 pop
            opr1 = evalPop(); // 스택에서 pop
            switch (getToken(symbol))
            {
            case plus:                 // 더하기 연산
                evalPush(opr1 + opr2); // 스택에 push
                break;
            case minus:                // 빼기 연산
                evalPush(opr1 - opr2); // 스택에 push
                break;
            case times:                // 곱하기 연산
                evalPush(opr1 * opr2); // 스택에 push
                break;
            case divide:               // 나누기 연산
                evalPush(opr1 / opr2); // 스택에 push
                break;
            default:
                break;
            }
        }
    }
    evalResult = evalPop(); // 계산 결과를 스택에서 pop
}