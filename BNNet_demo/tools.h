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
void append_data_to_file(const string& filePath, const vector<string>& data);
void append_line_to_file(const string& filePath, const string& lineData);

/// 读入 .data 文件, 注意有一行表头
vector<vector<string>> read_sample_from_file(const string& filePath);

// 读 .bif 文件初始化 BNNet
void read_prob_from_file(const string& filePath, BNNet* net);

/// 用于检测生成的数据集是否符合要求的概率分布, earthquake 模型检验通过, 别拼错了就行
/// @input:	"Earthquake:True"	/	"Alarm:True|Earthquake:True,Burglary:False", "end" 结束
/// @output: "P(Earthquake:True) = ...", "P(Alarm:True|Earthquake:True,Burglary:False) = ..."
void verify_sample_prob(const vector<vector<string>>& sample);

void parse_prob_data(const string& data, BNNet* net);

void parse_net_name(const vector<string>& slice, int& curLine, BNNet* net);

void parse_net_variable(const vector<string>& slice, int& curLine, BNNet* net);

void parse_net_probability(const vector<string>& slice, int& curLine, BNNet* net);

// 这两个 parse_net_ .. _prob 函数里面注释掉的输出应该为以下形式，供对比
/*
Burglary
        empty
0.01 0.99
Earthquake
        empty
0.02 0.98
Alarm
        Burglary
        Earthquake
        Burglary:True,Earthquake:True
0.95 0.05
        Burglary:False,Earthquake:True
0.29 0.71
        Burglary:True,Earthquake:False
0.94 0.06
        Burglary:False,Earthquake:False
0.001 0.999
JohnCalls
        Alarm
        Alarm:True
0.9 0.1
        Alarm:False
0.05 0.95
MaryCalls
        Alarm
        Alarm:True
0.7 0.3
        Alarm:False
0.01 0.99
*/
void parse_net_prior_prob(const vector<string>& slice, int& curLine, BNNet* net);

void parse_net_cond_prob(const vector<string>& slice, int& curLine, BNNet* net);

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