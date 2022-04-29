#pragma once

#ifndef __BNNET_H__
#define __BNNET_H__

#include "BNNode.h"

class BNNet {
public:
    /*
        һ������������ BNNet(filePath) {
            ... readFile(filePath)  ... // static function
        }
    */
    BNNet(string fileName);

public:
    // private:
        // net name   e.g. "Earthquake"
    string net_name;
    // ��¼����Щ�ڵ�
    vector<BNNode*> variables;
    // �������еĽڵ� name ����� variables �е��±�
    unordered_map<string, int> node_index_dic;

public:
    // ��ýڵ����������
    vector<int> get_topo_sort_seq();
    // ���ݸ��ʷֲ��������һ��ֵ
    string get_single_rand_sample(vector<int>& topoOrder);
    // ����ָ���������������
    vector<string> get_rand_sample(int count);
    // 
    void save_sample_to_file(const vector<string>& sample);
};

#endif // !__BNNET_H__