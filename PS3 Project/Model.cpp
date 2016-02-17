#include "Model.h"
#include "GraphicsManager.h"
#include "AssetManager.h"

Model::Model()
{ // Set members to their default value
	drawMode = NONE;
  normalMode = GEN_NONE;
  vertices = 0;
  stride = 10;
  hasTexture = false;
  textureIndex = 0;
  glGenBuffers(2, VBOs);
  
} // constructor()

Model::~Model()
{   // Clears the binds for deletion
  glBindBuffer (GL_ARRAY_BUFFER, VBOs[0]);
  glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_STATIC_DRAW);
  glBindBuffer (GL_ARRAY_BUFFER, VBOs[1]);
  glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_STATIC_DRAW);
  glDeleteBuffers(2, VBOs);
} // destructor()



void Model::Draw()
{ // If the model is direct data, draw using drawarrays
	if (drawMode == DIRECT) 
	{
		glDrawArrays(GL_TRIANGLES, 0, vertices);
	}
  else if (drawMode == INDEXED) // else draw using draw range elements
	{
		glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, VBOs[1]);
		glDrawRangeElements(GL_TRIANGLES, 0, indMax, vertices, GL_UNSIGNED_SHORT, (void *)(GLintptr) 0);
	}//if
}//draw()


bool Model::loadModel(char* _filename)
{ // Loads the file into the model object
  ModelFileReader myFile(_filename, this);

  glBindBuffer (GL_ARRAY_BUFFER, VBOs[0]);
  glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_STATIC_DRAW);
  glBufferData(GL_ARRAY_BUFFER, vertices * 12 * sizeof(float), pModelData, GL_STATIC_DRAW);

  if (drawMode == INDEXED) // Loads in the indices if needed
  {
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, NULL, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertices * 3 * sizeof(unsigned short), pIndices, GL_STATIC_DRAW);
  }

  return true;
} // loadModel



