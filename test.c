#include"shell.h"

void separator();

void testPrompt();

void testStringCut();

int main() {

    separator();
    testPrompt();
    separator();
    testStringCut();
    separator();
    

    return 0;
}

void separator() {
    puts("\n------------------------------------------------------\n");
}

void testPrompt() {
    puts("Test prompt");
    printPrompt();
    puts("");
}

void testStringCut() {
    puts("Test stringCut: \"esto es una string\"");
    char text[] = "esto es una string";
    char* parts[6]; 
    int nParts;
    
    nParts = stringCut(text, parts);

    for (int i = 0; i < nParts; i++) {
        puts(parts[i]);
    }
    printf("Número de partes %d\n", nParts);
}

