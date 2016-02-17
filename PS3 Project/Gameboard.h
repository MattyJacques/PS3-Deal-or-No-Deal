#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include "CashNotice.h"

class AssetManager;

class Gameboard
{
public:
  static float cashPrizes[22];    // Holds all the available cash prizes for the game
  CashNotice* theNotices[22];     // Array of all the notices for the gameboard
  int activeBox;                  // See which box the player is interacting with
  int boxesOpen;                  // Int on how many boxes are currently open
  bool bankerOffer;               // Bool to tell if a current banker offer is active
  bool endGame;                   // Bool to check if end of game is reached.
  bool bankerAccepted;            // Check to see if the player accepted the banker offer
  float playerCash;               // Float to what cash prize is in the players box. 
  int bankersOffer;               // Int to hold the bankers offer to the player
  int timer;                      // int to make sure the player can't spam open boxes.
  float cashWon;                  // float to hold the amount of cash the player has won

  Gameboard();
  void DrawBoard();
  void GenBoxes();
  void CheckBox();
  void OpenBox();
  void BankOffer();
};

#endif
