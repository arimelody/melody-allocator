#include <stdio.h>
#include <stdlib.h>

int main() {

    long long unsigned int size = 0;
    
    printf("please enter the number of sizes (GiB) you would like to allocate (i.e. 4 = 4GiB):\n> ");
    int digit = 0;
    while (1) {
        char input = getchar();
        if (input == '\n') break;
        if (input < '0' || input > '9') continue;

        char number = input - 48;
        if (digit > 0) number = number * 10 * digit;
        if (input == '0') number = 10 * digit;

        size = size + number;
        digit++;
    }

    size = size * 1024 * 1024 * 1024;

    printf("you are about to allocate %llu bytes of heap memory. are you sure? (y/n)\n> ", size);

    char input = getchar();
    if (input != 'y') return 0;
    while (getchar() != '\n');

    printf("please wait...\n");
    char* wtf = (char*) malloc(size * sizeof(char));

    for (int i = 0; i < size; i++) {
        wtf[i] = 255;
    }

    printf("%d bytes of heap memory has been allocated. you are insane.\n", size);
    printf("press any key to release this memory, or CTRL+C to exit the program.\n");

    getchar();

    free(wtf);

    return 0;

}
