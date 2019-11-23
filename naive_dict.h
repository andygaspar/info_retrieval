#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <map>
#include <iterator>
#include <vector>
#include <ctype.h>
#include <locale> 

using std::cout;
using std::endl;
using std::string;
using std::vector;





bool is_valid_term(string term);
string get_term(string line, short int& i);
bool is_in_dict(string term,std::map<string, vector<long int>>& dictionary);
bool is_in_posting(long int doc_ID, vector<long int>& posting);
string load_invalid_char();


//enviromental vairables
string invalid_char_file="../storage/invalid_char.csv";
string invalid_chars=load_invalid_char();





string load_invalid_char(){
    std::ifstream g{invalid_char_file};
    string line="";
    string invalid_chars="";
    while(getline(g,line)) invalid_chars+=line;
    return invalid_chars;
}

// check validity of a term

bool is_valid_term(string term){
    

    if (term == ""  or  ispunct(term[0]) or isspace(term[0])) return false;

     
    if (invalid_chars.find(term) != string::npos) return false;
    return true;
}


//get a term from a line starting from position i

string get_term(string line, short int& i){
    string term="";
    std::locale loc;
    while(!isspace(line[i]) and !ispunct(line[i]) and i<line.length()) {
        term += tolower(line[i]);
        i++;
    }
    return term;
}


//check wether a term is already in a dict

bool is_in_dict(string term,std::map<string, vector<long int>>& dictionary){
    std::map<std::string, vector<long int>>::iterator invalid_key= dictionary.end();

    if (dictionary.find(term) == invalid_key) return  false;
    else return true;
}


//check wether an ID is already in a posting list

bool is_in_posting(long int doc_ID, vector<long int>& posting){
    for (long int other: posting){
        if(doc_ID==other) return true;
    }
    return false;
}






