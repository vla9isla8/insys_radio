#include "stdafx.h"

int main()
{
	Radio radio;
	short diveces_count, g_lid;
	//Inintialing radio
	if (!radio.init()) {
		system("pause");
		return radio.err;
	}
	//Searching available boards
	diveces_count	=	radio.displayDevices();
	if (!diveces_count) {
		system("pause");
		return 1;
	}
	if (diveces_count != 1 || !radio.openDevice()){
		while (1) {
			cout	<<	"Enter LID of necessary board: ";
			cin		>>	g_lid;
			if (g_lid <= 0)
			{
				cout << "Enter valid LID! Take it from list." << endl << endl;
				continue;
			}
			if (radio.openDevice(g_lid)) 
			{
				break;
			}
			cout << "Try again" << endl << endl;
		}
	}
	radio.displayPuList();
	int pld_state;
	if (!radio.getPLDState(&pld_state)) {
		cout << "Programm error" << endl;
		system("pause");
		return 1;
	}
	if (pld_state) {
		cout << CURRFILE " INFO: PLD was pragrammed" << endl << endl;
	}
	else {
		char q[10];
		while (1) 
		{
			cout << CURRFILE " INFO: PLD was not pragrammed" << endl << endl;
			cout << CURRFILE " INFO: Want do it now?(y/n)!" << endl;
			cin >> q;
			if (strcmp(q, "y")==0 || strcmp(q, "n")==0) {
				break;
			}
		}
		if (strcmp(q, "y")==0) {
			if (!radio.loadPLD(&pld_state)) {
				system("pause");
				return radio.err;
			}
			if (pld_state) {
				cout << CURRFILE " INFO: PLD was programed successfully!" << endl << endl;
			}else{
				cout << CURRFILE " ERROR: PLD stayed unprogrammed!" << endl << endl;
				system("pause");
				return radio.err;
			}
		}
		else {
			cout << CURRFILE " INFO: Program cannot work without programmed PLD!" << endl;
			system("pause");
			return 0;
		}
	}
	if (radio.loadDSPProgram()) {
		radio.startDSPProgram();
		system("pause");
		radio.stopDSPProgram();
	}	
	radio.closeDevice();
	system("pause");
    return 0;
}
