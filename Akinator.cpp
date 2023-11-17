#define TX_USE_SPEAK
#include "TxLib.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "Akinator.h"
#include "Types.h"
#include "logfile.h"
#include "Stack.h"
#include "FunnyColours.h"

#define ABOBA printf("<<ABOBA - %s(%d) in %s>>\n\n", __func__, __LINE__, __FILE__);

char INITAL_ELEMENT[12]   = "Unknown who";

int main()
{
    Tree tree = {};
    TreeCtor(&tree);

    FILE* DataBase = fopen(DATA_BASE, "r+");

    if (DataBase != nullptr)
    {
        BaseCtor(&tree, DataBase);

        DataBaseProcess(&tree);
        assert(DataBase != nullptr);
    }

    else
    {
        fclose(DataBase);
        DataBase = fopen(DATA_BASE, "w+t");

        TreeCtor(&tree);
        assert(DataBase != nullptr);
    }

    int akinator = 1;
    while(akinator)
    {
        switch(GetMode())
        {
            case (Mode::DATABASE_DRAWING):
            {
                GraphicDump(&tree);
                break;
            }
            case (Mode::GAME):
            {
                AkinatorGame(&tree);
                break;
            }
            case (Mode::OBJECT_DEFINITION):
            {
                GetDefinition(&tree);
                break;
            }
            case (Mode::COMPARISON):
            {
                GetComparison(&tree);
                break;
            }
            default:
            {
                fprintf(stderr, "Unknown mode\n");
                break;
            }

        }


        akinator = PlayAgain();
    }

    fprintf(stderr, COLOUR_CYAN("Do you want to save the DataBase\n"));
    fprintf(stderr, COLOUR_CYAN("Enter Y - if if your answer is yes, enter N - if your answer is no.\n"));

    txSpeak("Õîòèòå ëè Âû ñîõðàíèòü áàçó äàííûõ?");

    char decision[] = "";
    scanf("%1s", decision);
    if (decision[0] == 'Y')
    {
        AddDataBase(DataBase, &tree);
    }
    TreeDtor(&tree);
    return 0;
}

void GetComparison(Tree* tree)
{
    assert(tree);

    printf(COLOUR_CYAN("Enter the first object\n"));
    txSpeak("Ââåäèòå ïåðâûé îáúåêò.");
    char* name1 = GetNameOfObject();
    struct stack stk1 = {};
    StackCtor(&stk1, MAX_STACK_LEN);

    Node* node1 = StackDefenition(name1, &stk1, tree->root);
    if (node1 == nullptr)
    {
        printf("There is no such element in the database\n");
        return;
    }

    printf(COLOUR_CYAN("Enter the name of the second object\n"));
    txSpeak("Ââåäèòå âòîðîé îáúåêò.");

    char* name2 = GetNameOfObject();
    struct stack stk2 = {};
    StackCtor(&stk2, MAX_STACK_LEN);

    Node* node2 = StackDefenition(name2, &stk2, tree->root);
    if (node2 == nullptr)
    {
        printf("There is no such element in the database\n");
        return;
    }

    GiveDifferences(&stk1, &stk2);

    StackDtor(&stk1);
    StackDtor(&stk2);

}

void GiveDifferences(struct stack* stk1, struct stack* stk2)
{
    assert(stk1);
    assert(stk2);

    if (stk1->data[1] == stk2->data[1])
    {
        printf(COLOUR_YELLOW("These objects %s and %s are similar in that they are both: "), ((Node*)(stk1->data[stk1->size - 1]))->data, ((Node*)(stk2->data[stk2->size - 1]))->data);
        txSpeak("Îáúåêòû %s è %s ïîõîæè òåì, ÷òî: ", ((Node*)(stk1->data[stk1->size - 1]))->data, ((Node*)(stk2->data[stk2->size - 1]))->data);
    }

    int both_characteristic = 0;
    for(int i = 0; (i < stk1->size - 1) and (i < stk2->size - 1) and (stk1->data[i + 1] == stk2->data[i + 1]); ++i)
    {
        if (stk1->data[i]->right == stk1->data[i + 1])
        {
            printf(COLOUR_YELLOW("not "));
            txSpeak("íå ");
        }

        printf(COLOUR_YELLOW("%s "), stk1->data[i]->data);
        txSpeak("%s ", stk1->data[i]->data);
        ++both_characteristic;
    }

    if (stk1->data[1] == stk2->data[1])
    {
        printf(COLOUR_YELLOW("But "));
        txSpeak("íî ");
    }
    printf(COLOUR_YELLOW("%s "), ((Node*) (stk1->data[stk1->size - 1]))->data);
    txSpeak("%s ", ((Node*) (stk1->data[stk1->size - 1]))->data);

    for(int i = both_characteristic; i < stk1->size - 1; ++i)
    {
        if (stk1->data[i]->right == stk1->data[i + 1])
        {
            printf(COLOUR_YELLOW("not "));
            txSpeak("íî ");
        }

        printf(COLOUR_YELLOW("%s"), stk1->data[i]->data);
        txSpeak("%s ", stk1->data[i]->data);
        printf(COLOUR_YELLOW(", "));
    }

    printf(COLOUR_YELLOW("and %s "), ((Node*) (stk2->data[stk2->size - 1]))->data);
    txSpeak("à ", ((Node *) (stk2->data[stk2->size - 1]))->data);

    for (int i = both_characteristic; i < stk2->size - 1; ++i)
    {
        if (stk2->data[i]->right == stk2->data[i + 1])
        {
            printf(COLOUR_YELLOW("not "));
            txSpeak("íå ");
        }

        printf(COLOUR_YELLOW("%s"), stk2->data[i]->data);
        txSpeak("%s ", stk2->data[i]->data);

        if (i != stk2->size - 2)
        {
            printf(", ");
        }
    }
    printf("\n");
}

void GetDefinition(Tree* tree)
{
    char* name = (char*)calloc(MAX_NAME_LEN, sizeof(char));
    if (name == nullptr)
    {
        printf(COLOUR_RED("Name can't be nullptr\n"));
    }

    printf(COLOUR_CYAN("Enter the object whose definition you want to know\n"));
    txSpeak("Ââåäèòå íàçâàíèå îáúåêòà, íàçâàíèå êîòîðîãî õîòèòå óçíàòü");

    rewind(stdin);
    fgets(name, MAX_NAME_LEN, stdin);
    name[strlen(name) - 1] = '\0';

    struct stack stk = {};
    StackCtor(&stk, MAX_STACK_LEN);

    Node* node  = StackDefenition(name, &stk, tree->root);
    if (node == nullptr)
    {
        printf("There is no such element in the database\n");
        return;
    }

    printf(COLOUR_MAGENTA("%s - "), ((Node*) (stk.data[stk.size - 1]))->data);
    txSpeak("%s - ", ((Node*) (stk.data[stk.size - 1]))->data);

    for (int i = 0; i < stk.size - 1; ++i)
    {
        if (stk.data[i]->right == stk.data[i + 1])
        {
            printf(COLOUR_MAGENTA("not "));
            txSpeak("íå ");
        }

        printf(COLOUR_MAGENTA("%s"), stk.data[i]->data);
        txSpeak("%s", stk.data[i]->data);

        if (i != stk.size - 2)
        {
            printf(COLOUR_MAGENTA(", "));
        }
    }
    printf("\n");

    StackDtor(&stk);

}

Node* StackDefenition(char* object, struct stack* stk, Node* node)
{
    assert(node);

    StackPush(stk, node);
    if (strcmp(node->data, object) == 0)
    {
        return node;
    }

    if ((node->left  != nullptr) and (StackDefenition(object, stk, node->left) != nullptr))
    {
        return (Node*)(stk->data[stk->size - 1]);
    }

    if ((node->right != nullptr) and (StackDefenition(object, stk, node->right) != nullptr))
    {
        return (Node*)(stk->data[stk->size - 1]);
    }

    StackPop(stk);

    return nullptr;
}

int DataBaseProcess(Tree* tree)
{
    assert(tree != nullptr);

    struct stack stk = {};
    StackCtor(&stk, MAX_STACK_LEN);

    Node* node = NodeCtor(nullptr, nullptr);
    tree->root = node;

    char* symbol = tree->DataBase.information;
    if (*symbol != '(')
    {
        fprintf(stderr, "Your DataBase is not suitable for reading - %d, check the correctness of the entered data\n", __LINE__);
    }
    StackPush(&stk, nullptr);
    ++symbol;

    char* words_ending = nullptr;

    while((symbol - tree->DataBase.information) < tree->DataBase.size)
    {
        switch (*symbol)
        {
            case '(':
                StackPush(&stk, node);

                if(node->left == nullptr)
                {
                    node->left = NodeCtor(nullptr, nullptr);
                    node = node->left;
                }
                else
                {
                    if(node->right != nullptr)
                    {
                        fprintf(stderr, "Your DataBase is not suitable for reading - %d, check the correctness of the entered data\n", __LINE__);
                        return (int)Errors::ERROR_OF_BASE;
                    }
                    node->right = NodeCtor(nullptr, nullptr);
                    node = node->right;
                }
                ++symbol;
            break;

            case ')':
                if (stk.size == 0)
                {
                    if (symbol - tree->DataBase.information != tree->DataBase.size - 1)
                    {
                        fprintf(stderr, "Your DataBase is not suitable for reading - %d, check the correctness of the entered data\n", __LINE__);
                        return (int)Errors::ERROR_OF_BASE;
                    }

                    break;
                }
                node = (Node*) StackPop(&stk);
                ++symbol;
            break;

            case '*':

                words_ending = strchr(symbol + 1, '*');
                if(words_ending == nullptr)
                {
                    fprintf(stderr, "Your DataBase is not suitable for reading, check the correctness of the entered data\n");
                    return (int)Errors::ERROR_OF_BASE;
                }

                if (words_ending - (symbol + 1) > MAX_NAME_LEN)
                {
                    printf("Your question is too big\n");
                    return (int)Errors::ERROR_LEN;
                }
                else
                {
                    strncpy(node->data, symbol + 1, (size_t) (words_ending - (symbol + 1)));
                    symbol = words_ending + 1;
                }
            break;

            default:
                fprintf(stderr, "Your DataBase is not suitable for reading, check the correctness of the entered data\n");
                return (int)Errors::ERROR_OF_BASE;
        }
    }

    StackDtor(&stk);

    return (int)Error::NULL_ERRORS;
}

void TreeCtor(Tree* tree)
{
    assert(tree);

    tree->root = NodeCtor(nullptr, nullptr);
    tree->root->data = INITAL_ELEMENT;
}

void BaseCtor(Tree* tree, FILE* fp)
{
    assert(tree);

    int file_size = GetSizeOfFile(fp);
    tree->DataBase.information = (char*)calloc(file_size, sizeof(char));
    if (tree->DataBase.information == nullptr)
    {
        printf("Error of memory: tree->DataBase.information = nullptr");
    }

    tree->DataBase.size = (int)fread(tree->DataBase.information, sizeof(char), file_size, fp);
}

void AddDataBase(FILE* fp, Tree* tree)
{
    assert(tree);
    assert(fp != nullptr);

    fseek (fp, 0, SEEK_SET);
    PrintNodePre(fp, tree->root);
    //PrintNodeIn(tree->root);
    fclose(fp);
}

Node* NodeCtor(Node* left, Node* right)
{

    Node* n = (Node*)calloc(1, sizeof(Node));

    n->data  = (char*) calloc(MAX_NAME_LEN, sizeof(char));
    if (n->data == nullptr)
    {
        fprintf(stderr, "Error of memory data of node is nullptr\n");
    }

    n->left  = left;
    n->right = right;

    return n;
}

int NodeDtor(Node* n)
{
    if (n->left != nullptr)
    {
        NodeDtor(n->left);
    }

    if (n->right)
    {
        NodeDtor(n->right);
    }

    n->left  = nullptr;
    n->right = nullptr;
    n->data  = nullptr;

    free(n);

    return (int)Errors::NULL_ERRORS;
}

int TreeDtor(Tree* tree)
{
    assert(tree);

    NodeDtor(tree->root);

    tree->root = nullptr;
    tree->size = POISON_ELEMENT;

    return (int)Errors::NULL_ERRORS;
}

void PrintNodePre(FILE* fp, Node* n)
{
    assert(n);
    assert(fp != nullptr);

    fprintf(fp, "(");
    fprintf(fp, "^");
    fprintf(fp, "%s", n->data);
    fprintf(fp, "^");

    if (n->left != nullptr)
    {
        PrintNodePre(fp, n->left);
    }

    if (n->right != nullptr)
    {
        PrintNodePre(fp, n->right);
    }

    fprintf(fp, ")");
}

void PrintNodeIn(Node* n)
{
    assert(n);

    fprintf(stderr, "(");
    fprintf(stderr, "^");
    fprintf(stderr, "%s", n->data);
    fprintf(stderr, "^");

    if (n->left != nullptr)
    {
        PrintNodeIn(n->left);
    }

    if (n->right != nullptr)
    {
        PrintNodeIn(n->right);
    }

    fprintf(stderr, ")");
}

void PrintNodePost(Node* n)
{
    if (n == NULL)
    {
        printf("nil\n");
        return;
    }

    printf("(\n");
    PrintNodePost(n->left);
    PrintNodePost(n->right);
    printf("%s\n", n->data);
    printf(")\n");
}

void GraphicDump(Tree* tree)
{
    FILE* fp = fopen("list.dot", "wb");
    fprintf(fp, "digraph G{\n");
    fprintf(fp, "  rankdir=HR;\n");

    PrintElement(fp, tree->root);
    PrintAdress(fp, tree->root);

    fprintf(fp, "}\n");

    fclose(fp);

    char graph[MAX_NAME_LEN] = {};
    sprintf(graph, "dot -Tpng list.dot -o list.png");
    system(graph);
    system("start");

}

void PrintElement(FILE* fp, Node* n)
{
    if (n == NULL)
    {
        return;
    }

    fprintf(fp, "  Node_%p [shape=record, style=\"filled\", fillcolor=\"#F08080\", label=\" %s\"];\n", n, n->data);
    PrintElement(fp, n->left);
    PrintElement(fp, n->right);
}

void PrintAdress(FILE* fp, Node* n)
{
    if (n == NULL)
    {
        return;
    }

    if (n->left != NULL)
    {
        fprintf(fp, "Node_%p -> Node_%p[color=\"red\"]\n", n, n->left);

    }
    if (n->right != NULL)
    {
        fprintf(fp, "Node_%p -> Node_%p[color=\"red\"]\n", n, n->right);
    }
    PrintAdress(fp, n->left);
    PrintAdress(fp, n->right);
}

char* GetNameOfObject()
{
    char* name = (char*)calloc(MAX_NAME_LEN, sizeof(char));
    if (name == nullptr)
    {
        printf(COLOUR_RED("Name can't be nullptr"));
    }

    printf(COLOUR_CYAN("What or who was it?\n"));
    txSpeak("Êòî èëè ÷òî ýòî?");

    rewind(stdin);
    fgets(name, MAX_NAME_LEN, stdin);
    name[strlen(name) - 1] = '\0';

    return name;
}

char* GetCharacteristic(char* first, char* second)
{
    printf(COLOUR_CYAN("What is the difference between a %s and a %s?\n"), second, first);
    txSpeak("×åì %s îòëè÷àåòñÿ îò %s?", second, first);

    char* characteristic = (char*)calloc(MAX_NAME_LEN, sizeof(char));
    if (characteristic == nullptr)
    {
        printf(COLOUR_RED("Name can't be nullptr"));
    }

    fseek (stdin, 0, SEEK_SET);
    fgets(characteristic, MAX_NAME_LEN, stdin);
    characteristic[strlen(characteristic) - 1] = '\0';
    return characteristic;
}

int GetMode()
{
    int mode = 0;
    printf(COLOUR_CYAN("Select the mode of operation: 1 - DataBase drawing; 2 - game; 3 - object definition; 4 - comparison\n"));
    txSpeak("Ñïàñèáî, ÷òî ïðèçâàëè ÷óäåñíîãî äæèíà - Àêèíàòîðà!");

    scanf("%d", &mode);

    return mode;
}

Answers GetAnswer(Node* n)
{
    printf(COLOUR_GREEN("%s? Enter Y - if if your answer is yes, enter N - if your answer is no.\n"), n->data);
    txSpeak("Ýòîò îáúåêò %s?", n->data);

    char answer[MAX_NAME_LEN];
    scanf("%s", answer);

    if (strcmp(answer, "Y") == 0)
    {
        return Answers::YES;
    }

    if (strcmp(answer, "N") == 0)
    {
        return Answers::NO;
    }
    else
    {
        return Answers::UNKNOWN_ANSWER;
    }
}

void StartPlayGame(Node* node, Tree* tree)
{
    assert(tree);

    Answers answer = GetAnswer(node);

    while(answer == Answers::UNKNOWN_ANSWER)
    {
        printf(COLOUR_RED("Unknown answer, try again\n"));
        answer = GetAnswer(node);
    }

    if (answer == Answers::YES)
    {
        if (node->left == nullptr and node->right == nullptr)
        {
            printf(COLOUR_RED("ARTIFICIAL INTELLIGENCE WON THIS WAR\n"));
            txSpeak("ÈÑÊÓÑÑÒÂÅÍÍÛÉ ÈÍÒÅËËÅÊÒ ÏÎÁÅÄÈË Â ÝÒÎÉ ÂÎÉÍÅ, ÑÊËÎÍÈÒÅÑÜ ËÞÄÈ!!!");
            return;
        }

        else
        {
            StartPlayGame(node->left, tree);
        }
    }


    if (answer == Answers::NO)
    {
        if (node->right == nullptr and node->left == nullptr)
        {
            NewElement(node);
        }
        else
        {
            StartPlayGame(node->right, tree);

        }
    }

}

void NewElement(Node* node)
{
    char *new_object = GetNameOfObject();
    char *difference = GetCharacteristic(node->data, new_object);

    node->left  = NodeCtor(nullptr, nullptr);
    node->right = NodeCtor(nullptr, nullptr);

    node->left->data  = new_object;
    node->right->data = node->data;
    node->data = difference;
}

void PlayGame(Tree* tree)
{
    assert(tree);
    Node* node = tree->root;

    StartPlayGame(node, tree);
}

int GetSizeOfFile(FILE* fp)
{
    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    fseek (fp, 0, SEEK_SET);

    return size;
}

int PlayAgain()
{
    fprintf(stderr, COLOUR_CYAN("Do you want to continue the game\n"));
    fprintf(stderr, COLOUR_CYAN("Enter Y - if if your answer is yes, enter N - if your answer is no.\n"));
    txSpeak("Õîòèòå ëè âû ïðîäîëæèòü èãðó?");

    char decision[MAX_NAME_LEN] = {};
    scanf("%1s", decision);

    return (strcmp(decision, "Y") == 0);
}

void AkinatorGame(Tree* tree)
{
    assert(tree);

    Node* node = tree->root;

    int check = 0;
    while(check == 0)
    {
        Answers answer = GetAnswer(node);

        while(answer == Answers::UNKNOWN_ANSWER)
        {
            printf(COLOUR_RED("Incorrect answer option, try to enter it again\n"));
            answer = GetAnswer(node);
        }

        switch(answer)
        {
            case Answers::YES:
            {
                if (node->left == nullptr and node->right == nullptr)
                {
                    printf(COLOUR_RED("ARTIFICIAL INTELLIGENCE WON THIS WAR\n"));
                    txSpeak("ÈÑÊÓÑÑÒÂÅÍÍÛÉ ÈÍÒÅËËÅÊÒ ÏÎÁÅÄÈË Â ÝÒÎÉ ÂÎÉÍÅ, ÑÊËÎÍÈÒÅÑÜ ËÞÄÈ!!!");

                    return;
                }

                else
                {
                    node = node->left;
                }
            }
            break;

            case Answers::NO:
            {
                if (node->left == nullptr and node->right == nullptr)
                {
                    NewElement(node);
                    return;
                }
                else
                {
                    node = node->right;
                }

            }
            break;

            case Answers::UNKNOWN_ANSWER:
                printf("Unknown answer\n");
            break;

            default:
            break;

        }
    }
}
