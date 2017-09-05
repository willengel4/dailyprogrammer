/* Will Engel
 * Intermediate challenge #326 - Scrabble In Reverse
 * Now, can you reverse a Scrabble game? That is, given a 
 * board can you infer what words were played and in what order? */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.c"

struct block
{
    char * word;
    int x1, y1, x2, y2;
};

//-----------------For Debugging------------------------------------------
void printBlock(struct block * b)
{
    printf("Block: %s\nLoc: (%d, %d)---(%d, %d)\n", b->word, b->x1, b->y1, b->x2, b->y2);    
}

void printBoard(char ** board, int width, int height)
{
    for(int i = 0; i < height; i++)
    {
        for(int f = 0; f < width; f++)
        {
            printf("%c", board[i][f]);            
        }
        printf("\n");
    }
}
//----------------------------------------------------------------------------

/* Finds the largest word, searching horizontally in the specified row */
void findMaxWordHorizontal(char ** dictionary, int dictionaryLength, char ** board, int row, int width, char ** maxWord, int * startCol)
{
    printf("finding max word horizontal at row %d\n", row);

    /* temp will be used to build each block
     * word will be used to store the biggest word */
    char * temp;
    char * word;
    strinit(&temp, width, '\0');
    strinit(&word, width, '\0');
 
    /* Move col accross the width of the board */
    for(int tempIndex = 0, col = 0; col < width; col++)
    {
        printf("ti=%d col=%d c=%c ", tempIndex, col, board[row][col]);
        
        /* When we reach a '.', that indicates the end of a block 
         * and the block actually contains characters */
        if(board[row][col] == '.' || col >= width - 1)
        {
            if(col >= width - 1)
                temp[tempIndex++] = board[row][col];
            
            /* If temp is larger than word and temp is an actual english word
             * Replace word with temp */
            if(strlen(temp) > strlen(word) && findWord(dictionary, temp, dictionaryLength))
            {
                strcpy(word, temp);
                *startCol = col - tempIndex;
                printf("word=%s sc=%d max=%lu ", word, *startCol, strlen(word));
            }

            /* Reset temp */
            strclear(temp, width, '\0');
            tempIndex = 0;
        }

        /* If the current character isn't an empty piece, add it to temp */
        else
        {
            temp[tempIndex++] = board[row][col];
            printf("temp=%s ", temp);            
        }

        printf("\n");
    }

    /*  */
    free(temp);
    *maxWord = word;
    printf("The max word that was found was %s\n", word);
}

int main()
{
    /* Load the scrabble board, width, and height */
    char ** board;
    int width, height;
    readLinesFromFile("resources/scrabble_board.txt", &board, &height);
    width = strlen(board[0]);
    printBoard(board, width, height);
    
    /* Load the dictionary */
    char ** dictionary;
    int dictionaryLength;
    readLinesFromFile("resources/dictionary.txt", &dictionary, &dictionaryLength);

    int testRow = 2;
    char * maxWord;
    int startCol;
    findMaxWordHorizontal(dictionary, dictionaryLength, board, testRow, width, &maxWord, &startCol);
}