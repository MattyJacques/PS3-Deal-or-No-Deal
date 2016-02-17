#ifndef MATERIAL_H
#define MATERIAL_H

#include <PSGL/psgl.h>
#include <PSGL/psglu.h>

class Material
{
public:
  float diffuse[4];  // Holds the diffuse of the material
  float ambient[4];   // Holds the ambient of the material
  float emission[4];    // Holds the emission of the material
  float specular[4];    // Holds the specular of the material
  int shininess;        // Holds the shininess of the material
  float black[4];       // Holds the colour black for colour
  GLuint TOName;        // Holds the texture name for binding
  bool gotTexture;      // Holds if the material has a texture
  bool enabled;         // Holds if the material is enabled

  Material();
  void SetDiffuse(float colR, float colG, float colB, float alpha);
  void SetSpecular(float colR, float colG, float colB, float alpha);
  void ApplyMaterial();
  void EnableTexture(int textureIndex);
  void SetAmbient(float colR, float colG, float colB, float alpha);
  void BindTexture(int textureIndex);
  void LoadMaterial(char* _filename);
};

#endif
