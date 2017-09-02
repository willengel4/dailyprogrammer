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

//------------------------------------------------------------------------
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

void searchLine(char ** board, int startX, int startY, int yIncr, int xIncr, int width, int height)
{
    int l = ((yIncr * height) + (xIncr * width)) * sizeof(char);
    printf("L = %d\n", l);
    char * line = malloc( ((yIncr * height) + (xIncr * width)) * sizeof(char) );
    
    for(int i = 0, x = startX, y = startY; x < width && y < height; x += xIncr, y += yIncr, i++)
    {
        line[i] = board[y][x];
        printf("%d %d\n", x, y);        
    }
    
    printf("%s\n", line);

    free(line);
}

int main()
{
    char ** board;
    int width, height;

    readLinesFromFile("resources/scrabble_board.txt", &board, &height);
    width = strlen(board[0]);
    
    printBoard(board, width, height);

    searchLine(board, 0, 2, 0, 1, width, height);
}