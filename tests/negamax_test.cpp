// This file is a driver utilized for testing the iterative negamax algorithm
#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <cmath>
#include <limits>

using namespace std;

class NegaMax {
    public:
        NegaMax(int depth, int start, vector<int> edges, queue<int> values) : depth(depth), start(start), edges(edges), values(values) {}

        bool validate_tree() {
            int out_edges;
            if ((int)edges.size() == 0) {
                return ((int)values.size() == 0);
            }
            // The first point with outgoing edges does not take away a leaf node
            out_edges = edges[0];
            for (int i = 1; i < (int)edges.size(); ++i) {
                // For every other node we add, we add edge[i] leaf nodes and take away 1 leaf node since it becomes filled
                out_edges += edges[i] - 1;
            }
            // Check to make sure the number of values is appropriate for number of values
            return (out_edges == (int)values.size());
        }

        int get_max() {
            if (!validate_tree()) {
                cout << "Tree provided was invalid!\n";

                exit(1);
            }
            // Create node tree to visit to simulate iterative depth first search:
            vector< vector<int> > node_connections;
            int curr_ind = 1;
            for (int i = 0; i < (int)edges.size(); ++i) {
                vector<int> connections = {};
                for (int j = 0; j < edges[i]; ++j) {
                    connections.push_back(curr_ind);
                    ++curr_ind;
                }
                node_connections.push_back(connections);
            }

            // Implement the iterative NegaMax alogrithm using depth first search:
            int curr_depth = 0;
            vector< vector<int> > moves;
            for (int i = 0; i < depth + 1; ++i) {
                moves.push_back({});
            }
            moves[0].push_back(0);
            
            // Implement the score path stack:
            for (int i = 0; i < depth; ++i) {
                path_scores.push((i % 2) ? numeric_limits<int>::max() : numeric_limits<int>::min());
            }
            // cout << "initial path socres:\n";
            // print_path_scores(path_scores);
            // cout << "Initial path top: " << path_scores.top() << "\n";
            // cout << max(path_scores.top(), -1) << "\n";

            // cout << "Initial path score lengths: " << path_scores.size() << "\n";

            while (true) {
                // cout << "curr depth: " << curr_depth << "\n";
                // Check if we are at max depth and return score for this
                while (curr_depth > depth || moves[curr_depth].size() == 0) {
                    // print_path_scores(path_scores);
                    // cout << "Entered undo loop\n";    
                    // Eval score is defined and we can apply negamax selection:
                    if (curr_depth > depth) {
                        // cout << "\nReached value: " << values.front() << "\n\n";
                        path_scores.push(values.front());
                        values.pop();
                    }
                    // Pop the top two element off the stack:
                    // cout << "length of path scores:" << path_scores.size() << "\n";
                    if (path_scores.size() == 1) {
                        return path_scores.top();
                    }
                    // cout << "path scores size: " << path_scores.size() << "\n";
                    int new_score = path_scores.top();
                    path_scores.pop();
                    int old_score = path_scores.top();
                    path_scores.pop();
                    // Add the calculated score back on the path scores stack
                    int mult = (curr_depth % 2) ? -1 : 1;
                    // cout << "mult: " << mult << "\n";
                    path_scores.push(mult * max(mult * old_score, mult * new_score));
                    --curr_depth;
                    // cout << "Left undo loop\n";
                }
                // Add remaining placeholders to path scores until filled:
                // cout << "current depth after while: " << curr_depth << "\n";
                // cout << "current path size: " << path_scores.size() << "\n";
                for (int i = path_scores.size(); i < depth; ++i) {
                    path_scores.push((i % 2) ? numeric_limits<int>::max() : numeric_limits<int>::min());
                }
                
                // Take the first element off the curr_move level:
                int next_move = moves[curr_depth][0];
                moves[curr_depth].erase(moves[curr_depth].begin());
                // Add the connected nodes from this node to the next level
                ++curr_depth;
                // cout << "curr depth before adding: " << curr_depth << "\n"; 
                // cout << "before adding moves size = " << moves[curr_depth].size() << "\n";
                if (curr_depth <= depth) {
                    for (int i = 0; i < (int)node_connections[next_move].size(); ++i) {
                        // cout << "\nadding connection: " << node_connections[next_move][i] << "\n\n";
                        moves[curr_depth].push_back(node_connections[next_move][i]);
                    }
                }
                // cout << "after adding moves size = " << moves[curr_depth].size() << "\n";
            }
        }

        void print_path_scores(stack<int> path_scores) {
            vector<int> scores = {};
            cout << "current path scores: ";
            while (path_scores.size()) {
                scores.push_back(path_scores.top());
                cout << path_scores.top() << " ";
                path_scores.pop();
            }
            cout << "\n";
        }

    private:
        int depth;
        int start;
        vector<int> edges;
        queue<int> values;
        stack<int> path_scores;
};

int main() {
    // Run tests to test the negamax iterative algorithm
    // Test 1
    /*
                            .  
                          /   \
                        .      .
                      /  \    / \
                    10   20  15  30

                    Correct Answer: 15
    */
    vector<int> edges1 = {2, 2, 2};
    vector<int> values1_vec = {10, 20, 15, 30};
    queue<int> values1;
    for (auto ele: values1_vec) {
        values1.push(ele);
    }
    NegaMax negamax1 = NegaMax(2, 0, edges1, values1);
    int ret_val1 = negamax1.get_max();
    if (ret_val1 != 15) {
        cout << "Failed test 1\nExpected: 15\nReceived: " << ret_val1 << "\n";
    }
    else {
        cout << "Passsed test 1!\n";
    }

    // Test 2
    /*
                                    . 
                                  /  \
                                /     \
                              /        \ 
                             .          .
                           / | \      / | \
                          /  |  \    /  |  \
                        10 20 -10  30 100  -20
    */
    vector<int> edges2 = {2, 3, 3};
    vector<int> values2_vec = {10, 20, -10, 30, 100, -20};
    queue<int> values2;
    for (auto ele: values2_vec) {
        values2.push(ele);
    }
    NegaMax negamax2 = NegaMax(2, 0, edges2, values2);
    int ret_val2 = negamax2.get_max();
    if (ret_val2 != -10) {
        cout << "Failed test 2\nExpected: -10\nReceived: " << ret_val2 << "\n";
    }
    else {
        cout << "Passsed test 2!\n";
    }
    // Test 3:
    /*
                                    . 
                                  /  \
                                /     \
                              /        \ 
                            /           \
                          /              \
                        /                 \
                      /                    \
                    .                       .
                /   |   \               /   |   \
              /    |     \            /    |     \ 
            /     |       \          /     |       \
           .      .        .        .       .        .
        / | \   / | \    / | \    / | \   / | \    / | \   \
       5  4 -1 3  2  1  0  -1 -2  3 4 7  9  8 2   -1 -3 -4 -8
    */

    vector<int> edges3 = {2, 3, 3, 3, 3, 3, 3, 3, 4};
    vector<int> values3_vec = {5, 4, -1, 3, 2, 1, 0, -1, -2, 3, 4, 7, 9, 8, 2, -1, -3, -4, -8};
    queue<int> values3;
    for (auto ele: values3_vec) {
        values3.push(ele);
    }
    NegaMax negamax3 = NegaMax(3, 0, edges3, values3);
    int ret_val3 = negamax3.get_max();
    if (ret_val3 != 0) {
        cout << "Failed test 3\nExpected: 0\nReceived: " << ret_val3 << "\n";
    }
    else {
        cout << "Passsed test 3!\n";
    }
}