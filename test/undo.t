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
use Test::More tests => 15;

# Create the rc file.
if (open my $fh, '>', 'undo.rc')
{
  print $fh "data.location=.\n",
            "echo.command=no\n";
  close $fh;
  ok (-r 'undo.rc', 'Created undo.rc');
}

# Test the add/do/undo commands.
my $output = qx{../src/task rc:undo.rc add one; ../src/task rc:undo.rc info 1};
ok (-r 'pending.data', 'pending.data created');
like ($output, qr/Status\s+Pending\n/, 'Pending');

$output = qx{../src/task rc:undo.rc do 1; ../src/task rc:undo.rc info 1};
ok (-r 'completed.data', 'completed.data created');
like ($output, qr/Status\s+Completed\n/, 'Completed');

$output = qx{echo 'y'|../src/task rc:undo.rc undo; ../src/task rc:undo.rc info 1};
ok (-r 'completed.data', 'completed.data created');
like ($output, qr/Status\s+Pending\n/, 'Pending');

$output = qx{../src/task rc:undo.rc do 1; ../src/task rc:undo.rc list};
like ($output, qr/No matches/, 'No matches');

# Cleanup.
ok (-r 'pending.data', 'Need to remove pending.data');
unlink 'pending.data';
ok (!-r 'pending.data', 'Removed pending.data');

ok (-r 'completed.data', 'Need to remove completed.data');
unlink 'completed.data';
ok (!-r 'completed.data', 'Removed completed.data');

ok (-r 'undo.data', 'Need to remove undo.data');
unlink 'undo.data';
ok (!-r 'undo.data', 'Removed undo.data');

unlink 'undo.rc';
ok (!-r 'undo.rc', 'Removed undo.rc');

exit 0;

