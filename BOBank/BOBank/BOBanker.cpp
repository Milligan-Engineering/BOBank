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


	int i = 1;
	int numberPlayers = 0;
	int value;

	cout.setf(ios::fixed);
	cout.setf(ios::showpoint);
	cout.precision(2);

	cout << "Welcome to the B&O Banker Assitant \n";

	while((numberPlayers < 3) || (numberPlayers > 6))
	{
		cout << "Enter number of players between 3 and 6. \n";
		cin >> numberPlayers;
	}
		cout << "There will be " << numberPlayers << " players. \n";
	while(i <= numberPlayers)
	{
		cout << "What is the name of the player " << i << "? \n";
		cin >> playerName[i];
		cout << "Player " << i << " is " << playerName[i] << endl << endl; // echoing input for validation
		i++;
	}

	cout << "Enter the desired minutes of turn time: ";
	cin >> turnTimeMinutes;

	cout << "Enter the desired seconds of turn time: ";
	cin >> turnTimeSeconds;

	double turnTimeCombined = turnTimeMinutes + turnTimeSeconds / secondsPerMinute;
	cout << "The turn time is " << turnTimeMinutes << " minutes and " << turnTimeSeconds << " seconds or " << turnTimeCombined << " minutes \n";

	cout << "Enter character to continue \n";
	cin >> value;

	return(0);
}