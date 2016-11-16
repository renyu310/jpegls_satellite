#include <iostream>
#include <mpi.h>
#include <fstream>
#include "JpegLs.h"
#include "BinaryIO.h"
#include <vector>
using namespace std;


bool checkFormat(BinaryInputStream &in);
bool readJLSFrame(BinaryInputStream &in,Options &opt);

//调用该函数文件指针会重置到文件头
void findSosPostion(BinaryInputStream &in, vector<streampos> &pos);
bool writeOutHeader(ofstream &outf,int rows,int cols,int maxval);

int main1(int argc, char **argv)
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
	findSosPostion(in, pos);
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

int main(int argc, char** argv)
{
	int rank;
	int nodes;
	int namelen;
	char processorName[MPI_MAX_PROCESSOR_NAME];

	double t1, t2, t3, t4, t5, t6, t7;


	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nodes);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Get_processor_name(processorName, &namelen);

	if (rank == 0)
	{
		t1 = MPI_Wtime();
		cout << "Program start!" << endl;
	}
	

	//输入参数
	string conf = "conf.txt";
	Options opt(conf);
	if (rank == 0)
	{
		opt.showConf();
		cout << endl << "------------------------" << endl << endl;
	}
	
	
	//获取
	//string infile = opt.inFile;
	//string outfile = opt.ouFile;

	opt.ENDIAN = BigEndian;
	opt.ROWS = 256;
	opt.COLOMNS = 256;
	opt.BASIC_T1 = 3;
	opt.BASIC_T2 = 7;
	opt.BASIC_T3 = 21;
	opt.BITS = 10;

	string infile = "data/parall/testt16.jls";
	string outfile = "data/parall/mpitest.dat";

	if (rank == 0)
	{
		t2 = MPI_Wtime();
		cout << "read input config:"<<t2 - t1 << endl;
	}


	//打开输入文件
	ifstream ifs(infile.c_str(), ios::binary | ios::in);
	if (!ifs.is_open())
	{
		cout << "Input file: " << infile << " open failure" << endl;
		return 1;
	}
	BinaryInputStream in(ifs.rdbuf(), opt.ENDIAN);

	//检查文件格式和头参数
	Uint16 str;
	if (!checkFormat(in)){ cout << "unknown format!" << endl; return 1; }
	if (!readJLSFrame(in, opt)){ cout << "unknown frame header!" << endl; return 1; }
	
	if (rank == 0)
	{
		t3 = MPI_Wtime();
		cout << "input data header check and read:" << t3 - t2 << endl;
	}

	//找到每个独立块位置
	vector<streampos> pos;
	findSosPostion(in, pos);
	cout << pos.size() << endl;

	if (rank == 0)
	{
		t4 = MPI_Wtime();
		cout << "find add sos position:" << t4 - t3 << endl;
	}

	if (rank == 0)
	{
		//0节点创建输出文件
		ofstream createOutFile(outfile.c_str(), ios::binary | ios::out);
		createOutFile.close();
	}

	//等待0节点创建完输出文件
	MPI_Barrier(MPI_COMM_WORLD);

	//打开输出文件
	ofstream ofs(outfile.c_str(), ios::binary | ios::out | ios::in);
	if (!ofs.is_open())
	{
		cout << "Output file: " << outfile << " open failure" << endl;
		return 1;
	}

	writeOutHeader(ofs, opt.ROWS*pos.size(), opt.COLOMNS, pow(2, opt.BITS) - 1);
	BinaryOutputStream out(ofs.rdbuf(), BigEndian);

	streampos outStartPos = out.tellp();
	//使用0来初始化

	if (rank == 0)
	{
		t5 = MPI_Wtime();
		cout << "initialize output file:" << t5 - t4 << endl;
	}

	int rows = opt.ROWS;
	int cols = opt.COLOMNS;
	int p = ceil(opt.BITS / 8.0);
	cout << "rows:"<<rows <<"|cols:"<< cols <<"|p:" <<p << endl;

	//为每个节点分配输出文件位置
	int allBlockSize = pos.size();

	MPI_Barrier(MPI_COMM_WORLD);
	if (rank == 0)
	{
		cout << "start decoder" << endl;
	}
	for (int i = 0; i < allBlockSize; i++)
	{
		if (i%nodes == rank)
		{
			in.seekg(pos[i]);
			out.seekp(outStartPos+streampos(i*p*rows*cols));

			decoder(opt, in, out);
			cout << "Node:"<<processorName<<" | Rank:"<<rank<<" | component " << i + 1 << "  decoder complete" << endl;
		}

	}

	if (rank == 0)
	{
		t6 = MPI_Wtime();
		cout << "decoder time:" << t6 - t5 << endl;
	}
	ifs.close();
	ofs.close();

	MPI_Barrier(MPI_COMM_WORLD);
	if (rank == 0)
	{
		t7 = MPI_Wtime();
		cout << "decoder total time:" << t7 - t1 << endl;
	}

	MPI_Finalize();
	return 0;
}

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

	string outfile = "hello.dat";

	ofstream createOutFile(outfile.c_str(), ios::binary | ios::out);
	createOutFile.close();


	//打开输出文件
	ofstream ofs(outfile.c_str(), ios::binary | ios::out | ios::in);
	if (!ofs.is_open())
	{
		cout << "Output file: " << outfile << " open failure" << endl;
		return 1;
	}

	writeOutHeader(ofs, 512, 512, pow(2, 10) - 1);
	BinaryOutputStream out(ofs.rdbuf(), BigEndian);

	streampos outStartPos = out.tellp();
	//使用0来初始化

	out.seekp(512, ios::cur);
	out.write8('A');

	out.seekp(512, ios::cur);
	out.write8('B');
	out.write8('C');

	out.seekp(outStartPos);
	out.write8('Z');


	ofs.close();
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

void findSosPostion(BinaryInputStream &in, vector<streampos> &pos)
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

