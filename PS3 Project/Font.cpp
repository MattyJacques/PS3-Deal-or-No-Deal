#include "Font.h"
#include "AssetManager.h"
#include "PNGReader.h"
#include "FontList.h"
#include <stdio.h>

Font::Font()
{ // Generates the texture objects for the font chars
  glGenTextures(95, texObjects);
} // Constructor()

void Font::LoadFolder(char* _filename)
{ // Loads the folder full of the font char images to use as the textures
  PNGReader myPNGGetter; // Creates a png getter object to load in PNGs
  for (int j = 0; j < FONT_COUNT; j++)
  { // Loops through to load of the chars for all of the fonts
    for (int i = 0; i < 95; i++)
    {
      char filename[1024] = "";
      strcat(filename, SYS_APP_HOME);
      strcat(filename, FONT_PATH);
      strcat(filename, _filename);
      strcat(filename, ptFontFilenames[i]);

      glBindTexture(GL_TEXTURE_2D, texObjects[i]); // Bind the current texture object
      myPNGGetter.loadFile(filename);              // Loads the file we are up to from the array
    }
  }
} // LoadFolder()

bool Font::UseChar(char c)
{ // Binds the appropriate char to the next model to use as text.
  bool result = false;
  int asciiNum = c-32;
  if (asciiNum >= 0 && asciiNum <= 96)
  { // Checks to see if it is a valid char before binding
    result = true;
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, texObjects[asciiNum]);
  }

  return result;
} // UseChar()

bool Font::UseInt(int num)
{ // Binds the appropriate texture using num as the index
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, texObjects[num]);

  return true;
} // UseChar()