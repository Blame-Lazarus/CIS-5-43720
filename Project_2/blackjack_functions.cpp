#include "blackjack.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <string>
#include <ctime>
#include <vector>

using namespace std;

int getCard() {
    static int callCnt = 0;  //Static variable
    callCnt++;
    //The following line can be activated to check the number of times the getCard function is called
    //cout << "getCard has been called " << callCount << " times." << endl;
    int card = rand() % 13 + 1;
    if (card > 10) card = 10;  //Jack, Queen and King are valued as 10
    if (card == 1) card = 11;  //Aces are initially valued at 11
    return card;
}

void showBal(float balance) {
    cout << "Balance: $" << fixed << setprecision(2) << balance << endl;
}

//Saving the results in a log
void logRslt(ofstream &log, float inBal, float fnalBal, const string &result) {
    log << "Initial balance: $" << fixed << setprecision(2) << inBal << endl;
    log << "Final balance: $" << fixed << setprecision(2) << fnalBal << endl;
    log << "Result: " << result << endl;
}

void plceBet(float &balance, float &bet, float minBet) {
    showBal(balance);
    cout << "Place bet (minimum $5.00): ";
    cin >> bet;
    while (bet < 5.0 || bet > balance) {
        cout << "Invalid bet. Please place a bet of at least $" << fixed << setprecision(2) << minBet 
             << " and not more than your balance: ";
        cin >> bet;
    }
    balance -= round(bet * 100) / 100;     //Deduct bet from balance, rounding to the nearest cent
}

bool checkBJ(const vector<int> &hand) {
    return handVal(hand) == 21;
}

//The following function checks for the value of the ace card, if the handVal exceeds 21, ace value is changed from 11 to 1
int handVal(const vector<int> &hand) {
    int value = 0;
    int aceCnt = 0;
    for (int card : hand) {
        value += card;
        if (card == 11) aceCnt++;
    }
    while (value > 21 && aceCnt > 0) {
        value -= 10;
        aceCnt--;
    }
    return value;
}

void plyrTrn(vector<int> &pHand, int &pScore, float &balance, float &bet, bool &plyrBst) {
    bool plyrTrn = true;
    bool doubled = false;
    while (plyrTrn) {
        cout << "Total: " << pScore << endl;
        if (pScore > 21) {
            cout << "Player busts! Total: " << pScore << endl;
            plyrBst = true;
            break;
        }

        cout << "Hit, stand, or double? (h/s/d): ";
        char choice;
        cin >> choice;

        //Hit
        if (choice == 'h') {
            int card = getCard();
            pHand.push_back(card);
            pScore = handVal(pHand);
            cout << "Dealt card: " << card << " Total: " << pScore << endl;
        }

        //Stand 
        else if (choice == 's') {
            plyrTrn = false;
        } 
        
        //Double
        else if (choice == 'd') {
            bool canDble = false;
            int aceCount = 0;

            for (int card : pHand) {
                if (card == 11) {
                    aceCount++;
                }
            }
            if ((pScore == 9 || pScore == 10 || pScore == 11) && aceCount == 0) {
                canDble = true;
            } 
            else if ((pScore == 16, pScore == 17 || pScore == 18) && aceCount > 0) {
                canDble = true;
            }

            if (canDble && balance >= bet) {
                balance -= round(bet * 100) / 100;  //Deduct the double bet
                bet *= 2;
                int card = getCard();
                pHand.push_back(card);
                pScore = handVal(pHand);
                cout << "Dealt card: " << card << " Total: " << pScore << endl;
                doubled = true;
                plyrTrn = false;
            } 
            else {
                if (!canDble) {
                    cout << "You can only double with a total of 9, 10, or 11 without an ace, or 16, 17, or 18 with an ace." << endl;
                } 
                else {
                    cout << "Not enough balance to double. Choose hit or stand." << endl;
                }
            }
        } 
        else {
            cout << "Invalid choice, try again." << endl;
        }
    }
}

//After player turn ends
void housTrn(vector<int> &hHand, int &hScore) {
    hScore = handVal(hHand);
    cout << "House reveals second card: " << hHand[1] << " Total: " << hScore << endl;
    while (hScore < 17) {
        int card = getCard();
        hHand.push_back(card);
        hScore = handVal(hHand);
        cout << "House dealt card: " << card << " Total: " << hScore << endl;
    }
}

//Calculate and display results
void hndlRes(float &balance, float bet, int pScore, int hScore, bool plyrBst, ofstream &log, float inBal) {
    //If the player busts
    if (plyrBst) {
        cout << "House wins! New Balance: $" << fixed << setprecision(2) << balance << endl;
        logRslt(log, inBal, balance, "Player busts");
    } 
    //If the house busts
    else if (hScore > 21) {
        cout << "House busts! Player wins!" << endl;
        balance += bet * 2;
        logRslt(log, inBal, balance, "House busts, player wins");
    }
    //If the house has a higher value hand 
    else if (hScore >= pScore) {
        cout << "House wins! Total: " << hScore << endl;
        logRslt(log, inBal, balance, "House wins");
    } 
    //If the player has a higher value hand
    else {
        cout << "Player wins! Total: " << pScore << endl;
        balance += bet * 2;
        logRslt(log, inBal, balance, "Player wins");
    }
}

//Display history after the game
void disHist(int gmeHtry[], int gameCnt) {
    cout << "Game History: " << endl;
    for (int i = 0; i < gameCnt; i++) {
        cout << "Game " << i + 1 << ": ";
        if (gmeHtry[i] == 1) {
            cout << "Player wins" << endl;
        } else if (gmeHtry[i] == -1) {
            cout << "House wins" << endl;
        } else {
            cout << "Tie" << endl;
        }
    }
}

//bubble sort
void bubSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

//Selection sort
void selSort(vector<int> &vec) {
    int n = vec.size();
    for (int i = 0; i < n - 1; i++) {
        int minIndx = i;
        for (int j = i + 1; j < n; j++) {
            if (vec[j] < vec[minIndx]) {
                minIndx = j;
            }
        }
        if (minIndx != i) {
            int temp = vec[i];
            vec[i] = vec[minIndx];
            vec[minIndx] = temp;
        }
    }
}

//Linear search
int lnrSrch(const int arr[], int n, int key) {
    for (int i = 0; i < n; i++) {
        if (arr[i] == key) {
            return i;
        }
    }
    return -1;
}