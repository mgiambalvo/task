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

#include <algorithm>
#include <Cmd.h>
#include <Context.h>
#include <util.h>
#include <text.h>
#include <i18n.h>
#include <main.h>

extern Context context;

////////////////////////////////////////////////////////////////////////////////
Cmd::Cmd ()
: command ("")
{
}

////////////////////////////////////////////////////////////////////////////////
Cmd::Cmd (const std::string& input)
{
  parse (input);
}

////////////////////////////////////////////////////////////////////////////////
Cmd::~Cmd ()
{
}

////////////////////////////////////////////////////////////////////////////////
// Determines whether the string represents a unique command name or custom
// report name.
//
// To be a valid command:
//   1. 'input' should autocomplete to one of 'commands'.
bool Cmd::valid (const std::string& input)
{
  load ();

  std::vector <std::string> matches;
  autoComplete (lowerCase (input), commands, matches);
  if (matches.size () == 1)
    return true;

  return false;
}

////////////////////////////////////////////////////////////////////////////////
// Determines whether the string represents a valid custom report name.
//
// To be a valid custom command:
//   1. 'input' should autocomplete to one of 'commands'.
//   2. the result, canonicalized, should autocomplete to one of
//      'customreports'.
bool Cmd::validCustom (const std::string& input)
{
  load ();

  std::vector <std::string> matches;
  autoComplete (lowerCase (input), commands, matches);
  if (matches.size () == 1)
  {
    std::string canonical = context.canonicalize (matches[0]);
    matches.clear ();
    autoComplete (canonical, customReports, matches);
    if (matches.size () == 1)
      return true;
  }

  return false;
}

////////////////////////////////////////////////////////////////////////////////
// To be a valid custom command:
//   1. 'input' should autocomplete to one of 'commands'.
//   2. the result may then canonicalize to another command.
void Cmd::parse (const std::string& input)
{
  load ();

  std::vector <std::string> matches;
  autoComplete (input, commands, matches);
  if (1 == matches.size ())
    command = context.canonicalize (matches[0]);

  else if (0 == matches.size ())
    command = "";

  else
  {
    std::string error = "Ambiguous command '" + input + "' - could be either of "; // TODO i18n

    std::sort (matches.begin (), matches.end ());
    std::string combined;
    join (combined, ", ", matches);
    throw error + combined;
  }
}

////////////////////////////////////////////////////////////////////////////////
void Cmd::load ()
{
  if (commands.size () == 0)
  {
    // Commands whose names are not localized.
    commands.push_back ("_projects");
    commands.push_back ("_tags");
    commands.push_back ("_commands");
    commands.push_back ("_ids");
    commands.push_back ("_config");
    commands.push_back ("_version");
    commands.push_back ("_urgency");
    commands.push_back ("_query");
    commands.push_back ("_zshcommands");
    commands.push_back ("_zshids");
    commands.push_back ("export.csv");
    commands.push_back ("export.ical");
    commands.push_back ("export.yaml");
    commands.push_back ("history.monthly");
    commands.push_back ("history.annual");
    commands.push_back ("ghistory.monthly");
    commands.push_back ("ghistory.annual");
    commands.push_back ("burndown.daily");
    commands.push_back ("burndown.weekly");
    commands.push_back ("burndown.monthly");
    commands.push_back ("count");

    // Commands whose names are localized.
    commands.push_back (context.stringtable.get (CMD_ADD,         "add"));
    commands.push_back (context.stringtable.get (CMD_APPEND,      "append"));
    commands.push_back (context.stringtable.get (CMD_ANNOTATE,    "annotate"));
    commands.push_back (context.stringtable.get (CMD_DENOTATE,    "denotate"));
    commands.push_back (context.stringtable.get (CMD_CALENDAR,    "calendar"));
    commands.push_back (context.stringtable.get (CMD_COLORS,      "colors"));
    commands.push_back (context.stringtable.get (CMD_CONFIG,      "config"));
    commands.push_back (context.stringtable.get (CMD_SHOW,        "show"));
    commands.push_back (context.stringtable.get (CMD_DELETE,      "delete"));
    commands.push_back (context.stringtable.get (CMD_DIAGNOSTICS, "diagnostics"));
    commands.push_back (context.stringtable.get (CMD_DONE,        "done"));
    commands.push_back (context.stringtable.get (CMD_DUPLICATE,   "duplicate"));
    commands.push_back (context.stringtable.get (CMD_EDIT,        "edit"));
    commands.push_back (context.stringtable.get (CMD_HELP,        "help"));
    commands.push_back (context.stringtable.get (CMD_IMPORT,      "import"));
    commands.push_back (context.stringtable.get (CMD_INFO,        "info"));
    commands.push_back (context.stringtable.get (CMD_LOG,         "log"));
    commands.push_back (context.stringtable.get (CMD_PREPEND,     "prepend"));
    commands.push_back (context.stringtable.get (CMD_PROJECTS,    "projects"));
#ifdef FEATURE_SHELL
    commands.push_back (context.stringtable.get (CMD_SHELL,       "shell"));
#endif
    commands.push_back (context.stringtable.get (CMD_START,       "start"));
    commands.push_back (context.stringtable.get (CMD_STATS,       "stats"));
    commands.push_back (context.stringtable.get (CMD_STOP,        "stop"));
    commands.push_back (context.stringtable.get (CMD_SUMMARY,     "summary"));
    commands.push_back (context.stringtable.get (CMD_TAGS,        "tags"));
    commands.push_back (context.stringtable.get (CMD_TIMESHEET,   "timesheet"));
    commands.push_back (context.stringtable.get (CMD_UNDO,        "undo"));
    commands.push_back (context.stringtable.get (CMD_VERSION,     "version"));
    commands.push_back (context.stringtable.get (CMD_MERGE,       "merge"));
    commands.push_back (context.stringtable.get (CMD_PUSH,        "push"));
    commands.push_back (context.stringtable.get (CMD_PULL,        "pull"));

    // Now load the custom reports.
    std::vector <std::string> all;
    context.config.all (all);

    foreach (i, all)
    {
      if (i->substr (0, 7) == "report.")
      {
        std::string report = i->substr (7);
        std::string::size_type columns = report.find (".columns");
        if (columns != std::string::npos)
        {
          report = report.substr (0, columns);

          // Make sure a custom report does not clash with a built-in
          // command.
          if (std::find (commands.begin (), commands.end (), report) != commands.end ())
            throw std::string ("Custom report '") + report +
                  "' conflicts with built-in task command.";

          // A custom report is also a command.
          customReports.push_back (report);
          commands.push_back (report);
        }
      }
    }

    // Now load the aliases.
    foreach (i, context.aliases)
      commands.push_back (i->first);
  }
}

////////////////////////////////////////////////////////////////////////////////
void Cmd::allCustomReports (std::vector <std::string>& all) const
{
  all = customReports;
}

////////////////////////////////////////////////////////////////////////////////
void Cmd::allCommands (std::vector <std::string>& all) const
{
  all.clear ();
  foreach (command, commands)
    if (command->substr (0, 1) != "_")
      all.push_back (*command);
}

////////////////////////////////////////////////////////////////////////////////
// Commands that do not directly modify the data files.
bool Cmd::isReadOnlyCommand ()
{
  if (command == "_projects"                                                 ||
      command == "_tags"                                                     ||
      command == "_commands"                                                 ||
      command == "_ids"                                                      ||
      command == "_config"                                                   ||
      command == "_version"                                                  ||
      command == "_urgency"                                                  ||
      command == "_query"                                                    ||
      command == "_zshcommands"                                              ||
      command == "_zshids"                                                   ||
      command == "export.csv"                                                ||
      command == "export.ical"                                               ||
      command == "export.yaml"                                               ||
      command == "history.monthly"                                           ||
      command == "history.annual"                                            ||
      command == "ghistory.monthly"                                          ||
      command == "ghistory.annual"                                           ||
      command == "burndown.daily"                                            ||
      command == "burndown.weekly"                                           ||
      command == "burndown.monthly"                                          ||
      command == "count"                                                     ||
      command == context.stringtable.get (CMD_CALENDAR,    "calendar")       ||
      command == context.stringtable.get (CMD_COLORS,      "colors")         ||
      command == context.stringtable.get (CMD_DIAGNOSTICS, "diagnostics")    ||
      command == context.stringtable.get (CMD_CONFIG,      "config")         ||
      command == context.stringtable.get (CMD_SHOW,        "show")           ||
      command == context.stringtable.get (CMD_HELP,        "help")           ||
      command == context.stringtable.get (CMD_INFO,        "info")           ||
      command == context.stringtable.get (CMD_PROJECTS,    "projects")       ||
			command == context.stringtable.get (CMD_PUSH,        "push")           ||
      command == context.stringtable.get (CMD_SHELL,       "shell")          ||
      command == context.stringtable.get (CMD_STATS,       "stats")          ||
      command == context.stringtable.get (CMD_SUMMARY,     "summary")        ||
      command == context.stringtable.get (CMD_TAGS,        "tags")           ||
      command == context.stringtable.get (CMD_TIMESHEET,   "timesheet")      ||
      command == context.stringtable.get (CMD_VERSION,     "version")        ||
      validCustom (command))
    return true;

  return false;
}

////////////////////////////////////////////////////////////////////////////////
// Commands that directly modify the data files.
bool Cmd::isWriteCommand ()
{
  if (command == context.stringtable.get (CMD_MERGE,     "merge")     ||
      command == context.stringtable.get (CMD_ADD,       "add")       ||
      command == context.stringtable.get (CMD_APPEND,    "append")    ||
      command == context.stringtable.get (CMD_ANNOTATE,  "annotate")  ||
      command == context.stringtable.get (CMD_DENOTATE,  "denotate")  ||
      command == context.stringtable.get (CMD_DELETE,    "delete")    ||
      command == context.stringtable.get (CMD_DONE,      "done")      ||
      command == context.stringtable.get (CMD_DUPLICATE, "duplicate") ||
      command == context.stringtable.get (CMD_EDIT,      "edit")      ||
      command == context.stringtable.get (CMD_IMPORT,    "import")    ||
      command == context.stringtable.get (CMD_LOG,       "log")       ||
      command == context.stringtable.get (CMD_PREPEND,   "prepend")   ||
      command == context.stringtable.get (CMD_PULL,      "pull")      ||
      command == context.stringtable.get (CMD_START,     "start")     ||
      command == context.stringtable.get (CMD_STOP,      "stop")      ||
      command == context.stringtable.get (CMD_UNDO,      "undo"))
    return true;

  return false;
}

////////////////////////////////////////////////////////////////////////////////
