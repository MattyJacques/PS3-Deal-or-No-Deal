#include "MapReader.h"
#include <cctype>

MapReader::MapReader(char* _filename, Map* theMap)
{ // Sets the members to their default values
  buffer = 0;

  pTheMap = theMap;

	OpenFile(_filename); // All is well, continue
} // Constructor()


MapReader::~MapReader()
{ // clears the buffer for deletion
  buffer = 0;
} // Destructor()


void MapReader::OpenFile(char* _filename)
{ // Opens the file given as a parameter
  err = cellFsOpen(_filename, CELL_FS_O_RDONLY, &fd, NULL, 0);

	if (!err == CELL_FS_SUCCEEDED) 
    PrintError(err);
	
  err = cellFsFstat(fd, &sb);
	
  if (!err == CELL_FS_SUCCEEDED) 
    PrintError(err);
	
  buffer = new char[sb.st_size+1];
	err = cellFsRead(fd, (void *)buffer, sb.st_size, &nrw);
	
  if (!err == CELL_FS_SUCCEEDED) 
    PrintError(err);
	
  buffer[sb.st_size+1]='\0';  //sticks a eof at the end
	cellFsClose(fd);

  SetArray();
} // OpenFile()


void MapReader::PrintError(CellFsErrno err)
{ // If error found, print error to output so we know what happened
	printf("File i/o error:\n");
	switch (err)
	{
	case CELL_FS_ENOTMOUNTED:
		printf("File system corresponding to path is not mounted\n");
		break;
	case CELL_FS_ENOENT: 
		printf("The file specified by path does not exist or The specified file does not exist and CELL_FS_O_CREAT is not specified in flags\n");
		break;
	case CELL_FS_EINVAL: 
		printf("The specified path is invalid or Extended argument is invalid\n");
		break;
	case CELL_FS_EMFILE: 
		printf("The number of file descriptors that can be opened at the same time has reached the maximum\n");
		break;
	case CELL_FS_EISDIR: 
		printf("The specified path is a directory\n");
		break;
	case CELL_FS_EIO: 
		printf("I/O error has occurred\n");
		break;
	case CELL_FS_ENOMEM: 
		printf("Memory is insufficient\n");
		break;
	case CELL_FS_ENOTDIR: 
		printf("Components in path contain something other than a directory\n");
		break;
	case CELL_FS_ENAMETOOLONG: 
		printf("path or components in path exceed the maximum length\n");
		break;
	case CELL_FS_EFSSPECIFIC: 
		printf("File system specific internal error has occurred\n");
		break;
	case CELL_FS_EFAULT: 
		printf("When size is not 0, a NULL pointer is specified in arg path or fd is NULL\n");
		break;
	case CELL_FS_EPERM: 
		printf("Permission is invalid\n");
		break;
	case CELL_FS_EACCES: 
		printf("Permission is invalid (Only CFS, FAT and HOSTFS)\n");
		break;
	case CELL_FS_EEXIST: 
		printf("When the specified file exists, CELL_FS_O_CREAT and CELL_FS_O_EXCL are specified\n");
		break;
	case CELL_FS_ENOSPC: 
		printf("When CELL_FS_O_CREAT is specified in flags and the file does not exist, there is no area to create a new file\n");
		break;
	case CELL_FS_EROFS: 
		printf("When CELL_FS_O_CREAT is specified in flags and the file does not exist, the directory in which it is to be created does not permit writing (Only CFS (SYS_DEV_HDD1) and FAT)\n");
		break;
	case CELL_FS_ENOTMSELF: 
		printf("A file other than the MSELF file is specified in path, and either CELL_FS_O_MSELF or CELL_FS_O_RDONLY|CELL_FS_O_MSELF is specified in flags\n");
		break;
	case CELL_FS_EBUSY:
		printf("File is locked by stream support API\n");
		break;
	case CELL_FS_EBADF:
		printf("fd is invalid or the file system on a removable media has been unmounted\n");
		break;
	} // switch
} // PrintError()

void MapReader::SetArray()
{ // Takes in the buffer from the file and places it in a 2D array to read as the map
  int j=0;

  for (int i = 0; i < 4096; i++)
  {
    if(std::isspace(buffer[j])) // If new line found, increment past it
    {
      j++;
      j++;
    }
    pTheMap->mapArray[int(i/64)][i%64] = buffer[j]; // otherwise place in array
    j++;
  }
} // SetArray()
