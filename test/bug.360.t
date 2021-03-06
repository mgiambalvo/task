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
use Test::More tests => 9;

# Create the rc file.
if (open my $fh, '>', 'bug.rc')
{
  print $fh "data.location=.\n",
            "confirmation=no\n";
  close $fh;
  ok (-r 'bug.rc', 'Created bug.rc');
}

# Setup: Add a recurring task, generate an instance, then add a project.
qx{../src/task rc:bug.rc add foo due:tomorrow recur:daily};
qx{../src/task rc:bug.rc ls};

# Result: trying to add the project generates an error about removing
# recurrence from a task.
my $output = qx{../src/task rc:bug.rc 1 project:bar};
unlike ($output, qr/You cannot remove the recurrence from a recurring task./ms, 'No recurrence removal error');

# Now try to generate the error above via regular means - ie, is it actually
# doing what it should?
$output = qx{../src/task rc:bug.rc 1 recur:};
like ($output, qr/You cannot remove the recurrence from a recurring task./ms, 'Recurrence removal error');

# Prevent removal of the due date from a recurring task.
$output = qx{../src/task rc:bug.rc 1 due:};
like ($output, qr/You cannot remove the due date from a recurring task./ms, 'Cannot remove due date from a recurring task');

# Allow removal of the due date from a non-recurring task.
qx{../src/task rc:bug.rc add nonrecurring};
$output = qx{../src/task rc:bug.rc ls};
my ($id) = $output =~ /(\d+)\s+nonrecurring/;
$output = qx{../src/task rc:bug.rc $id due:};
unlike ($output, qr/You cannot remove the due date from a recurring task./ms, 'Can remove due date from a non-recurring task');

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

