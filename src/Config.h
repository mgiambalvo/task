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
#ifndef INCLUDED_CONFIG
#define INCLUDED_CONFIG

#include <map>
#include <vector>
#include <string>
#include "File.h"

class Config : public std::map <std::string, std::string>
{
public:
  Config ();
  Config (const std::string&);

  Config (const Config&);
  Config& operator= (const Config&);

  void load (const std::string&, int nest = 1);
  void parse (const std::string&, int nest = 1);

  void createDefaultRC (const std::string&, const std::string&);
  void createDefaultData (const std::string&);
  void setDefaults ();
  void clear ();

  const std::string get        (const std::string&);
  const int         getInteger (const std::string&);
  const double      getReal    (const std::string&);
  const bool        getBoolean (const std::string&);

  void set (const std::string&, const int);
  void set (const std::string&, const double);
  void set (const std::string&, const std::string&);
  void all (std::vector <std::string>&) const;

  std::string checkForDeprecatedColor ();
  std::string checkForDeprecatedColumns ();

public:
  File original_file;

private:
  static std::string defaults;
};

#endif

////////////////////////////////////////////////////////////////////////////////
