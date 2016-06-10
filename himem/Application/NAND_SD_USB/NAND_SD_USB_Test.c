#include "./system/include/system_head.h"
#include "./driver/include/driver_head.h"
#include "./component/include/component_head.h"

int SystemIntoUDisk();
void Nand_SD_USB_Test(void)
{
	SystemIntoUDisk();			//test usb
}