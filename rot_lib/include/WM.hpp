#pragma once

#include "El_Gammal_config.hpp"
#include "enc_index.pb.h"
#include <Poco/Logger.h>
#include <vector>
#include <string>
#include <iostream>

class WM {
	private:
		Poco::Logger &log = Poco::Logger::get("WM");

		std::vector<std::vector<int>> rankCF;
		int lg_sigma;
		int text_len;

	public:

		WM (std::string file_name);
		std::ostream& operator<<(std::ostream &os);
		int query_rankCF_pos(int pos, int vi);		//query position on vi
		int query_rankCF(int pos, int bit, int vi); 
		int query_rankCF(int pos, int query_val);	//query full char (all bits)

		int get_text_len(){ return text_len; }
		int get_lg_sigma(){ return lg_sigma; }

		friend std::ostream& operator<<(std::ostream& os, const WM& wm);
};
