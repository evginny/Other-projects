#include "stack.h"
#include <iostream>
#include <string>

using namespace cop4530;
using namespace std;

bool isOperator(string str);
int findPrecedence(string str);
void infixToPostfix(string &expression, bool &isFirst, string &opr,
                    string &preOpr, bool &variableFound, string &postfix,
                    Stack<string> &stack, bool &isError,
                    bool &openParenthNotFound);
void postfixEvaluation(string &opr, bool &variableFound, Stack<string> &stack,
                       bool &isError, string &postfix, bool &isEvalError);

int main()
{
    Stack<string> stack;
    while (!cin.eof())
    {
        string expression;
        string postfix = "";
        string opr = "";
        string preOpr = "";
        bool canEvaluate = false;
        bool isFirst = true;
        bool isError = false;
        bool openParenthNotFound = true;
        bool variableFound = false;
        bool isEvalError = false;

        cout << "\nEnter infix expression (\"exit\" to quit): ";
        getline(cin, expression);

        if (expression.compare("exit") == 0)
        {
            return (EXIT_SUCCESS);
        }

        infixToPostfix(expression, isFirst, opr, preOpr, variableFound, postfix,
                       stack, isError, openParenthNotFound);

        postfixEvaluation(opr, variableFound, stack, isError, postfix, isEvalError);
    }
}

void postfixEvaluation(string &opr, bool &variableFound, Stack<string> &stack,
                       bool &isError, string &postfix, bool &isEvalError)
{
    if (!variableFound && stack.empty() && !isError)
    {
        opr = "";

        for (int i = 0; i < postfix.length(); ++i)
        {
            if (isspace(postfix[i]))
            {
                break;
            }
            while (!isspace(postfix[i]))
            {
                opr.push_back(postfix[i]);
                ++i;
                if (i == postfix.length())
                {
                    break;
                }
            }
            if (!isOperator(opr))
            {
                stack.push(opr);
            }
            else
            {
                if (stack.empty())
                {
                    isEvalError = true;
                    cout << "\nERROR: no more operands in the stack\n";
                    break;
                }
                double operand2 = stod(stack.top());
                stack.pop();
                if (stack.empty())
                {
                    isEvalError = true;
                    break;
                }
                double operand1 = stod(stack.top());
                double result = 0.0;
                stack.pop();
                if (opr.compare("+") == 0)
                {
                    result = operand1 + operand2;
                }
                else if (opr.compare("-") == 0)
                {
                    result = operand1 - operand2;
                }
                else if (opr.compare("*") == 0)
                {
                    result = operand1 * operand2;
                }
                else if (opr.compare("/") == 0)
                {
                    result = operand1 / operand2;
                }
                if (result == static_cast<int>(result))
                {
                    int resultInt = result;
                    stack.push(std::to_string(resultInt));
                }
                else
                {
                    stack.push(std::to_string(result));
                }
            }
            opr = "";
        }
        string res = "";
        if (stack.empty())
        {
            res = "0";
        }
        else
        {
            res = stack.top();
            stack.pop();
        }
        if (!stack.empty())
        {
            cout << "\nERROR: more that one operands left in the stack\n";
            isEvalError = true;
        }

        if (!isError && !isEvalError)
        {
            cout << "Postfix Evaluation: " << postfix << "= " << res << endl;
        }
    }
    else
    {
        if (!isError)
            cout << "Postfix Evaluation: " << postfix << "= " << postfix << endl;
    }
}

void infixToPostfix(string &expression, bool &isFirst, string &opr,
                    string &preOpr, bool &variableFound, string &postfix,
                    Stack<string> &stack, bool &isError,
                    bool &openParenthNotFound)
{

    bool skip = false;
    for (int i = 0; i < expression.length(); ++i)
    {
        if (i == 0)
        {
            isFirst = true;
        }
        else
        {
            isFirst = false;
        }

        if (!isspace(expression[i]))
        {
            opr.push_back(expression[i]);
            skip = true;
            if (i == expression.length() - 1)
            {
                skip = false;
            }
        }
        else
        {
            skip = false;
        }

        if (isFirst)
        {
            isFirst = false;
            preOpr = opr;
        }
        if (!skip)
        {
            if (!isOperator(opr))
            {
                if (!variableFound)
                {
                    for (int j = 0; j < opr.length(); ++j)
                    {
                        if (isalpha(opr[j]))
                        {
                            variableFound = true;
                        }
                    }
                }
                postfix.append(opr);
                postfix.append(" ");
            }
            if (isOperator(opr))
            {
                if (opr.compare("(") == 0)
                {
                    stack.push(opr);
                }
                else if (opr.compare(")") == 0)
                {
                    if (isOperator(preOpr))
                    {
                        isError = true;
                        cout << "\nERROR: the last input befor \")\" was an operator\n";
                        break;
                    }
                    while (!stack.empty())
                    {
                        if (stack.top() == "(")
                        {
                            openParenthNotFound = false;
                            stack.pop();
                            break;
                        }
                        else
                        {
                            postfix.append(stack.top());
                            postfix.append(" ");
                            stack.pop();
                        }
                    }
                    if (openParenthNotFound)
                    {
                        cout << "\nERROR: stack is emptied without a \"(\" being found\n";
                        isError = true;
                        break;
                    }
                }
                else
                {
                    int precedence = findPrecedence(opr);
                    if (stack.empty())
                    {
                        stack.push(opr);
                    }
                    else
                    {
                        if (findPrecedence(stack.top()) < precedence)
                        {
                            stack.push(opr);
                        }
                        else
                        {
                            while (!stack.empty())
                            {
                                if (stack.top() == "(")
                                {
                                    break;
                                }
                                else if (findPrecedence(stack.top()) < precedence)
                                {
                                    break;
                                }
                                else
                                {
                                    postfix.append(stack.top());
                                    postfix.append(" ");
                                    stack.pop();
                                }
                            }
                            stack.push(opr);
                        }
                    }
                }
            }
            if (!isFirst)
            {
                preOpr = opr;
            }
            opr = "";
        }
    }
    if ((preOpr.compare("(") == 0) && (!isError))
    {
        cout << "\nERROR: the last input is a \"(\" operator\n";
        isError = true;
    }
    while (!stack.empty())
    {
        if ((stack.top().compare("(")) == 0 && (!isError))
        {
            cout << "\nERROR: extra operator \"(\" is encountered\n";
            isError = true;
            break;
        }
        postfix.append(stack.top());
        postfix.append(" ");
        stack.pop();
        if (stack.empty())
        {
            break;
        }
    }
    if (!isError)
    {
        cout << "\nPostfix expression: " << postfix << endl;
    }
}

bool isOperator(string str)
{
    return ((str.compare("+") == 0) || (str.compare("-") == 0) || (str.compare("*") == 0) || (str.compare("/") == 0) || (str.compare("(") == 0) || (str.compare(")") == 0));
}
int findPrecedence(string str)
{
    if ((str.compare("+") == 0) || str.compare("-") == 0)
    {
        return 1;
    }
    else if ((str.compare("*") == 0) || str.compare("/") == 0)
    {
        return 2;
    }
    else if (str.compare("(") == 0)
    {
        return 3;
    }
    return -1;
}

string opr = "";
string postfix = "";
for (int i = 0; i < infix.lenght(); ++i)
{
    if (!isspace(infix[i]))
    {
        char letter = infix[i];
        // opt + letter;
    }
    else
    {
        if (isOperator(opr))
        {
            // if stack is empty - add to stack
            // else check precedence:
            // if opr <= stack_top - output everything from the stack and add opr to the stack
            // postfix + opr instead of outputing
            // else add opr to stack
        }
        else
        {
            //  postfix + opr instead of outputing
        }
    }
}