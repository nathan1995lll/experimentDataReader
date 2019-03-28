#include "function.h"
/*
*���ͺ���
*��stringת��ΪType(e.g. double,int,float,long,etc.)
*/
template <class Type>
Type stringToNum(const std::string& str)
{
	std::istringstream iss(str);
	Type num;
	iss >> num;
	return num;
}
/*
*��ȡ�����Լ�����allen����
***************
*���룺
**origin��ԭʼ���ݴ��·��
**destination�����������ݴ��·��
**position��Ҫ��ȡ����������
**����allen���������Ĭ��ֵΪ0
**allen����������洢·����Ĭ��ֵΪ""
***************
*Ĭ������¸ú�����һ������Ԥ������
*/
void process(std::string origin, std::string destination, std::vector<std::string>position, unsigned int num_pts, std::string result) {
	std::fstream fin(origin, std::ios::in);
	std::string row;
	std::vector<std::string> row_separated;
	std::vector<double> numbers;
	std::vector<int> index;
	int flag = 0;
	std::vector<std::vector<double>> data;
	data.resize(position.size());
	std::ofstream fout(destination, std::ios::out);
	while (std::getline(fin, row)) {
		if (isdigit(row[0])) {
			row_separated = separate(row);
			numbers = str2num(row_separated, index);
			for (int i = 0; i != index.size(); ++i)
				data[i].push_back(numbers[i]);
		}
		else if (row[0] == 'T'&&flag == 0) {
			row_separated = separate(row);
			index = get_index(row_separated, position);
			flag = 1;
		}
	}
	double holder = 0.0;
	//ʱ��������
	for (auto i = data[0].begin(); i != data[0].end(); i++) {
		if (*i == 0 && i != data[0].begin()) {
			holder = *(i - 1) + 0.01;
		}
		*i += holder;
	}
	double interval = (*(data[0].end() - 1) - *(data[0].begin())) / data[0].size();
	int i = 0;
	for (auto &e : data[0]) {
		e = 0 + i * interval;
		++i;
	}
	//ȥ���쳣ֵ
	for (auto v = data.begin() + 1; v != data.end(); ++v) {
		for (auto &j : *v)
			if (abs(j) > 0.1)
				j = 0.03;
	}
	for (auto v : data) {
		for (auto i = v.begin(); i != v.end(); ++i) {
			if (i != v.end() - 1)
				fout << *i << ',';
			else
				fout << *i << std::endl;
		}
	}
	fin.close();
	fout.close();
}

/*
*�������룬ѡ���ȡ������
*���룺��ͷ��Ҫ������������
*���������
**************
*e.g. header["time","data1","data2","data3"],position["time","data2"]->[0,2]
**************
*/
std::vector<int> get_index(std::vector<std::string> header, std::vector<std::string> position) {
	std::vector<int> index;
	for (auto a : position) {
		int i = 0;
		for (auto b : header) {
			if (a == b) {
				index.push_back(i);
			}
			++i;
		}
	}
	return index;
}
/*
*����ȡ��һ�����ݷֿ�
*���룺�����ָ������ַ���
*������ַ���vector
**************
*e.g. "12  12 34 17" -> ["12","12","34","17"]
**************
*/
std::vector<std::string> separate(std::string row) {
	std::vector<std::string> row_separated;
	std::string temp;
	for (auto i : row) {
		if (isspace(i)) {
			if(!temp.empty())
				row_separated.push_back(temp);
			temp = "";
		}
		else {
			temp += i;
		}
	}
	return row_separated;
}
/*
*�Ѵ�txt�ļ������ȡ���ַ�������ת��Ϊdouble������
*���룺�ַ���vector,Ҫ��ȡ������
*������ַ�����Ӧ������vector
***************
*e.g. row_seperated["12","12","34","17"],index[0,2]->[12,34]
***************
*/
std::vector<double> str2num(std::vector<std::string> row_separated, std::vector<int> index) {
	std::vector<double> numbers;
	for (auto i : index) {
		numbers.push_back(stringToNum<double>(row_separated[i]));
	}
	return numbers;
}
/*
*����allen�����������浽result
*���룺����data��ÿһ�д���ͬ�����ݣ���һ��Ϊʱ��
*������������ݶ�ʱ���allen����
*/

/*
void allen(std::vector<std::vector<double>> data, unsigned int num_pts,std::string result) {
	std::vector<double> time = data[0];
	double interval = time[1] - time[0];
	double start = 1,end = 1e5;
	double log_interval = (log10(end) - log10(start)) / num_pts;
	std::vector<double> log_index;
	std::ofstream out(result, std::ios::out);
	for (int i = 0; i != num_pts; ++i) {
		log_index.push_back(log_interval*i + log10(start));
	}
	std::vector<std::vector<double>>output;
	output.resize(data.size());
	for (auto ind : log_index) {
		double tau = pow(10, ind);
		output[0].push_back(tau);
		int m = (int)(tau / interval);
		for (int i = 1; i != data.size(); ++i) {
			double sum = 0;
			for (int j = 0; j != data[i].size()-2*m-1; ++j) {
				sum += (data[i][j + 2 * m] + data[i][j] - 2 * data[i][j + m])*(data[i][j + 2 * m] + data[i][j] - 2 * data[i][j + m]);
			}
			output[i].push_back(sum / (2 * tau*tau*data[0].size()));
		}
			
	}
	for (auto v : output)
		for (auto ite = v.begin(); ite != v.end();++ite) {
			if (ite != v.end() - 1)
				out << *ite << ",";
			else
				out << *ite << std::endl;
	}
	out.close();
	
}

std::vector<std::vector<double>>load(std::string destination) {
	std::vector<std::vector<double>>data;
	data.emplace_back();
	std::fstream cleaned(destination, std::ios::binary | std::ios::in);
	int flag = 0;
	double temp;
	while (cleaned >> temp) {
		if (abs(temp - '\n')<1e-3) {
			data.emplace_back();
			++flag;
		}
		else if (abs(temp - ',') < 1e-3) {
			cleaned >> temp;
		}
		else {
			data[flag].push_back(temp);
		}
	}
	return data;
}
*/