#include "OthelloView.h"
#include <iostream>
using namespace std;

void OthelloView::PrintBoard(ostream &s) const {
   s << "   0 1 2 3 4 5 6 7\n";
   s << "   ---------------\n";
   for (int i = 0; i < BOARD_SIZE; i++) {
      s << i << "| ";
      for (int j = 0; j < BOARD_SIZE; j++) {
         if (mOthelloBoard->mBoard[i][j] == 1)
            s << "B ";
         else if (mOthelloBoard->mBoard[i][j] == -1)
            s << "W ";
         else s << ". ";
      }
      s << endl;
   }
}

ostream& operator<< (std::ostream &oss, const OthelloView &view) {
   view.PrintBoard(oss);
   return oss;
}