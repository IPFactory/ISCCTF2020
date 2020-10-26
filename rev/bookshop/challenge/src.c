#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/ptrace.h>

#define NAMELEN 20
#define ITEMCOUNT 6

struct Item {
    char name[NAMELEN];
    uint32_t price;
};

struct Shop {
    struct Item items[ITEMCOUNT];
    uint32_t stock[ITEMCOUNT];
};

void printShop(struct Shop *shop) {
    puts("shop status is ...\n");
    printf("%-20s|%-7s|%-7s\n", "item", "price", "stock");
    puts("-----------------------------------");
    for (int i = 0; i < ITEMCOUNT; i++) {
        printf("%-20s %-7d %-7d\n", shop->items[i].name, shop->items[i].price, shop->stock[i]);
    }
    puts("-----------------------------------\n");
}

struct User {
    struct Item belongings[10];
    int count;
    uint16_t possession;
};

void printUser(struct User *user) {
    puts("your status is ...\n"
         "--------------------");
    if (user->count > 0) {
        puts("having...");
        for (int i = 0; i < user->count; i++) {
            printf("%s\n", user->belongings[i].name);
        }
        puts("");
    }
    printf("current money is ... %d\n", user->possession);
    puts("-------------------\n");
}

void printUserBelonging(struct User *user) {
    puts("you having...");
    for (int i = 0; i < user->count; i++) {
        printf("%s\n", user->belongings[i].name);
    }
    puts("");
}

void nameEnc(char *name, char *buf) {
    for (int i = 0; i < strlen(name); i++)
        buf[i] = name[i] ^ 10;
}

void itemCheck(struct User *user, char *name) {
    for (int i = 0; i < user->count; i++) {
        if (strcmp(name, user->belongings[i].name) == 0) {
            char d[NAMELEN] = {0};
            nameEnc(name, d);
            if (memcmp(d, (char[]){108, 102, 107, 109}, 4) == 0) {
                char e[47] = {183,173,189,189,170,184,133,135,206,139,161,157,202,144,161,142,202,138,157,150,161,156,207,144,202,140,207,205,141,161,137,207,138,150,161,134,134,154,161,202,144,154,161,136,151,147,131};
                for(int i = 0; i < 47; i++) {
                    char c = e[i] ^ 0xfe;
                    write(1, &c, 1);
                }
            } else {
                printf("Sorry can't read %s\n", user->belongings[i].name);
            }
        }
    }
}

void readbook(struct User *user) {
    if (user->count > 0) {
        printUserBelonging(user);
        printf("Which one do you read?\n>");
        char name[NAMELEN];
        scanf("%19s", name);
        getchar(); // skip \n
        struct Item *item = NULL;
        itemCheck(user, name);
    } else {
        puts("you don't have any books");
    }
}

void add(struct User *user, struct Item *item) {
    memcpy(&(user->belongings[user->count]), item, sizeof(struct Item));
    user->count++;
    user->possession -= item->price;
}

enum BUYERROR {
    NOT_ENOUGH_PAYMENT = -1,
    OUT_OF_STOCK = -2,
    ITEM_NOT_FOUND = -3
};

int buy(struct User *user, const char *name, struct Shop *shop) {
    struct Item *item = NULL;
    int i;
    for (i = 0; i < ITEMCOUNT; i++) {
        if (strcmp(shop->items[i].name, name) == 0) {
                item = &(shop->items[i]);
                break;
        }
    }

    if (item == NULL) return ITEM_NOT_FOUND;

    uint32_t price = item->price;
    if (price > user->possession)
        return NOT_ENOUGH_PAYMENT; 

    if (shop->stock[i] == 0)
        return OUT_OF_STOCK;

    shop->stock[i]--;
    puts("");
    printf("buying %s ...\n", item->name);
    add(user, item);

    return 0;
}

int main() {
    struct User user = {
        .belongings = {0},
        .count  = 0,
        .possession = 2000,
    };

    struct Shop shop = {
        .items = {
            {
                .name = "Kirara",
                .price = 380,
            },
            {
                .name = "MAX",
                .price = 380,
            },
            {
                .name = "Carat",
                .price = 490,
            },
            {
                .name = "Forward",
                .price = 590,
            },
            {
                .name = "Miracle",
                .price = 366,
            },
            {
                .name = "flag",
                .price = 0xffff,
            }

        },
        .stock = {
            3,
            3,
            1,
            2,
            3,
            1,
        }
    };

    printf("Welcome to BookStore!!\n");
    while(true) {
        puts("What will you do?\nbuy(b)/status(s)/read(r)");
        switch (getchar()) {
            case 'b':
                printf("now stock is...\n");
                printShop(&shop);
                printf("what do you want?\n> ");
                char name[NAMELEN] = {0};
                scanf("%19s", name);
                switch (buy(&user, name, &shop)) {
                    case NOT_ENOUGH_PAYMENT:
                        printf("Oops!!\nYou don't have enough money to buy it\nYour current money is %d\n", user.possession);
                        break;
                    case OUT_OF_STOCK:
                        puts("Oops!!\nSorry!! it's sold out!!");
                        break;
                    case ITEM_NOT_FOUND:
                        puts("そんなもの\nウチにはないよ...");
                        break;
                    case 0:
                        puts("Thank you for your purchase!!");
                        break;
                    default:
                        puts("Oops");
                        break;
                }
                puts("");
                break;
            case 's':
                printShop(&shop);
                printUser(&user);
                break;
            case 'r':
                readbook(&user);
                break;
            case EOF:
                puts("Bye");
                exit(0);
                break;
            default:
                puts("'b' or 's' or 'r'");
                break;
        }
        getchar(); // skip \n
    }

}

__attribute__((constructor))
void setup() {
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
}
