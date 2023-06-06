#include "passserver.h"

void Menu();
bool isValid(char c);

int main(){
    int capacity;
    char choice;
    string filename;
    string username, password, newPassword;
    pair<string, string> user;
    bool cont = true;

    cout << "\nEnter preferred hash table capacity (integer): ";
    cin >> capacity;

    while (cin.fail()){
        cout << "Please enter an integer: ";
        cin.clear();
        cin.ignore(256,'\n');
        cin >> capacity;
    }

    while (capacity <= 0){
        capacity = 2;
    }
    PassServer server(capacity);

    while (cont){
        Menu();
        cin >> choice;

        while (!isValid(choice)){
            cout << "\nError: Invalid entry. Try again: ";
            cin >> choice;
        }

        switch (choice){
            case 'l':
                cout << "\nEnter password file name to load from: ";
                cin >> filename;
                if (!server.load(filename.c_str())){
                    cout << "*****Error: Cannot open file \'" << filename << "\'.\n";
                }
                break;

            case 'a':
                cout << "Enter username: ";
                cin >> username;
                cout << "Enter password: ";
                cin >> password;

                user.first = username;
                user.second = password;

                if (server.addUser(user)){
                    cout << "\nUser \'" << username << "\' added.\n";
                }
                else {
                    cout << "\n*****Error: User already exists. Could not add user.\n";
                }
                break;

            case 'r':
                cout << "Enter username: ";
                cin >> username;
                
                if (server.removeUser(username)){
                    cout << "User \'" << username << "\' deleted.\n";
                }
                else {
                    cout << "\n*****Error: User not found. Could not delete user.\n";
                }
                break;

            case 'c':
                cout << "Enter username: ";
                cin >> username;
                cout << "Enter current password: ";
                cin >> password;
                cout << "Enter new password: ";
                cin >> newPassword;

                user.first = username;
                user.second = password;

                if (!server.changePassword(user, newPassword)){
                    cout << "\n*****Error: Could not change user password.\n";
                }
                else {
                    cout << "Password was changed successfully.\n";
                }
                break;
            
            case 'f':
                cout << "Enter username: ";
                cin >> username;

                if (server.find(username)){
                    cout << "\nUser \'" << username << "\' found.\n";
                }
                else {
                    cout << "\nUser \'" << username << "\' not found.\n";
                }
                break;

            case 'd':
                server.dump();
                break;
            
            case 's':
                cout << "Size of hash table: " << server.size() << "\n";
                break;
            
            case 'w':
                cout << "Enter password file name to write to: ";
                cin >> filename;
                if (!server.write_to_file(filename.c_str())){
                    cout << "\nError*****: Could not write to this file.\n";
                }
                break;
            
            case 'x':
                cont = false;
                break;
        }
    }
}

void Menu(){
    cout << "\n\n"
         << "l - Load From File\n"
         << "a - Add User\n"
         << "r - Remove User\n"
         << "c - Change User Password\n"
         << "f - Find User\n"
         << "d - Dump HashTable\n"
         << "s - HashTable Size\n"
         << "w - Write to Password File\n"
         << "x - Exit Program\n"
         << "\nEnter choice: ";
}

bool isValid(char c){
    return ((c == 'l') || (c == 'a') || (c == 'r') || (c == 'c') ||
            (c == 'f') || (c == 'd') || (c == 's') || (c == 'w') || 
            (c == 'x'));
} 
