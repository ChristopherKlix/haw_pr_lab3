#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


short get_short(char[], short, short);


int main(int argc, char const *argv[])
{
    /* code */
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
        printf("%s: ", txt);
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