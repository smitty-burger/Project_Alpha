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

using namespace std;
// Function Prototypes
void update_console(vector< vector<int> > pvalue, int arms, vector<int> bank, int pulln);
		//When called, display an updated main screen

//===========================================================================					MAIN
int main()
{
	int arms;
	cout << "How many arms do you want your Bandit to have?" << endl;
	cin >> arms;

	//insert value checker here
	//
	//
	//

	// Create arms value vectors
	vector< vector<int> > pvalue;
	// Initialize Previous Payout to 0
	vector<int> last;
	for (int i = 0; i < arms; i++)
	{
		last.push_back(0);
	}
	pvalue.push_back(last);

	// Create Bank
	vector<int> bank;
	bank.push_back(1000);

	// Initialize pull number
	int pulln = 0;

	// Update Console
	update_console(pvalue, arms, bank, pulln);
	return (0);
}

//===========================================================================					update_console
/*	||	Brief	||
Creats an updated version of the screen. All arms will be displayed along
with their most recent pull value. The number of pulls and the amount earned or
lost will be displayed.
*/
void update_console(vector< vector<int> > pvalue, int arms, vector<int> bank, int pulln)
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
		cout << "$" << pvalue[0][i] << "\t";
	}

	//Keep a running tab of pulls and the amount earned or lost
	cout << "\n\n\n\tBank\t$" << bank[0] << endl;
	cout << "\n\tPull #\t" << pulln << "\n\n\n" << endl;
}
