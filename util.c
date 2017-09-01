/* Goes through the memory byte by byte, printing each byte in hex format */
void printMemory(char * base, int size)
{
    printf("Printing memory\n");
    for(int i = 0; i < size; i++)
        printf("%d\t%p\t0x%02x\n", i, (base + i), *(base + i));
}

/* Writes str to loc */
int writeToFile(char * str, char * loc)
{
    /* Create the file pointer */
    FILE * file = fopen(loc, "w");
    
    /* Copy the string into the file */
    int result = fputs(str, file);

    /* Close the file */
    fclose(file);

    /* Returns the success or failure indication number */
    return result;
}

/* Returns the characters in the file with a null byte at the end */
char * readFromFile(char * loc)
{
    /* Creates content buffer, file pointer, and some integers to help with the process */
    char * content = malloc(sizeof(char));
    FILE * file = fopen(loc, "r");
    int count, c, bufferSize = 1;
    
    /* Go through the file character by character, counting as it goes, stopping at EOF */
    for(count = 0; (c = fgetc(file)) != EOF; count++)
    {
        /* Uses a "doubling" approach to dynamically make room for an unkown amount of file characters */
        if(count >= bufferSize)
        {
            bufferSize *= 2;
            content = realloc(content, bufferSize * sizeof(char));
        }

        /* Copy the character into the buffer */
        content[count] = c;
    }

    /* Closes the file */
    fclose(file);

    /* Ensures that the allocated memory only contains enough space for the content
     * And one extra byte for the null byte */
    content = realloc(content, (count + 1) * sizeof(char));
    content[count] = '\0';

    /* Returns the content */
    return content;
}