#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <limits.h>

#define KiB (unsigned long long int) 0x400
#define MiB (unsigned long long int) 0x100000
#define GiB (unsigned long long int) 0x40000000
#define TiB (unsigned long long int) 0x10000000000

#define LOADS TiB * 9999

char* get_shorthand(unsigned long long int size) {
        if (size < KiB) return 0;

        if (size > LOADS) {
                return ">9999 TiB";
        }

        char* buffer = malloc(sizeof(char) * 12);

        if (size >= TiB) {
                snprintf(buffer, 12, "%.2f TiB", (float) size / TiB);
                return buffer;
        }
        if (size >= GiB) {
                snprintf(buffer, 12, "%.2f GiB", (float) size / GiB);
                return buffer;
        }
        if (size >= MiB) {
                snprintf(buffer, 12, "%.2f MiB", (float) size / MiB);
                return buffer;
        }
        if (size >= KiB) {
                snprintf(buffer, 12, "%.2f KiB", (float) size / KiB);
                return buffer;
        }
}

unsigned long long int adjust_by_denomination(unsigned long long int size, char denomination) {
        switch (denomination) {
                case 'K':
                        return size * KiB;
                case 'M':
                        return size * MiB;
                case 'G':
                        return size * GiB;
                case 'T':
                        return size * TiB;
                default:
                        return size;
        }
}

int main() {

        unsigned long long int size = 0;

        printf("please enter the amount of memory you would like to allocate (\"1G\" = 1 GiB = 1073741824 bytes):\n> ");
        unsigned int digit = 0;
        unsigned char last_char = 0;
        while (1) {
                unsigned char input = getchar();
                if (input == '\n') break;
                last_char = input;

                if (input < '0' || input > '9') continue;

                if (digit > 0) size *= 10;

                size += (unsigned int) input - 48;
                digit++;
        }

        size = adjust_by_denomination(size, last_char);

        // even if the user doesn't specify a denomination, it still would be nice to display a truncated amount if available.
        char* shorthand = get_shorthand(size);

        if (shorthand) {
                printf("you are about to allocate %s (%llu bytes) of heap memory. are you sure? (y/n)\n> ", shorthand, size);
        } else {
                printf("you are about to allocate %llu byte%c of heap memory. are you sure? (y/n)\n> ", size, size == 1 ? 0 : 's');
        }

        unsigned char input = getchar();
        if (input != 'y') return 0;
        while (getchar() != '\n');

        printf("please wait.");
        fflush(stdout);

        unsigned long long int* wtf = (unsigned long long int*) malloc(size);

        for (unsigned long long int i = 0; i < size / sizeof(long long int); i++) {
                wtf[i] = ULLONG_MAX;
                if (i > 0 && i % (1024 * 1024 * 1024 / sizeof(long long int)) == 0) {
                        printf(".");
                        fflush(stdout);
                }
        }
        printf("\n");

        if (shorthand) {
                printf("%s (%llu bytes) of heap memory has been allocated. you are insane.\n", shorthand, size);
        } else {
                printf("%llu byte%c of heap memory ha%s been allocated. you are insane.\n", size, size == 1 ? 0 : 's', size == 1 ? "s" : "ve");
        }
        free(shorthand);
        printf("press ENTER to release this memory, or CTRL+C to exit the program.\n");

        getchar();

        free(wtf);

        return 0;

}
