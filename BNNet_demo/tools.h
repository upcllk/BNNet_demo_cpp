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
* 不用担心"\r\n", 直接写 '\n' 就行做了判断的
*/
vector<string> split(const string& str, char seperator);

// 去除两头的空格
string trim_space(const string& str);

template<typename T>
inline void printVec(const vector<T>& vec)
{
	for (auto x : vec) {
		cout << x << " ";
	}
	cout << endl;
}

// 清空文件
void truncate_file(const string& filePath);

// 追加写
void append_data_to_file(const string& filePath, const vector<string>& data);
void append_line_to_file(const string& filePath, const string& lineData);

/// 注意有一行表头
vector<vector<string>> read_sample_from_file(const string& filePath);

/// 用于检测生成的数据集是否符合要求的概率分布, earthquake 模型检验通过, 别拼错了就行
/// @input:	"Earthquake=True"	/	"Alarm=True|Earthquake=True,Burglary=False"
/// @output: "P(Earthquake=True) = ...", "P(Alarm=True|Earthquake=True,Burglary=False) = ..."
void verify_sample_prob(const vector<vector<string>>& sample);

// vector<vector<string>> readFile

#endif // !__TOOLS_H__