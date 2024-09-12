#include <iostream>
#include <fstream>
#include <map>
using namespace std;

//Read mappgin file  char->int
//Read sequences files, generate the int query file used by client.
int main(int argc, char* argv[]){
	cout << "Opening file: " << argv[1] << endl;

	ifstream infile(argv[1]);
	ifstream traces_file(argv[2]);

	int file_name=1;

	if(!infile){
		cerr << "Error abriendo archivo " << argv[1] << endl;
		return -1;
	}

	if(!traces_file){
		cerr << "Error abriendo archivo " << argv[2] << endl;
		return -1;
	}

	char c;
	int i;
	std::map<char,int> char_map;

	while (infile >> c >> i)
		char_map[tolower(c)] = i;

	cout << "========================== char_maps" << endl;;
	for(const auto& elem : char_map) {
		std::cout << elem.first << " " << elem.second << "\n";
	}
	cout << "==========================";

	system("mkdir -p query_files/");

	cout << "========================== traces" << endl;;
	std::string trace;
	while (traces_file >> trace){
		trace = trace + ";";

		ofstream out_file( "query_files/" + std::to_string(file_name) + ".txt");

		out_file << trace.size() << endl;
		out_file << char_map.size() << endl;

		reverse(trace.begin(), trace.end());

		for (char car: trace) {
			out_file << char_map[car] << " ";
		}
		out_file << endl << endl;
		out_file << trace << endl;

		cout << trace << endl;
		out_file.close();
		file_name++;

	}
	cout << "==========================";
}

