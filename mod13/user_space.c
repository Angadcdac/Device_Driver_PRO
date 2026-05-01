#include <stdio.h>
#include <stdlib.h>


int main(){

	system("sudo insmod i2c-stub.ko chip_addr=0x25");
	/* Writing in memory: */
	system("sudo i2cset -y -a 3 0x25 0x00 0x48");
	system("sudo i2cset -y -a 3 0x25 0x01 0x65");
	system("sudo i2cset -y -a 3 0x25 0x02 0x6C");
	system("sudo i2cset -y -a 3 0x25 0x03 0x6C");
	system("sudo i2cset -y -a 3 0x25 0x04 0x6F");
	system("sudo i2cset -y -a 3 0x25 0x05 0x57");
	system("sudo i2cset -y -a 3 0x25 0x06 0x6F");
	system("sudo i2cset -y -a 3 0x25 0x07 0x72");
	system("sudo i2cset -y -a 3 0x25 0x08 0x6C");
	system("sudo i2cset -y -a 3 0x25 0x09 0x64");
	/* Reading from memory: */
	system("sudo i2cdump -y -r 0x0-0x10 -a 3 0x25");
	system("sudo insmod mod13.ko");
	system("i2cdetect -y 3");
	system("sudo rmmod mod13.ko");
	system("sudo rmmod i2c-stub.ko");
	return 0;
}
