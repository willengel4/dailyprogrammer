/* Will Engel
 * Its a word game similar to master mind 
 * Example gameplay:
 * Difficulty (1-5)? 3
 * SCORPION
 * FLOGGING
 * CROPPERS
 * MIGRAINE
 * FOOTNOTE
 * REFINERY
 * VAULTING
 * VICARAGE
 * PROTRACT
 * DESCENTS
 * Guess (4 left)? migraine
 * 0/8 correct
 * Guess (3 left)? protract
 * 2/8 correct
 * Guess (2 left)? croppers
 * 8/8 correct
 * You win! */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "util.c"

int main()
{
    /* The dictionary 2d array and words 2d array */
    char ** dictionary; 
    char ** words;

    /* Integers for helping with the control process */
    unsigned int numLetters, difficulty, numWords, numGuesses, dictionarySize;

    /* Load the dictionary */
    readLinesFromFile("resources/dictionary.txt", &dictionary, &dictionarySize);
    
    /* Initialize the random number generator */
    srand(time(NULL));

    /* Ask for difficulty and calculate the number of letters, words and guesses based off of that */
    printf("Difficulty (1-5)?: ");
    scanf("%d", &difficulty);
    numLetters = (difficulty + 1) * 2;
    numWords = 5 + (2 * (difficulty));
    numGuesses = 4;

    /* Choose the words at random */
    words = malloc(numWords * sizeof(char*));
    for(int i = 0; i < numWords; i++)
    {
        int index = rand() % dictionarySize;
        while(strlen(dictionary[index]) != numLetters + 1)
            index = rand() % dictionarySize;             
        words[i] = dictionary[index];
    }

    /* Display the words */
    for(int i = 0; i < numWords; i++)
        printf("%s\n", words[i]);

    /* Choose 1 word to be the answer */
    int chosenWordIndex = rand() % numWords;
    char * chosenWord = words[chosenWordIndex];

    /* Guess loop */
    int gameOver = 0;
    while(!gameOver)
    {
        /* Get the user's guess */
        char * guess;
        printf("Guess (%d left): ", numGuesses);
        scanf("%s", guess);

        /* Calculate how many are correct */
        int numCorrect = 0;
        for(int i = 0; i < numLetters; i++)
            if(guess[i] == chosenWord[i])
                numCorrect++;

        /* Display how many correct */
        printf("%d/%d correct\n", numCorrect, numLetters);
        
        /* If they guessed the word, they win */
        if(numCorrect == numLetters)
            printf("You Win!\n");

        numGuesses--;

        gameOver = numGuesses == 0 || numCorrect == numLetters;
    }
}