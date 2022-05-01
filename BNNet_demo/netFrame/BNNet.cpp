#include <algorithm>
#include <fstream>

#include "BNNet.h"
#include "../utils/tools.h"

BNNet::BNNet(string fileName) {
    net_name = fileName;
    // 这些改为类的 private 函数算了吧 反正是用于初始化网络
    readProbFromFile();
    /*
    // 之前手动构造的代码
    if (fileName == "earthquake") {
        // 内存泄漏不管了
        BNNode* E = new BNNode("Earthquake");
        BNNode* B = new BNNode("Burglary");
        BNNode* A = new BNNode("Alarm");
        BNNode* J = new BNNode("JohnCalls");
        BNNode* M = new BNNode("MaryCalls");

        // 这两个是对应的
        variables = { E, B, A, J, M };
        name_index_dic = {
            {E->node_name, 0},
            {B->node_name, 1},
            {A->node_name, 2},
            {J->node_name, 3},
            {M->node_name, 4}
        };

        variables = { J, B, M, A, E };
        name_index_dic = {
            {J->node_name, 0},
            {B->node_name, 1},
            {M->node_name, 2},
            {A->node_name, 3},
            {E->node_name, 4}
        };

        E->children.insert(A);
        B->children.insert(A);
        A->children.insert(J);
        A->children.insert(M);
        A->parents.insert(E);
        A->parents.insert(B);
        J->parents.insert(A);
        M->parents.insert(A);

        // 地震模型为简单二值
        E->value_range = B->value_range = A->value_range = J->value_range = M->value_range = { "True", "False" };
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
    */
}

vector<int> BNNet::getTopoSortSeq()
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

vector<vector<string>> BNNet::getRandSample(int count)
{
    if (count <= 0) {
        exit(GET_RAND_SIZE_ERROR);
    }
    cout << "generating sample ... \n";
    vector<vector<string>> ret(count);
    vector<int> topoOrder = getTopoSortSeq();
    do {
        if (count % 10000 == 0) {
            cout << count << endl;
        }
        count--;
        ret[count] = getSingleRandSample(topoOrder);
    } while (count > 0);
    return ret;
}

vector<string> BNNet::getSingleRandSample(const vector<int>& topoOrder)
{
    vector<string> ret;
    const int size = variables.size();
    for (int i = 0; i < size; ++i) {
        BNNode* curNode = variables[topoOrder[i]];
        // "Earthquake:True"形式
        ret.push_back(curNode->get_random_value());
    }
    return ret;
}

void BNNet::parseProbData(const string& data)
{
    vector<string> slice = str_split(data, '\n');
    int curLine = 0;
    while (curLine < slice.size()) {
        if (str_begin_with(slice[curLine], "network")) {
            parseNetName(slice, curLine);
        }
        else if (str_begin_with(slice[curLine], "variable")) {
            parseNetVariable(slice, curLine);
        }
        else if (str_begin_with(slice[curLine], "probability")) {
            parseNetProbability(slice, curLine);
        }
        else {
            exit(PROB_FILE_FORMAT_ERROR);
        }
    }
}

void BNNet::parseNetName(const vector<string>& slice, int& curLine)
{
    /*network earthquake{
    }*/
    net_name = str_split(str_split(slice[curLine], ' ')[1], '{')[0];
    curLine += 2;
}

void BNNet::parseNetVariable(const vector<string>& slice, int& curLine)
{
    /*variable Burglary{
        type discrete[2] { True, False };
    }*/
    string nodeName = str_split(str_split(slice[curLine], ' ')[1], '{')[0];
    curLine++;
    //cout << "nodeName : " << nodeName << endl;
    BNNode* node = new BNNode(nodeName);
    node->value_range = str_split(str_split(str_split(slice[curLine], '{')[1], '}')[0], ',');
    for (string& value : node->value_range) {
        value = str_trim_space(value);
        //cout << "\t" << value << endl;
    }
    for (int i = 0; i < node->value_range.size(); ++i) {
        name_index_dic[node->value_range[i]] = i;
    }
    name_index_dic[node->node_name] = variables.size();
    variables.push_back(node);
    curLine += 2;
}

void BNNet::parseNetProbability(const vector<string>& slice, int& curLine)
{
    if (str_contain(slice[curLine], '|')) {
        parseNetCondProb(slice, curLine);
    }
    else {
        parseNetPriorProb(slice, curLine);
    }
}

void BNNet::parseNetCondProb(const vector<string>& slice, int& curLine)
{
    /*
    probability ( Alarm | Burglary, Earthquake ) {
      (True, True) 0.95, 0.05;
      (False, True) 0.29, 0.71;
      (True, False) 0.94, 0.06;
      (False, False) 0.001, 0.999;
    }
    */
    string nodeName = str_trim_space(str_split(str_split(str_split(slice[curLine], '(')[1], ')')[0], '|')[0]);
    vector<string> parents = str_split(str_split(str_split(str_split(slice[curLine], '(')[1], ')')[0], '|')[1], ',');

    BNNode* node = variables[name_index_dic[nodeName]];
    cout << nodeName << endl;
    for (string& pa : parents) {
        pa = str_trim_space(pa);
        //cout << "\t" << pa << "\t\n";
        BNNode* parentNode = variables[name_index_dic[pa]];
        node->parents.insert(parentNode);
        parentNode->children.insert(node);
    }
    curLine++;
    while (!slice[curLine].empty() && slice[curLine][0] != '}') {
        //process cond_prob
        //(True, True) 0.95, 0.05;
        //...
        vector<string> status = str_split(str_split(str_split(slice[curLine], ')')[0], '(')[1], ',');
        vector<string> probs = str_split(str_split(str_split(slice[curLine], ')')[1], ';')[0], ',');
        for (int i = 0; i < parents.size(); ++i) {
            const string parentName = parents[i];
            BNNode* parentNode = variables[name_index_dic[parentName]];
            parentNode->value = str_trim_space(status[i]);
        }
        string parentStatusKey = node->get_parents_status_key();
        for (const string& probStr : probs) {
            node->probability[parentStatusKey].push_back(
                static_cast<double>(stof(str_trim_space(probStr)))
            );
        }
        //cout << "\t" << parentStatusKey << endl;
        //print_vec(node->probability[parentStatusKey]);
        curLine++;
    }
    curLine++;
}

void BNNet::parseNetPriorProb(const vector<string>& slice, int& curLine)
{
    /*
	    probability(Earthquake) {
		    table 0.02, 0.98;
	    }
    */
    string nodeName = str_trim_space(str_split(str_split(slice[curLine], '(')[1], ')')[0]);
    //cout << nodeName << endl;
    BNNode* node = variables[name_index_dic[nodeName]];
    curLine++;
    // 前面还一个 "table"
    vector<string> probs = str_split(str_split(str_trim_space(slice[curLine]), ';')[0], ' ');
    probs.erase(probs.begin());
    string parentStatusKey = node->get_parents_status_key();
    for (const string& probStr : probs) {
        node->probability[parentStatusKey].push_back(
            static_cast<double>(stof(str_trim_space(probStr)))
        );
    }
    //cout << "\t" << parentStatusKey << endl;
    //print_vec(node->probability[parentStatusKey]);
    curLine += 2;
}


vector<vector<string>> BNNet::readSampleFromFile()
{
    const string filePath = "data\\sample\\" + net_name + ".data";
    cout << "reading file from " << filePath << endl;
    ifstream ifs;
    ifs.open(filePath, ios::in);
    if (!ifs.is_open()) {
        exit(OPEN_FILE_ERROR_WHEN_READ);
    }
    vector<vector<string>> sample;
    sample.clear();
    while (!ifs.eof()) {
        string str;
        getline(ifs, str);
        sample.push_back(str_split(str, '\t'));
    }
    ifs.close();
    // 向文件写入时每行都有一个 endl, 需要把最后的空行去掉
    if (!sample.empty()) {
        sample.pop_back();
    }
    cout << "read end. \n";
    return sample;
}

void BNNet::saveSampleToFile(const vector<vector<string>>& sample)
{
    // ./data/sample/earthquake.data
    const string filePath = "data\\sample\\" + this->net_name + ".data";
    // 防止追加写入中间出现头部行
    truncate_file(filePath);

    cout << "saving sample to file " << filePath << endl;

    string headInfo;
    // 同一网络多次获取拓扑排序结果一样
    vector<int> order = getTopoSortSeq();
    for (int index : order) {
        BNNode* cur = variables[index];
        headInfo += (cur->node_name + "\t");
    }
    headInfo.pop_back();
    append_line_to_file(filePath, headInfo);
    append_data_to_file(filePath, sample);

    cout << "save file end. \n";
}

void BNNet::readProbFromFile()
{
    cout << "reading .bif ... \n";
    const string filePath = "data\\prob\\" + net_name + ".bif";
    ifstream ifs;
    ifs.open(filePath, ios::in);
    if (!ifs.is_open()) {
        exit(OPEN_FILE_ERROR_WHEN_READ);
    }
    // 一次全部读入
    const string data = string(
        istreambuf_iterator<char>(ifs),
        istreambuf_iterator<char>()
    );
    parseProbData(data);
    cout << "end. \n";
}

void BNNet::verifySampleProb(const vector<vector<string>>& sample)
{
    if (sample.empty()) {
        exit(SAMPLE_EMPTY);
    }
    cout << "processing data ...\n";
    vector<string> variables = sample[0];
    const int size = variables.size();
    vector<unordered_map<string, string>> sampleInfo;
    for (int i = 1; i < sample.size(); ++i) {
        unordered_map<string, string> temp;
        for (int j = 0; j < size; ++j) {
            temp[variables[j]] = sample[i][j];
        }
        sampleInfo.push_back(temp);
    }
    cout << "example :  \"Earthquake:True\" / \"Alarm:True|Earthquake:True,Burglary:False\", end with \"end\"\n";
    cout << "variables : \n";
    for (auto str : variables) {
        cout << str << " ";
    }
    cout << endl;

    while (1) {
        string str;
        string query, cond;
        string output;
        cout << "input query and cond :\n\t";
        cin >> str;
        if (str == "end") {
            return;
        }
        auto temp = str_split(str, '|');
        int count = 0, total = 0;
        double result = 0;
        if (temp.size() == 1) {
            // 先验
            query = temp[0];
            output = ("P(" + query + ")");
            vector<string> queryVec = str_split(query, ':');
            for (auto& dic : sampleInfo) {
                if (dic[queryVec[0]] == queryVec[1]) {
                    ++count;
                }
                ++total;
            }
        }
        else {
            query = temp[0];
            cond = temp[1];
            vector<string> queryVec = str_split(query, ':');
            output += queryVec[0] + ":" + queryVec[1] + "|";

            vector<string> singleCond = str_split(cond, ',');
            vector<vector<string>> condVec;

            for (string& str : singleCond) {
                vector<string> temp = str_split(str, ':');
                condVec.push_back(temp);
                output += (temp[0] + ":" + temp[1] + ",");
            }
            output.pop_back();
            output = "P(" + output + ")";
            for (auto& dic : sampleInfo) {
                bool status = true;
                for (auto& cond : condVec) {
                    if (dic[cond[0]] != cond[1]) {
                        status = false;
                        break;
                    }
                }
                if (status) {
                    ++total;
                    if (dic[queryVec[0]] == queryVec[1]) {
                        ++count;
                    }
                }
            }
        }
        result = count * 1.0 / total;
        cout << output + " = " + to_string(result) << endl;;
    }
}
