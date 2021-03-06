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

#include <string>
#include "Keymap.h"

////////////////////////////////////////////////////////////////////////////////
Keymap::Keymap ()
{
}

////////////////////////////////////////////////////////////////////////////////
Keymap::Keymap (const Keymap& other)
{
  throw std::string ("unimplemented Keymap::Keymap");
//  mOne = other.mOne;
}

////////////////////////////////////////////////////////////////////////////////
Keymap& Keymap::operator= (const Keymap& other)
{
  throw std::string ("unimplemented Keymap::operator=");
  if (this != &other)
  {
//    mOne = other.mOne;
  }

  return *this;
}

////////////////////////////////////////////////////////////////////////////////
Keymap::~Keymap ()
{
}

////////////////////////////////////////////////////////////////////////////////
void Keymap::load (const std::string& file)
{
  throw std::string ("unimplemented Keymap::load");
}

////////////////////////////////////////////////////////////////////////////////
