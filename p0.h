#include <stdbool.h>

void dateCmd (int numParts, char* parts[]);//Date [-t|-d]
void date();
void time();

void authorsCmd (int numParts, char* parts[]);//Authors [-l|-n]
void autName();
void autLogin();

void exitCmd(bool finished);//Exit/Quit/Bye