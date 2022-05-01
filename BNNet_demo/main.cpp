#include <iostream>

#include "netFrame/BNNet.h"
#include "utils/tools.h"

using namespace std;

/*
    ���Ż�����ʵ node.parents �� node.children ˳���Ѿ�����ν��, ������ȫ���ƹ������������ˣ�
    ���߱���һ���ź���� variable.nameS, Ȼ��ÿ�α���������� names �ж��Ƿ��� parents ����
    ����Ҳ�ܱ�֤����
*/

vector<vector<string>> g_sample;

int main()
{
    srand(time(nullptr));
    /// name Ϊ�Ѵ��� ./data/prob/ Ŀ¼�µ� .bif �ļ���
    /// bif �ļ����еĿո��ʡ�����ر��Ƕ��ź����֪����ʲô bug
    string name;
    ///  ע��������ļ����� .bif .data ���� .bif ���б���һ��
    name = "earthquake";
    //name = "knowledge_clip";
    //name = "child";

    BNNet mainNet = BNNet(name);

    vector<vector<string>> sample_generated;
    sample_generated = mainNet.getRandSample(100000);
    mainNet.saveSampleToFile(sample_generated);
    g_sample = mainNet.readSampleFromFile();
    mainNet.verifySampleProb(g_sample);
    return 0;
}

