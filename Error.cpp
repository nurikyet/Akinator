#include <stdio.h>
#include <assert.h>

#include "Types.h"
#include "Stack.h"
#include "logfile.h"
#include "Error.h"

void PrintError(FILE* fp, int result)
    {
    assert(fp);

    if ((result & (int)Error::ERROR_CAPACITY) != 0)
        fprintf(fp, "Capacity must be > 0\n");

    if ((result & (int)Error::ERROR_SIZE) != 0)
        fprintf(fp, "Size must be <= capacity\n");

    if ((result & (int)Error::ERROR_DATA) != 0)
        fprintf(fp, "address of data != nullptr\n");

    if ((result & (int)Error::ERROR_STRUCT) != 0)
        fprintf(fp, "address of struct != nullptr\n");
    }

//-----------------------------------------------------------------------------

int StackOk(FILE* fp, struct stack* stk)
    {
    int result = 0;

    if (!stk->capacity)
        {
        result |= (int)Error::ERROR_CAPACITY;
        }
    if (stk->capacity < 0)
        {
        result |= (int)Error::ERROR_CAPACITY;
        }
    if (stk->size > stk->capacity)
        {
        result |= (int)Error::ERROR_SIZE;
        }
    if (!stk->data)
        {
        result |= (int)Error::ERROR_DATA;
        }

    if (stk == nullptr)
        {
        result |= (int)Error::ERROR_STRUCT;
        }

    PrintError(fp, result);
    return result;
    }
