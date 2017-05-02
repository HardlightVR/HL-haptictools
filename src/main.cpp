// HapticAssetTools.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "HapticAssetTools.h"
#include <iostream>
#include <vector>
#include <boost\program_options.hpp>
#include "AssetToolsLibrary.h"
#include <iostream>


int main(int argc, char* argv[])
{
	namespace po = boost::program_options;
	po::options_description description("Allowed options");
	description.add_options()
		("help", "produce this message")
		("root-path", po::value<std::string>()->required(), "set root haptics directory")
		("list-packages", "list out the haptic packages present")
		("json", "encode output in json format")
		("generate-asset", po::value<std::string>(), "specify effect to generate binary asset")
		("out-file", po::value<std::string>(), "specify out path of binary asset")
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
		std::cout << e.what();
		exit(1);
	}
	
	//weird mix of dll api style and c++ objects. Once I settle on the better solution,
	//I'll fix the error handling to match it. Current idea is command line program. 

	AssetToolsLibrary assetTool;
	
	if (!assetTool.InitializeFromDirectory(vm["root-path"].as<std::string>().c_str())) {
		std::cout << "Error:  " << assetTool.SafeGetError() << '\n';
		exit(1);
	}

	if (vm.count("list-packages")) {
		auto packages = assetTool.GetPackages();
		if (vm.count("json")) {
			using namespace rapidjson;
			Document d;
			d.SetArray();
			for (const auto& p : packages) {
				p.to_parsable_string(d);
			}
			StringBuffer buff;
			Writer<StringBuffer> writer(buff);
			d.Accept(writer);
			std::cout << buff.GetString();
		}
		else {
			for (const auto& p : packages) {
				std::cout << p.to_readable_string() << '\n';
			}
		}
	}
	
	
	else if (vm.count("generate-asset")) {
		std::string outpath = vm.count("out-file") > 0 ? vm["out-file"].as<std::string>() : "";
		
		if (vm.count("json")) {
			try {
				assetTool.CreateJsonAsset(vm["generate-asset"].as<std::string>(), outpath);
			}
			catch (const HapticsLoadingException& e) {
				std::cout << "Error: "<<e.what() << '\n';
			}
		}
		else {
			try {
				assetTool.CreateBinaryAsset(vm["generate-asset"].as<std::string>(), outpath);
			}
			catch (const HapticsLoadingException& e) {
				std::cout << "Error: " << e.what() << '\n';
			}
		}

	}
	

    return 0;
}
