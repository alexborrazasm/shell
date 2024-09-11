#define MAX_BUFFER_INPUT 50
#define MAX_ARGS 10

typedef struct tItemH
{
    char command[MAX_BUFFER_INPUT];
} tItemH;

typedef struct tCommand {
    const char* name;
    void (*func)(int,  char **);
} tCommand;