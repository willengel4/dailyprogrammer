/* Will Engel
 * Intermediate challenge #326 - Scrabble In Reverse
 * Now, can you reverse a Scrabble game? That is, given a 
 * board can you infer what words were played and in what order? */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.c"

#define LOG 1

struct block
{
    char *word;
    int x1, y1, x2, y2;
};

//-----------------For Debugging------------------------------------------
void printBlock(struct block *b)
{
    printf("Block: %s\nLoc: (%d, %d)---(%d, %d)\n", b->word, b->x1, b->y1, b->x2, b->y2);
}

void printBoard(char **board, int width, int height)
{
    for (int i = 0; i < height; i++)
    {
        for (int f = 0; f < width; f++)
        {
            printf("%c", board[i][f]);
        }
        printf("\n");
    }
}
//----------------------------------------------------------------------------

/* Finds the largest word, searching horizontally in the specified row */
void findMaxWord(char **dictionary, int dictionaryLength, char **board,
                 int initRow, int initCol, int rowIncr, int colIncr, int width, int height, char **maxWord, int *startCol, int *startRow)
{
    if (LOG)
        printf("finding max word with ir=%d ic=%d ri=%d ci=%d\n", initRow, initCol, rowIncr, colIncr);

    /* temp will be used to build each block
     * word will be used to store the biggest word */
    int blockSize = (width * colIncr) + (height * rowIncr);
    char *temp;
    char *word = *maxWord;
    strinit(&temp, blockSize, '\0');

    /* Move col accross the width of the board */
    for (int tempIndex = 0, row = initRow, col = initCol;
         col < width && row < height;
         col += colIncr, row += rowIncr)
    {
        if (LOG)
            printf("ti=%d col=%d row=%d c=%c ", tempIndex, col, row, board[row][col]);

        /* When we reach a '.', that indicates the end of a block 
         * and the block actually contains characters */
        if (board[row][col] == '.' || (colIncr && col >= width - 1) || (rowIncr && row >= height - 1))
        {
            if (board[row][col] != '.' && ((colIncr && col >= width - 1) || (rowIncr && row >= height - 1)))
                temp[tempIndex++] = board[row][col];

            /* If temp is larger than word and temp is an actual english word
             * Replace word with temp */
            if (strlen(temp) > strlen(word))
            {
                int rdi = -1, nrdi = findWord(dictionary, temp, dictionaryLength);

                if (nrdi == -1)
                {
                    streverse(temp);
                    rdi = findWord(dictionary, temp, dictionaryLength);
                }

                if (LOG)
                    printf("%s nrdi=%d rdi=%d ", temp, nrdi, rdi);

                if (nrdi != -1 || rdi != -1)
                {
                    strcpy(word, temp);

                    if (board[row][col] != '.' && ((colIncr && col >= width - 1) || (rowIncr && row >= height - 1)))
                    {
                        *startCol = col;
                        *startRow = row; 
                    }
                    else
                    {
                        if(colIncr == 1)
                        {
                            *startCol = col - 1;
                            *startRow = row;                            
                        }
                        else
                        {
                            *startCol = col;
                            *startRow = row - 1;  
                        }
                    }

                    if (LOG)
                        printf("word=%s sc=%d sr=%d max=%lu ", word, *startCol, *startRow, strlen(word));
                }
            }

            /* Reset temp */
            strclear(temp, blockSize, '\0');
            tempIndex = 0;
        }

        /* If the current character isn't an empty piece, add it to temp */
        else
        {
            temp[tempIndex++] = board[row][col];

            if (LOG)
                printf("temp=%s ", temp);
        }

        if (LOG)
            printf("\n");
    }

    free(temp);
}

void searchAndDestroy(char **dictionary, int dictionaryLength, char **board, int width, int height)
{
    /* maxWord will store the largest word on the board */
    char *maxWord, *maxVertical;
    strinit(&maxWord, height, '\0');
    int startCol, startRow, maxWordIsVertical, maxWordSize, colDecr, rowDecr;

    /* Finds the max word searching horizontally */
    for (int i = 0; i < width; i++)
        findMaxWord(dictionary, dictionaryLength, board, 0, i, 1, 0, width, height, &maxWord, &startCol, &startRow);

    /* Stores the horizontally searched max word in maxVertical */
    strcpy(maxVertical, maxWord);

    /* Finds the max word searching vertically */
    for (int i = 0; i < height; i++)
        findMaxWord(dictionary, dictionaryLength, board, i, 0, 0, 1, width, height, &maxWord, &startCol, &startRow);

    /* Determine if the max word was found in the horizontal or vertical direction */
    maxWordIsVertical = !strcmp(maxVertical, maxWord);
    maxWordSize = strlen(maxWord);
    colDecr = !maxWordIsVertical;
    rowDecr = maxWordIsVertical;

    if (LOG)
    {
        printf("Max word: %s length=%d\nr=%d c=%d\nMaxVertical: %s\nmaxWordIsVertical: %d\n", maxWord, maxWordSize, startRow, startCol, maxVertical, maxWordIsVertical);
        printMemory(maxWord, strlen(maxWord) + 1);
        printMemory(maxVertical, strlen(maxVertical) + 1);
    }

    for (int i = 0, r = startRow, c = startCol; i < maxWordSize; i++, c -= colDecr, r -= rowDecr)
    {
        int sidesEmpty = ((c + 1 >= width || board[r][c + 1] == '.') && (c - 1 < 0 || board[r][c - 1] == '.'));
        int tbEmpty = ((r + 1 >= height || board[r + 1][c] == '.') && (r - 1 < 0 || board[r - 1][c] == '.'));
        
        if(LOG)
            printf("i=%d r=%d c=%d s.e=%d tb.e=%d", i, r, c, sidesEmpty, tbEmpty);

        if ((maxWordIsVertical && sidesEmpty) || (!maxWordIsVertical && tbEmpty))
        {
            if(LOG)
                printf(" Removing %c at (%d, %d)", board[r][c], r, c);

            board[r][c] = '.';
        }

        if(LOG)
            printf("\n");
    }

    printBoard(board, width, height);
}


int main()
{
    /* Load the scrabble board, width, and height */
    char **board;
    int width, height;
    readLinesFromFile("resources/scrabble_board.txt", &board, &height);
    width = strlen(board[0]);
    printBoard(board, width, height);

    /* Load the dictionary */
    char **dictionary;
    int dictionaryLength;
    readLinesFromFile("resources/dictionary.txt", &dictionary, &dictionaryLength);

    searchAndDestroy(dictionary, dictionaryLength, board, width, height);
    searchAndDestroy(dictionary, dictionaryLength, board, width, height);
    searchAndDestroy(dictionary, dictionaryLength, board, width, height);
    searchAndDestroy(dictionary, dictionaryLength, board, width, height);
    searchAndDestroy(dictionary, dictionaryLength, board, width, height);
    searchAndDestroy(dictionary, dictionaryLength, board, width, height);
    searchAndDestroy(dictionary, dictionaryLength, board, width, height);
}