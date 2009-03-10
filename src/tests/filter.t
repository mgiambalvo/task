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
use Test::More tests => 108;

# Create the rc file.
if (open my $fh, '>', 'filter.rc')
{
  print $fh "data.location=.\n";
  close $fh;
  ok (-r 'filter.rc', 'Created filter.rc');
}

# Test the filters.
qx{../task rc:filter.rc add project:A priority:H +tag one foo};
qx{../task rc:filter.rc add project:A priority:H      two};
qx{../task rc:filter.rc add project:A                 three};
qx{../task rc:filter.rc add           priority:H      four};
qx{../task rc:filter.rc add                      +tag five};
qx{../task rc:filter.rc add                           six foo};
qx{../task rc:filter.rc add           priority:L      seven bar foo};

my $output = qx{../task rc:filter.rc list};
like   ($output, qr/one/,   'a1');
like   ($output, qr/two/,   'a2');
like   ($output, qr/three/, 'a3');
like   ($output, qr/four/,  'a4');
like   ($output, qr/five/,  'a5');
like   ($output, qr/six/,   'a6');
like   ($output, qr/seven/, 'a7');

$output = qx{../task rc:filter.rc list project:A};
like   ($output, qr/one/,   'b1');
like   ($output, qr/two/,   'b2');
like   ($output, qr/three/, 'b3');
unlike ($output, qr/four/,  'b4');
unlike ($output, qr/five/,  'b5');
unlike ($output, qr/six/,   'b6');
unlike ($output, qr/seven/, 'b7');

$output = qx{../task rc:filter.rc list priority:H};
like   ($output, qr/one/,   'c1');
like   ($output, qr/two/,   'c2');
unlike ($output, qr/three/, 'c3');
like   ($output, qr/four/,  'c4');
unlike ($output, qr/five/,  'c5');
unlike ($output, qr/six/,   'c6');
unlike ($output, qr/seven/, 'c7');

$output = qx{../task rc:filter.rc list priority:};
unlike ($output, qr/one/,   'd1');
unlike ($output, qr/two/,   'd2');
like   ($output, qr/three/, 'd3');
unlike ($output, qr/four/,  'd4');
like   ($output, qr/five/,  'd5');
like   ($output, qr/six/,   'd6');
unlike ($output, qr/seven/, 'd7');

$output = qx{../task rc:filter.rc list foo};
like   ($output, qr/one/,   'e1');
unlike ($output, qr/two/,   'e2');
unlike ($output, qr/three/, 'e3');
unlike ($output, qr/four/,  'e4');
unlike ($output, qr/five/,  'e5');
like   ($output, qr/six/,   'e6');
like   ($output, qr/seven/, 'e7');

$output = qx{../task rc:filter.rc list foo bar};
unlike ($output, qr/one/,   'f1');
unlike ($output, qr/two/,   'f2');
unlike ($output, qr/three/, 'f3');
unlike ($output, qr/four/,  'f4');
unlike ($output, qr/five/,  'f5');
unlike ($output, qr/six/,   'f6');
like   ($output, qr/seven/, 'f7');

$output = qx{../task rc:filter.rc list +tag};
like   ($output, qr/one/,   'g1');
unlike ($output, qr/two/,   'g2');
unlike ($output, qr/three/, 'g3');
unlike ($output, qr/four/,  'g4');
like   ($output, qr/five/,  'g5');
unlike ($output, qr/six/,   'g6');
unlike ($output, qr/seven/, 'g7');

$output = qx{../task rc:filter.rc list project:A priority:H};
like   ($output, qr/one/,   'h1');
like   ($output, qr/two/,   'h2');
unlike ($output, qr/three/, 'h3');
unlike ($output, qr/four/,  'h4');
unlike ($output, qr/five/,  'h5');
unlike ($output, qr/six/,   'h6');
unlike ($output, qr/seven/, 'h7');

$output = qx{../task rc:filter.rc list project:A priority:};
unlike ($output, qr/one/,   'i1');
unlike ($output, qr/two/,   'i2');
like   ($output, qr/three/, 'i3');
unlike ($output, qr/four/,  'i4');
unlike ($output, qr/five/,  'i5');
unlike ($output, qr/six/,   'i6');
unlike ($output, qr/seven/, 'i7');

$output = qx{../task rc:filter.rc list project:A foo};
like   ($output, qr/one/,   'j1');
unlike ($output, qr/two/,   'j2');
unlike ($output, qr/three/, 'j3');
unlike ($output, qr/four/,  'j4');
unlike ($output, qr/five/,  'j5');
unlike ($output, qr/six/,   'j6');
unlike ($output, qr/seven/, 'j7');

$output = qx{../task rc:filter.rc list project:A +tag};
like   ($output, qr/one/,   'k1');
unlike ($output, qr/two/,   'k2');
unlike ($output, qr/three/, 'k3');
unlike ($output, qr/four/,  'k4');
unlike ($output, qr/five/,  'k5');
unlike ($output, qr/six/,   'k6');
unlike ($output, qr/seven/, 'k7');

$output = qx{../task rc:filter.rc list project:A priority:H foo};
like   ($output, qr/one/,   'l1');
unlike ($output, qr/two/,   'l2');
unlike ($output, qr/three/, 'l3');
unlike ($output, qr/four/,  'l4');
unlike ($output, qr/five/,  'l5');
unlike ($output, qr/six/,   'l6');
unlike ($output, qr/seven/, 'l7');

$output = qx{../task rc:filter.rc list project:A priority:H +tag};
like   ($output, qr/one/,   'm1');
unlike ($output, qr/two/,   'm2');
unlike ($output, qr/three/, 'm3');
unlike ($output, qr/four/,  'm4');
unlike ($output, qr/five/,  'm5');
unlike ($output, qr/six/,   'm6');
unlike ($output, qr/seven/, 'm7');

$output = qx{../task rc:filter.rc list project:A priority:H foo +tag};
like   ($output, qr/one/,   'n1');
unlike ($output, qr/two/,   'n2');
unlike ($output, qr/three/, 'n3');
unlike ($output, qr/four/,  'n4');
unlike ($output, qr/five/,  'n5');
unlike ($output, qr/six/,   'n6');
unlike ($output, qr/seven/, 'n7');

$output = qx{../task rc:filter.rc list project:A priority:H foo +tag baz};
unlike ($output, qr/one/,   'n1');
unlike ($output, qr/two/,   'n2');
unlike ($output, qr/three/, 'n3');
unlike ($output, qr/four/,  'n4');
unlike ($output, qr/five/,  'n5');
unlike ($output, qr/six/,   'n6');
unlike ($output, qr/seven/, 'n7');

# Cleanup.
unlink 'pending.data';
ok (!-r 'pending.data', 'Removed pending.data');

unlink 'filter.rc';
ok (!-r 'filter.rc', 'Removed filter.rc');

exit 0;
