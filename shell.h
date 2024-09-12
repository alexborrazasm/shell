#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "historyList.h"
#include "p0lib.h"

void printPrompt();

void readInput(char* input, tArgs *args, tListH *history);

bool processInput(tArgs args, tListH history);

int stringCut(char* input, char* parts[]); // free me please!!!!!
