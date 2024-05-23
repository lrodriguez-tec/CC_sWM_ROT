#include <fstream>
#include <iostream>
#include <Poco/Util/Option.h>
#include <Poco/Util/OptionSet.h>
#include <Poco/Util/Application.h>
#include <Poco/Util/HelpFormatter.h>
#include <Poco/Util/OptionCallback.h>

#include "server.hpp"
#include "config.hpp"

using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;
using Poco::Util::OptionCallback;

class Server_app: public Application {

	Poco::Logger &log = Poco::Logger::get("Server_app");

	private:

		bool helpRequested;
		std::string input_file;
		int port;
		int allowed_deletes = 10;

	public:
		Server_app(): helpRequested{false}, port{9999} {}

	protected:	

		void initialize(Application& self) {
			loadConfiguration();
			Application::initialize(self);
		}

		void uninitialize() {
			Application::uninitialize();
		}

		void reinitialize(Application& self) {
			Application::reinitialize(self);
		}

		void defineOptions(OptionSet& options){
			Application::defineOptions(options);

			options.addOption(
					Option("help", "h", "Display this help information")
					.callback(OptionCallback<Server_app>(this, &Server_app::handleHelp)));

			options.addOption(
					Option("input_file", "i", "input file name")
					.required(true)
					.argument("<file_name>")
					.callback(OptionCallback<Server_app>(this, &Server_app::set_input_file)));

			options.addOption(
					Option("allowed_deletes", "d", "number of allowed deletes from user query")
					.argument("<allowed_deletes>")
					.callback(OptionCallback<Server_app>(this, &Server_app::set_allowed_deletes)));

			options.addOption(
					Option("server port number", "p", "port")
					.argument("<port>")
					.callback(OptionCallback<Server_app>(this, &Server_app::set_port)));
		}

		void set_input_file(const std::string& name, const std::string& value) { 
			input_file = value;
		}

		void set_allowed_deletes(const std::string& name, const std::string& value) { 
			allowed_deletes = std::stoi(value);
		}

		void set_port(const std::string& name, const std::string& value) { 
			port = std::stoi(value);
		}

		void handleHelp(const std::string& name, const std::string& value) {
			helpRequested = true;
			displayHelp();
			stopOptionsProcessing();
		}

		void displayHelp() {
			HelpFormatter helpFormatter(options());
			helpFormatter.setCommand(commandName());
			helpFormatter.setUsage("<options>");
			helpFormatter.setHeader("A command line interface (cli) application for AES encryption/decryption.");
			helpFormatter.format(std::cout);
		}

		int main(const std::vector<std::string>& args){
			if (!helpRequested){
				log.information(std::string{"***** Running : "} + PROJECT_NAME,__FILE__,__LINE__);
				log.information(std::string{"***** Version : "} + PROJECT_VER,__FILE__,__LINE__);

				log.information("Application options : " , 					__FILE__,__LINE__);
				log.information("  -input_file_name  : " + input_file , 	__FILE__,__LINE__);
				log.information("  -port             : " + std::to_string(port) , 	__FILE__,__LINE__);
				log.information("  -allowed deletes  : " + std::to_string(allowed_deletes) , 	__FILE__,__LINE__);

				Elgamal_sysInit();
				Server server{port, input_file, allowed_deletes};
			}

			return Application::EXIT_OK;
		}
};

POCO_APP_MAIN(Server_app)
