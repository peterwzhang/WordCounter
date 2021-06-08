#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

class Word {
   public:
    string word;
    int count;
    Word() {
        word = "";
        count = 0;
    }
    Word(string s) {
        word = s;
        count = 0;
    }
    void PrintWords() { cout << word << ":" << count << endl; }
};

string RemoveSymbols(string s);
string ConvertToLower(string s);
bool CheckQues(string s);
int GetStringCount(string arr[], int size, string s);
int CheckExistingWord(Word arr[], int size, string s);
bool MatchWildCard(string s, int numQ, string s2);

int main(int argc, char **argv) {
    string text;
    ifstream fileToRead;
    fileToRead.open(argv[1]);
    if (fileToRead.is_open())  // make sure file is openable
    {
        getline(fileToRead, text, '\0');
        // cout << text << endl;
    } else {
        return 0;
    }
    string newText = RemoveSymbols(text);
    newText = ConvertToLower(newText);
    istringstream iss(newText);
    string tempText[100000];
    int numWords = 0;
    iss >> tempText[numWords];
    numWords++;
    while (iss) {
        iss >> tempText[numWords];

        if (tempText[numWords] == "\0") break;
        numWords++;
    }
    // put count duplicate words and put into arr
    string newWords[numWords];
    int numUniqueWords = 0;
    for (int i = 0; i < numWords; i++) {
        bool unique = true;
        for (int j = 0; j < numWords; j++) {
            if (i == j) continue;
            if (tempText[i] == tempText[j]) unique = false;
        }
        if (unique) {
            newWords[numUniqueWords] = tempText[i];
            numUniqueWords++;
        } else if (!unique)  // put into non unique words into count for proj
                             // requirement
        {
            bool existing = false;
            for (int k = 0; k < numUniqueWords; k++) {
                if (newWords[k] == tempText[i]) {
                    existing = true;
                    break;
                }
            }
            if (!existing) {
                newWords[numUniqueWords] = tempText[i];
                numUniqueWords++;
            }
            // if existing do nothing
        }
    }
    // numUniqueWords++; // because we start at index 0
    // numWords++; // because we start at index 0
    Word uniqueWords[numUniqueWords];
    // put word into class form
    for (int i = 0; i < numUniqueWords; i++) {
        uniqueWords[i].word = newWords[i];
        uniqueWords[i].count =
            GetStringCount(tempText, numWords, uniqueWords[i].word);
    }
    // //print all words and count
    // for (int i = 0; i < numUniqueWords; i++)
    // {
    //     uniqueWords[i].PrintWords();
    // }

    // print output and begin statements
    cout << "The number of words found in the file was " << numWords << endl;
    cout << "The number of distinct words found in the file was "
         << numUniqueWords << endl;
    cout << endl;

    // begin loop for user input
    string wordToSearch;
    while (1) {
        cout << "Please enter a word: ";
        cin >> wordToSearch;
        if (cin.eof()) {
            break;
        }
        int numQuestion = 0;
        for (int i = 0; i < (int)wordToSearch.length(); i++) {
            if (wordToSearch[i] == '?') numQuestion++;
        }
        // cout << numQuestion << endl;
        if (numQuestion != 0) {
            // do recursion to find wildcard char
            for (int i = 0; i < numUniqueWords; i++) {
                // uniqueWords[i].PrintWords();
                if (MatchWildCard(wordToSearch, numQuestion,
                                  uniqueWords[i].word)) {
                    cout << "The word " << uniqueWords[i].word << " appears "
                         << uniqueWords[i].count << " times in the document"
                         << endl;
                }
            }
        } else {
            cout << "The word " << wordToSearch << " appears "
                 << GetStringCount(tempText, numWords, wordToSearch)
                 << " times in the document" << endl;
        }
        cout << endl;
    }
    return 0;
}

string RemoveSymbols(string s) {
    for (int i = 0; i <= (int)s.length(); i++) {
        s[i] = (isalpha(s[i])) ? s[i] : ' ';
    }
    return s;
}

string ConvertToLower(string s) {
    for (int i = 0; i <= (int)s.length(); i++) {
        s[i] = tolower(s[i]);
    }
    return s;
}

int CheckExistingWord(Word arr[], int size,
                      string s)  // searches for first occur. of string and
                                 // returns index if found, 0 if not
{
    for (int i = 0; i < size; i++) {
        if (arr[i].word == s) return i;
    }
    return 0;
}

int GetStringCount(string arr[], int size,
                   string s)  // get count of words in string arr
{
    int count = 0;
    for (int i = 0; i < size; i++) {
        if (arr[i] == s) count++;
    }
    return count;
}

bool CheckQues(string s)  // get count of words in string arr
{
    if (s.find("?") != string::npos) return true;
    return false;
}

bool CheckAllQues(string s)  // get count of words in string arr
{
    int num = 0;
    while (num < (int)s.length() + 1) {
        num++;
        if (s.find("?", num) != string::npos) continue;
        return false;
    }
    return true;
}

bool MatchWildCard(string s, int numQ,
                   string s2)  // s is wildcard string, s2 is comparison
{
    if ((int)s2.length() == 0) return ((int)s.length() == numQ);
    if (numQ == 0) return s == s2;
    if (s[0] != '?') {
        if (s[0] != s2[0]) return false;
        return MatchWildCard(s.substr(1), numQ, s2.substr(1));
    }
    bool matchOneRes = MatchWildCard(s.substr(1), numQ - 1, s2.substr(1));
    if (matchOneRes) return true;
    return MatchWildCard(s.substr(1), numQ - 1, s2);
}
