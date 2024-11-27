#ifndef LIBP3_H
#define LIBP3_H

#include "types.h"

// getuid
void cmdGetuid(tArgs args, tLists *L);

// setuid [-l] id
void cmdSetuid(tArgs args, tLists *L);

// showvar v1 v2 ...
void cmdShowvar(tArgs args, tLists *L);

// changevar [-a|-e|-p] var val
void cmd(tArgs args, tLists *L);

// subsvar [-a|-e] v1 v2 val
void cmd(tArgs args, tLists *L);

// environ [-environ|-addr] 
void cmd(tArgs args, tLists *L);

// fork
void cmd(tArgs args, tLists *L);

// search [-add dir -del dir -clear -path]
void cmd(tArgs args, tLists *L);

// 
void cmd(tArgs args, tLists *L);

//
void cmd(tArgs args, tLists *L);

//
void cmd(tArgs args, tLists *L);

//
void cmd(tArgs args, tLists *L);

//
void cmd(tArgs args, tLists *L);

//
void cmd(tArgs args, tLists *L);

//
void cmd(tArgs args, tLists *L);

#endif // LIBP3_H