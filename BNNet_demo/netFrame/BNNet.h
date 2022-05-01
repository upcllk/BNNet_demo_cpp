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
    BNNet() = delete;
    BNNet(string fileName);

/// <summary>
///  todo  
///     BNNet(vector<BNNodes*> nodes)    // 赋值一次，因为存的也是节点指针后续节点内更改同步，搜索阶段使用
///     void remove_edge()
///     void add_edge()
///     
/// </summary>

public:
    // net name   e.g. "Earthquake"
    string net_name;
    // 记录有哪些节点
    vector<BNNode*> variables;
    // 根据网中的节点 name 获得在 variables 中的下标
    unordered_map<string, int> name_index_dic;

public:

    // 生成指定数量的随机样本存入 sample 
    vector<vector<string>> getRandSample(int count);
    // 从 . / data.sample / net_name.data 文件中读入的用例，
    vector<vector<string>> readSampleFromFile();
    // 
    void saveSampleToFile(const vector<vector<string>>& sample);
    // 从 ./data/prob/net_name.bif 文件中读入概率分布, 目前用处只有生成 samples
    void readProbFromFile();

    /// 前期测试，用于检测生成的数据集是否符合要求的概率分布
    /// @input:	"Earthquake:True"	/	"Alarm:True|Earthquake:True,Burglary:False", "end" 结束
    /// @output: "P(Earthquake:True) = ...", "P(Alarm:True|Earthquake:True,Burglary:False) = ..."
    void verifySampleProb(const vector<vector<string>>& sample);

private:
    // 获得节点的拓扑排序
    vector<int> getTopoSortSeq();
    // 根据概率分布生成随机一个值
    vector<string> getSingleRandSample(const vector<int>& topoOrder);

    void parseProbData(const string& data);

    void parseNetName(const vector<string>& slice, int& curLine);
    void parseNetVariable(const vector<string>& slice, int& curLine);
    void parseNetProbability(const vector<string>& slice, int& curLine);

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
    void parseNetCondProb(const vector<string>& slice, int& curLine);
    void parseNetPriorProb(const vector<string>& slice, int& curLine);
};

#endif // !__BNNET_H__