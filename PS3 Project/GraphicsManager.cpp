#include <sys/paths.h>
#include "GraphicsManager.h"
#include "Camera.h"


GraphicsManager* GraphicsManager::instance = NULL;


GraphicsManager::GraphicsManager()
{ // Initialises PSGL for to use for drawing and translating, sets up the draw settings
  // then creates the light and HUD.
  InitialisePSGL();
  SetupDraw();
  CreateLight();
  pHUD = new HUD();
} // Constructor()


GraphicsManager::~GraphicsManager()
{
} // Destructor()


void GraphicsManager::CreateCamera()
{ // Creates a new camera object
  theCamera = new Camera();
} // CreateCamera()


void GraphicsManager::CreateLight()
{ // Creates two lights to light up the game world
  theLight[0] = new Light(10.0, 20.0, 10.0, 1.0, 1.0, 
                       1.0, 1.0);
  theLight[1] = new Light(-10.0, -20.0, -10.0, 1.0, 1.0, 
                       1.0, 1.0);
} // CreateLight()


GraphicsManager* GraphicsManager::GetInstance()
{ // Creates a new instance of graphics manager if not already made
  // and then returns the instance
 if(!instance) 
 {
   instance = new GraphicsManager();
 }

 return instance;

} // GetInstance()


void GraphicsManager::InitialisePSGL()
{ // Initialise the PSGL functions to use for the game
  psglInit(NULL);
	device = psglCreateDeviceAuto(GL_ARGB_SCE,GL_DEPTH_COMPONENT24,GL_MULTISAMPLING_4X_SQUARE_ROTATED_SCE);
	if (!device) 
  { 
    printf("!! Failed to init the device \n!! Exiting Program \n" );
    exit(1);
  }
	context = psglCreateContext();
	if (!context) 
  { 
    fprintf(stderr, "Error creating PSGL context\n"); 
    exit(1); 
  }
	psglMakeCurrent(context, device);
	psglResetCurrentContext();
  psglLoadShaderLibrary(SYS_APP_HOME"/shaders.bin");
} // InitialisePSGL()


void GraphicsManager::SetupDraw()
{
  //drawing settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
  glEnable(GL_VSYNC_SCE);

  //clear screen settings
	glClearColor (1.0f, 0.0f, 0.0f, 1.0f);
	glClearStencil (0);
	glClearDepthf (1.0f);

  //do a first clear and swap - probably redundant
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	psglSwap();	

  //setup drawing mode
	//note: it's common to have to keep changing some of this, 
	//but right now we have only one model  
	glMatrixMode(GL_MODELVIEW);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_FLAT);

  // Enable transparency PSGL things
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
} // SetUpDraw


void GraphicsManager::RefreshLight(Camera *theCamera)
{ // refreshes the light so it is fixed in the game world
  glMatrixMode(GL_MODELVIEW);
  glLightfv(GL_LIGHT0, GL_POSITION, theLight[0]->position);
  glLightfv(GL_LIGHT1, GL_POSITION, theLight[1]->position);
} // RefreshLight()

void GraphicsManager::DrawWorld()
{ // Draws the game world but calling all of the appropriate draw functions
  AssetManager* pMyAM = AssetManager::GetInstance();
  if (!pMyAM->theBoard->bankerOffer) // If no banker off active draw normally
  {
    theCamera->Process();
    glPushMatrix();
    pMyAM->theMap->DrawData();
    glPopMatrix();
    glPushMatrix();
    pMyAM->theBoard->DrawBoard();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(63, -3, 63);
    pMyAM->DrawModel(MODEL_FLOOR, TEX_FLOOR);
    glTranslatef(0, 6, 0);
    glRotatef(180, 1, 0, 0);
    pMyAM->DrawModel(MODEL_FLOOR, TEX_CEILING);
    glPopMatrix();
    pMyAM->theBoard->CheckBox();
  }
  else                          // else draw end game or banker offer
  {
    pHUD->StartHUD();
    if (pMyAM->theBoard->endGame)
    {
      pHUD->DisplayWin();
    }
    else
    {
      pHUD->DisplayOffer();
    }
    pHUD->StopHUD();
  }

  pHUD->StartHUD(); // Starts and stops the drawing for the HUD to the screen
  pHUD->DisplayHUD();
  pHUD->StopHUD();
} // DrawWorld()