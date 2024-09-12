#include <fstream>
#include <iostream>
#include <Poco/Util/Option.h>
#include <Poco/Util/OptionSet.h>
#include <Poco/Util/Application.h>
#include <Poco/Util/HelpFormatter.h>
#include <Poco/Util/OptionCallback.h>

#include "client.hpp"
#include "config.hpp"

using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;
using Poco::Util::OptionCallback;

class Client_app: public Application {

	Poco::Logger &log = Poco::Logger::get("Client_app");

	private:

		bool helpRequested;
		std::string input_file;
		std::string server_name;
		int port;

	public:
		Client_app(): helpRequested{false}, port{9999}, server_name{"localhost"}, input_file{"resources/query.txt"} {}

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
					.callback(OptionCallback<Client_app>(this, &Client_app::handleHelp)));

			options.addOption(
					Option("input_file", "i", "input file name")
					.required(true)
					.argument("<file_name>")
					.callback(OptionCallback<Client_app>(this, &Client_app::set_input_file)));

			options.addOption(
					Option("Client port number", "p", "port")
					.required(false)
					.argument("<port>")
					.callback(OptionCallback<Client_app>(this, &Client_app::set_port)));

			options.addOption(
					Option("Server name", "s", "server_name")
					.required(false)
					.argument("<server_name>")
					.callback(OptionCallback<Client_app>(this, &Client_app::set_server_name)));
	
		}

		void set_input_file(const std::string& name, const std::string& value) { 
			input_file = value;
		}

		void set_server_name(const std::string& name, const std::string& value) { 
			server_name = value;
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
				log.information("  -server_name      : " + server_name , 	__FILE__,__LINE__);
				log.information("  -port             : " + std::to_string(port) , 	__FILE__,__LINE__);

				Elgamal::PrivateKey prvt = generate_prvt_key();
				const Elgamal::PublicKey &pubt = prvt.getPublicKey();

				log.information("*** prvt: " + prvt.getStr());
				log.information("*** pubt: " + pubt.getStr());

				Client client{server_name, port, input_file , prvt, pubt};
			}

			return Application::EXIT_OK;
		}
};

POCO_APP_MAIN(Client_app)
