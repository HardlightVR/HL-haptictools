#pragma once
#include <stdexcept>

class HapticsLoadingException : public std::runtime_error {
public:
	HapticsLoadingException(std::string message) :
	
		std::runtime_error(message) {}
	
};