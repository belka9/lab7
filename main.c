#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define STACK_UNDERFLOW -1
#define STACK_OVERFLOW -2
#define MAX_SIZE 1000


typedef int T;

typedef struct Node_tag {
    T value;
    struct Node_tag * next;
} Node_t;


typedef struct Stack_tag{
    T data[MAX_SIZE];
    int size;
} Stack_t;

void push(Stack_t *stack, T value);

T peek(Stack_t *stack);

T pop(Stack_t *stack);

int is_empty(Stack_t *stack);
void push(Stack_t *stack, T value){
    if (stack->size == MAX_SIZE)
        exit(STACK_OVERFLOW);
    stack->data[stack->size++] = value;
}

T peek(Stack_t *stack){
    if (stack->size == 0)
        return STACK_UNDERFLOW;
    return stack->data[stack->size-1];
}

T pop(Stack_t *stack){
    if (stack->size == 0)
        return STACK_UNDERFLOW;
    stack->size--;
    return stack->data[stack->size];
}

int is_empty(Stack_t *stack){
    return !stack->size;
}

void error(int flag){
    flag ? printf("syntax error") :	printf("division by zero");
    exit(0);
}

int get_priority(char sign){
    if (sign == '*' || sign == '/')
        return 4;
    else if (sign == '+' || sign == '-')
        return 3;
    else if (sign == '=')
        return 2;
    else if (sign == '(' || sign == ')')
        return 1;
}

char *get_postfix_form(Stack_t *stack, char str[], int str_len, int arr[]){
    char *postfix_form = (char *)malloc(sizeof(char) * (str_len + 1));
    memset(postfix_form, 0, sizeof(char) * (str_len + 1));
    char number[MAX_SIZE] = {0};
    int postfix_index = 0;
    int flag = 0;
    int k = 0;
    int sign_count = 0;
    int number_count = 0;
    int open_brackets = 0;
    int close_brackets = 0;
    for (int i = 0; i < str_len; i++){
        if (isdigit(str[i])){
            while (isdigit(str[i])){
                number[k++] = str[i++];
            }
            postfix_form[postfix_index++] = 'n';
            i--;
            int n = atoi(number);
            arr[number_count++] = n;
            memset(number, 0, sizeof(char) * k);
            k = 0;
            flag = 0;
        }
        else if (str[i] == '('){
            push(stack, str[i]);
            flag = 1;
            open_brackets++;
        }
        else if (str[i] == ')'){
            if (flag)
                error(1);
            while (!is_empty(stack) && peek(stack) != '(')
                postfix_form[postfix_index++] = pop(stack);
            if (is_empty(stack))
                error(1);
            pop(stack);
            close_brackets++;
        }
        else if (str[i] == '+' || str[i] == '-' || str[i] == '/' || str[i] == '*') {
            while (!is_empty(stack) && get_priority(peek(stack)) >= get_priority(str[i]))
                postfix_form[postfix_index++] = pop(stack);
            push(stack, str[i]);
            sign_count++;
            flag = 0;
        }
        else
            error(1);
    }
    if (number_count != sign_count + 1 || open_brackets != close_brackets)
        error(1);
    while (!is_empty(stack))
        postfix_form[postfix_index++] = pop(stack);
    return postfix_form;
}

int calculate(char str[], int str_len, int arr[], Stack_t *stack){
    int j = 0;
    for (int i = 0; i < str_len; i++){
        if (str[i] == 'n')
            push(stack, arr[j++]);
        else{
            int num1 = pop(stack);
            int num2 = pop(stack);
            if (str[i] == '+')
                push(stack, num1 + num2);
            else if (str[i] == '-')
                push(stack, num2 - num1);
            else if (str[i] == '/'){
                if (!num1)
                    error(0);
                push(stack, num2 / num1);
            }
            else if (str[i] == '*')
                push(stack, num1 * num2);
        }
    }
    return pop(stack);
}

int main(){
    char str[MAX_SIZE + 1] = {0};
    gets(str);
    int str_len = strlen(str);
    int arr[MAX_SIZE] = {0};
    Stack_t stack;
    stack.size = 0;
    char *postfix_form = get_postfix_form(&stack, str, str_len, arr);
    int postfix_form_len = strlen(postfix_form);
    int result = calculate(postfix_form, postfix_form_len, arr, &stack);
    printf("%d", result);
    free(postfix_form);
    return 0;

}