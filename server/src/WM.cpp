#include "WM.hpp"

#include <iomanip>
#include <iostream>
#include <fstream>

WM::WM(std::string file_name){
	std::ifstream ifs(file_name);

	if(!ifs){
		std::cerr << "Error abriendo archivo: " << file_name << std::endl;
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

std::ostream& operator<<(std::ostream& os, const WM& wm){

	os << "lg_sigma=" << wm.lg_sigma<<", text_len=" << wm.text_len << std::endl;

	os << std::endl;
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

Elgamal::CipherText WM::query_rankCF_pos(std::vector<Elgamal::CipherText> ciph_pos, int vi){
	auto &rank_vec = rankCF.at(vi);

	Elgamal::CipherText cr;
	cr.mul(0);

	for(int i=0; i<ciph_pos.size(); i++){
		ciph_pos.at(i).mul(rank_vec.at(i));
		cr.add( ciph_pos.at(i) ); 
	}

	return cr;
}
