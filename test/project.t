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
use Test::More tests => 13;

# Create the rc file.
if (open my $fh, '>', 'pro.rc')
{
  print $fh "data.location=.\n",
            "confirmation=off\n";
  close $fh;
  ok (-r 'pro.rc', 'Created pro.rc');
}

# Test the project status numbers.
my $output = qx{../src/task rc:pro.rc add one pro:foo};
like ($output, qr/The project 'foo' has changed\.  Project 'foo' is 0% complete \(1 of 1 tasks remaining\)\./, 'add one');

$output = qx{../src/task rc:pro.rc add two pro:'foo'};
like ($output, qr/The project 'foo' has changed\.  Project 'foo' is 0% complete \(2 of 2 tasks remaining\)\./, 'add two');

$output = qx{../src/task rc:pro.rc add three pro:'foo'};
like ($output, qr/The project 'foo' has changed\.  Project 'foo' is 0% complete \(3 of 3 tasks remaining\)\./, 'add three');

$output = qx{../src/task rc:pro.rc add four pro:'foo'};
like ($output, qr/The project 'foo' has changed\.  Project 'foo' is 0% complete \(4 of 4 tasks remaining\)\./, 'add four');

$output = qx{../src/task rc:pro.rc 1 done};
like ($output, qr/Project 'foo' is 25% complete \(3 of 4 tasks remaining\)\./, 'done one');

$output = qx{../src/task rc:pro.rc 2 delete};
like ($output, qr/The project 'foo' has changed\.  Project 'foo' is 33% complete \(2 of 3 tasks remaining\)\./, 'delete two');

$output = qx{../src/task rc:pro.rc 3 pro:bar};
like ($output, qr/The project 'foo' has changed\.  Project 'foo' is 50% complete \(1 of 2 tasks remaining\)\./, 'change project');
like ($output, qr/The project 'bar' has changed\.  Project 'bar' is 0% complete \(1 of 1 tasks remaining\)\./, 'change project');

# Cleanup.
unlink 'pending.data';
ok (!-r 'pending.data', 'Removed pending.data');

unlink 'completed.data';
ok (!-r 'completed.data', 'Removed completed.data');

unlink 'undo.data';
ok (!-r 'undo.data', 'Removed undo.data');

unlink 'pro.rc';
ok (!-r 'pro.rc', 'Removed pro.rc');

exit 0;

