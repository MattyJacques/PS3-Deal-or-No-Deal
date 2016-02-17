#ifndef MODEL_H
#define MODEL_H

#include <PSGL/psgl.h>
#include <PSGL/psglu.h>
#include <GLES/glext.h>
#include "ModelFileReader.h"
#include "Vector.h"
#include "Material.h"


class Model
{
	public:
	float *pModelData;          // holds the array for the model data
	unsigned short *pIndices;   // holds the indices array
	int vertices;               // holds how many vertices there are in current model
  int faceMode;               // holds the int for the facemode
	int indMax;                 // holds the max index value
  int stride;                 // holds the stride for the data pointers
  int textureIndex;           // holds the texture index for the model
  bool hasTexture;            // holds if the model has a texture
	enum ptDrawMode { INDEXED, DIRECT, NONE };        // Enum for the drawmode
  enum ptNormalMode { GEN_NORMALS, GEN_SPLAYED, GEN_NONE };       // Enum to how which normals we need
  ptNormalMode normalMode;          // Holds which normal mode we need
	ptDrawMode drawMode;              // Holds which draw mode we need
	GLuint VBOs[2];                   // The two VBOs to bind when needed


	Model();
	~Model();
	void Draw();
	bool loadModel(char* _filename);

};

#endif



