using namespace cop4530;

template <typename T>
Stack<T>::Stack() {}

template <typename T>
Stack<T>::~Stack() {}

// copy constructor
template <typename T>
Stack<T>::Stack(const Stack<T> &a)
{
    for (int i = 0; i < a.size(); ++i)
    {
        this->vec.push_back(a.vec[i]);
    }
}

// move constructor
template <typename T>
Stack<T>::Stack(Stack<T> &&a)
{
    for (int i = 0; i < a.size(); ++i)
    {
        this->vec.push_back(a.vec[std::move(i)]);
    }
}

// copy assignment operator=
template <typename T>
Stack<T> &Stack<T>::operator=(const Stack<T> &rhs)
{
    // Stack<T> copy = rhs;
    for (int i = 0; i < rhs.size(); ++i)
    {
        this->vec.push_back(rhs.vec[i]);
    }

    return *this;
}

// move assignment operator=
template <typename T>
Stack<T> &Stack<T>::operator=(Stack<T> &&rhs)
{
    for (int i = 0; i < rhs.size(); ++i)
    {
        this->vec.push_back(rhs.vec[std::move(i)]);
    }
    return *this;
}

// adds  x  to the Stack.   copy version.
template <typename T>
void Stack<T>::push(const T &x)
{
    this->vec.push_back(x);
}

// adds x to the Stack. move version.
template <typename T>
void Stack<T>::push(T &&x)
{
    this->vec.push_back(std::move(x));
}

template <typename T>
void Stack<T>::clear()
{
    if (this->size() != 0)
    {
        while (!this->empty())
        {
            this->pop();
        }
    }
}

// returns true if the Stack contains no elements, and false otherwise.
template <typename T>
bool Stack<T>::empty() const
{
    return (this->vec.empty());
}

// removes and discards the most recently added element of the Stack.
template <typename T>
void Stack<T>::pop()
{
    this->vec.pop_back();
}

//  mutator that returns a reference to the most recently added element of the Stack.
template <typename T>
T &Stack<T>::top()
{
    return (this->vec.back());
}

// accessor that returns the most recently added element of the Stack.
template <typename T>
const T &Stack<T>::top() const
{
    return (this->vec.back());
}

// returns the number of elements stored in the Stack.
template <typename T>
int Stack<T>::size() const
{
    return (this->vec.size());
}

template <typename T>
void Stack<T>::print(std::ostream &os, char ofc) const
{
    for (int i = 0; i < this->size(); ++i)
    {
        os << this->vec[i] << ofc;
    }
}

template <typename T>
std::ostream &operator<<(std::ostream &os, const Stack<T> &a)
{
    a.print(os);
    return os;
}
template <typename T>
bool operator==(const Stack<T> &a, const Stack<T> &b)
{
    if (a.size() != b.size())
    {
        return false;
    }
    Stack<T> copyA = a;
    Stack<T> copyB = b;
    while (!copyA.empty())
    {
        if (copyA.top() != copyB.top())
        {
            return false;
        }
        else
        {
            copyA.pop();
            copyB.pop();
        }
    }
    // for (int i = 0; i < a.size(); ++i){
    //     if (a.vec[i] != b.vec[i]){
    //         return false;
    //     }
    // }
    return true;
}

template <typename T>
bool operator!=(const Stack<T> &a, const Stack<T> &b)
{
    Stack<T> copyA = a;
    Stack<T> copyB = b;
    while (!copyA.empty())
    {
        if (copyA.top() == copyB.top())
        {
            return false;
        }
        else
        {
            copyA.pop();
            copyB.pop();
        }
    }
    // for (int i = 0; i < a.size(); ++i){
    //     if (a.vec[i] == b.vec[i]){
    //         return false;
    //     }
    // }
    return true;
}

template <typename T>
bool operator<=(const Stack<T> &a, const Stack<T> &b)
{
    if (a.size() > b.size())
    {
        return false;
    }
    Stack<T> copyA = a;
    Stack<T> copyB = b;

    while (!copyA.empty())
    {
        if (copyA.top() <= copyB.top())
        {
            copyA.pop();
            copyB.pop();
        }
        else
        {
            return false;
        }
    }
    return true;
}
