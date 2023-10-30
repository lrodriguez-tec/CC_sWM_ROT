#pragma once

#include <string>
#include "WM.hpp"
#include "nnxx/socket.h"

class Server {

	public:
		Server (int port_, std::string file_name_);
		virtual ~Server (){}

		void start_server();
		void attend_query(QueryConfig &query_config);

	private:

		Poco::Logger &log = Poco::Logger::get("Server");

		std::string file_name;
		int port;
		WM wm;

		int lg_sigma;
		int text_len;
		int array_len;

		nnxx::socket serv_socket;
};

