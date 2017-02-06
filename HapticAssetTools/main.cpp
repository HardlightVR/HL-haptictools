// HapticAssetTools.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "HapticAssetTools.h"
#include <iostream>

int main()
{
	auto context = NSAT_InitializeFromDirectory("C:/Hatics");

	PackageInfo p;
	if (NSAT_IsPackage(context, "C:/Haptics/NS Demos", p)) {
		std::cout << "Yup";
	}
    return 0;
}

