#include "OthelloExceptions.h"
#include "OthelloBoard.h"
#include "OthelloView.h"
#include "OthelloMove.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {
   // Initialization
   OthelloBoard board; // the state of the game board
   OthelloView v(&board); // a View for outputting the board via operator<<
   string userInput; // a string to hold the user's command choice
   vector<OthelloMove *> possMoves; // a holder for possible moves
   const vector<OthelloMove *> *history = board.GetMoveHistory();

   // Main loop
   do {
	   // Print the game board using the OthelloView object
      cout << v;

      // Print all possible moves
      OthelloMove *m = board.CreateMove();
      board.GetPossibleMoves(&possMoves);
      cout << "Possible moves are: ";
      for (OthelloMove *mov : possMoves) {
         cout << (string)*mov << " ";
      }
      cout << endl;

// ---------------------------------------MAIN COMMAND LOOP--------------------
//=============================================================================
      bool moveApplied = false;
      while (!moveApplied) {
         try {
            if (board.GetNextPlayer() == 1)
               cout << "Black's Turn. ";
            else
               cout << "White's Turn. ";
            cout << "Enter A Command: ";
            getline(cin, userInput);

            //--------------------------SHOW BOARD VALUE-----------------------
            if (userInput == "showValue") {
               cout << "\nCurrent board value is " << board.GetValue() << endl;
            }
            //-----------------------------SHOW HISTORY------------------------
            else if (userInput == "showHistory") {
               int player = board.GetNextPlayer();
               cout << "\nBOARD HISTORY: " << endl;
               for (vector<OthelloMove*>::const_reverse_iterator 
                itr = history->rbegin(); itr != history->rend(); itr++) {
                  if (player == -1)
                     cout << "BLACK played ";
                  else
                     cout << "WHITE played ";
                  cout << (string)**itr << endl;
                  player *= -1;
               }
            }
            //--------------------------------UNDO-----------------------------
            else if (userInput.substr(0, 4) == "undo") {
               string num = userInput.substr(5, userInput.size() - 1);
               int numUndos;
               stringstream(num) >> numUndos;
               while (board.GetMoveCount() > 0 && numUndos > 0) {
                  board.UndoLastMove();
                  numUndos--;
               }
               moveApplied = true;
            }
            //---------------------------QUIT THE GAME-------------------------
            else if (userInput == "quit") {
               *m = "pass";
               board.ApplyMove(m);
               board.ApplyMove(m);
               moveApplied = true;
            }
            //-------------------------------APPLY A MOVE----------------------
            else if (userInput.substr(0, 4) == "move"){
               *m = userInput.substr(5, userInput.size() - 5);

               //checks possible moves, if user input is one of these moves, 
               //then it will be applied 
               for (OthelloMove *mov : possMoves) {
                  if (*mov == *m) {
                     board.ApplyMove(m);
                     moveApplied = true;
                  }
               } //FOR
               if (moveApplied == false)
                  cout << "Invalid move. Try again..." << endl;
            }
         }
         catch (OthelloException &e) {
            cout << "Invalid move. ";
            cout << e.what() << endl;
         }
      }//END WHILE


      for (OthelloMove *mov : possMoves) {
         delete mov;
      }
      possMoves.clear();


   } while (!board.IsFinished()); 

   cout << "------------GAME OVER!-------------" << endl;
   if (board.GetValue() > 0)
      cout << "BLACK WINS!" << endl;
   else if (board.GetValue() < 0)
      cout << "WHITE WINS!" << endl;
   else
      cout << "ITS A TIE!" << endl;

   system("PAUSE");
}