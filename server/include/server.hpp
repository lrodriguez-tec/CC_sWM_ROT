#pragma once

#include <string>
#include "WM.hpp"

class Server {

	public:
		Server (int port_, std::string file_name_);
		virtual ~Server (){}

	private:
		std::string file_name;
		int port;
		WM wm;
};

