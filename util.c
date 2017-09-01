void printMemory(int * base, int size)
{
    printf("Printing memory\n");
    for(int i = 0; i < size; i++)
        printf("%d\t%p\t%d\n", i, (base + i), *(base + i));
}