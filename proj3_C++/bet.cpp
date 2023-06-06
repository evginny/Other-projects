#include "bet.h"

BET::BET()
{
    root = new BinaryNode;
    oper1 = new BinaryNode;
    oper2 = new BinaryNode;
}

BET::BET(const string postfix)
{
    root = new BinaryNode;
    oper1 = new BinaryNode;
    oper2 = new BinaryNode;
    buildFromPostfix(postfix);
}

BET::BET(const BET &rhs)
{
    root = clone(rhs.root);
}

BET::~BET()
{
    makeEmpty(root);
}

const BET &BET::operator=(const BET &rhs)
{
    root = clone(rhs.root);
    return *this;
}

bool BET::buildFromPostfix(const string postfix)
{

    if (!empty())
    {
        makeEmpty(root);
        root = new BinaryNode();
    }

    vector<string> tokens;
    string opr = "";

    for (int i = 0; i != postfix.length(); ++i)
    {
        if (isspace(postfix[i]))
        {
            tokens.push_back(opr);
            opr = "";
        }
        else
        {
            opr.push_back(postfix[i]);
        }
        if (i == postfix.length() - 1)
        {
            tokens.push_back(opr);
        }
    }
    vector<string>::iterator itr = tokens.begin();

    while (itr != tokens.end())
    {
        if (!isOperator(*itr))
        {
            st.push(new BinaryNode(*itr));
            st.top()->isLeaf = true;
        }
        else
        {
            if (st.size() < 2)
            {
                cout << "\nPostfix expression is invalid.\n";
                return false;
            }
            st.top()->isLeft = false;
            BinaryNode *rightNode = st.top();
            st.pop();
            BinaryNode *leftNode = st.top();
            st.pop();
            st.push(new BinaryNode(*itr, leftNode, rightNode));
        }
        ++itr;
    }
    if (st.size() != 1)
    {
        cout << "\nPostfix expression is invalid.\n";
        return false;
    }
    root = st.top();
    root->isLeft = true;
    st.pop();
    return true;
}

typename BET::BinaryNode *BET::clone(const BinaryNode *t) const
{
    if (t == nullptr)
        return nullptr;
    else
        return new BinaryNode{t->data, clone(t->left), clone(t->right)};
}

bool BET::empty()
{
    return (root == nullptr);
}

void BET::printPostfixExpression()
{
    if (empty())
    {
        cout << "Empty tree\n";
    }
    else
    {
        printPostfixExpression(root);
        cout << endl;
    }
}

void BET::printPostfixExpression(BinaryNode *n)
{
    if (n != nullptr)
    {
        printPostfixExpression(n->left);
        printPostfixExpression(n->right);
        cout << n->data << " ";
    }
}

void BET::printInfixExpression()
{
    if (empty())
    {
        cout << "Empty tree\n";
    }
    else
    {
        printInfixExpression(root);
        cout << endl;
    }
}

void BET::printInfixExpression(BinaryNode *n)
{
    if (n != nullptr)
    {
        if (n == root)
        {
            // left-hand-side operator
            oper1 = root;
            printInfixExpression(root->left);
            // print out all closing parenthese in stack (if it is not empty)
            // after printing out all nodes of the left subtree
            while (!parentheses.empty())
            {
                cout << parentheses.top();
                parentheses.pop();
            }
            cout << root->data << " ";
            // left-hand-side operator changed to root operator after we done with the left subtree
            oper1 = root;
            printInfixExpression(root->right);
            // print out all closing parenthese in stack (if it is not empty)
            // after printing out all nodes of the right subtree
            while (!parentheses.empty())
            {
                cout << parentheses.top();
                parentheses.pop();
            }
        }
        else
        {
            // check if node is an operator (not operand)
            if (isOperator(n->data))
            {
                // right-hand-side operator
                oper2 = n;
                // check the if the left-hand-side operator has the higher precedence
                // than right-hand-side operator
                if (checkForParenth(oper1, oper2))
                {
                    // if yes, print out open parentheses
                    // and push close parentheses into the stack
                    cout << "( ";
                    parentheses.push(") ");
                }
                oper1 = oper2;
            }
            printInfixExpression(n->left);
            cout << n->data << " ";
            printInfixExpression(n->right);
        }
    }
}
void BET::makeEmpty(BinaryNode *&t)
{
    if (t != nullptr)
    {
        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
    }
    t = nullptr;
}

size_t BET::size()
{
    return size(root);
}

size_t BET::size(BinaryNode *t)
{
    if (t == nullptr)
    {
        return 0;
    }
    else
    {
        return (size(t->left) + 1 + size(t->right));
    }
}

size_t BET::leaf_nodes()
{
    return leaf_nodes(root);
}

size_t BET::leaf_nodes(BinaryNode *t)
{
    if (t == nullptr)
    {
        return 0;
    }
    if (t->isLeaf)
    {
        return 1;
    }
    else
    {
        return leaf_nodes(t->left) + leaf_nodes(t->right);
    }
}

// check if parentheses are needed based on the precedence
// of the left-hand-side and right-hand-side operators and
// based on wheather there are right or left children
bool BET::checkForParenth(BinaryNode *n1, BinaryNode *n2)
{

    if (((n1->isLeft && n1->data == "+") && (!n2->isLeft && n2->data == "*")) ||
        ((n1->isLeft && n1->data == "+") && (!n2->isLeft && n2->data == "/")) ||
        ((n1->isLeft && n1->data == "-") && (!n2->isLeft && n2->data == "*")) ||
        ((n1->isLeft && n1->data == "-") && (!n2->isLeft && n2->data == "/")))
    {
        return false;
    }
    else if ((!n2->isLeft) ||
             (n1->data == "*" && n2->data == "+") ||
             (n1->data == "*" && n2->data == "-") ||
             (n1->data == "/" && n2->data == "+") ||
             (n1->data == "/" && n2->data == "-"))
    {
        return true;
    }
    return false;
}
bool isOperator(string str)
{
    return ((str.compare("+") == 0) || (str.compare("-") == 0) || (str.compare("*") == 0) || (str.compare("/") == 0));
}
