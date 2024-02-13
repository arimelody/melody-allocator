#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>

#define BYTES_GIVEN     1
#define FILENAME_GIVEN  2
#define SILENT          4
#define BYPASS_WARN     8

#define KiB (unsigned long long int) 0x400
#define MiB (unsigned long long int) 0x100000
#define GiB (unsigned long long int) 0x40000000
#define TiB (unsigned long long int) 0x10000000000

#define LOADS TiB * 9999

void print_help();
unsigned char* get_shorthand(unsigned long long int size);
void adjust_by_denomination(unsigned long long int* size, char denomination);
unsigned long long int* allocate_heap(unsigned long long int size, char silent);
void save_heap(unsigned long long int* heap, unsigned long long int size, char* filename, char silent);

int main(int argc, char* argv[]) {

        unsigned char flags = 0;
        unsigned long long int size = 0;
        unsigned char* filename = NULL;
        unsigned char denominator = 0;

        int arg;
        while ((arg = getopt(argc, argv, "hb:o:sw")) != -1) {
                switch (arg) {
                        case 'h':
                                print_help();
                                exit(0);
                        case 'b':
                                size = strtoull(optarg, NULL, 10);
                                if (errno == EINVAL) {
                                        fprintf(stderr, "argument -b used with invalid value '%s'!\n", optarg);
                                        exit(0);
                                }
                                if (errno == ERANGE) {
                                        fprintf(stderr, "byte argument ''%s' out of range!\n", optarg);
                                        exit(0);
                                }
                                denominator = optarg[strlen(optarg) - 1];
                                flags |= BYTES_GIVEN;
                                break;
                        case 'o':
                                filename = optarg;
                                flags |= FILENAME_GIVEN;
                                break;
                        case 's':
                                if (size == 0 || filename == NULL) {
                                        fprintf(stderr, "cannot run silently without -b and -o set!\n");
                                        print_help();
                                }
                                flags |= SILENT;
                                break;
                        case 'w':
                                flags |= BYPASS_WARN;
                                break;
                        case '?':
                                if (optopt == 'b' || optopt == 'o')
                                        fprintf(stderr, "argument -%c requires an argument!\n", optopt);
                                else if (isprint(optopt))
                                        fprintf(stderr, "unknown argument '-%c'.\n", optopt);
                                else
                                        fprintf(stderr, "unknown argument character '\\x%x'.\n", optopt);
                                exit(1);
                        default:
                                exit(1);
                }
        }

        if (!(flags & BYTES_GIVEN)) {
                fprintf(stderr, "argument -b is required!\n", optopt);
                print_help();
                exit(1);
        }

        adjust_by_denomination(&size, denominator);

        // even if the user doesn't specify a denomination, it still would be nice to display a truncated amount if available.
        unsigned char* shorthand = get_shorthand(size);
        if (!(flags & SILENT) && !(flags & BYPASS_WARN)) {
                if (shorthand) {
                        printf("you are about to allocate %s (%llu bytes) of heap memory. are you sure? (y/N)\n> ", shorthand, size);
                } else {
                        printf("you are about to allocate %llu byte%c of heap memory. are you sure? (y/N)\n> ", size, size == 1 ? 0 : 's');
                }

                unsigned char input = getchar();
                if (input != 'y') exit(0);
                while (getchar() != '\n');
        }

        unsigned long long int* heap = allocate_heap(size, flags & SILENT);

        if (!(flags & SILENT)) {
                if (shorthand) {
                        printf("%s (%llu bytes) of heap memory has been allocated. you are insane.\n", shorthand, size);
                } else {
                        printf("%llu byte%c of heap memory ha%s been allocated. you are insane.\n", size, size == 1 ? 0 : 's', size == 1 ? "s" : "ve");
                }
                free(shorthand);

                printf("press ENTER to release this memory, or CTRL+C to exit the program.\n");
                getchar();
                free(heap);
                exit(0);
        }

        if (filename != NULL) save_heap(heap, size, filename, SILENT);

        if (!(flags & SILENT)) printf("done!\n");

        free(heap);
        return 0;

}

void print_help() {
        printf("usage: allocatememory -b <bytes> [-o <output file>] [-s]\n\n");
        printf("OPTIONS\n");
        printf("  -h\tdisplays help\n");
        printf("  -b\tnumber of bytes to allocate (e.g. 1024, 1g, 64K)\n");
        printf("  -o\tfile to output to\n");
        printf("  -s\trun silently (requires -b and -o)\n");
        printf("  -w\tdisable warnings (\"i know what i'm doing!\")\n");
        exit(0);
}

unsigned char* get_shorthand(unsigned long long int size) {
        if (size < KiB) return 0;

        if (size > LOADS) {
                return ">9999 TiB";
        }

        unsigned char* buffer = malloc(sizeof(char) * 12);

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

        return 0;
}

void adjust_by_denomination(unsigned long long int* size, char denomination) {
        if (denomination >= 'a' && denomination <= 'z') denomination -= 32;
        switch (denomination) {
                case 'K':
                        *size *= KiB;
                        return;
                case 'M':
                        *size *= MiB;
                        return;
                case 'G':
                        *size *= GiB;
                        return;
                case 'T':
                        *size *= TiB;
                        return;
        }
}

unsigned long long int* allocate_heap(unsigned long long int size, char silent) {
        if (!silent) {
                printf("please wait.");
                fflush(stdout);
        }

        unsigned long long int* heap = (unsigned long long int*) malloc(size);

        for (unsigned long long int i = 0; i < size / sizeof(long long int); i++) {
                heap[i] = i;
                if (!silent && i > 0 && i % (1024 * 1024 * 1024 / sizeof(long long int)) == 0) {
                        printf(".");
                        fflush(stdout);
                }
        }
        if (!silent) printf("\n");

        return heap;
}

void save_heap(unsigned long long int* heap, unsigned long long int size, char* filename, char silent) {
        FILE* file = fopen(filename, "wb");
        if (file == NULL) {
                printf("failed to open %s!", filename);
                exit(1);
        }

        if (!silent) printf("please wait...\n");
        /*
        for (unsigned long long int i = 0; i < size / sizeof(unsigned long long int); i++) {
                for (int p = 0; p < 8; p++) {
                        fputc(*(&heap[i] + p), file);
                }
                if (i > 0 && i % (1024 * 1024 * 1024 / sizeof(long long int)) == 0) {
                        if (!silent) printf(".");
                        fflush(stdout);
                }
        }
        if (!silent) printf("\n");
        */
        fwrite(heap, sizeof(char), size, file);
        fputs("\n", file);
        fclose(file);
}
