#include <iostream>
#include <string>
#include <vector>
#include <cmath>
using namespace std;
//------------------------------------------------------------ Hashing Techniques------------------------------------
//division method h(k) = k mod(n)
//k->item n->table size
int divisionMethod(int k, int n){
    return k % n ;
}
//Multiplication Method h(k) = floor( n( kA mod 1 ) )
//
int multiplicationMethod(int k, int n){
    const double A = 0.618033;
    double internalPart = fmod(k*A, 1);
    return ::floor(n*internalPart);
}
//Mid-Square Method
//Steps: Square the key. Extract the middle digits of the squared value.
int midSquareMethod(int k, int n){
    int square = k*k;
    string str = to_string(square);
    int len = str.length();
    string nStr = to_string(n);
    int nLen = nStr.length();
    int beg = (len-nLen) / 2;
    string midStr = str.substr(beg, nLen);
    return stoi(midStr) % n;
}

//Folding Method.
//Steps:Divide the key into parts. Sum the parts. Take the modulo 𝑚m of the sum.
//Notice that the partition depends on the address size
int foldingMethod(int k, int n){
    int partsSum = 0;
    string str = to_string(k);
    for (int i = 0; i < str.size(); i+=2) {
        string part = str.substr(i ,2);
        partsSum += stoi(part);
    }
    return partsSum % n;
}

//------------------------------------------------------------Collision Resolution Techniques----------------------------
//Chaining Method
class chainMethod{
  vector<vector<int>>hashTable;
  int hashtableSize;
  //constructor to initialize size
public:
    chainMethod(int size){
      this->hashtableSize = size;
      hashTable.resize(size);
  }
  //function to insert the value into table
  void insert(int k){
      //call a hash function to identify index of the value
      int index = divisionMethod(k, hashtableSize);
      hashTable[index].push_back(k);
  }
  //function for displaying items in table
  void displayChainHashTable(){
      for (int i = 0; i < hashtableSize; ++i) {
          cout << i << ":";
          for (int j : hashTable[i]) {
              cout << j << " ";
          }
          cout << "\n";
      }
  }
};

// Open Addressing Method.
struct hashNode{
    int k;
    int value;
};
//A hash collision is resolved by probing with:
// Linear Probing h(k,i) = (h′(k) + f(i) ) mod m
class linearProbing{
    int hashTableSize;
    int size = 0;
    struct hashNode** table;
public:
    linearProbing(int hashSize){
        this->hashTableSize = hashSize;
        table = new hashNode*[hashTableSize];
        for (int i = 0; i < hashTableSize; ++i) {
            table[i] = NULL;
        }
    }
    //key and value
    void insert(int k, int i){
        struct hashNode* tempNode = new hashNode;
        //create new node with the key and value sent
        tempNode->k = k;
        tempNode->value = i;
        //get hash index
        int index = divisionMethod(k, hashTableSize);
        //we linearly probe for the next bucket We keep probing until an empty bucket is found.
        while (table[index] != NULL && table[index]->k != k && table[index]->k != -1){
            index++;
            index %= hashTableSize;
        }
        //insert node and increment size by one
        if (table[index] == NULL || table[index]->k == -1){
            size++;
        }
        table[index] = tempNode;
    }
    void displayLinearProbing(){
        for (int i = 0; i < hashTableSize; ++i) {
            if (table[i] != NULL && table[i]->k != -1){
                cout << "Index: " << i << " key: " << table[i]->k  << " value: " << table[i]->value << "\n";

            }
            else {
                cout << "Index: empty\n";
            }
        }
    }
};
//Quadratic Probing h(k,i) = (h′(k) + f(i) ) mod m
class quadraticProbing{
    int hashTableSize;
    int size = 0;
    struct hashNode** table;
public:
    quadraticProbing(int hashSize){
        this->hashTableSize = hashSize;
        table = new hashNode*[hashTableSize];
        for (int i = 0; i < hashTableSize; ++i) {
            table[i] = NULL;
        }
    }

    void insert(int k, int i){
        struct hashNode* tempNode = new hashNode;
        //create new node with the key and value sent
        tempNode->k = k;
        tempNode->value = i;
        //get hash index
        int index = divisionMethod(k, hashTableSize);
        int j =1;
        //we probe for 𝑖^2 th bucket in 𝑖^𝑡ℎ iteration. We keep probing until an empty bucket is found.
        while (table[index] != NULL && table[index]->k != k && table[index]->k != -1){
            index = index + (j*j);
            index %= hashTableSize;
            j++;
        }
        //insert node and increment size by one
        if (table[index] == NULL || table[index]->k == -1){
            size++;
        }
        table[index] = tempNode;
    }
    void displayQuadraticProbing(){
        for (int i = 0; i < hashTableSize; ++i) {
            if (table[i] != NULL && table[i]->k != -1){
                cout << "Index: " << i << " key: " << table[i]->k  << " value: " << table[i]->value << "\n";

            }
            else {
                cout << "Index: empty\n";
            }
        }
    }
};
//Double Hashing

bool isPrime(int num){
    if (num <= 1)
        return false;
    if (num <= 3)
        return true;
    if (num % 2 == 0 || num % 3 == 0)
        return false;
    for (int i = 5; i*i < num; i+=6) {
        if (num%i == 0 || num%(i+2) == 0)
            return false;
    }
    return true;
}
class doubleHashing{
    vector<int>hashTable;
    int hashTableSize;
    int prime;
    static int getPrimeNumber(int hashSize){
        for (int i = hashSize-1; i >= 2; --i) {
            if (isPrime(i))
                return i;
        }
        return -1;
    }
public:
    doubleHashing(int size){
        this->hashTableSize = size;
        prime = getPrimeNumber(hashTableSize);
        hashTable.resize(hashTableSize);
        for (int i = 0; i < hashTableSize; ++i) {
            hashTable[i] = -1;
        }
    }

    int hashMethod1(int value){
        return divisionMethod(value, hashTableSize);
    }

    int hashMethod2(int value){
        return prime - (value % prime);
    }


    void insert(int value){
        int h1 = hashMethod1(value);
        int h2 = hashMethod2(value);
        while (hashTable[h1] != -1){
            h1 = (h1+h2) % hashTableSize;
        }
        hashTable[h1] = value;
    }

    void displayDoubleHashing(){
        for (int i = 0; i < hashTableSize; ++i) {
            if (hashTable[i] != -1)
                cout << "index: " << i << " value: " << hashTable[i] << "\n";
            else
                cout << "empty!\n";
        }
    }

};

int main() {
//    cout << multiplicationMethod(150, 100) << "\n";
//    cout << midSquareMethod(1234, 1000) << "\n";
//    cout << foldingMethod(12345678, 100);
//    chainMethod chain(5);
//    chain.insert(5);
//    chain.insert(5);
//    chain.insert(5);
//    chain.insert(4);
//    chain.insert(2);
//    chain.displayChainHashTable();

//    linearProbing lp(20);
//
//    lp.insert(1, 5);
//    lp.insert(2, 15);
//    lp.insert(3, 20);
//    lp.insert(4, 7);
//    std::cout << "Hash Table Contents:" << std::endl;
//    lp.displayLinearProbing();
//    quadraticProbing qp(20);
//
//    qp.insert(1, 5);
//    qp.insert(2, 15);
//    qp.insert(3, 20);
//    qp.insert(4, 7);
//
//    std::cout << "Hash Table Contents:" << std::endl;
//    qp.displayQuadraticProbing();


    doubleHashing dh(13);

    dh.insert(115);
    dh.insert(12);
    dh.insert(87);
    dh.insert(66);
    dh.insert(123);

    cout << "Hash Table Contents:" << endl;
    dh.displayDoubleHashing();

    return 0;
}
