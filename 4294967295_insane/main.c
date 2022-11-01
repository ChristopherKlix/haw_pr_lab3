// ---------
// includes & defines
// ---------

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <stdbool.h>

#if _WIN32
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#endif


// ---------
// custom type definitions
// ---------

// Contain setup instructions for each game
// cost_per_game:     in cents
// random_input:      auto-fills the user input
// automated_plays:   removes animations when automode
// force_win:         triggers 6 matches
// print_matrix:      adds the bet matrix to the UI
typedef struct game_config
{
    double cost_per_game;
    bool random_input;
    bool automated_plays;
    bool force_win;
    bool print_matrix;
} game_config;


// game win enum
typedef enum game_win {
    GAME_WIN_OPTION_NONE = 0,
    GAME_WIN_OPTION_FIVE = 5,
    GAME_WIN_OPTION_FIFTY = 50,
    GAME_WIN_OPTION_THOUSAND = 1000,
    GAME_WIN_OPTION_FIFTYTHOUSAND = 50000,
    GAME_WIN_OPTION_MILLION = 1000000
} game_win;


// return value of play
typedef struct game_result
{
    int matching_numbers_count;
    int money_won;
    game_win _game_win;
} game_result;


// for counting game wins in auto_game
typedef struct game_wins
{
    int none;
    int five;
    int fifty;
    int thousand;
    int fiftythousand;
    int million;
} game_wins;


// keeping track of the auto_game
typedef struct auto_game_state
{
    bool automated_game;
    bool is_running;
    int rounds_played;
    game_win highest_game_win;
    game_wins _game_wins;
    int *total_game_wins;
    double total_costs;
    double total_wins;
    double total;
} auto_game_state;

typedef struct get_short_ret_val
{
    short n;
    bool success;
    int scanf_ret_val;
    bool invalid_char;
    bool map_fail;
    char *err_msg;
} get_short_ret_val;


// ---------
// prototypes
// ---------

game_result play(game_config);
get_short_ret_val get_short(char*);
bool element_in_list(int, short *, short);
int linear_sort(short*, short*, short);
int is_unique(short, short*, short);
int get_random_num(int, int);
int get_unique_number(short *, short, short, short);
char *total_to_emoji(double);
void print_line(char, short);
int msleep(long);

// ---------
// execution
// ---------

int main(int argc, char const *argv[])
{
    // game configuration
    game_config g_game_config;
    g_game_config.cost_per_game = 1.2;
    g_game_config.random_input = false;
    g_game_config.automated_plays = false;
    g_game_config.force_win = false;
    g_game_config.print_matrix = false;

    // arg parse
    int opt;
    char args[] = { 'r', 'a', 'f', 'm' };
    while ((opt = getopt(argc, (char *const *) argv, args)) != -1)
    {
        switch (opt)
        {
            case 'r': g_game_config.random_input = true; break;
            case 'a': g_game_config.automated_plays = true; break;
            case 'f': g_game_config.force_win = true; break;
            case 'm': g_game_config.print_matrix = true; break;
            default:
                fprintf(stderr, "Usage: %s [-%s]\n", argv[0], args);
                exit(EXIT_FAILURE);
        }
    }

    // initializing auto_game
    auto_game_state g_auto_game_state = {
        .automated_game = g_game_config.automated_plays,
        .is_running = false,
        .highest_game_win = GAME_WIN_OPTION_NONE,
        .rounds_played = 0,
        .total_costs = 0.f,
        .total_wins = 0.f,
        .total = 0.f
    };

    do
    {
        // total cost counter
        g_auto_game_state.total_costs += g_game_config.cost_per_game;

        // stores the current game result of each iteration
        game_result g_game_result = play(g_game_config);

        // count rounds played
        g_auto_game_state.rounds_played++;

        // total win counter
        g_auto_game_state.total_wins += g_game_result._game_win;
        
        // increment auto_game_state wins
        switch (g_game_result._game_win)
        {
            case GAME_WIN_OPTION_FIVE:
                g_auto_game_state._game_wins.five++; break;
            case GAME_WIN_OPTION_FIFTY:
                g_auto_game_state._game_wins.fifty++; break;
            case GAME_WIN_OPTION_THOUSAND:
                g_auto_game_state._game_wins.thousand++; break;
            case GAME_WIN_OPTION_FIFTYTHOUSAND:
                g_auto_game_state._game_wins.fiftythousand++; break;
            case GAME_WIN_OPTION_MILLION:
                g_auto_game_state._game_wins.million++; break;
            default:
                g_auto_game_state._game_wins.none++;
        }

        // keep track of highest game win
        if (g_game_result._game_win > g_auto_game_state.highest_game_win)
        {
            g_auto_game_state.highest_game_win = g_game_result._game_win;
        }

        // update total
        g_auto_game_state.total = g_auto_game_state.total_wins - g_auto_game_state.total_costs;

        // UI
        printf("Rounds played: %d\n", g_auto_game_state.rounds_played);
        printf("Nothing   won: %d\n", g_auto_game_state._game_wins.none);
        printf("5€        won: %d\n", g_auto_game_state._game_wins.five);
        printf("50€       won: %d\n", g_auto_game_state._game_wins.fifty);
        printf("1000€     won: %d\n", g_auto_game_state._game_wins.thousand);
        printf("50000€    won: %d\n", g_auto_game_state._game_wins.fiftythousand);
        printf("1000000€  won: %d\n", g_auto_game_state._game_wins.million);
        print_line('-', 32);
        printf("Total Costs:  -%.2f€\n", g_auto_game_state.total_costs);
        printf("Total Win:    +%.2f€\n", g_auto_game_state.total_wins);
        printf("Highest Win:   %u€\n", g_auto_game_state.highest_game_win);
        int _sign = (g_auto_game_state.total < 0) ? '-' : '+';
        char *_emoji = total_to_emoji(g_auto_game_state.total);
        print_line('=', 32);
        printf("Total:        %c%.2f€ %s\n", _sign, fabs(g_auto_game_state.total), _emoji);
        print_line('=', 32);

        // slow down iterations to avoid flicker
        // and interpolate velocity over different systems
        msleep(50);

        // terminate auto_game when reached 1M
        if (g_auto_game_state.highest_game_win == GAME_WIN_OPTION_MILLION)
        {
            print_line('=', 32);
            printf("Won 1000000€ after %d rounds.\n", g_auto_game_state.rounds_played);
            print_line('=', 32);
            break;
        }

    } while (g_auto_game_state.automated_game);

    return 0;
}


game_result play(game_config g_game_config)
{
    printf("\033[0m");

    // Seed pseudo-random numbers
    srand(time(NULL));
    // Avoid recurring first integer
    for (size_t i = 0; i < 100; i++) rand();
    

    const short NUMBER_COUNT = 6;
    typedef const struct range {
        int min;
        int max;
    } range;

    const range NUMBER_RANGE = {
        .min = 1,
        .max = 49
    };

    typedef struct anim {
        double length;
        double velocity;
        double iterations;
    } anim;

    const anim ANIM = {
        .length = 1.f,
        .velocity = 50.f,
        .iterations = ANIM.length / ANIM.velocity * 1000.f
    };

    short *numbers = malloc(NUMBER_COUNT * sizeof(short));

    const char *iter_strs[] = {
        "first",
        "second",
        "third",
        "fourth",
        "fifth",
        "sixth"
    };

    system("clear");
    print_line('=', 32);
    printf("         \033[1m6 out of 49\033[22m            \n");
    printf("Enter your six lucky numbers!   \n");
    printf("--------------------------------\n");
    printf("╔═════════════════════════════╗\n");
    printf("║[  ] [  ] [  ] [  ] [  ] [  ]║\n");
    printf("╚═════════════════════════════╝\n");

    for (int i = 0; i < NUMBER_COUNT; i++)
    {
        char s[100] = "Enter the ";
        strcat(s, iter_strs[i]);
        strcat(s, ": ");
        double random_factor;
        int random_number;
        short n;

        do
        {
            random_number = rand() % NUMBER_RANGE.max;

            if (g_game_config.random_input)
            {
                n = random_number;
            }
            else
            {
                get_short_ret_val tmp;
                bool valid_user_input = false;

                do
                {
                    tmp = get_short(s);

                    if (tmp.invalid_char || tmp.map_fail)
                    {
                        fprintf(stderr, "\033[31m");
                        fprintf(stderr, "ERR: %s", tmp.err_msg);
                        msleep(1000);
                        fprintf(stderr, ".");
                        msleep(1000);
                        fprintf(stderr, ".");
                        msleep(1000);
                        fprintf(stderr, ".");
                        msleep(1000);
                        printf("\033[1F\033[0K");
                        fprintf(stderr, "\r\033[0K");
                        fprintf(stderr, "\033[0m");
                    }
                    else if (tmp.n < NUMBER_RANGE.min)
                    {
                        fprintf(stderr, "Number must be >= %d", NUMBER_RANGE.min);
                        msleep(1000);
                        fprintf(stderr, ".");
                        msleep(1000);
                        fprintf(stderr, ".");
                        msleep(1000);
                        fprintf(stderr, ".");
                        msleep(1000);
                        printf("\033[1F\033[0K");
                        fprintf(stderr, "\r\033[0K");
                        fprintf(stderr, "\033[0m");
                    }
                    else if (tmp.n > NUMBER_RANGE.max)
                    {
                        fprintf(stderr, "Number must be <= %d", NUMBER_RANGE.max);
                        msleep(1000);
                        fprintf(stderr, ".");
                        msleep(1000);
                        fprintf(stderr, ".");
                        msleep(1000);
                        fprintf(stderr, ".");
                        msleep(1000);
                        printf("\033[1F\033[0K");
                        fprintf(stderr, "\r\033[0K");
                        fprintf(stderr, "\033[0m");
                    }
                    else
                    {
                        n = tmp.n;
                        valid_user_input = true;
                    }

                } while (!valid_user_input);
            }

            if (g_game_config.random_input) printf("\n");

            printf("\033[1F\033[0K");

            if (!is_unique(n, numbers, NUMBER_COUNT) && !g_game_config.automated_plays && !g_game_config.random_input)
            {
                fprintf(stderr, " Please enter a unique number.");
                msleep(1000);
                fprintf(stderr, ".");
                msleep(1000);
                fprintf(stderr, ".");
                msleep(1000);
                fprintf(stderr, "\r\033[0K");
            }
        } while (!is_unique(n, numbers, NUMBER_COUNT));

        numbers[i] = n;

        fprintf(stderr, "\033[3F");
        for (size_t j = 0; j < i+1; j++)
        {
            if (j > 0) fprintf(stderr, "\033[3C");

            fprintf(stderr, "\033[2C");
        }
        
        fprintf(stderr, "%2d", n);
        fprintf(stderr, "\033[3E");
    }
    

    short *winning_numbers = malloc(NUMBER_COUNT * sizeof(short));


    for (int i = 0; i < NUMBER_COUNT; i++)
    {
        if (g_game_config.force_win) {
            winning_numbers[i] = numbers[i];
            continue;
        }

        winning_numbers[i] = get_unique_number(winning_numbers, NUMBER_RANGE.min, NUMBER_RANGE.max, NUMBER_COUNT);
    }
    printf("--------------------------------\n");
    printf("And the winning numbers are... \n");
    printf("--------------------------------\n");

    short *sorted_winning_numbers = malloc(NUMBER_COUNT * sizeof(short));
    linear_sort(winning_numbers, sorted_winning_numbers, NUMBER_COUNT);

    for (size_t i = 0; i < NUMBER_COUNT+1; i++)
    {
        for (size_t j = 0; j < ANIM.iterations; j++)
        {
            for (size_t k = 0; k < NUMBER_COUNT; k++)
            {
                if (k < i)
                {
                    fprintf(stderr, "\033[1m");
                    fprintf(stderr, "%3d", winning_numbers[k]);
                    fprintf(stderr, "\033[22m");
                    fprintf(stderr, "\033[3D");
                    fprintf(stderr, "\033[1B");
                    fprintf(stderr, " ▒▒");
                    fprintf(stderr, "\033[1A");
                }
                else if (!g_game_config.automated_plays)
                {
                    fprintf(stderr, "%3d",  get_random_num(1, NUMBER_RANGE.max));
                }
            }

            fprintf(stderr, "\033[13C");
            if (i < NUMBER_COUNT && !g_game_config.automated_plays) msleep(ANIM.velocity);

            fprintf(stderr, "\r");
        }
    }

    if (!g_game_config.automated_plays) msleep(500);
    fprintf(stderr, "\n");
    fprintf(stderr, "\n");

    int matching_numbers_count = 0;
    short matching_numbers[NUMBER_COUNT] = {0};

    for (size_t i = 0; i < NUMBER_COUNT; i++)
    {
        for (size_t j = 0; j < NUMBER_COUNT; j++)
        {
            if (winning_numbers[i] == numbers[j])
            {
                matching_numbers[i] = numbers[i];
                matching_numbers_count++;
            }
        }
    }

    printf("--------------------------------\n");
    printf("Your numbers: \n");
    printf("--------------------------------\n");

    for (size_t i = 0; i < NUMBER_COUNT; i++)
    {
        printf("\033[1m");

        if (element_in_list(numbers[i], winning_numbers, NUMBER_COUNT))
        {
            printf("\033[32m");
            printf("%3d", numbers[i]);
            printf("\033[0m");
        }
        else
        {
            printf("\033[31m");
            printf("%3d", numbers[i]);
            printf("\033[0m");
        }

        printf("\033[0m");
    }

    printf("\n");

    // printing the bet matrix
    if (g_game_config.print_matrix)
    {
        printf("--------------------------------\n");

        for (size_t i = 0; i < NUMBER_RANGE.max + 1; i++)
        {
            // skip first int 0
            if (i == 0)
            {
                // printf("    ");
                continue;
            }

            // format all user numbers
            if (element_in_list(i, numbers, NUMBER_COUNT))
            {
                printf("\033[34m"); // set blue
            }

            // format all winning numbers
            if (element_in_list(i, winning_numbers, NUMBER_COUNT))
            {
                printf("\033[5m"); // set blinking
                printf("\033[1m"); // set bold

                if (element_in_list(i, numbers, NUMBER_COUNT))
                {
                    printf("\033[32m"); // set green
                }
                else
                {
                    printf("\033[31m"); // set red
                }
            }

            // print element & remove formatting
            printf("%3zu ", i);
            printf("\033[0m");

            // line break
            if (((i) % 7) == 0) printf("\n");
        }
    }

    printf("--------------------------------\n");
    printf("You have %i number%s correct!\n", matching_numbers_count, (matching_numbers_count != 1 ? "s" : ""));
    printf("--------------------------------\n");

    printf("You have won ");
    switch (matching_numbers_count)
    {
        case 2: printf("5€."); break;
        case 3: printf("50€."); break;
        case 4: printf("1000€."); break;
        case 5: printf("50000€."); break;
        case 6: printf("1000000€."); break;
        default: printf("no money.");
    }
    printf("\n");

    printf("--------------------------------\n");

    int money_won;
    game_win _game_win;

    switch (matching_numbers_count)
    {
        case 2:
            money_won = 5;
            _game_win = GAME_WIN_OPTION_FIVE;
            break;
        case 3:
            money_won = 50;
            _game_win = GAME_WIN_OPTION_FIFTY;
            break;
        case 4:
            money_won = 1000;
            _game_win = GAME_WIN_OPTION_THOUSAND;
            break;
        case 5:
            money_won = 50000;
            _game_win = GAME_WIN_OPTION_FIFTYTHOUSAND;
            break;
        case 6:
            money_won = 1000000;
            _game_win = GAME_WIN_OPTION_THOUSAND;
            break;
        default:
            money_won = 0;
            _game_win = GAME_WIN_OPTION_NONE;
    }

    // consolidating game results for return value
    game_result _game_result = {
        .matching_numbers_count = matching_numbers_count,
        .money_won = money_won,
        ._game_win = _game_win
    };

    // free memory allocations
    free(numbers);
    free(winning_numbers);
    free(sorted_winning_numbers);

    // clear formatting before returning
    printf("\033[0m");

    return _game_result;
}


// helper functions
int is_unique(short n, short *numbers, short arr_len)
{
    for (int i = 0; i < arr_len; i++)
    {
        if (numbers[i] == n) return 0;
    }

    return 1;
}


int get_random_num(int min, int max)
{
    return (rand() % max) + min;
}


int get_unique_number(short *numbers, short min, short max, short arr_len)
{
    int random_number = get_random_num(min, max);

    if (is_unique(random_number, numbers, arr_len))
    {
        return random_number;
    }
    else
    {
        return get_unique_number(numbers, min, max, arr_len);
    }
}


get_short_ret_val get_short(char *prompt)
{
    short n;
    char terminating_ch = '\0';

    get_short_ret_val ret_val = {
        .n = 0,
        .err_msg = ""
    };

    printf("%s", prompt);
    ret_val.scanf_ret_val = scanf("%hd%c", &n, &terminating_ch);

    if (ret_val.scanf_ret_val != 2) 
    {
        ret_val.success = false;
        ret_val.map_fail = true;
        ret_val.err_msg = "Could not assign input to format-string";
    }
    else if (terminating_ch != '\n')
    {
        ret_val.success = false;
        ret_val.invalid_char = true;
        ret_val.err_msg = "Unexpected terminating character";
    }
    else
    {
        ret_val.n = n;
        ret_val.success = true;
    }

    while (terminating_ch != '\n') scanf("%c", &terminating_ch);
    
    return ret_val;
}


bool element_in_list(int n, short *elements, short arr_len)
{
    for (size_t i = 0; i < arr_len; i++)
    {
        if (n == elements[i]) return true;
    }
    
    return false;
}


int linear_sort(short *unsorted_array, short *buffer, short arr_len)
{
    short smallest_n = 100;
    short last_n = 0;

    for (size_t i = 0; i < arr_len; i++)
    {
        for (size_t j = 0; j < arr_len; j++)
        {
            if (unsorted_array[j] <= smallest_n && unsorted_array[j] > last_n)
            {
                smallest_n = unsorted_array[j];
            }
        }

        buffer[i] = smallest_n;
        last_n = smallest_n;
        smallest_n = 100;
    }

    return 0;
}


// utility functions
void print_line(char delimiter, short count)
{
    for (size_t i = 0; i < count; i++)
    {
        printf("%c", delimiter);
    }

    printf("\n");
}


char *total_to_emoji(double total)
{
    if (total > 1000000) return "\U0001F929";
    else if (total > 5000) return "\U0001F92A";
    else if (total > 1000) return "\U0001F911";
    else if (total > 500) return "\U0001F61D";
    else if (total > 100) return "\U0001F924";
    else if (total < -200) return "\U0001F92E";
    else if (total < -100) return "\U0001F635";
    else if (total < 0) return "\U0001FAE3";
    else return "\U0001F974";
}

#if _WIN32
int msleep(long msec)
{
    Sleep(msec);
    return 0;
}
#else
int msleep(long msec)
{
    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}
#endif
