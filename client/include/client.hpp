#pragma once

#include <Poco/Logger.h>
#include <string>
#include <vector>

#include <mcl/fp.hpp>
#include <mcl/ec.hpp>
#include <mcl/elgamal.hpp>
#include <mcl/ecparam.hpp>

#include "El_Gammal_config.hpp"
#include "nnxx/socket.h"

class Client {

	public:
		Client (std::string server_name, int port, std::string query_file_name, Elgamal::PrivateKey &prvt_, const Elgamal::PublicKey &pubt);
		virtual ~Client (){}

		int query_pos(int pos, int query_val);
		void start_server();

	private:

		Poco::Logger &log = Poco::Logger::get("Client");

		int port;
		std::string server_name;
		nnxx::socket client_socket;

		int query_size;
		int alph_size;
		std::vector<int> query;

		int lg_sigma;
		int text_len;
		int array_len;

		Elgamal::PrivateKey &prvt;
		const Elgamal::PublicKey &pubt;
};
