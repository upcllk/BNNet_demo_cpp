#include <iostream>

#include "BNNet.h"
#include "tools.h"

using namespace std;

/*
    ���Ż�����ʵ node.parents �� node.children ˳���Ѿ�����ν��, ������ȫ���ƹ������������ˣ�
    ���߱���һ���ź���� variable.nameS, Ȼ��ÿ�α���������� names �ж��Ƿ��� parents ����
    ����Ҳ�ܱ�֤����
*/
int main()
{
    srand(time(nullptr));
    string name = "earthquake";
    BNNet mainNet = BNNet(name);

    auto t = split(",,,abc,sdsa,dsafassd,dsafa,,,", ',');
    printVec(t);
    auto order = mainNet.get_topo_sort_seq();
    printVec(order);
    cout << "*************************\n";
    //vector<string> sample = mainNet.get_rand_sample(10000);
    //mainNet.save_sample_to_file(sample);
    /// todo : rand_prob_from_file, BNNet ���캯�����ݴ���� fileName ���� filePath ����
    /// todo : ��ֵ���������Լ���֤
    string filePath = "data\\sample\\" + name + ".data";
    vector<vector<string>> sampleRead = read_sample_from_file(filePath);
    verify_sample_prob(sampleRead);
    return 0;
}

