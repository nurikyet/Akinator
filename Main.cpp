#include <stdio.h>

#include "Akinator.h"
#include "Types.h"

int main()
{
    Tree tree = {};
    TreeCtor(&tree);

    FILE* DataBase = fopen(DATA_BASE, "r+");   // this is krivo!!!

    AkinatorInteractive(DataBase, &tree);

    TreeDtor(&tree);
    return 0;
}
