#include <iostream>
using namespace std;

class MyMemoryBlock{
    friend ostream& operator<<(ostream& s, const MyMemoryBlock& block);
    public:
        MyMemoryBlock(int ind = 0, int len = 0);
        MyMemoryBlock(const MyMemoryBlock& b);
        int get_length() const;
        int get_index() const;
        int allocate_mem(int len);
        void free_mem(int len);
    private:
        //a unique index (memory address)
        int index;
        //the length of the memory block
        int length;
};