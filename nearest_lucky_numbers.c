#include <stdio.h>
#include <stdlib.h>

void printMemory(int * base, int size)
{
    printf("Printing memory\n");
    for(int i = 0; i < size; i++)
        printf("%d\t%p\t%d\n", i, (base + i), *(base + i));
}

void mark(int * base, int size, int step)
{
    for(int i = step - 1; i < size; i += step)
        *(base + i) = 0;
}

void shiftDown(int * base, int size, int from)
{
    for(int i = from; i < size - 1; i++)
        *(base + i) = *(base + i + 1);        
}

int removeMarked(int * base, int size)
{
    for(int i = size - 1; i > 0; i--)
    {
        if(*(base + i) == 0)
        {
            shiftDown(base, size, i);
            size--;
        }
    }

    return size;
}

int main()
{
    int size = 25, counter, step = 2, * base, round = 1;

    base = malloc(sizeof(int) * size);

    /* Initialize the sieve to all 1's */
    for(counter = 0; counter < size; counter++)
        *(base + counter) = counter + 1;
    
    printMemory(base, size);

    for(int round = 1; round <= 3; round++)
    {
        mark(base, size, step);
        size = removeMarked(base, size);
        printMemory(base, size);
        step = *(base + round);
    }

    return 0;
}