#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


short getShort (char*, short, short);
short rand_number (short, short); 
void fill(char, int); 
int check_bet(short*, short*, int);
void print_wins(int);
int is_unique(short*, int, int);
int is_input_unique(short*, int, int);
int is_lottery_unique(short*, int, int);


int main(void)
{
	short min = 1;
	short max = 49;
	const int n_num = 6;

	short *numbers = malloc(n_num * sizeof(short));
	short *lottery_num = malloc(n_num * sizeof(short));

	srand(time(NULL));

	fill('*', 35);
	printf("\n");

	//Input
	printf("Enter %d numbers\n", n_num);

	for (int i = 0; i < n_num; i++)
	{  
		short tmp_num;
		do
		{
			tmp_num = getShort("Number", min, max);
		}   
		while (!is_input_unique(numbers, n_num, tmp_num));

		numbers[i] = tmp_num;
	}

	system("cls");
	fill('*', 35);
	printf("\n");

	printf("Your numbers: ");
	for (int i = 0; i < n_num; i++)
	{
		printf("%3hd", numbers[i]);
	}

	printf("\n");
	fill('*', 35);
	printf("\n");
	
	//Random lottery numbers
	for (int i = 0; i < n_num; i++)
	{
		short tmp_num;

		do
		{ 
			tmp_num = rand_number(min, max); 

		} while (!is_unique(lottery_num, n_num, tmp_num));

		lottery_num[i] = tmp_num; 
	}
	
	printf("The lotterynumbers are: ");

	//Print lottery numbers
	for (int i = 0; i < n_num; i++) 
	{
		printf("%hd ", lottery_num[i]); 
	}

	printf("\n");
	fill('*', 35);
	printf("\n"); 

	//check correct
	printf("Correct numbers are: "); 
	int check_val = check_bet(numbers, lottery_num, n_num);
	if (check_val == 0)
	{
		printf("NONE");
	}

	printf("\n"); 
	fill('*', 35);
	printf("\n");

	//print wins 
	printf("Your wins are: ");
	print_wins(check_val); 

	printf("\n");
	fill('*', 35);
	printf("\n");

	free(numbers);
	free(lottery_num);

	return 0;
}


short getShort(char text[], short min, short max) 
{
	short value; 
	int finished = 0; 
	char ch; 
	int retVal; 

	do
    {
		printf("%s: ", text);
		ch = '\0';
		retVal = scanf("%hd%c", &value, &ch);

		if (retVal != 2) printf("Not a correct number\n");
		else if (ch != '\n') printf("Unexpected character\n");
		else if (value < min) printf("Number has to be 1 or greater\n");
		else if (value > max) printf("Number has to be 49 or smaller\n");
		else finished = 1;

		while (ch != '\n') scanf("%c", &ch);

	} while (!finished);

	return value;
}


short rand_number (short min, short max)
{
	int ln = ( rand() % max) + min;  
	  
	return ln; 
}


void fill(char sign, int n_stars)
{
	for (int i = 0; i < n_stars; i++)
	{
		printf("%c", sign);
	}
	return; 
}


int check_bet(short *numbers, short *lottery_num, int n_num)
{
	int check_val = 0; 

	for (int i = 0; i < n_num; i++)
	{
		for (int j = 0; j < n_num; j++)
		{
			if (numbers[i] == lottery_num[j])
			{
				printf("%3hd ", numbers[i]);
				check_val++;
			}
		}
	}

	return check_val; 
}


void print_wins(int check_val)
{
	char *_prize = "$"; 
	switch (check_val)
	{
        case 2: printf("5 %s", _prize); break;
        case 3: printf("50 %s", _prize); break; 
        case 4: printf("1000 %s", _prize); break;
        case 5: printf("50.000 %s", _prize); break;
        case 6: printf("1.000.000 %s", _prize); break;
        default: printf("0 %s", _prize);
	}

	return;
}


int is_unique(short *p_array, int arr_num, int tmp_num)
{ 
	for (int i = 0; i < arr_num; i++) 
	{
		if (tmp_num == p_array[i]) 
		{
			return 0;
		}
	}

	return 1; 
}


int is_input_unique(short* p_array, int arr_num, int tmp_num)
{
	if (is_unique(p_array, arr_num, tmp_num))
	{
		return 1;
	}
    else
    {
		printf("You already entered this number\n");
	    return 0;
    }
}
