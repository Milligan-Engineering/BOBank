// Filename: Test.cpp 
// Author: JJ Giesey
//Email: jjgiesey@milligan.edu
//Project: Quiz 09
//Description: Example of function call
//Last Modified: 02/09/18

#include <iostream>
#include <string>
#include <stdlib.h> // need to include library for fundtion rand, srand
#include <time.h>  // need to include library for fundtion time

using namespace std;

// Function Declarations
int summer(int value1, int value2);
//Precondition: The two values to be added are passed in value1 and value 2
//Postcondition: The sum of these values is returned.


int main()
{
	int a = 3, b=4;
	int a,b,sum;

	a = 3;
	b = 4;
	sum = summer(a, b)
	cout << "a=" << a << " and b=" << b << " so sum is " << sum << endl;

	return(0);
}

int summmer(int val1, int val2)
{
	int TheSum;
	TheSumm = val1 + val2;
	return(TheSum);
}
