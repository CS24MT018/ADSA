#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <unordered_map>
#include <fstream>
#include <algorithm>

using namespace std;

// Function to convert string to lowercase
string toLowercase(const string& s) {
    string result = s;
    for (char& c : result) {
        c = tolower(c);
    }
    return result;
}

// Function to get the sorted version of a word
string getSortedWord(const string& word) {
    string sortedWord = toLowercase(word); // Ensure it's lowercase
    sort(sortedWord.begin(), sortedWord.end()); // Sort the characters
    return sortedWord; // Return sorted word
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " <words.txt> <table_size> <query.txt>" << endl;
        return 1;
    }

    string wordsFile = argv[1];
    string queryFile = argv[3];

    // Hash table for storing words with their sorted versions as keys
    unordered_map<string, list<string>> hashTable;

    // Load words from words.txt into hash table
    ifstream wordStream(wordsFile);
    if (!wordStream.is_open()) {
        cerr << "Error: Could not open " << wordsFile << endl;
        return 1;
    }

    string word;
    while (getline(wordStream, word)) {
        string sortedWord = getSortedWord(word); // Get the sorted version
        hashTable[sortedWord].push_back(word); // Store original word in the list
    }
    wordStream.close();

    // Print the hash table with all values and linked lists
    for (const auto& entry : hashTable) {
        cout << "Key (sorted): " << entry.first << " -> ";
        for (const string& storedWord : entry.second) {
            cout << storedWord << " -> ";
        }
        cout << "End" << endl;
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

    // Close streams
    queryStream.close();
    outputStream.close();

    return 0;
}

