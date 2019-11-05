/*
John Kolden, SCPD
Standford University CS106B
Filename: Boggle.h
Assignment 4
May 4, 2015
Section Leader: Sarah Spikes

Purpose of Program:
This is a new class that is called by bogglemain and contains all of the logic for a user to play games
of the word game Boggle against a computer.
*/

#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <string>
#include "lexicon.h"
#include "grid.h"
#include "vector.h"
#include "point.h"
#include "hashset.h"
#include "direction.h"

using namespace std;

class Boggle {
public:
    Boggle(Lexicon& dictionary, string boardText = "");
    char getLetter(int row, int col);
    bool checkWord(string word);
    bool humanWordSearch(string word);
    Set<string> computerWordSearch();
    int getScoreHuman();
    int getScoreComputer();
    Set<string> returnHumanWords();
    void addHumanWord(string humanWord);
    Grid<char> boggleGrid;
    int const ROWS = 4;
    int const COLUMNS = 4;
    int const MIN_WORD_SIZE = 4;


private:
    void computerWordSearchHelper( string foundSoFar, int x, int y, Set<string>& computerWords, HashSet<Point> visitedCubes);
    bool humanWordSearchHelper(string word, string foundSoFar, int x, int y, HashSet<Point>& successfulPath);
    void populateBoard (string boardText);

    Set<string> computerWords;
    Lexicon myDictionary;
    int minBoundary (int min);
    int maxBoundary(int max);
    Set<string> validUserWords;

    friend ostream& operator<<(ostream& out, Boggle& boggle);


};

#endif // _boggle_h
