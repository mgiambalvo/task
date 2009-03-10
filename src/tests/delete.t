#! /usr/bin/perl
################################################################################
## task - a command line task list manager.
##
## Copyright 2006 - 2009, Paul Beckingham.
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
use Test::More tests => 16;

# Create the rc file.
if (open my $fh, '>', 'undelete.rc')
{
  print $fh "data.location=.\n";
  close $fh;
  ok (-r 'undelete.rc', 'Created undelete.rc');
}

# Add a task, delete it, undelete it.
my $output = qx{../task rc:undelete.rc add one; ../task rc:undelete.rc info 1};
ok (-r 'pending.data', 'pending.data created');
like ($output, qr/Status\s+Pending\n/, 'Pending');

$output = qx{../task rc:undelete.rc delete 1; ../task rc:undelete.rc info 1};
like ($output, qr/Status\s+Deleted\n/, 'Deleted');
ok (! -r 'completed.data', 'completed.data not created');

$output = qx{../task rc:undelete.rc undelete 1; ../task rc:undelete.rc info 1};
like ($output, qr/Status\s+Pending\n/, 'Pending');
ok (! -r 'completed.data', 'completed.data not created');

$output = qx{../task rc:undelete.rc delete 1; ../task rc:undelete.rc list};
like ($output, qr/^No matches/, 'No matches');
ok (-r 'completed.data', 'completed.data created');

$output = qx{../task rc:undelete.rc undelete 1};
like ($output, qr/reliably undeleted/, 'can only be reliable undeleted...');

$output = qx{../task rc:undelete.rc info 1};
like ($output, qr/No matches./, 'no matches');

# Cleanup.
ok (-r 'pending.data', 'Need to remove pending.data');
unlink 'pending.data';
ok (!-r 'pending.data', 'Removed pending.data');

ok (-r 'completed.data', 'Need to remove completed.data');
unlink 'completed.data';
ok (!-r 'completed.data', 'Removed completed.data');

unlink 'undelete.rc';
ok (!-r 'undelete.rc', 'Removed undelete.rc');

exit 0;
