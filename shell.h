#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "historyList.h"
#include "p0lib.h"

void printPrompt();

void readInput(char* input, tArgs *args, tListH *history);

void processInput(tArgs args, tListH history, bool *end);

int stringCut(char* input, char* parts[]); // free me please!!!!!

void freeHistoryList(tListH* list);