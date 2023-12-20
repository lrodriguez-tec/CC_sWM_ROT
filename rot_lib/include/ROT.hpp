#pragma once

#include <vector>
#include <enc_index.pb.h>

#include "El_Gammal_config.hpp"
#include "WM.hpp"
#include <Poco/Logger.h>

class ROT{

		Poco::Logger &log = Poco::Logger::get("ROT");

	public:

		//output: cipher index as vector in enc_index
		void prep_query(const int index, const int array_len, const Elgamal::PrivateKey &prvt, const Elgamal::PublicKey &pubt, EncIndex &enc_index);

		//remove random r from enc_index vector
		std::vector<Elgamal::CipherText> req_query(EncIndex &enc_index, int r, Elgamal::PrivateKey &prvt);

		//query WM and add random r
		Elgamal::CipherText RanOT(std::vector<Elgamal::CipherText> &ciph_pos, int vi, int r, WM &wm);
};
