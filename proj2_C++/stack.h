#ifndef DL_LIST_H
#define DL_LIST_H
#include <iostream>
#include <vector>

namespace cop4530 {
    template<typename T>
    class Stack {
        public:
            // zero-argument constructor
            Stack();
            // destructor
            ~Stack();
            //copy constructor
            Stack(const Stack<T>& rhs);
            // move constructor
            Stack(Stack<T> &&);
            // copy assignment operator=.
            Stack<T>& operator=(const Stack<T>&);
            //move assignment operator=
            Stack<T>& operator=(Stack<T>&&);
            // returns true if the Stack contains no elements, and false otherwise.  
            bool empty() const;
            // delete all elements from the stack
            void clear();
            //  adds  x  to the Stack.   copy version.
            void push(const T& x);
            // adds x to the Stack. move version.
            void push(T && x);
            // removes and discards the most recently added element of the Stack.  
            void pop();
            //  mutator that returns a reference to the most recently added element of the Stack.  
            T& top();
            // accessor that returns the most recently added element of the Stack. 
            const T& top() const;
            // returns the number of elements stored in the Stack.
            int size() const;
            void print(std::ostream& os, char ofc = ' ') const;
        private:
            std::vector<T> vec;
    };
    template <typename T>
        std::ostream& operator<<(std::ostream& os, const Stack<T>& a);
    template <typename T>
        bool operator==(const Stack<T>&, const Stack<T>&);
    template <typename T>
        bool operator !=(const Stack<T>& a, const Stack <T>& b);
    template <typename T>
        bool operator<=(const Stack<T>& a, const Stack<T>& b);

    #include "stack.hpp"
}
#endif