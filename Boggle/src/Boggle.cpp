/*
John Kolden, SCPD
Standford University CS106B
Filename: Boggle.cpp
Assignment 4
May 4, 2015
Section Leader: Sarah Spikes

Purpose of Program:
This is a new class that is called by bogglemain and contains all of the logic for a user to play games
of the word game Boggle against a computer.
*/

#include "Boggle.h"
#include "grid.h"
#include "random.h"
#include "point.h"
#include "lexicon.h"
#include "bogglegui.h"

// letters on all 6 sides of every cube
static string CUBES[16] = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

// letters on every cube in 5x5 "Big Boggle" version (extension)
static string BIG_BOGGLE_CUBES[25] = {
   "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
   "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
   "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
   "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
   "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};

//creates the boggle board and assigns letters entered by the user, or letters on random cubes.
Boggle::Boggle(Lexicon& dictionary, string boardText) {

    myDictionary = dictionary;
    boggleGrid.resize(ROWS,COLUMNS);

    if (boardText.empty()) {
        for (int i = 0; i < ROWS * COLUMNS; i++) {
            boardText += CUBES[i][randomInteger(0,5)];
            populateBoard(boardText);
            shuffle(boggleGrid);
        }
    } else {
        populateBoard(boardText);
    }
}

//populates the boggle board once the letters are available
void Boggle::populateBoard (string boardText) {

    int i = 0;
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLUMNS; col++){
            boggleGrid[row][col] = boardText[i];
            i++;
        }
    }
}

//returns a single letter based on a provided cordinate
char Boggle::getLetter(int row, int col) {

    if   (boggleGrid.inBounds(row,col)) {
        return boggleGrid[row][col];
    }
    return 0;
}

//validates user entered words
bool Boggle::checkWord(string word) {

    if (word.size() < MIN_WORD_SIZE
            ||validUserWords.contains(toUpperCase(word))
            ||!myDictionary.contains(word)) {
        return false;
    }
    return true;
}

//calls of word search helper to deteremine if the user entered word can be formed on the board
bool Boggle::humanWordSearch(string word) {

    HashSet<Point> coordinates;

    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLUMNS; col++) {
            BoggleGUI::setHighlighted(row, col, false);
        }
    }
  return humanWordSearchHelper(word, "", 0, 0, coordinates);
}

//helper function that performs exhaustive search and backtracking to find the users' words on the board
bool Boggle::humanWordSearchHelper(string word, string foundSoFar, int x, int y, HashSet<Point>& coordinates) {

    word = toUpperCase(word);
    int currentPosition = foundSoFar.size();
    if (foundSoFar == word) {//base case, we are done
        return true;
    } else {//look for first char on board
        if (foundSoFar.empty()) {
            for (int col = 0; col < boggleGrid.numCols(); col ++) {//start with all the letters
                for (int row = 0; row < boggleGrid.numRows(); row++) {
                    char currentCube = getLetter(row, col);
                    if (currentCube == word[0]) {
                        foundSoFar += currentCube;
                        Point point(row, col);
                        BoggleGUI::setHighlighted(row, col,true);
                        coordinates.add(point);
                        if (humanWordSearchHelper(word, foundSoFar, row, col, coordinates)) {
                            return true;
                        } else {
                            coordinates.clear();
                            foundSoFar.erase();
                            BoggleGUI::setHighlighted(row, col, false);
                        }
                    }
                }
            }

        } else {//foundSoFar is not empty so we need to search around where we just found each letter
            int minY = minBoundary(y);
            int maxY = maxBoundary(y);
            int minX = minBoundary(x);
            int maxX = maxBoundary(x);

            for (int neighborCol = minY ; neighborCol <= maxY; neighborCol++) {
                for (int neighborRow = minX; neighborRow <= maxX; neighborRow++) {
                    if (!(neighborRow == x && neighborCol == y)) {
                        char currentCube = getLetter( neighborRow,neighborCol);
                        Point point(neighborRow, neighborCol);
                        if (currentCube == word[currentPosition] && !coordinates.contains(point)) {
                            foundSoFar += currentCube;//choose
                            coordinates.add(point);//choose
                            BoggleGUI::setHighlighted(neighborRow, neighborCol, true);
                            if (humanWordSearchHelper(word, foundSoFar, neighborRow, neighborCol, coordinates)) {//explore
                                return true;
                            } else {
                                foundSoFar = foundSoFar.substr(0, foundSoFar.length() - 1);//unchoose
                                coordinates.remove(point);//unchoose
                                BoggleGUI::setHighlighted(neighborRow, neighborCol, false);
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}

//returns the minimum boundary based on the grid coordinate
int Boggle::minBoundary(int min) {

    if (min == 0) {
        return 0;
    } else {
        return min-1;
    }
    return 0;
}

//returns the maximum boundary based on the grid coordinate
int Boggle::maxBoundary(int max) {

    if (max == 3) {
        return 3;
    } else {
        return max + 1;
    }
    return 0;
}

//adds human words to a set
void Boggle::addHumanWord(string humanWord) {

    validUserWords.add(toUpperCase(humanWord));
}

//returns the set of valid human words
Set<string> Boggle::returnHumanWords() {

    return validUserWords;
}

//computes the human score
int Boggle::getScoreHuman() {

    int score = 0;
    for (string s:  validUserWords) {
        score += (s.length() - 3);
    }
    return score;
}

//initiates and calls the computerWordSearchHelper to find all remaining words on the board
Set<string> Boggle::computerWordSearch() {

    string foundSoFar;
    HashSet<Point> visitedCubes;

    for (int col = 0; col < boggleGrid.numCols(); col ++) {
        for (int row = 0; row < boggleGrid.numRows(); row++) {
            BoggleGUI::setHighlighted(row, col,false);  //clear GUI
            char currentCube = getLetter(row,col);
            foundSoFar = currentCube;
            Point point(row, col);
            visitedCubes.add(point);//choose
            computerWordSearchHelper(foundSoFar, row, col, computerWords, visitedCubes);//explore
            visitedCubes.remove(point);//unchoose
        }
    }

 return  computerWords;

}

//performs exhaustive search and backtracking to identify all remaining words on a given board
void Boggle::computerWordSearchHelper(string foundSoFar, int x, int y, Set<string>& computerWords, HashSet<Point> visitedCubes) {

    if (foundSoFar.length() >= MIN_WORD_SIZE
            && !validUserWords.contains(foundSoFar)
            && myDictionary.contains(foundSoFar)) {//base case, we are done
        computerWords.add(foundSoFar);
    }

    if (!myDictionary.containsPrefix(foundSoFar)) {
        return;
    }

    int minY = minBoundary(y);
    int maxY = maxBoundary(y);
    int minX = minBoundary(x);
    int maxX = maxBoundary(x);

    for (int neighborCol = minY ; neighborCol <= maxY; neighborCol++) {
        for (int neighborRow = minX; neighborRow <= maxX; neighborRow++) {
            Point point(neighborRow, neighborCol);
            if (!(neighborRow == x && neighborCol == y)
                    && !visitedCubes.contains(point)
                    ) {//explore neighbor cubes but not the cube we are starting with
                char currentCube = getLetter( neighborRow,neighborCol);
                visitedCubes.add(point);//choose
                computerWordSearchHelper(foundSoFar + currentCube, neighborRow, neighborCol, computerWords, visitedCubes);//explore
                visitedCubes.remove(point);//unchoose
            }
        }
    }
}

//computes the computer score
int Boggle::getScoreComputer() {
    int computerScore = 0;
    for (string s:  computerWords) {
        computerScore += (s.length() - 3);
    }
    return computerScore;
}

//prints a Boggle board to the concole
ostream& operator<<(ostream& out, Boggle& boggle) {

    for (int row = 0; row < boggle.ROWS; row++) {
        for (int col = 0; col < boggle.COLUMNS; col++){
          out <<  boggle.boggleGrid[row][col];
        }
        out << endl;
}
    return out;
}
