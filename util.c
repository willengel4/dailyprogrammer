/* Will Engel
 * I wrote util.c so that common functions could be used by all of the programs in this project */

/* Goes through the memory byte by byte, printing each byte in hex format */
void printMemory(char * base, int size)
{
    printf("Printing memory\n");
    for(int i = 0; i < size; i++)
        printf("%d\t%p\t0x%02x\n", i, (base + i), *(base + i));
}

/* Creates a string of specified length with the placeholder char replicated throughout
 * the string. It will insert a null byte at the end */
void strinit(char ** str, int length, char placeHolder)
{
    /* Allocate space for the string */
    char * mem = malloc(sizeof(char) * (length + 1));

    /* Copy the placeholder char into each position in the string */
    for(int i = 0; i < length; i++)
        mem[i] = placeHolder;
    
    /* Insert the null byte at the end */
    mem[length] = '\0';

    /* Point str at the memory space */
    *str = mem;
}

/* Clears the contents of the string */
void strclear(char ** str, int length, char clearTo)
{
    printMemory(*str, length);        
    
    for(int i = 0; i < length; i++)
    {
        *str[i] = clearTo;        
    }

    printMemory(*str, length);        
    
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

/* Returns the index of word in words
 * if word doesn't exist returns -1
 * Uses binary search so words must be sorted */
int findWord(char ** words, char * word, int length)
{
    int min = 0, max = length - 1, mid, dne = 0;

    while(!dne)
    {
        mid = min + ((max - min) / 2);
        
        int x = strcmp(words[mid], word);

        if(x < 0)
            min = mid + 1;
        else if(x == 0)
            return mid;
        else
            max = mid - 1;

        if (max < min)
            dne = 1;
    }

    return -1;
}

/* At the end, callerLines and callerSize will have the lines and size of the file
 * 2 dimensional */
void readLinesFromFile(char * loc, char *** callerLines, int * callerSize)
{
    /* Initializes the 2d array that will eventually hold every line */
    char ** lines = malloc(sizeof(char*));

    /* This char * will be used to build each individual line */
    char * currLine = malloc(sizeof(char));

    /* The file pointer */
    FILE * file = fopen(loc, "r");

    /* Integers that will help control the process */
    int c, numLines = 0, linesBufferSize = 1, lineCharCount = 0, lineCharBufferSize = 1, reachedEnd = 0;

    /* Until the end is reached.. */
    while(!reachedEnd)
    {
        /* Grab the character */
        c = fgetc(file);

        /* If the character isn't a new line or end of file
         * It needs to be added to the currline memory space
         * If the currline memory space is too small, makes room */
        if(c != '\n' && c != EOF)
        {
            /* Creates memory if needed
             * "Doubling" dynamic memory approach. Will be resized later */
            if(lineCharCount >= lineCharBufferSize)
            {
                lineCharBufferSize *= 2;
                currLine = realloc(currLine, lineCharBufferSize * sizeof(char));
            }

            /* Put the character in the next memory space */
            currLine[lineCharCount++] = c;
        }

        /* If the character is a new line or end of file character
         * The lines 2d array needs to point at it.
         * If the lines 2d array needs more space, it makes room.
         * Also, the currline's memory space is refined to only what it needs.
         * After the address of the first memory space in currline is referenced by lines, 
         * currLine begins to point at a new memory space.
         * The integer process parameters are reset */
        else
        {
            /* Makes space in the 2d array for the next line
             * "Doubling" approach */
            if(numLines >= linesBufferSize)
            {
                linesBufferSize *= 2;
                lines = realloc(lines, linesBufferSize * sizeof(char*));
            }

            /* Refines the current line's memory space. */
            currLine = realloc(currLine, (lineCharCount + 1) * sizeof(char));
            currLine[lineCharCount] = '\0';

            /* Adds currLine to the end of the 2d array */
            if(lineCharCount > 0)
                lines[numLines++] = currLine;
                
            /* Reset currline and the integer process parameters */
            currLine = malloc(sizeof(char));
            lineCharBufferSize = 1;
            lineCharCount = 0;
        }

        /* Check if the end has been reached */
        reachedEnd = c == EOF;
    }

    /* Refines the 2d arrays space to only what it needs */
    lines = realloc(lines, numLines * sizeof(char*));

    /* Make the caller point at the lines and numlines */
    *callerLines = lines;
    *callerSize = numLines;

    /* Closes the file */
    fclose(file);
}