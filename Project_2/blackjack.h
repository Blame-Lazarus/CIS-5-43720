/* 
 * File:   blackjack.h
 * Author: Luis Y Vazquez Quiroz
 * Created on June 3, 2024, 4:59 PM
 */

#ifndef BLACKJACK_H
#define BLACKJACK_H

#include <vector>
#include <fstream>
#include <iomanip>

using namespace std;

// Function Prototypes
int getCard();
void disBlce(float balance);
void logRslt(ofstream &log, float inBal, float fnalBal, const string &result);
void plceBet(float &balance, float &bet, float minBet = 5.0);   // Default argument
bool checkBJ(const vector<int> &hand);
int handVal(const vector<int> &hand);
void plyrTrn(vector<int> &pHand, int &pScore, float &balance, float &bet, bool &plyrBst);
void housTrn(vector<int> &hHand, int &hScore);
void hndlRes(float &balance, float bet, int pScore, int hScore, bool plyrBst, ofstream &log, float inBal);
void disHist(int gmeHtry[], int gameCnt);
void bubSort(int arr[], int n);
void selSort(vector<int> &vec);
int lnrSrch(const int arr[], int n, int key);

#endif // BLACKJACK_H