#include "tools.h"
#include <fstream>
#include <streambuf>

vector<string> str_split(const string& str, char seperator) {
	vector<string> ret;
	const int size = str.size();
	int left = 0, right = 0;
	while (left < size) {
		while (right < size && str[right] != seperator) {
			++right;
		}
		// str[right] == seperator || right == size
		string temp = str.substr(left, right - left);
		if (!temp.empty() && temp.back() == '\r') {
			temp.pop_back();
		}
		if (!temp.empty()) {
			ret.push_back(temp);
		}
		left = right + 1;
		right = left;
	}
	return ret;
}

bool str_begin_with(const string& str, const string& pre)
{
	if (pre.empty()) {
		return true;
	}
	if (str.empty() || str.size() < pre.size()) {
		return false;
	}
	int index = 0;
	while (index < pre.size() && str[index] == pre[index]) {
		index++;
	}
	return index == pre.size();
}

bool str_contain(const string& str, char ch)
{
	for (char c : str) {
		if (c == ch) {
			return true;
		}
	}
	return false;
}

string str_trim_space(const string& str)
{
	if (str.empty()) {
		return "";
	}
	string ret;
	int index = 0;
	while (index < str.size() && (str[index] == ' ' || str[index] == '\t')) {
		++index;
	}
	while (index < str.size()) {
		ret.push_back(str[index]);
		++index;
	}
	while (!ret.empty() && ret.back() == ' ') {
		ret.pop_back();
	}
	return ret;
}

void append_data_to_file(const string& filePath, const vector<vector<string>>& data)
{
	// 保证路径的文件夹存在 ... 
	ofstream ofs;
	ofs.open(filePath, ios::out | ios::app);
	if (!ofs.is_open()) {
		exit(OPEN_FILE_ERROR_WHEN_APPEND);
	}
	for (const vector<string>& lineData : data) {
		string outData;
		for (const string& str : lineData) {
			outData += (str + '\t');
		}
		outData.pop_back();
		ofs << outData << endl;
	}
	ofs.flush();
	ofs.close();
}

void append_line_to_file(const string& filePath, const string& lineData)
{
	// 保证路径的文件夹存在 ... 
	ofstream ofs;
	ofs.open(filePath, ios::out | ios::app);
	if (!ofs.is_open()) {
		exit(OPEN_FILE_ERROR_WHEN_APPEND);
	}
	ofs << lineData << endl;
	ofs.flush();
	ofs.close();
}

void truncate_file(const string& filePath)
{
	ofstream ofs;
	ofs.open(filePath, ios::out | ios::trunc);
	if (!ofs.is_open()) {
		exit(OPEN_FILE_ERROE_WHEN_TRUNCATE);
	}
	ofs.close();
}
