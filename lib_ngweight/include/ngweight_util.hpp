#pragma once

#include <string>
#include <vector>
#include <map>

using namespace std;


int getID(const string& str, unordered_map<string, int>& word2id);
string getTerm(const vector<int>& T, const int beg, const int len, const vector<string>& id2word, string separator=" ");
 
