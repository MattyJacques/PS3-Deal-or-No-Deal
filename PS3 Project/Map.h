#ifndef MAP_H
#define MAP_H

class Map
{
public:
  char mapArray[64][64]; // Holds the map array for the draw
  bool firstPass;        // holds a bool to check if objects needed to be created this pass

  Map(char* _filename);
  void DrawData();
  bool IsWall(int index1, int index2);
};

#endif
