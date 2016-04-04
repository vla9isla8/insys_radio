#include	"stdafx.h"
#include	"Radio.h"

d_list Radio::getdeviceList()
{
	U32			lidArray[16];
	U32			itemReal;
	BRD_Info	info = { sizeof(info) };
	U32			ii;
	d_list		devicesList;

	BRD_lidList(lidArray, 16, &itemReal);
	if (itemReal != 0)
	{
		for (ii = 0; ii < itemReal; ii++)
		{
			BRD_getInfo(lidArray[ii], &info);
			devicesList[lidArray[ii]] = info;
		}
		return devicesList;
	} else {
		cout <<CURRFILE" ERROR: None Board" << endl << endl;
		return devicesList;
	}
}
Radio::Radio()
{
	BRD_displayMode(BRDdm_VISIBLE);
}
Radio::~Radio()
{

}
boolean Radio::init()
{
	err = BRD_initEx(BRDinit_FILE_KNOWN, "brd.ini", "log.ini", &num);
	if (0>err)
	{
		if (BRD_errcmp(err, BRDerr_NONE_DEVICE))
			cout << CURRFILE" ERROR: None Board" << endl << endl;
		else
			cout << CURRFILE" ERROR: Can't Initialize BRD Shell" << endl << endl;
		return false;
	}
	return true;
}
boolean Radio::openDevice(U32 g_lid)
{
	if (deviceList.size() == 0) {
		deviceList = this->getdeviceList();
	}
	if (deviceList.size() == 0) {
		cout << CURRFILE" ERROR: Devices not found" << endl << endl;
		return false;
	}
	if (deviceList.count(g_lid) == 0) {
		cout << CURRFILE" ERROR: There's not board with LID: " << g_lid << endl;
		return false;
	}
	handle = BRD_open(g_lid, BRDopen_EXCLUSIVE, &openMode);
	if (handle <= 0)
	{
		printf(CURRFILE" ERROR: Can't Open device (LID=%d)\n\n", g_lid);
		return false;
	}
	printf(CURRFILE" INFO: Open device (LID=%d), Open Mode = %s(%d)\n\n", g_lid,
		(openMode == BRDopen_EXCLUSIVE) ? "EXCLUSIVE" :
		(openMode == BRDopen_SHARED) ? "SHARED" :
		(openMode == BRDopen_SPY) ? "SPY" : "UNKNOWN",
		openMode
		);
	return true;
}
void Radio::closeDevice()
{
	if (handle < 0) {
		cout << CURRFILE"ERROR: There is not opened device" << endl;
	}
	err =	BRD_close(handle);
	if (err < 0) {
		if (BRD_errcmp(err, BRDerr_BAD_HANDLE))
			cout << CURRFILE" ERROR: Incorrect descriptor" << endl;
		else
			cout << CURRFILE" ERROR: Trying to close not opened device" << endl;
	}
	cout << CURRFILE" INFO: Device closed successfuly" << endl;
}
BRD_PuList * Radio::getPuList()
{
	BRD_PuList		*paPuList;
	U32				item;
	int				isPuOK = 0;

	//
	// Get PU LIST
	//
	err = BRD_puList(handle, NULL, 0, &item);
	paPuList = (BRD_PuList*)malloc(item * sizeof(BRD_PuList));
	err = BRD_puList(handle, paPuList, item, &item);
	if (err<0)
	{
		cout << CURRFILE" ERROR: Device doesn't have Programmable Units" << endl << endl;
		free(paPuList);
		return nullptr;
	}
	return paPuList;
}
int Radio::displayDevices()
{
	if (deviceList.size() ==	0) {
		deviceList = this->getdeviceList();
	}
	if (deviceList.size() == 0) {
		cout << CURRFILE" ERROR: Devices not found" << endl << endl;
		return deviceList.size();
	}
	cout << endl << endl << "--------List available devices---------" << endl << endl;
	for (map <U32, BRD_Info>::iterator it = deviceList.begin(); it != deviceList.end(); it++) {
		printf("LID=%2d, BOARD=%s, PID=%d(0x%X), BUS=%d, SLOT=%d\n", it->first, it->second.name, it->second.pid, it->second.pid, it->second.bus, it->second.slot);
	}
	cout << endl << endl;
	return deviceList.size();
}
boolean Radio::loadDSPProgramm(char * fileName)
{
	BRD_load(handle, g_nodeId, fileName, 0, NULL);
	BRD_start(handle, g_nodeId);
	return boolean();
}
