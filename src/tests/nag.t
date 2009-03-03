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
use Test::More tests => 9;

# Create the rc file.
if (open my $fh, '>', 'nag.rc')
{
  print $fh "data.location=.\n",
            "nag=NAG\n";
  close $fh;
  ok (-r 'nag.rc', 'Created nag.rc');
}

my $setup = "../task rc:nag.rc add due:yesterday one;"
          . "../task rc:nag.rc add due:tomorrow two;"
          . "../task rc:nag.rc add priority:H three;"
          . "../task rc:nag.rc add priority:M four;"
          . "../task rc:nag.rc add priority:L five;"
          . "../task rc:nag.rc add six;";
qx{$setup};

my $output = qx{../task rc:nag.rc do 6};
like (qx{../task rc:nag.rc do 6}, qr/NAG/, 'do pri: -> nag');
like (qx{../task rc:nag.rc do 5}, qr/NAG/, 'do pri:L -> nag');
like (qx{../task rc:nag.rc do 4}, qr/NAG/, 'do pri:M-> nag');
like (qx{../task rc:nag.rc do 3}, qr/NAG/, 'do pri:H-> nag');
like (qx{../task rc:nag.rc do 2}, qr/NAG/, 'do due:tomorrow -> nag');
ok (qx{../task rc:nag.rc do 1} !~ qr/NAG/, 'do due:yesterday -> no nag');

# Cleanup.
unlink 'pending.data';
ok (!-r 'pendind.data', 'Removed pending.data');

unlink 'nag.rc';
ok (!-r 'nag.rc', 'Removed nag.rc');

exit 0;
