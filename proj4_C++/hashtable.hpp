
using namespace cop4530;

template <typename K, typename V>
HashTable<K,V>::HashTable(size_t size) : currentSize{0}{
    setSize(size);
}

// return true if hashtable is empty, false otherwise
template <typename K, typename V>
bool HashTable<K,V>::isEmpty() const{
    return (currentSize == 0);
}

// destructor
template <typename K, typename V>
HashTable<K,V>::~HashTable(){
    clear();
}

// clear every list of theLists vector
template <typename K, typename V>
void HashTable<K,V>::makeEmpty(){
    for (auto& thisList : theLists){
        thisList.clear();
    }
    currentSize = 0;
}

template <typename K, typename V>
void HashTable<K,V>::setSize(size_t size){
  theLists.resize(prime_below(size));
}

template <typename K, typename V>
bool HashTable<K,V>::load(const char* filename){
    std::string delimiter = " ";
    std::ifstream myfile;
    std::string line;
    myfile.open(filename);
    // continue only if the file is successfully opened
    if (myfile.is_open()){
        // read a file line by line util the end of file
        while (getline(myfile, line)){
            //since the key and value of every pair are separated by whitespace
            // assign 'key' to the substring that starts from the beginning
            // of the line and ends before the whitespace.
            std::string key = line.substr(0, line.find(delimiter));
            // assign 'value' to the substring that starts from the whitespace
            // to the end of the line
            std::string value = line.substr(line.find(delimiter), line.size());
            // remove the whitespace from 'value'
            value.erase(0, 1);
            // create a new pair of key/value
            std::pair<K,V> newPair(key, value);
            // add this pair to the hashtable
            insert(newPair);
        }
        // close the file
        myfile.close();
        return true;
    }
    return false;
}


template <typename K, typename V>
bool HashTable<K,V>::write_to_file(const char* filename) const{
    std::ofstream myfile(filename);
    // continue only if a file was successfully opened
    if (myfile.is_open()){
      // write every pair of key/value from every list of the theLists vector
        for (int i = 0; i < theLists.size(); ++i){
            for (auto itr = theLists[i].begin(); itr != theLists[i].end(); ++itr){
                myfile << itr->first << " " << itr->second << "\n";
            }
        }
        myfile.close();
        return true;
    }    
    return false;
}

template <typename K, typename V>
size_t HashTable<K,V>::myhash(const K& k) const{
    static std::hash<K> hf;
    return hf(k) % theLists.size();
}

template <typename K, typename V>
bool HashTable<K,V>::contains(const K& k) const{
    // find the list based on the key
    auto& whichList = theLists[myhash(k)];
    auto itr = whichList.begin();
    // iterate through this list
    while (itr != whichList.end()){
      // if the list conatins a pair with the specified key - return true
      if (itr->first == k){
        return true;
      }
      ++itr;
    }
    return false;
}

template <typename K, typename V>
bool HashTable<K,V>::match(const std::pair<K,V>& kv) const{
    // find the list based on the key
    auto& thisList = theLists[myhash(kv.first)];
    return (find(begin(thisList), end(thisList), kv) != end(thisList));
}

// return true if the pair with the specified key is found and removed,
// return false otherwise
template <typename K, typename V>
bool HashTable<K,V>::remove(const K& k){
    auto& thisList = theLists[myhash(k)];
    auto itr = thisList.begin();
    while (itr != thisList.end()){
        if (itr->first == k){
            thisList.erase(itr);
            --currentSize;
            return true;
        }
        ++itr;
    }
    return false;
}

template <typename K, typename V>
void HashTable<K,V>::clear(){
    makeEmpty();
}

// insert the new key/value pair only if any other pair in any list of the
// theLists vector does not contain the pair with the same key
template <typename K, typename V>
bool HashTable<K,V>::insert(const std::pair<K,V>& kv){
    auto& whichList = theLists[myhash(kv.first)];
    if (find(begin(whichList), end(whichList), kv) != end(whichList)){
        return false;
    }
    whichList.push_back(kv);
    if (++currentSize > theLists.size()){
        rehash();
    }
    return true;
}

template <typename K, typename V>
bool HashTable<K,V>::insert(std::pair<K,V> && kv){
    const std::pair<K,V> newPair = std::move(kv);
    return insert(newPair);
}

template <typename K, typename V>
void HashTable<K,V>::rehash(){
    std::vector<std::list<std::pair<K,V>>> oldLists = theLists;
    theLists.resize(prime_below(2 * theLists.size()));
    for (auto& thisList : theLists){
        thisList.clear();
    }
    currentSize = 0;
    for (auto& thisList : oldLists){
      for (auto& kvpair : thisList){
        insert(kvpair);
      }
    }
}

template <typename K, typename V>
size_t HashTable<K,V>::size() const{
    return currentSize;
}

template <typename K, typename V>
void HashTable<K,V>::dump() const{
    if (currentSize == 0){
        std::cout << "Table is empty.\n";
        return;
    }
    for (int i = 0; i < theLists.size(); ++i){
        std::cout << "V[" << i << "]: ";
        auto itr = theLists[i].begin();
        while (itr != theLists[i].end()){
            if (itr != theLists[i].begin()){
                std::cout << ":";
            }
            std::cout << itr->first << " " << itr->second;
            ++itr;
        }
        std::cout << "\n";
    }
}

template <typename K, typename V>
unsigned long HashTable<K, V>::prime_below (unsigned long n)
{
  if (n > max_prime)
    {
      std::cerr << "** input too large for prime_below()\n";
      return 0;
    }
  if (n == max_prime)
    {
      return max_prime;
    }
  if (n <= 1)
    {
		std::cerr << "** input too small \n";
      return 0;
    }

  // now: 2 <= n < max_prime
  std::vector <unsigned long> v (n+1);
  setPrimes(v);
  while (n > 2)
    {
      if (v[n] == 1)
	return n;
      --n;
    }

  return 2;
}

template <typename K, typename V>
void HashTable<K, V>::setPrimes(std::vector<unsigned long>& vprimes)
{
  int i = 0;
  int j = 0;

  vprimes[0] = 0;
  vprimes[1] = 0;
  int n = vprimes.capacity();

  for (i = 2; i < n; ++i)
    vprimes[i] = 1;

  for( i = 2; i*i < n; ++i)
    {
      if (vprimes[i] == 1)
        for(j = i + i ; j < n; j += i)
          vprimes[j] = 0;
    }
}
