#include <iostream>
using namespace std;

struct Server {
	static string* mas_of_places;
	static int amount_of_places;
	static string* times_of_request;

	Server(int version);
	virtual void point_place(string s) = 0;

	void enter_request(string s) {
		mas_of_places[amount_of_places-1] = s;
	}

	friend std::ostream& operator<<(std::ostream& os, const Server& t) {
		for (int i = 0; i < t.amount_of_places; i++) {
			os << "Time: " << t.times_of_request[i];
			os << "; Place: " << t.mas_of_places[i] << endl;
		}
		return os;
	}
};

string* Server::mas_of_places = nullptr;
string* Server::times_of_request = nullptr;
int Server::amount_of_places = 0;

Server::Server(int version) {
	if (version != 2) {
		if (amount_of_places == 0) {
			mas_of_places = new string[1];
			mas_of_places[amount_of_places] = "";
		}
		else {
			string* temp = new string[amount_of_places + 1];
			for (int i = 0; i < amount_of_places; i++) {
				temp[i] = mas_of_places[i];
			}
			temp[amount_of_places] = "";
			mas_of_places = temp;
		}
		cout << "Point the time of your request (hours:minutes:seconds):" << "\n";
		string s = "";
		cin >> s;
		if (amount_of_places == 0) {
			times_of_request = new string[1];
			times_of_request[0] = s;
		}
		else {
			string* temp = new string[amount_of_places + 1];
			for (int i = 0; i < amount_of_places; i++) {
				temp[i] = times_of_request[i];
			}
			temp[amount_of_places] = s;
			times_of_request = temp;
		}
		amount_of_places += 1;
	}
}

class person_request : public Server {
public:
	void point_place(string s) {
		enter_request(s);
	}
	person_request(string pl=""): Server(1){
		point_place(pl);
	}
	person_request(int a, int b):Server(2) {}
};

int main() {
	int i = -1;
	while (i != 2) {
		cout << "1: Make a request to 'server'" << "\n";
		cout << "2: leave the program" << "\n";
		cin >> i;
		if (i == 1) {
			cout << "Point your location" << "\n";
			string st;
			cin >> st;
			person_request A(st);
		}
		if (i == 2) {
			int a = 0;
			int b = 0;
			person_request A(a, b);
			cout << A;
		}
	}
	if (Server::mas_of_places != nullptr)
	{
		delete[] Server::mas_of_places;
	}
	if (Server::times_of_request != nullptr)
	{
		delete[] Server::times_of_request;
	}
	return 0;
}