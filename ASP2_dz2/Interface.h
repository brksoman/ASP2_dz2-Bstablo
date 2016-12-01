#ifndef _interface_h_
#define _interface_h_

#include <iostream>
using namespace std;


void main_interface() {
	cout << endl << "Kontrole:" << endl << "\tn - Unos novog stringa u bazu." << endl << "\tf - Pronalazenje stringa u bazi." << endl;
	cout << "\td - Brisanje stringa iz baze." << endl << "\ti - Ispis stringova u bazi." << endl << "\tb - Ispis B-stabla indekasa nad bazom.";
	cout << "\tq - Zavrsetak programa.";
	cout << endl << endl << "\t";
}

void new_interface() { cout << endl << "Novi string: "; }
void new_interface_success() { cout << "String je uspesno umetnut." << endl; }
void new_interface_failure() { cout << "String se vec nalazi u bazi." << endl; }

void find_interface() { cout << endl << "String: "; }
void find_interface_success(int index) { cout << "String je uspesno pronadjen na lokaciji " << index << '.' << endl; }
void find_interface_failure() { cout << "String nije uspesno pronadjen."; }

void del_interface() { cout << endl << "String: "; }
void del_interface_success() { cout << "String je uspesno obrisan." << endl; }
void del_interface_failure() { cout << "String se ne nalazi u bazi." << endl; }

void write_interface() { cout << "Baza:" << endl; }

void write_btree_interface() { cout << "Stablo:" << endl; }

void quit_interface() { cout << "Kraj."; }
#endif