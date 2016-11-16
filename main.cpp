#include <iostream>
//#include <mpi.h>
#include <fstream>
#include "JpegLs.h"
#include "BinaryIO.h"
#include <vector>
using namespace std;


bool checkFormat(BinaryInputStream &in);
bool readJLSFrame(BinaryInputStream &in,Options &opt);

//调用该函数文件指针会重置到文件头
void findSofPostion(BinaryInputStream &in, vector<streampos> &pos);
bool writeOutHeader(ofstream &outf,int rows,int cols,int maxval);

int main(int argc, char **argv)
{

	string conf = "conf.txt";

	Options opt(conf);
	opt.showConf();
	cout << endl << "------------------------" << endl << endl;;

	//string infile = opt.inFile;
	//string outfile = opt.ouFile;

	string infile = "data/parall/testt16.jls";
	string outfile = "data/parall/hahaha.dat";

	ifstream ifs(infile.c_str(), ios::binary|ios::in);
	if (!ifs.is_open())
	{
		cout << "Input file: "<< infile <<" open failure" << endl;
		return 1;
	}
	BinaryInputStream in(ifs.rdbuf(),BigEndian);
	

	//testDecoder();
	
	Uint16 str;
	if (!checkFormat(in)){ cout << "unknown format!"<< endl; return 1; }
	if (!readJLSFrame(in, opt)){ cout << "unknown frame header!"<< endl; return 1; }

	vector<streampos> pos;
	findSofPostion(in, pos);
	cout << pos.size() << endl;

	ofstream createOutFile(outfile.c_str(), ios::binary | ios::out);
	createOutFile.close();

	ofstream ofs(outfile.c_str(), ios::binary | ios::out | ios::in);
	if (!ofs.is_open())
	{
		cout << "Output file: " << outfile << " open failure" << endl;
		return 1;
	}
	writeOutHeader(ofs, opt.ROWS*pos.size(), opt.COLOMNS, pow(2, opt.BITS) - 1);
	BinaryOutputStream out(ofs.rdbuf(), BigEndian);



	for (int i = 0; i < pos.size();i++)
	{
		in.seekg(pos[i]);
		decoder(opt, in, out);
		cout << "component " << i + 1 << "decoder complete" << endl;
	}

	ifs.close();
	ofs.close();
	cout << "success!" << endl;


	return 0;
}

//int main(int argc, char** argv)
//{
//	int rank;
//	int nodes;
//
//	MPI_Init(&argc, &argv);
//	MPI_Comm_size(MPI_COMM_WORLD, &nodes);
//	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//
//	//输入参数
//	string conf = "conf.txt";
//	Options opt(conf);
//	if (rank == 0)
//	{
//		opt.showConf();
//		cout << endl << "------------------------" << endl << endl;
//	}
//	
//	
//	//获取
//	string infile = opt.inFile;
//	string outfile = opt.ouFile;
//
//	ifstream ifs(infile, ios::binary | ios::in);
//	if (!ifs.is_open())
//	{
//		cout << "Input file: " << infile << " open failure" << endl;
//		return 1;
//	}
//	BinaryInputStream in(ifs.rdbuf(), opt.ENDIAN);
//
//
//	//testDecoder();
//
//	Uint16 str;
//	if (!checkFormat(in)){ cout << "unknown format!" << endl; return 1; }
//	if (!readJLSFrame(in, opt)){ cout << "unknown frame header!" << endl; return 1; }
//
//	vector<streampos> pos;
//	findSofPostion(in, pos);
//	cout << pos.size() << endl;
//
//	ofstream createOutFile(outfile, ios::binary | ios::out);
//	createOutFile.close();
//
//	ofstream ofs(outfile, ios::binary | ios::out | ios::in);
//	if (!ofs.is_open())
//	{
//		cout << "Output file: " << outfile << " open failure" << endl;
//		return 1;
//	}
//	writeOutHeader(ofs, opt.ROWS*pos.size(), opt.COLOMNS, pow(2, opt.BITS) - 1);
//	BinaryOutputStream out(ofs.rdbuf(), BigEndian);
//
//
//
//	for (int i = 0; i < pos.size(); i++)
//	{
//		in.seekg(pos[i]);
//		decoder(opt, in, out);
//		cout << "component " << i + 1 << "decoder complete" << endl;
//	}
//
//	ifs.close();
//	ofs.close();
//	cout << "success!" << endl;
//
//
//	cout << "Hello:" << nodes << "," << rank << endl;
//
//	MPI_Finalize();
//	return 0;
//}

int main3(int argc, char **argv)
{
	string conf = "conf.txt";
	Options opt(conf);
	opt.showConf();
	cout << "-----------------------------------" << endl;
	//encoder(opt);

	//decoder(opt);

	return 0;
}

int main4()
{
	string file = "test.dat";
	int row = 200;
	int col = 400;
	int maxval = 1023;
	//writeOutHeader(file, row, col, maxval);
	cout << "success" << endl;
	return 0;
}


bool checkFormat(BinaryInputStream &in)
{
	Uint16 soi = in.read16();
	if (soi != 0xFFD8)
	{
		cout << "Unsupported format!" << endl;
		return false;
	}
	return true;
}

bool readJLSFrame(BinaryInputStream &in, Options &opt)
{	
	bool v = opt.VERBOSE;

	Uint16 sof55 = in.read16();

	if (sof55 != 0xFFF7)
	{
		cout << "Unknown SOF55" << endl;
		return false;
	}

	int length = in.read16();

	if(v) cout <<"length:" << length << endl;
	if (length != int(11))
	{
		cout << "unknown frame length" << endl;
		return false;
	}

	int bits = in.read8();
	if(v) cout <<"bits:" <<bits << endl;
	if (bits > 16)
	{
		cout << "bits max value is 16" << endl;
		return false;
	}
	opt.BITS = bits;


	int rows = in.read16();
	int cols = in.read16();
	if(v) cout << "rows:" << rows << endl;
	if(v) cout << "cols:" << cols << endl;

	opt.ROWS = rows;
	opt.COLOMNS = cols;

	int compSize = in.read8();
	int comId = in.read8();
	if (v) cout << "number of components in frame is :" << compSize << endl;
	if (v) cout << "Component's ID:" << comId << endl;

	in.read16();

	return true;
}

void findSofPostion(BinaryInputStream &in, vector<streampos> &pos)
{
	in.seekg(0, ios::beg);
	Uint8 tempstr;
	streampos temppos;
	while (!in.eof())
	{
		tempstr = in.read8();
		if (tempstr == 0xFF)
		{
			tempstr = in.read8();
			if (tempstr == 0xDA)
			{
				temppos = in.tellg();
				pos.push_back(temppos - streampos(2));
			}
		}
	}

	in.clear();
	in.seekg(0, ios::beg);
}

bool writeOutHeader(ofstream &outf, int rows, int cols, int maxval)
{
	if (!outf.is_open())
	{
		cout << "输出文件打开失败" << endl;
		return false;
	}
	outf << "P5" << endl;
	outf << cols << " " << rows << endl;
	outf << maxval << endl;
	return true;
}

