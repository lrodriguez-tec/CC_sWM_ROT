#pragma once

#include <string>
#include <vector>

#include <mcl/fp.hpp>
#include <mcl/ec.hpp>
#include <mcl/elgamal.hpp>
#include <mcl/ecparam.hpp>

#include "El_Gammal_config.hpp"

class Client {

	public:
		Client (std::string server_name, int port, std::string query_file_name, Elgamal::PrivateKey &prvt_, const Elgamal::PublicKey &pubt);
		virtual ~Client (){}

	private:
		int port;
		std::string server_name;

		int query_size;
		int alph_size;
		std::vector<int> query;

		Elgamal::PrivateKey &prvt;
		const Elgamal::PublicKey &pubt;
};
