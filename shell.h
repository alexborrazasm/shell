#include "libp0.h"

void printPrompt();

bool readInput(tLists *L);

void processInput(tLists L, bool *end);

void freeHistoryList(tListH* list);