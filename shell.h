#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "historyList.h"

void printPrompt();

void readInput(int *nArgs, char* input, char* args[], tListH *history);

bool processInput();

int stringCut(char* input, char* parts[]); // free me please!!!!!

