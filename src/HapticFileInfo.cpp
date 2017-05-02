#include "stdafx.h"
#include "HapticFileInfo.h"
#include <boost/algorithm/string/case_conv.hpp>
#include <locale>
std::vector<std::string> Glue(std::string firstPart, std::string separator, const std::vector<std::string>& lastParts) 
{
	std::vector<std::string> results;
	for (auto part : lastParts)
	{
		results.push_back(firstPart + separator + part);
	}
	return results;
}

std::string HapticFileInfo::normalizeFullId(std::string id) {
	//std::locale::global(std::locale("en_US.utf8"));
	//auto& f = std::use_facet<std::ctype<wchar_t>>(std::locale());
//	f.tolower(&id[0], &id[0] + id.size());
	boost::algorithm::to_lower(id);
	return id;
}
//Note: transforms package name to lower case
const std::string HapticFileInfo::_packageSeparator = ".";
std::string HapticFileInfo::getPackage(std::string thing)
{
	std::vector<std::string> parts;
	
	boost::split(parts, thing, boost::is_any_of(_packageSeparator));
	if (parts.size() <= 1) {
		throw InvalidPackageNameException(thing);
	}
	auto range = boost::make_iterator_range(parts.begin(), parts.end() - 1);
	return  boost::join(range, _packageSeparator);

}

std::string HapticFileInfo::getName(std::string thing)
{
	std::vector<std::string> parts;
	boost::split(parts, thing, boost::is_any_of(_packageSeparator));
	return parts[parts.size() - 1];
}

SequenceFileInfo::SequenceFileInfo(std::string fullId):HapticFileInfo(HapticFileType::Sequence, fullId)
{
}

SequenceFileInfo::~SequenceFileInfo()
{
}


std::vector<std::string> SequenceFileInfo::GetValidFileNames() const
{
	return Glue(Name, ".", {"sequence"});
}

std::string SequenceFileInfo::GetDirectory() const
{
	return "sequences";
}

PatternFileInfo::PatternFileInfo(std::string fullId):HapticFileInfo(HapticFileType::Pattern, fullId)
{
}

PatternFileInfo::~PatternFileInfo()
{
}

std::vector<std::string> PatternFileInfo::GetValidFileNames() const
{
	return Glue(Name, ".", { "pattern" });
}

std::string PatternFileInfo::GetDirectory() const
{
	return "patterns";
}

ExperienceFileInfo::ExperienceFileInfo(std::string fullId):HapticFileInfo(HapticFileType::Experience, fullId)
{

}

ExperienceFileInfo::~ExperienceFileInfo()
{
}

std::vector<std::string> ExperienceFileInfo::GetValidFileNames() const
{
	return Glue(Name, ".", {"experience" });
}

std::string ExperienceFileInfo::GetDirectory() const
{
	return "experiences";
}

HapticFileInfo::HapticFileInfo(HapticFileType ftype, std::string fullId):
	FullId(normalizeFullId(fullId)),
	FullyQualifiedPackage(getPackage(FullId)), //<-- using normalized FullId 
	Name(getName(FullId)),  //<-- using normalized FullId
	FileType(ftype)
{


}

HapticFileInfo::~HapticFileInfo()
{
}


HapticFileType StringFileType(std::string s)
{
	if (s == "sequence") {
		return HapticFileType::Sequence;
	}
	if (s == "pattern") {
		return HapticFileType::Pattern;
	}
	if (s == "experience") {
		return HapticFileType::Experience;
	}
	return HapticFileType::Unknown;


}

std::string FileTypeString(HapticFileType t) {
	switch (t) {
	case HapticFileType::Pattern:
		return "pattern";
	case HapticFileType::Sequence:
		return "sequence";
	case HapticFileType::Experience:
		return "experience";
	default:
		throw std::runtime_error(
			"Unknown filetype: " + std::to_string((int)t)
		);
	}
}