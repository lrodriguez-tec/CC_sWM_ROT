#include "WM.hpp"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <fstream>

WM::WM(std::string file_name){
	std::ifstream ifs(file_name);

	if(!ifs){
		log.error("Error opening file: " + file_name, __FILE__, __LINE__);
		exit(0);
	}

	ifs >> lg_sigma;
	ifs >> text_len;

	rankCF.resize(lg_sigma);
	for (auto &vec: rankCF){
		vec.resize(text_len*2);
		for (auto &item : vec) 
			ifs >> item;
	}
}


int WM::query_rankCF_pos(int pos, int vi){
	return rankCF.at(vi).at(pos);
}


int WM::query_rankCF(int pos, int bit, int vi){
	pos += (bit == 0) ? 0: (text_len);
	return query_rankCF_pos(pos, vi);
}


int WM::query_rankCF(int pos, int query_val){

	for(int i=0; i<lg_sigma; i++, query_val >>= 1)
		pos = query_rankCF(pos, 1 & query_val, i);

	return pos;
}

std::ostream& operator<<(std::ostream& os, const WM& wm){

	os << "lg_sigma=" << wm.lg_sigma<<", text_len=" << wm.text_len << std::endl;

	for(int i=0; i<wm.text_len*2; i++)
		os << "----";
	os << std::endl;

	for(int i=0; i<wm.text_len*2; i++)
		os << std::setw(3) << i << ",";
	os << std::endl;

	for(int i=0; i<wm.text_len*2; i++)
		os << "----";
	os << std::endl;

	for (auto &vec: wm.rankCF){
		for (auto &item : vec)
			os << std::setw(3) << item << ",";
		os << std::endl;
	}

	return os;
}
