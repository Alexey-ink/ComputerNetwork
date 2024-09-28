#include "Network.h"

Network::Network() {
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

	for (int i = 0, back=COMMUTATORS-1, group=GROUPS-1; i < COMMUTATORS; i++, back--, group-=COMMUTATORS) {

		for (int ii = count_nodes + i, group_right=group ; ii < count_vertex; ii+=COMMUTATORS, group_right++) {
			if (group_right >= GROUPS) {
				group_right -= GROUPS;
			}
			for (int iii = 0, current_group = group_right; iii < COMMUTATORS; iii++) {
				current_group = (group_right - iii) < 0 ? GROUPS + (group_right - iii) : group_right - iii;
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

int Network::checking_input(int max) {
	int n;

	while (true) {
		cin >> n;
		// ���������, �������� �� ��������� �������� ����� ������ � ��������� � ���������� ��������
		if (cin.fail() || n < 0 || n > max || cin.peek() != '\n') {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ������� ����� �����
			cout << "������! ������������ ����! " << endl;
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

	cout << "������� ��������� � ������� �����_��������� �����_��������� �����_����������, " << endl;
	cout << "��� ������ ��������� � �������� �� 0 �� " << count_nodes - 1 << " ������������" << endl;

	do {

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
				break;
			}
			else {
				cout << "����������� ����! ���������� �����" << endl;
			}
		}
		else cout << "����������� ����! ���������� �����" << endl;

	} while (true);

	messages.push_back(vector<int>());
	messages.back().push_back(dep);
	messages.back().push_back(length);
	messages.back().push_back(arr);
	
	print_matrix("������� ���������", messages);

}


void Network::menu() {

	cout << "���������, ������������ ������ ��������� �������� ��������� � ���������������� ���� ���������������" << endl;

	bool secondMenu = false;

	while (true) {
		if (!secondMenu) {

			cout << "\n �������� ��������: \n";
			cout << " [1] ��������� ��������� \n";
			cout << " [0] ����� �� ��������� \n";

			int b = checking_input(1);

			switch (b) {

			case 1:
				adding_msg();
				secondMenu = true;
				break;


			case 0:
				cout << "����� �� ���������...\n";
				return;
			}
		}
		else {

			cout << "\n �������� ��������: \n";
			cout << " [1] ��������� ��������� (�� 10 ����) \n";
			cout << " [2] ��������� ��� \n";
			cout << " [0] ����� �� ��������� \n";

			int b = checking_input(2);
			int msg_count;

			switch (b) {
			case 1:

				cout << "������� ���������� ���������, ������� ������ ��������� (�� 0 �� 10): ";
				msg_count = checking_input(11);

				for (int i = 0; i < msg_count; i++) {
					adding_msg();
				}
				
				break;

			case 2:
				if (messages.size() == 0) {
					cout << " ��� �������� ��������� �������� ���������!" << endl;
					secondMenu = false;
					break;
				}

				cout << "������� � ���������� ����.\n";
				break;

			case 0:
				cout << "����� �� ���������...\n";
				return;
			}
		}
	}
}




