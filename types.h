#define COMMAND_LENGTH_LIMIT 25

typedef struct tItemL {
    char command[COMMAND_LENGTH_LIMIT];
    int arguments;
} tItemL;