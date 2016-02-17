#include "Material.h"
#include "MaterialFileReader.h"

Material::Material()
{ // Sets the default values for the material object
  SetDiffuse(0.0, 0.0, 0.0, 1.0);
  ambient[0] = 0.0;
  ambient[1] = 0.0;
  ambient[2] = 0.0;
  ambient[3] = 1.0;
  emission[0] = 0.0;
  emission[1] = 0.0;
  emission[2] = 0.0;
  emission[3] = 1.0;
  specular[0] = 0.0;
  specular[1] = 0.0;
  specular[2] = 0.0;
  specular[3] = 1.0;
  shininess = 127;
  black[0] = 0.0;
  black[1] = 0.0;
  black[2] = 0.0;
  black[3] = 1.0;
  TOName = 1;

  gotTexture = true;
  enabled = true;
} // Constructor()

void Material::SetDiffuse(float colR, float colG, float colB, float alpha)
{ // Sets a custom diffuse for the material
	diffuse[0] = colR;
	diffuse[1] = colG;
	diffuse[2] = colB;
	diffuse[3] = alpha;
}//setDiffuse()

void Material::SetSpecular(float colR, float colG, float colB, float alpha)
{ // Sets a custom specular for the material
  specular[0] = colR;
	specular[1] = colG;
	specular[2] = colB;
	specular[3] = alpha;
} // SetSpecular()

void Material::SetAmbient(float colR, float colG, float colB, float alpha)
{ // Sets a custom ambient for the material
  ambient[0] = colR;
	ambient[1] = colG;
	ambient[2] = colB;
	ambient[3] = alpha;
} // SetAmbient()

void Material::ApplyMaterial()
{ // Applies the material to the buffer by binding the material
  if (gotTexture) // if has texture, enable texture otherwise disable texture
  {
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  }
  else
  {
    glDisable(GL_TEXTURE_2D);
  }

  if (enabled) // If material enabled, disable all default colours of the model
  {
    glDisableClientState(GL_COLOR_ARRAY);
    glDisable(GL_COLOR_MATERIAL);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
  }
  else // otherwise enable default model colour
  {
    glEnableClientState(GL_COLOR_ARRAY);
    glEnable(GL_COLOR_MATERIAL); 
    glColorPointer(4, GL_FLOAT, sizeof(GLfloat)*12, (void *)(GLintptr) 0);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, black);
  }
} // ApplyMaterial()

void Material::EnableTexture(int textureIndex)
{ // Enables the chosen texture
  gotTexture = true;
  enabled = true;
  BindTexture(textureIndex);
} // SetTexture

void Material::BindTexture(int textureIndex)
{ // Gives the texture name to the material
  TOName = textureIndex;
} // BindTexture()

void Material::LoadMaterial(char* _filename)
{ // Load the material from file
  MaterialFileReader myMaterial(_filename, this);
} // LoadMaterial()