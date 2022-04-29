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
    /// name Ϊ�Ѵ��� ./data/prob/ Ŀ¼�µ� .bif �ļ���
    /// bif �ļ����еĿո��ʡ�����ر��Ƕ��ź����֪����ʲô bug
    string name;
    ///  ע��������ļ����� .bif .data ���� .bif ���б���һ��
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

