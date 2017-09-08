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
    printf("Printing the board...\n");
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
        }
    }

}

void searchAndDestroy(char **dictionary, int dictionaryLength, char **board, int width, int height, struct block * currBlock)
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

    for (int i = 0, r = startRow, c = startCol; i < maxWordSize; i++, c -= colDecr, r -= rowDecr)
    {
        int sidesEmpty = ((c + 1 >= width || board[r][c + 1] == '.') && (c - 1 < 0 || board[r][c - 1] == '.'));
        int tbEmpty = ((r + 1 >= height || board[r + 1][c] == '.') && (r - 1 < 0 || board[r - 1][c] == '.'));

        if ((maxWordIsVertical && sidesEmpty) || (!maxWordIsVertical && tbEmpty))
            board[r][c] = '.';
    }

    currBlock->word = maxWord;
    currBlock->x1 = maxWordIsVertical ? startCol : startCol - (maxWordSize - 1);
    currBlock->y1 = maxWordIsVertical ? startRow - (maxWordSize - 1) : startRow;
    currBlock->x2 = startCol;
    currBlock->y2 = startRow;

}

int touches(struct block * block1, struct block * block2)
{
    int x1Incr = (block1->x2 - block1->x1) / (strlen(block1->word) - 1);
    int y1Incr = (block1->y2 - block1->y1) / (strlen(block1->word) - 1);
    int x2Incr = (block2->x2 - block2->x1) / (strlen(block2->word) - 1);
    int y2Incr = (block2->y2 - block2->y1) / (strlen(block2->word) - 1);
    
    //printf("%s %d %d\n%s %d %d\n", block1->word, x1Incr, y1Incr, block2->word, x2Incr, y2Incr);

    for(int i = 0, x1 = block1->x1, y1 = block1->y1; i < strlen(block1->word); i++, x1 += x1Incr, y1 += y1Incr)
    {
        for(int f = 0, x2 = block2->x1, y2 = block2->y1; f < strlen(block2->word); f++, x2 += x2Incr, y2 += y2Incr)
            if( abs(x1 - x2) + abs(y1 - y2) <= 1 )
                return 1;
    }


    return 0;
}

int main()
{
    /* Load the scrabble board, width, and height */
    char **board;
    int width, height, numWords = 7;
    readLinesFromFile("resources/scrabble_board.txt", &board, &height);
    width = strlen(board[0]);
    printBoard(board, width, height);

    /* Load the dictionary */
    char **dictionary;
    int dictionaryLength;
    readLinesFromFile("resources/dictionary.txt", &dictionary, &dictionaryLength);

    struct block * blocks = malloc(sizeof(struct block) * numWords);

    for(int i = 0; i < numWords; i++)
    {
        searchAndDestroy(dictionary, dictionaryLength, board, width, height, &blocks[i]);
    }

    for(int i = 0; i < numWords; i++)
    {
        printf("%d. %s\n", i, blocks[i].word);
    }

    for(int i = 0; i < numWords; i++)
    {
        for(int f = 0; f < numWords; f++)
        {
            if(i != f)
            {
                printf("Does %s touch %s? ", blocks[i].word, blocks[f].word);
                
                if(touches( &blocks[i], &blocks[f] ))
                {
                    printf("yes\n");
                }
                else
                {
                    printf("no\n");
                }
            }
        }
    }
}