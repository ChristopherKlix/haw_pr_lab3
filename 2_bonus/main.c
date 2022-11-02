#include "util.h"

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
    clear_screen();
    print_delimiter("-", 32);
    print_header("Welcome to 6 out of 49");
    print_delimiter("-", 32);

    // Assign user input to array
    for (int i = 0; i < NUMBER_COUNT; i++)
    {
        // Holding the user number
        short user_number;

        // Get number from user
        // repeat if the user number
        // is not unique in the user_numbers
        do
        {
            user_number = get_short("Enter a number: ", MIN, MAX);
        } while (!number_is_unique(user_number, user_numbers, NUMBER_COUNT));
        
        // Now that we found a unique user number
        // we assign it to the user_numbers array
        user_numbers[i] = user_number;
    }

    // Print array to console
    print_delimiter("-", 32);
    printf("Your numbers are:\n");
    print_array(user_numbers, NUMBER_COUNT);
    print_delimiter("-", 32);

    // Step 5
    // seed random number generator
    seed_rand();

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
            random_number = get_rand(MIN, MAX);
        } while (!number_is_unique(random_number, winning_numbers, NUMBER_COUNT));
        
        // Now that we found a unique random number
        // we assign it to the winning_numbers array
        winning_numbers[i] = random_number;
    }

    // Print array to console
    printf("The winning numbers are:\n");
    print_array(winning_numbers, NUMBER_COUNT);
    print_delimiter("-", 32);

    // Step 6
    // Check for correct numbers
    short correct_numbers = 0;

    // First, iterate over user number one by one
    for (int i = 0; i < NUMBER_COUNT; i++)
    {
        // check each user number against every winning number
        for (int j = 0; j < NUMBER_COUNT; j++)
        {
            if (user_numbers[i] == winning_numbers[j])
            {
                // when a match was found then increment
                correct_numbers++;
            }
        }
    }

    printf("You have %d number(s) correct!\n", correct_numbers);
    print_delimiter("-", 32);

    // Step 7
    switch (correct_numbers)
    {
        case 2: printf("You have won 5€!\n");
        case 3: printf("You have won 50€!\n");
        case 4: printf("You have won 1,000€!\n");
        case 5: printf("You have won 50,000€!\n");
        case 6: printf("You have won 1,000,000€!\n");
        default: printf("You didn't win any money.\n");
    }
    
    print_delimiter("-", 32);
    
    return 0;
}
