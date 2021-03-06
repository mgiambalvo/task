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
#include <iostream>
#include "main.h"
#include "text.h"
#include "test.h"

Context context;

////////////////////////////////////////////////////////////////////////////////
int main (int argc, char** argv)
{
  UnitTest t (208);

  // void wrapText (std::vector <std::string>& lines, const std::string& text, const int width)
  std::string text = "This is a test of the line wrapping code.";
  std::vector <std::string> lines;
  wrapText (lines, text, 10);
  t.is (lines.size (), (size_t) 5, "wrapText 'This is a test of the line wrapping code.' -> total 5 lines");
  t.is (lines[0], "This is a",     "wrapText line 0 -> 'This is a'");
  t.is (lines[1], "test of",       "wrapText line 1 -> 'test of'");
  t.is (lines[2], "the line",      "wrapText line 2 -> 'the line'");
  t.is (lines[3], "wrapping",      "wrapText line 3 -> 'wrapping'");
  t.is (lines[4], "code.",         "wrapText line 4 -> 'code.'");

#ifdef NOPE
  // void wrapText (std::vector <std::string>& lines, const std::string& text, const int width)
  text = "This ☺ is a test of utf8 line extraction.";
  lines.clear ();
  wrapText (lines, text, 7);
  t.is (lines.size (), (size_t) 7, "wrapText 'This ☺ is a test of utf8 line extraction.' -> total 7 lines");
  t.is (lines[0], "This ☺",     "wrapText line 0 -> 'This ☺'");
  t.is (lines[1], "is a",          "wrapText line 1 -> 'is a'");
  t.is (lines[2], "test of",       "wrapText line 2 -> 'test of'");
  t.is (lines[3], "utf8",          "wrapText line 3 -> 'utf8'");
  t.is (lines[4], "line",          "wrapText line 4 -> 'line'");
  t.is (lines[5], "extrac-",       "wrapText line 5 -> 'extrac-'");
  t.is (lines[6], "tion.",         "wrapText line 6 -> 'tion.'");

  // void extractLine (std::string& text, std::string& line, int length)
  text = "This ☺ is a test of utf8 line extraction.";
#endif
  std::string line;
#ifdef NOPE
  extractLine (text, line, 7);
  t.is (line, "line 1", "extractLine 7 'This ☺ is a test of utf8 line extraction.' -> 'This ☺'");
#endif

  // void extractLine (std::string& text, std::string& line, int length)
  text = "line 1\nlengthy second line that exceeds width";
  extractLine (text, line, 10);
  t.is (line, "line 1", "extractLine 10 'line 1\\nlengthy second line that exceeds width' -> 'line 1'");

  extractLine (text, line, 10);
  t.is (line, "lengthy", "extractLine 10 'lengthy second line that exceeds width' -> 'lengthy'");

  extractLine (text, line, 10);
  t.is (line, "second", "extractLine 10 'second line that exceeds width' -> 'second'");

  extractLine (text, line, 10);
  t.is (line, "line that", "extractLine 10 'line that exceeds width' -> 'line that'");

  extractLine (text, line, 10);
  t.is (line, "exceeds", "extractLine 10 'exceeds width' -> 'exceeds'");

  extractLine (text, line, 10);
  t.is (line, "width", "extractLine 10 'width' -> 'width'");

  extractLine (text, line, 10);
  t.is (line, "", "extractLine 10 '' -> ''");

  // void split (std::vector<std::string>& results, const std::string& input, const char delimiter)
  std::vector <std::string> items;
  std::string unsplit = "";
  split (items, unsplit, '-');
  t.is (items.size (), (size_t) 0, "split '' '-' -> 0 items");

  unsplit = "a";
  split (items, unsplit, '-');
  t.is (items.size (), (size_t) 1, "split 'a' '-' -> 1 item");
  t.is (items[0], "a",             "split 'a' '-' -> 'a'");

  split (items, unsplit, '-');
  t.is (items.size (), (size_t) 1, "split 'a' '-' -> 1 item");
  t.is (items[0], "a",             "split 'a' '-' -> 'a'");

  unsplit = "-";
  split (items, unsplit, '-');
  t.is (items.size (), (size_t) 2, "split '-' '-' -> '' ''");
  t.is (items[0], "",              "split '-' '-' -> [0] ''");
  t.is (items[1], "",              "split '-' '-' -> [1] ''");

  split_minimal (items, unsplit, '-');
  t.is (items.size (), (size_t) 1, "split '-' '-' -> '-'");

  unsplit = "-a-bc-def";
  split (items, unsplit, '-');
  t.is (items.size (), (size_t) 4, "split '-a-bc-def' '-' -> '' 'a' 'bc' 'def'");
  t.is (items[0], "",              "split '-a-bc-def' '-' -> [0] ''");
  t.is (items[1], "a",             "split '-a-bc-def' '-' -> [1] 'a'");
  t.is (items[2], "bc",            "split '-a-bc-def' '-' -> [2] 'bc'");
  t.is (items[3], "def",           "split '-a-bc-def' '-' -> [3] 'def'");

  split_minimal (items, unsplit, '-');
  t.is (items.size (), (size_t) 3, "split '-a-bc-def' '-' -> 'a' 'bc' 'def'");
  t.is (items[0], "a",             "split '-a-bc-def' '-' -> [1] 'a'");
  t.is (items[1], "bc",            "split '-a-bc-def' '-' -> [2] 'bc'");
  t.is (items[2], "def",           "split '-a-bc-def' '-' -> [3] 'def'");

  // void split (std::vector<std::string>& results, const std::string& input, const std::string& delimiter)
  unsplit = "";
  split (items, unsplit, "--");
  t.is (items.size (), (size_t) 0, "split '' '--' -> 0 items");

  split_minimal (items, unsplit, "--");
  t.is (items.size (), (size_t) 0, "split '' '--' -> 0");

  unsplit = "a";
  split (items, unsplit, "--");
  t.is (items.size (), (size_t) 1, "split 'a' '--' -> 1 item");
  t.is (items[0], "a",             "split 'a' '-' -> 'a'");

  unsplit = "--";
  split (items, unsplit, "--");
  t.is (items.size (), (size_t) 2, "split '-' '--' -> '' ''");
  t.is (items[0], "",              "split '-' '-' -> [0] ''");
  t.is (items[1], "",              "split '-' '-' -> [1] ''");

  unsplit = "--a--bc--def";
  split (items, unsplit, "--");
  t.is (items.size (), (size_t) 4, "split '-a-bc-def' '--' -> '' 'a' 'bc' 'def'");
  t.is (items[0], "",              "split '-a-bc-def' '--' -> [0] ''");
  t.is (items[1], "a",             "split '-a-bc-def' '--' -> [1] 'a'");
  t.is (items[2], "bc",            "split '-a-bc-def' '--' -> [2] 'bc'");
  t.is (items[3], "def",           "split '-a-bc-def' '--' -> [3] 'def'");

  // void join (std::string& result, const std::string& separator, const std::vector<std::string>& items)
  std::vector <std::string> unjoined;
  std::string joined;

  join (joined, "", unjoined);
  t.is (joined.length (), (size_t) 0,  "join -> length 0");
  t.is (joined,           "",          "join -> ''");

  unjoined.push_back ("");
  unjoined.push_back ("a");
  unjoined.push_back ("bc");
  unjoined.push_back ("def");
  join (joined, "", unjoined);
  t.is (joined.length (), (size_t) 6, "join '' 'a' 'bc' 'def' -> length 6");
  t.is (joined,           "abcdef",   "join '' 'a' 'bc' 'def' -> 'abcdef'");

  join (joined, "-", unjoined);
  t.is (joined.length (), (size_t) 9,  "join '' - 'a' - 'bc' - 'def' -> length 9");
  t.is (joined,           "-a-bc-def", "join '' - 'a' - 'bc' - 'def' -> '-a-bc-def'");

  // void join (std::string& result, const std::string& separator, const std::vector<int>& items)
  std::vector <int> unjoined2;

  join (joined, "", unjoined2);
  t.is (joined.length (), (size_t) 0, "join -> length 0");
  t.is (joined,           "",         "join -> ''");

  unjoined2.push_back (0);
  unjoined2.push_back (1);
  unjoined2.push_back (2);
  join (joined, "", unjoined2);
  t.is (joined.length (), (size_t) 3, "join 0 1 2 -> length 3");
  t.is (joined,           "012",      "join 0 1 2 -> '012'");

  join (joined, "-", unjoined2);
  t.is (joined.length (), (size_t) 5, "join 0 1 2 -> length 5");
  t.is (joined,           "0-1-2",    "join 0 1 2 -> '0-1-2'");

  // std::string trimLeft (const std::string& in, const std::string& t /*= " "*/)
  t.is (trimLeft (""),                     "",            "trimLeft '' -> ''");
  t.is (trimLeft ("",              " \t"), "",            "trimLeft '' -> ''");
  t.is (trimLeft ("xxx"),                  "xxx",         "trimLeft 'xxx' -> 'xxx'");
  t.is (trimLeft ("xxx",           " \t"), "xxx",         "trimLeft 'xxx' -> 'xxx'");
  t.is (trimLeft ("  \t xxx \t  "),        "\t xxx \t  ", "trimLeft '  \\t xxx \\t  ' -> '\\t xxx \\t  '");
  t.is (trimLeft ("  \t xxx \t  ", " \t"), "xxx \t  ",    "trimLeft '  \\t xxx \\t  ' -> 'xxx \\t  '");

  // std::string trimRight (const std::string& in, const std::string& t /*= " "*/)
  t.is (trimRight (""),                     "",            "trimRight '' -> ''");
  t.is (trimRight ("",              " \t"), "",            "trimRight '' -> ''");
  t.is (trimRight ("xxx"),                  "xxx",         "trimRight 'xxx' -> 'xxx'");
  t.is (trimRight ("xxx",           " \t"), "xxx",         "trimRight 'xxx' -> 'xxx'");
  t.is (trimRight ("  \t xxx \t  "),        "  \t xxx \t", "trimRight '  \\t xxx \\t  ' -> '  \\t xxx \\t'");
  t.is (trimRight ("  \t xxx \t  ", " \t"), "  \t xxx",    "trimRight '  \\t xxx \\t  ' -> '  \\t xxx'");

  // std::string trim (const std::string& in, const std::string& t /*= " "*/)
  t.is (trim (""),                     "",          "trim '' -> ''");
  t.is (trim ("",              " \t"), "",          "trim '' -> ''");
  t.is (trim ("xxx"),                  "xxx",       "trim 'xxx' -> 'xxx'");
  t.is (trim ("xxx",           " \t"), "xxx",       "trim 'xxx' -> 'xxx'");
  t.is (trim ("  \t xxx \t  "),        "\t xxx \t", "trim '  \\t xxx \\t  ' -> '\\t xxx \\t'");
  t.is (trim ("  \t xxx \t  ", " \t"), "xxx",       "trim '  \\t xxx \\t  ' -> 'xxx'");

  // std::string unquoteText (const std::string& text)
  t.is (unquoteText (""),         "",     "unquoteText '' -> ''");
  t.is (unquoteText ("x"),        "x",    "unquoteText 'x' -> 'x'");
  t.is (unquoteText ("'x"),       "'x",   "unquoteText ''x' -> ''x'");
  t.is (unquoteText ("x'"),       "x'",   "unquoteText 'x'' -> 'x''");
  t.is (unquoteText ("\"x"),      "\"x",  "unquoteText '\"x' -> '\"x'");
  t.is (unquoteText ("x\""),      "x\"",  "unquoteText 'x\"' -> 'x\"'");
  t.is (unquoteText ("''"),       "",     "unquoteText '''' -> ''");
  t.is (unquoteText ("'''"),      "'",    "unquoteText ''''' -> '''");
  t.is (unquoteText ("\"\""),     "",     "unquoteText '\"\"' -> ''");
  t.is (unquoteText ("\"\"\""),    "\"",  "unquoteText '\"\"\"' -> '\"'");
  t.is (unquoteText ("''''"),     "''",   "unquoteText '''''' -> ''''");
  t.is (unquoteText ("\"\"\"\""), "\"\"", "unquoteText '\"\"\"\"' -> '\"\"'");
  t.is (unquoteText ("'\"\"'"),   "\"\"", "unquoteText '''\"\"' -> '\"\"'");
  t.is (unquoteText ("\"''\""),   "''",   "unquoteText '\"''\"' -> ''''");
  t.is (unquoteText ("'x'"),      "x",    "unquoteText ''x'' -> 'x'");
  t.is (unquoteText ("\"x\""),    "x",    "unquoteText '\"x\"' -> 'x'");

  // std::string commify (const std::string& data)
  t.is (commify (""),           "",              "commify '' -> ''");
  t.is (commify ("1"),          "1",             "commify '1' -> '1'");
  t.is (commify ("12"),         "12",            "commify '12' -> '12'");
  t.is (commify ("123"),        "123",           "commify '123' -> '123'");
  t.is (commify ("1234"),       "1,234",         "commify '1234' -> '1,234'");
  t.is (commify ("12345"),      "12,345",        "commify '12345' -> '12,345'");
  t.is (commify ("123456"),     "123,456",       "commify '123456' -> '123,456'");
  t.is (commify ("1234567"),    "1,234,567",     "commify '1234567' -> '1,234,567'");
  t.is (commify ("12345678"),   "12,345,678",    "commify '12345678' -> '12,345,678'");
  t.is (commify ("123456789"),  "123,456,789",   "commify '123456789' -> '123,456,789'");
  t.is (commify ("1234567890"), "1,234,567,890", "commify '1234567890' -> '1,234,567,890'");

  t.is (commify ("pre"),         "pre",          "commify 'pre' -> 'pre'");
  t.is (commify ("pre1234"),     "pre1,234",     "commify 'pre1234' -> 'pre1,234'");
  t.is (commify ("1234post"),    "1,234post",    "commify '1234post' -> '1,234post'");
  t.is (commify ("pre1234post"), "pre1,234post", "commify 'pre1234post' -> 'pre1,234post'");

  // std::string lowerCase (const std::string& input)
  t.is (lowerCase (""),            "",            "lowerCase '' -> ''");
  t.is (lowerCase ("pre01_:POST"), "pre01_:post", "lowerCase 'pre01_:POST' -> 'pre01_:post'");

  // std::string upperCase (const std::string& input)
  t.is (upperCase (""),            "",            "upperCase '' -> ''");
  t.is (upperCase ("pre01_:POST"), "PRE01_:POST", "upperCase 'pre01_:POST' -> 'PRE01_:POST'");

  // bool digitsOnly (const std::string&);
  t.ok    (digitsOnly (""),                       "digitsOnly '' -> true");
  t.ok    (digitsOnly ("0"),                      "digitsOnly '0' -> true");
  t.ok    (digitsOnly ("123"),                    "digitsOnly '123' -> true");
  t.notok (digitsOnly ("12fa"),                   "digitsOnly '12fa' -> false");

  // bool noSpaces (const std::string&);
  t.ok    (noSpaces (""),                         "noSpaces '' -> true");
  t.ok    (noSpaces ("a"),                        "noSpaces 'a' -> true");
  t.ok    (noSpaces ("abc"),                      "noSpaces 'abc' -> true");
  t.notok (noSpaces (" "),                        "noSpaces ' ' -> false");
  t.notok (noSpaces ("ab cd"),                    "noSpaces 'ab cd' -> false");

  // bool noVerticalSpace (const std::string&);
  t.ok    (noVerticalSpace (""),                  "noVerticalSpace '' -> true");
  t.ok    (noVerticalSpace ("a"),                 "noVerticalSpace 'a' -> true");
  t.ok    (noVerticalSpace ("abc"),               "noVerticalSpace 'abc' -> true");
  t.notok (noVerticalSpace ("a\nb"),              "noVerticalSpace 'a\\nb' -> false");
  t.notok (noVerticalSpace ("a\rb"),              "noVerticalSpace 'a\\rb' -> false");
  t.notok (noVerticalSpace ("a\fb"),              "noVerticalSpace 'a\\fb' -> false");

  text = "Hello, world.";
  //      0123456789012
  //      s   e  s   e

  // bool isWordStart (const std::string&, std::string::size_type);
  t.notok (isWordStart ("", 0),                   "isWordStart (\"\", 0) -> false");
  t.ok    (isWordStart ("foo", 0),                "isWordStart (\"foo\", 0) -> true");
  t.ok    (isWordStart (text, 0),                 "isWordStart (\"Hello, world.\", 0) -> true");
  t.notok (isWordStart (text, 1),                 "isWordStart (\"Hello, world.\", 1) -> false");
  t.notok (isWordStart (text, 2),                 "isWordStart (\"Hello, world.\", 2) -> false");
  t.notok (isWordStart (text, 3),                 "isWordStart (\"Hello, world.\", 3) -> false");
  t.notok (isWordStart (text, 4),                 "isWordStart (\"Hello, world.\", 4) -> false");
  t.notok (isWordStart (text, 5),                 "isWordStart (\"Hello, world.\", 5) -> false");
  t.notok (isWordStart (text, 6),                 "isWordStart (\"Hello, world.\", 6) -> false");
  t.ok    (isWordStart (text, 7),                 "isWordStart (\"Hello, world.\", 7) -> true");
  t.notok (isWordStart (text, 8),                 "isWordStart (\"Hello, world.\", 8) -> false");
  t.notok (isWordStart (text, 9),                 "isWordStart (\"Hello, world.\", 9) -> false");
  t.notok (isWordStart (text, 10),                "isWordStart (\"Hello, world.\", 10) -> false");
  t.notok (isWordStart (text, 11),                "isWordStart (\"Hello, world.\", 11) -> false");
  t.notok (isWordStart (text, 12),                "isWordStart (\"Hello, world.\", 12) -> false");

  // bool isWordEnd (const std::string&, std::string::size_type);
  t.notok (isWordEnd ("", 0),                     "isWordEnd (\"\", 0) -> false");
  t.ok    (isWordEnd ("foo", 2),                  "isWordEnd (\"foo\", 2) -> true");
  t.notok (isWordEnd (text, 0),                   "isWordEnd (\"Hello, world.\", 0) -> false");
  t.notok (isWordEnd (text, 1),                   "isWordEnd (\"Hello, world.\", 1) -> false");
  t.notok (isWordEnd (text, 2),                   "isWordEnd (\"Hello, world.\", 2) -> false");
  t.notok (isWordEnd (text, 3),                   "isWordEnd (\"Hello, world.\", 3) -> false");
  t.ok    (isWordEnd (text, 4),                   "isWordEnd (\"Hello, world.\", 4) -> true");
  t.notok (isWordEnd (text, 5),                   "isWordEnd (\"Hello, world.\", 5) -> false");
  t.notok (isWordEnd (text, 6),                   "isWordEnd (\"Hello, world.\", 6) -> false");
  t.notok (isWordEnd (text, 7),                   "isWordEnd (\"Hello, world.\", 7) -> false");
  t.notok (isWordEnd (text, 8),                   "isWordEnd (\"Hello, world.\", 8) -> false");
  t.notok (isWordEnd (text, 9),                   "isWordEnd (\"Hello, world.\", 9) -> false");
  t.notok (isWordEnd (text, 10),                  "isWordEnd (\"Hello, world.\", 10) -> false");
  t.ok    (isWordEnd (text, 11),                  "isWordEnd (\"Hello, world.\", 11) -> true");
  t.notok (isWordEnd (text, 12),                  "isWordEnd (\"Hello, world.\", 12) -> false");

  // bool compare (const std::string&, const std::string&, bool caseless = false);
  // Make sure degenerate cases are handled.
  t.ok    (compare ("", ""),    "'' == ''");
  t.notok (compare ("foo", ""), "foo != ''");
  t.notok (compare ("", "foo"), "'' != foo");

  // Make sure the default is case-sensitive.
  t.ok    (compare ("foo", "foo"), "foo == foo");
  t.notok (compare ("foo", "FOO"), "foo != foo");

  // Test case-sensitive.
  t.notok (compare ("foo", "xx", true),  "foo != xx");

  t.ok    (compare ("foo", "foo", true), "foo == foo");
  t.notok (compare ("foo", "FOO", true), "foo != FOO");
  t.notok (compare ("FOO", "foo", true), "FOO != foo");
  t.ok    (compare ("FOO", "FOO", true), "FOO == FOO");

  // Test case-insensitive.
  t.notok (compare ("foo", "xx", false),   "foo != foo (caseless)");

  t.ok    (compare ("foo", "foo", false),  "foo == foo (caseless)");
  t.ok    (compare ("foo", "FOO", false),  "foo == FOO (caseless)");
  t.ok    (compare ("FOO", "foo", false),  "FOO == foo (caseless)");
  t.ok    (compare ("FOO", "FOO", false),  "FOO == FOO (caseless)");

  // std::string::size_type find (const std::string&, const std::string&, bool caseless = false);
  // Make sure degenerate cases are handled.
  t.is ((int) find ("foo", ""), (int) 0,                 "foo !contains ''");
  t.is ((int) find ("", "foo"), (int) std::string::npos, "'' !contains foo");

  // Make sure the default is case-sensitive.
  t.is ((int) find ("foo", "fo"), 0,                       "foo contains fo");
  t.is ((int) find ("foo", "FO"), (int) std::string::npos, "foo !contains fo");

  // Test case-sensitive.
  t.is ((int) find ("foo", "xx", true), (int) std::string::npos, "foo !contains xx");
  t.is ((int) find ("foo", "oo", true), 1,                       "foo contains oo");

  t.is ((int) find ("foo", "fo", true), 0,                       "foo contains fo");
  t.is ((int) find ("foo", "FO", true), (int) std::string::npos, "foo !contains fo");
  t.is ((int) find ("FOO", "fo", true), (int) std::string::npos, "foo !contains fo");
  t.is ((int) find ("FOO", "FO", true), 0,                       "foo contains fo");

  // Test case-insensitive.
  t.is ((int) find ("foo", "xx", false),  (int) std::string::npos, "foo !contains xx (caseless)");
  t.is ((int) find ("foo", "oo", false),  1,                       "foo contains oo (caseless)");

  t.is ((int) find ("foo", "fo", false),  0, "foo contains fo (caseless)");
  t.is ((int) find ("foo", "FO", false),  0, "foo contains FO (caseless)");
  t.is ((int) find ("FOO", "fo", false),  0, "FOO contains fo (caseless)");
  t.is ((int) find ("FOO", "FO", false),  0, "FOO contains FO (caseless)");

  // Test start offset.
  t.is ((int) find ("one two three", "e",  3, true), (int) 11, "offset obeyed");
  t.is ((int) find ("one two three", "e", 11, true), (int) 11, "offset obeyed");

  // int strippedLength (const std::string&);
  t.is (strippedLength (std::string ("")),                                  0, "strippedLength                              -> 0");
  t.is (strippedLength (std::string ("abc")),                               3, "strippedLength abc                          -> 3");
  t.is (strippedLength (std::string ("one\033[5;38;255mtwo\033[0mthree")), 11, "strippedLength one^[[5;38;255mtwo^[[0mthree -> 11");
  t.is (strippedLength (std::string ("\033[0m")),                           0, "strippedLength ^[[0m                        -> 0");
  t.is (strippedLength (std::string ("\033[1m\033[0m")),                    0, "strippedLength ^[[1m^[[0m                   -> 0");

  // int characters (const std::string&);
  t.is (characters ("Çirçös"),            6, "characters (Çirçös) == 6");
  t.is (characters ("ツネナラム"),        5, "characters (ツネナラム) == 5");
  t.is (characters ("Zwölf Boxkämpfer"), 16, "characters (Zwölf Boxkämpfer) == 16");

  // std::string format (char);
  t.is (format ('A'), "A", "format ('A') -> A");

  // std::string format (int);
  t.is (format (0),  "0",  "format (0) -> 0");
  t.is (format (-1), "-1", "format (-1) -> -1");

  // std::string formatHex (int);
  t.is (formatHex (0),   "0",  "formatHex (0) -> 0");
  t.is (formatHex (10),  "a",  "formatHex (10) -> a");
  t.is (formatHex (123), "7b", "formatHex (123) -> 7b");

  // std::string format (float, int, int);
  t.is (format (1.23456789, 8, 1), "       1",  "format (1.23456789, 8, 1) -> _______1");
  t.is (format (1.23456789, 8, 2), "     1.2",  "format (1.23456789, 8, 2) -> _____1.2");
  t.is (format (1.23456789, 8, 3), "    1.23",  "format (1.23456789, 8, 3) -> ____1.23");
  t.is (format (1.23456789, 8, 4), "   1.235",  "format (1.23456789, 8, 4) -> ___1.235");
  t.is (format (1.23456789, 8, 5), "  1.2346",  "format (1.23456789, 8, 5) -> __1.2346");
  t.is (format (1.23456789, 8, 6), " 1.23457",  "format (1.23456789, 8, 6) ->  1.23457");
  t.is (format (1.23456789, 8, 7), "1.234568",  "format (1.23456789, 8, 7) -> 1.234568");
  t.is (format (1.23456789, 8, 8), "1.2345679", "format (1.23456789, 8, 8) -> 1.2345679");

  // std::string format (double, int, int);

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
