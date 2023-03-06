#include <iostream>
using namespace std;
class Mail_adress {
	long index;
	string area;
	string city;
	string street;
	int house;
	int flat;
public:

	Mail_adress(long a = 0, string b = "", string c = "", string d = "",
		int e = 0, int f = 0) {
		index = a;
		area = b;
		city = c;
		street = d;
		house = e;
		flat = f;
	}

	void change_index(long a) {
		index = a;
	};

	void change_area(string b) {
		area = b;
	};

	void change_city(string c) {
		city = c;
	};

	void change_street(string d) {
		street = d;
	};

	void change_house(int e) {
		house = e;
	};

	void change_flat(int f) {
		flat = f;
	};

	friend ostream& operator<<(ostream& s, Mail_adress& A) {
		s << A.index << ", " << A.area << ", " << A.city << ", " << A.street << ", " << A.house << ", " << A.flat;
		return s;
	}

};

int main() {
	int mode = -1;
	long index = -1;
	string area = "";
	string city = "";
	string street = "";
	int house = -1;
	int flat = -1;
	Mail_adress A;

	cout << "0: initialize the whole address" << endl;
	cout << "1: change the index" << endl;
	cout << "2: change the area" << endl;
	cout << "3: change the city" << endl;
	cout << "4: change the street" << endl;
	cout << "5: change the house" << endl;
	cout << "6: change the flat number" << endl;
	cout << "7: cout your adress" << endl;

	while (mode != 8) {
		cout << "mode type:" << endl;
		cin >> mode;
		switch (mode) {
			case 0: {
				cout << "Write your index, area, city, street, house, flat with ' ' between parameters" << endl;
				cin >> index >> area >> city >> street >> house >> flat;
				A.change_index(index);
				A.change_area(area);
				A.change_city(city);
				A.change_street(street);
				A.change_house(house);
				A.change_flat(flat);
				break;
			}
			case 1: {
				cout << "Write your index" << endl;
				cin >> index;
				A.change_index(index);
				break;
			}
			case 2: {
				cout << "Write your area" << endl;
				cin >> area;
				A.change_area(area);
				break;
			}
			case 3: {
				cout << "Write your city" << endl;
				cin >> city;
				A.change_city(city);
				break;
			}
			case 4: {
				cout << "Write your street" << endl;
				cin >> street;
				A.change_street(street);
				break;
			}
			case 5: {
				cout << "Write your house" << endl;
				cin >> house;
				A.change_house(house);
				break;
			}
			case 6: {
				cout << "Write your flat" << endl;
				cin >> flat;
				A.change_flat(flat);
				break;
			}
			case 7: {
				cout << "How it looks like with your parameters: " << A << endl;
				break;
			}
			default: break;
		}
	}
	return 0;
}