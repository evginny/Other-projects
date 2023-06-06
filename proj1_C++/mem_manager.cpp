#include "MyMemory.h"

void print_menu();
bool validate(char c);
bool Negative(int i);

int main(){
    char choice = '\0';
    int num_of_blocks = 0;
    int block_size = 0;

    cout << "\nMemory configuration:\n";
    cout << "Number of memory blocks (integer): ";
    cin >> num_of_blocks;
    while(Negative(num_of_blocks)){
        cout << "Invalid number of memory blocks.\n"
             << "Please, enter a valid number of memory blocks: ";
        cin >> num_of_blocks;
    }
    cout << "Memory block size (integer): ";
    cin >> block_size;
    while(Negative(block_size)){
        cout << "Invalid memory block size.\n"
             << "Please, enter a valid memory block size: ";
        cin >> block_size;
    }
    MyMemory memory(num_of_blocks, block_size);

    while(true){
        print_menu();
        cin >> choice;
        while(!validate(choice)){
            cout << "Invalide choice. Please, enter your choice again: ";
            cin >> choice;
        }
        if (choice == 'd'){
            memory.dump();
        }
        else if (choice == 'r'){
            int size;
            cout << "Requested memory size (integer): ";
            cin >> size;
            while(Negative(size)){
                cout << "Size cannot be negative.\n"
                     << "Requested memory size (integer) ";
                cin >> size;
            }
            int req_memory = memory.req_mem(size);
            if (req_memory == -1){
                cout << "Block with enough memory is not found\n";
            }
        }
        else if (choice == 'l'){
            int index, length;
            cout << "Release memory index (integer): ";
            cin >> index;
            cout << "Release memory length (integer): ";
            cin >> length;
            bool release = memory.release_mem(index, length);
            if (!release){
                cout << "Unable to perform release memory operation\n";
            }
        }
        else if (choice == 'm'){
            memory.merge_mem();
        }
        else if (choice == 'x'){
            break;
        }
    }

}

void print_menu(){
    cout << string(20, '=') << endl;
    cout << "d: memory dump\n"
         << "r: request memory\n"
         << "l: release memory\n"
         << "m: merge memory\n"
         << "x: quit program\n"
         << "Your choice: ";
}

bool validate(char c){
    return ((c == 'd') || (c == 'r') || (c == 'l') || (c == 'm') || (c == 'x'));
}

bool Negative(int i){
    return (i < 0);
}