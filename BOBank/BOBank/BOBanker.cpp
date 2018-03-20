#include <iostream>
#include <string>
#include <stdlib.h>     
#include <time.h>  
#include <fstream>

using namespace std;

//Constants
const int maxPlayers = 6;
const int maxCompanies = 10;
const int maxTrains = 30;
const int maxCities = 40;
const int initialCash[6] = { 1500, 750, 500, 375, 300, 250 };
const int valuations[26] = { 34, 37, 41, 45, 50, 55, 60, 66, 74, 82, 91, 100, 110, 121, 133, 146, 160, 176, 194, 213, 234, 257, 282, 310, 341, 375 };
const int initialValuations[6][3] = { { 5, 6, 7 },
{ 6, 7, 8 },
{ 7, 8, 9 },
{ 8, 9, 10 },
{ 9, 10, 11 } };
const char companyStartcities[maxCompanies][35] = { "Baltimore", "Boston", "Richmond", "Saint Louis", "Buffalo", "Albany", "Richmond", "Saint Louis","Buffalo", "Albany" };
const int totalInitialCash = 1500;

//Global Variables
int numberPlayers = 3;
int numberActiveCompanies;
int currentPlayer;
int currentCompany;
int currentTechlevel = 1; //Actual tech level between 1 and 6
int levelValuation;
int playerTurnorder[maxPlayers]; //ordered list of the players in order of play
int companyTurnorder[maxCompanies] = { 0,1,2,3,4,5,6,7,8,9 };//ordered list of the companies in order of play
char testArray[30];


															 // Player Variables
string playerName[maxPlayers];
int playerCash[maxPlayers];
int playerNetworth[maxPlayers];
int playerShares[maxPlayers][maxCompanies] = { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } };
int playerSold[maxPlayers][maxCompanies] = { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } };


// Structures
struct Company
{
	string name;
	int available;
	int cash;
	int netprofit;
	int valuation;
	int shares;
	int orphans;
	int president;
	int order;
	bool started;
	bool sold;
	bool recievership;
	bool cities[maxCities];
	int trains[maxTrains]; // 0 = never owned, 1=owned, -1 = scrapped
};


Company companies[maxCompanies];

// Structure of arrays
struct Trains
{

	const int cost[30] = { 100, 95,90,85,80,140,130,120,110,100,200,185,170,155,135,280,260,240,220,200,380,355,330,305,280,500,470,440,410,380 };
	const int level[30] = { 1,1,1,1,1,2,2,2,2,2,3,3,3,3,3,4,4,4,4,4,5,5,5,5,5,6,6,6,6,6 };
	const int scrap[30] = { 20,20,20,20,20,40,40,40,40,40,60,60,60,60,60,80,80,80,80,80,100,100,100,100,100,120,120,120,120,120 };
	const int number[30] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30 };

};

Trains train;


struct Cities
{
	const string name[maxCities] = { "Albany", "Augusta", "Baltimore", "Boston", "Buffalo", "Burlington", "Cairo", "Chicago", "Cincinnati", "Cleveland", "Concord", "Detroit", "Dover", "FortWayne", "Harrisburg", "Hartford", "Hunington", "Indainapolis", "Lexington", "Lousiville", "NewHaven", "NewYork", "Norfolk", "Philadelphia", "Pittsburg", "Portsmouth", "Provience", "Richmond", "Roanoke", "SaintLouis", "Springfield", "Syracuse", "Utica", "Washington", "Wheeling", "Coal1", "Coal2", "Coal3", "Coal4", "Coal5" };
};

Cities cities;

// Function Declarations
int setupPlayers(string playerName[], int playerCash[], int playerTurnorder[]);
//Precondition: The maximum number of players is stored in constant maxPlayers
//Postcondition: Player variables playerName, playerCash, playerTurnorder are populated with initial values. 
//				Returns number of players if sucessful and 0 if not.

void PrintInformation(string SArray[], int IArray1[], int OArray[], int ArraySize);
//Precondition: Equal sized arrays of string and integer are passed SArray1 and IArray1. 
//				The order of the arrays to be printes is passed in OArray and the size is passed in ArraySize.
//Postcondition: The values of the three arrays will be output to console seperated by tabs in order.

int setPlayerTurnorder(int cash[], int order[], int numberPlayers);
// Preconditions: Current player cash and turn order are in playerNetworth and playerTurnorder.
//				The number of players is passed in numberPlayers.
// Postcondition: playerTurnorder is reevaluated so that the lowest cash is first (order =0) and the highest is last (order = numberPlayers).
//				Returns 0 if successful.

int Sorter(int Values[], int Order[], int Size);
//Preconditions: An array with maginitudes stored in Values and the order is stored in Order (0 first, 1 second, ...)
//				The size of the arrays are stored in Size
//Postcondition: Order is modified so that the smallest value has order 0 the second smallest has order 1 ...

int attemptBuy(int currentPlayer);
//Preconditions: The index of the player planning on buying is passed  in currentPlayer
//Postconditions: If successful the shares and money of player and company are adjusted based on user input and a 1 is returned.
//				  If not a zero is returned and the reason for failure is output to screen

int attemptSell(int currentPlayer);
//Preconditions: The index of the player planning on selling is passed  in currentPlayer
//Postconditions: If successful the shares and money of player and company are adjusted based on user input and a 1 is returned.
//				  If not a zero is returned and the reason for failure is output to screen

int attemptOrphan(int currentPlayer);
//Preconditions: The index of the player planning on buying orphans for their company is passed  in currentPlayer
//Postconditions: If successful the shares and money of the  company are adjusted based on user input and a 1 is returned.
//				  If not a zero is returned and the reason for failure is output to screen


void setCompanyTurnorder(int direction, int companyTurnorder[], int currentCompany);
//Preconditions: The current order of companies is given in companyTurnorder and the valuations are in companyValuations.
//the company whose value has changed is listed in current Company and the direction of the change is given in
// direction 1=increase and -1 = decrease.
//Postcondition: companyTurnorder is modified so that currentCompany is put in its proper place (0 is first) due to the change.

int setCompanyPresident(int currentCompany, int playerShares[][maxCompanies], int companyCurrentPresident);
// Preconditions: currentCompany is a vaild company. The shares ownd by players is passed through playerShares and the current president it passed
// through companyPresident
// Postcondidtion: The number of shares owned by each player is compared and if a new player has more shares their number is returned through setCompanyPresident.
// If not the old president is returned.

void setCompanyValuation(int CurrentCompany, int netProfitChange, char companyDividenPlan);
//Preconditions: For the company given by CurrentCompany that had a net profit change given by netProfitChange (decrease = -1, increase = 1
// and same = 0) and the decision to withhold (w) or distribute (d) dividens given in companyDividenPlan. Function also uses companyValuations
//Postcondition: Valuation of company is decrease if it had a decrease in net profit. It increase if net profit increased and dividens were paied. 
//Otherwise valuation stays the same.

void setCompanyDividens(int currentCompany, char companyDividenPlan);
//Preconditions: For the current company given in currentCompany to pay dividens to shareholders and itself. Uses companyNetprofit, companySharesowned, and playerShares.
//Postcondition: Profits distributed to companyCash and playerCash

void decreaseCompanyValuation(int currentCompany);
// Preconditions: The company specified by currentCompany and the company valuations given in companyValuation.
//Postcondition: The valuation for current company is reduce by one level. Company turn order is reevaluated.

void increaseCompanyValuation(int currentCompany);
// Preconditions: The company specified by currentCompany and the company valuations given in companyValuation.
//Postcondition: The valuation for current company is increased by one level. Company turn order is reevaluated.

int companyPlayerSharesOwned(int currentCompany, const int playerShares[][maxCompanies]);
//Preconditions: For the company - currentCompany the number of shares owned by players is 
// in playerShares.
//Postconditions: function returns the sum of shares currently owned by players

int fetchFileData(string pfileName, int orderPlayers[]);
//Preconditions: Name of the file is passed in pfileName.
//Postcondition: Player, company and train arrays are populated with values taken from file.
//				If successful a 0 is returned.		

int pushFileData(string pfileName);
//Preconditions: Name of the file is passed in pfileName.
//Postcondition: Player, company and train arrays are stored in file.
//				If successful a 0 is returned.		

char readValue(ifstream& inputStream, char cArray[]);
//Precondtions: Open stream in in inputStrean and character array is returned in cArray
//Postcondition: Will return last character read.

char findEOL(ifstream& inputStream);
//Precondition: Stream to be read for end of line is in inputStream
//				It is expected that all relevant values have been read
//Postcondition: Stream cleared. Last character returned. Should be /n if successful




void companyInformation();


int main()
{
	int playerSuccess;
	string wfileName;
	char option;

	// initialize company values
	const int companyAvailable[maxCompanies] = { 1,1,1,3,3,1,3,1,1,3 };
	const char companyName[maxCompanies][35] = { "Baltimore & Ohio", "Boston & Maine", "Chesapeake & Ohio", "Illinois Central", "Erie", "New York Central","Nickel Plate", "New York New Haven & Hartford", "Pennsylvania", "Wabash" };
	for (int j = 0; j < maxCompanies; j++)
	{
		companies[j].name = companyName[j];
		companies[j].available = companyAvailable[j];
		companies[j].cash = 0;
		companies[j].netprofit = 0;
		companies[j].valuation = 0;
		companies[j].shares = 10;
		companies[j].orphans = 0;
		companies[j].president = maxPlayers;
		companies[j].order = j;
		companies[j].started = false;
		companies[j].recievership = false;
		for (int k = 0; k < maxCities; k++)
		{
			companies[j].cities[k] = false;
		}
		for (int k = 0; k < maxCities; k++)
		{
			companies[j].trains[k] = 0;
		}
	}





	cout << "BO Banker Program \n \n";
	//  Set up

	cout << "Name of game data file:";
	cin >> wfileName;
	wfileName = wfileName + ".csv";
//	strcat_s(wfileName, ".txt");
	cout << "What would you like to do? \n";
	cout << "   'n' = Start new game\n";
	cout << "   'f' = Read game state from file\n";
	cout << "Option: ";
	cin >> option;
	cout << endl;

	switch (option)
	{
	case 'f':
		fetchFileData(wfileName,playerTurnorder);
		break;
	default:
	numberPlayers = setupPlayers(playerName, playerCash, playerTurnorder);	
	}
	pushFileData(wfileName);
	PrintInformation(playerName, playerCash, playerTurnorder, numberPlayers);

	do
	{
		// Market Phase
		char marketOption;
		int numberPasses = 0;

		cout << "MARKET PHASE \n";
		while (numberPasses < numberPlayers)
		{
			numberPasses = 0;
			for (int i = 0; i < numberPlayers; i++)
			{
				currentPlayer = playerTurnorder[i];
				playerSuccess = 0;
				cout << playerName[currentPlayer] << ": Cash = $" << playerCash[currentPlayer] << " and New Worth = $" << playerNetworth[currentPlayer] << "\n";
				while (playerSuccess != 1)
				{
					cout << "Choose Option: b=Buy, s=Sell, o=Oprhan Buyback, p=Pass:";
					cin >> marketOption;
					switch (marketOption)
					{
					case 'b':
						playerSuccess = attemptBuy(currentPlayer);
						break;
					case 's':
						playerSuccess = attemptSell(currentPlayer);
						break;
					case 'o':
						playerSuccess = attemptOrphan(currentPlayer);
						break;
					case 'p':
						numberPasses++;
						cout << playerName[currentPlayer] << " passes. Pass:" << numberPasses << endl;
						playerSuccess = 1;
						break;
					default:
						cout << "Invalid option. \n";
					}
				}

			}
			cout << "\n \n Next Round \n";
			PrintInformation(playerName, playerCash, playerTurnorder, numberPlayers);

		}
		// Build 1 Round
		int companyBuildcosts;
		int companyNewNetprofit;
		char companyConfirm;
		int netprofitChange;
		char companyDividenPlan;


		cout << "\n \n All player have passed. Begin Build Round 1 \n";
		setPlayerTurnorder(playerCash, playerTurnorder, numberPlayers);
		companyInformation();


		for (int j = 0; j < maxCompanies; j++)
		{
			currentCompany = companyTurnorder[j];
			if (companies[currentCompany].started != 0)
			{
				cout << "\n Company " << companies[currentCompany].name << " can build. \n";
				do
				{
					cout << j << ": " << companies[currentCompany].name << " has $" << companies[currentCompany].cash<< " and " << companies[currentCompany].shares << " regular shares and " << companies[currentCompany].orphans << " orphaned shares at a value of $" << valuations[companies[currentCompany].valuation] << " per share\n";
					cout << "Enter total build costs (enter 0 to finish building): ";
					cin >> companyBuildcosts;
					if (companyBuildcosts <= companies[currentCompany].cash)
					{
						companies[currentCompany].cash = companies[currentCompany].cash - companyBuildcosts;
						cout << companies[currentCompany].name << " built $" << companyBuildcosts << " and has $" << companies[currentCompany].cash << " cash \n";
					}
					else
					{
						cout << companies[currentCompany].name << " does not have enough cash to build $" << companyBuildcosts << endl;
					}
				} while (companyBuildcosts != 0);
				companyConfirm = 'n';
				do
				{
					cout << "Finished building. Enter new net profit $";
					cin >> companyNewNetprofit;
					cout << "New net profit for " << companies[currentCompany].name << " is $" << companyNewNetprofit << ". Is this correct? (y/n) \n";
					cin >> companyConfirm;
				} while (companyConfirm != 'y');
				companyConfirm = 'n';
				if (companyNewNetprofit > companies[currentCompany].netprofit)
					netprofitChange = 1;
				else if (companyNewNetprofit < companies[currentCompany].netprofit)
					netprofitChange = -1;
				else
					netprofitChange = 0;
				companies[currentCompany].netprofit = companyNewNetprofit;
				do
				{
					cout << "Do you want to distribute=d or withold=w dividens? ";
					cin >> companyDividenPlan;
					if (companyDividenPlan == 'd')
					{
						cout << companies[currentCompany].name << " would like to distribute dividens, Is this correct? (y/n) \n";
						cin >> companyConfirm;
						setCompanyDividens(currentCompany, companyDividenPlan);
					}
					else if (companyDividenPlan == 'w')
					{
						cout << companies[currentCompany].name << " would like to withhold dividens, Is this correct? (y/n) \n";
						cin >> companyConfirm;
						setCompanyDividens(currentCompany, companyDividenPlan);
					}
					else
					{
						cout << "Invalid entry. Please try again \n";
					}
				} while (companyConfirm != 'y');
				setCompanyValuation(currentCompany, netprofitChange, companyDividenPlan);
			}
		}
	} while (currentTechlevel < 6);
	return 0;
}

int setupPlayers(string playerName[], int playerCash[], int playerTurnorder[])
{
	// Set up players names
	int numberPlayers = 0;

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
		playerCash[i] = totalInitialCash / numberPlayers;
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
	int test = Sorter(playerCash, playerTurnorder, numberPlayers);

	return(0);
}

void decreaseCompanyValuation(int currentCompany)
{
	if (companies[currentCompany].valuation> 0)
	{
		companies[currentCompany].valuation --;
		cout << companies[currentCompany].name << " has valuation reduced to $" << valuations[companies[currentCompany].valuation] << " per share \n";
	}
	else
	{
		cout << companies[currentCompany].name << " valuations remains at $" << valuations[companies[currentCompany].valuation] << " per share \n";
	}
	setCompanyTurnorder(-1, companyTurnorder, currentCompany);
	companyInformation();
	return;
}

void increaseCompanyValuation(int currentCompany)
{
	if (companies[currentCompany].valuation < 26)
	{
		companies[currentCompany].valuation ++;
		cout << companies[currentCompany].name << " has valuation increased to $" << valuations[companies[currentCompany].valuation] << " per share \n";
	}
	else
	{
		cout << companies[currentCompany].name << " valuations remains at $" << valuations[companies[currentCompany].valuation] << " per share \n";
	}
	setCompanyTurnorder(1, companyTurnorder, currentCompany);
	companyInformation();
	return;
}

int attemptBuy(int currentPlayer)
{
	// Player requests to buy

	int numberShares;
	int currentCompany;
	int companyRequestValuation;

	cout << playerName[currentPlayer] << " wants buys stock. \n";
	cout << "Select company to buy from:\n";
	//List shares for sale
	for (int j = 0; j < maxCompanies; j++)
	{
		if (companies[j].available <= currentTechlevel)
		{
			if (playerSold[currentPlayer][j] != 1)
			{
				if (companies[j].started == 0)
				{
					cout << j << ": " << companies[j].name << " is available to be started at $" << valuations[initialValuations[currentTechlevel - 1][0]] << ", $" << valuations[initialValuations[currentTechlevel - 1][1]] << " or $" << valuations[initialValuations[currentTechlevel - 1][2]] << " per share\n";
				}
				else
				{
					cout << j << ": " << companies[j].name << " has " << companies[j].shares << " shares available at $" << valuations[companies[j].valuation] << " per share. \n";
				}
			}
		}
	}
	cin >> currentCompany;
	//Check if company available
	if (companies[currentCompany].available != 1)
	{
		cout << "Company " << companies[currentCompany].name << " is not available. \n";
		return 0;
	}
	// Check to see if company has not been started and needs intial value set
	if (companies[currentCompany].started == 0)
	{
		cout << "\t 1: $" << valuations[initialValuations[currentTechlevel - 1][0]] << endl;
		cout << "\t 2: $" << valuations[initialValuations[currentTechlevel - 1][1]] << endl;
		cout << "\t 3: $" << valuations[initialValuations[currentTechlevel - 1][2]] << endl;
		cout << "Select inital company valuation for " << companies[currentCompany].name << " :";
		cin >> levelValuation;
		companyRequestValuation = initialValuations[currentTechlevel - 1][levelValuation - 1];
		setCompanyTurnorder(1, companyTurnorder, currentCompany);
		companyInformation();
		cout << companies[currentCompany].name << " has requested an intial valuations of $" << valuations[companyRequestValuation] << " per share \n";
	}
	else
	{
		companyRequestValuation = companies[currentCompany].valuation;
	}
	cout << "How many shares of " << companies[currentCompany].name << " do you want to buy: ";
	cin >> numberShares;
	// check validity of request
	if (numberShares > companies[currentCompany].shares)
	{
		cout << "Only " << companies[currentCompany].shares << " shares available.\n";
		return 0;
	}
	else if (numberShares*valuations[companyRequestValuation] > playerCash[currentPlayer])
	{
		cout << "Need " << numberShares * valuations[companyRequestValuation] << " for purchase\n";
		return 0;
	}
	else // Proceed with sale
	{
		companies[currentCompany].started= 1;
		companies[currentCompany].valuation = companyRequestValuation;
		companies[currentCompany].shares = companies[currentCompany].shares - numberShares;
		playerShares[currentPlayer][currentCompany] = playerShares[currentPlayer][currentCompany] + numberShares;
		playerCash[currentPlayer] = playerCash[currentPlayer] - numberShares * valuations[companies[currentCompany].valuation];
		companies[currentCompany].cash = companies[currentCompany].cash + numberShares * valuations[companies[currentCompany].valuation];
		cout << companies[currentCompany].name << " now has $" << companies[currentCompany].cash << " and " << companies[currentCompany].shares << " regular shares and " << companies[currentCompany].orphans << " orphaned shares \n";
		cout << playerName[currentPlayer] << " now has " << playerShares[currentPlayer][currentCompany] << " shares of " << companies[currentCompany].name << endl;
		companies[currentCompany].president = setCompanyPresident(currentCompany, playerShares, companies[currentCompany].president);
		return 1;
	}
}

int attemptSell(int currentPlayer)
// This function will return a 1 when the player has sucessfully sold a stock for their turn.
{
	int numberShares;
	cout << playerName[currentPlayer] << " wants to sell stock. \n";
	for (int j = 0; j < maxCompanies; j++)
	{
		if (playerShares[currentPlayer][j] != 0)
		{
			cout << j << " " << companies[j].name << ": " << playerShares[currentPlayer][j] << " at $" << valuations[companies[j].valuation] << " per share \n";
		}
	}
	cout << "Select company to sell from:\n";
	cin >> currentCompany;
	cout << "How many shares of " << companies[currentCompany].name << " do you want to sell: ";
	cin >> numberShares;
	// check validity of request
	if (numberShares > playerShares[currentPlayer][currentCompany])
	{
		cout << "Only " << playerShares[currentPlayer][currentCompany] << " shares owned.\n";
		return 0;
	}
	else if (companyPlayerSharesOwned(currentCompany, playerShares) - numberShares <1)
	{
		cout << "Can not sell all shares of company " << companies[currentCompany].name << ". Reduce request. \n";
		return 0;
	}
	{
		companies[currentCompany].orphans = companies[currentCompany].orphans + numberShares;
		playerShares[currentPlayer][currentCompany] = playerShares[currentPlayer][currentCompany] - numberShares;
		playerCash[currentPlayer] = playerCash[currentPlayer] + numberShares * valuations[companies[currentCompany].valuation];
		cout << companies[currentCompany].name << " now has $" << companies[currentCompany].cash << " and " << companies[currentCompany].shares << " regular shares and " << companies[currentCompany].orphans << " orphaned shares \n";
		cout << playerName[currentPlayer] << " now has " << playerShares[currentPlayer][currentCompany] << " shares of " << companies[currentCompany].name << endl;
		companies[currentCompany].president = setCompanyPresident(currentCompany, playerShares, companies[currentCompany].president);
		// reduce evaluation if first sale of round
		playerSold[currentPlayer][currentCompany] = 1;
		decreaseCompanyValuation( currentCompany);
		return 0;
	}

}

int attemptOrphan(int currentPlayer)
{
	int numberShares;
	int numberOrphanCompaniesControlled = 0;

	cout << playerName[currentPlayer] << " wants a company to buy orphaned stock \n";
	cout << "Select company to buy its orphaned stock:\n";
	//List orphaned shares for sale
	for (int j = 0; j < maxCompanies; j++)
	{
		if (companies[j].orphans != 0 && currentPlayer == companies[j].president)
		{
			cout << j << ": " << companies[j].name << " has " << companies[j].orphans << " orphaned shares available at $" << valuations[companies[currentCompany].valuation] << " per share. \n";
			numberOrphanCompaniesControlled++;
		}
	}
	if (numberOrphanCompaniesControlled == 0)
	{
		cout << "No companies with orphan stocks \n";
		return 0;
	}
	cin >> currentCompany;
	if (currentPlayer != companies[currentCompany].president)
	{
		cout << "Only " << companies[currentCompany].name << " president, " << playerName[currentPlayer] << " can buy orphan shares on behalf of company \n";
		return 0;
	}
	cout << "How many orphaned shares of " << companies[currentCompany].name << " do you want to buy back: ";
	cout << companies[currentCompany].name << " has $" << companies[currentCompany].cash << endl;
	cin >> numberShares;
	// check validity of request
	if (numberShares > companies[currentCompany].orphans)
	{
		cout << "Only " << companies[currentCompany].orphans << " shares available.\n";
		return 0;
	}
	else if (numberShares*valuations[companies[currentCompany].valuation] > companies[currentCompany].cash)
	{
		cout << "Need " << numberShares * valuations[companies[currentCompany].valuation] << " for purchase\n";
		return 0;
	}
	else
	{
		companies[currentCompany].orphans = companies[currentCompany].orphans - numberShares;
		companies[currentCompany].shares = companies[currentCompany].shares + numberShares;
		companies[currentCompany].cash= companies[currentCompany].cash - numberShares * valuations[companies[currentCompany].valuation];
		cout << companies[currentCompany].name << " now has " << companies[currentCompany].shares << " regular shares and " << companies[currentCompany].orphans << " orphaned shares \n";
		return 1;
	}
}

int setCompanyPresident(int currentCompany, int playerShares[][maxCompanies], int companyCurrentPresident)
{
	int companyPresident;
	companyPresident = companyCurrentPresident;
	for (int i = 0; i < numberPlayers; i++)
	{
		if (playerShares[i][currentCompany] > playerShares[companyPresident][currentCompany])
		{
			companyPresident = i;
			cout << "New president for " << companies[currentCompany].name << " is " << playerName[i] << endl;
		}
	}
	return(companyPresident);
}

void companyInformation()
{
	int currentCompany;
	for (int j = 0; j < maxCompanies; j++)
	{
		currentCompany = companyTurnorder[j];
		if (companies[currentCompany].started == 1)
		{
			cout << j << ": " << companies[currentCompany].name << " now has $" << companies[currentCompany].cash << " and " << companies[currentCompany].shares << " regular shares and " << companies[currentCompany].orphans << " orphaned shares at a value of $" << valuations[companies[currentCompany].valuation] << " per share\n";
		}
		else if (companies[currentCompany].available > currentTechlevel)
		{
			cout << j << ": " << companies[currentCompany].name << " is not available\n";
		}
		else
		{
			cout << j << ": " << companies[currentCompany].name << " is not started\n";
		}
	}
	return;
}

void playerInformation()
{
	// Determine player net worths
	for (int i = 0; i < numberPlayers; i++)
	{
		playerNetworth[i] = playerCash[i];
		for (int j = 0; j < maxCompanies; j++)
		{
			playerNetworth[i] = playerNetworth[i] + playerShares[i][j] * valuations[companies[j].valuation];
		}
	}
	// List player cash and net worth in turn order
	cout << "Player turn order, cash and net worth \n";
	for (int i = 0; i < numberPlayers; i++)
	{
		cout << i << "  " << playerName[playerTurnorder[i]] << ": Cash = $" << playerCash[playerTurnorder[i]] << " and Net Worth = $" << playerNetworth[playerTurnorder[i]] << "\n";
	}
	return;

}

void setCompanyTurnorder(int direction, int companyTurnorder[], int currentCompany)
{
	int jstart;
	if (direction == 0) // Valuation decrement
	{
		jstart = 0;
		while (companyTurnorder[jstart] != currentCompany)
		{
			jstart++;
		}
		for (int j = jstart + 1; j < maxCompanies; j--)
		{
			if (companies[currentCompany].valuation < companies[companyTurnorder[j]].valuation)
			{
				companyTurnorder[j - 1] = companyTurnorder[j];
				companyTurnorder[j] = currentCompany;
			}
		}
		return;
	}
	else
	{
		jstart = maxCompanies - 1;
		while (companyTurnorder[jstart] != currentCompany)
		{
			jstart--;
		}
		for (int j = jstart - 1; j >= 0; j--)
		{
			if (companies[j].valuation > companies[companyTurnorder[j]].valuation)
			{
				companyTurnorder[j + 1] = companyTurnorder[j];
				companyTurnorder[j] = currentCompany;
			}
		}
		return;
	}
}


void setCompanyValuation(int CurrentCompany, int netProfitChange, char companyDividenPlan)
{
	if (netProfitChange < 0)
	{
		decreaseCompanyValuation(currentCompany);
	}
	else if ((netProfitChange > 0) && (companyDividenPlan == 'd'))
	{
		increaseCompanyValuation(currentCompany);
	}
	else
	{
		cout << "Company valuation remains the same \n";
	}
	return;
}

void setCompanyDividens(int currentCompany, char companyDividenPlan)
{
	companies[currentCompany].cash = companies[currentCompany].cash + companies[currentCompany].shares * companies[currentCompany].netprofit / 10;
	if (companyDividenPlan == 'd')
	{
		for (int i = 0; i < numberPlayers; i++)
		{
			playerCash[i] = playerCash[i] + playerShares[i][currentCompany] * companies[currentCompany].netprofit/ 10;
		}
		return;
	}
	else
	{
		for (int i = 0; i < numberPlayers; i++)
		{
			companies[currentCompany].cash = companies[currentCompany].cash + playerShares[i][currentCompany] * companies[currentCompany].netprofit / 10;
		}
		return;

	}
}

int companyPlayerSharesOwned(int currentCompany, const int playerShares[][maxCompanies])
{
	int shareSum = 0;
	for (int i = 0; i < numberPlayers; i++)
	{
		shareSum = shareSum + playerShares[i][currentCompany];
	}
	return shareSum;
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


char readValue(ifstream& inputStream, char cArray[])
{
	char getChar;
	inputStream.get(getChar);
	int j = 0;
	while ((getChar != ',') && (getChar != '\n'))
	{
		cArray[j] = getChar;
		j++;
		inputStream.get(getChar);
	}
	cArray[j] = '\0';
	return(getChar);
}

char findEOL(ifstream& inputStream)
{
	char getChar;
	do
	{
		inputStream.get(getChar);
	} while (getChar != '\n');
	return(getChar);
}


int fetchFileData(string pfileName, int orderPlayers[])
{
//	int val;
	char getChar;
	ifstream inDataStream;
	int rnumberPlayers = 0;
	string pName;
	inDataStream.open(pfileName);
	if (inDataStream.fail())
	{
		cout << "Input file stream open failed \n";
		return(1);
	}
	getChar = readValue(inDataStream, testArray);
	getChar = readValue(inDataStream, testArray);
	while (strcmp(testArray, "Done#") != 0)	//Read player names
	{
		playerName[rnumberPlayers] = testArray;
		orderPlayers[rnumberPlayers] = rnumberPlayers;
		rnumberPlayers++;
		getChar = readValue(inDataStream, testArray);

	}
	numberPlayers = rnumberPlayers;
	findEOL(inDataStream);
	getChar = readValue(inDataStream, testArray);
	for (int i = 0; i < numberPlayers; i++) // Read player cash
	{
		getChar = readValue(inDataStream, testArray);
		playerCash[i] =  atoi(testArray);
	}
	findEOL(inDataStream);
	getChar = readValue(inDataStream, testArray);
	for (int j = 0; j < maxCompanies; j++) // Read player shares owned
	{
		for (int i = 0; i < numberPlayers; i++)
		{
			getChar = readValue(inDataStream, testArray);
			playerShares[i] [j] = atoi(testArray);
		}
		findEOL(inDataStream);
		getChar = readValue(inDataStream, testArray);
	}
	for (int j = 0; j < maxCompanies; j++) //Read company name (and discard since fixed)
	{
		getChar = readValue(inDataStream, testArray);
	}
	findEOL(inDataStream);
	getChar = readValue(inDataStream, testArray);
	for (int j = 0; j < maxCompanies; j++)//Read company cash
	{
		getChar = readValue(inDataStream, testArray);
		companies[j].cash = atoi(testArray);
	}
	findEOL(inDataStream);
	getChar = readValue(inDataStream, testArray);
	for (int j = 0; j < maxCompanies; j++) //Read company net profit
	{
		getChar = readValue(inDataStream, testArray);
		companies[j].netprofit= atoi(testArray);
	}
	findEOL(inDataStream);
	getChar = readValue(inDataStream, testArray);
	for (int j = 0; j < maxCompanies; j++) //Read company valuation
	{
		getChar = readValue(inDataStream, testArray);
		companies[j].valuation = atoi(testArray);
	}
	findEOL(inDataStream);
	getChar = readValue(inDataStream, testArray);
	for (int j = 0; j < maxCompanies; j++) //Read company presidents
	{
		getChar = readValue(inDataStream, testArray);
		companies[j].president = atoi(testArray);
	}
	findEOL(inDataStream);
	getChar = readValue(inDataStream, testArray);
	for (int j = 0; j < maxCompanies; j++) //Read company shares
	{
		getChar = readValue(inDataStream, testArray);
		companies[j].shares =  atoi(testArray);
	}
	findEOL(inDataStream);
	getChar = readValue(inDataStream, testArray);
	for (int j = 0; j < maxCompanies; j++) //Read comapny orphans
	{
		getChar = readValue(inDataStream, testArray);
		companies[j].orphans = atoi(testArray);
	}
	findEOL(inDataStream);
	getChar = readValue(inDataStream, testArray);
	for (int j = 0; j < maxCompanies; j++) //Read company sold
	{
		getChar = readValue(inDataStream, testArray);
		companies[j].started = atoi(testArray);
	}
	findEOL(inDataStream);
	getChar = readValue(inDataStream, testArray);
	for (int j = 0; j < maxCompanies; j++) //Read company turn order
	{
		getChar = readValue(inDataStream, testArray);
		companies[j].order = atoi(testArray);
	}
	findEOL(inDataStream);
	getChar = readValue(inDataStream, testArray);
	for (int m = 0; m < maxCities; m++) // Read cities
	{
		getChar = readValue(inDataStream, testArray);
		for (int j = 0; j < maxCompanies; j++)
		{
			getChar = readValue(inDataStream, testArray);
			if (atoi(testArray) == 1)
				companies[j].cities[m]=1;
			else
				companies[j].cities[m] = 0;
		}
		findEOL(inDataStream);
	}
	for (int k = 0; k < maxTrains; k++) // Read trains
	{
		getChar = readValue(inDataStream, testArray);
		for (int j = 0; j < maxCompanies; j++)
		{
			getChar = readValue(inDataStream, testArray);
			companies[j].trains[k]= atoi(testArray);
		}
		findEOL(inDataStream);
	}
	return(currentTechlevel);
}

int pushFileData(string pfileName)
{

	ofstream outDataStream;
	outDataStream.open(pfileName);
	if (outDataStream.fail())
	{
		cout << "Output file stream open failed \n";
		return(1);
	}

	outDataStream << "Name, ";
	for (int i = 0; i < numberPlayers; i++) // Save player name
	{
		outDataStream << playerName[i] << ",";
	}
	outDataStream << playerName[numberPlayers] << "Done#," << "\n";
	outDataStream << "Cash, ";
	for (int i = 0; i < numberPlayers; i++) // Save player cash
	{
		outDataStream << playerCash[i] << ",";
	}
	outDataStream << "\n";
	for (int j = 0; j < maxCompanies; j++) // Save player shares owned
	{
		outDataStream << companies[j].name << ",";
		for (int i = 0; i < numberPlayers; i++)
		{
			outDataStream << playerShares[i][ j] << ",";
		}
		outDataStream << "\n";
	}
	outDataStream << "Names, ";
	for (int j = 0; j < maxCompanies; j++) // Save company names
	{
		outDataStream << companies[j].name << ",";
	}
	outDataStream << "\n";
	outDataStream << "Cash, ";
	for (int j = 0; j < maxCompanies; j++) // Save company cash
	{
		outDataStream << companies[j].cash << ",";
	}
	outDataStream << "\n";
	outDataStream << "NetProfit, ";
	for (int j = 0; j < maxCompanies; j++) // Save company netprofit
	{
		outDataStream << companies[j].netprofit << ",";
	}
	outDataStream << "\n";
	outDataStream << "Valuation, ";
	for (int j = 0; j < maxCompanies; j++) //Save valuation
	{
		outDataStream << companies[j].valuation << ",";
	}
	outDataStream << "\n";

	outDataStream << "President, ";
	for (int j = 0; j < maxCompanies; j++) //Save valuation
	{
		outDataStream << companies[j].president << ",";
	}
	outDataStream << "\n";
	outDataStream << "Shares, ";
	for (int j = 0; j < maxCompanies; j++) // Save Company shares
	{
		outDataStream << companies[j].shares << ",";
	}
	outDataStream << "\n";
	outDataStream << "Orphans, ";
	for (int j = 0; j < maxCompanies; j++) // Sace Company orphans
	{
		outDataStream << companies[j].orphans << ",";
	}
	outDataStream << "\n";
	outDataStream << "Started, ";
	for (int j = 0; j < maxCompanies; j++) // SAve company started
	{
		outDataStream << companies[j].started << ",";
	}
	outDataStream << "\n";
	outDataStream << "TurnOrder, ";
	for (int j = 0; j < maxCompanies; j++) // Save company turnorder
	{
		outDataStream << companies[j].order << ",";
	}
	outDataStream << "\n";
	for (int m = 0; m < maxCities; m++) // Save cities
	{
		outDataStream << cities.name[m] << ",";
		for (int j = 0; j < maxCompanies; j++)
		{
			outDataStream << companies[j].cities[m] << ",";
		}
		outDataStream << "\n";
	}
	for (int k = 0; k < maxTrains; k++) // Save trains
	{
		outDataStream << k + 1 << ",";
		for (int j = 0; j < maxCompanies; j++)
		{
			outDataStream << companies[j].trains[k] << ",";
		}
		outDataStream << "\n";
	}
	outDataStream.close();
	return(currentTechlevel);
}

