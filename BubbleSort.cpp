/*	Name:		Joel Santiago
 *	Purpose:	Main source file for the BubbleSort program which creates a randomized array, sorts it, then prints it
 *
 *				File contains the following functions:
 *					1. BubbleSort() = Sorts array
 *					2. PrintArray() = Displays the array values
 *					3. Main()		= Creates randomized array, calls PrintArray(), BubbleSort() and PrintArray() functions
 *	Date:		Feb 26, 2013
 *
 */////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
// Header file necessary for the time variable in the SRAND(time(NULL)) declaration
#include <ctime>

using namespace std;

/*	Function:	BubbleSort(int *array, int length)
 *	return:		function returns the array back in sorted asending order
 *	parameter:	int *array = pointer for the array and values being passed into the BubbleSort() function to get sorted
 *				int length = the array length identifier that specifies how long the for loops should run for
 *	Desc:		Function is built to take the random array and values it has been passed and sort them in ascending order
 */
void BubbleSort(int *array, int length)
{
	// Array location identifiers that specify aray that signify the unsorted and sorted arrays and their locations
	int i, j;
    
	// For loop that counts through the passed array
	for (i = 0; i < length; i++)
	{
		// For loop that counts through the alternate array
		for (j = 0; j < i; j++)
		{
			// Check to see if the first array is less than the alternate array, ascending order is specified here
			// a > would result in descending order
			if (array[i] < array[j])
			{
				// Assigns the value of array[i] to the temp variable
				int temp = array[i];
				// Assigns the value of array[j] to array[i]
				array[i] = array[j];
				// Assigns the value stored in temp originally from array[i] to array[j]
				array[j] = temp;
			}
		}
	}
}


/*	Function:	printArray(int *array, int length)
 *	return:		function outputs the passed array value on screen
 *	parameters:	int *array = pointer for the array and values being passed into the BubbleSort() function to get sorted
 *				int length = the array length identifier that specifies how long the for loop should run for
 *	Desc:		Function is built to output the values of whatever array is passed to it
 */
void PrintArray(int *array, int length)
{
	// For loop that counts through the entire length of the array
	for (int i = 0; i < length; i++)
		
		// Code that outputs the value of the current array location
		cout << array[i] << " ";
}


/*	Function:	main()
 *	return:		main function returns the randomized array, sorts the array, and then outputs the sorted array
 *	Desc:		Function is built to create an array of 100 randomized values ranging from 0 - 1000, out the
 *				randomized array, sort it in ascending order using the BubbleSort() function and then output
 *				the newly sorted array
 */
int main()
{
	// Identifier for the overall length of the array
	const int length = 100;
	// Max range value for the rand() function
	int high = 1000;
	// Low range value for the rand() function
	int low = 0;
	// Array definition with the length identifier specifying total length of array
	int a[length];
    
	// SRAND declaration with time NULL as an argument that ensures random results are printed at each run of the program
	srand(time(NULL));
	
	// For loop that counts through the entire length of the array using the length identifier of 100
	for(int i = 0; i < length; i++)
		
		// Assigns a random value within the range of 0 and 1000 to the current array element
		a[i] = rand() % (high - low + 1);
    
	// Function call for the printArray function that outputs the initial unsorted array and its values
	// parameter:	a = the array being passed containing all randomly chosen and unsorted values
	//				length = value identifier for how long the array is, is used for the for loops that run through the array
	PrintArray(a,length);
	cout << endl << endl;
	
	// Function call for the BubbleSort function that takes pased array with randomly chosen, unsorted values, and sorts it in
	// ascending order
	// parameter:	a = the array being passed containing all randomly chosen and unsorted values
	//				length = value identifier for how long the array is, is used for the for loops that run through the array
	BubbleSort(a,length);
	
	// Function call for the printArray function that outputs the newly sorted array and its values
	// parameter:	a = the array being passed containing all randomly chosen but now sorted values
	//				length = value identifier for how long the array is, is used for the for loops that run through the array
	PrintArray(a,length);
	cout << endl << endl;
}