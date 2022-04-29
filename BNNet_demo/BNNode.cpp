#include "BNNode.h"
#include "tools.h"
#include <algorithm>

BNNode::BNNode(string nodeName) {
    node_name = nodeName;
}

string BNNode::get_random_value()
{
    string probKey = get_parents_status_key();
    //cout << "KEYS : " << probKey << endl;
    //printVec(probability[probKey]);
    return get_value_by_prob_table(probability[probKey]);
}

string BNNode::get_parents_status_key()
{
    if (this->parents.empty()) {
        return "empty";
    }
    string str;
    // 因为 parents 存的 Node*, 不能保证 name 有序
    /*for(BNNode* pa : parents) {
        str += (pa->node_name + ":" + pa->value + ",");
    }*/
    vector<BNNode*> sorted_Node(parents.begin(), parents.end());
    sort(sorted_Node.begin(), sorted_Node.end(),
        [](const BNNode* const node1, const BNNode* const node2) {
            return node1->node_name < node2->node_name;
        });

    for(BNNode* pa : sorted_Node) {
        str += (pa->node_name + ":" + pa->value + ",");
    }
    str.pop_back();
    return str;
}

string BNNode::get_value_by_prob_table(const vector<double>& probTable)
{
    double target = static_cast<double>(rand()) / RAND_MAX;
    int index = 0;
    while (index < probTable.size() && probTable[index] < target) {
        target -= probTable[index];
        ++index;
    }
    if (index == probTable.size()) {
        if (fabs(target) < 1E-10) {
            // 这样就返回最后一个吧浮点数精度问题 ? 出现过 bug
            index--;
        }
        else {
            exit(GET_RAND_VALUE_ERROR);
        }
    }
    this->value = valueRange[index];
    return this->value;
}
