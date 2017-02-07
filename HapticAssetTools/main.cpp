// HapticAssetTools.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "HapticAssetTools.h"
#include <iostream>
#include <vector>
#include "boost\program_options.hpp"
#include "AssetToolsLibrary.h"
#include <iostream>
class ErrorString {
public:
	ErrorString(NSAT_Context_t* context):_ptr(NSAT_GetError(context)), _error(_ptr) {

	}
	~ErrorString() {
		NSAT_FreeError(_ptr);
	}
	std::string What() const {
		return _error;
	}
	friend std::ostream& operator<<(std::ostream& stream, const ErrorString& e) {
		stream << e.What();
		return stream;
	}
private:
	char* _ptr = nullptr;
	std::string _error;
};


int main(int argc, char* argv[])
{
	namespace po = boost::program_options;
	po::options_description description("Allowed options");
	description.add_options()
		("help", "produce this message")
		("root-path", po::value<std::string>()->required(), "set root haptics directory")
		("list-packages", "list out the haptic packages present")
	;
	
	po::variables_map vm;

	try {
		po::store(po::parse_command_line(argc, argv, description), vm);
		if (vm.count("help")) {
			std::cout << description;
			exit(0);
		}
		po::notify(vm);

	}
	catch (const po::required_option& e) {
		if (vm.count("help")) {
			std::cout << description << '\n';
			exit(0);
		}
		else {
			std::cout << e.what() << '\n';
			std::cout << description;
			exit(1);
		}
	} 
	catch (const std::exception& e) {
		std::cout << description;
		exit(1);
	}

	AssetToolsLibrary assetTool;
	if (!assetTool.InitializeFromDirectory(vm["root-path"].as<std::string>().c_str())) {

	}
	
    return 0;
}

