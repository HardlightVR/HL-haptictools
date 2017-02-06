#pragma once
#include <stdexcept>

class StringPointerException : public std::runtime_error {
public:
	StringPointerException(const char* errString) :
		_errString(errString),
		std::runtime_error(errString) {}
	const char* GetMsgPointer() const {
		return _errString;
	}
private:
	const char* _errString;
};