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

Elgamal::CipherText WM::query_rankCF_pos(std::vector<Elgamal::CipherText> &ciph_pos, int vi){
	auto &rank_vec = rankCF.at(vi);

	Elgamal::CipherText cr;
	cr.mul(0);

	for(int i=0; i<ciph_pos.size(); i++){
		ciph_pos.at(i).mul(rank_vec.at(i));
		cr.add( ciph_pos.at(i) ); 
	}
return cr;
}

Elgamal::CipherText WM::query_rankCF_pos(EncIndex &enc_index, int vi){

	std::vector<Elgamal::CipherText> ciph_pos( enc_index.query_size() );
	for(int i=0; i<enc_index.query_size() ; i++)
		ciph_pos.at(i).fromStr( enc_index.query(i) );

	return query_rankCF_pos( ciph_pos, vi);
}

Elgamal::CipherText WM::query_rankCF_pos(EncIndex &enc_index, int vi, int prev_r, int current_r){

	std::vector<Elgamal::CipherText> ciph_pos( enc_index.query_size() );

	for(int i=0; i<enc_index.query_size() ; i++)
		ciph_pos.at(i).fromStr( enc_index.query(i) );

	return query_rankCF_pos( ciph_pos, vi, prev_r, current_r);
}

Elgamal::CipherText WM::query_rankCF_pos(std::vector<Elgamal::CipherText> &ciph_pos, int vi, int prev_r, int current_r){
	auto &rank_vec = rankCF.at(vi);

	Elgamal::CipherText cr;
	cr.mul(0);

	for(int i=0; i< prev_r; i++) 
		std::rotate(ciph_pos.begin(), ciph_pos.begin() + 1, ciph_pos.end());

	for(int i=0; i<ciph_pos.size(); i++){
		ciph_pos.at(i).mul( (rank_vec.at(i) + current_r) % text_len);
		cr.add( ciph_pos.at(i) ); 
	}

	return cr;
}

