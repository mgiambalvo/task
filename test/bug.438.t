#! /usr/bin/perl
################################################################################
## taskwarrior - a command line task list manager.
##
## Copyright 2006 - 2011, Paul Beckingham, Federico Hernandez.
## All rights reserved.
##
## This program is free software; you can redistribute it and/or modify it under
## the terms of the GNU General Public License as published by the Free Software
## Foundation; either version 2 of the License, or (at your option) any later
## version.
##
## This program is distributed in the hope that it will be useful, but WITHOUT
## ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
## FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
## details.
##
## You should have received a copy of the GNU General Public License along with
## this program; if not, write to the
##
##     Free Software Foundation, Inc.,
##     51 Franklin Street, Fifth Floor,
##     Boston, MA
##     02110-1301
##     USA
##
################################################################################

use strict;
use warnings;
use Test::More tests => 11;

# Create the rc file.
if (open my $fh, '>', 'bug.rc')
{
  print $fh "data.location=.\n",
            "dateformat=SNHDMY\n",
            "report.foo.columns=entry,start,end,description\n",
            "report.foo.dateformat=SNHDMY\n";
  close $fh;
  ok (-r 'bug.rc', 'Created bug.rc');
}

# Bug #438: Reports sorting by end, start, and entry are ordered incorrectly, if
#           time is included.

# Ensure the two tasks have a 1 second delta in entry.
qx{../src/task rc:bug.rc add older};
diag ("1 second delay");
sleep 1;
qx{../src/task rc:bug.rc add newer};

my $output = qx{../src/task rc:bug.rc rc.report.foo.sort:entry+ foo};
like ($output, qr/older.+newer/ms, 'sort:entry+ -> older newer');

$output = qx{../src/task rc:bug.rc rc.report.foo.sort:entry- foo};
like ($output, qr/newer.+older/ms, 'sort:entry- -> newer older');

# Ensure the two tasks have a 1 second delta in start.
qx{../src/task rc:bug.rc start 1};
diag ("1 second delay");
sleep 1;
qx{../src/task rc:bug.rc start 2};

$output = qx{../src/task rc:bug.rc rc.report.foo.sort:start+ foo};
like ($output, qr/older.+newer/ms, 'sort:start+ -> older newer');

$output = qx{../src/task rc:bug.rc rc.report.foo.sort:start- foo};
like ($output, qr/newer.+older/ms, 'sort:start- -> newer older');

# Ensure the two tasks have a 1 second delta in end.
qx{../src/task rc:bug.rc done 1};
diag ("1 second delay");
sleep 1;
qx{../src/task rc:bug.rc done 2};

$output = qx{../src/task rc:bug.rc rc.report.foo.sort:end+ foo};
like ($output, qr/older.+newer/ms, 'sort:end+ -> older newer');

$output = qx{../src/task rc:bug.rc rc.report.foo.sort:end- foo};
like ($output, qr/newer.+older/ms, 'sort:end- -> newer older');

# Cleanup.
unlink 'pending.data';
ok (!-r 'pending.data', 'Removed pending.data');

unlink 'completed.data';
ok (!-r 'completed.data', 'Removed completed.data');

unlink 'undo.data';
ok (!-r 'undo.data', 'Removed undo.data');

unlink 'bug.rc';
ok (!-r 'bug.rc', 'Removed bug.rc');

exit 0;

