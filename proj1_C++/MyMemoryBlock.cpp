#include "MyMemoryBlock.h"

MyMemoryBlock::MyMemoryBlock(int ind, int len)
                            : index{ind}, length{len} {}

MyMemoryBlock::MyMemoryBlock(const MyMemoryBlock& b) 
                            : index{b.index}, length{b.length} {}

int MyMemoryBlock::get_length() const{
    return length;
}

int MyMemoryBlock::get_index() const{
    return index;
}

int MyMemoryBlock::allocate_mem(int len){
    if (len > length){
        return -1;
    }
    else {
        length -= len;
        return len;
    }
}

void MyMemoryBlock::free_mem(int len){
    length += len;
}

ostream& operator<<(ostream& s, const MyMemoryBlock& block){
    s << "Block (index = " << block.get_index() << "): " << block.get_length() << endl;
    return s;
}