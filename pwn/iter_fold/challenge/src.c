#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MUL 0
#define ADD 1
#define SUB 3
#define DIV 5

typedef long (*operation)(long*, long);

long add(long *acc, long x) {
    return *acc += x;
}

long sub(long *acc, long x) {
    return *acc -= x;
}

long multiplication(long *acc, long x) {
    return *acc *= x;
}

long division(long *acc, long x) {
    return *acc /= x;
}

int op_to_index(char op) {
    return op - '*';
}

void help() {
    printf("%p < トゥットゥルー♪ libcです☆\n", &printf - 0x64f00);
    puts("I've implemented something like \"fold\"!");
}

struct Fold {
    long array[5];
    operation ops[6];
};

long fold(long* array, long* init, operation op) {
    for (int i = 0; i < 5; i++)
        op(init, array[i]);

    return *init;
}

int main() {
    help();
    struct Fold f = {
        .array = {0, 0, 0, 0, 0},
        .ops[MUL] = multiplication,
        .ops[ADD] = add,
        .ops[SUB] = sub,
        .ops[DIV] = division,
    };
    char op;

    for (int i = 0; i < 5; i++) {
        printf("array[%d] = ", i);
        scanf("%ld", &f.array[i]);
    }
    printf("init = ");
    long init;
    scanf("%ld", &init);
    getchar(); // skip '\n' 
    puts("operator please(+,-,*,/)");
    scanf("%c", &op);
    getchar(); // skip '\n'
    printf("vec![");
    for (int i = 0; i < 5; i++) {
        printf("%ld", f.array[i]);
        if (i < 4)
            printf(", ");
    }
    printf("].iter().fold(%ld, |ans, &x| ans %c x);\n", init, op);
    fold(f.array, &init, f.ops[op_to_index(op)]);
    printf("ans = %ld\n", init);
}

__attribute__((constructor))
void setup() {
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    alarm(60);
}
