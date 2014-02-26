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
	inline std::string findAndReplace(const std::string& str, const std::string& find, const std::string& replace) {
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
	}

	//inline std::string findAndReplace(const std::string& str, const std::string &find, const std::string &replace)
	//{
	//	std::string result;
	//	for (unsigned int i = 0; i < str.size(); i++) {
	//		if(exactMatchAtOffset(find, i))
	//		{
	//			result += replace;
	//			i += find.size() - 1;
	//		}
	//		else result += m_data[i];
	//	}
	//	return result;
	//}

	//! removes all characters from a string
	inline void removeChar(std::string &str, const char c) {
		 str.erase (std::remove(str.begin(), str.end(), c), str.end());
	}
	inline std::string removeChar(const std::string &strInput, const char c) {
		std::string str(strInput);
		str.erase (std::remove(str.begin(), str.end(), c), str.end());
		return str;
	}







	//////////////////////
	// native functions //
	//////////////////////
	inline int convertStringToINT(const std::string& s) {
		return atoi(s.c_str());
	}
	inline unsigned int convertStringToUINT(const std::string& s) {
		return (unsigned int)convertStringToINT(s);
	}
	inline double convertStringToDOUBLE(const std::string& s) {
		return atof(s.c_str());
	}
	inline float convertStringToFLOAT(const std::string& s) {
		return (float)convertStringToDOUBLE(s);
	}
	inline char convertStringToCHAR(const std::string& s) {
		return s[0];
	}
	template<class U> inline point2d<U> convertStringToPoint2D(const std::string& s) {
		point3d<U> ret;
		std::stringstream ss(removeChar(s,'f'));
		ss >> ret.x >> ret.y;
		return ret;
	}
	template<class U> inline point3d<U> convertStringToPoint3D(const std::string& s) {
		point3d<U> ret;
		std::stringstream ss(removeChar(s,'f'));
		ss >> ret.x >> ret.y >> ret.z;
		return ret;
	}
	template<class U> inline point4d<U> convertStringToPoint4D(const std::string& s) {
		point4d<U> ret;
		std::stringstream ss(removeChar(s,'f'));
		ss >> ret.x >> ret.y >> ret.z >> ret.w;
		return ret;
	}
	inline bool convertStringToBOOL(const std::string& s) {
		if (s == "false" || s == "0")	return false;
		else return true;		
	}

	////////////////////////
	// template overloads //
	////////////////////////
	//template<class T> inline T convertTo(const std::string& s) {
	//	std::string res;
	//	convertStringTo(s, res);
	//	return res;
	//}

	template<class T>	inline void convertStringTo(const std::string& s, T& res);

	template<>	inline void convertStringTo<int>(const std::string& s, int& res) {
		res = convertStringToINT(s);
	}
	template<>	inline void convertStringTo<unsigned int>(const std::string& s, unsigned int& res) {
		res = convertStringToUINT(s);
	}
	template<>	inline void convertStringTo<double>(const std::string& s, double& res) {
		res = convertStringToDOUBLE(s);
	}
	template<>	inline void convertStringTo<float>(const std::string& s, float& res) {
		res = convertStringToFLOAT(s);
	}
	template<>	inline void convertStringTo<std::string>(const std::string& s, std::string& res) {
		res = s;
	}
	template<>	inline void convertStringTo<char>(const std::string& s, char& res) {
		res = convertStringToCHAR(s);
	}
	template<class U> inline void convertStringTo(const std::string& s, point2d<U>& res) {
		std::stringstream ss(removeChar(s,'f'));
		ss >> res.x >> res.y;
	}
	template<class U> inline void convertStringTo(const std::string& s, point3d<U>& res) {
		std::stringstream ss(removeChar(s,'f'));
		ss >> res.x >> res.y >> res.z;
	}
	template<class U> inline void convertStringTo(const std::string& s, point4d<U>& res) {
		std::stringstream ss(removeChar(s,'f'));
		ss >> res.x >> res.y >> res.z >> res.w;
	}
	template<> inline void convertStringTo<bool>(const std::string& s, bool& res) {
		res = convertStringToBOOL(s);
	}
}


#endif