#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <utility>
#include <unordered_map>
//#include "cmdline.h"
#include "esa.hxx"
#include "wat_array.hpp"

using namespace std;


int getID(const string& str, unordered_map<string, int>& word2id){
  unordered_map<string, int>::const_iterator it = word2id.find(str);
  if (it == word2id.end()){
    int newID = (int)word2id.size();
    word2id[str] = newID;
    return newID;
  } else {
    return it->second;
  }
}

string getTerm(const vector<int>& T, const int beg, const int len, const vector<string>& id2word, string separator){
  string term = "";
  for (int i = 0; i < len; ++i){
    int c = T[beg + i];
    if (id2word.size() > 0){
      term += id2word[c] + separator;
    } else {
      term += (isspace((char)c) ? '_' : (char)c);
    }
  }
  return term;
}

