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
    // 对应 node 取值范围, 注意多值
    // 比如 {"True", "False"}, {"<5", "5-12", ">12"}, ...
    vector<string> value_range;
    // 由value得到value_range中的下标值
    unordered_map<string, int> value_index_dic;
    // 当前节点的value值, 必包含于 value_range 
    /// 在构造函数里没有初始化, 在生成样本的时候初始化
    string value;

    // P(A | B = b, C = c) = probability[key]
    // key = "B:b,C:c"  // key 要 sort, 为了防止重复 sort，这要求 Node->parents 内部有序 -> set
    // A : node->  {a0, a1, a2} -> 对应 prob[key][0, 1, 2]
    // P(D) = probality["empty"]
    /// 注意查询得到的是一个概率分布 vector, 他跟value_range长度相等下标一一对应，所以才有value_index_dic转换
    //map<string, vector<double>> probability;   
    unordered_map<string, vector<double>> probability;

public:
    // 根据先验概率或者父节点条件概率取得随机值, mainNet 的 sorted_variables 传入作为 key 顺序参数
    string get_random_value();

    // 没有父节点返回 "empty" , 这边需要 mainNet 的 sorted_variables 传入作为 key 顺序参数
    string get_parents_status_key();

    // 根据概率分布得到随机值
    string get_value_by_prob_table(const vector<double>& probTable);
};

#endif