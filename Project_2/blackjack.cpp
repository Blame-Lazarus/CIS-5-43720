/* 
 * Author: Luis Y Vazquez Quiroz
 * Created on: 05/10/2024
 * Purpose: Blackjack Game Project
 */

//System Libraries
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>

using namespace std;

//User Libraries
#include "blackjack.h"

//Global Constants Only!

//Execution Begins Here!
int main(int argc, char** argv) {
    //Set a Random number seed here.
    srand(static_cast<unsigned int>(time(0)));
    
    //File use
    ofstream log("game_log.txt", ios::app); //This line creates or opens a file to save game results.
    
    //Introduction
    string welcome = "Welcome to the Blackjack Game!";
    cout << welcome << endl;
    cout << "Rules: Get a higher hand value than the dealer without going over 21. You are dealt two cards and can then" << endl
         << "choose to “hit” (receive additional cards) or “stand” (keep your current hand). The dealer also receives" << endl
         << "two cards, but only one is face up. If your hand exceeds 21, you “bust” and lose the game. If the dealer" << endl
         << "busts, all remaining you win. If neither busts, the one with the highest hand value wins. GOOD LUCK!" << endl;

    
    //Set up bank
    float balance = 100.0;                //Initial balance for bets
    float inBal = balance;                //Starting balance tracking
    bool playing = true;                  //Boolean to check the status of the game

    //Arrays for game history and hands
    int gmeHtry[100] = {0};               //Single-dimensional array
    int crdHtry[100][2] = {0};            //Two-dimensional array
    int gameCnt = 0;                      //Number of games played
    
    while (playing) {
        float bet;                        //Bet size
        plceBet(balance, bet);            //Placing bet function
        
        //Deal two cards to player
        vector<int> pHand = { getCard(), getCard() };       //Cards are in a vector for dinamyc memory allocation
        selSort(pHand);                                     //Selection sort to sort the cards
        int pScore = handVal(pHand);                        //Total value of the player hand
        cout << "Player dealt cards: " << pHand[0] << " " << pHand[1] << " Total: " << pScore << endl;
        
        //Deal two cards to house
        vector<int> hHand = { getCard(), getCard() };                       //Same as player
        int hScore = hHand[0];                                              //Same as player
        cout << "House dealt card: " << hHand[0] << " [Hidden]" << endl;    //Only one card is shown, the other is hidden

        //Log initial hands into card history
        crdHtry[gameCnt][0] = pScore;
        crdHtry[gameCnt][1] = hScore;

        //Check if player has a blackjack
        if (checkBJ(pHand)) {
            cout << "Player has Blackjack!" << endl;
            //If dealer also has blackjack, it's a tie
            if (checkBJ(hHand)) {
                cout << "House also has Blackjack!" << endl;
                balance += bet;
                cout << "It's a tie! New Balance: $" << fixed << setprecision(2) << balance << endl;
                logRslt(log, inBal, balance, "Tie");
                gmeHtry[gameCnt] = 0;
            } 
            else {
            //Player wins with blackjack
            balance += 1.5 * bet;  //Blackjack pays 1.5x
            cout << "Player wins with Blackjack! New Balance: $" << fixed << setprecision(2) << balance << endl;
            logRslt(log, inBal, balance, "Player wins with Blackjack");
            gmeHtry[gameCnt] = 1;
            }
            gameCnt++;
            continue;
            }

        //Check if house has a blackjack
        if (checkBJ(hHand)) {
            cout << "House has Blackjack!" << endl;
            cout << "House wins! New Balance: $" << fixed << setprecision(2) << balance << endl;
            logRslt(log, inBal, balance, "House wins with Blackjack");
            gmeHtry[gameCnt] = -1;
            gameCnt++;
            continue;
        }
        
        //Player's turn
        bool plyrBst = false;
        plyrTrn(pHand, pScore, balance, bet, plyrBst);
        
        //House's turn
        if (!plyrBst) {
            housTrn(hHand, hScore);
        }
        
        //game result
        hndlRes(balance, bet, pScore, hScore, plyrBst, log, inBal);
        
        //Store the game result in game history
        if (plyrBst) {
            gmeHtry[gameCnt] = -1;
        } 
        else if (hScore > 21 || pScore > hScore) {
            gmeHtry[gameCnt] = 1;
        } 
        else {
            gmeHtry[gameCnt] = -1;
        }

        gameCnt++;

        //Ask if they want to play again
        cout << "Play again? (y/n): ";
        char playAgn;
        cin >> playAgn;
        if (playAgn == 'n' || playAgn == 'N') {
            playing = false;
        } 
        else if (playAgn != 'y' && playAgn != 'Y') {
            cout << "Invalid input. Please enter 'y' or 'n'." << endl;
        }
    }
    
    logRslt(log, inBal, balance, "Final balance after quitting the game");
    log.close();  //Closing the log file

    //Bubble sort the history before displaying it
    bubSort(gmeHtry, gameCnt);

    //Display game history
    disHist(gmeHtry, gameCnt);
    
    //History search
    cout << "Do you want to search for a specific game result in the history? (y/n): ";
    char srchCho;
    cin >> srchCho;

    if (srchCho == 'y') {
        cout << "Enter 1 to search for Player wins, -1 for House wins, or 0 for Tie: ";
        int srchKey;
        cin >> srchKey;

        //Linear search
        int srchRes = lnrSrch(gmeHtry, gameCnt, srchKey);
        if (srchRes != -1) {
            cout << "Game " << srchRes + 1 << " has the result." << endl;
        } 
        else {
            cout << "No game found with the specified result." << endl;
        }
    }
    
    // Exit program
    return 0;
}