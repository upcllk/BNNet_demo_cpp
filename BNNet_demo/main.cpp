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
    /// name 为已存在 ./data/prob/ 目录下的 .bif 文件名
    /// bif 文件该有的空格别省，，特别是逗号后面鬼知道有什么 bug
    string name;
    ///  注意这里的文件名根 .bif .data 还有 .bif 首行保持一致
    name = "earthquake";
    name = "knowledge_clip";
    name = "child";

    BNNet mainNet = BNNet(name);

    vector<string> sample = mainNet.get_rand_sample(100000);
    mainNet.save_sample_to_file(sample);
    string filePath = "data\\sample\\" + name + ".data";
    vector<vector<string>> sampleRead = read_sample_from_file(filePath);
    verify_sample_prob(sampleRead);
    return 0;
}

