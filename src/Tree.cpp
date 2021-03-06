////////////////////////////////////////////////////////////////////////////////
// taskwarrior - a command line task list manager.
//
// Copyright 2010 - 2011, Paul Beckingham, Federico Hernandez.
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

#include <algorithm>
#include <iostream>
#include <text.h>
#include <Tree.h>

////////////////////////////////////////////////////////////////////////////////
//  - Tree, Branch and Node are synonymous.
//  - A Tree may contain any number of branches.
//  - A Branch may contain any number of name/value pairs, unique by name.
//  - The destructor will delete all branches recursively.
//  - Tree::enumerate is a snapshot, and is invalidated by modification.
//  - Branch sequence is preserved.
Tree::Tree (const std::string& name)
: _trunk (NULL)
, _name (name)
{
}

////////////////////////////////////////////////////////////////////////////////
Tree::~Tree ()
{
  for (std::vector <Tree*>::iterator i = _branches.begin ();
       i != _branches.end ();
       ++i)
    delete *i;
}

////////////////////////////////////////////////////////////////////////////////
Tree::Tree (const Tree& other)
{
  throw "Unimplemented Tree::Tree (Tree&)";
}

////////////////////////////////////////////////////////////////////////////////
Tree& Tree::operator= (const Tree& other)
{
  throw "Unimplemented Tree::operator= ()";
  return *this;
}

////////////////////////////////////////////////////////////////////////////////
Tree* Tree::operator[] (const int branch)
{
  if (branch < 0 ||
      branch > (int) _branches.size () - 1)
    throw "Tree::operator[] out of range";

  return _branches[branch];
}

////////////////////////////////////////////////////////////////////////////////
void Tree::addBranch (Tree* branch)
{
  branch->_trunk = this;
  _branches.push_back (branch);
}

////////////////////////////////////////////////////////////////////////////////
void Tree::removeBranch (Tree* branch)
{
  for (std::vector <Tree*>::iterator i = _branches.begin ();
       i != _branches.end ();
       ++i)
  {
    if (*i == branch)
    {
      _branches.erase (i);
      return;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
void Tree::replaceBranch (Tree* from, Tree* to)
{
  for (unsigned int i = 0; i < _branches.size (); ++i)
  {
    if (_branches[i] == from)
    {
      to->_trunk = this;
      _branches[i] = to;
      return;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
int Tree::branches ()
{
  return _branches.size ();
}

////////////////////////////////////////////////////////////////////////////////
void Tree::name (const std::string& name)
{
  _name = name;
}

////////////////////////////////////////////////////////////////////////////////
std::string Tree::name () const
{
  return _name;
}

////////////////////////////////////////////////////////////////////////////////
// Accessor for attributes.
void Tree::attribute (const std::string& name, const std::string& value)
{
  _attributes[name] = value;
}

////////////////////////////////////////////////////////////////////////////////
// Accessor for attributes.
void Tree::attribute (const std::string& name, const int value)
{
  _attributes[name] = format (value);
}

////////////////////////////////////////////////////////////////////////////////
// Accessor for attributes.
void Tree::attribute (const std::string& name, const double value)
{
  _attributes[name] = format (value, 1, 8);
}

////////////////////////////////////////////////////////////////////////////////
// Accessor for attributes.
std::string Tree::attribute (const std::string& name)
{
  // Prevent autovivification.
  std::map<std::string, std::string>::iterator i = _attributes.find (name);
  if (i != _attributes.end ())
    return i->second;

  return "";
}

////////////////////////////////////////////////////////////////////////////////
void Tree::removeAttribute (const std::string& name)
{
  _attributes.erase (name);
}

////////////////////////////////////////////////////////////////////////////////
int Tree::attributes () const
{
  return _attributes.size ();
}

////////////////////////////////////////////////////////////////////////////////
std::vector <std::string> Tree::allAttributes () const
{
  std::vector <std::string> names;
  std::map <std::string, std::string>::const_iterator it;
  for (it = _attributes.begin (); it != _attributes.end (); ++it)
    names.push_back (it->first);

  return names;
}

////////////////////////////////////////////////////////////////////////////////
// Recursively completes a list of Tree* objects, left to right, depth first.
// The reason for the depth-first enumeration is that a client may wish to
// traverse the tree and delete nodes.  With a depth-first iteration, this is a
// safe mechanism, and a node pointer will never be dereferenced after it has
// been deleted.
void Tree::enumerate (std::vector <Tree*>& all) const
{
  for (std::vector <Tree*>::const_iterator i = _branches.begin ();
       i != _branches.end ();
       ++i)
  {
    (*i)->enumerate (all);
    all.push_back (*i);
  }
}

////////////////////////////////////////////////////////////////////////////////
Tree* Tree::parent () const
{
  return _trunk;
}

////////////////////////////////////////////////////////////////////////////////
bool Tree::hasTag (const std::string& tag) const
{
  if (std::find (_tags.begin (), _tags.end (), tag) != _tags.end ())
    return true;

  return false;
}

////////////////////////////////////////////////////////////////////////////////
void Tree::tag (const std::string& tag)
{
  if (! hasTag (tag))
    _tags.push_back (tag);
}

////////////////////////////////////////////////////////////////////////////////
int Tree::tags () const
{
  return _tags.size ();
}

////////////////////////////////////////////////////////////////////////////////
std::vector <std::string> Tree::allTags () const
{
  return _tags;
}

////////////////////////////////////////////////////////////////////////////////
int Tree::count () const
{
  int total = 1; // this one.

  for (std::vector <Tree*>::const_iterator i = _branches.begin ();
       i != _branches.end ();
       ++i)
  {
    // Recurse and count the branches.
    total += (*i)->count ();
  }

  return total;
}

////////////////////////////////////////////////////////////////////////////////
Tree* Tree::find (const std::string& path)
{
  std::vector <std::string> elements;
  split (elements, path, '/');

  // Must start at the trunk.
  Tree* cursor = this;
  std::vector <std::string>::iterator it = elements.begin ();
  if (cursor->name () != *it)
    return NULL;

  // Perhaps the trunk is what is needed?
  if (elements.size () == 1)
    return this;

  // Now look for the next branch.
  for (++it; it != elements.end (); ++it)
  {
    bool found = false;

    // If the cursor has a branch that matches *it, proceed.
    for (int i = 0; i < cursor->branches (); ++i)
    {
      if ((*cursor)[i]->name () == *it)
      {
        cursor = (*cursor)[i];
        found = true;
        break;
      }
    }

    if (!found)
      return NULL;
  }

  return cursor;
}

////////////////////////////////////////////////////////////////////////////////
void Tree::dumpNode (Tree* t, int depth)
{
  // Dump node
  for (int i = 0; i < depth; ++i)
    std::cout << "  ";

  std::cout << t << " \033[1m" << t->name () << "\033[0m";

  // Dump attributes.
  std::string atts;
  std::vector <std::string> attributes = t->allAttributes ();
  std::vector <std::string>::iterator it;
  for (it = attributes.begin (); it != attributes.end (); ++it)
  {
    if (it != attributes.begin ())
      atts += " ";

    atts += *it + "='\033[33m" + t->attribute (*it) + "\033[0m'";
  }

  if (atts.length ())
    std::cout << " " << atts;

  // Dump tags.
  std::string tags;
  std::vector <std::string> allTags = t->allTags ();
  for (it = allTags.begin (); it != allTags.end (); ++it)
    tags += (tags.length () ? " " : "") + *it;

  if (tags.length ())
    std::cout << " \033[32m" << tags << "\033[0m";

  std::cout << "\n";

  // Recurse for branches.
  for (int i = 0; i < t->branches (); ++i)
    dumpNode ((*t)[i], depth + 1);
}

////////////////////////////////////////////////////////////////////////////////
void Tree::dump ()
{
  std::cout << "Tree (" << count () << " nodes)\n";
  dumpNode (this, 1);
}

////////////////////////////////////////////////////////////////////////////////

