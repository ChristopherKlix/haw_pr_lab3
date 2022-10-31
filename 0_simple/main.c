#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>


void print_array(short[], short);
bool number_is_unique(short, short[], short);
short get_short(char[], short, short);


int main(int argc, char const *argv[])
{
    // Declare & Init constants
    const short MIN = 1;
    const short MAX = 49;


    // Array length
    const int NUMBER_COUNT = 6;

    // Declare array of 6 int
    // Initialize array
    short user_numbers[NUMBER_COUNT] = { 0 };
    short winning_numbers[NUMBER_COUNT] = { 0 };

    // User Interface
    printf("--------------------------------\n");
    printf("    Welcome to 6 out of 49\n");
    printf("--------------------------------\n");

    // Assign user input to array
    for (int i = 0; i < NUMBER_COUNT; i++)
    {
        user_numbers[i] = get_short("Enter a number: ", MIN, MAX);
    }

    // Print array to console
    printf("--------------------------------\n");
    printf("Your numbers are:\n");
    print_array(user_numbers, NUMBER_COUNT);
    printf("--------------------------------\n");

    // pick numbers
    srand(time(NULL));

    // Assign random numbers to array
    for (int i = 0; i < NUMBER_COUNT; i++)
    {
        // Holding our generated random number
        short random_number;

        // Generate a random number and
        // repeat if the random number
        // is not unique in the winning_numbers
        do
        {
            // generating a pseudo-random number with rand()
            // by using modulo we map
            //      0 - MAX_RANGE       MAX_RANGE: provided by stdlib.h
            //      to
            //      0 - (MAX-1)         0 - 48
            // 
            // we then map
            //      0 - (MAX-1)         0 - 48
            //      to
            //      1 - MAX             1 - 49
            // by adding 1
            random_number = (rand() % MAX) + 1;
        } while (!number_is_unique(random_number, winning_numbers, NUMBER_COUNT));
        
        // Now that we found a unique random number
        // we assign it to the winning_numbers array
        winning_numbers[i] = random_number;
    }

    // Print array to console
    printf("The winning numbers are:\n");
    print_array(winning_numbers, NUMBER_COUNT);
    printf("--------------------------------\n");


    // Check for correct numbers
    short correct_numbers;

    for (int i = 0; i < NUMBER_COUNT; i++)
    {
        for (int j = 0; j < NUMBER_COUNT; j++)
        {
            if (user_numbers[i] == winning_numbers[j])
            {
                correct_numbers++;
            }
        }
    }

    printf("You have %d number(s) correct!\n", correct_numbers);
    printf("--------------------------------\n");

    
    return 0;
}


// helper function to print array inline
void print_array(short array[], short arr_len)
{
    for (int i = 0; i < arr_len; i++)
    {
        printf("%3hd", array[i]);
    }

    printf("\n");

    return;
}


// check for uniqueness in array
bool number_is_unique(short number, short array[], short arr_len)
{
    // Iterate over array and see if there are matching numbers
    // If so, then return with false
    for (int i = 0; i < arr_len; i++)
    {
        if (number == array[i])
        {
            return false;
        }
    }

    // Since the for-loop completed and found no
    // matching numbers, we return true
    return true;
}


// util function to ask for user input
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