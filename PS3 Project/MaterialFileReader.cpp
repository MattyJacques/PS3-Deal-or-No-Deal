#include "MaterialFileReader.h"
#include <cctype>

MaterialFileReader::MaterialFileReader(char* _filename, Material *theMaterial)
{ // Sets pointer to the material to load into then called open file
  buffer = 0;

  pTheMaterial = theMaterial; // Pointer to material object

	OpenFile(_filename); // All is well, continue
} // Constructor()


MaterialFileReader::~MaterialFileReader()
{ // clears the buffer for deletion
  buffer = 0;
} // Destructor()


void MaterialFileReader::OpenFile(char* _filename)
{ // Opens the material file and loads in the buffer
  err = cellFsOpen(_filename, CELL_FS_O_RDONLY, &fd, NULL, 0);

	if (err != CELL_FS_SUCCEEDED) 
    PrintError(err);
	
  err = cellFsFstat(fd, &sb);
	
  if (!err == CELL_FS_SUCCEEDED) 
    PrintError(err);
	
  buffer = new char[sb.st_size+1000];
	err = cellFsRead(fd, (void *)buffer, sb.st_size, &nrw);
	
  if (!err == CELL_FS_SUCCEEDED) 
    PrintError(err);
	
  buffer[sb.st_size+1]='\0';  //sticks a eof at the end
	cellFsClose(fd);

  ParseData();
} // OpenFile()


void MaterialFileReader::PrintError(CellFsErrno err)
{ // If error has occured print so we know what happened
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


void MaterialFileReader::SkipWhiteSpace()
{// Checks if cursor is currently space, if so increment until nonspace

  // If space, increment and try again
  if (std::isspace(*cursor) && IsNotEOF()) 
  {
    cursor++;
    SkipWhiteSpace();
  }
  else if(cursor[0] == '#') // Comment found, ignoring
  {
    while(cursor[0] != '\r')
      cursor++;
    SkipWhiteSpace();
  }
} // SkipWhiteSpace()


void MaterialFileReader::SkipToNewLine()
{ // Skips to the next new line
  while (( cursor[0] != '\n') && IsNotEOF())
	  cursor++;
  cursor++;
} // SkipToNewLine()


bool MaterialFileReader::IsNotEOF()
{ // checks to see if the file has not reached the end
  return cursor < (buffer + sb.st_size);
} // IsNotEOF()


void MaterialFileReader::GetNextFloat(float *num)
{// Reads in float to *num, else put error value in (-9999)

  bool isNegative = false;
  float divisor = 1.0;

  SkipWhiteSpace(); // Skip initial whietspace

  if (*cursor == '-') // Checks to see if negative
  {
    isNegative = true;
    cursor++;
  }

  if (!std::isdigit(*cursor) && *cursor != '.') // checks for format
  {
    fprintf(stderr, "Format error\n");
    *num = -9999;
  }

  *num = 0;

  while (std::isdigit(*cursor) && IsNotEOF()) // Handles digits before decimal
  {
    *num *= 10;
    *num += (*cursor - '0');
    cursor++;
  }

  if (*cursor == '.') // Handles decimal point
  {
    cursor++;

    while (std::isdigit(*cursor) && IsNotEOF()) // adds number after decimal
    {
      divisor /= 10;
      *num += (*cursor - '0')*divisor;
      cursor++;
    } // while (isdigit(*cursor) && isNotEOF())
  } // if (*cursor == '.')

  if (isNegative) // converts to negative if need be
    *num = - *num;

} // GetNextFloat()


void MaterialFileReader::GetNextWord(char *word)
{// Skips whitespace, then copies char until next whitespace 

  int index = 0;

  SkipWhiteSpace(); // Skip initial white space

  // If not whitespace, add to word
  while (!std::isspace(*cursor) && IsNotEOF())
  {
    word[index] = *cursor;
    cursor++;
    index++;
  }

  word[index] = '\0';

} // GetNextWord()


void MaterialFileReader::GetNextInt(int *num)
{ // Skips whitespace, if looking at digit, parse int else put error code
  // in *num. Checks if int is negative

  bool isNegative = false;

  SkipWhiteSpace(); // Skip initial whitespace

  if (*cursor == '-')
  { // Checks to see if int is negative
    isNegative = true;
    cursor++;
  }

  if (!std::isdigit(*cursor)) 
  { // Makes sure we currently have a digit, else format error
    fprintf(stderr, "Format error\n");
    *num = -9999;
  }

  *num = 0;

  while (std::isdigit(*cursor) && IsNotEOF())
  { // Copies the ints over
    *num *= 10;
    *num += (*cursor - '0');
    cursor++;
  }

  if (isNegative) // If supposed to be negative, convert
    *num = - *num;

} // GetNextInt()


void MaterialFileReader::ParseData()
{ // Handles the whole parsing of the data calling the correct cases
  // for each line type
  float f0, f1, f2, f3;
  int i;

  cursor = buffer;

  while(IsNotEOF())
  {
    switch(*cursor)
    {
    case 'D':
      //fprintf(stderr, "Parsing D ");
      cursor++;

      GetNextFloat(&f0);
      pTheMaterial->diffuse[0] = f0;
      GetNextFloat(&f1);
      pTheMaterial->diffuse[1] = f1;
      GetNextFloat(&f2);
      pTheMaterial->diffuse[2] = f2;
      GetNextFloat(&f3);
      pTheMaterial->diffuse[3] = f3;

      //fprintf(stderr, ": %f, %f, %f, %f\n", f0, f1, f2);
      SkipToNewLine();
      break;

    case 'A':
      //fprintf(stderr, "Parsing A ");
      cursor++;

      GetNextFloat(&f0);
      pTheMaterial->ambient[0] = f0;
      GetNextFloat(&f1);
      pTheMaterial->ambient[1] = f1;
      GetNextFloat(&f2);
      pTheMaterial->ambient[2] = f2;
      GetNextFloat(&f3);
      pTheMaterial->ambient[3] = f3;

      //fprintf(stderr, ": %f, %f, %f, %f\n", f0, f1, f2);
      SkipToNewLine();
      break;

    case 'E':
      //fprintf(stderr, "Parsing E ");
      cursor++;

      GetNextFloat(&f0);
      pTheMaterial->emission[0] = f0;
      GetNextFloat(&f1);
      pTheMaterial->emission[1] = f1;
      GetNextFloat(&f2);
      pTheMaterial->emission[2] = f2;
      GetNextFloat(&f3);
      pTheMaterial->emission[3] = f3;

      //fprintf(stderr, ": %f, %f, %f, %f\n", f0, f1, f2);
      SkipToNewLine();
      break;

    case 'S':
      //fprintf(stderr, "Parsing S ");
      cursor++;

      GetNextFloat(&f0);
      pTheMaterial->specular[0] = f0;
      GetNextFloat(&f1);
      pTheMaterial->specular[1] = f1;
      GetNextFloat(&f2);
      pTheMaterial->specular[2] = f2;
      GetNextFloat(&f3);
      pTheMaterial->specular[3] = f3;

      //fprintf(stderr, ": %f, %f, %f, %f\n", f0, f1, f2);
      SkipToNewLine();
      break;

    case 'H':
      //fprintf(stderr, "Parsing H ");
      cursor++;

      GetNextInt(&i);
      pTheMaterial->shininess = i;

      //fprintf(stderr, ": %i\n", i);
      SkipToNewLine();
      break;

    case 'T':
      //fprintf(stderr, "Parsing T ");
      cursor++;

      GetNextInt(&i);
      pTheMaterial->EnableTexture(i);

      SkipToNewLine();
      break;
    default:
      SkipToNewLine();
      break;
    } // switch(*cursor)
  } // while(IsNotEOF())
} // ParseData()