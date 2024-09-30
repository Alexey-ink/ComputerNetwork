#include "Network.h"

Network::Network():Network(5,2,1,4,2) {}

Network::Network(int GROUPS, int COMMUTATORS, int NODES, int BANDWIDTH_IN_GROUP, int BANDWIDTH_BETWEEN_GROUP)  {
	this->GROUPS = GROUPS; 
	this->COMMUTATORS = COMMUTATORS;
	this->NODES = NODES;
	this->BANDWIDTH_IN_GROUP = BANDWIDTH_IN_GROUP;
	this->BANDWIDTH_BETWEEN_GROUP = BANDWIDTH_BETWEEN_GROUP;

	count_commutators = GROUPS * COMMUTATORS; 
	count_nodes = count_commutators * NODES; 
	count_vertex = count_commutators + count_nodes;

	vector<vector<int>> new_bandwidth(count_vertex, vector<int>(count_vertex, 0));
	matrix_bandwidth = new_bandwidth;


	for (int i = 0, ii=count_nodes, k = NODES; i < count_nodes;i++) {
		k--;
		matrix_bandwidth[i][ii] = BANDWIDTH_IN_GROUP; //���������� ����������� � ������
		matrix_bandwidth[ii][i] = BANDWIDTH_IN_GROUP;
		if (k == 0) {
			ii++;
			k = NODES;
		}	
	}


	for (int i = 0, back=COMMUTATORS-1, group=GROUPS-1; i < COMMUTATORS; i++, back--, group-=GROUPS/COMMUTATORS) {


		for (int ii = count_nodes + i, group_right=group ; ii < count_vertex; ii+=COMMUTATORS, group_right++) {
			if (group_right >= GROUPS) {
				group_right -= GROUPS;
			}
			for (int iii = 0, current_group = group_right; iii < GROUPS/COMMUTATORS; iii++) {
				current_group = (group_right - iii) < 0 ? GROUPS + (group_right - iii) : group_right - iii;
				int buf = count_nodes + current_group * COMMUTATORS + back; 
				matrix_bandwidth[ii][count_nodes + current_group * COMMUTATORS + back] = BANDWIDTH_BETWEEN_GROUP;//���������� ����������� ����� ��������
				matrix_bandwidth[count_nodes + current_group * COMMUTATORS + back][ii] = BANDWIDTH_BETWEEN_GROUP;
			}
		}
	}

	for (int i = 0; i < GROUPS; i++) {
		for (int ii = count_nodes + i * COMMUTATORS; ii < count_nodes + (i + 1) * COMMUTATORS; ii++) {
			for (int iii = ii; iii < count_nodes + (i + 1) * COMMUTATORS; iii++) {
				if (ii != iii) {
					matrix_bandwidth[ii][iii] = BANDWIDTH_IN_GROUP;//���������� ����������� � ������
					matrix_bandwidth[iii][ii] = BANDWIDTH_IN_GROUP;
				}
			}
		}
	}

	matrix_load = matrix_bandwidth;
}

void Network::print_matrix(string str, vector<vector<int>> matr) {
	
	string s = to_string(matr.size());
	cout << "\n   ";
	cout << str << "\n\n";

	for (int i = 0; i < s.size(); i++) {
		cout << " ";
	}
	cout << " | ";
	for (int i = 0; i < matr[0].size(); i++) {
		if (i != 0)
			cout << setw(4);
		else
			cout << setw(2);
		cout << i;
	}
	cout << endl;

	for (int i = 0; i < s.size(); i++) {
		cout << "-";
	}
	for (int i = 0; i < matr[0].size(); i++) {
		cout << "----";
	}
	cout << endl;
	for (int i = 0; i < matr.size(); i++) {
		cout << setw(s.size()) << i << " | ";
		for (int j = 0; j < matr[i].size(); j++) {
			if (j != 0)
				cout << setw(4);
			else
				cout << setw(2);
			cout << matr[i][j];
		}
		cout << "\n";
	}
	cout << "\n";
}


void Network::Print_matrix_bandwidth() {
	print_matrix("Matrix_adj", matrix_bandwidth);	
}


vector<int> Network::Dijkstra_algorythm(vector<int> message) {
	int index_from = message[0];
	int index_to = message[2];
	int cost = message[1];

	int count = 0;
	vector<int> vertex(count_vertex);
	for (int i = 0; i < vertex.size(); i++) {
		vertex[i] =  INT_MAX;
	}
	vector<bool> vertex_check(count_vertex, false);
	vector<vector<int>> route(count_vertex);
	vertex[index_from] = 0;

	int current;

	while (CheckBoolVector(vertex_check) == false) {
		current = Min_Index(vertex, vertex_check, cost) ;
		if (current == -1)
			break;
		vertex_check[current] = true;

		for (int i = 0; i < matrix_load[current].size(); i++) {
			if (matrix_load[current][i] != 0) {
				int buf = cost / matrix_load[current][i];
				buf = cost % matrix_load[current][i]==0? buf :buf + 1;
				if (!(current == index_from || current == index_to)) {
					buf++; //���������� �����
				}
				bool condition = vertex[current] + buf <= vertex[i];
					if (condition) {

					vertex_check[i] = false;
					route[i].clear();
					route[i] = route[current];
					route[i].push_back(current);
					vertex[i] = vertex[current] + buf;
				}
			}
		}
	}
	for (int i = 0; i < vertex.size(); i++) {
		if (vertex[i] == INT_MAX || vertex[i] == INT_MAX + 1)
			vertex[i] = 0;
	}
	for (int i = 0; i < route.size(); i++) {
		if (route[i].size() != 0)
			route[i].push_back(i);
	}

	
	vector<int> current_route;
	for (int i = 0; i < route[index_to].size() - 1; i++) {
		int index_1;
		int index_2;
		int cost_inner;
		int broad;
		if (i != route[index_to].size() - 2) {
			index_1 = route[index_to][i];
			index_2 = route[index_to][i + 1];
			cost_inner = matrix_load[index_1][index_2];

			broad = cost / cost_inner;
			broad = cost % cost_inner == 0 ? broad : broad+1;
			for (int ii = 0; ii < broad; ii++) {
				current_route.push_back(index_1);
				current_route.push_back(index_2);
			}

			//������� �����
			current_route.push_back(index_2);
			current_route.push_back(index_2);
		}
		else {
			cost_inner = this->BANDWIDTH_IN_GROUP;
			index_1 = route[index_to][i];
			index_2 = route[index_to][i+1];

			broad = cost / cost_inner;
			broad = cost % cost_inner == 0 ? broad : broad+1;
			for (int ii = 0; ii < broad; ii++) {
				current_route.push_back(index_1);
				current_route.push_back(index_2);
			}
		}
	}

	return current_route;
}


int Network::Min_Index(vector<int> v, vector<bool> fl, int cost) {
	int result = INT_MAX;
	int index = -1;
	for (int i = 0; i < v.size(); i++) {
		if (v[i] < result && fl[i] == false) {
			result = v[i];
			index = i;
		}
	}
	return index;
}


bool Network::CheckBoolVector(vector<bool> v) {
	bool result = true;
	for (int i = 0; i < v.size(); i++) {
		result = result && v[i];
	}
	return result;
}


int Network::checking_input(int max) {
	int n;

	while (true) {
		cin >> n;
		// ���������, �������� �� ��������� �������� ����� ������ � ��������� � ���������� ��������
		if (cin.fail() || n < 0 || n > max || cin.peek() != '\n') {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ������� ����� �����
			cout <<endl<< "������! ������������ ����! " << endl;
		}
		else {
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			break; // ���� ���������, ������� �� �����
		}
	}
	return n;
}


void Network::adding_msg() {
	string input;
	int dep, length, arr;
	char space1, space2;

	cout << "\n������� ��������� � ������� �����_��������� �����_��������� �����_����������, " << endl;
	cout << "��� ������ ��������� � �������� �� 0 �� " << count_nodes - 1 << " ������������" << endl;

	do {
	int flag_out1 = false;
	int flag_out2 = false;

		getline(cin, input);
		istringstream iss(input);

		if (iss >> dep >> length >> arr) {

			if (dep == arr) {
				cout << "������� ��������� ���� ����������� � ��������" << endl;
			}
			else if (length <= 0) {
				cout << "����� ��������� ������ ���� �������������!" << endl;
			}
			else if (dep >= 0 && dep < count_nodes && arr >= 0 && arr < count_nodes && iss.eof()) {
				flag_out1 = true;
			}
			else {
				cout << "����������� ����! ���������� �����" << endl;
			}
		}
		else cout << "����������� ����! ���������� �����" << endl;

		if (messages.size() == 0) {
			flag_out2 = true;
		}
		else {
			for (int i = 0; i < messages.size(); i++) {
				if (dep != messages[i][0] ||
					length != messages[i][1] ||
					arr != messages[i][2]) {
					flag_out2 = true;
				}
			}
		}

		if (flag_out2 == false) {
			cout << "����� �������� ��� ����������! ���������� �����" << endl;
		}

		if (flag_out1 && flag_out2)
			break;

	} while (true);

	this->AddMessage({ dep, length, arr });
}

string Network::statusInfo(int index) {
	if (status[index].size() != 0) 
		return status[index][0];
	
	else {

		int a = path[index][0];
		int b = path[index][1];

		if (path[index][0] == path[index][1]) {
			string str = "�����. ��������� � " + to_string(a) + " �����������";
			return str;
		}

		int done = messages[index][1] - reminder[index] + reserv[index];
		if (done > messages[index][1]) done = messages[index][1];

		string str1, str2;
		if (a >= count_nodes) str1 = "�� ����������� " + to_string(a);
		else str1 = "�� ���� " + to_string(a);

		if (b >= count_nodes) str2 = " � ���������� " + to_string(b);
		else str2 = " � ���� " + to_string(b);

		string str = str1 + str2 + " �������� " + to_string(done) + "/" + to_string(messages[index][1]);

		return str;

	}
}



void Network::menu() {
	int msg_count;

	while (true) {
		cout << "������ ���������: \n\n";
		if (messages.size() != 0) {
			for (int i = 0; i < messages.size(); i++) {
				cout << "{" << messages[i][0] << "," << messages[i][1] << "," << messages[i][2];
				cout << "} : " << statusInfo(i) << endl;
			}
		}
		else cout << "��� ��������� ��� ���������!" << endl;

		cout << "\n �������� ��������: \n";
		cout << " [1] ��������� ��������� (�� 10 ����) \n";
		if (messages.size() != 0)
			cout << " [2] ��������� ��� \n";
		cout << " [0] ������� �������� \n";

		int b;
		if (messages.size() == 0)
			b = checking_input(1);
		else
			b = checking_input(2);

		switch (b) {
		case 1:
			cout << "������� ���������� ���������, ������� ������ ��������� (�� 0 �� 10): ";
			msg_count = checking_input(11);

			for (int i = 0; i < msg_count; i++) {
				adding_msg();
			}
			printf("\n----------------------------------------------------------------------------\n");
			break;

		case 2:
			printf("\n----------------------------------------------------------------------------");
			cout << "\n\t\t��������� ���.\n\n";
			NextStep();
			break;

		case 0:
			cout << "\n����� � ������� ����...\n";
			return;
		}
	}
};


void Network::AddMessage(vector<int> message) {
	vector <int> path;
	this->messages.push_back(message);
	if (Can_Make_Route(message))
		path = Dijkstra_algorythm(message);
	this->path.push_back(path);
	this->status.push_back({ "��������� ������� ��������" });
	this->reminder.push_back(message[1]);

	if (Can_Make_Route(message)) {
		int can_send = message[1] < matrix_load[path[0]][path[1]] ? message[1] : matrix_load[path[0]][path[1]];
		this->reserv.push_back(can_send);
		matrix_load[path[0]][path[1]] -= can_send;
	}
	else
		this->reserv.push_back(0);
}

void Network::printInfo() {

	cout << "\n���������� �����: " << GROUPS << endl;
	cout << "���������� ������������ � ������: " << COMMUTATORS << endl;
	cout << "���������� ����� �� ����������: " << NODES << endl;
	cout << "���������� ����������� ������ ������: " << BANDWIDTH_IN_GROUP << endl;
	cout << "���������� ����������� ����� ��������: " << BANDWIDTH_BETWEEN_GROUP << endl;
	printf("\n\n----------------------------------------------------------------------------\n");
}

void Network::NextStep() {

	for (int i = 0; i < messages.size(); i++) {
		
		if (status[i].size() == 0) {//��������������� �������� ����� ��������� ��� ����������
			
			int local_from = path[i][0]; //����� � ������� ������ ��������� ���������
			int local_to = path[i][1]; //����� � ������� ���������� ��� �� ������ ����
			int index_from = messages[i][0]; //�����������
			int index_to = messages[i][2]; //����������
			int message_cost = messages[i][1]; //����� ���������


			if (local_from == path[i][1]) {// ���ya��� ����� ��������� ��������� � ����� ���� �� �� ������ (�� ���� ������ �� ����������)
				reserv[i] = 0;
				bool check = false;
				for (int ii = 0; ii < count_vertex; ii++) {
					if (matrix_load[ii][index_to] != 0) {
						check = true;
						break;
					}
				}
				if (check) {
					vector<int> new_path = Dijkstra_algorythm({ local_from, message_cost, index_to });
					new_path.insert(new_path.begin(), { path[i][0],path[i][1] });
					path[i] = new_path;
					reminder[i] = message_cost;

					int can_send = message_cost < matrix_load[path[i][2]][path[i][3]] ? message_cost : matrix_load[path[i][2]][path[i][3]];

					reserv[i] = can_send;
					matrix_load[path[i][2]][path[i][3]] -= can_send;
					matrix_load[path[i][3]][path[i][2]] -= can_send;
				}
				else {
					string status = "������� ������������ ����. ��������� � " + to_string(local_from) + " �����������";
					this->status[i].push_back({ status });
				}
			}
			else { //�������� ����� ���������� ��������� �� ������ ���� � ������
				//���������� ����������� ����������

				if (matrix_load[local_from][local_to] > 0) {
					int can_send = reminder[i] < matrix_load[local_from][local_to] ? reminder[i] : matrix_load[local_from][local_to];

					int need_steps = reminder[i] / can_send;
					need_steps = reminder[i] % can_send == 0 ? need_steps : need_steps + 1;

					matrix_load[local_from][local_to] -= can_send;
					matrix_load[local_to][local_from] -= can_send;
					reserv[i] = can_send;

					if (path[i].size() > 2) {//��������� ����� ���������� ���� ���������� ����������� �����������

						int old_steps = 0;//������� ������� ���� ����� 
						for (int ii = 0; ii < path[i].size() - 2; ii += 2) {
							if (local_from == path[i][ii] && local_to == path[i][ii + 1]) {
								old_steps++;
							}
							else {
								break;
							}
						}
						if (need_steps< old_steps) {
							
							for (int iii = 0; iii < old_steps - need_steps; iii++) {
								path[i].erase(path[i].begin());
								path[i].erase(path[i].begin());
							}

						}
					}
					reminder[i] -= can_send;
				}
				else {
					reminder[i] -= reserv[i];
				}

			}
				//� ����� ���� ��� ����� ������� ������ ���
				if (path[i].size() > 2) {
					if (reserv[i] == 0) {
						matrix_load[path[i][0]][path[i][1]] += reserv[i];
						matrix_load[path[i][1]][path[i][0]] += reserv[i];
					}
					path[i].erase(path[i].begin());
					path[i].erase(path[i].begin());

				}
				else {
					messages.erase(messages.begin() + i);
					matrix_load[path[i][0]][path[i][1]] += reserv[i];
					matrix_load[path[i][1]][path[i][0]] += reserv[i];
					path.erase(path.begin() + i);
					status.erase(status.begin() + i);
					reminder.erase(reminder.begin() + i);
					reserv.erase(reserv.begin() + i);
					i--;
				}
		}
		else if(path[i].size()>0 && path[i][0]!= messages[i][0]) {// ����� � ������, � ����� �������� ����������

			int local_from = path[i][0]; //����� � ������� ������ ��������� ���������
			int local_to = path[i][1]; //����� � ������� ���������� ��� �� ������ ����
			int index_from = messages[i][0]; //�����������
			int index_to = messages[i][2]; //����������
			int message_cost = messages[i][1]; //����� ���������

				reserv[i] = 0;
				bool check = false;
				for (int ii = 0; ii < count_vertex; ii++) {
					if (matrix_load[ii][index_to] != 0) {
						check = true;
						break;
					}
				}
				if (check) {
					vector<int> new_path = Dijkstra_algorythm({ local_from, message_cost, index_to });
					new_path.insert(new_path.begin(), { path[i][0],path[i][1] });
					path[i] = new_path;
					reminder[i] = message_cost;

					int can_send = message_cost < matrix_load[path[i][2]][path[i][3]] ? message_cost : matrix_load[path[i][2]][path[i][3]];

					reserv[i] = can_send;
					matrix_load[path[i][2]][path[i][3]] -= can_send;
					matrix_load[path[i][3]][path[i][2]] -= can_send;
				}
		}
	}



	for (int i = 0; i < messages.size(); i++) {
	
		if (status[i].size() != 0) {
			if (path[i].size() > 0 && path[i][0] != messages[i][0]) { //����� ������� ������������ ����
				bool check = false;
				for (int ii = 0; ii < count_vertex; ii++) {
					if (matrix_load[ii][messages[i][2]] != 0) {
						check = true;
						break;
					}
				}
				if (check) {
					status[i].erase(status[i].begin());
				}
			}
			else {
				if (Can_Make_Route(messages[i])) {//��� ��� ��������� ������
					status[i].erase(status[i].begin());
					if (reserv[i] == 0) {
						path[i] = Dijkstra_algorythm(messages[i]);
						int can_send = messages[i][1] < matrix_load[path[i][0]][path[i][1]] ? messages[i][1] : matrix_load[path[i][0]][path[i][1]];
						reserv[i] = can_send;
						matrix_load[path[i][0]][path[i][1]] -= can_send;
						matrix_load[path[i][1]][path[i][0]] -= can_send;
					}
				}
			}
		}
	}
	matrix_load = matrix_bandwidth;
}


bool Network::Can_Make_Route(vector<int> message) {
	bool flag1 = false;
	bool flag2 = false;
	bool flag3 = false;
	for (int i = 0; i < messages.size(); i++) {
		if (message[0] == messages[i][0] &&
			message[1] == messages[i][1] &&
			message[2] == messages[i][2]){

			if (i < reserv.size() && reserv[i] > 0) {
				flag1 = true;
			}
		}
	}
	for (int i = 0; i < count_vertex; i++) {
		if (matrix_load[message[0]][i])
			flag2 = true;
	}
	
	return flag1 || flag2;
}
