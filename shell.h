#ifndef SHELL_H
#define SHELL_H

#include "types.h"
#include "historicList.h"
#include "filesList.h"
#include "libp0.h"

void printPrompt();

bool readInput(tLists *L);

void processInput(tLists L, bool *end);

void selectCommand(tArgs args, char *input, tLists L, bool *end);

void freeHistoryList(tListH *list);

void freeFileList(tListF *list);

int stringCut(char *input, char *parts[]);

void printError(char *name, char *msg);

void pPrintError(char *name);

int getCommandsLen();

void getCommands(tCommand *pointer);

#endif // SHELL_H