#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <cmath>
using namespace std;

// https://www.geeksforgeeks.org/2d-vector-in-cpp-with-user-defined-size/
const vector<vector<int>> solvedPuzzle { {1, 2, 3}, 
                                         {4, 5, 6}, 
                                         {7, 8, 0} };

// Contains all relevant state information
class Node {
    private:
        vector<vector<int>> currPuzzle;
        pair<unsigned, unsigned> zero_pos; // zero_pos.first = row (m), zero_pos.second = column (n)      

        // Verifying Legal Input on Puzzle
        void VerifyInput ();
        bool RepeatedDigit (vector<int>);

    public:
        Node () {
            currPuzzle = { {1, 2, 3}, 
                           {4, 5, 6}, 
                           {7, 8, 0} };
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
        vector<vector<int>> GetPuzzleData () { return currPuzzle; };

        // Used for calculating h(n) for A* search
        void A_Misplaced ();
        void A_Euclidean ();

        // Shift functions return true if shift occurs
        bool shiftUp ();
        bool shiftDown ();
        bool shiftLeft ();
        bool shiftRight ();
};

void DisplayIntro ();
int GetPuzzleType ();
int SelectAlgorithm ();
void Search (Node *, int);

int main () {
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
    while (algorithm < 0 || algorithm > 3) {
        algorithm = SelectAlgorithm();
        cout << endl;
    }

    if (algorithm == 1) {
        Search(initNode, 0);
    }
    if (algorithm == 2) {
        Search(initNode, 1);
    }
    if (algorithm == 3) {
        Search(initNode, 2);
    }

    return 0;
}

// https://www.geeksforgeeks.org/functors-in-cpp/
struct compare_nodes // kind of reminds me of sml
{
    bool operator() (const Node * lhs, const Node * rhs) const
    {
        if (lhs->f_of_n > rhs->f_of_n) {
            return true;
        }
        return false;
    }
};



void Search (Node * initNode, int typeOfSearch) {
    int total_nodes = 0;
    int queue_size = 0;
    priority_queue<Node *, vector<Node *>, compare_nodes> frontier; // https://stackoverflow.com/questions/15646451/stl-priority-queue-and-overloading-with-pointers
    list<Node *> exploredSet;
    list<Node *> frontierSet;

    frontier.push(initNode);
    frontierSet.push_back(initNode);

    Node * currNode;
    Node * expandedNodes[4];

    while(1) {
        if (queue_size < frontier.size()) {
            queue_size = frontier.size();
        }

        if (frontier.empty()) {
            cout << "Failure: No goal found." << endl;
            cout << "The search algorithm expanded a total of " << total_nodes << " nodes." << endl;
            cout << "The maximum number of nodes in queue at any one time: " << queue_size << "." << endl;
            break;
        }

        currNode = frontier.top();
        frontier.pop();

        if (currNode->isGoal()) {
            cout << "Goal Found!" << endl;
            currNode->DisplayMatrix();
            cout << endl;
            cout << "To solve this problem the search algorithm expanded a total of " << total_nodes << " nodes." << endl;
            cout << "The maximum number of nodes in queue at any one time: " << queue_size << "." << endl;
            cout << "The depth of the goal node was " << currNode->depth << "." << endl;
            break;
        }

        // Pushing non-goal node to explored set before expanding
        exploredSet.push_back(currNode);
        total_nodes += 1;

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

        cout << "The best state to expand with g(n) = " << currNode->g_of_n << " and h(n) = " << currNode->h_of_n << " is :" << endl;
        currNode->DisplayMatrix();
        cout << endl;

        // Expanding Nodes
        for (unsigned i = 0; i < 4; i++) {
            expandedNodes[i] = new Node(currNode->GetPuzzleData());
        }

        expandedNodes[0]->shiftUp();
        expandedNodes[1]->shiftDown();
        expandedNodes[2]->shiftLeft();
        expandedNodes[3]->shiftRight();

        // Checking for duplicate nodes
        for (unsigned i = 0; i < 4; i++) {
            for (it = frontierSet.begin(); it != frontierSet.end(); it++) {
                compareNode = *it;
                if (compareNode->GetPuzzleData() == expandedNodes[i]->GetPuzzleData()) {
                    delete expandedNodes[i];
                    expandedNodes[i] = nullptr;
                    break;
                }
            }
            if (expandedNodes[i] != nullptr) {
                for (it = exploredSet.begin(); it != exploredSet.end(); it++) {
                    compareNode = *it;
                    if (compareNode->GetPuzzleData() == expandedNodes[i]->GetPuzzleData()) {
                        delete expandedNodes[i];
                        expandedNodes[i] = nullptr;
                        break;
                    }
                }
            }
            if (expandedNodes[i] != nullptr) {
                expandedNodes[i]->depth = currNode->depth + 1;
                expandedNodes[i]->g_of_n = currNode->g_of_n + 1;

                if (typeOfSearch == 1) {
                    expandedNodes[i]->A_Misplaced();
                }
                if (typeOfSearch == 2) {
                    expandedNodes[i]->A_Euclidean();
                }

                expandedNodes[i]->f_of_n = expandedNodes[i]->g_of_n + expandedNodes[i]->h_of_n;

                frontier.push(expandedNodes[i]);
                frontierSet.push_back(expandedNodes[i]);
            }
        }

    }
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

bool Node::shiftUp () {
    int shiftTemp = 0;
    if (zero_pos.first != 0) {
        shiftTemp = currPuzzle.at(zero_pos.first - 1).at(zero_pos.second);
        currPuzzle.at(zero_pos.first - 1).at(zero_pos.second) = 0;
        currPuzzle.at(zero_pos.first).at(zero_pos.second) = shiftTemp;
        zero_pos.first = zero_pos.first - 1;
        return true;
    }
    return false;
}

bool Node::shiftDown () {
    int shiftTemp = 0;
    if (zero_pos.first != 2) {
        shiftTemp = currPuzzle.at(zero_pos.first + 1).at(zero_pos.second);
        currPuzzle.at(zero_pos.first + 1).at(zero_pos.second) = 0;
        currPuzzle.at(zero_pos.first).at(zero_pos.second) = shiftTemp;
        zero_pos.first = zero_pos.first + 1;
        return true;
    }
    return false;
}

bool Node::shiftLeft () {
    int shiftTemp = 0;
    if (zero_pos.second != 0) {
        shiftTemp = currPuzzle.at(zero_pos.first).at(zero_pos.second - 1);
        currPuzzle.at(zero_pos.first).at(zero_pos.second - 1) = 0;
        currPuzzle.at(zero_pos.first).at(zero_pos.second) = shiftTemp;
        zero_pos.second = zero_pos.second - 1;
        return true;
    }
    return false;
}

bool Node::shiftRight () {
    int shiftTemp = 0;
    if (zero_pos.second != 2) {
        shiftTemp = currPuzzle.at(zero_pos.first).at(zero_pos.second + 1);
        currPuzzle.at(zero_pos.first).at(zero_pos.second + 1) = 0;
        currPuzzle.at(zero_pos.first).at(zero_pos.second) = shiftTemp;
        zero_pos.second = zero_pos.second + 1;
        return true;
    }
    return false;
}

void Node::A_Euclidean () {
    h_of_n = 0;
    int euclidVals[9];

    int m = 0.0;
    int n = 0.0;

    for (unsigned i = 0; i < 3; i++) {
        for (unsigned j = 0; j < 3; j++) {
            if (currPuzzle.at(i).at(j) == 1) {
                m = (i - 0);
                m = m * m;

                n = (j - 0);
                n = n * n;

                m = abs(m);
                n = abs(n);

                euclidVals[1] = m + n;
            }
            else if (currPuzzle.at(i).at(j) == 2) {
                m = (i - 0);
                m = m * m;

                n = (j - 1);
                n = n * n;

                m = abs(m);
                n = abs(n);

                euclidVals[2] = m + n;
            }
            else if (currPuzzle.at(i).at(j) == 3) {
                m = (i - 0);
                m = m * m;

                n = (j - 2);
                n = n * n;

                m = abs(m);
                n = abs(n);

                euclidVals[3] = m + n;
            }
            else if (currPuzzle.at(i).at(j) == 4) {
                m = (i - 1);
                m = m * m;

                n = (j - 0);
                n = n * n;

                m = abs(m);
                n = abs(n);

                euclidVals[4] = m + n;
            }
            else if (currPuzzle.at(i).at(j) == 5) {
                m = (i - 1);
                m = m * m;

                n = (j - 1);
                n = n * n;

                m = abs(m);
                n = abs(n);

                euclidVals[5] = m + n;
            }
            else if (currPuzzle.at(i).at(j) == 6) {
                m = (i - 1);
                m = m * m;

                n = (j - 2);
                n = n * n;

                m = abs(m);
                n = abs(n);

                euclidVals[6] = m + n;
            }
            else if (currPuzzle.at(i).at(j) == 7) {
                m = (i - 2);
                m = m * m;

                n = (j - 0);
                n = n * n;

                m = abs(m);
                n = abs(n);

                euclidVals[7] = m + n;
            }
            else if (currPuzzle.at(i).at(j) == 8) {
                m = (i - 2);
                m = m * m;

                n = (j - 1);
                n = n * n;

                m = abs(m);
                n = abs(n);

                euclidVals[8] = m + n;
            }
            else { /*if (currPuzzle.at(i).at(j) == 0)*/
                m = (i - 2);
                m = m * m;

                n = (j - 2);
                n = n * n;

                m = abs(m);
                n = abs(n);

                euclidVals[0] = m + n;
            }
        }
    }

    for (unsigned i = 0; i < 9; i++) {
        h_of_n += euclidVals[i];
    }
}

void Node::A_Misplaced () {
            int h_of_n_temp = 0;
            for (unsigned i = 0; i < 3; i++) {
                for (unsigned j = 0; j < 3; j++) {
                    if (currPuzzle.at(i).at(j) == solvedPuzzle.at(i).at(j)) {
                        h_of_n_temp += 1;
                    }
                }
            }
            h_of_n = 9 - h_of_n_temp;
        }
