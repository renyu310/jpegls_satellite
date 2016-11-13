#ifndef JPEGLS_HEADER
#define JPEGLS_HEADER

#include "CommomFunction.h"
#include "BinaryIO.h"
#include <cassert>
#include <fstream>

class Options;

void test();

bool encoder(Options opt);
bool decoder(Options opt);

class Options
{
public:
	Options(const string optionFile);

	int getBT1(){ return BASIC_T1; }
	int getBT2(){ return BASIC_T2; }
	int getBT3(){ return BASIC_T3; }
	int getNear(){ return NEAR; }
	int getReset(){ return RESET; }
	int getRows(){ return ROWS; }
	int getCols(){ return COLOMNS; }
	int getMaxval(){ return MAXVAL; }
	int getBits(){ return BITS; }
	Endian getEndian(){return ENDIAN;}
	bool haveMarker(){ return !NOMARKER; }
	bool haveRuns(){ return !NORUNS; }
	bool isVerbose(){ return VERBOSE; }

	string getInputFile(){ return inFile; }
	string getOutputFile(){ return ouFile; }


	void setBT1(int t1)   {  BASIC_T1 = t1; }
	void setBT2(int t2)   {  BASIC_T2 = t2; }
	void setBT3(int t3)   {  BASIC_T3 = t3; }
	void setNear(int near)  {  NEAR = near; }
	void setReset(int reset) {  RESET = reset; }
	void setRows(int rows)  {  ROWS = rows; }
	void setCols(int cols)  {  COLOMNS = cols; }
	void setMaxval(int maxval){  MAXVAL = maxval; }
	void setBits(int bits)  {  BITS = bits; }
	Endian setEndian(Endian endian){  ENDIAN = endian; }
	bool setNoMarker(bool marker){ NOMARKER = marker; }
	bool setNoRuns(bool runs){ NORUNS = runs; }
	bool setVerbose(bool v){ VERBOSE = v; }

	string setInputFile(const string ifile){  inFile = ifile; }
	string setOutputFile(const string ofile){ ouFile = ofile; }

	void showConf();

private:
	int BASIC_T1;
	int BASIC_T2;
	int BASIC_T3;
	int NEAR;
	int RESET;
	int ROWS;
	int COLOMNS;
	int MAXVAL;
	int BITS;
	Endian ENDIAN;
	bool NOMARKER;
	bool NORUNS;
	bool VERBOSE;

	string inFile;
	string ouFile;

};


#endif
