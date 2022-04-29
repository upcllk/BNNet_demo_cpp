#include <algorithm>

#include "BNNet.h"
#include "tools.h"

BNNet::BNNet(string fileName) {
    net_name = fileName;
    string filePath = "data\\prob\\" + fileName + ".bif";
    read_prob_from_file(filePath, this);
}


vector<int> BNNet::get_topo_sort_seq()
{
    vector<int> seq;
    const int size = variables.size();
    vector<bool> visited(size, false);
    vector<int> inDgrees(size, 0);
    for (int i = 0; i < size; ++i) {
        inDgrees[i] = variables[i]->parents.size();
    }

    int preSize = 0;
    while (seq.size() < size) {
        // update
        for (int i = 0; i < size; ++i) {
            if (visited[i] || inDgrees[i] != 0) {
                continue;
            }
            visited[i] = true;
            seq.push_back(i);
            for (BNNode* next : variables[i]->children) {
                int index = name_index_dic[next->node_name];
                inDgrees[index]--;
            }
        }
        if (preSize == seq.size()) {
            // 遇到了环
            exit(GET_RING_WHEN_TOPO);
        }
        preSize = seq.size();
    }
    return seq;
}

string BNNet::get_single_rand_sample(vector<int>& topoOrder)
{
    string ret;
    const int size = variables.size();
    vector<int> sample(size);
    for (int i = 0; i < size; ++i) {
        BNNode* curNode = variables[topoOrder[i]];
        // "Earthquake:True"形式
        string value = curNode->get_random_value();
        ret += (value + "\t");
    }
    ret.pop_back();
    return ret;
}

vector<string> BNNet::get_rand_sample(int count)
{
    if (count <= 0) {
        exit(GET_RAND_SIZE_ERROR);
    }
    cout << "generating sample ... \n";
    vector<string> ret(count);
    vector<int> topoOrder = get_topo_sort_seq();
    do {
        if (count % 10000 == 0) {
            cout << count << endl;
        }
        count--;
        ret[count] = get_single_rand_sample(topoOrder);
    } while (count > 0);
    return ret;
}

void BNNet::save_sample_to_file(const vector<string>& sample)
{
    // ./data/sample/earthquake.data
    const string filePath = "data\\sample\\" + this->net_name + ".data";
    // 防止追加写入中间出现头部行
    truncate_file(filePath);

    cout << "saving sample to file " << filePath << endl;

    string headInfo;
    // 同一网络多次获取拓扑排序结果一样
    vector<int> order = get_topo_sort_seq();
    for (int index : order) {
        BNNode* cur = variables[index];
        headInfo += (cur->node_name + "\t");
    }
    headInfo.pop_back();
    append_line_to_file(filePath, headInfo);
    append_data_to_file(filePath, sample);

    cout << "save file end. \n";
}
