#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <map>
#include <unordered_map>
using namespace std;
struct Quadruple {
    string op;
    string arg1;
    string arg2;
    string result;
};
struct DAGNode {
    string label;
    vector<int> children;
};
vector<Quadruple> generate3AddressCode(const string& expression) {
    vector<Quadruple> quadruples;
    stack<char> operators;
    stack<string> operands;
    map<char, int> precedence;
    precedence['+'] = precedence['-'] = 1;
    precedence['*'] = precedence['/'] = precedence['%'] = 2;
    precedence['^'] = 3;

    for (char c : expression) {
        if (c == '(') {
            operators.push(c);
        } else if (isdigit(c) || isalpha(c)) {
            operands.push(string(1, c));
        } else if (c == ')') {
            while (!operators.empty() && operators.top() != '(') {
                char op = operators.top();
                operators.pop();
                string arg2 = operands.top();
                operands.pop();
                string arg1 = operands.top();
                operands.pop();
                string result = "t" + to_string(quadruples.size() + 1);
                quadruples.push_back({string(1, op), arg1, arg2, result});
                operands.push(result);
            }
            operators.pop();
        } else {
            while (!operators.empty() && precedence[operators.top()] >= precedence[c]) {
                char op = operators.top();
                operators.pop();
                string arg2 = operands.top();
                operands.pop();
                string arg1 = operands.top();
                operands.pop();
                string result = "t" + to_string(quadruples.size() + 1);
                quadruples.push_back({string(1, op), arg1, arg2, result});
                operands.push(result);
            }
            operators.push(c);
        }
    }
    while (!operators.empty()) {
        char op = operators.top();
        operators.pop();
        string arg2 = operands.top();
        operands.pop();
        string arg1 = operands.top();
        operands.pop();
        string result = "t" + to_string(quadruples.size() + 1);
        quadruples.push_back({string(1, op), arg1, arg2, result});
        operands.push(result);
    }

    return quadruples;
}
vector<DAGNode> constructDAG(const vector<Quadruple>& quadruples) {
    unordered_map<string, int> nodeMap; 
    vector<DAGNode> DAG;

    for (const auto& quad : quadruples) {
        DAGNode newNode;
        newNode.label = quad.result;
        if (nodeMap.find(quad.arg1) == nodeMap.end()) {
            DAGNode arg1Node;
            arg1Node.label = quad.arg1;
            DAG.push_back(arg1Node);
            nodeMap[quad.arg1] = DAG.size() - 1;
        }
        if (nodeMap.find(quad.arg2) == nodeMap.end()) {
            DAGNode arg2Node;
            arg2Node.label = quad.arg2;
            DAG.push_back(arg2Node);
            nodeMap[quad.arg2] = DAG.size() - 1;
        }
        newNode.children.push_back(nodeMap[quad.arg1]);
        newNode.children.push_back(nodeMap[quad.arg2]);
        DAG.push_back(newNode);
        nodeMap[quad.result] = DAG.size() - 1;
    }

    return DAG;
}
void printDAG(const vector<DAGNode>& DAG) {
    cout << "Directed Acyclic Graph (DAG):\n";
    for (size_t i = 0; i < DAG.size(); ++i) {
        cout << DAG[i].label << " -> ";
        for (int child : DAG[i].children) {
            cout << DAG[child].label << " ";
        }
        cout << endl;
    }
}
int main() {
	cout<<"Enter a Expression: ";
	string expression;cin>>expression;
	cout<<"\nThree address Code: \n";	
    vector<Quadruple> quadruples = generate3AddressCode(expression);
    for (const auto& quad : quadruples) {
        cout << quad.result << " = " << quad.arg1 << " " << quad.op << " " << quad.arg2 << endl;
    }
    cout << endl;
    vector<DAGNode> DAG = constructDAG(quadruples);
    printDAG(DAG);
    return 0;
}

