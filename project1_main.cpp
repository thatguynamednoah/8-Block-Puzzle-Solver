#include <iostream>
#include <vector>
using namespace std;

vector<vector<int>> matrix { {1, 2, 3}, {4, 5, 6}, {7, 8, 0} }; // https://www.geeksforgeeks.org/2d-vector-in-cpp-with-user-defined-size/

// m rows x n columns
class Node {
    vector<vector<int>> currPuzzle;
    Node *parent;
    pair<unsigned, unsigned> zero_pos; // zero_pos.first = row (y), zero_pos.second = column (x)
    int shiftTemp;

    // Shift functions return true if shift occurs
    bool shiftUp () {
        if (zero_pos.first != 0) {
            shiftTemp = currPuzzle.at(zero_pos.first - 1).at(zero_pos.second);
            currPuzzle.at(zero_pos.first - 1).at(zero_pos.second) = 0;
            currPuzzle.at(zero_pos.first).at(zero_pos.second) = shiftTemp;
            zero_pos.first = zero_pos.first - 1;
            return true;
        }
        return false;
    }
    bool shiftDown () {
        if (zero_pos.first != 2) {
            shiftTemp = currPuzzle.at(zero_pos.first + 1).at(zero_pos.second);
            currPuzzle.at(zero_pos.first + 1).at(zero_pos.second) = 0;
            currPuzzle.at(zero_pos.first).at(zero_pos.second) = shiftTemp;
            zero_pos.first = zero_pos.first + 1;
            return true;
        }
        return false;
    }
    bool shiftLeft () {
        if (zero_pos.second != 0) {
            shiftTemp = currPuzzle.at(zero_pos.first).at(zero_pos.second - 1);
            currPuzzle.at(zero_pos.first).at(zero_pos.second - 1) = 0;
            currPuzzle.at(zero_pos.first).at(zero_pos.second) = shiftTemp;
            zero_pos.second = zero_pos.second - 1;
            return true;
        }
        return false;
    }
    bool shiftRight () {
        if (zero_pos.second != 2) {
            shiftTemp = currPuzzle.at(zero_pos.first).at(zero_pos.second + 1);
            currPuzzle.at(zero_pos.first).at(zero_pos.second + 1) = 0;
            currPuzzle.at(zero_pos.first).at(zero_pos.second) = shiftTemp;
            zero_pos.second = zero_pos.second + 1;
            return true;
        }
        return false;
    }
    
};

void DisplayIntro ();
int GetPuzzleType ();
void GetPuzzleInput ();
void VerifyInput ();
bool RepeatedDigit (vector<int>);
void DisplayMatrix ();
int SelectAlgorithm ();

void UniformCostSearch ();
void A_MisplacedSearch ();
void A_EuclideanSearch ();

int main () {
    DisplayIntro();

    int puzzleType = -1;
    while (puzzleType < 0) {
        puzzleType = GetPuzzleType();
    }
    if (puzzleType == 2) {
        GetPuzzleInput();
    } 

    int algorithm = -1;
    while (algorithm < 0) {
        algorithm = SelectAlgorithm();
    }

    if (algorithm == 1) {
        UniformCostSearch();
    }
    if (algorithm == 2) {
        A_MisplacedSearch();
    }
    if (algorithm == 3) {
        A_EuclideanSearch();
    }

    return 0;
}

void UniformCostSearch () {

}

void DisplayIntro () {
    cout << "Welcome to ndem002's 8 puzzle solver." << endl;
}

int GetPuzzleType () {
    cout << "Type 1 to use a default puzzle, or 2 to enter your own puzzle." << endl;
    int type;
    cin >> type;
    if (type < 1 || type > 2) {
        return -1;
    }
    return type;
}

void GetPuzzleInput () {
    cout << "Enter 9 digits. After each digit put a space. Digits must be between 0-8 and not repeated. The 0 represents the empty tile and must be included." << endl;
    for (unsigned i = 0; i < 3; i++) {
        for (unsigned j = 0; j < 3; j++) {
            cin >> matrix.at(i).at(j);
        }
    }
    cout << endl;
    VerifyInput();
}

void VerifyInput () {
    vector<int> repeatCheck;
    bool hasEmpty = false;
    for (unsigned i = 0; i < 3; i++) {
        for (unsigned j = 0; j < 3; j++) {
            repeatCheck.push_back(matrix.at(i).at(j));
            if (matrix.at(i).at(j) == 0) {
                hasEmpty = true;
            }
            if (matrix.at(i).at(j) > 8 || matrix.at(i).at(j) < 0) {
                cout << "// Invalid Input: One or More Numbers Either Greater Than 8 or Less Than 0 //" << endl;
                DisplayMatrix();
                cout << endl;
                GetPuzzleInput();
            };
        }
    }
    if (RepeatedDigit(repeatCheck)) {
        cout << "// Invalid Input: Repeated Digit //" << endl;
        DisplayMatrix();
        cout << endl;
        GetPuzzleInput();
    }
    if (!hasEmpty) {
        cout << "// Invalid Input: Missing 0 //" << endl;
        DisplayMatrix();
        cout << endl;
        GetPuzzleInput();
    }
}

bool RepeatedDigit (vector<int> repeatCheck) {
    int repeatValue;
    for (unsigned i = 0; i < repeatCheck.size(); i++) {
        repeatValue = repeatCheck.at(i);
        for (unsigned j = 0; j < repeatCheck.size(); j++) {
            if (((j != i) && (repeatValue == repeatCheck.at(j))) || ((j != i) && (repeatCheck.at(j) == 1 && repeatValue == 1))) {
                return true;
            }
        }
    }
    return false;
}

void DisplayMatrix () {
    for (unsigned i = 0; i < 3; i++) {
        for (unsigned j = 0; j < 3; j++) {
            cout << matrix.at(i).at(j) << ' ';
        }
        cout << endl;
    }
}

int SelectAlgorithm () {
    int algorithmNum;
    cout << "Enter your choice of Algorithm:\n"
    << "1 - Uniform Cost Search\n"
    << "2 - A* with the Misplaced Tile heuristic\n"
    << "3 - A* with the Euclidian distance heuristic\n";
    cin >> algorithmNum;
    if (algorithmNum > 0 && algorithmNum < 4) {
        return algorithmNum;
    }
    else {
        return -1;
    }
}