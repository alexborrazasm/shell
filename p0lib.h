#include <stdbool.h>

void exitCmd(bool *end);


void dateCmd (tArgs args, tListH history);  //Date [-t|-d]
void date();
void timeC();

void pidCmd(tArgs args, tListH history);
void ppidCmd(tArgs args, tListH history);

void exitCmd(bool *end);

void authorsCmd (tArgs args, tListH history);
void autName();
void autLogin();

/*
void exitCmd(int numParts, bool *finished);//Exit/Quit/Bye
void exit(bool *finished);

void pidCmd(int numParts, char* parts[]);//Pid

void ppidCmd(int numParts, char* parts[]);//Ppid
*/