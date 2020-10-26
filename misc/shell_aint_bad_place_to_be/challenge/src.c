#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int sh_exit(char **args) {
    puts("Bye");
    return 0;
}

int sh_cd(char **args) {
    if (chdir((args[1] != NULL? args[1]: "/")) != 0)
        perror("chdir");
    return 1;
}

int sh_ls(char **args) {
    const char *path = args[1]?args[1]:".";
    DIR* d = opendir(path);
    if (!d)
        return perror("opendir"), 1;

    struct dirent *e;
    while((e = readdir(d))) {
        if (e->d_name[0] == '.')
            continue;
        printf("%s ", e->d_name);
    }
    puts("");
    closedir(d);
    return 1;
}

int sh_pwd(char **_args) {
    char buf[1024];
    if (getcwd(buf, 1024))
        printf("%s\n", buf);
    return 1;
}

int sh_chroot(char **args) {
    if(chroot(args[1]?args[1]:"/"))
        perror("chroot");
    return 1;
}

int sh_mkdir(char **args) {
    if (!args[1])
        return puts("Usage: mkdir dirname"),1;

    if(mkdir(args[1], 0777))
        perror("mkdir");

    return 1;
}

int sh_cat(char **args) {
    if (!args[1])
        return puts("Usage: cat filepath"),1;

    int fd = open(args[1], O_RDONLY);
    if (fd < 0)
        return perror("open"), 1;
    char buf[0xff];
    int count;
    while((count = read(fd, buf, 0xff)))
        write(1, buf, count);

    return 1;
}

char *cmd_str[] = {
    "exit",
    "cd",
    "ls",
    "pwd",
    "chroot",
    "mkdir",
    "cat",
};

int (*cmd_fn[]) (char**) =  {
    &sh_exit,
    &sh_cd,
    &sh_ls,
    &sh_pwd,
    &sh_chroot,
    &sh_mkdir,
    &sh_cat,
};

int cmd_num() {
    return sizeof(cmd_str) / sizeof(char *);
}

void cmd_help() {
    puts("You can only use these commands");
    for (int i = 0; i < cmd_num(); i++) {
        printf("%s ", cmd_str[i]);
    }
    puts("");
}


int execute(char **args) {
    if (args[0] ==NULL)
        return 1;

    for (int i = 0; i < cmd_num(); i++) {
        if (strcmp(args[0], cmd_str[i]) == 0)
            return (*cmd_fn[i])(args);
    }
    cmd_help();
    return 1;
}

#define RL_BUFSIZE 1024

char *readln() {
    int bufsize = RL_BUFSIZE;
    int pos = 0;
    char *buf = malloc(sizeof(char) * RL_BUFSIZE);
    int c;

    if (!buf) {
        perror("allocate");
        exit(EXIT_FAILURE);
    }

    while (1) {
        c = getchar();

        if (c == EOF) {
            puts("Bye");
            exit(EXIT_SUCCESS);
        } else if (c == '\n') {
            buf[pos] = '\0';
            return buf;
        } else {
            buf[pos] = c;
        }
        pos++;

        if (pos >= RL_BUFSIZE) {
            bufsize += RL_BUFSIZE;
            buf = realloc(buf, bufsize);
            if (!buf) {
                perror("allocate");
                exit(EXIT_FAILURE);
            }
        }
    }
}

#define TOK_BUFSIZE 64
#define TOK_DELIM " \t\r\n\a"

char **cmd_tokenize(char *line) {
    int bufsize = TOK_BUFSIZE, pos = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token, **tokens_bk;

    if (!tokens)
        return fprintf(stderr, "Can't tokenize \"%s\"\n", line), NULL;

    token = strtok(line, TOK_DELIM);
    while (token != NULL) {
        tokens[pos] = token;
        pos++;

        if (pos >= bufsize) {
                bufsize += TOK_BUFSIZE;
                tokens_bk = tokens;
                tokens = realloc(tokens, bufsize * sizeof(char ));
                if (!tokens) {
                    free(tokens_bk);
                    perror("allocate");
                }
        }

        token = strtok(NULL, TOK_DELIM);
    }
    tokens[pos] = NULL;
    return tokens;
}

void loop() {
    char *line;
    char **args;
    int status;

    do {
        printf("> ");
        line = readln();
        args = cmd_tokenize(line);
        if (args)
            status = execute(args);

        free(line);
        if (args)
            free(args);
    } while(status);
}

int main(int argc, const char **argv, char **envp) {
    if (argc < 2) return fprintf(stderr, "Usage: %s root\n", *argv), 1;
    if (chroot(argv[1])) return perror("chroot"), 1;
    if (chdir("/")) return perror("chdir"), 1;
    puts("Welcome to simple jail!!");
    cmd_help();
    puts("You're in /home/misc/jail");
    puts("flag is in /home/misc");
    puts("and this environment is cleanup each 10 minutes");
    loop();
    return EXIT_SUCCESS;
}

__attribute__((constructor))
void setup() {
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    alarm(60);
}
