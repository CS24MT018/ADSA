#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <unordered_map>
#include <fstream>
#include <algorithm>

using namespace std;

// Hash function to compute hash value for a word
int hashFunction(const string& word, int M) {
    int sum = 0;
    for (char c : word) {
        sum += static_cast<int>(c); // Convert char c into ASCII value
    }
    return sum % M;
}

string toLowercase(const string& s) {
    string result = s;
    for (char& c : result) {
        c = tolower(c);
    }
    return result;
}

// Function to get sorted word
string getSortedWord(const string& word) {
    string sortedWord = toLowercase(word);
    sort(sortedWord.begin(), sortedWord.end());
    return sortedWord;
}

// Function to check if two words are anagrams
bool areAnagrams(const string& word1, const string& word2) {
    string w1 = toLowercase(word1);
    string w2 = toLowercase(word2);

    if (w1.length() != w2.length()) {
        return false;
    }

    unordered_map<char, int> charCount;
    for (char c : w1) {
        charCount[c]++;
    }
    for (char c : w2) {
        charCount[c]--;
        if (charCount[c] < 0) return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " <words.txt> <table_size> <query.txt>" << endl;
        return 1;
    }

    string wordsFile = argv[1];
    int M = stoi(argv[2]);
    string queryFile = argv[3];

    // Hash table for storing words with chaining
    unordered_map<string, list<string>> hashTable; // Changed to use unordered_map

    // Load words from words.txt into hash table
    ifstream wordStream(wordsFile);
    if (!wordStream.is_open()) {
        cerr << "Error: Could not open " << wordsFile << endl;
        return 1;
    }

    string word;
    while (getline(wordStream, word)) {
        string sortedWord = getSortedWord(word); // Get sorted version
        hashTable[sortedWord].push_front(word);  // Insert at the beginning of the linked list
    }
    wordStream.close();

    // Print the hash table with the desired format
    int index = 0; // To keep track of the index for output
    for (const auto& entry : hashTable) {
        const list<string>& anagrams = entry.second; // Get the list of anagrams

        if (!anagrams.empty()) {
            cout << "Index " << index++ << ": "; // Print index
            for (const string& storedWord : anagrams) {
                cout << storedWord << " -> "; // Print each anagram
            }
            cout << "End" << endl; // End of list
        }
    }

    // Read query.txt file to process queries and find anagrams
    ifstream queryStream(queryFile);
    if (!queryStream.is_open()) {
        cerr << "Error: Could not open " << queryFile << endl;
        return 1;
    }

    ofstream outputStream("result_anagrams.txt");
    if (!outputStream.is_open()) {
        cerr << "Error: Could not create output file result_anagrams.txt" << endl;
        return 1;
    }

    string query;
    while (getline(queryStream, query)) {
        string sortedQuery = getSortedWord(query);  // Get the sorted version for the query

        // Get the list of anagrams for this sorted query
        auto it = hashTable.find(sortedQuery);
        if (it != hashTable.end()) {
            const list<string>& anagrams = it->second; // Get the list of anagrams
            // Write anagrams to the output file (reverse order, as required)
            vector<string> anagramList(anagrams.begin(), anagrams.end());
            reverse(anagramList.begin(), anagramList.end()); // Reverse order

            for (size_t i = 0; i < anagramList.size(); i++) {
                outputStream << anagramList[i];
                if (i != anagramList.size() - 1) {
                    outputStream << " "; // Space between anagrams
                }
            }
            outputStream << endl;  // Move to next line for the next query
        } else {
            outputStream << endl; // If no anagrams found, output empty line
        }
    }

    queryStream.close();
    outputStream.close();

    return 0;
}

