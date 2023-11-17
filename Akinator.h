#ifndef AKINATOR_H_INCLUDED
#define AKINATOR_H_INCLUDED
#include "Types.h"

static const int MAX_NAME_LEN   = 255;

struct Node
{
    char* data;

    Node* left;
    Node* right;
};

struct DataBase
{
    char* information;
    int size;
};

struct Tree
{
    DataBase DataBase = {};

    Node* root;
    int size;
};

enum Mode
{
    DATABASE_DRAWING  = 1,
    GAME              = 2,
    OBJECT_DEFINITION = 3,
    COMPARISON        = 4
};

enum Answers
{
    YES,
    NO,
    UNKNOWN_ANSWER
};

enum Errors
{
    NULL_ERRORS          = 0,
    ERROR_MEMORY         = 1,
    ERROR_UNKNOWN_ANSWER = 2,
    ERROR_LEN            = 3,
    ERROR_OF_BASE        = 4,
    ERROR_OBJECT         = 5
};

static const char* DATA_BASE           = "DataBase.txt";
static const int POISON_ELEMENT = -777;
static const int MAX_STACK_LEN  = 30;

void AkinatorInteractive(FILE* DataBase, Tree* tree);
void PrintNodePre(FILE* fp, Node* n);
void PrintNodeIn(Node* n);
void PrintNodePost(Node* n);
void GraphicDump(Tree* tree);
void PrintElement(FILE* fp, Node* n);
void PrintAdress(FILE* fp, Node* n);
void PlayGame(Tree* tree);
void BaseCtor(Tree* tree, FILE* fp);
void TreeCtor(Tree* tree);
void AddDataBase(FILE* fp, Tree* tree);
void StartPlayGame(Node* node, Tree* tree);
void GetDefinition(Tree* tree);
void AkinatorStatement(Node* main_element, Node* element);
void GetComparison(Tree* tree);
void AkinatorGame(Tree* tree);
void AddNewObject(Node* node);
void GiveDifferences(struct stack* stk1, struct stack* stk2);

int NodeDtor(Node* n);
int TreeDtor(Tree* tree);
int GetMode();
int DataBaseProcess(Tree *tree);
int GetSizeOfFile(FILE* fp);
int PlayAgain();

Node* NodeCtor(Node* left, Node* right);
Node* StackDefenition(char* object, struct stack* stk, Node* node);

char* GetNameOfObject();
char* GetCharacteristic(char* first, char* second);

Answers GetAnswer(Node* n);
#endif // AKINATOR_H_INCLUDED
