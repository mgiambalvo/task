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
use Test::More tests => 10;

# Create the rc file.
if (open my $fh, '>', 'custom.rc')
{
  print $fh "data.location=.\n",
            "report.foo.description=DESC\n",
            "report.foo.columns=id,tag_indicator\n",
            "report.foo.labels=ID,T\n",
            "report.foo.sort=id+\n";
  close $fh;
  ok (-r 'custom.rc', 'Created custom.rc');
}

# Generate the usage screen, and locate the custom report on it.
qx{../src/task rc:custom.rc add foo +tag};
qx{../src/task rc:custom.rc add bar};
my $output = qx{../src/task rc:custom.rc foo 2>&1};
like ($output,   qr/ID T/,   'Tag indicator heading');
like ($output,   qr/1\s+\+/, 'Tag indicator t1');
unlike ($output, qr/2\s+\+/, 'No tag indicator t2');

$output = qx{../src/task rc:custom.rc foo rc.tag.indicator=TAG 2>&1};
like ($output,   qr/1\s+TAG/, 'Custom ag indicator t1');
unlike ($output, qr/2\s+TAG/, 'No custom tag indicator t2');

# Cleanup.
unlink 'pending.data';
ok (!-r 'pending.data', 'Removed pending.data');

unlink 'completed.data';
ok (!-r 'completed.data', 'Removed completed.data');

unlink 'undo.data';
ok (!-r 'undo.data', 'Removed undo.data');

unlink 'custom.rc';
ok (!-r 'custom.rc', 'Removed custom.rc');

exit 0;

