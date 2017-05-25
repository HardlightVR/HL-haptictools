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
		("list-packages", "list the haptic packages present")
		("json", "encode output in json format [deprecated, used by default now]")
		("out-file", po::value<std::string>(), "write output to a file path specified with this option")
		("generate-asset", po::value<std::string>(), "create a single HDF from the specified file, written to standard out as JSON")
		("convert-package", po::value<std::string>(), "Specify an entire package to be converted to a target asset format. Specify the package name here, e.g. \"ns.demos\"")
		("hdf-out", po::value<std::string>(), "generate a mirror of the package directory structure passed with --convert-package, where every file is an HDF asset, in the directory specified here")
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
		
	
		try {
			assetTool.CreateJsonAsset(vm["generate-asset"].as<std::string>(), outpath);
		}
		catch (const HapticsLoadingException& e) {
			std::cout << "Error: "<<e.what() << '\n';
		}
		
		

	} else if (vm.count("convert-package")) {
		if (!vm.count("hdf-out")) {
			std::cout << "Error: You must specify the output format. For now, only hdf-out is supported.\n";
			return 0;
		}
		const std::string& outPath = vm["hdf-out"].as<std::string>();
		const std::string& potentialPackageId = vm["convert-package"].as<std::string>();
		if (!assetTool.PackageExists(potentialPackageId)) {
			std::cout << "The package ID [" << potentialPackageId << "] does not exist in root directory [" << vm["root-path"].as<std::string>() << "].\n";
			return 0;
		}
		assetTool.CreateHDFPackageStructure(outPath);
		assetTool.ConvertPackageToHDFs(potentialPackageId, outPath);

	}
	

    return 0;
}

