# memory allocator

it allocates memory

## how to use

```
usage: allocatememory -b <bytes> [-o <output file>] [-s]

OPTIONS
  -h    displays help
  -b    number of bytes to allocate (e.g. 1024, 1g, 64K)
  -o    file to output to
  -s    run silently (requires -b and -o)
  -w    disable warnings ("i know what i'm doing!")
```

## how to compile

- get a c compiler (i use gcc or clang)
- `gcc -o ./allocatememory -O2 ./main.c`
- ???
- profit

## why

i was bored
