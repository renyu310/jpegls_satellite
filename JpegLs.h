#ifndef JPEGLS_HEADER
#define JPEGLS_HEADER

#include "CommomFunction.h"
#include "BinaryIO.h"
#include <cassert>
#include <stdio.h>
#include <stdlib.h>

#define CLAMP_1(i)	(((i) > MAXVAL || (i) < NEAR+1) ? NEAR+1 : (i))
#define CLAMP_2(i)	(((i) > MAXVAL || (i) < T1) ? T1 : (i))
#define CLAMP_3(i)	(((i) > MAXVAL || (i) < T2) ? T2 : (i))

class Options;

bool encoder(Options opt);
//bool decoder(Options opt);
bool decoder(Options opt, BinaryInputStream &in, BinaryOutputStream &out);
void createTestFile();
void testDecoder();

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
	void setEndian(Endian endian){  ENDIAN = endian; }
	void setNoMarker(bool marker){ NOMARKER = marker; }
	void setNoRuns(bool runs){ NORUNS = runs; }
	void setVerbose(bool v){ VERBOSE = v; }

	void setInputFile(const string ifile){  inFile = ifile; }
	void setOutputFile(const string ofile){ ouFile = ofile; }

	void showConf();

public:
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
