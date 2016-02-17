#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include "Model.h"
#include "Map.h"
#include "Font.h"
#include "Gameboard.h"
#include "Box.h"

// Defines counts for arrays
#define TEX_COUNT 51
#define MODEL_COUNT 9
#define MATERIAL_COUNT 51
#define FONT_COUNT 1

// Defines texture types
#define TEXTYPE_GIF 0
#define TEXTYPE_PNG 1

// Defines paths for files
#define TEX_PATH "/assets/textures/"
#define MODEL_PATH "/assets/models/"
#define MATERIAL_PATH "/assets/materials/"
#define FONT_PATH "/assets/fonts/"

// Defines texture constants
#define TEX_WALL     0
#define TEX_CEILING  1
#define TEX_FLOOR    2
#define TEX_TABLE    3
#define TEX_1P       4
#define TEX_10P      5
#define TEX_50P      6
#define TEX_1        7
#define TEX_5        8
#define TEX_10       9
#define TEX_50       10
#define TEX_100      11
#define TEX_250      12
#define TEX_500      13
#define TEX_750      14
#define TEX_1000     15
#define TEX_3000     16
#define TEX_5000     17
#define TEX_10000    18
#define TEX_15000    19
#define TEX_20000    20
#define TEX_35000    21
#define TEX_50000    22
#define TEX_75000    23
#define TEX_100000   24
#define TEX_250000   25
#define TEX_BLACK    26
#define TEX_RED      27
#define TEX_BOX_1    28
#define TEX_BOX_2    29
#define TEX_BOX_3    30
#define TEX_BOX_4    31
#define TEX_BOX_5    32
#define TEX_BOX_6    33
#define TEX_BOX_7    34
#define TEX_BOX_8    35
#define TEX_BOX_9    36
#define TEX_BOX_10   37
#define TEX_BOX_11   38
#define TEX_BOX_12   39
#define TEX_BOX_13   40
#define TEX_BOX_14   41
#define TEX_BOX_15   42
#define TEX_BOX_16   43
#define TEX_BOX_17   44
#define TEX_BOX_18   45
#define TEX_BOX_19   46
#define TEX_BOX_20   47
#define TEX_BOX_21   48
#define TEX_BOX_22   49
#define TEX_WOOD     50

// Defines model constants
#define MODEL_HUD             0
#define MODEL_DIRECT_CUBE     1
#define MODEL_FLOOR           2
#define MODEL_WALL            3
#define MODEL_CASH_NOTICE     4
#define MODEL_BOX             5
#define MODEL_BOX_LID         6
#define MODEL_BOARD_SIDE      7
#define MODEL_BOARD_BOTTOM    8

// Defines font constants
#define FONT_HUDFONT 0

class AssetManager
{
private:
  static AssetManager *instance;
  AssetManager();
  static char* ptTexFiles[TEX_COUNT];             // Holds the array of Texture filenames
  static char* ModelFiles[MODEL_COUNT];           // Holds the array of model filenames
  static char* ptMaterialFiles[MATERIAL_COUNT];   // Holds the array of material filenames
  static char* ptFontFolders[FONT_COUNT];         // Holds the array of font folders
  int currentFont;                                // Holds the currentFont number
  int currentModel;                               // Holds the currentModel number
  int currentMaterial;                            // Holds the currentMaterial number
  Model theModels[MODEL_COUNT];           // Holds the models
  Material theMaterials[MATERIAL_COUNT];  // Holds the materials
  GLuint texObjects[TEX_COUNT];           // Holds the textures

public:
  Gameboard* theBoard;                    // Points to the board object
  Map* theMap;                            // Points to the map object
  Font* theFont;                          // Points to the font object
  Box* theBoxes[22];                      // Holds the box objects
  int boxTexArray[22];                    // Holds the texture sequence


  static void Start();
  static void Terminate();
  static AssetManager* GetInstance();
  void LoadAll();
  Model* GetModel(int modelIndex);
  Material* GetMaterial(int materialIndex);
  void DrawModel(int modelIndex, int materialIndex);
  void ChangeModel(int modelIndex);
  void ChangeMaterial(int materialIndex);
  void ChangeFont(int fontIndex);
  int GetFont();
  void GenBoxes();
};
#endif