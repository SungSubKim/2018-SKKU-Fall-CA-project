#include <stdio.h>
#include <stdlib.h>

#define MAX_STACK 100
typedef enum {false , true} bool;
typedef struct {
    int items[MAX_STACK];
    int top;
}Stack;

int take_5digitR(int num, int digit,int option);
int take_5digit(int num,int digit,int option);
void InitStack (Stack *pstack);
bool IsFull(Stack *pstack);
bool IsEmpty(Stack *pstack);
void Push(Stack *pstack,int item);
int Pop(Stack *pstack); 
int mypow(int num,int cnt);
void HW2A(int n, int sum_min); 

int main() {
    int cnt,cnt2,n,x;
    scanf("%d%d",&n,&x);
    HW2A(n,x);
}
void HW2A(int n, int sum_min) {
    int sum,num,num_max,cnt,cnt2,lim;
    num_max = mypow(5,n)-1;
    for(num=0;num<=num_max;num++) {
        sum = take_5digitR(num,n,0);
        if(sum>=sum_min) {
            take_5digitR(num,n,1);
            printf("\n");
        }
        
    }
}
int mypow(int num,int cnt) {
    int cnt2,result=1;
    for(cnt2=0;cnt2<cnt;cnt2++) result*=num;
    return result;
}

int take_5digit(int num,int digit,int option){
    int cnt,n,result=0;
    Stack *pstack = malloc(sizeof(Stack));
    InitStack(pstack);
    for(cnt=0;cnt<digit;cnt++) {
        if(num==0) Push(pstack,0);
        else {
            Push(pstack,num%5);
            num/=5;
        }
    }
    for(cnt=0;cnt<digit;cnt++) {
        n = Pop(pstack);
        if(option) printf("%d",n);
        result+=n;
    }
    if(option) printf("\n");
    free(pstack);
    return result;
}
int take_5digitR(int num, int digit,int option) {
    int cnt,n,result =0;
    if(digit>0) result += num%5 + take_5digitR(num/5,digit-1,option);
    if(option&&digit!=0) {
        if(num==0) printf("0");
        else printf("%d",num%5);
    }
    return result;
}

void InitStack(Stack *pstack) {
    pstack->top = -1;
}
bool IsFull(Stack *pstack) {
    return pstack->top == MAX_STACK -1;
}
bool IsEmpty(Stack *pstack) {
    return pstack->top==-1;
}
void Push(Stack *pstack,int item) {
    if (IsFull(pstack)) exit(1);
    pstack->items[++(pstack->top)] = item;
}
int Pop(Stack *pstack) {
    if(IsEmpty(pstack)) exit(1);
    return pstack->items[(pstack->top)--];
}
