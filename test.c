#include "shell.h"

void separator();

void testPrompt();

void testStringCut();

void printList(tListH list);

void testDynamicList();

int main()
{
    // separator();
    // testPrompt();
    // separator();
    // testStringCut();
    separator();
    testDynamicList();
    separator();

    return 0;
}

void separator()
{
    puts("\n------------------------------------------------------\n");
}

void testPrompt()
{
    puts("Test prompt");
    printPrompt();
    puts("");
}

void testStringCut()
{
    puts("Test stringCut: \"esto es una string\"");
    char text[] = "esto es una string";
    char *parts[6];
    int nParts;

    nParts = stringCut(text, parts);

    for (int i = 0; i < nParts; i++)
    {
        puts(parts[i]);
    }
    printf("Número de partes %d\n", nParts);
}

void printList(tListH list)
{
    tPosH pos;
    tItemH item;

    printf("(");
    if (!isEmptyList(list))
    {
        pos = first(list);
        while (pos != NULL)
        {
            item = getItem(pos, list);
            printf("Command: %s ", item.command);
            pos = next(pos, list);
        }
    }
    printf(")\n");
}

void testDynamicList()
{
    tListH list;
    tItemH item;

    /* create */
    createEmptyList(&list);
    printList(list);

    /* insert */
    strcpy(item.command, "Command 1");

    insertItem(item, NULL, &list);
    printList(list);

    strcpy(item.command, "Command 2");
    insertItem(item, NULL, &list);
    printList(list);

    strcpy(item.command, "Command 3");
    insertItem(item, NULL, &list);
    printList(list);

    strcpy(item.command, "Command 4");
    insertItem(item, NULL, &list);
    printList(list);

    strcpy(item.command, "Command 5");
    insertItem(item, NULL, &list);
    printList(list);

    /* remove */
    while (first(list) != NULL)
    {
        deleteAtPosition(first(list), &list);
        printList(list);
    }
}