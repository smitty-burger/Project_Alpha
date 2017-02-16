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

#define NDEBUG
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
void TestB_Function(deque<double> pull_count, vector<double> mean, vector<double> stddev, int arms, int pulln);
		//run self test B
void Save_to(deque<double> big_bank, deque<double> big_pull0, deque<double> big_pull1, deque<double> big_pull2, deque<double> big_pull3, deque<double> big_pull4);

//===========================================================================					MAIN
int main()
{
	////For big iterative testing
	deque<double> big_bank; 
	big_bank.assign(1000, 0);
	deque<double> mini_bank(1000);
	deque<double> big_pull0(1000);
	deque<double> big_pull1(1000);
	deque<double> big_pull2(1000);
	deque<double> big_pull3(1000);
	deque<double> big_pull4(1000);
	int arms = 5;
	vector<double> mean;
	mean.push_back(0);
	mean.push_back(-2);
	mean.push_back(-1);
	mean.push_back(2);
	mean.push_back(1);
	vector < double> stddev;
	stddev.push_back(.7);
	stddev.push_back(5);
	stddev.push_back(2);
	stddev.push_back(.3);
	stddev.push_back(.5);
	int UQ_bool = 1;
	double epsilon = .2;
	double alpha = .5;
	int life_cycle = 1000;
	////


	cout << fixed << showpoint << setprecision(2);

	for (int ijk = 0; ijk < 30; ijk++)
	{
		////
		//// Get number of arms from user
		//int arms;
		//cout << "How many arms do you want your Bandit to have?" << endl;
		//cin >> arms;

		//// Check choice
		//while (arms < 0)
		//{
		//	cout << "\t\t    ***" << arms << " is not a valid arm number***\n\n\n\n\n " <<
		//		"Please Pick a new number of arms\n\n\n\n\n\n" << endl;
		//	cin >> arms;
		//}

		//// Get or assign mean and stddev for each arm
		//vector<double> mean;
		//vector < double> stddev;
		//random_or_assigned(mean, stddev, arms);

		//// User play or Q-Learner play?
		//int UQ_bool;
		//double epsilon;
		//double alpha;
		//int life_cycle;
		//UQ_bool = UQ_prompt(epsilon, alpha, life_cycle);

		//// Pause the console screen and wait until user input
		//system("PAUSE");
		////

		// Create arms value vectors and history
		deque<double> pvalue;
		deque<double> history;
		deque<double> pull_count;
		// Initialize Previous Payout to 0
		for (int i = 0; i < arms; i++)
		{
			history.push_back(0.0);
			pvalue.push_back(0.0);
			pull_count.push_back(0.0);
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
				//update_console(pvalue, arms, bank, pulln);

				// Q-Learner Pull
				choice = Q_pull_arm(mean, stddev, arms, pvalue, pulln, bank, epsilon, alpha, life_cycle, history);

				// Keep count of how many times each arm gets pulled
				if (choice != -1 && life_cycle > 299)
				{
					pull_count[choice] = pull_count[choice] + 1;
				}

				////
				if (choice != -1)
				{
					big_bank.at(pulln - 1) = big_bank.at(pulln - 1) + pvalue[choice];
					cout << big_bank.at(pulln - 1) << endl;// = big_bank[pulln] + pvalue[choice];

				
				switch (choice)
				{
				case 0:
					big_pull0.at(pulln - 1) = big_pull0.at(pulln - 1) + 1;
					break;

				case 1:
					big_pull1.at(pulln - 1) = big_pull1.at(pulln - 1) + 1;
					break;

				case 2:
					big_pull2.at(pulln - 1) = big_pull2.at(pulln - 1) + 1;
					break;

				case 3:
					big_pull3.at(pulln - 1) = big_pull3.at(pulln - 1) + 1;
					break;

				case 4:
					big_pull4.at(pulln - 1) = big_pull4.at(pulln - 1) + 1;
					break;
				}
				}

				////

			} while (choice != -1);
		}
		else if (UQ_bool == -1)
		{
			// Run Self test A
			TestA_Function(mean, stddev, arms);
		}

		// Send variables to TestB_Function for test for Pass/Fail grading
		if (arms < 7 && life_cycle > 299)
		{
			TestB_Function(pull_count, mean, stddev, arms, pulln);
		}
	}
	Save_to(big_bank, big_pull0, big_pull1, big_pull2, big_pull3, big_pull4);

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
	//Initialize random device
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

	//Notify user when all arm variables have been asigned
	cout << "\nAll arm variables have been assigned." << endl;
}

//===========================================================================					user_pull_arm
int user_pull_arm(vector<double> &mean, vector<double> &stddev, int arms, deque<double> &pvalue, int &pulln, deque<double> &bank)
{
	//Prompt user
	cout << "Which arm do you want to pull? (enter -1 to quit)" << endl;
	int choice;
	cin >> choice;
	
	//Initialize random device
	std::random_device rd;
	std::mt19937 gen(rd());

	//Loop to make sure the user enters an appropriate arm number, -1 will terminate program
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

	// Perform this to create a new "payout" from the called arm, skip if -1 is entered
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

	//Prompt User
	cout << "\n\n\tUser Play or Action-Value-Learner(A.V.L.)? (U/q)	[T for Test A]" << endl;
	cin >> letter;

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

		cout << "\n\nPlease choose a number of cycles for the A.V.L. to iterate" << endl;
		cin >> life_cycle;
		while (life_cycle < 0)
		{
			cout << "\t\t    ***" << life_cycle << " is not a valid value***\n\n " <<
				"Value must be a positive integer" << endl;
			cin >> life_cycle;
		}

		break;

		//If another letter is picked then start self test A
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
	//Pick arm
	random_arm = gen() % arms;
	//Exploit?
	double exp_thresh;
	exp_thresh = epsilon * 10000;
	exploit = gen() % 10000;
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
	//Initialize random number generator
	std::random_device rd;
	std::mt19937 gen(rd());

	//Variables
	deque<double> temp;
	double u;
	double t;
	int i;
	int j;
	int life_cycle = 150000;
	double tol = .1;

	//Clear screen and prompt user that Self Test A is now running
	system("CLS");
	cout << "\t     ***Running Self Test A***\n" << endl;
	//cout << "Cycles Run: " << life_cycle << "\n" << endl;

	//Run test for each arm
	for (i = 0; i < arms; i++)
	{
		//Display arm number
		cout << "Arm: " << i << endl;

		//Initialize distribution for arm i
		std::normal_distribution<double> distm(mean[i], stddev[i]);

		//Create temporary deque of statistical values
		for (j = 0; j < life_cycle; j++)
		{
			t = (distm(gen));
			temp.push_back(t);
		}
		t = 0;

		//Sum all of the statistical values
		for (j = 0; j < life_cycle; j++)
		{
			t = t + temp[j];
		}

		//Compute mean of data set
		u = t / life_cycle;

		//Compare mean values
		cout << "Mean:  \t" << mean[i] << "\t\t" << "Calculated Mean:     \t"<< u << endl;

		//Check for 10% error in mean
		assert(abs(mean[i] + mean[i] * tol) > abs(u));
		assert(abs(u) > abs(mean[i] - mean[i] * tol));

		//Find sum of the squares for standard deviation
		t = 0;
		for (j = 0; j < life_cycle; j++)
		{
			t = t + (temp[j] - u) * (temp[j] - u);
		}

		//Compute standard deviation
		t = sqrt(t / (life_cycle - 1));

		//Compare StdDev values
		cout << "StdDev:\t" << stddev[i] << "\t\t" << "Calculated StdDev: \t" << t << "\n\n" << endl;

		//Check for 10% error is StdDev
		assert(stddev[i] + stddev[i] * tol > t);
		assert(t > stddev[i] - stddev[i] * tol);

		//Clear deque to save memory, deque is reused for each iteration
		temp.clear();
		

	}
}

//===========================================================================					TestB_Function
void TestB_Function(deque<double> pull_count, vector<double> mean, vector<double> stddev, int arms, int pulln)
{
	int high_count;
	deque<double> all_arm;
	int best_arm;

	auto biggest = max_element(begin(pull_count), end(pull_count));
	high_count = distance(begin(pull_count), biggest);

	for (int i = 0; i < arms; i++)
	{
		all_arm.push_back(((mean[i] + stddev[i]) / (stddev[i] * 2)) * 100);
	}

	auto biggins = max_element(begin(all_arm), end(all_arm));
	best_arm = distance(begin(all_arm), biggins);

	assert(best_arm == high_count);
	
	cout << "\n\nA.V.L. has picked " << best_arm << " as the best arm!\n\n" << endl;

}

//===========================================================================					Save_to
void Save_to(deque<double> big_bank, deque<double> big_pull0, deque<double> big_pull1, deque<double> big_pull2, deque<double> big_pull3, deque<double> big_pull4)
{

	//Create output file
	ofstream output_file;
	output_file.open("MAB_Output");
	output_file << fixed << showpoint << setprecision(2);

	//User console update
	cout << "Now Writing Data To File\n\n\n" << endl;

	//Write to file
	for (int i = 0; i < 1000; i++)
	{
		output_file << big_bank[i] << '\t' << big_pull0[i] << '\t' << big_pull1[i] << '\t'
			<< big_pull2[i] << '\t' << big_pull3[i] << '\t' << big_pull4[i] << endl;
	}

	//Close output file
	output_file.close();

	//User console update
	cout << "Data Has Been Written To File\n\n\n" << endl;
}