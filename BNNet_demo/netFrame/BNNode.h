#pragma once

#ifndef __BNNODE_H__
#define __BNNODE_H__

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <map>

#include "../utils/exitCode.h"

using namespace std;

class BNNode {
public:
    BNNode(string nodeName);

public:
    // private:
    string node_name;

    // vector<Node*> parents;
    unordered_set<BNNode*> parents;
    // vector<Node*> children;
    unordered_set<BNNode*> children;
    // ��Ӧ node ȡֵ��Χ, ע���ֵ
    // ���� {"True", "False"}, {"<5", "5-12", ">12"}, ...
    vector<string> value_range;
    // ��value�õ�value_range�е��±�ֵ
    unordered_map<string, int> value_index_dic;
    // ��ǰ�ڵ��valueֵ, �ذ����� value_range 
    /// �ڹ��캯����û�г�ʼ��, ������������ʱ���ʼ��
    string value;

    // P(A | B = b, C = c) = probability[key]
    // key = "B:b,C:c"  // key Ҫ sort, Ϊ�˷�ֹ�ظ� sort����Ҫ�� Node->parents �ڲ����� -> set
    // A : node->  {a0, a1, a2} -> ��Ӧ prob[key][0, 1, 2]
    // P(D) = probality["empty"]
    /// ע���ѯ�õ�����һ�����ʷֲ� vector, ����value_range��������±�һһ��Ӧ�����Բ���value_index_dicת��
    //map<string, vector<double>> probability;   
    unordered_map<string, vector<double>> probability;

public:
    // ����������ʻ��߸��ڵ���������ȡ�����ֵ, mainNet �� sorted_variables ������Ϊ key ˳�����
    string get_random_value();

    // û�и��ڵ㷵�� "empty" , �����Ҫ mainNet �� sorted_variables ������Ϊ key ˳�����
    string get_parents_status_key();

    // ���ݸ��ʷֲ��õ����ֵ
    string get_value_by_prob_table(const vector<double>& probTable);
};

#endif