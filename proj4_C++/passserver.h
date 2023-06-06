
#include "hashtable.h"
#include <iostream>
#include <string>
#include <cstring>
#include <crypt.h>
#include <utility>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>

using namespace std;
using namespace cop4530;

class PassServer : public HashTable<string, string>
{
public:
    PassServer(size_t size = 101);
    ~PassServer();
    bool load(const char *filename);
    void dump();
    bool addUser(pair<string, string> &kv);
    bool addUser(pair<string, string> &&kv);
    bool removeUser(const string &k);
    bool find(const string &user) const;
    bool changePassword(const pair<string, string> &p, const string &newpassword);
    size_t size() const;
    bool write_to_file(const char *filename) const;

private:
    HashTable<string, string> table;
    string encrypt(const string &str);
};
