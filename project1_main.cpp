#include <iostream>
#include <vector>
#include <list>
#include <queue>
using namespace std;

// Contains all relevant state information
class Node {
    private:
        vector<vector<int>> currPuzzle { {1, 2, 3}, 
                                         {4, 5, 6}, 
                                         {7, 8, 0} };  // https://www.geeksforgeeks.org/2d-vector-in-cpp-with-user-defined-size/
                                         
        // Node * parent;
        // vector<Node *> children;
        pair<unsigned, unsigned> zero_pos; // zero_pos.first = row (m), zero_pos.second = column (n)
        int shiftTemp;
        int g_of_n = 0;
        int h_of_n = 0;
        int f_of_n = 0;
        char lastMove = ' ';        

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

        // Verifying Legal Input on Puzzle
        void VerifyInput ();
        bool RepeatedDigit (vector<int>);

    public:
        void SetPuzzleInput ();
        void DisplayMatrix ();
        bool isGoal ();
        void expandNode (Node *);
        int GetF_N () const;
        const vector<vector<int>> GetPuzzleData ();
        // void SetF_N(int);
};

void DisplayIntro ();
int GetPuzzleType ();
// void TestPriorityQueue ();

int SelectAlgorithm ();

void UniformCostSearch (Node *);
void A_MisplacedSearch (Node *);
void A_EuclideanSearch (Node *);

int main () {
    Node * initNode = new Node;
    DisplayIntro();

    int puzzleType = -1;
    while (puzzleType < 0) {
        puzzleType = GetPuzzleType();
    }
    if (puzzleType == 2) {
        initNode->SetPuzzleInput();
    }

    int algorithm = -1;
    initNode->DisplayMatrix();
    while (algorithm < 0) {
        algorithm = SelectAlgorithm();
    }

    if (algorithm == 1) {
        UniformCostSearch(initNode);
    }
    if (algorithm == 2) {
        // A_MisplacedSearch(initNode);
    }
    if (algorithm == 3) {
        // A_EuclideanSearch(initNode);
    }

    return 0;
}

// https://www.geeksforgeeks.org/functors-in-cpp/
struct compare_nodes
{
    bool operator() (const Node * lhs, const Node * rhs) const
    {
        if (lhs->GetF_N() > rhs->GetF_N()) {
            return true;
        }
        return false;
    }
};



void UniformCostSearch (Node * initNode) {
    priority_queue<Node *, vector<Node *>, compare_nodes> frontier; // https://stackoverflow.com/questions/15646451/stl-priority-queue-and-overloading-with-pointers
    list<Node *> exploredSet;
    list<Node *> frontierSet;

    frontier.push(initNode);
    exploredSet.clear();
    Node * currNode;

    while(1) {
        if (frontier.empty()) {
            cout << "Failure: No goal found." << endl;
            break;
        }

        currNode = frontier.top();

        if (currNode->isGoal()) {
            cout << "Goal Found!" << endl;
            break;
        }

        list<Node *>::iterator it;
        Node * compareNode;
        for (it = frontierSet.begin(); it != frontierSet.end(); it++) {
            compareNode = *it;
            if (compareNode->GetPuzzleData() == currNode->GetPuzzleData()) {
                frontierSet.erase(it);
                break;
            }
        }
        
        exploredSet.push_back(currNode);

        // Expand Here

        frontier.pop();
    }
}



// void TestPriorityQueue () {
//     Node * initNode = new Node;
//     initNode->SetPuzzleInput();
//     Node * aNode = new Node;
//     initNode->DisplayMatrix();
//     aNode->DisplayMatrix();
//     if (initNode->GetPuzzleData() == aNode->GetPuzzleData()) {
//         cout << "Same" << endl;
//     }
//     else {
//         cout << "Not same" << endl;
//     }
//     // aNode->SetF_N(4);
//     // frontier.push(aNode);
//     // frontier.push(initNode);
//     // Node * thing = frontier.top();
//     // cout << thing->GetF_N() << endl;
// }

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

void Node::SetPuzzleInput () {
    cout << "Enter 9 digits. After each digit put a space. Digits must be between 0-8 and not repeated. The 0 represents the empty tile and must be included." << endl;
    for (unsigned i = 0; i < 3; i++) {
        for (unsigned j = 0; j < 3; j++) {
            cin >> currPuzzle.at(i).at(j);
        }
    }
    cout << endl;
    VerifyInput();
}

void Node::VerifyInput () {
    vector<int> repeatCheck;
    bool hasEmpty = false;
    for (unsigned i = 0; i < 3; i++) {
        for (unsigned j = 0; j < 3; j++) {
            repeatCheck.push_back(currPuzzle.at(i).at(j));
            if (currPuzzle.at(i).at(j) == 0) {
                hasEmpty = true;
            }
            if (currPuzzle.at(i).at(j) > 8 || currPuzzle.at(i).at(j) < 0) {
                cout << "// Invalid Input: One or More Numbers Either Greater Than 8 or Less Than 0 //" << endl;
                DisplayMatrix();
                cout << endl;
                SetPuzzleInput();
            };
        }
    }
    if (RepeatedDigit(repeatCheck)) {
        cout << "// Invalid Input: Repeated Digit //" << endl;
        DisplayMatrix();
        cout << endl;
        SetPuzzleInput();
    }
    if (!hasEmpty) {
        cout << "// Invalid Input: Missing 0 //" << endl;
        DisplayMatrix();
        cout << endl;
        SetPuzzleInput();
    }
}

bool Node::RepeatedDigit (vector<int> repeatCheck) {
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

void Node::DisplayMatrix () {
    for (unsigned i = 0; i < 3; i++) {
        for (unsigned j = 0; j < 3; j++) {
            cout << currPuzzle.at(i).at(j) << ' ';
        }
        cout << endl;
    }
}

bool Node::isGoal () {
    const vector<vector<int>> solvedPuzzle { {1, 2, 3}, 
                                             {4, 5, 6}, 
                                             {7, 8, 0} };

    for (unsigned i = 0; i < 3; i++) {
        for (unsigned j = 0; j < 3; j++) {
            // cout << "currPuzzle.at(i).at(j): " << currPuzzle.at(i).at(j) << " | " << "solvedPuzzle.at(i).at(j): " << solvedPuzzle.at(i).at(j) << endl;
            if (currPuzzle.at(i).at(j) != solvedPuzzle.at(i).at(j)) {
                return false;
            }
        }
    }
    return true;
}

// void Node::SetF_N(int num) {
//     f_of_n = num;
// }

int Node::GetF_N() const {
    return f_of_n;
}

const vector<vector<int>> Node::GetPuzzleData () {
    return currPuzzle;
}
