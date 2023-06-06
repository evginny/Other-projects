#include "MyMemory.h"

// constructor
MyMemory::MyMemory(int num, int len) : max_index{num - 1}, block_size{len}{
    for (int i = 0; i != num; ++i){
        MyMemoryBlock block(i, len);
        mlist.push_back(block);
    }
}

// deletes all elements from the mlist
MyMemory::~MyMemory(){
    if (!mlist.empty()){
        mlist.clear();
    } 
}

void MyMemory::resize(int num, int len){
    if (!mlist.empty()){
        mlist.clear();
    }
    max_index = num - 1;
    block_size = len;
    for (int i = 0; i != num; ++i){
        MyMemoryBlock block(i, len);
        mlist.push_back(block);
    }
}

int MyMemory::req_mem(int len){
    if (mlist.empty()){
        return -1;
    }
    int index;
    List<MyMemoryBlock>::iterator itr = mlist.begin();
    while (itr != mlist.end()){
        if (itr->get_length() >= len){
            itr->allocate_mem(len);
            index = itr->get_index();
            if (itr->get_length() == 0){
                mlist.erase(itr);
            }
            return index;
        }
        ++itr;
    }
    return -1;
}

bool MyMemory::release_mem(int ind, int len){
    // returns false if ind and len are not in a valid range
    if (((ind < 0) || (ind > max_index))
        || ((len < 0) || (len > block_size))){
        return false;
    }
    // if the block with a given index exists, iterate through the mlist 
    // to find it, and add len to this block length
    // return true afterward
    List<MyMemoryBlock>::iterator itr = mlist.begin();
    while(itr != mlist.end()){
        if (itr->get_index() == ind){
            itr->free_mem(len);
            return true;
        }
        ++itr;
    }
    // if the block with a given index does not exist, create a new block 
    // with a given index and length, and insert it in a way that the indexes 
    // of the memory blocks will be in increasing order in mlist
    MyMemoryBlock new_block(ind, len);
    itr = mlist.begin();
    while (itr != mlist.end()){
        if (ind < itr->get_index()){
            mlist.insert(itr, new_block);
            return true;
        }
        ++itr;
    }
    mlist.push_back(new_block);
    return true;
}

void MyMemory::merge_mem(){
    List<MyMemoryBlock>::iterator itr = mlist.begin();
    while(itr != --mlist.end()){
        if ((itr->get_length() + (++itr)->get_length()) <= block_size){
            (--itr)->free_mem((++itr)->get_length());
            itr = mlist.erase(itr);
            --itr;
        }
    }
}

void MyMemory::dump(){
    cout << "\nNumber of blocks: " << mlist.size() << endl;
    List<MyMemoryBlock>::const_iterator itr = mlist.begin();
    while(itr != mlist.end()){
        cout << *itr;
        ++itr;
    }
}

// void MyMemory::print(ostream& os) const{
//     List<MyMemoryBlock>::const_iterator itr = mlist.begin();
//     while(itr != mlist.end()){
//         cout << *itr;
//         ++itr;
//     }
// }

// ostream& operator<<(ostream& s, const MyMemory& m){
//     m.print(s);
//     return s;
// }

