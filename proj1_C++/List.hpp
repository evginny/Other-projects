
using namespace cop4530;

template <typename T>
List<T>::const_iterator::const_iterator() : current{nullptr} {}

template <typename T>
const T &List<T>::const_iterator::operator*() const
{
    return retrieve();
}

// check if it is actually correct
template <typename T>
const T *List<T>::const_iterator::operator->() const
{
    return &(retrieve());
}

template <typename T>
typename List<T>::const_iterator &List<T>::const_iterator::operator++()
{
    current = current->next;
    return *this;
}

template <typename T>
typename List<T>::const_iterator List<T>::const_iterator::operator++(int)
{
    const_iterator old = *this;
    ++(*this);
    return old;
}

template <typename T>
typename List<T>::const_iterator &List<T>::const_iterator::operator--()
{
    current = current->prev;
    return *this;
}

template <typename T>
typename List<T>::const_iterator List<T>::const_iterator::operator--(int)
{
    const_iterator old = *this;
    --(*this);
    return old;
}

template <typename T>
bool List<T>::const_iterator::operator==(const const_iterator &rhs) const
{
    return current == rhs.current;
}

template <typename T>
bool List<T>::const_iterator::operator!=(const const_iterator &rhs) const
{
    return !(*this == rhs);
}

// protected
template <typename T>
T &List<T>::const_iterator::retrieve() const
{
    return current->data;
}

template <typename T>
List<T>::const_iterator::const_iterator(Node *p) : current{p} {}

// iterator nested class
template <typename T>
List<T>::iterator::iterator() {}

template <typename T>
T &List<T>::iterator::operator*()
{
    return const_iterator::retrieve();
}

template <typename T>
const T &List<T>::iterator::operator*() const
{
    return const_iterator::operator*();
}

template <typename T>
T *List<T>::iterator::operator->()
{
    return &(const_iterator::retrieve());
}

template <typename T>
const T *List<T>::iterator::operator->() const
{
    return const_iterator::operator->();
}

template <typename T>
typename List<T>::iterator &List<T>::iterator::operator++()
{
    this->current = this->current->next;
    return *this;
}

template <typename T>
typename List<T>::iterator List<T>::iterator::operator++(int)
{
    iterator old = *this;
    ++(*this);
    return old;
}

template <typename T>
typename List<T>::iterator &List<T>::iterator::operator--()
{
    this->current = this->current->prev;
    return *this;
}

template <typename T>
typename List<T>::iterator List<T>::iterator::operator--(int)
{
    iterator old = *this;
    --(*this);
    return old;
}

// protected
template <typename T>
List<T>::iterator::iterator(Node *p) : const_iterator{p} {}

// List<T> class
// Default zero-parameter constructor
template <typename T>
List<T>::List()
{
    init();
}

// Copy constructor
template <typename T>
List<T>::List(const List &rhs)
{
    init();
    for (auto &x : rhs)
        push_back(x);
}

// move constructor
template <typename T>
List<T>::List(List &&rhs)
    : theSize{rhs.theSize}, head{rhs.head}, tail{rhs.tail}
{
    rhs.theSize = 0;
    rhs.head = nullptr;
    rhs.tail = nullptr;
}

// Construct a list with "num" number of elements,
// all initialized with value val
template <typename T>
List<T>::List(int num, const T &val)
{
    init();
    for (int i = 0; i != num; ++i)
    {
        push_back(val);
    }
}

// construct a List with elements from another list between start
// and end. Including the element referred to by the start iterator,
// but not the end iterator, that is, the elements in the range [start, end).
// List(const_iterator start, const_iterator end);
template <typename T>
List<T>::List(const_iterator start, const_iterator end)
{
    init();
    List<T>::const_iterator itr = start;
    while (itr != end)
    {
        push_back(itr.current->data);
        ++itr;
    }
}

// destructor
template <typename T>
List<T>::~List()
{
    clear();
    delete head;
    delete tail;
}

// copy assignment operator
template <typename T>
const List &List<T>::operator=(const List<T> &rhs)
{
    <T> List<T> copy = rhs;
    swap(*this, copy);
    return *this;
}

// move assignment operator
// List & operator=(List && rhs);
template <typename T>
List<T> &List<T>::operator=(List<T> &&rhs)
{
    std::swap(theSize, rhs.theSize);
    std::swap(head, rhs.head);
    std::swap(tail, rhs.tail);
    return *this;
}

// return the number of elements in the List.
template <typename T>
int List<T>::size() const
{
    return theSize;
}

// return true if no element is in the list; otherwise, return false.
template <typename T>
bool List<T>::empty() const
{
    return size() == 0;
}

// reverse the order of the elements in the list. That is,
// the original first element becomes the last,
// while the original last element becomes the first.
template <typename T>
void List<T>::reverse()
{
    if (head == nullptr)
    {
        return;
    }
    Node *temp = head;
    head = tail;
    tail = temp;
    Node *tempHead = head;
    while (tempHead != nullptr)
    {
        temp = tempHead->prev;
        tempHead->prev = tempHead->next;
        tempHead->next = temp;
        tempHead = tempHead->next;
    }
}
/*
template<typename T>
void List<T>::reverse(){
    if (head == nullptr){                   -> O(1)
        return;                             -> O(1)
    }
    Node *temp = head;                      -> O(1)
    head = tail;                            -> O(1)
    tail = temp;                            -> O(1)
    Node *tempHead = head;                  -> O(1)
    while(tempHead != nullptr){           { -> O(1)
        temp = tempHead->prev;              -> O(1)
        tempHead->prev = tempHead->next;    -> O(1)
        tempHead->next = temp;              -> O(1)
        tempHead = tempHead->next;          -> O(1) }
    }
}
The worst time complexity of reverse function:
T = 4 x O(1) + N x (5 x O(1)) =
  = c1 + N x c2 = N x c2 = O(N)



bool MyMemory::release_mem(int ind, int len){
    if (((ind < 0) || (ind > max_index))
        || ((len < 0) || (len > block_size))){          -> O(1)
        return false;
    }
    List<MyMemoryBlock>::iterator itr = mlist.begin();  -> O(1)
    while(itr != mlist.end()){                        { -> O(1)
        if (itr->get_index() == ind){                   -> O(1)
            itr->free_mem(len);
            return true;
        }
        ++itr;                                          -> O(1) }
    }
    MyMemoryBlock new_block(ind, len);                  -> O(1)
    itr = mlist.begin();                                -> O(1)
    while (itr != mlist.end()){                       { -> O(1)
        if (ind < itr->get_index()){                    -> O(1)
            mlist.insert(itr, new_block);
            return true;
        }
        ++itr;                                          -> O(1) }
    }
    mlist.push_back(new_block);                         -> O(1)
    return true;                                        -> O(1)
}

The worst time complexity of release_mem function:
T = 2 x O(1) + N x (3 x O(1)) + 2 x O(1) + N x (3 x O(1)) + 2 x O(1) =
  = c1 + (N x c2) + c3 + (N x c4) + c5 =
  = c6 + 2(N x c7) = 2N x c8 = 2N = O(N)

*/

// return reference to the first and last element in the list, respectively.
//  T &front(); // reference to the first element
//  const T& front() const;
//  T &back(); // reference to the last element
//  const T & back() const;
template <typename T>
T &List<T>::front()
{
    return *begin();
}

template <typename T>
const T &List<T>::front() const
{
    return *begin();
}

template <typename T>
T &List<T>::back()
{
    return *--end();
}

template <typename T>
const T &List<T>::back() const
{
    return *--end();
}

// void push_front(const T & val);
//  insert to the beginning
template <typename T>
void List<T>::push_front(const T &val)
{
    insert(begin(), val);
}
// void push_front(T && val);
//  move version of insert
template <typename T>
void List<T>::push_front(T &&val)
{
    insert(begin(), std::move(val));
}

// void push_back(const T & val);
//  insert to the end
template <typename T>
void List<T>::push_back(const T &val)
{
    insert(end(), val);
}
// void push_back(T && val);
//  move version of insert
template <typename T>
void List<T>::push_back(T &&val)
{
    insert(end(), std::move(val));
}

// void pop_front();
//  delete first element
template <typename T>
void List<T>::pop_front()
{
    erase(begin());
}
// void pop_back();
//  delete last element
template <typename T>
void List<T>::pop_back()
{
    erase(--end());
}

// void remove(const T &val);
//  remove all elements with value = val
template <typename T>
void List<T>::remove(const T &val)
{
    List<T>::iterator itr = begin();
    while (itr != end())
    {
        if (itr.current->data == val)
        {
            itr = --erase(itr);
            //--itr;
        }
        ++itr;
    }
}
// print out all elements. ofc is deliminitor
template <typename T>
void print(std::ostream &os, char ofc) const
{
    List<T> ist<T>:: ::const_iterator itr = begin();
    while (itr != end())
    {
        os << itr.current->data << ofc;
        ++itr;
    }
}
// iterator begin();
//  iterator to first element
template <typename T>
typename List<T>::iterator List<T>::begin()
{
    return iterator(head->next);
}

// const_iterator begin() const;
template <typename T>
typename List<T>::const_iterator List<T>::begin() const
{
    return const_iterator(head->next);
}

// iterator end();
//  end marker iterator
template <typename T>
typename List<T>::iterator List<T>::end()
{
    return iterator(tail);
}

// const_iterator end() const;
template <typename T>
typename List<T>::const_iterator List<T>::end() const
{
    return const_iterator(tail);
}

// iterator insert(iterator itr, const T& val);
//  insert val ahead of itr
template <typename T>
typename List<T>::iterator List<T>::insert(iterator itr, const T &val)
{
    Node *p = itr.current;
    ++theSize;
    return (p->prev = p->prev->next = new Node{val, p->prev, p});
}

// iterator insert(iterator itr, T && val);
//  move version of insert
template <typename T>
typename List<T>::iterator List<T>::insert(iterator itr, T &&val)
{
    Node *p = itr.current;
    ++theSize;
    return iterator(p->prev = p->prev->next = new Node{std::move(val), p->prev, p});
}

// iterator erase(iterator itr);
//  erase one element
template <typename T>
typename List<T>::iterator List<T>::erase(iterator itr)
{
    Node *p = itr.current;
    iterator retVal(p->next);
    p->prev->next = p->next;
    p->next->prev = p->prev;
    delete p;
    --theSize;
    return retVal;
}

// iterator erase(iterator start, iterator end);
//  erase [start, end)
template <typename T>
typename List<T>::iterator List<T>::erase(iterator start, iterator end)
{
    for (iterator itr = start; itr != end;)
    {
        itr = erase(itr);
    }
    return end;
}

// delete all elements
template <typename T>
void List<T>::clear()
{
    while (!empty())
    {
        pop_front();
    }
}

// private:
// initialize the member variables of list
template <typename T>
void List<T>::init()
{
    theSize = 0;
    head = new Node;
    tail = new Node;
    head->next = tail;
    tail->prev = head;
}

template <typename T>
bool operator==(const List<T> &lhs, const List<T> &rhs)
{
    if (lhs.size() != rhs.size())
    {
        return false;
    }
    else
    {
        typename List<T>::const_iterator itrL = lhs.begin();
        typename List<T>::const_iterator itrR = rhs.begin();
        while (itrL != lhs.end())
        {
            if (*itrL != *itrR)
            {
                return false;
            }
            ++itrL;
            ++itrR;
        }
        return true;
    }
}

template <typename T>
bool operator!=(const List<T> &lhs, const List<T> &rhs)
{
    return !(lhs == rhs);
}

template <typename T>
std::ostream &operator<<(std::ostream &os, const List<T> &l)
{
    l.print(os);
    return os;
}
