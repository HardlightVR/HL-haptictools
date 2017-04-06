#pragma once
#include <boost/algorithm/string.hpp>
enum class HapticFileType
{
	Pattern = 0,
	Sequence = 1,
	Experience = 2,
	Unknown = 3
};

HapticFileType StringFileType(std::string s);
std::string FileTypeString(HapticFileType t);

class InvalidPackageNameException : public std::runtime_error {
public:
	InvalidPackageNameException(const std::string& package) : std::runtime_error(std::string("The package name " + package + " is invalid").c_str()) {}

};
class HapticFileInfo
{
public:
	HapticFileInfo(HapticFileType ftype, std::string fullId);
	virtual ~HapticFileInfo();
	const std::string FullId;

	const std::string FullyQualifiedPackage;
	const std::string Name;
	const HapticFileType FileType;
	virtual std::string GetDirectory() const
	{
		return "";
	}
	


	virtual std::vector<std::string> GetValidFileNames() const
	{
		return std::vector<std::string>();
	}


private:
	static std::string getPackage(std::string thing);
	static std::string getName(std::string thing);
	static std::string normalizeFullId(std::string thing);
protected:
	static const std::string _packageSeparator;

};

class SequenceFileInfo : public HapticFileInfo
{
public: 
	SequenceFileInfo(std::string fullId);
	~SequenceFileInfo();
	std::vector<std::string> GetValidFileNames() const override;
	std::string GetDirectory() const override;
};


class PatternFileInfo : public HapticFileInfo
{
public:
	PatternFileInfo(std::string fullId);
	~PatternFileInfo();
	std::vector<std::string> GetValidFileNames() const override;
	std::string GetDirectory() const override;
};

class ExperienceFileInfo : public HapticFileInfo
{
public:
	ExperienceFileInfo(std::string fullId);
	~ExperienceFileInfo();
	std::vector<std::string> GetValidFileNames() const override;
	std::string GetDirectory() const override;
};