#include "OthelloBoard.h"
#include <iostream>
using namespace std;

OthelloBoard::OthelloBoard()
   : mNextPlayer(1), mValue(0)
{
   memset(mBoard, 0, BOARD_SIZE * BOARD_SIZE);
   mBoard[BOARD_SIZE / 2 - 1][BOARD_SIZE / 2] = BLACK;
   mBoard[BOARD_SIZE / 2][BOARD_SIZE / 2 - 1] = BLACK;
   mBoard[BOARD_SIZE / 2 - 1][BOARD_SIZE / 2 - 1] = WHITE;
   mBoard[BOARD_SIZE / 2][BOARD_SIZE / 2] = WHITE;
}


void OthelloBoard::GetPossibleMoves(std::vector<OthelloMove *> *list) const {
   //checks every space on the board
   for (int i = 0; i < BOARD_SIZE; i++)
      for (int j = 0; j < BOARD_SIZE; j++) {
         //IF SPOT IS EMPTY THEN CHECK FOR POSSIBLE MOVES
         if (mBoard[i][j] == EMPTY) {
            //checks each direction.
            for (int k = -1; k <= 1; k++)
               for (int s = -1; s <= 1; s++) {
                  int flp = 1;
                  bool canFlip = true, isDuplicate = false;
                  while (canFlip) {
                     
                     //if neighbor is an enemy, increment flip
                     if (mBoard[i + k * flp][j + s * flp] == mNextPlayer * -1)
                        flp++;

                     //CANT FLIP if immediate neighbor is
                     //empty, same team, or move is out of bounds
                     if (mBoard[i + k][j + s] == mNextPlayer
                      || mBoard[i + k][j + s] == EMPTY
                      || !InBounds(i + k * flp, j + s * flp))
                        canFlip = false;
                    
                     if (flp > 1 && InBounds(i + k * flp, j + s * flp)) {
                        //CANT FLIP if enemy found, but end on an empty,
                        //or end out of bounds.
                        if (mBoard[i + k * flp][j + s * flp] == EMPTY)
                           canFlip = false;

                        //IS A VALID MOVE if you have found enemies, 
                        //and the next checked is your own.
                        else if (mBoard[i + k * flp][j + s * flp] 
                         == mNextPlayer) {
                           OthelloMove *move = new OthelloMove(i, j);
                           
                           //check for duplicates. 
                           //if duplicate, DONT add it to getPossibleMoves
                           for (vector<OthelloMove*>::const_iterator 
                            itr = list->cbegin(); itr != list->cend(); itr++) {
                              if (**itr == *move) {
                                 isDuplicate = true;
                                 delete move;
                              }
                           }
                           if (!isDuplicate)
                              list->push_back(move);
                           canFlip = false;
                        }
                     }
                  }
               }
         }
      }
   if (list->size() == 0) {
      list->push_back(new OthelloMove(-1, -1));
   }
}//GET POSSIBLE MOVES

void OthelloBoard::ApplyMove(OthelloMove *move) {
   //applies move to designated row/column
   if (move->IsPass()) {
      mPassCount++;
      move->AddFlipSet(OthelloMove::FlipSet(0, 0, 0));
   }
   else {
      mBoard[move->mRow][move->mCol] = mNextPlayer;
      mValue += mNextPlayer;
      mPassCount = 0;
      //series of loops and decisions determine which tiles will be flipped
      for (int i = -1; i <= 1; i++) {
         for (int j = -1; j <= 1; j++) {
            int flp = 1;
            while (flp > 0) {
               //if out of bounds, empty space, or neighbor is same team
               //then break loop
               if (!InBounds(move->mRow + i * flp, move->mCol + j * flp)
                || mBoard[move->mRow + i * flp][move->mCol + j * flp] == EMPTY
                || (mBoard[move->mRow + i][move->mCol + j] == mNextPlayer))
                  flp = 0;
               //if enemy is found, number of flip increases by one
               if (mBoard[move->mRow + i * flp][move->mCol + j * flp] 
                == (mNextPlayer * -1))
                  flp++;
               //if ally is found, flips the appropriate tiles
               if (flp > 1 && mBoard
                [move->mRow + i * flp][move->mCol + j * flp] == mNextPlayer
                && InBounds(move->mRow + i * flp, move->mCol + j * flp)) {
                  move->AddFlipSet(OthelloMove::FlipSet(flp, i, j));
                  for (; flp >= 1; flp--) {
                     mBoard[move->mRow + i * flp][move->mCol + j * flp] 
                      = mNextPlayer;
                     if (flp > 1)
                        mValue += (2 * mNextPlayer);
                  }
               }
            }
         }
      }
   }
   //adds move to history
   mHistory.push_back(move);
   mNextPlayer *= -1;
}

void OthelloBoard::UndoLastMove() {
   OthelloMove *lastMove = mHistory[mHistory.size() - 1];
   for (int i = 0; i < lastMove->mFlips.size(); i++) {
      int rowDir = lastMove->mFlips[i].rowDelta;
      int colDir = lastMove->mFlips[i].colDelta;
      int numSwitched = lastMove->mFlips[i].switched - 1;
      //UNDO THE MOVE
      for (; numSwitched > 0; numSwitched--) {
         mBoard[lastMove->mRow + numSwitched * rowDir]
          [lastMove->mCol + numSwitched * colDir] *= -1;
         mValue += (2 * mNextPlayer);
      }
   }
   if (!(lastMove->IsPass())) {
      mBoard[lastMove->mRow][lastMove->mCol] = EMPTY;
      mValue += mNextPlayer;
   }
   mNextPlayer *= -1;

   mHistory.pop_back();
   delete lastMove;
}