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
#ifndef INCLUDED_SEQUENCE
#define INCLUDED_SEQUENCE

#include <vector>
#include <string>

#define SEQUENCE_MAX 1000

class Sequence : public std::vector <int>
{
public:
  Sequence ();                           // Default constructor
  Sequence (const std::string&);         // Parse
  ~Sequence ();                          // Destructor

  bool valid (const std::string&) const;
  void parse (const std::string&);
  void combine (const Sequence&);

private:
  bool validId (const std::string&) const;
};

#endif
////////////////////////////////////////////////////////////////////////////////
