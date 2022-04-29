#include "tools.h"
#include <fstream>
#include <streambuf>

vector<string> split(const string& str, char seperator) {
	vector<string> ret;
	const int size = str.size();
	int left = 0, right = 0;
	while (left < size) {
		while (right < size && str[right] != seperator) {
			++right;
		}
		// str[right] == seperator || right == size
		string temp = str.substr(left, right - left);
		if (!temp.empty() && temp.back() == '\r') {
			temp.pop_back();
		}
		if (!temp.empty()) {
			ret.push_back(temp);
		}
		left = right + 1;
		right = left;
	}
	return ret;
}

bool str_begin_with(const string& str, const string& pre)
{
	if (pre.empty()) {
		return true;
	}
	if (str.empty() || str.size() < pre.size()) {
		return false;
	}
	int index = 0;
	while (index < pre.size() && str[index] == pre[index]) {
		index++;
	}
	return index == pre.size();
}

inline bool str_contain(const string& str, char ch)
{
	for (char c : str) {
		if (c == ch) {
			return true;
		}
	}
	return false;
}


string str_trim_space(const string& str)
{
	if (str.empty()) {
		return "";
	}
	string ret;
	int index = 0;
	while (index < str.size() && (str[index] == ' ' || str[index] == '\t')) {
		++index;
	}
	while (index < str.size()) {
		ret.push_back(str[index]);
		++index;
	}
	while (!ret.empty() && ret.back() == ' ') {
		ret.pop_back();
	}
	return ret;
}

void append_data_to_file(const string& filePath, const vector<string>& data)
{
	// 保证路径的文件夹存在 ... 
	ofstream ofs;
	ofs.open(filePath, ios::out | ios::app);
	if (!ofs.is_open()) {
		exit(OPEN_FILE_ERROR_WHEN_APPEND);
	}
	for (const string& line : data) {
		ofs << line << endl;
	}
	ofs.flush();
	ofs.close();
}

void append_line_to_file(const string& filePath, const string& lineData)
{
	// 保证路径的文件夹存在 ... 
	ofstream ofs;
	ofs.open(filePath, ios::out | ios::app);
	if (!ofs.is_open()) {
		exit(OPEN_FILE_ERROR_WHEN_APPEND);
	}
	ofs << lineData << endl;
	ofs.flush();
	ofs.close();
}

void truncate_file(const string& filePath)
{
	ofstream ofs;
	ofs.open(filePath, ios::out | ios::trunc);
	if (!ofs.is_open()) {
		exit(OPEN_FILE_ERROE_WHEN_TRUNCATE);
	}
	ofs.close();
}

vector<vector<string>> read_sample_from_file(const string& filePath)
{
	cout << "reading file from " << filePath << endl;
	ifstream ifs;
	ifs.open(filePath, ios::in);
	if (!ifs.is_open()) {
		exit(OPEN_FILE_ERROR_WHEN_READ);
	}
	vector<vector<string>> sample;
	while (!ifs.eof()) {
		string str;
		getline(ifs, str);
		sample.push_back(split(str, '\t'));
	}
	ifs.close();
	// 向文件写入时每行都有一个 endl, 需要把最后的空行去掉
	if (!sample.empty()) {
		sample.pop_back();
	}
	cout << "read end. \n";
	return sample;
}

void read_prob_from_file(const string& filePath, BNNet* net)
{
	cout << "reading .bif ... \n";
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
	parse_prob_data(data, net);
	cout << "end. \n";
}

void parse_prob_data(const string& data, BNNet* net)
{
	vector<string> slice = split(data, '\n');
	int curLine = 0;
	while (curLine < slice.size()) {
		if (str_begin_with(slice[curLine], "network")) {
			parse_net_name(slice, curLine, net);
		}
		else if(str_begin_with(slice[curLine], "variable")) {
			parse_net_variable(slice, curLine, net);
		}
		else if (str_begin_with(slice[curLine], "probability")) {
			parse_net_probability(slice, curLine, net);
		}
		else {
			exit(PROB_FILE_FORMAT_ERROR);
		}
	}
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

void parse_net_name(const vector<string>& slice, int& curLine, BNNet* net)
{
	/*network earthquake{
	}*/
	net->net_name = split(split(slice[curLine], ' ')[1], '{')[0];
	curLine += 2;
}

void parse_net_variable(const vector<string>& slice, int& curLine, BNNet* net)
{
	/*variable Burglary{
		type discrete[2] { True, False };
	}*/
	string nodeName = split(split(slice[curLine], ' ')[1], '{')[0];
	curLine++;
	//cout << "nodeName : " << nodeName << endl;
	BNNode* node = new BNNode(nodeName);
	node->value_range = split(split(split(slice[curLine], '{')[1], '}')[0], ',');
	for (string& value : node->value_range) {
		value = str_trim_space(value);
		//cout << "\t" << value << endl;
	}
	for (int i = 0; i < node->value_range.size(); ++i) {
		net->name_index_dic[node->value_range[i]] = i;
	}
	net->name_index_dic[node->node_name] = net->variables.size();
	net->variables.push_back(node);
	curLine += 2;
}

void parse_net_probability(const vector<string>& slice, int& curLine, BNNet* net)
{	
	if (str_contain(slice[curLine], '|')) {
		parse_net_cond_prob(slice, curLine, net);
	}
	else {
		parse_net_prior_prob(slice, curLine, net);
	}
}

/*
	probability(Earthquake) {
		table 0.02, 0.98;
	}
*/
void parse_net_prior_prob(const vector<string>& slice, int& curLine, BNNet* net)
{
	string nodeName = str_trim_space(split(split(slice[curLine], '(')[1], ')')[0]);
	//cout << nodeName << endl;
	BNNode* node = net->variables[net->name_index_dic[nodeName]];
	curLine++;
	// 前面还一个 "table"
	vector<string> probs = split(split(str_trim_space(slice[curLine]), ';')[0], ' ');
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


/*
	probability ( Alarm | Burglary, Earthquake ) {
	  (True, True) 0.95, 0.05;
	  (False, True) 0.29, 0.71;
	  (True, False) 0.94, 0.06;
	  (False, False) 0.001, 0.999;
	}
*/
void parse_net_cond_prob(const vector<string>& slice, int& curLine, BNNet* net) 
{
	string nodeName = str_trim_space(split(split(split(slice[curLine], '(')[1], ')')[0], '|')[0]);
	vector<string> parents = split(split(split(split(slice[curLine], '(')[1], ')')[0], '|')[1], ',');

	BNNode* node = net->variables[net->name_index_dic[nodeName]];
	cout << nodeName << endl;
	for (string& pa : parents) {
		pa = str_trim_space(pa);
		//cout << "\t" << pa << "\t\n";
		BNNode* parentNode = net->variables[net->name_index_dic[pa]];
		node->parents.insert(parentNode);
		parentNode->children.insert(node);
	}
	curLine++;
	while (!slice[curLine].empty() && slice[curLine][0] != '}') {
		//process cond_prob
		//(True, True) 0.95, 0.05;
		//...
		vector<string> status = split(split(split(slice[curLine], ')')[0], '(')[1], ',');
		vector<string> probs = split(split(split(slice[curLine], ')')[1], ';')[0], ',');
		for (int i = 0; i < parents.size(); ++i) {
			const string parentName = parents[i];
			BNNode* parentNode = net->variables[net->name_index_dic[parentName]];
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

void verify_sample_prob(const vector<vector<string>>& sample)
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
		auto temp = split(str, '|');
		int count = 0, total = 0;
		double result = 0;
		if (temp.size() == 1) {
			// 先验
			query = temp[0];
			output = ("P(" + query + ")");
			vector<string> queryVec = split(query, ':');
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
			vector<string> queryVec = split(query, ':');
			output += queryVec[0] + ":" + queryVec[1] + "|";

			vector<string> singleCond = split(cond, ',');
			vector<vector<string>> condVec;

			for (string& str : singleCond) {
				vector<string> temp = split(str, ':');
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

