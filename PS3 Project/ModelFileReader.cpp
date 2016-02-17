#include "ModelFileReader.h"
#include <cctype>
#include "Model.h"


ModelFileReader::ModelFileReader(char* _filename, Model *theModel)
{ // Clears buffer if not empty then opens the file specified

  buffer = 0;

  pTheModel = theModel; // Pointer to model object
  
  pTheModel->indMax = 0; // Set index max to 0;

	OpenFile(_filename); // All is well, continue
} // constructor(filename)


ModelFileReader::~ModelFileReader()
{ // Clears buffer for deletion
  buffer = 0;
} // destructor


void ModelFileReader::SkipWhiteSpace()
{ // Checks if cursor is currently space, if so increment until nonspace

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


void ModelFileReader::SkipToNewLine()
{
  while (( cursor[0] != '\n') && IsNotEOF())
	  cursor++;
} // SkipToNewLine()


bool ModelFileReader::IsNotEOF()
{ // Returns true if not at EOF
  return cursor < (buffer + sb.st_size);
} // isNotEOF()


void ModelFileReader::GetNextWord(char *word)
{ // Skips whitespace, then copies char until next whitespace 

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

} // getNextWord()


void ModelFileReader::GetNextInt(int *num)
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
    fprintf(stderr, "Format error - GetNextInt\n");
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

} // getNextInt()


void ModelFileReader::GetNextFloat(float *num)
{ // Reads in float to *num, else put error value in (-9999)

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
    fprintf(stderr, "Format error - GetNextFloat\n");
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

} // getNextFloat()


int ModelFileReader::CheckLineCount(char lineType)
{ // Counts lines of certain type, to make sure it's all there
  int min = 9999, max = -9999, count = 0, num = 0;

  cursor = buffer; // reset the cursor to start of buffer

  while (IsNotEOF()) // loop until EOF
  {
    SkipWhiteSpace(); // Skip whitespace

    if (*cursor == lineType) // If chars match, increment
    {
      count++;

      while (!std::isspace(*cursor) && IsNotEOF()) // advance past nonspace
        cursor++;

      GetNextInt(&num); // read integer

      if (num < 0) // format error
        return -1;

      if (num > max) // update max, min
        max = num;
      if (num < min)
        min = num;
    } // if (*cursor == lineType)

    SkipToNewLine(); // Advance new line
  } // while (isNotEOF())

  if (count > 0 && (min != 0 || max != count - 1))
  {
    return -1;
  }

	return count;

} // checkLineCount()


void ModelFileReader::OpenFile(char* _filename)
{ // Open and read in the file into the buffer array
  err = cellFsOpen(_filename, CELL_FS_O_RDONLY, &fd, NULL, 0);

	if (!err == CELL_FS_SUCCEEDED) 
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
} // openFile()


void ModelFileReader::PrintError(CellFsErrno err)
{ // If error is found, print the error so we know what happened
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
} // printError()


void ModelFileReader::ParseData()
{ // Parses all the data after the file has been read

  char word[40];
  float f0, f1, f2, f3;
  int i, i0, i1, i2;

  pTheModel->drawMode = pTheModel->NONE;
  pTheModel->faceMode = 0;
  cursor = buffer;

  while (IsNotEOF())
  {
    SkipWhiteSpace();

    switch(*cursor)
    {
      case 'X': // X found, parsing draw mode
      {
        //fprintf(stderr, "Parsing X ");
        cursor++;
        GetNextWord(word);

        if (strcmp(word, "GL_TRIANGLES") == 0)
        {
          pTheModel->faceMode = GL_TRIANGLES;
          //fprintf(stderr, "GL_TRIANGLES\n");
        }
		    else if (strcmp(word, "PT_INDEXED") == 0)
        {
          pTheModel->drawMode = pTheModel->INDEXED;
          //fprintf(stderr, "INDEXED\n");
        }
		    else if (strcmp(word, "PT_DIRECT") == 0)
        {
          pTheModel->drawMode = pTheModel->DIRECT;
          //fprintf(stderr, "DIRECT\n");
        }
        else if (strcmp(word, "PT_GEN_NORMALS") == 0)
        {
          pTheModel->normalMode = pTheModel->GEN_NORMALS;
          //fprintf(stderr, "GEN_NORMALS\n");
        }
        else if (strcmp(word, "PT_GEN_SPLAYED") == 0)
        {
          pTheModel->normalMode = pTheModel->GEN_SPLAYED;
          //fprintf(stderr, "GEN_SPLAYED\n");
        }
        else if (strcmp(word, "PT_ONE_COLOUR") == 0)
        {
          //fprintf(stderr, "ONE_COLOUR - IGNORING\n");
        }
        else if (strcmp(word, "PT_TEXTURE_COORDINATES") == 0)
        {
          pTheModel->hasTexture = true;
          //fprintf(stderr, "TEXTURE_COORDINATES\n");
        }
        else if (strcmp(word, "PT_LOAD_NORMALS") == 0)
        {
          pTheModel->normalMode = pTheModel->GEN_NONE;
          //fprintf(stderr, "LOAD_NORMALS\n");
        }
        SkipToNewLine();
        break;
      }

      case 'P': // P found, parse positions
        //fprintf(stderr, "Parsing P ");
        cursor++;

        GetNextInt(&i);
        GetNextFloat(&f0);
        positionsVec.push_back(f0);
        GetNextFloat(&f1);
        positionsVec.push_back(f1);
        GetNextFloat(&f2);
        positionsVec.push_back(f2);
        //fprintf(stderr, "%i: %f,%f,%f\n", i, f0, f1, f2);
        SkipToNewLine();
        break;

      case 'C': // C found, parse colours
        //fprintf(stderr, "Parsing C ");
        cursor++;

        GetNextInt(&i);
        GetNextFloat(&f0);
        coloursVec.push_back(f0);
        GetNextFloat(&f1);
        coloursVec.push_back(f1);
        GetNextFloat(&f2);
        coloursVec.push_back(f2);
        GetNextFloat(&f3);
        coloursVec.push_back(f3);
        //fprintf(stderr, "%i: %f,%f,%f,%f\n", i, f0, f1, f2, f3);
        SkipToNewLine();
        break;

      case 'T': // T found, parse indices
        //fprintf(stderr, "Parsing T ");
        cursor++;

        GetNextInt(&i);
			  GetNextInt(&i0);
        indicesVec.push_back((unsigned short)i0);
			  GetNextInt(&i1);
        indicesVec.push_back((unsigned short)i1);
			  GetNextInt(&i2);
        indicesVec.push_back((unsigned short)i2);
			  //fprintf(stderr, "%i: %i,%i,%i\n", i, i0, i1, i2);
        SkipToNewLine();
        break;

      case 'U': // U found, parse textures
        //fprintf(stderr, "Parsing U ");
        cursor++;

        GetNextInt(&i);
        GetNextFloat(&f0);
        textureVec.push_back(f0);
        GetNextFloat(&f1);
        textureVec.push_back(f1);
        //fprintf(stderr, "%i: %f, %f\n", i, f0, f1);
        SkipToNewLine();
        break;

      case 'N':
        //fprintf(stderr, "Parsing N ");
        cursor++;

        GetNextInt(&i);
        GetNextFloat(&f0);
        normalsVec.push_back(f0);
        GetNextFloat(&f1);
        normalsVec.push_back(f1);
        GetNextFloat(&f2);
        normalsVec.push_back(f2);
        //fprintf(stderr, "%i: %i, %i, %i\n", i, i0, i1, i2);
        SkipToNewLine();
        break;

      default:
      {
        //fprintf(stderr, "First char not recognised, setting data\n");
        //pTheModel->vertices = CheckLineCount('P');
        
        if (pTheModel->drawMode == pTheModel->INDEXED)
        {
          pTheModel->vertices = (CheckLineCount('P') * CheckLineCount('T'));
          pTheModel->indMax = CheckLineCount('P'); 
        }
        else if (pTheModel->drawMode == pTheModel->DIRECT)
        {
          pTheModel->vertices = CheckLineCount('P');
        }
        
        if (pTheModel->normalMode == pTheModel->GEN_NORMALS)
        {
          GenFlatNormals();
        }
        else if (pTheModel->normalMode == pTheModel->GEN_SPLAYED)
        {
          GenSplayNormals();
        }

        SetData();
        break;
      }
    } // Switch

    SkipToNewLine();
  } // while (isNotEOF())
} // parseData()


void ModelFileReader::GenFlatNormals()
{ // Generates the flat normals using vectors and cross products
  Vector position1;
  Vector position2;
  Vector position3;
  Vector subtractVec1;
  Vector subtractVec2;
  Vector normalisedVec;

  for (int i = 0; i < positionsVec.size(); i += 9)
  {
    position1.xValue = positionsVec[i];
    position1.yValue = positionsVec[i+1];
    position1.zValue = positionsVec[i+2];
    position2.xValue = positionsVec[i+3];
    position2.yValue = positionsVec[i+4];
    position2.zValue = positionsVec[i+5];
    position3.xValue = positionsVec[i+6];
    position3.yValue = positionsVec[i+7];
    position3.zValue = positionsVec[i+8];

    subtractVec1 = Vector::SubtractVector(position2, position1);
    subtractVec2 = Vector::SubtractVector(position3, position1);
    normalisedVec = Vector::CrossProduct(subtractVec1, subtractVec2);
    normalisedVec.Normalise();

    for(int j = 0; j < 3; j++)
    {   // Pushes the normals onto the normals vector
      normalsVec.push_back(normalisedVec.xValue);
      normalsVec.push_back(normalisedVec.yValue);
      normalsVec.push_back(normalisedVec.zValue);
    }
  }// for (int i = 0; i < tempPositions.size(); i += 9)
} // GenFlatNormal


void ModelFileReader::GenSplayNormals()
{ // Generate splayed normals for lighting
  Vector normalisedVec;

  for (int i = 0; i < positionsVec.size(); i += 3)
  { // Splayed normals are positions so load them in
    normalisedVec.xValue = positionsVec[i];
    normalisedVec.yValue = positionsVec[i+1];
    normalisedVec.zValue = positionsVec[i+2];
    normalisedVec.Normalise();

    for (int i = 0; i < 3; i++)
    { // Push the normals onto the normal vector
      normalsVec.push_back(normalisedVec.xValue);
      normalsVec.push_back(normalisedVec.yValue);
      normalsVec.push_back(normalisedVec.zValue);
    }
  }
} // GenSplayedNormals()

void ModelFileReader::SetData()
{ // Set the data into the data array, also interweaves the data
  int j = 0;
  int k = 0;

  if (indicesVec.size() > 0) // If we have any indices, create a array for indices
  {
    pTheModel->pIndices = new unsigned short[indicesVec.size()];
    for(int i = 0; i < indicesVec.size(); i++)
    {
      pTheModel->pIndices[i] = indicesVec[i];
    }
  }

  if (pTheModel->hasTexture == false) // If no texture coord we read in, use this interweave
  {
    pTheModel->pModelData = new float[(coloursVec.size() + positionsVec.size() + normalsVec.size())];

    for(int i = 0; i < (coloursVec.size() + positionsVec.size() 
        + normalsVec.size()); i += 10)
    {
      pTheModel->pModelData[i]   = coloursVec[j];
      pTheModel->pModelData[i+1] = coloursVec[j+1]; 
      pTheModel->pModelData[i+2] = coloursVec[j+2];
      pTheModel->pModelData[i+3] = coloursVec[j+3];
      pTheModel->pModelData[i+4] = normalsVec[k];
      pTheModel->pModelData[i+5] = normalsVec[k+1];
      pTheModel->pModelData[i+6] = normalsVec[k+2];
      pTheModel->pModelData[i+7] = positionsVec[k];
      pTheModel->pModelData[i+8] = positionsVec[k+1];
      pTheModel->pModelData[i+9] = positionsVec[k+2];

      j += 4;
      k += 3;
    }
  }
  else // else texture coord were found, use this interweave, creating a new array of 
  { // appropriate size
    pTheModel->pModelData = new float[(coloursVec.size() + positionsVec.size() + normalsVec.size() + textureVec.size())];
    int l = 0;
     
    for(int i = 0; i < (coloursVec.size() + positionsVec.size() + normalsVec.size() + textureVec.size()); i += 12)
    {
      pTheModel->pModelData[i]    = coloursVec[j];
      pTheModel->pModelData[i+1]  = coloursVec[j+1]; 
      pTheModel->pModelData[i+2]  = coloursVec[j+2];
      pTheModel->pModelData[i+3]  = coloursVec[j+3];
      pTheModel->pModelData[i+4]  = textureVec[l];
      pTheModel->pModelData[i+5]  = textureVec[l+1];
      pTheModel->pModelData[i+6]  = normalsVec[k];
      pTheModel->pModelData[i+7]  = normalsVec[k+1];
      pTheModel->pModelData[i+8]  = normalsVec[k+2];
      pTheModel->pModelData[i+9]  = positionsVec[k];
      pTheModel->pModelData[i+10] = positionsVec[k+1];
      pTheModel->pModelData[i+11] = positionsVec[k+2];

      j += 4;
      k += 3;
      l += 2;
    }
  }
} // SetData()