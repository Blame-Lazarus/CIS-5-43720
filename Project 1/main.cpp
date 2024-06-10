/* 
 * Author: Luis Y Vazquez Quiroz
 * Created on: 05/10/2024
 * Purpose: Game
 */

//System Libraries
#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <string>
#include <ctime>

using namespace std;

//User Libraries

//Global Constants

//Menu Function Prototypes

//Execution Begins Here
int main(int argc, char** argv) {
    //Set a Random number seed here.
    srand(static_cast<unsigned int>(time(0)));
    
    //File use
    ofstream log("game_log.txt", ios::app); //This line opens a file to save game results.
    
    //Introduction
    string welcome = "Welcome to the Blackjack Game!";  //String library use
    cout << welcome << endl;
    
    //Declare Variables
    int pCardVal, pScore = 0;             //Player card value and score
    int hCardVal, hScore = 0;             //House card value and score
    char choice;                          //Choice variable to save player choice (hit or stand)
    float balance = 100.0;                //Initial balance for bets
    float bet;                            //Bet size
    bool playing = true;                  //Boolean to check the status of the game
    
    //Place bets
    cout << "Balance: $" << fixed << setprecision(2) << balance << endl
         << "Place bet: ";
    cin >> bet;
    balance -= round(bet * 100) / 100;     //Deduct bet from balance, rounding to the nearest cent
    
    //Deal two cards to player
    for (int i = 0; i < 2; ++i) {
        pCardVal = rand() % 10 + 1;       //Deal random card to player between 1 and 10
        //Need to add Ace card, 1 or 11 case
        cout << "Player dealt card: " << pCardVal << endl;
        pScore += pCardVal;               //Add the card value to the score
    }
    
    //Deal two cards to house
    for (int i = 0; i < 2; ++i) {
        hCardVal = rand() % 10 + 1;       //Deal random card to house between 1 and 10
        //Need to add Ace card, 1 or 11 case
        if (i < 1) {                      //Only one card is visible to the player
        cout << "House Dealt card: " << hCardVal << endl;
        hScore += hCardVal;               //Add the card value to the score
        }
    }
    
    //Check if house has a blackjack
    if (hScore == 21) {
        //If player has 21 (tie)
        if (pScore == 21) {
            balance += bet;
            cout << "House score: " << hScore << endl << "Player score: " 
                 << pScore << endl << "Game tied." << endl << "New Balance: " 
                 << fixed << setprecision(2) << balance << endl;                 //Display result
            log << "Game results" << endl << "House score: " << hScore << endl
                << "Player score: " << pScore << endl << "Tie" << endl;         //Save results in log
            log.close();                  //Close the file
            return 0;                     //End the game
        }
        
        //If player does not have 21
        else {
            cout << "House score: " << hScore << endl << "Player score: " 
                 << pScore << endl<< "Game lost." << endl << "New Balance: " 
                 << fixed << setprecision(2) << balance << endl;                //Display result
            log << "Game results" << endl << "House score: " << hScore << endl
                << "Player score: " << pScore << endl << "Player lose" << endl; //Save results in log
            log.close();                  //Close the file
            return 0;                     //End the game
        }
    }
    
    /*The following while loop contains a do loop but it's unnecessary, it could be changed
     to a switch statement but due to the requirement that the program cannot include any
     breaks, a switch statement would not work*/
    while (playing) {
        do {
            cout << "Total: " << pScore << endl;
            if (pScore >= 21) {
                //Display result
                cout << "Game over. Your total is " << pScore << (pScore > 21 ? ". Bust!" : ".") << endl
                     << "New Balance: " << fixed << setprecision(2) << balance << endl;
                //Save result
                log << "Game results" << endl << "House score: " << hScore << "Player score: " 
                    << pScore << endl << "Busted" << endl;
                log.close();                        //Close the file
                return 0;                           //Exit game
            }

            cout << "Hit or stand? (h/s): ";        //Ask the player if they want to hit or stand
            cin >> choice;

            if (choice == 'h') {                    //Hit
                pCardVal = rand() % 10 + 1;         //Deal another card
                cout << "Dealt card: " << pCardVal << endl;
                pScore += pCardVal;                 //Add the card value to the score
            } 
            else if (choice == 's') {               //Stand
                cout << "Final total: " << pScore << endl;
                //Need to add score checker here
                //Display result
                cout << "Player stands with total: " << pScore << endl
                     << "New Balance: " << fixed << setprecision(2) << balance << endl;
                //Save result
                log << "Game results" << endl << "House score: " << hScore << "Player score: " 
                    << pScore << endl << "Result" << endl;
                //Need to add score checker here
                log.close();                        //Close the file
                return 0;                           //Exit game
            } 
            else {                                  //In case of invalid input
                cout << "Invalid choice, try again." << endl;
            }
        } while (playing);
    }
    
    //Exit program
    return 0;
}

//Function Definitions