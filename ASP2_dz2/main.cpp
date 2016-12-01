#include "Main_structure.h"
#include "Interface.h"



int main() {
	Data_w_tree d;

	cout << "Da li zelite da se stringovi ucitaju iz fajla? d/n: ";
	
	char c;
	cin >> c;
	
	if (c == 'd') {
		cout << endl << "Ime fajla: ";
		string ime;
		cin >> ime;
		
		ifstream dat;
		dat.open(ime);
		
		string rec;
		while (dat.eof() == false) {
			getline(dat, rec);
			d += rec;
		}
	}
	else if (c != 'n') { return EXIT_FAILURE; }

	main_interface();
	cin >> c;

	while (c != 'q') {
		string s;
		int index;

		switch (c) {
		case 'n':
			new_interface();
			cin >> s;
			if (d.find(s) != -1) { new_interface_failure(); }
			else { d += s; new_interface_success; }
			break;

		case 'f':
			find_interface();
			cin >> s;
			index = d.find(s);
			if (index == -1) { find_interface_failure(); }
			else { find_interface_success(index); }
			break;

		case 'd':
			del_interface();
			cin >> s;
			if (d.find(s) == -1) { del_interface_failure; }
			else { d -= s; del_interface_success; }
			break;

		case 'i':
			write_interface();
			cout << d;
			break;

		case 'b':
			write_btree_interface();
			out(cout, d);
			break;

		default:
			cout << "Greska." << endl;
		}

		main_interface;
		cin >> c;
	}
	quit_interface();

	return EXIT_SUCCESS;
}