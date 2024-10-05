#ifndef LIBP1_H
#define LIBP1_H

#include "types.h"
#include "libp0.h"

// creates a file
void cmdMakefile(tArgs args, tLists *L);

// creates a directory
//void cmdMakedir(tArgs args, tLists *L);

// gives information on files or directories
//void cmdListfile(tArgs args, tLists *L);

// prints current working directory
void cmdCwd(tArgs args, tLists *L); 

// lists directories contents
//void cmdListdir(tArgs args, tLists *L); 

// lists directories recursively (subdirectories after)
//void cmdReclist(tArgs args, tLists *L); 

// lists directories recursively (subdirectories before)
//void cmdRevlist(tArgs args, tLists *L); 

// deletes files and/or empty directories
//void cmdErase(tArgs args, tLists *L); 

// deletes files and/pr non empty directories recursively
//void cmdDelrec(tArgs args, tLists *L); 

#endif // LIBP1_H