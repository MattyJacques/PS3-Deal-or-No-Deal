#ifndef MODELFILEREADER_H
#define MODELFILEREADER_H

#include <stdio.h>
#include <string.h>
#include <sys/paths.h>
#include <cell/cell_fs.h>
#include <cell/sysmodule.h>
#include <vector>

class Model;

class ModelFileReader
{
public:
  int fd;  //file identifier
  CellFsErrno err; //error code
  char *buffer; //buffer - this is where the data goes
  char *cursor; //pointer into buffer for scanning 
  CellFsStat sb; //stats about the file
  uint64_t nrw; //just a utitilty number for a piece of info I'm not interested in
  std::vector<float> positionsVec; // A vector to hold the position data
  std::vector<float> coloursVec;   // A vector to hold the colour data
  std::vector<unsigned short> indicesVec;   // A vector to hold the indices
  std::vector<float> normalsVec;   // A vector to hold the normals
  std::vector<float> textureVec;   // A vector to hold the texture data
  Model *pTheModel;               // A pointer to the model object we are loading in

  ModelFileReader(char* _filename, Model *theModel);
  ~ModelFileReader();
  void OpenFile(char* _filename);
  void PrintError(CellFsErrno err);
  void SkipWhiteSpace();
  void SkipToNewLine();
  bool IsNotEOF();
  void GetNextWord(char *word);
  void GetNextInt(int *num);
  void GetNextFloat(float *num);
  int CheckLineCount(char lineType);
  void ParseData();
  void GenFlatNormals();
  void GenSplayNormals();
  void SetData();
};

#endif
