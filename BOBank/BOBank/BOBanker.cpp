#include <iostream>
using namespace std;



int main()
{
	// Provide space for storing the player names
	char player1;
	char player2;
	char player3;
	char player4;
	char player5;
	char player6;

	//Provide space for storing the turn times
	int turnTimeMinutes;
	int turnTimeSeconds;
	int secondsPerMinute = 60;


	int value;

	cout.setf(ios::fixed);
	cout.setf(ios::showpoint);
	cout.precision(2);

	cout << "Welcome to the B&O Banker Assitant \n";

	cout << "What is the initial of the first player? \n";
	cin >> player1;
	cout << "Player 1 is " << player1 << endl; // echoing input for validation

	cout << "What is the initial of the second player? \n";
	cin >> player2;
	cout << "Player 2 is " << player2 << endl;

	cout << "What is the initial of the third player? \n";
	cin >> player3;
	cout << "Player 3 is " << player3 << endl;

	cout << "What is the initial of the fourth player? \n";
	cin >> player4;
	cout << "Player 4 is " << player4 << endl;

	cout << "What is the initial of the fifth player? \n";
	cin >> player5;
	cout << "Player 5 is " << player5 << endl;

	cout << "What is the initial of the sixth player? \n";
	cin >> player6;
	cout << "Player 6 is " << player6 << endl;

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