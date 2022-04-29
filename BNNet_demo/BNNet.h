#pragma once

#ifndef __BNNET_H__
#define __BNNET_H__

#include "BNNode.h"

class BNNet {
public:
    /*
        一个理想的情况是 BNNet(filePath) {
            ... readFile(filePath)  ... // static function
        }
    */
    BNNet(string fileName);
/// <summary>
///  todo  
///     BNNet(vector<BNNodes*> nodes)    // 赋值一次，因为存的也是节点指针后续节点内更改同步，搜索阶段使用
///     void remove_edge()
///     void add_edge()
///     
/// </summary>

// private:
public:
        // net name   e.g. "Earthquake"
    string net_name;
    // 记录有哪些节点
    vector<BNNode*> variables;
    // 根据网中的节点 name 获得在 variables 中的下标
    unordered_map<string, int> name_index_dic;

public:
    // 获得节点的拓扑排序
    vector<int> get_topo_sort_seq();
    // 根据概率分布生成随机一个值
    string get_single_rand_sample(vector<int>& topoOrder);
    // 生成指定数量的随机样本
    vector<string> get_rand_sample(int count);
    // 
    void save_sample_to_file(const vector<string>& sample);
};

#endif // !__BNNET_H__