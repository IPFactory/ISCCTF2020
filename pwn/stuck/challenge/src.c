#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void win() {
    execl("/bin/cat", "cat", "flag.txt");
    exit(0);
}

int main() {
    char name[0x61];

    printf("What's your name?\n> ");
    gets(name);
    printf("\nHello %s, I'm full stuck engineer!!!!!.\n", name);
}

__attribute__((constructor))
void setup() {
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    alarm(60);
}
