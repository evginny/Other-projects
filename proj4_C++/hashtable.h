#include <vector>
#include <stdlib.h>
#include <list>
#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>

namespace cop4530{
    template <typename K, typename V>
    class HashTable{
        public:
            explicit HashTable(size_t size = 101);
            ~HashTable();
            size_t size() const;
            void setSize(size_t size);
            bool isEmpty() const;
            bool contains(const K& k) const;
            bool match(const std::pair<K,V>& kv) const;
            bool insert(const std::pair<K,V>& kv);
            bool insert(std::pair<K,V>&& kv);
            bool remove(const K& k);
            void clear();
            bool load(const char* filename);
            void dump() const;
            bool write_to_file(const char* filename) const;

        private:
            static const unsigned int default_capacity = 101;
            static const unsigned int max_prime = 1301081;
            unsigned long prime_below (unsigned long);
            void setPrimes(std::vector<unsigned long>& vprimes);
            std::vector<std::list<std::pair<K,V>>> theLists;
            int currentSize;
            void makeEmpty();
            void rehash();
            size_t myhash(const K& k) const;
    };
    #include "hashtable.hpp"
}