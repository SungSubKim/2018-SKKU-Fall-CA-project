#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STACK 100

typedef enum {false , true} bool;

typedef struct {
    char small;
    char large;
    // *Variable "operator" 
    // used in ConvInfixToPostfix function
    char operator;	
}Penta_num;

typedef struct {
    Penta_num items[MAX_STACK];
    int top;
}Stack;

/* Modify from here */
int extract(Penta_num p);
Penta_num compose(int c); 
/* Modify to here */

Penta_num add(Penta_num b1,Penta_num b2);
Penta_num subtract(Penta_num b1,Penta_num b2);
Penta_num multiply(Penta_num b1,Penta_num b2);
Penta_num division(Penta_num b1,Penta_num b2);

void InitStack (Stack *pstack);
bool IsFull(Stack *pstack);
bool IsEmpty(Stack *pstack);
Penta_num Peek(Stack *pstack);
void Push(Stack *pstack,Penta_num item);
void Pop(Stack *pstack); 
void ConvInfixToPostfix(char* exp,char* convExp, int len);
Penta_num EvalPostfix(char* exp,int len);
void print_Penta_num(Penta_num result);



int main() {
    char infix_exp[100];
    char postfix_exp[100];
    Penta_num result;
    scanf("%s",infix_exp);
    ConvInfixToPostfix(infix_exp,postfix_exp,strlen(infix_exp));
    printf("%s\n%s\n",infix_exp,postfix_exp);
    result = EvalPostfix(postfix_exp,strlen(postfix_exp));
    print_Penta_num(result);
    return 0;
}
/* Modify from here */
void InitStack(Stack *pstack) {
    pstack->top = -1;
}
bool IsFull(Stack *pstack) {
    return pstack->top == MAX_STACK -1;
}
bool IsEmpty(Stack *pstack) {
    return pstack->top==-1;
}
Penta_num Peek(Stack *pstack){
    if(IsEmpty(pstack)) exit(1);
    return pstack->items[pstack->top];
}
void Push(Stack *pstack,Penta_num item) {
    if (IsFull(pstack)) exit(1);
    pstack->items[++(pstack->top)] = item;
}
void Pop(Stack *pstack) {
    if(IsEmpty(pstack)) exit(1);
    pstack->top--;
}
void ConvInfixToPostfix(char* exp,char* convExp, int len){
    int cnt=0,cnt2=0;
    Penta_num num;
    Stack* operators=malloc(sizeof(Stack));
    InitStack(operators);
    while(1) {
        convExp[cnt2++]=exp[cnt++];
        convExp[cnt2++]=exp[cnt++];
        if(exp[cnt]=='+' || exp[cnt]== '-') {
            if(IsEmpty(operators)) {
                num.operator = exp[cnt++];
                Push(operators,num);
            }
            else{
                while(!IsEmpty(operators)) {
                    convExp[cnt2++]=Peek(operators).operator;
                    Pop(operators);
                }
                num.operator = exp[cnt++];
                Push(operators,num);
            }
        }
        else if(exp[cnt]=='*'||exp[cnt]=='/') {
            if(IsEmpty(operators)) {
                num.operator = exp[cnt++];
                Push(operators,num);
            }
            else if(Peek(operators).operator=='+'||Peek(operators).operator=='-') {
                num.operator = exp[cnt++];
                Push(operators,num);
            }
            else if(Peek(operators).operator=='*'||Peek(operators).operator=='/') {
                convExp[cnt2++]=Peek(operators).operator;
                Pop(operators);
                num.operator = exp[cnt++];
                Push(operators,num);
            }            
        }
        else {
            while(!IsEmpty(operators)){
                convExp[cnt2++]=Peek(operators).operator;
                Pop(operators);
            }
            break;
        }
    }
    convExp[cnt2]='\0';
    free(operators);
}
Penta_num EvalPostfix(char* exp,int len) {
    int cnt =0;
    Penta_num num,num2,tmp,tmp1;
    Stack* nums=malloc(sizeof(Stack));
    InitStack(nums);
    while(cnt<len) {
        if(exp[cnt]=='+') {
            num=Peek(nums);
            Pop(nums);
            num2=Peek(nums);
            Pop(nums);
            num = add(num,num2);
            Push(nums,num);
        }
        else if(exp[cnt]=='-') {
            num=Peek(nums);
            Pop(nums);
            num2=Peek(nums);
            Pop(nums);
            num = subtract(num2,num);
            Push(nums,num);
        }
        else if(exp[cnt]=='*') {
            num=Peek(nums);
            Pop(nums);
            num2=Peek(nums);
            Pop(nums);
            num = multiply(num,num2);
            Push(nums,num);
        }
        else if(exp[cnt]=='/') {
            num=Peek(nums);
            Pop(nums);
            num2=Peek(nums);
            Pop(nums);
            num = division(num2,num);
            Push(nums,num);
        }
        else {
            num.large=exp[cnt++];
            num.small=exp[cnt];
            Push(nums,num);
        }
        cnt++;
    }
    num = Peek(nums);
    free(nums);
    return num;
}

Penta_num add(Penta_num b1,Penta_num b2) {
    return compose(extract(b1)+extract(b2));
}
Penta_num subtract(Penta_num b1,Penta_num b2) {
    return compose(extract(b1)-extract(b2));
}
Penta_num multiply(Penta_num b1,Penta_num b2){
    return compose(extract(b1)*extract(b2));
}
Penta_num division(Penta_num b1,Penta_num b2) {
    return compose(extract(b1)/extract(b2));
}

int extract(Penta_num p) {
    if(p.large>64) p.large-=55;
    else p.large-=48;
    if(p.small>64) p.small-=55;
    else p.small-=48;
    return p.large*15+p.small;
}
Penta_num compose(int c) {
    Penta_num p;
    p.large=c/15;
    p.small=c%15;
    if(p.large>=10) p.large+=55;
    else p.large+= 48;
    if(p.small>=10) p.small+=55;
    else p.small+= 48;
    return p;
}
void print_Penta_num(Penta_num result) {
    printf("%c%c\n",result.large,result.small);
}
/* Modify to here */
