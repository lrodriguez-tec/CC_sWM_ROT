#pragma once

#include <random>
#include <string>
#include "WM.hpp"
#include "nnxx/socket.h"


#include "nngpp/error.h"
#include "nngpp/msg.h"
#include "nngpp/view.h"
#include <nngpp/nngpp.h>
#include <nngpp/protocol/req0.h>
#include <nngpp/protocol/rep0.h>
#include <cstdio>


class Server {

	public:
		Server (int port_, std::string file_name_, int allowed_deletes_);
		virtual ~Server (){}

		void start_server();
		void attend_query(QueryConfig &query_config, Elgamal::PublicKey &pubt, Elgamal::PrivateKey &prvt);

	private:

		Poco::Logger &log = Poco::Logger::get("Server");

		std::string file_name;
		int port;
		WM wm;

		int lg_sigma;
		int text_len;
		int array_len;

		int allowed_deletes;

		//https://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution
		std::random_device rd;  // a seed source for the random number engine
		std::mt19937 gen; 		// mersenne_twister_engine seeded with rd()
		std::uniform_int_distribution<> distrib;

		nnxx::socket serv_socket;
		nng::socket  nserv_sock;
};

