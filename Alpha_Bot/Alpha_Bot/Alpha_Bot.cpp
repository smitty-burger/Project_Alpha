//===========================================================================
//Alpha_Bot.cpp
//This program is designed to replicate a multi-armed-bandit problem. The multi-
//arm-bandit can have any number of arms as defined by the user. The different
//arms can either have a random mean and standard distribution or a user defined
//mean and standard distribution. The user can then either choose to manually 
//each of the arms or have a Q-Learner program pick the arms in order to maximize
//the payout (or minimize loss).

//ME493 Multi-Armed-Bandit
//History:	    Date		Resp. Eng.			Modification
//              _______		_________________	_____________
//				2/9/17		Tyler Burger		initial 
//
//Input:
//	N/A
//			
//Output:
//  N/A
//
//===========================================================================

/*
DEBUG HELPER

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
int chkpnt = 0;
cout << "checkpoint " << chkpnt << endl;
chkpnt++;
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
cout << "checkpoint " << chkpnt << endl;
chkpnt++;
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

*/


#include "stdafx.h"
#include <conio.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <cassert>
#include <random>
#include <vector>
#include <iomanip>
#include <deque>

using namespace std;

// Function Prototypes
void update_console(deque<double> pvalue, int arms, deque<double> bank, int pulln);
		//When called, display an updated main screen
void random_or_assigned(vector<double> &mean, vector<double> &stddev, int arms);
		//When called, get or assign mean and stddev for each arm
int user_pull_arm(vector<double> &mean, vector<double> &stddev, int arms, deque<double> &pvalue, int &pulln, deque<double> &bank);
		//simulate an arm pull and give an output value for the pull
//===========================================================================					MAIN
int main()
{
	// Get number of arms from user
	int arms;
	cout << "How many arms do you want your Bandit to have?" << endl;
	cin >> arms;

	// Check choice
	while (arms < 0)
	{
		cout << "\t\t    ***" << arms << " is not a valid arm number***\n\n\n\n\n " <<
			"Please Pick a new number of arms\n\n\n\n\n\n" << endl;
		cin >> arms;
	}

	// Get or assign mean and stddev for each arm
	vector<double> mean;
	vector < double> stddev;
	random_or_assigned(mean, stddev, arms);

	// Create arms value vectors
	deque<double> pvalue;
	// Initialize Previous Payout to 0
	for (int i = 0; i < arms; i++)
	{
		pvalue.push_back(0.0);
	}

	// Create Bank
	deque<double> bank;
	bank.push_back(1000);

	// Initialize pull number
	int pulln = 0;
	int choice;

	do
	{
		// Update Console
		update_console(pvalue, arms, bank, pulln);

		// User Pull
		choice = user_pull_arm(mean, stddev, arms, pvalue, pulln, bank);

	} while (choice != -1);
	

	return (0);
}

//===========================================================================					update_console
/*	||	Brief	||
Creats an updated version of the screen. All arms will be displayed along
with their most recent pull value. The number of pulls and the amount earned or
lost will be displayed.
*/
void update_console(deque<double> pvalue, int arms, deque<double> bank, int pulln)
{
	// Clear Console Screen
	system("CLS");

	//Display Arm Number
	cout << "Arm Number";
	int i = 0;
	while (i < arms)
	{
		cout << "\t" << i;
		i++;
	}

	//Display separation line
	cout << "\n\t\t";
	for (int i = 0; i < arms; i++)
	{
		cout << "========";
	}

	//Display most recent payout for each arm
	cout << "\nPrev. Payout\t";
	for (int i = 0; i < arms; i++)
	{
		cout << "$" << pvalue[i] << "\t";
	}

	//Keep a running tab of pulls and the amount earned or lost
	cout << "\n\n\n\tBank\t$" << bank[0] << endl;
	cout << "\n\tPull #\t" << pulln << "\n\n\n" << endl;
}
//===========================================================================					random_or_assigned
/*	||	Brief	||
Prompt user for a random option for the mean and standard distribution. If random 
is picked, randomly asign variables. If user chooses to enter it in, prompt for
each variable.
*/
void random_or_assigned(vector<double> &mean, vector<double> &stddev, int arms)
{
	char letter;
	std::default_random_engine gen;
	std::normal_distribution<double> distm(0.0, 10.0);
	std::normal_distribution<double> distd(0.0, 25.0);

	// Prompt user for random distribution or arm specific distribution
	cout << "\t\tRandom StdDev and Mean? (Y/n)" << endl;
	cin >> letter;

	int i;
	switch (letter)
	{
		//If Y(y) then assign random variables
	case 'Y':
	case 'y':
		for ( i = 0; i < arms; i++)
		{
			mean.push_back(distm(gen));
			stddev.push_back(distd(gen));
		}
		break;

		//If n(N) then prompt user for values
	case'N':
	case'n':
		int t;
		for ( i = 0; i < arms; i++)
		{
			cout << "Mean for arm " << i << endl;
			cin >> t;
			mean.push_back(t);
			cout << "StdDev for arm " << i << endl;
			cin >> t;
			stddev.push_back(t);
		}
		break;
	}
	cout << "All arm variables have been assigned." << endl;
	system("PAUSE");
}
//===========================================================================					user_pull_arm
int user_pull_arm(vector<double> &mean, vector<double> &stddev, int arms, deque<double> &pvalue, int &pulln, deque<double> &bank)
{
	cout << "Which arm do you want to pull? (enter -1 to quit)" << endl;
	int choice;
	cin >> choice;
	
	while (choice < -1 || choice > arms)
	{
		if (choice < -1)
		{
			cout << "\t\t    ***" << choice << " is not a valid arm number***\n\n " <<
				"Please Pick a new arm number. (enter -1 to quit)\n" << endl;
			cin >> choice;
		}
		else if (choice > arms)
		{
			cout << "\t\t    ***" << choice << " is not a valid arm number***\n\n " <<
				"Please Pick a new arm number. (enter -1 to quit)\n" << endl;
			cin >> choice;
		}
	}

	if (choice > -1)
	{
		std::default_random_engine gen;
		std::normal_distribution<double> distm(mean[choice], stddev[choice]);

		pvalue[choice] = (distm(gen));
		pulln++;
		bank.push_front(bank[0] + pvalue[choice]);
	}

	return choice;
}