#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

bool authenticate(long pass, char* str) {
    if (pass == 0xc0ffee)
        return system(str), true;
    return system("cowsay cat flag.txt"), false;
}

void message() {
    puts("We trust you have received the usual lecture from the local System");
    puts("Administrator. It usually boils down to these three things:");
    puts("");
    puts("#1) Respect the privacy of others.");
    puts("#2) Think before you type.");
    puts("#3) With great power comes great responsibility.");
}

int main() {
    char buf[20] = {0};
    message();
    printf("> ");
    gets(buf);
}

__attribute__((constructor))
void setup() {
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    alarm(60);
}
