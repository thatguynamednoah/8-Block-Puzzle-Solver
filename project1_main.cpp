#include <iostream>
#include <vector>
#include <list>
#include <queue>
using namespace std;

// Contains all relevant state information
class Node {
    private:
        vector<vector<int>> currPuzzle;
                                         
        // Node * parent;
        // vector<Node *> children;
        pair<unsigned, unsigned> zero_pos; // zero_pos.first = row (m), zero_pos.second = column (n)
        // char lastMove = ' ';        

        // Verifying Legal Input on Puzzle
        void VerifyInput ();
        bool RepeatedDigit (vector<int>);

    public:
        Node () {
            currPuzzle = { {1, 2, 3}, 
                           {4, 5, 6}, 
                           {7, 8, 0} }; // https://www.geeksforgeeks.org/2d-vector-in-cpp-with-user-defined-size/
            zero_pos.first = 2;
            zero_pos.second = 2;
        }
        Node (vector<vector<int>> puzzleVals) {
            currPuzzle = puzzleVals;
            for (unsigned i = 0; i < 3; i++) {
                for (unsigned j = 0; j < 3; j++) {
                    if (currPuzzle.at(i).at(j) == 0) {
                        zero_pos.first = i;
                        zero_pos.second = j;
                    }
                }
            }
        }

        int depth = 0;
        int g_of_n = 0;
        int h_of_n = 0;
        int f_of_n = 0;
        
        void SetPuzzleInput ();
        void DisplayMatrix ();
        bool isGoal ();
        void expandNode (Node *);
        int GetF_N () const;
        vector<vector<int>> GetPuzzleData ();

        // Shift functions return true if shift occurs
        bool shiftUp () {
            int shiftTemp = 0;
            if (zero_pos.first != 0) {
                shiftTemp = currPuzzle.at(zero_pos.first - 1).at(zero_pos.second);
                currPuzzle.at(zero_pos.first - 1).at(zero_pos.second) = 0;
                currPuzzle.at(zero_pos.first).at(zero_pos.second) = shiftTemp;
                zero_pos.first = zero_pos.first - 1;
                g_of_n += 1;
                f_of_n = h_of_n + g_of_n;
                return true;
            }
            return false;
        }
        bool shiftDown () {
            int shiftTemp = 0;
            if (zero_pos.first != 2) {
                shiftTemp = currPuzzle.at(zero_pos.first + 1).at(zero_pos.second);
                currPuzzle.at(zero_pos.first + 1).at(zero_pos.second) = 0;
                currPuzzle.at(zero_pos.first).at(zero_pos.second) = shiftTemp;
                zero_pos.first = zero_pos.first + 1;
                g_of_n += 1;
                f_of_n = h_of_n + g_of_n;
                return true;
            }
            return false;
        }
        bool shiftLeft () {
            int shiftTemp = 0;
            if (zero_pos.second != 0) {
                shiftTemp = currPuzzle.at(zero_pos.first).at(zero_pos.second - 1);
                currPuzzle.at(zero_pos.first).at(zero_pos.second - 1) = 0;
                currPuzzle.at(zero_pos.first).at(zero_pos.second) = shiftTemp;
                zero_pos.second = zero_pos.second - 1;
                g_of_n += 1;
                f_of_n = h_of_n + g_of_n;
                return true;
            }
            return false;
        }
        bool shiftRight () {
            int shiftTemp = 0;
            if (zero_pos.second != 2) {
                shiftTemp = currPuzzle.at(zero_pos.first).at(zero_pos.second + 1);
                currPuzzle.at(zero_pos.first).at(zero_pos.second + 1) = 0;
                currPuzzle.at(zero_pos.first).at(zero_pos.second) = shiftTemp;
                zero_pos.second = zero_pos.second + 1;
                g_of_n += 1;
                f_of_n = h_of_n + g_of_n;
                return true;
            }
            return false;
        }
        
        // void SetF_N(int);
};

void DisplayIntro ();
int GetPuzzleType ();
// void TestPriorityQueue ();

int SelectAlgorithm ();

void UniformCostSearch (Node *);
void A_MisplacedSearch (Node *);
void A_EuclideanSearch (Node *);

// void TesterGo (Node * initNode) {
//     cout << endl;

//     initNode->shiftRight();
//     initNode->DisplayMatrix();
//     cout << endl;

//     initNode->shiftLeft();
//     initNode->DisplayMatrix();
//     cout << endl;

//     initNode->shiftRight();
//     initNode->DisplayMatrix();
//     cout << endl;

//     initNode->shiftRight();
//     initNode->DisplayMatrix();
//     cout << endl;

//     initNode->shiftDown();
//     initNode->DisplayMatrix();
//     cout << endl;

//     initNode->shiftUp();
//     initNode->DisplayMatrix();
//     cout << endl;

//     initNode->shiftDown();
//     initNode->DisplayMatrix();
//     cout << endl;

//     initNode->shiftDown();
//     initNode->DisplayMatrix();
//     cout << endl;

//     initNode->DisplayMatrix();
//     if (initNode->isGoal()) {
//             cout << "Goal Found!" << endl;
//     }
//     else {
//         cout << "fail" << endl;
//     }
// }

int main () {
    // vector<vector<int>> thing = { {0, 1, 2}, 
    //                               {4, 5, 3}, 
    //                               {7, 8, 6} };
    // Node * initNode = new Node(thing);
    // initNode->DisplayMatrix();
    // TesterGo(initNode);

    Node * initNode = new Node;
    DisplayIntro();

    int puzzleType = -1;
    while (puzzleType < 0 || puzzleType > 2) {
        puzzleType = GetPuzzleType();
    }
    if (puzzleType == 2) {
        initNode->SetPuzzleInput();
    }

    int algorithm = -1;
    initNode->DisplayMatrix();
    while (algorithm < 0 || algorithm > 3) {
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
        if (lhs->f_of_n > rhs->f_of_n) {
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
    frontierSet.push_back(initNode);

    Node * currNode;
    Node * expandedNodes[4];

    while(1) {
        cout << frontier.size() << endl;
        if (frontier.empty()) {
            cout << "Failure: No goal found." << endl;
            break;
        }

        currNode = frontier.top();
        cout << currNode->f_of_n << endl;
        // currNode->DisplayMatrix();
        // cout << "---" << endl;
        if (currNode->isGoal()) {
            cout << "Goal Found!" << endl;
            break;
        }

        // Removing currNode from frontier
        list<Node *>::iterator it;
        Node * compareNode;
        for (it = frontierSet.begin(); it != frontierSet.end(); it++) {
            compareNode = *it;
            if (compareNode->GetPuzzleData() == currNode->GetPuzzleData()) {
                frontierSet.erase(it);
                break;
            }
        }

        // Pushing non-goal node to explored set before expanding
        exploredSet.push_back(currNode);

        // Expanding Nodes
        for (unsigned i = 0; i < 4; i++) {
            expandedNodes[i] = new Node(currNode->GetPuzzleData());
        }
        cout << "Starting Expansion" << endl;
        expandedNodes[0]->shiftUp();
        expandedNodes[1]->shiftDown();
        expandedNodes[2]->shiftLeft();
        expandedNodes[3]->shiftRight();
        cout << "End Expansion" << endl;

        for (unsigned i = 0; i < 4; i++) {
            cout << "Starting Frontier Set" << endl;
            for (it = frontierSet.begin(); it != frontierSet.end(); it++) {
                compareNode = *it;
                if (compareNode->GetPuzzleData() == expandedNodes[i]->GetPuzzleData()) {
                    delete expandedNodes[i];
                    expandedNodes[i] = nullptr;
                    break;
                }
            }
            cout << "Ending Frontier Set" << endl;
            cout << "Starting Explored Set" << endl;
            for (it = exploredSet.begin(); it != exploredSet.end(); it++) {
                compareNode = *it;
                if (compareNode->GetPuzzleData() == expandedNodes[i]->GetPuzzleData()) {
                    delete expandedNodes[i];
                    expandedNodes[i] = nullptr;
                    break;
                }
            }
            cout << "Ending Explored Set" << endl;
            if (expandedNodes[i] != nullptr) {
                expandedNodes[i]->g_of_n = currNode->g_of_n + 1;
                expandedNodes[i]->f_of_n = expandedNodes[i]->g_of_n + expandedNodes[i]->h_of_n;
                frontier.push(expandedNodes[i]);
                frontierSet.push_back(expandedNodes[i]);
            }
            cout << "End Add" << endl;
        }

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
                zero_pos.first = i;
                zero_pos.second = j;
                hasEmpty = true;
            }
            if (currPuzzle.at(i).at(j) > 8 || currPuzzle.at(i).at(j) < 0) {
                cout << "// Invalid Input: One or More Numbers Either Greater Than 8 or Less Than 0 //" << endl;
                DisplayMatrix();
                cout << endl;
                SetPuzzleInput();
                return;
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

const vector<vector<int>> solvedPuzzle { {1, 2, 3}, 
                                         {4, 5, 6}, 
                                         {7, 8, 0} };
bool Node::isGoal () {
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

vector<vector<int>> Node::GetPuzzleData () {
    return currPuzzle;
}
