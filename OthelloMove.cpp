#include "OthelloMove.h"
#include "OthelloBoard.h"
#include "OthelloExceptions.h"
#include<sstream>
using namespace std;

int OthelloMove::mOnHeap = 0;

OthelloMove::OthelloMove() 
   :mRow(-1), mCol(-1)
{
}

OthelloMove::OthelloMove(int row, int col) 
   : mRow(row), mCol(col)
{
}

OthelloMove& OthelloMove::operator=(const std::string &s) {
   istringstream iss(s);
   char temp;
   if (s == "pass") {
      mRow = -1;
      mCol = -1;
   }
   else {
      iss >> temp >> mRow >> temp >> mCol >> temp;
      if (!OthelloBoard::InBounds(mRow, mCol))
         throw OthelloException("That move is out of Bounds");
   }
   return *this;
}

bool operator==(const OthelloMove &lhs, const OthelloMove &rhs) {
   return (lhs.mRow == rhs.mRow && lhs.mCol == rhs.mCol);
}

OthelloMove::operator std::string() const {
   ostringstream oss;
   IsPass() ? oss << "pass" : oss << "(" << mRow << ", " << mCol << ")";
   return oss.str();
}