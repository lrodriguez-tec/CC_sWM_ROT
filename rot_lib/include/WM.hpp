#pragma once

#include "El_Gammal_config.hpp"
#include "enc_index.pb.h"
#include <Poco/Logger.h>
#include <vector>
#include <string>
#include <iostream>


class WM {
	public:

		WM (std::string file_name);
		std::ostream& operator<<(std::ostream &os);
		int query_rankCF(int pos, int bit, int vi);
		int query_rankCF(int pos, int query_val);
		int query_rankCF_pos(int pos, int vi);
		Elgamal::CipherText query_rankCF_pos(std::vector<Elgamal::CipherText> &ciph_pos, int vi);
		Elgamal::CipherText query_rankCF_pos(EncIndex &enc_index, int vi);

		Elgamal::CipherText query_rankCF_pos(std::vector<Elgamal::CipherText> &ciph_pos, int vi, int prev_r, int current_r);
		Elgamal::CipherText query_rankCF_pos(EncIndex &enc_index, int vi, int prev_r, int currenrt_r, Elgamal::PrivateKey  &prvt);

		int get_text_len(){ return text_len; }
		int get_lg_sigma(){ return lg_sigma; }

		friend std::ostream& operator<<(std::ostream& os, const WM& wm);

	private:

		Poco::Logger &log = Poco::Logger::get("WM");

		std::vector<std::vector<int>> rankCF;
		int lg_sigma;
		int text_len;
};
