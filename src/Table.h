////////////////////////////////////////////////////////////////////////////////
// taskwarrior - a command line task list manager.
//
// Copyright 2006 - 2011, Paul Beckingham, Federico Hernandez.
// All rights reserved.
//
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation; either version 2 of the License, or (at your option) any later
// version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the
//
//     Free Software Foundation, Inc.,
//     51 Franklin Street, Fifth Floor,
//     Boston, MA
//     02110-1301
//     USA
//
////////////////////////////////////////////////////////////////////////////////
#ifndef INCLUDED_TABLE
#define INCLUDED_TABLE

#include <map>
#include <vector>
#include <string>
#include "Color.h"
#include "Grid.h"

class Table
{
  friend bool sort_compare (int, int);

public:
  enum just   {left, center, right};
  enum order  {ascendingNumeric,
               ascendingCharacter,
               ascendingPriority,
               ascendingDate,
               ascendingDueDate,
               ascendingPeriod,
               descendingNumeric,
               descendingCharacter,
               descendingPriority,
               descendingDate,
               descendingDueDate,
               descendingPeriod};
  enum sizing {minimum = -1, flexible = 0};

           Table ();
  virtual ~Table ();

           Table (const Table&);
           Table& operator= (const Table&);

           void setTableAlternateColor (const Color&);
           void setTablePadding (int);
           void setTableIntraPadding (int);
           void setTableWidth (int);
           void setTableDashedUnderline ();

           int addColumn (const std::string&);
           void setColumnUnderline (int);
           void setColumnPadding (int, int);
           void setColumnWidth (int, int);
           void setColumnWidth (int, sizing);
           void setColumnJustification (int, just);
           void setColumnCommify (int);
           void sortOn (int, order);

           int addRow ();
           void setRowColor (int, const Color&);

           void addCell (int, int, const std::string&);
           void addCell (int, int, char);
           void addCell (int, int, int);
           void addCell (int, int, float);
           void addCell (int, int, double);
           void setCellColor (int, int, const Color&);

           void setDateFormat (const std::string&);
           void setReportName (const std::string&);

           int rowCount ();
           int columnCount ();
           const std::string render (int maxrows = 0, int maxlines = 0);

private:
           std::string getCell (const int, const int);
           Color getColor (const int, const int, const int);
           Color getHeaderUnderline (const int);
           int getPadding (const int);
           int getIntraPadding ();
           void calculateColumnWidths ();
           just getJustification (const int, const int);
           just getHeaderJustification (const int);
           const std::string formatHeader (const int, const int, const int);
           const std::string formatHeaderDashedUnderline (const int, const int, const int);
           void formatCell (const int, const int, const int, const int, const int, std::vector <std::string>&, std::string&);

private:
  std::vector <std::string> mColumns;
  int mRows;
  int mIntraPadding;
  std::map <std::string, Color> mColor;
  std::map <std::string, Color> mUnderline;
  bool mDashedUnderline;
  Color alternate;

  // Padding...
  int mTablePadding;
  std::vector <int> mColumnPadding;

  // Width...
  int mTableWidth;
  std::vector <int> mSpecifiedWidth;
  std::vector <int> mMaxDataWidth;
  std::vector <int> mCalculatedWidth;

  std::map <int, just> mJustification;
  std::map <int, bool> mCommify;
  Grid mData;

  // Sorting...
  std::vector <int> mSortColumns;
  std::map <int, order> mSortOrder;

  // Misc...
  std::string mDateFormat;
  std::string mReportName;
};

#endif

////////////////////////////////////////////////////////////////////////////////
