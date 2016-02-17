#ifndef MAPREADER_H
#define MAPREADER

#include <cell/cell_fs.h>
#include <cell/sysmodule.h>
#include <sys/paths.h>
#include "Map.h"

class MapReader
{
public:
  int fd;  //file identifier
  CellFsErrno err; //error code
  char *buffer; //buffer - this is where the data goes
  char *cursor; //pointer into buffer for scanning 
  CellFsStat sb; //stats about the file
  uint64_t nrw; //just a utitilty number for a piece of info I'm not interested in
  Map *pTheMap;

  MapReader(char* _filename, Map* theMap);
  ~MapReader();
  bool IsNotEOF();
  void OpenFile(char* _filename);
  void PrintError(CellFsErrno err);
  void SetArray();
};

#endif
