#ifndef MATERIALFILEREADER_H
#define MATERIALFILEREADER_H

#include <stdio.h>
#include <string.h>
#include <sys/paths.h>
#include <cell/cell_fs.h>
#include <cell/sysmodule.h>
#include "Material.h"

class MaterialFileReader
{
public:
  int fd;  //file identifier
  CellFsErrno err; //error code
  char *buffer; //buffer - this is where the data goes
  char *cursor; //pointer into buffer for scanning 
  CellFsStat sb; //stats about the file
  uint64_t nrw; //just a utitilty number for a piece of info I'm not interested in
  Material *pTheMaterial;

  MaterialFileReader(char* _filename, Material *theMaterial);
  ~MaterialFileReader();
  void OpenFile(char* _filename);
  void PrintError(CellFsErrno err);
  void SkipWhiteSpace();
  void SkipToNewLine();
  bool IsNotEOF();
  void GetNextFloat(float *num);
  void GetNextWord(char *word);
  void GetNextInt(int *num);
  void ParseData();
};

#endif