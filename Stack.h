#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include "Types.h"
#include "Error.h"

#define STACK_CONSTRUCT(nick, quantity) stack nick = {};          \
                                        StackCtor(&nick, quantity);      \
                                        nick.name = #nick;

#define LOG

#define STACK_DUMP(stk) StackDump(LOG_FILE, stk, __func__, __FILE__, __LINE__)
#define VERIFY(stk) {int error = StackOk(LOG_FILE, stk);              \
                    if (error != 0)                                   \
                        {                                             \
                        PrintError(LOG_FILE, error);                  \
                        STACK_DUMP(stk);                              \
                        return error;                                 \
                        }                                             \
                    }


//! @brief prints all stack data in file
//! @param [in] stk all structure data
//! @param [in] file the file from which it was called
//! @param [in] line the line from which it was called
//! @param [in] function the function from which it was called
//!
//! makes a complete printout of the stack and its data
void StackDump(FILE* fp, stack* stk, const char* func, const char* file, const int line);

//! @brief prints all stack data
//! @param [in] stk all structure data
//! @param [in] file the file from which it was called
//!
//! makes a complete printout of the stack and its data
void PrintStack(FILE* fp, const stack *stk);

//! @brief prints all stack data in console
//! @param [in] stk all structure data
//!
//! makes a complete printout of the stack and its data
void PrintInConsole(const stack *stk);

//! @brief deletes elements to the stack
//! @param [in]  stk all structure data
//! @param [out] ans stack element that is removed and returned
//!
//! returns and removes elements from the stack
elem_t StackPop(stack* stk);

//! @brief destroys all stack data
//! @param [in] stk all structure data
//!
//! resets all data and stack pointers
int StackDtor(stack* stk);

int StackRealloc(stack *stk, int newcapacity);

//! @brief creator of all data on the stk
//! @param [in] stk all structure data
//!
//! sets initial parameters for the structure and allocates memory for the initial stack size
int StackCtor(stack* stk, size_t cpt);

//
//! @brief adding elements to the stack
//! @param [in] stk all structure data
//!
//! adds elements to the stack and increases the stack size
int StackPush(stack* stk, const elem_t value);

/**
 * @brief unused elements constant
 *
 * if the stack element is not yet defined, then this value is used as the value of the element
 */

/**
 * @brief constant realloc
 *
 * denotes how many times the stack size needs to be increased when new memory is allocated
 */
const int REALLOC_MORE_MEMORY_MULTIPLIER = 3;

/**
 * @brief constant realloc
 *
 * denotes how many times the stack size needs to be reduced when allocating new memory
 */
const int REALLOC_LESS_MEMORY_MULTIPLIER = 2;

/**
 * @brief               Elements needed to work in a stack
 * @param stack_first   canary at the beginning of the struct
 * @param size          stack size
 * @param capacity      stack volume
 * @param data          data array
 * @param data_hash     data hash
 * @param struct_hash   stack hash
 * @param stack_last    canary at the ending of the struct
 */
struct stack
    {
    const char* name;

    int size;
    int capacity;
    elem_t* data;
    };

enum class Error
    {
    NULL_ERRORS         = 0,
    ERROR_SIZE          = 1 << 0,
    ERROR_CAPACITY      = 1 << 1,
    ERROR_DATA          = 1 << 2,
    ERROR_MEMORY        = 1 << 3,
    ERROR_DATA_CANARY   = 1 << 4,
    ERROR_STRUCT_CANARY = 1 << 5,
    ERROR_STRUCT        = 1 << 6,
    ERROR_FILE          = 1 << 7
    };

#endif // STACK_H_INCLUDED
