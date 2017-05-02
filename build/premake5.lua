-- We are using PCH, so you must disable PCH on the protobuf files, and enable generation on stdafx.cpp


workspace "AssetTools"
	configurations {"Debug", "Release"}
	platforms {"Win32", "Win64"}
	language "C++"
	
	


project "AssetTool" 
	kind "ConsoleApp"
	
	targetdir "bin/%{cfg.buildcfg}/%{cfg.platform}"
	targetname "HapticAssetTools"
	
	-- dependencies
	protobuf_incl_dir = "D:/protobuf-3.0.0/cmake/build/solution/include"
	boost_incl_dir = "D:/Libraries/boost/boost_1_61_0"
	protobuf_def_incl_dir = "C:/Users/NullSpace Team/Documents/NS_Unreal_SDK/src/Driver/protobuff_defs"
		disablewarnings {"4800"}


	includedirs {
		protobuf_incl_dir,
		boost_incl_dir,
		protobuf_def_incl_dir
	}

	flags {
		"MultiProcessorCompile",
		"C++11"

	}
	--links {"System", "UnityEditor", "UnityEngine", "System.ServiceProcess"}

	files {
		"../src/*.cpp",
		"../src/*.h",
		"../src/*.hpp",
		"../src/rapidjson/**.h",
		path.join(protobuf_def_incl_dir, "HapticEffect.pb.cc")

		
	}

	

	-- for protobuf


	boost_win32_dir = "D:/Libraries/boost/boost_1_61_0/stage/win32/lib"
	boost_win64_dir = "D:/Libraries/boost/boost_1_61_0/stage/x64/lib"

	protobuf_win32_dir = "D:/protobuf-3.0.0/cmake/build/solution"
	protobuf_win64_dir = "D:/protobuf-3.0.0/cmake/build/solution64"
	

	pchheader "stdafx.h"
	pchsource "../src/stdafx.cpp"


	defines {"RAPIDJSON_HAS_STDSTRING", "NS_ASSETTOOLS_EXPORTS"}
	
	filter {"files:**.pb.cc"}
		flags {'NoPCH'}


	-- input: libprotobuf
	filter {"platforms:Win32", "configurations:Debug"}
		libdirs {
			path.join(protobuf_win32_dir, "Debug")
		}
	filter {"platforms:Win32", "configurations:Release"}
		libdirs {
			path.join(protobuf_win32_dir, "Release")
		}
	filter {"platforms:Win64", "configurations:Debug"}
		libdirs {
			path.join(protobuf_win64_dir, "Debug")
		}
	filter {"platforms:Win64", "configurations:Release"}
		libdirs {
			path.join(protobuf_win64_dir, "Release")
		}


	filter "platforms:Win32" 
		system "Windows"
		architecture "x86"
		libdirs {
			boost_win32_dir
		}
		defines {"WIN32"}
	filter "platforms:Win64"
		system "Windows"
		architecture "x86_64"
		libdirs {
			boost_win64_dir
		}
	filter "configurations:Debug"
		defines {"DEBUG", "_DEBUG"}
		symbols "On"
		optimize "Off"
		links {"libprotobufd"}

	filter "configurations:Release"
		defines {"NDEBUG"}
		optimize "On" 
		links {"libprotobuf"}

	filter {"system:Windows"}
		defines {"_WINDOWS"}

	filter {"system:Windows", "configurations:Debug"}
		buildoptions {"-D_SCL_SECURE_NO_WARNINGS"}

