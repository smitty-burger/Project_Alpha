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
//				2/12/17		Tyler Burger		Q=Learner added
//				2/14/17		Tyler Burger		Added TestA_Function
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
#include <algorithm>
#include <numeric>
#include <functional>

using namespace std;

// Function Prototypes
void update_console(deque<double> pvalue, int arms, deque<double> bank, int pulln);
		//When called, display an updated main screen
void random_or_assigned(vector<double> &mean, vector<double> &stddev, int arms);
		//When called, get or assign mean and stddev for each arm
int user_pull_arm(vector<double> &mean, vector<double> &stddev, int arms, deque<double> &pvalue, int &pulln, deque<double> &bank);
		//simulate an arm pull and give an output value for the pull
int UQ_prompt(double &epsilon, double &alpha, int &life_cycle);
		//prompt user for values associated with q-learner program
int Q_pull_arm(vector<double> &mean, vector<double> &stddev, int arms, deque<double> &pvalue, int &pulln, deque<double> &bank, double epsilon, double alpha, int life_cycle, deque<double> &history);
		//simulate arm pulls with a q-learner
void TestA_Function(vector<double> &mean, vector<double> &stddev, int arms);
		//run self test A

//===========================================================================					MAIN
int main()
{
	cout << fixed << showpoint << setprecision(2);

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

	// User play or Q-Learner play?
	int UQ_bool;
	double epsilon;
	double alpha;
	int life_cycle;
	UQ_bool = UQ_prompt(epsilon, alpha, life_cycle);

	// Pause the console screen and wait until user input
	system("PAUSE");

	// Create arms value vectors and history
	deque<double> pvalue;
	deque<double> history;
	// Initialize Previous Payout to 0
	for (int i = 0; i < arms; i++)
	{
		history.push_back(0.0);
		pvalue.push_back(0.0);
	}

	// Create Bank
	deque<double> bank;
	bank.push_back(1000);

	// Initialize pull number
	int pulln = 0;
	int choice;
	int cycles = 0;

	if (UQ_bool == 0)
	{
		do
		{
			// Update Console
			update_console(pvalue, arms, bank, pulln);

			// User Pull
			choice = user_pull_arm(mean, stddev, arms, pvalue, pulln, bank);

		} while (choice != -1);
	}
	else if (UQ_bool == 1)
	{
		do
		{
			// Update Console
			update_console(pvalue, arms, bank, pulln);

			// Q-Learner Pull
			choice = Q_pull_arm(mean, stddev, arms, pvalue, pulln, bank, epsilon, alpha, life_cycle, history);

		} while (choice != -1);
	}
	else if (UQ_bool == -1)
	{
		// Run Self test A
		TestA_Function(mean, stddev, arms);
	}

	return (0);
}

//===========================================================================					update_console
void update_console(deque<double> pvalue, int arms, deque<double> bank, int pulln)
{
	// Clear Console Screen
	system("CLS");

	//Display Arm Number
	cout << "Arm Number";
	int i = 0;
	while (i < arms)
	{
		cout << "\t" << i << "\t";
		i++;
	}

	//Display separation line
	cout << "\n\t\t";
	for (int i = 0; i < arms; i++)
	{
		cout << "===============";
	}

	//Display most recent payout for each arm
	cout << "\nPrev. Payout\t";
	for (int i = 0; i < arms; i++)
	{
		cout << "$" << pvalue[i] << "\t\t";
	}

	//Keep a running tab of pulls and the amount earned or lost
	cout << "\n\n\n\tBank\t$" << bank[0] << endl;
	cout << "\n\tPull #\t" << pulln << "\n\n\n" << endl;
}

//===========================================================================					random_or_assigned
void random_or_assigned(vector<double> &mean, vector<double> &stddev, int arms)
{
	char letter;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::normal_distribution<double> distm(0.0, 15.0);
	std::normal_distribution<double> distd(0.0, 30.0);

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

			double temp;
			do
			{
				temp = distd(gen);
			} while (temp <= 0);
			stddev.push_back(temp);
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
}

//===========================================================================					user_pull_arm
int user_pull_arm(vector<double> &mean, vector<double> &stddev, int arms, deque<double> &pvalue, int &pulln, deque<double> &bank)
{
	cout << "Which arm do you want to pull? (enter -1 to quit)" << endl;
	int choice;
	cin >> choice;
	
	std::random_device rd;
	std::mt19937 gen(rd());

	while (choice < -1 || choice >= arms)
	{
		if (choice < -1)
		{
			cout << "\t\t    ***" << choice << " is not a valid arm number***\n\n " <<
				"Please Pick a new arm number. (enter -1 to quit)\n" << endl;
			cin >> choice;
		}
		else if (choice >= arms)
		{
			cout << "\t\t    ***" << choice << " is not a valid arm number***\n\n " <<
				"Please Pick a new arm number. (enter -1 to quit)\n" << endl;
			cin >> choice;
		}
	}

	if (choice > -1)
	{
		
		std::normal_distribution<double> distm(mean[choice], stddev[choice]);

		pvalue[choice] = (distm(gen));
		pulln++;
		bank.push_front(bank[0] + pvalue[choice]);
	}

	return choice;
}

//===========================================================================					UQ_prompt
int UQ_prompt(double &epsilon, double &alpha, int &life_cycle)
{
	int UQ_bool;
	char letter;

	cout << "\n\n\tUser Play or Action-Value-Learner? (U/q)	[T for Test A]" << endl;
	cin >> letter;

	int i;
	switch (letter)
	{
		//If U(u) then let the user play
	case 'U':
	case 'u':
		UQ_bool = 0;
		epsilon = -1;
		alpha = -1;
		life_cycle = -1;

		break;

		//If q(Q) then assign user defined variables
	case 'Q':
	case 'q':
		UQ_bool = 1;
		cout << "\n\nPlease pick an exploration ratio (epsilon) (0 <= E <= 1)" << endl;
		cin >> epsilon;
		while (epsilon < 0 || epsilon > 1)
		{
			if (epsilon < 0)
			{
				cout << "\t\t    ***" << epsilon << " is not a valid value***\n\n " <<
					"Please pick a new value (0 <= E <= 1)" << endl;
				cin >> epsilon;
			}
			else if (epsilon > 1)
			{
				cout << "\t\t    ***" << epsilon << " is not a valid value***\n\n " <<
					"Please pick a new value (0 <= E <= 1)" << endl;
				cin >> epsilon;
			}
		}

		cout << "\n\nPlease pick a learning ratio (alpha) (0 <= A <= 1)" << endl;
		cin >> alpha;
		while (alpha < 0 || alpha > 1)
		{
			if (alpha < 0)
			{
				cout << "\t\t    ***" << alpha << " is not a valid value***\n\n " <<
					"Please pick a new value (0 <= A <= 1)" << endl;
				cin >> alpha;
			}
			else if (alpha > 1)
			{
				cout << "\t\t    ***" << alpha << " is not a valid value***\n\n " <<
					"Please pick a new value (0 <= A <= 1)" << endl;
				cin >> alpha;
			}
		}

		cout << "\n\nPlease choose a number of cycles for the Q-Learner to iterate" << endl;
		cin >> life_cycle;
		while (life_cycle < 0)
		{
			cout << "\t\t    ***" << life_cycle << " is not a valid value***\n\n " <<
				"Value must be a positive integer" << endl;
			cin >> life_cycle;
		}

		break;
	default:
		UQ_bool = -1;
		break;

	}
	return UQ_bool;
}

//===========================================================================					Q_pull_arm
int Q_pull_arm(vector<double> &mean, vector<double> &stddev, int arms, deque<double> &pvalue, int &pulln, deque<double> &bank, double epsilon, double alpha, int life_cycle, deque<double> &history)
{
	int random_arm;
	int exploit;
	int choice;
	int best;
	int i = 0;

	std::random_device rd;
	std::mt19937 gen(rd());

	//Find the best arm based on history
	auto biggest = max_element(begin(history), end(history));
	best = distance(begin(history), biggest);

	//Randomly pick an arm to pull, and if the next pull should be exploratory
	//Get System Time
	unsigned seed = time(0);
	//Seed Random Number Generator
	srand(seed);
	//Pick arm
	random_arm = rand() % arms;
	//Exploit?
	double exp_thresh;
	exp_thresh = epsilon * 10000;
	exploit = rand() % 10000;
	if (exploit <= exp_thresh)
	{
		choice = random_arm;
	}
	else
	{
		choice = best;
	}

	//Terminate program at the end o fthe last cycle
	if (life_cycle == pulln)
	{
		choice = -1;
	}
	//Pull selected arm and update history and payout value
	if (choice > -1)
	{

		std::normal_distribution<double> distm(mean[choice], stddev[choice]);
		pvalue[choice] = (distm(gen));
		history[choice] = pvalue[choice] * alpha + history[choice] * (1 - alpha);
		pulln++;
		bank.push_front(bank[0] + pvalue[choice]);
	}
	return choice;
}

//===========================================================================					TestA_Function
void TestA_Function(vector<double> &mean, vector<double> &stddev, int arms)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	deque<double> temp;
	double u;
	double t;
	int i;
	int j;
	int life_cycle = 150000;
	double tol = .1;

	system("CLS");
	cout << "\t     ***Running Self Test A***\n" << endl;
	//cout << "Cycles Run: " << life_cycle << "\n" << endl;

	for (i = 0; i < arms; i++)
	{
		cout << "Arm: " << i << endl;
		std::normal_distribution<double> distm(mean[i], stddev[i]);
		for (j = 0; j < life_cycle; j++)
		{
			t = (distm(gen));
			temp.push_back(t);
		}
		t = 0;
		for (j = 0; j < life_cycle; j++)
		{
			t = t + temp[j];
		}
		u = t / life_cycle;
		cout << "Mean:  \t" << mean[i] << "\t\t" << "Calculated Mean:     \t"<< u << endl;
		assert(abs(mean[i] + mean[i] * tol) > abs(u));
		assert(abs(u) > abs(mean[i] - mean[i] * tol));

		t = 0;
		for (j = 0; j < life_cycle; j++)
		{
			t = t + (temp[j] - u) * (temp[j] - u);
		}
		t = sqrt(t / (life_cycle - 1));
		cout << "StdDev:\t" << stddev[i] << "\t\t" << "Calculated StdDev: \t" << t << "\n\n" << endl;
		assert(stddev[i] + stddev[i] * tol > t);
		assert(t > stddev[i] - stddev[i] * tol);
		temp.clear();
		

	}
}