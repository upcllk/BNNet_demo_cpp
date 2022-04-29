#pragma once

#ifndef __TOOLS_H__
#define __TOOLS_H__

#include <iostream>
#include <vector>
#include <string>
#include "BNNet.h"

using namespace std;

/*
*@parma str
* ���õ���"\r\n", ֱ��д '\n' ���������жϵ�
*/
vector<string> split(const string& str, char seperator);

// ȥ����ͷ�Ŀո�
string trim_space(const string& str);

template<typename T>
inline void printVec(const vector<T>& vec)
{
	for (auto x : vec) {
		cout << x << " ";
	}
	cout << endl;
}

// ����ļ�
void truncate_file(const string& filePath);

// ׷��д
void append_data_to_file(const string& filePath, const vector<string>& data);
void append_line_to_file(const string& filePath, const string& lineData);

/// ע����һ�б�ͷ
vector<vector<string>> read_sample_from_file(const string& filePath);

/// ���ڼ�����ɵ����ݼ��Ƿ����Ҫ��ĸ��ʷֲ�, earthquake ģ�ͼ���ͨ��, ��ƴ���˾���
/// @input:	"Earthquake=True"	/	"Alarm=True|Earthquake=True,Burglary=False"
/// @output: "P(Earthquake=True) = ...", "P(Alarm=True|Earthquake=True,Burglary=False) = ..."
void verify_sample_prob(const vector<vector<string>>& sample);

// vector<vector<string>> readFile

#endif // !__TOOLS_H__