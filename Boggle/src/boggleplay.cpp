/*
John Kolden, SCPD
Standford University CS106B
Filename: boggleplay.cpp
Assignment 4
May 4, 2015
Section Leader: Sarah Spikes

Purpose of Program:
This controls all of the user interaction during one or more games of Boggle.
*/

#include "lexicon.h"
#include "simpio.h"
#include <iostream>
#include <ostream>
#include "Boggle.h"
#include "map.h"
#include "bogglegui.h"

using namespace std;

void playOneGame(Lexicon& dictionary) {

    string boardText;
    string userWord;

    string userPref = toLowerCase(getLine("Do you want to generate a random board? "));

    if (userPref == "n") {

        boardText = toUpperCase(getLine("Type the 16 letters to appear on the board: "));

        while (boardText.size() != (16) ||
               boardText.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ") != string::npos) {
            cout << "That is not a valid 16-letter board string. Try again." << endl;
            boardText = toUpperCase(getLine("Type the 16 letters to appear on the board: "));
        }
    }

    Boggle myBoggle(dictionary, boardText);
    BoggleGUI::initialize(myBoggle.ROWS, myBoggle.COLUMNS);
    string boardTextString;

    for (int row = 0; row < myBoggle.ROWS; row++) {
        for (int col = 0; col < myBoggle.COLUMNS; col++) {
            boardTextString += myBoggle.getLetter(row, col);
        }
    }

    BoggleGUI::labelAllCubes(boardTextString);

    clearConsole();
    cout << "It's your turn!" << endl;
    BoggleGUI::setStatusMessage("It's your turn!");

    while (true) {
        cout << myBoggle;
        cout << endl;
        cout << "Your words (" << myBoggle.returnHumanWords().size() << "): " << myBoggle.returnHumanWords() <<  endl;
        cout << "Your score: " << myBoggle.getScoreHuman() << endl;
        userWord = getLine("Type a word (or Enter to stop): ");

        if (userWord == "") {
            cout << endl;
            break;
        } else {
            clearConsole();
            if (!myBoggle.checkWord(userWord)) {
                cout << "You must enter an unfound 4+ letter word from the dictionary." << endl;
                BoggleGUI::setStatusMessage("You must enter an unfound 4+ letter word from the dictionary.");

            } else {

                if (!myBoggle.humanWordSearch(userWord)) {
                    cout << "That word can't be formed on this board." << endl;
                    BoggleGUI::setStatusMessage("That word can't be formed on this board.");
                } else {
                    //4+ char and not useed before, can make on board and is in dictionary
                    cout << "You found a new word! "  << toUpperCase(userWord) << endl;
                    BoggleGUI::setStatusMessage("You found a new word! " + toLowerCase((userWord)) );
                    myBoggle.addHumanWord(toUpperCase(userWord));
                    BoggleGUI::recordWord(userWord, BoggleGUI::HUMAN);
                    setScore(myBoggle.getScoreHuman(), BoggleGUI::HUMAN);
                }
            }
        }
    }

    //computer turn:
    cout << "It's my turn!" << endl;
    BoggleGUI::setStatusMessage("It's my turn!");

    Set<string> compWords =  myBoggle.computerWordSearch();
    cout << "My words (" << compWords.size() << "): " << compWords <<endl;
    cout << "My score: " << myBoggle.getScoreComputer() << endl;

    for (string s : compWords) {
        BoggleGUI::recordWord(s, BoggleGUI::COMPUTER);
    }

    setScore(myBoggle.getScoreComputer(), BoggleGUI::COMPUTER);

    if (myBoggle.getScoreComputer() > myBoggle.getScoreHuman()) {
        cout << "Ha ha ha, I destroyed you. Better luck next time, puny human!" << endl;
        BoggleGUI::setStatusMessage("Ha ha ha, I destroyed you. Better luck next time, puny human!");
    } else {
        cout << "WOW, you defeated me! Congratulations!";
        BoggleGUI::setStatusMessage("WOW, you defeated me! Congratulations!");
    }
}
