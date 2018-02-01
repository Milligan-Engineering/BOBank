#include <iostream>
#include <string>
using namespace std;
const int maxPlayers = 6;


int main()
{
	// Provide space for storing the player names
	string playerName[maxPlayers];


	//Provide space for storing the turn times
	int turnTimeMinutes;
	int turnTimeSeconds;
	double secondsPerMinute = 60;


	int i = 0;
	int numberPlayers = 0;
	int value;

	cout.setf(ios::fixed);
	cout.setf(ios::showpoint);
	cout.precision(2);

	cout << "Welcome to the B&O Banker Assitant \n";

	while((numberPlayers < 3) || (numberPlayers > 6)) // Check for valid input. If not - keep asking
	{
		cout << "Enter number of players between 3 and 6. \n";
		cin >> numberPlayers;
	}
		cout << "There will be " << numberPlayers << " players. \n";
	
		
	while(i < numberPlayers) // Get name for each player
	{
		cout << "What is the name of the player " << i << "? \n";
		cin >> playerName[i];
		cout << "Player " << i << " is " << playerName[i] << endl << endl; // echoing input for validation
		i++;
	}

//  Get turn time 
	cout << "Enter the desired minutes of turn time: ";
	cin >> turnTimeMinutes;

	cout << "Enter the desired seconds of turn time: ";
	cin >> turnTimeSeconds;

	double turnTimeCombined = turnTimeMinutes + turnTimeSeconds / secondsPerMinute; // Convert to fractional minutes
	cout << "The turn time is " << turnTimeMinutes << " minutes and " << turnTimeSeconds << " seconds or " << turnTimeCombined << " minutes \n";

	int currentPlayer = 0;
	bool goodInput = false;
	char option;

// Menu for player actions
	while (!goodInput)
	{
		cout << "What would " << playerName[currentPlayer] << " like to do? \n";
		cout << "   'b' = buy shares\n";
		cout << "   's' = sell shares\n";
		cout << "   'o' = orphan share buyback \n";
		cout << "   'p' = pass \n";
		cin >> option;
		switch (option)
		{
		case 'b':
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
			cout << "Invalid option. Try agian. \n";
		}
	}

	cout << "Enter character to continue \n";
	cin >> value;

	return(0);
}