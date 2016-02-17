#include <algorithm>
#include "AssetManager.h"
#include "GIFReader.h"
#include "PNGReader.h"

AssetManager* AssetManager::instance = NULL;

char* AssetManager::ptTexFiles[TEX_COUNT] = 
{
  "wall.gif",               //0
  "ceiling.gif",            //1
  "floor.gif",              //2
  "table.gif",              //3
  "1p.gif",                 //4
  "10p.gif",                //5
  "50p.gif",                //6
  "1.gif",                  //7
  "5.gif",                  //8
  "10.gif",                 //9
  "50.gif",                 //10
  "100.gif",                //11
  "250.gif",                //12
  "500.gif",                //13
  "750.gif",                //14
  "1000.gif",               //15
  "3000.gif",               //16
  "5000.gif",               //17
  "10000.gif",              //18
  "15000.gif",              //19
  "20000.gif",              //20
  "35000.gif",              //21
  "50000.gif",              //22
  "75000.gif",              //23
  "100000.gif",             //24
  "250000.gif",             //25
  "black.gif",              //26
  "red.gif",                //27
  "box1.gif",               //28
  "box2.gif",               //29
  "box3.gif",               //30
  "box4.gif",               //31
  "box5.gif",               //32
  "box6.gif",               //33
  "box7.gif",               //34
  "box8.gif",               //35
  "box9.gif",               //36
  "box10.gif",              //37
  "box11.gif",              //38
  "box12.gif",              //39
  "box13.gif",              //40
  "box14.gif",              //41
  "box15.gif",              //42
  "box16.gif",              //43
  "box17.gif",              //44
  "box18.gif",              //45
  "box19.gif",              //46
  "box20.gif",              //47
  "box21.gif",              //48
  "box22.gif",              //49
  "wood.gif",               //50

};


char* AssetManager::ModelFiles[MODEL_COUNT] = 
{
  "HUDtile.ptm",                //0
  "DirectCubeTextured.ptm",     //1
  "floor.ptm",                  //2
  "wall.ptm",                   //3
  "CashNotice.ptm",             //4
  "Box.ptm",                    //5
  "BoxLid.ptm",                 //6
  "BoardSide.ptm",              //7
  "BoardBottom.ptm",            //8
};


char* AssetManager::ptMaterialFiles[MATERIAL_COUNT] = 
{
  "texture1.ptmat",             //0
  "texture2.ptmat",             //1
  "texture3.ptmat",             //2
  "texture4.ptmat",             //3
  "texture5.ptmat",             //4
  "texture6.ptmat",             //5
  "texture7.ptmat",             //6
  "texture8.ptmat",             //7
  "texture9.ptmat",             //8
  "texture10.ptmat",            //9
  "texture11.ptmat",            //10
  "texture12.ptmat",            //11
  "texture13.ptmat",            //12
  "texture14.ptmat",            //13
  "texture15.ptmat",            //14
  "texture16.ptmat",            //15
  "texture17.ptmat",            //16
  "texture18.ptmat",            //17
  "texture19.ptmat",            //18
  "texture20.ptmat",            //19
  "texture21.ptmat",            //20
  "texture22.ptmat",            //21
  "texture23.ptmat",            //22
  "texture24.ptmat",            //23
  "texture25.ptmat",            //24
  "texture26.ptmat",            //25
  "texture27.ptmat",            //26
  "texture28.ptmat",            //27
  "texture29.ptmat",            //28
  "texture30.ptmat",            //29
  "texture31.ptmat",            //30
  "texture32.ptmat",            //31
  "texture33.ptmat",            //32
  "texture34.ptmat",            //33
  "texture35.ptmat",            //34
  "texture36.ptmat",            //35
  "texture37.ptmat",            //36
  "texture38.ptmat",            //37
  "texture39.ptmat",            //38
  "texture40.ptmat",            //39
  "texture41.ptmat",            //40
  "texture42.ptmat",            //41
  "texture43.ptmat",            //42
  "texture44.ptmat",            //43
  "texture45.ptmat",            //44
  "texture46.ptmat",            //45
  "texture47.ptmat",            //46
  "texture48.ptmat",            //47
  "texture49.ptmat",            //48
  "texture50.ptmat",            //49
  "texture51.ptmat",            //50

};

int ptTexTypes[TEX_COUNT] = 
{
	TEXTYPE_GIF,		//0
	TEXTYPE_GIF,		//1
	TEXTYPE_GIF,		//2
	TEXTYPE_GIF,		//3
};

char* AssetManager::ptFontFolders[FONT_COUNT] =
{
  "hudfont/"                  //0
};



AssetManager::AssetManager()
{ // Constructor that initialises boxTexArray and class members
  // to be null for safe measure. Then loads assests needed for the
  // game.
  for (int i = 0; i < 22; i++)
  {
    boxTexArray[i] = i;
  }

  currentFont = 0;
  theMap = NULL;
  theFont = NULL;
  theBoard = NULL;
  currentMaterial = -1;
  currentModel = -1;
  LoadAll();
} // Constructor()


AssetManager* AssetManager::GetInstance()
{ // Returns the instance of asset manager.
 return instance;
} // GetInstance()


void AssetManager::Start()
{ // Starts up the asset manager and returns the pointer to the
  // instance.
 if(!instance) 
 {
   instance = new AssetManager();
 }
} // Start()

void AssetManager::Terminate()
{ // Terminates the asset manager, deleting the instance.
 if(!instance) 
 {
   delete instance;
 }

 instance = NULL;
} // Terminate()

void AssetManager::LoadAll()
{ // Generates texture objects, then loads all of the various assets
  // needed to create the game.
  glGenTextures(TEX_COUNT, texObjects);
  GIFReader textureGetter; // Creates the GIFReader object
  PNGReader pngGetter;     // Creates the PNGReader object

  theMap = new Map(SYS_APP_HOME"/assets/maps/map2.ptl");

  for (int i = 0; i < TEX_COUNT; i++)
  { // Creates the filename, binds the texture object and
    // then loads the file
    char filename[1024] = "";
    strcat(filename, SYS_APP_HOME);
    strcat(filename, TEX_PATH);
    strcat(filename, ptTexFiles[i]);

    glBindTexture(GL_TEXTURE_2D, texObjects[i]);
    if (ptTexTypes[i] == TEXTYPE_GIF)
      textureGetter.loadFile(filename);
    else if (ptTexTypes[i] == TEXTYPE_PNG)
      pngGetter.loadFile(filename);
  }

  for (int i = 0; i < MODEL_COUNT; i++)
  { // Creates the filename, then loads the model file.
    char filename[1024] = "";
    strcat(filename, SYS_APP_HOME);
    strcat(filename, MODEL_PATH);
    strcat(filename, ModelFiles[i]);

    theModels[i].loadModel(filename);
  }

  for (int i = 0; i < MATERIAL_COUNT; i++)
  { // Creates the filename, binds the material object and
    // then loads the file
    char filename[1024] = "";
    strcat(filename, SYS_APP_HOME);
    strcat(filename, MATERIAL_PATH);
    strcat(filename, ptMaterialFiles[i]);

    theMaterials[i].LoadMaterial(filename);
  }

  // Creates a new gameboard
  theBoard = new Gameboard();
  
  // Creates a new font object then loads the font folder in.
  theFont = new Font();
  theFont->LoadFolder(ptFontFolders[0]);
} // LoadAll()


Model* AssetManager::GetModel(int modelIndex)
{ // Returns a pointer to a model object from the array
  return &theModels[modelIndex];
} // GetModel()


Material* AssetManager::GetMaterial(int materialIndex)
{ // Returns a pointer to a material object from the array
  return &theMaterials[materialIndex];
} // GetMaterial()


void AssetManager::DrawModel(int modelIndex, int materialIndex)
{ // Takes in two integers which are indexes for the model and material arrays.

  if (materialIndex != 999) // Checks if the material has changed since last draw
    ChangeMaterial(materialIndex); // if so, changes the material
  if (modelIndex != 999)    // Checks if the model has changed since last draw
    ChangeModel(modelIndex); // if so changes the model

  theModels[modelIndex].Draw(); // Draw the model
} // DrawModel()

void AssetManager::ChangeMaterial(int materialIndex)
{ // Checks to see if the material is different than the last used material
  // if so, binds the new material for the next draw.
  if (currentMaterial != materialIndex)
  {
   theMaterials[materialIndex].ApplyMaterial();
   if (theMaterials[materialIndex].enabled)
   {
     glBindTexture(GL_TEXTURE_2D, theMaterials[materialIndex].TOName);
   }
   // Changes the current material to the one just used for next check.
   currentMaterial = materialIndex; 
  }
} // ChangeMaterial()

void AssetManager::ChangeModel(int modelIndex)
{ // Checks to see if the model is different than the last used model
  // if so, loads the new model for the next draw.
  if (currentModel != modelIndex)
  {
    glBindBuffer(GL_ARRAY_BUFFER, theModels[modelIndex].VBOs[0]);
    if(theMaterials[currentMaterial].gotTexture)
    {
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
      glTexCoordPointer(2, GL_FLOAT, sizeof(GLfloat)*12, (void *)(GLintptr) (4* sizeof(float)));
    }
    else 
    {
      glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
    // Sets the pointers to the next model for the next draw
    glNormalPointer(GL_FLOAT, sizeof(GLfloat)*12, (void *)(GLintptr)(6* sizeof(float)));
    glVertexPointer(3, GL_FLOAT, sizeof(GLfloat)*12, (void *)(GLintptr)(9* sizeof(float)));
    // Changes the current model to the one just used for next check.
    currentModel = modelIndex;
  }
} // ChangeModel()

void AssetManager::ChangeFont(int fontIndex)
{ // Takes in a new font index to switch the font folder.
  if(currentFont != fontIndex)
  {
    currentFont = fontIndex;
  }
} // ChangeFont()

int AssetManager::GetFont()
{ // Returns the current font index/
  return currentFont;
} // GetFont()

void AssetManager::GenBoxes()
{ // Shuffles the boxes and cash prizes to give the random feel for the game.

  int playerChoice = 7; // Sets the player choice to box 8, remove when feature added.
  std::vector<int> boxVec;        // Creates a vector for the boxes.
  std::vector<float> cashVec;     // Creates a vector for the cash prizes.

  for (int i = 0; i < 22; i++)
  { // Pushes back the cash and boxes onto the vector
    cashVec.push_back(Gameboard::cashPrizes[i]);
    if (i != playerChoice)
    {
      boxVec.push_back(boxTexArray[i]);
    }
  }
  srand(time(0)); // Seeds the random number generator
  std::random_shuffle(boxVec.begin(), boxVec.end()); // Shuffles the box vector.
  std::random_shuffle(cashVec.begin(), cashVec.end());  // Shuffles the cash vector

  for (int i = 0; i < 21; i++)
  { // Places the shuffled cash and box order into the array
    boxTexArray[i] = boxVec[i];
    theBoxes[i]->boxNumber = boxVec[i];
    theBoxes[i]->cashAmount = cashVec[i];

  }
  theBoard->playerCash = cashVec[21]; // Sets the player box amount
  theBoard->cashWon = cashVec[21];    // Sets the initial cashWon value.
} // GenBoxes()