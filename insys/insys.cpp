#include "stdafx.h"
#include "Radio.h"

int main()
{
	Radio radio;
	short diveces_count, g_lid;
	if (!radio.init()) {
		system("pause");
		return radio.err;
	}
	diveces_count	=	radio.displayDevices();
	if (!diveces_count) {
		system("pause");
		return 1;
	}
	while (1) {
		cout << "Enter LID of necessary board: ";
		cin >> g_lid;
		if (radio.openDevice(g_lid)) 
		{
			break;
		}
		cout << "Try again" << endl << endl;
	}
	radio.closeDevice();
	system("pause");
    return 0;
}
