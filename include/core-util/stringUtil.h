#pragma once

#ifndef _STRING_UTIL_H_
#define _STRING_UTIL_H_


namespace StringUtil {
	

	//TODO TEST
	inline bool startsWith(const std::string& str, const std::string& startCandidate) {
		if (str.length() < startCandidate.length()) return false;
		for (UINT i = 0; i < startCandidate.length(); i++) {
			if(str[i] != startCandidate[i]) return false;
		}
		return true;
	}

	//TODO TEST
	inline bool endsWith(const std::string& str, const std::string& endCandidate) {
		if (str.length() < endCandidate.length()) return false;
		for (UINT i = 0; i < endCandidate.length(); i++) {
			if(str[str.length() - endCandidate.length() + i] != endCandidate[i]) return false;
		}
		return true;
	}

	//TODO TEST
	inline bool exactMatchAtOffset(const std::string& str, const std::string& find, size_t offset)
	{
		UINT MatchLength = 0;
		for(size_t i = 0; i + offset < str.length() && i < find.length(); i++)
		{
			if (str[i + offset] == find[i])
			{
				MatchLength++;
				if(MatchLength == find.length()) return true;
			}
		}
		return false;
	}

	//TODO TEST
	inline std::string replace(const std::string& str, const std::string& find, const std::string& replace) {
		std::string result;
		for (size_t i = 0; i < str.length(); i++) {
			if (exactMatchAtOffset(str, find, i)) {
				result += replace;
				i += find.length() - 1;
			}
			else result += str[i];
		}
		return result;
	}

	inline std::string replace(const std::string& str, char find, char replace) {
		return StringUtil::replace(str, std::to_string(find), std::to_string(replace));
	}


	//TODO TEST
	inline Vector<std::string> split(const std::string& str, const std::string& seperator, bool pushEmptyStrings = false) {
		MLIB_ASSERT_STR(seperator.length() >= 1, "empty seperator");
		Vector<std::string> result;
		std::string entry;
		for (size_t i = 0; i < str.length(); i++) {
			bool isSeperator = true;
			for(size_t testIndex = 0; testIndex < seperator.length() && i + testIndex < str.length() && isSeperator; testIndex++)
			{
				if(str[i + testIndex] != seperator[testIndex])
				{
					isSeperator = false;
				}
			}
			if(isSeperator)
			{
				if(entry.length() > 0 || pushEmptyStrings)
				{
					result.pushBack(entry);
					entry.clear();
				}
				i += seperator.size() - 1;
			}
			else
			{
				entry.push_back(str[i]);
			}
		}
		if (entry.length() > 0) result.pushBack(entry);
		return result;
	}

	inline Vector<std::string> split(const std::string& str, const char seperator, bool pushEmptyStrings = false) {
		return split(str, std::to_string(seperator), pushEmptyStrings);
	}


	//! converts all chars of a string to lowercase (returns the result)
	inline std::string toLower(const std::string& str) {
		std::string res(str);
		for (size_t i = 0; i < res.length(); i++) {
			if (res[i] <= 'Z' &&  res[i] >= 'A') {
				res[i] -= ('Z'-'z');
			}
		}
		return res;
	}
	//! converts all chars of a string to uppercase (returns the result)
	inline std::string toUpper(const std::string& str) {
		std::string res(str);
		for (size_t i = 0; i < res.length(); i++) {
			if (res[i] <= 'z' &&  res[i] >= 'a') {
				res[i] += ('Z'-'z');
			}
		}
		return res;
	}

	//! removes all characters from a string
	inline std::string removeChar(const std::string &strInput, const char c) {
		std::string str(strInput);
		str.erase(std::remove(str.begin(), str.end(), c), str.end());
		return str;
	}

	//! gets the file extension (ignoring case)
	inline std::string getFileExtension(const std::string& filename) {
		std::string extension = filename.substr(filename.find_last_of(".")+1);
		for (unsigned int i = 0; i < extension.size(); i++) {
			extension[i] = tolower(extension[i]);
		}
		return extension;
	}
}

#endif