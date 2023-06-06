#include "passserver.h"

PassServer::PassServer(size_t size){
    table.setSize(size);
}

PassServer::~PassServer(){
    if (!table.isEmpty()){
        table.clear();
    }
}

bool PassServer::load(const char* filename){
	cout << "check passserver 1\n";
    return table.load(filename);
}

void PassServer::dump(){
    table.dump();
}

bool PassServer::addUser(pair<string, string>& kv){
    if (!table.contains(kv.first)){
        kv.second = encrypt(kv.second);
        table.insert(kv);
        return true;
    }
    return false;
}

bool PassServer::addUser(pair<string, string>&& kv){
    if (!table.contains(kv.first)){
        kv.second = encrypt(kv.second);
        table.insert(move(kv));
        return true;
    }
    return false;
}

bool PassServer::removeUser(const string& k){
    if (table.contains(k)){
        table.remove(k);
        return true;
    }
    return false;
}

bool PassServer::find(const string& user) const{
    return table.contains(user);
}

bool PassServer::changePassword(const pair<string, string>& p, const string& newpassword){
    if (!find(p.first)){
        return false;
    }
    string oldpass = encrypt(p.second);
    string newpass = encrypt(newpassword);
    pair<string, string> oldPair(p.first, oldpass);
    if (!table.match(oldPair) || oldpass == newpass){
        return false;
    }
    pair<string, string> newPair(p.first, newpass);
    table.remove(p.first);
    table.insert(newPair);
    return true;
}

size_t PassServer::size() const{
    return table.size();
}

bool PassServer::write_to_file(const char* filename) const{
    return table.write_to_file(filename);
}

string PassServer::encrypt(const string& str){
    char salt[] = "$1$########";
    string password = crypt(str.c_str(), salt);
    password.replace(0, 12, "");
    return password;
}

