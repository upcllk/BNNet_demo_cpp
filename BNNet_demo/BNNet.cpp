#include <algorithm>

#include "BNNet.h"
#include "tools.h"

BNNet::BNNet(string fileName) {
    net_name = fileName;
    if (fileName == "earthquake") {
        // 内存泄漏不管了
        BNNode* E = new BNNode("Earthquake");
        BNNode* B = new BNNode("Burglary");
        BNNode* A = new BNNode("Alarm");
        BNNode* J = new BNNode("JohnCalls");
        BNNode* M = new BNNode("MaryCalls");

        // 这两个是对应的
        variables = { E, B, A, J, M };
        node_index_dic = {
            {E->node_name, 0},
            {B->node_name, 1},
            {A->node_name, 2},
            {J->node_name, 3},
            {M->node_name, 4}
        };

        /*variables = { J, B, M, A, E };
        node_index_dic = {
            {J->node_name, 0},
            {B->node_name, 1},
            {M->node_name, 2},
            {A->node_name, 3},
            {E->node_name, 4}
        };*/

        E->children.insert(A);
        B->children.insert(A);
        A->children.insert(J);
        A->children.insert(M);
        A->parents.insert(E);
        A->parents.insert(B);
        J->parents.insert(A);
        M->parents.insert(A);

        // 地震模型为简单二值
        E->valueRange = B->valueRange = A->valueRange = J->valueRange = M->valueRange = { "True", "False" };
        E->value_index_dic = B->value_index_dic = A->value_index_dic = J->value_index_dic = M->value_index_dic = {
            {"True", 0},
            {"False", 1}
        };

        // 这概率太低了人为调高点
        // B->probability["empty"] = {0.01, 0.99};
        // E->probability["empty"] = {0.02, 0.98};
        B->probability["empty"] = { 0.35, 0.65 };
        E->probability["empty"] = { 0.2, 0.8 };
        J->probability = {
            {"Alarm:True", {0.9, 0.1}},
            {"Alarm:False", {0.05, 0.95}}
        };
        M->probability = {
            {"Alarm:True", {0.7, 0.3}},
            {"Alarm:False", {0.33, 0.67}}
        };
        // 这边的 key 在生成的时候一定要注意有序，从 parents 中读入
        A->probability = {
            {"Burglary:True,Earthquake:True", {0.95, 0.05}},
            {"Burglary:False,Earthquake:True", {0.29, 0.71}},
            {"Burglary:True,Earthquake:False", {0.94, 0.06}},
            {"Burglary:False,Earthquake:False", {0.15, 0.85}}
        };
    }
    else {
        cout << "BNNet netType error \n";
        exit(NET_TYPE_ERROR);
    }
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
                int index = node_index_dic[next->node_name];
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
