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
    BNNet() = delete;
    BNNet(string fileName);

/// <summary>
///  todo  
///     BNNet(vector<BNNodes*> nodes)    // ��ֵһ�Σ���Ϊ���Ҳ�ǽڵ�ָ������ڵ��ڸ���ͬ���������׶�ʹ��
///     void remove_edge()
///     void add_edge()
///     
/// </summary>

public:
    // net name   e.g. "Earthquake"
    string net_name;
    // ��¼����Щ�ڵ�
    vector<BNNode*> variables;
    // �������еĽڵ� name ����� variables �е��±�
    unordered_map<string, int> name_index_dic;

public:

    // ����ָ������������������� sample 
    vector<vector<string>> getRandSample(int count);
    // �� . / data.sample / net_name.data �ļ��ж����������
    vector<vector<string>> readSampleFromFile();
    // 
    void saveSampleToFile(const vector<vector<string>>& sample);
    // �� ./data/prob/net_name.bif �ļ��ж�����ʷֲ�, Ŀǰ�ô�ֻ������ samples
    void readProbFromFile();

    /// ǰ�ڲ��ԣ����ڼ�����ɵ����ݼ��Ƿ����Ҫ��ĸ��ʷֲ�
    /// @input:	"Earthquake:True"	/	"Alarm:True|Earthquake:True,Burglary:False", "end" ����
    /// @output: "P(Earthquake:True) = ...", "P(Alarm:True|Earthquake:True,Burglary:False) = ..."
    void verifySampleProb(const vector<vector<string>>& sample);

private:
    // ��ýڵ����������
    vector<int> getTopoSortSeq();
    // ���ݸ��ʷֲ��������һ��ֵ
    vector<string> getSingleRandSample(const vector<int>& topoOrder);

    void parseProbData(const string& data);

    void parseNetName(const vector<string>& slice, int& curLine);
    void parseNetVariable(const vector<string>& slice, int& curLine);
    void parseNetProbability(const vector<string>& slice, int& curLine);

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
    void parseNetCondProb(const vector<string>& slice, int& curLine);
    void parseNetPriorProb(const vector<string>& slice, int& curLine);
};

#endif // !__BNNET_H__