#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Types.h"
#include "Stack.h"
#include "logfile.h"
#include "Error.h"

static elem_t POISON = (Node*) 0xDEADBEEF;

int StackCtor(struct stack* stk, size_t cpt)
    {
    assert(cpt);
    stk->capacity = (int)cpt;

    int data_size = (stk->capacity) * sizeof(elem_t);

    stk->data = (elem_t*) calloc(data_size, sizeof(char));
    if (stk->data == nullptr)
                {
                stk->capacity = 0;
                fprintf(LOG_FILE, "Function %s: not enough memory to allocate", __func__);
                return (int)Error::ERROR_MEMORY;
                }

    stk->size = 0;

    return (int)Error::NULL_ERRORS;
    }

//-----------------------------------------------------------------------------

int StackDtor(struct stack* stk)
    {
    VERIFY(stk)

    free(stk->data);
    stk->data     = nullptr;
    stk->size     = 0;
    stk->capacity = 0;

    return (int)Error::NULL_ERRORS;
    }

//-----------------------------------------------------------------------------

int StackPush(struct stack* stk, const elem_t value)
    {
    VERIFY(stk)

    if ((stk->size) >= (stk->capacity))
        {
        int new_capacity = REALLOC_MORE_MEMORY_MULTIPLIER*(stk->capacity);

        StackRealloc(stk, new_capacity);
        }
    (stk->data)[(stk->size)] = value;
    (stk->size)++;

    VERIFY(stk)
    return (int)Error::NULL_ERRORS;
    }

//-----------------------------------------------------------------------------

elem_t StackPop(struct stack* stk)
    {
    (stk->size)--;
    elem_t retvalue = (stk->data)[(stk->size)];
    (stk->data)[(stk->size)] = POISON;

    if ((stk->size) == (stk->capacity)/(REALLOC_LESS_MEMORY_MULTIPLIER))
        {
        int new_capacity = (stk->capacity)/REALLOC_LESS_MEMORY_MULTIPLIER;
        if (new_capacity == 0)
            {
            new_capacity = 1;
            }
        StackRealloc(stk, new_capacity);
        }
    return retvalue;
    }

//-----------------------------------------------------------------------------

void StackDump(FILE* fp, struct stack* stk, const char* func, const char* file, const int line)
    {
    assert(stk);
    assert(func);
    assert(file);
    assert(stk->data);

    fprintf(fp, "---------------------------------------------------\n");
    fprintf(fp, "START\n");

    fprintf(fp, "Stack[%p] \"stk\" from %s(%d) in function - %s.\n", stk,  file, line, func);
    fprintf(fp, "{\n");
    fprintf(fp, "size < %d\n", stk->size);
    fprintf(fp, "capacity = %d\n", stk->capacity);

    fprintf(fp, "data[%p]\n", stk->data);
    PrintStack(fp, stk);
    }

//-----------------------------------------------------------------------------

void PrintStack(FILE* fp, const struct stack *stk)
    {
    fprintf(fp, "stack %d > \n", stk->size);
    for (int i = 0; i < (stk->size); i++)
        {
        fprintf(fp, "data[%d] =", i);
        fprintf(fp, printfelem, stk->data[i]);
        fprintf(fp, "\n");
        }
    fprintf(fp, "All DATA\n");
    for (int i = 0; i < stk -> capacity; i++)
        {
        fprintf(fp, "data[%d] =", i);
        fprintf(fp, printfelem, stk->data[i]);
        fprintf(fp, "\n");
        }

    fprintf(fp, "\n");
    }

//-----------------------------------------------------------------------------

int StackRealloc(struct stack *stk, int new_capacity)
    {
    printf("start realloc\n");
    assert(stk);
    VERIFY(stk)

    int data_size = new_capacity * sizeof(elem_t*);

    elem_t *check = (elem_t*)realloc(stk->data, data_size);
    if (check != nullptr)
        {
        stk->data = check;
        }
    else
        {
        STACK_DUMP(stk);
        fprintf(LOG_FILE, "Function %s: not enough memory to allocate", __func__);
        return (int) Error::ERROR_MEMORY;
        }

    elem_t* left_elem = (elem_t*)((char*)(stk->data));

    stk->data     = left_elem;
    stk->capacity = new_capacity;

    VERIFY(stk)
    return (int)Error::NULL_ERRORS;
    }

//-----------------------------------------------------------------------------

void PrintInConsole(const struct stack *stk)
    {
    printf("stack %d > \n", stk->size);
    for (int i = 0; i < (stk->size); i++)
        {
        printf("data[%d] =", i);
        printf(printfelem, stk->data[i]);
        printf("\n");
        }
    printf("All DATA\n");
    for (int i = 0; i < stk -> capacity; i++)
        {
        printf("data[%d] =", i);
        printf(printfelem, stk->data[i]);
        printf("\n");
        }

    printf("\n");
    }
