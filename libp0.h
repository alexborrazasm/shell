#ifndef LIBP0_H
#define LIBP0_H

#include "types.h" // For tLists and tArgs
#include "historicList.h"

// authors [-l|-n]

/* Prints the names and logins of the program authors. authors -l prints
   only the logins and authors -n prints only the names.
*/
void cmdAuthors(tArgs args, tLists *L);

// pid

/* Prints the pid of the process executing the shell.
 */
void cmdPid(tArgs args, tLists *L);

// ppid

/* Prints the pid of the shell’s parent process.
 */
void cmdPpid(tArgs args, tLists *L);

// cd [dir]

/* Changes the current working directory of the shell to dir (using the
   chdir system call). When invoked without arguments it prints the current
   working directory (using the getcwd system call.
*/
void cmdChdir(tArgs args, tLists *L);

// date [-t|-d]

/* Prints the current date in the format DD/MM/YYYY and the current
   time in the format hh:mm:ss.
     -date -d: Prints the current date in the format DD/MM/YYYY.
     -date -t: Prints and the current time in the format hh:mm:ss.
*/
void cmdDate(tArgs args, tLists *L);

// historic [N|-N]

/* Shows the historic of commands executed by this shell. In order to do this.
     – historic: Prints all the commands that have been input with their
       order number.
     – historic N: Repeats command number N (from historic list).
     – historic -N Prints only the lastN commands.
*/
void cmdHistoric(tArgs args, tLists *L);

// open [file] mode
void cmdOpen(tArgs args, tLists *L);
/* Opens a file and adds it (together with the file descriptor and the opening
   mode to the list of shell open files. For the mode we’ll use cr for O CREAT,
   ap for O APPEND, ex for O EXCL, ro for O RDONLY, rw for O RDWR, wo for O
   WRONLY and tr for O TRUNC.
   Open without arguments lists the shell open files. For each file it lists its
   descriptor, the file name and the opening mode. The shell will inherit
   from its parent process open descriptors 0, 1 and 2 (stdin, stout, and
   stderr).
*/
void cmdOpen(tArgs args, tLists *L);

// close [df]
// void
/* Closes the df file descriptor and eliminates the corresponding item from
   the list.
*/
void cmdClose(tArgs args, tLists *L);

// dup [df]
// void
/* Duplicates the df file descriptor (using the dup system call, creating the
   corresponding new entry on the file list.
*/

// infosys
/* Prints information on the machine running the shell (as obtained via
   the uname system call/library function).
*/
void cmdInfosys(tArgs args, tLists *L);

// help [cmd]
/* help: Displays a list of available commands.
   help cmd: Gives a brief help on the usage of command cmd.
*/
void cmdHelp(tArgs args, tLists *L);

// quit
// exit
// bye
/* Ends the shell.
 */
void cmdExit(tArgs args, bool *end);

#endif
