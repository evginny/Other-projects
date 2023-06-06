#ifndef DL_LIST_H
#define DL_LIST_H
#include <string>
#include <vector>
#include <iostream>
#include <stack>
using namespace std;

class BET{
    public:
        //default zero-parameter constructor
        BET();
        //one-parameter constructor, 
        //where parameter "postfix" is string containing a postfix expression. 
        //The tree should be built based on the postfix expression. 
        //Tokens in the postfix expression are separated by space.
        BET(const string postfix);
        //copy constructor
        BET(const BET& rhs);
        //destructor
        ~BET();
        // assignment operator.
        const BET & operator= (const BET & rhs);
        bool buildFromPostfix(const string postfix);
        //return true if the tree is empty. Return false otherwise.
        bool empty(); 
        // call the private version of the printInfixExpression function to print out the infix expression.
        void printInfixExpression();
        // call the private version of the printPostfixExpression function to print out the postfix expression.
        void printPostfixExpression();
        // call the  private version of the size function to return the number of nodes in the tree.
        size_t size();
        // call the private version of the leaf_nodes function to return the number of leaf nodes in the tree.
        size_t leaf_nodes();
    private:
        struct BinaryNode {
            string data;
            BinaryNode* left;
            BinaryNode* right;
            bool isLeft;
            bool isLeaf;

            BinaryNode() : left{nullptr}, right{nullptr} , isLeft{true}, isLeaf{false}{}

            BinaryNode(const string& inputData, BinaryNode *lt = nullptr, BinaryNode *rt = nullptr)
                        : data{inputData}, left{lt}, right{rt}, isLeft{true}, isLeaf{false} {}

            BinaryNode(string& inputData, BinaryNode* lt = nullptr, BinaryNode* rt = nullptr)
                        : data{move(inputData)}, left{lt}, right{rt}, isLeft{true}, isLeaf{false} {}
        };
        
        BinaryNode* root;
        stack<BinaryNode*> st;    
        stack<string> parentheses;
        BinaryNode* oper1;
        BinaryNode* oper2;

        BinaryNode* clone(const BinaryNode *t) const;
        // print to the standard output the corresponding postfix expression.
        void printPostfixExpression(BinaryNode *n);
        // print to the standard output the corresponding infix expression.
        void printInfixExpression(BinaryNode *n); 
        //delete all nodes in the subtree pointed to by t. Called by functions such as the destructor.
        void makeEmpty(BinaryNode* &t);

        bool checkForParenth(BinaryNode* n1, BinaryNode* n2);
        // return the number of nodes in the subtree pointed to by t.
        size_t size(BinaryNode *t);
        // return the number of leaf nodes in the subtree pointed to by t.
        size_t leaf_nodes(BinaryNode *t);
};

bool isOperator(string str);
bool checkPresedence(string str1, string str2);
#endif