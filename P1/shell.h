#ifndef SHELL_H
#define SHELL_H

#include "types.h"
#include "historicList.h"
#include "filesList.h"
#include "libp0.h"
#include "libp1.h"

void printPrompt();

bool readInput(tLists *L);

void processInput(tLists *L, bool *end);

void selectCommand(tArgs args, char *input, tLists *L, bool *end);

void freeHistoryList(tListH *list);

void freeFileList(tListF *list);

int stringCut(char *input, char *parts[]);

void printError(char *name, char *msg);

void pPrintError(char *name);

void pPrintErrorFile(char *name, char *filename);

int getCommandsLen();

void getCommands(tCommand *pointer);

void initFList(tListF *L);

bool stringToInt(char *str, int *n);

#endif // SHELL_H