#include <iostream>
#include <fstream>
#include "JpegLs.h"
#include "BinaryIO.h"
using namespace std;

int main1(int argc, char **argv)
{

	test();
	
	cout << "success!" << endl;


	return 0;
}


int main(int argc, char **argv)
{
	string conf = "conf.txt";
	Options opt(conf);
	opt.showConf();
	cout << "-----------------------------------" << endl;
	//encoder(opt);

	decoder(opt);

	return 0;
}


