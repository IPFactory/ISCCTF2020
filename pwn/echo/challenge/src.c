#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>

int main() {
    FILE* flagfile = fopen("flag.txt", "r");
    if (flagfile == NULL)
        return err(1, "Oops! flag is broken!"),1;

    char *flag = malloc(61);
    fgets(flag, 62, flagfile);

    char buf[256] = {0};
    scanf("%255s", buf);
    printf(buf);
    putchar(10);
}

__attribute__((constructor))
void setup() {
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    alarm(60);
}
