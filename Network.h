#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>


using namespace std;

<<<<<<< HEAD
class Network
{
	int GROUPS; //���������� �����
	int COMMUTATORS; //���������� ������������ � ������
	int NODES; //���������� ����� �� ����������
	int BANDWIDTH_IN_GROUP; //���������� ����������� ������ ������
	int BANDWIDTH_BETWEEN_GROUP; //���������� ����������� ����� ��������
=======
class Network {

	int GROUPS = 5; //���������� �����
	int COMMUTATORS= 2; //���������� ������������ � ������
	int NODES = 1; //���������� ����� �� ����������
	int BANDWIDTH_IN_GROUP = 4; //���������� ����������� ������ ������
	int BANDWIDTH_BETWEEN_GROUP = 2; //���������� ����������� ����� ��������
>>>>>>> origin/TestingMain


	//int count_groups; 
	int count_commutators;
	int count_nodes;
	int count_vertex; //���������� ���� ������ (���� + �����������)
	//int bandwidth_in_group;
	//int bandwidth_between_group;
	vector<vector<int>> messages;
	vector<vector<string>> status;
<<<<<<< HEAD
	vector<vector<int>> path;
=======
>>>>>>> origin/TestingMain
	
	//vector<vector<int>> matrix_adj; //������� ���������
	//vector<vector<int>> matrix_wght; //������� �����
	//vector<int> vertex_degree; //������� ������
	vector<vector<int>> matrix_bandwidth; //������� ��������� ������������
	//vector<vector<int>> matrix_cost; //������� ���������
	vector<vector<int>> matrix_load; //������� �������������


	void print_matrix(string str, vector<vector<int>> matr);
	

	bool CheckBoolVector(vector<bool>);
	int Min_Index(vector<int>, vector<bool>, int cost);

public:
	vector<int> Dijkstra_algorythm(vector<int> message);//, vector<vector<int>> matrix);
	Network();
<<<<<<< HEAD
	Network(int GROUPS,	int COMMUTATORS, int NODES, int BANDWIDTH_IN_GROUP, int BANDWIDTH_BETWEEN_GROUP);
	void Print_matrix_bandwidth();
	void NextStep();
=======
	
	void Print_matrix_bandwidth();
	
	void menu();
	int checking_input(int max);
	void adding_msg();

>>>>>>> origin/TestingMain
};

