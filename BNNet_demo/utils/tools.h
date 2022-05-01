#pragma once

#ifndef __TOOLS_H__
#define __TOOLS_H__

#include <iostream>
#include <vector>
#include <string>

#include "../netFrame/BNNet.h"

using namespace std;

/*
*@parma str
* 不用担心"\r\n", 直接写 '\n' 就行做了判断的
*/
vector<string> str_split(const string& str, char seperator);

bool str_begin_with(const string& str, const string& pre);

bool str_contain(const string& str, char ch);

// 去除两头的空格
string str_trim_space(const string& str);

template<typename T>
inline void print_vec(const vector<T>& vec)
{
	for (auto x : vec) {
		cout << x << " ";
	}
	cout << endl;
}

// 清空文件
void truncate_file(const string& filePath);

// 追加写
void append_data_to_file(const string& filePath, const vector<vector<string>>& data);
void append_line_to_file(const string& filePath, const string& lineData);

// 0 - INT_MAX
inline int get_rand_num()
{
    int num = 0;
    for (int i = 0; i < 31; ++i) {
        num |= ((rand() & 1) << i);
    }
    return num;
}

#endif // !__TOOLS_H__