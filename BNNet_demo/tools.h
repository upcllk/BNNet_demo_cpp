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

bool str_begin_with(const string& str, const string& pre);

bool str_contain(const string& str, char ch);

// ȥ����ͷ�Ŀո�
string str_trim_space(const string& str);

template<typename T>
inline void print_vec(const vector<T>& vec)
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

/// ���� .data �ļ�, ע����һ�б�ͷ
vector<vector<string>> read_sample_from_file(const string& filePath);

// �� .bif �ļ���ʼ�� BNNet
void read_prob_from_file(const string& filePath, BNNet* net);

/// ���ڼ�����ɵ����ݼ��Ƿ����Ҫ��ĸ��ʷֲ�, earthquake ģ�ͼ���ͨ��, ��ƴ���˾���
/// @input:	"Earthquake:True"	/	"Alarm:True|Earthquake:True,Burglary:False", "end" ����
/// @output: "P(Earthquake:True) = ...", "P(Alarm:True|Earthquake:True,Burglary:False) = ..."
void verify_sample_prob(const vector<vector<string>>& sample);

void parse_prob_data(const string& data, BNNet* net);

void parse_net_name(const vector<string>& slice, int& curLine, BNNet* net);

void parse_net_variable(const vector<string>& slice, int& curLine, BNNet* net);

void parse_net_probability(const vector<string>& slice, int& curLine, BNNet* net);

// ������ parse_net_ .. _prob ��������ע�͵������Ӧ��Ϊ������ʽ�����Ա�
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