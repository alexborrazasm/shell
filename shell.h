#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "historyList.h"

void printPrompt();

void readInput(char* input, char* args[], int *nArgs, tListH *history);

bool processInput();

int stringCut(char* input, char* parts[]); // free me please!!!!!

