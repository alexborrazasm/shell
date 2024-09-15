#ifndef SHELL_H
#define SHELL_H

#include "types.h"
#include "historicList.h"
#include "libp0.h"

void printPrompt();

bool readInput(tLists *L);

void processInput(tLists L, bool *end);

void selectCommand(tArgs args, char* input, tLists L, bool *end);

void freeHistoryList(tListH* list);

int stringCut(char* input, char* parts[]);

#endif // SHELL_H