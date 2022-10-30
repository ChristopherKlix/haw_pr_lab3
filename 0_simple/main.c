#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


short get_short(char[], short, short);


int main(int argc, char const *argv[])
{
    // Declare & Init constants
    const short MIN = 1;
    const short MAX = 49;


    // Array length
    const int ARR_LEN = 6;

    // Declare array of 6 int
    // Initialize array
    short numbers[ARR_LEN] = { 0 };

    // Assign user input to array
    for (int i = 0; i < ARR_LEN; i++)
    {
        numbers[i] = get_short("Enter a number: ", MIN, MAX);
    }


    // Print array to console
    for (int i = 0; i < ARR_LEN; i++)
    {
        printf("%3hd", numbers[i]);
    }
    
    printf("\n");
    
    return 0;
}


short get_short(char txt[], short min, short max)
{
    // declare working variables
    short value;
    bool finished = false;
    char ch;
    int ret_val;

    // get user input
    do
    {
        // get user input
        printf("%s", txt);
        ch = '\0';
        ret_val = scanf("%hd%c", &value, &ch);


        // check for valid user input
        if (ret_val != 2) printf("Invalid input.\n");
        else if (ch != '\n') printf("Incorrect scanf behavior\n");
        else if (value < min) printf("Number out of bounce (min)\n");
        else if (value > max) printf("Number out of bounce (max)\n");
        else finished = true;
        
        // clear input stream buffer
        while (ch != '\n') scanf("%c", &ch);

    } while (!finished);
    
    // return value to caller function
    return value;
}