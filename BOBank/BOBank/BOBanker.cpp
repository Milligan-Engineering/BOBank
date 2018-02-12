// Filename: BOBanker.cpp 
// Author: JJ Giesey
//Email: jjgiesey@milligan.edu
//Project Milestone: 05 b
//Description: Program to handle banking duites in B&O Boardgame
//Last Modified: 02/08/18


#include <iostream>
#include <string>
#include <stdlib.h> // need to include library for fundtion rand, srand
#include <time.h>  // need to include library for fundtion time

using namespace std;

// Function Declarations
int setupPlayers(string playerName[], int playerCash[], int playerTurnorder[]);
//Precondition: The maximum number of players is stored in constant maxPlayers
//Postcondition: Player variables playerName, playerCash, playerTurnorder are populated with initial values. Returns number of players if sucessful 0 if not.

void PrintInformation(string SArray[], int IArray1[], int OArray[], int ArraySize);
//Precondition: Equal sized arrays of string and integer are passed SArray1 and IArray1. 
//The order of the arrays to be printes is passed in OArray and the size is passed in ArraySize.
//Postcondition: The values of the three arrays will be output to console seperated by tabs in order.

int setPlayerTurnorder(int cash[], int order[], int numberPlayers);
// Preconditions: Current player cash and turn order are in playerNetworth and playerTurnorder.
// The number of players is passed in numberPlayers.
// Postcondition: playerTurnorder is reevaluated so that the lowest cash is first (order =0) and the highest is last (order = numberPlayers).
// Returns true if successful.

int Sorter(int Values[], int Order[], int Size);
//Preconditions: An array with maginitudes stored in Values and the order is stored in Order (0 first, 1 second, ...)
//The size of the arrays are stored in Size
//Postcondition: Order is modified so that the smallest value has order 0 the second smallest has order 1 ...



const int maxPlayers = 6;
const int totalInitialCash = 1500;

// Provide space for storing the player names, turnorder and cash


int main()
{
	int numberPlayers = 0;
	int value;
	string playerName[maxPlayers];
	int playerTurnorder[maxPlayers]; // index 0 stores index of first player
	int playerCash[maxPlayers];


// Use two digits for double output
	cout.setf(ios::fixed);
	cout.setf(ios::showpoint);
	cout.precision(2);

	cout << "Welcome to the B&O Banker Assitant \n";

	numberPlayers = setupPlayers(playerName,playerCash,playerTurnorder);

	int currentPlayer = 0;
	bool goodInput;
	char option;

// Menu for player actions
	do
	{
		cout << "What would " << playerName[currentPlayer] << " like to do? \n";
		cout << "   'b' = buy shares\n";
		cout << "   's' = sell shares\n";
		cout << "   'o' = orphan share buyback \n";
		cout << "   'p' = pass \n";
		cin >> option;
		switch (option)
		{
		case ('b'):
			cout << playerName[currentPlayer] << " will buy shares. \n";
			goodInput = true;
			break;
		case 's':
			cout << playerName[currentPlayer] << " will sell shares. \n";
			goodInput = true;
			break;
		case 'o':
			cout << playerName[currentPlayer] << " will buy orphaned shares. \n";
			goodInput = true;
			break;
		case 'p':
			cout << playerName[currentPlayer] << " will pass. \n";
			goodInput = true;
			break;
		default:
			cout << "Invalid option. Try again. \n";
			goodInput = false;
		}
	} while (!goodInput);
	cout << "Enter character to continue \n";
	cin >> value;

	return(0);
}


int setupPlayers(string playerName[], int playerCash[], int playerTurnorder[])
{
	// Set up players names
	int numberPlayers=0;

	while ((numberPlayers < 3) || (numberPlayers > 6)) // Check for valid input. If not - keep asking
	{
		cout << "Enter number of players between 3 and 6. \n";
		cin >> numberPlayers;
	}
	cout << "There will be " << numberPlayers << " players. \n";


	for (int i = 0; i < numberPlayers; i++) // Get name for each player
	{
		cout << "What is the name of the player " << i << "? \n";
		cin >> playerName[i];
		cout << "Player " << i + 1 << " is " << playerName[i] << endl << endl; // echoing input for validation
	}


	// Set random initial turn order
	long int currentTime = static_cast<long int>(time(0)); //Generate random seed
	srand(currentTime);

	for (int i = 0; i < numberPlayers; i++)
	{
		playerTurnorder[i] = i;	// assign initial turn order
		playerCash[i] = rand() % 10000; // assign randome number
	}


	// list players in turnorder
	cout << "Name \t Cash\t Order\t \n";
	PrintInformation(playerName, playerCash, playerTurnorder, numberPlayers);

	//Sort players by cash
	int test = setPlayerTurnorder(playerCash, playerTurnorder, numberPlayers);

	// Set up initial player cash and net worth
	for (int i = 0; i < numberPlayers; i++)
	{
		playerCash[i] = totalInitialCash/numberPlayers;
	}

	// list players in turnorder
	cout << "Name \t Cash\t Order\t \n";
	PrintInformation(playerName, playerCash, playerTurnorder, numberPlayers);

	return(numberPlayers);

}




void PrintInformation(string SArray[], int IArray1[], int OArray[], int ArraySize)
{

	for (int i = 0; i < ArraySize; i++)
	{
		cout << SArray[OArray[i]] << " \t " << IArray1[OArray[i]] << " \t " << OArray[OArray[i]] << endl;
	}
	cout << endl;
	return;

}


int setPlayerTurnorder(int playerCash[], int playerTurnorder[], int numberPlayers)
{
	// Determine  player turn order		
	// Sort players in order of cash
	Sorter(playerCash, playerTurnorder, numberPlayers);
	
	return(0);
}

int Sorter(int Values[], int Order[], int Size)
{
	int temp;
	for (int i = 0; i < Size - 1; i++)
	{
		for (int j = 0; j < Size - i - 1; j++)
		{
			if (Values[Order[j]] > Values[Order[j + 1]])
			{
				temp = Order[j];
				Order[j] = Order[j + 1];
				Order[j + 1] = temp;
			}
		}
	}
	return(0);
}




/* 
double getTurnTime()
{

	// Provide space for storing the turn times
		int turnTimeMinutes;
		int turnTimeSeconds;
		double secondsPerMinute = 60;

	//  Get turn time 
	cout << "Enter the desired minutes of turn time: ";
	cin >> turnTimeMinutes;

	cout << "Enter the desired seconds of turn time: ";
	cin >> turnTimeSeconds;

	double turnTimeCombined = turnTimeMinutes + turnTimeSeconds / secondsPerMinute; // Convert to fractional minutes
	cout << "The turn time is " << turnTimeMinutes << " minutes and " << turnTimeSeconds << " seconds or " << turnTimeCombined << " minutes \n";

	return(turnTimeCombined);
}
*/