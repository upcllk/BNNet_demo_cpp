#include "tools.h"
#include <fstream>

vector<string> split(const string& str, char seperator) {
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

string trim_space(const string& str)
{
	if (str.empty()) {
		return "";
	}
	string ret;
	int index = 0;
	while (index < str.size() && str[index] == ' ') {
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

void append_data_to_file(const string& filePath, const vector<string>& data)
{
	// 保证路径的文件夹存在 ... 
	ofstream ofs;
	ofs.open(filePath, ios::out | ios::app);
	if (!ofs.is_open()) {
		exit(OPEN_FILE_ERROR_WHEN_APPEND);
	}
	for (const string& line : data) {
		ofs << line << endl;
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

vector<vector<string>> read_sample_from_file(const string& filePath)
{
	ifstream ifs;
	ifs.open(filePath, ios::in);
	if (!ifs.is_open()) {
		exit(OPEN_FILE_ERROR_WHEN_READ);
	}
	vector<vector<string>> sample;
	int control = 10;
	while (!ifs.eof()) {
		string str;
		getline(ifs, str);
		sample.push_back(split(str, '\t'));
	}
	ifs.close();
	// 向文件写入时每行都有一个 endl, 需要把最后的空行去掉
	if (!sample.empty()) {
		sample.pop_back();
	}
	return sample;
}

void verify_sample_prob(const vector<vector<string>>& sample)
{
	if (sample.empty()) {
		exit(SAMPLE_EMPTY);
	}
	vector<string> variables = sample[0];
	for (auto str : variables) {
		cout << str << " ";
	}
	cout << endl;
	const int size = variables.size();
	vector<unordered_map<string, string>> sampleInfo;
	for (int i = 1; i < sample.size(); ++i) {
		unordered_map<string, string> temp;
		for (int j = 0; j < size; ++j) {
			temp[variables[j]] = sample[i][j];
		}
		sampleInfo.push_back(temp);
	}
	while (1) {
		string str;
		string query, cond;
		string output;
		cout << "input query and cond :\n\t";
		cin >> str;
		auto temp = split(str, '|');
		int count = 0, total = 0;
		double result = 0;
		if (temp.size() == 1) {
			// 先验
			query = temp[0];
			output = ("P(" + query + ")");
			vector<string> queryVec = split(query, '=');
			for (auto& dic : sampleInfo) {
				if (dic[queryVec[0]] == queryVec[1]) {
					++count;
				}
				++total;
			}
		}
		else {
			query = temp[0];
			cond = temp[1];
			vector<string> queryVec = split(query, '=');
			output += queryVec[0] + "=" + queryVec[1] + "|";

			vector<string> singleCond = split(cond, ',');
			vector<vector<string>> condVec;

			for (string& str : singleCond) {
				vector<string> temp = split(str, '=');
				condVec.push_back(temp);
				output += (temp[0] + "=" + temp[1] + ",");
			}
			output.pop_back();
			output = "P(" + output + ")";
			for (auto& dic : sampleInfo) {
				bool status = true;
				for (auto& cond : condVec) {
					if (dic[cond[0]] != cond[1]) {
						status = false;
						break;
					}
				}
				if (status) {
					++total;
					if (dic[queryVec[0]] == queryVec[1]) {
						++count;
					}
				}
			}
		}
		result = count * 1.0 / total;
		cout << output + " = " + to_string(result) << endl;;
	}
}