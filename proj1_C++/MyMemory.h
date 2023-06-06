#include "List.h"
#include "MyMemoryBlock.h"

using namespace cop4530;
using namespace std;

class MyMemory{
    //friend ostream& operator<<(ostream& s, const MyMemory& mem);
    public:
        MyMemory(int num = 0, int len = 0);
        ~MyMemory();
        void resize(int num = 100, int len = 100);
        int req_mem(int len);
        bool release_mem(int ind, int len);
        void merge_mem();
        //void print(ostream& os) const; 
        void dump();

    private:
        List<MyMemoryBlock> mlist;  //doubly linked list of memory blocks 
        int max_index; // max index value of memory blocks used in the system
        int block_size; // the length of memory blocks
};