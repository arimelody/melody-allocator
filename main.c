#include <stdio.h>
#include <stdlib.h>

int main() {

        unsigned long long int size = 0;
        unsigned int gib = 0;

        printf("please enter the number of gibibytes (GiB) you would like to allocate (1 GiB = 1073741824 bytes):\n> ");
        unsigned int digit = 0;
        while (1) {
                unsigned char input = getchar();
                if (input == '\n') break;
                if (input < '0' || input > '9') continue;

                if (digit > 0) gib = gib * 10;

                gib = gib + (unsigned int) input - 48;
                digit++;
        }

        size = (unsigned long long int) gib * 1024 * 1024 * 1024;

        printf("you are about to allocate %d GiB (%llu bytes) of heap memory. are you sure? (y/n)\n> ", gib, size);

        unsigned char input = getchar();
        if (input != 'y') return 0;
        while (getchar() != '\n');

        printf("please wait");
        fflush(stdout);

        unsigned long long int* wtf = (unsigned long long int*) malloc(size);

        for (unsigned long long int i = 0; i < size / sizeof(long long int); i++) {
                wtf[i] = 0x80085;
                if (i > 0 && i % (1024 * 1024 * 1024 / sizeof(long long int)) == 0) {
                        printf(".");
                        fflush(stdout);
                }
        }
        printf("\n");

        printf("%d GiB (%llu bytes) of heap memory has been allocated. you are insane.\n", gib, size);
        printf("press ENTER to release this memory, or CTRL+C to exit the program.\n");

        getchar();

        free(wtf);

        return 0;

}
