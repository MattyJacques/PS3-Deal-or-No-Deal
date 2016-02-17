#ifndef FONT_H
#define FONT_H

#include <PSGL/psgl.h>
#include <PSGL/psglu.h>

class Font
{
private:
  GLuint texObjects[95];    // Holds the textures for each char in the font

public:
  Font();
  void LoadFolder(char* _filename);
  bool UseChar(char c);
  bool UseInt(int num);
};

#endif
