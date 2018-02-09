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
int setupPlayers();
//Precondition: The maximum number of players is stored in constant maxPlayers
//Postcondition: Player variables playerName, playerCash, playerTurnorder are populated with initial values. Returns number of players if sucessful 0 if not.

void playerInformation(int numberPlayers);
//Precondition: Player variables playerName, playerCash, playerTurnorder have their current values.
// The number of players is passed in numberPlayers.
//Postcondition: Player information is written to console

int setPlayerTurnorder(int numberPlayers);
// Preconditions: Current player cash and turn order are in playerNetworth and playerTurnorder.
// The number of players is passed in numberPlayers.
// Postcondition: playerTurnorder is reevaluated so that the lowest cash is first (order =0) and the highest is last (order = numberPlayers).
// Returns true if successful.


const int maxPlayers = 6;
const int totalInitialCash = 1500;

// Provide space for storing the player names, turnorder and cash
string playerName[maxPlayers];
int playerTurnorder[maxPlayers]; // index 0 stores index of first player
int playerCash[maxPlayers];


int main()
{
	int numberPlayers = 0;
	int value;

// Use two digits for double output
	cout.setf(ios::fixed);
	cout.setf(ios::showpoint);
	cout.precision(2);

	cout << "Welcome to the B&O Banker Assitant \n";

	numberPlayers = setupPlayers();

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


int setupPlayers()
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
	playerInformation(numberPlayers);

	//Sort players by cash
	int test = setPlayerTurnorder(numberPlayers);

	// list players in turnorder
	playerInformation(numberPlayers);

	// Set up initial player cash and net worth
	for (int i = 0; i < numberPlayers; i++)
	{
		playerCash[i] = totalInitialCash/numberPlayers;
	}
	return(numberPlayers);

}




void playerInformation(int numberPlayers)
{
	cout << endl;
	for (int i = 0; i < numberPlayers; i++)
	{
		cout << playerName[playerTurnorder[i]] << " has turn order " << i + 1 << " and $" << playerCash[playerTurnorder[i]] << endl;
	}
	return;

}


int setPlayerTurnorder(int numberPlayers)
{
	// Determine  player turn order		
	
	// Sort players in order of cash
	int temp;
	for (int i = 0; i < numberPlayers - 1; i++)
	{
		for (int j = 0; j < numberPlayers - i - 1; j++)
		{
			if (playerCash[playerTurnorder[j]] > playerCash[playerTurnorder[j + 1]])
			{
				temp = playerTurnorder[j];
				playerTurnorder[j] = playerTurnorder[j + 1];
				playerTurnorder[j + 1] = temp;
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