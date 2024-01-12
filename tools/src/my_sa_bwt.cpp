#include <fstream>
#include <iostream>
#include <map>
#include <random>
#include <memory>
#include <string>
#include <vector>
#include <utility>
#include "esa.hxx"
#include <Poco/Message.h>
#include <iomanip>

using namespace std;

int main(int argc, char* argv[]){

	if(argc < 2){
		cout << "use: " << argv[0] << " <file_name>" << endl;
		return 0;
	}
	
	vector<char> Tr;
	vector<int> T, SA, B, C;

	std::string file_name{argv[1]};
	cout << "Opening: " << file_name << endl;

	ifstream myfile{file_name};

	size_t lastindex = file_name.find_last_of("."); 
	string out_name = file_name.substr(0, lastindex) + "_to_ints.txt";
	ofstream outfile{ out_name };

	if ( !myfile ) { 
		perror(("Error opening file: " + file_name).c_str());
		return 0;
	}

	char c;
	int i = 0;
	std::map<char, int> maps;

	while( myfile >> c ){
		c = toupper(c);
		if(!maps.contains(c))
			maps[c] = 0;
		Tr.push_back( c );
	}
	
	for(const auto& elem : maps)
		maps[elem.first] = i++;
	maps['$'] = maps.size();
	maps['\0'] = 0;

	for(auto &elem: Tr){
		T.push_back( maps[elem]);
	}
	T.push_back(0);

	outfile << T.size() << endl;
	outfile << maps.size() << endl;
	for(auto &elem: T){
		outfile << elem << " ";
	}
	outfile << endl << endl;


	for(auto &elem: Tr){
		outfile << elem << " ";
	}
	outfile << endl << endl;

		int n = T.size();
	SA.resize(n, 0);
	B.resize(n, 0);
	C.resize(n, 0);
	int k = 26;

	if (saisxx(T.begin(), SA.begin(), n, k) == -1){ return -1; }
	if (saisxx_bwt(T.begin(), B.begin(), C.begin(), n, k) == -1){ return -1; }

	outfile << "P    : ";
	for(int i=0; i<T.size(); i++){
		outfile << setw(3) << i << ", ";
	}
	outfile << endl;

	outfile << "--------------------------------------------------------------------------------" << endl;

	outfile << "T    : ";
	for(auto &val: T)
		outfile << setw(3) << val << ", ";
	outfile << endl << endl;

	outfile << "SA   : ";
	for(auto &val: SA)
		outfile << setw(3) << val << ", ";
	outfile << endl << endl;

	/*
	 *outfile << "BWTB : ";
	 *for(auto &val: B)
	 *    outfile << setw(3) << val << ", ";
	 *outfile << endl << endl;
	 */

	outfile << "BWT  : ";
	for(auto &val: C)
		outfile << setw(3) << val << ", ";
	outfile << endl << endl;

	for(const auto& elem : maps)
		if(elem.first == '\0'){
			outfile << "\\0" << " : " << elem.second << endl;
		}else
			outfile << elem.first << " : " << elem.second << endl;

	outfile << endl;


	outfile << "SA:" << endl;
	int idx=0;
	for(auto index: SA){
		outfile <<"[" << setw(4) << idx++ << "]" << setw(5) << index << " : ";
		for(int j=index; j < T.size(); j++){
			outfile << setw(3) << T[j];
		}
		outfile << endl;
	}

	outfile.close();

	cout << "Results written in: " << out_name << endl;

	return 0;
}
