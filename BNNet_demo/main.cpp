#include <iostream>

#include "BNNet.h"
#include "tools.h"

using namespace std;

/*
    待优化：其实 node.parents 和 node.children 顺序已经无所谓了, 倒不如全复制过来再排序算了，
    或者保留一份排好序的 variable.nameS, 然后每次遍历这个排序 names 判断是否在 parents 里面
    这样也能保证有序
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
    /// todo : rand_prob_from_file, BNNet 构造函数根据传入的 fileName 生成 filePath 调用
    /// todo : 多值样本生成以及验证
    string filePath = "data\\sample\\" + name + ".data";
    vector<vector<string>> sampleRead = read_sample_from_file(filePath);
    verify_sample_prob(sampleRead);
    return 0;
}

