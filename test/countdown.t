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
use Test::More tests => 85;

# Create the rc file.
if (open my $fh, '>', 'countdown.rc')
{
  print $fh "data.location=.\n";
  print $fh "report.up.description=countdown+ report\n";
  print $fh "report.up.columns=id,countdown,description\n";
  print $fh "report.up.labels=ID,Countdown,Description\n";
  print $fh "report.up.filter=status:pending\n";
  print $fh "report.up.sort=countdown+\n";

  print $fh "report.down.description=countdown- report\n";
  print $fh "report.down.columns=id,countdown,description\n";
  print $fh "report.down.labels=ID,Countdown,Description\n";
  print $fh "report.down.filter=status:pending\n";
  print $fh "report.down.sort=countdown-\n";

  print $fh "report.upc.description=countdown_compact+ report\n";
  print $fh "report.upc.columns=id,countdown_compact,description\n";
  print $fh "report.upc.labels=ID,Countdown,Description\n";
  print $fh "report.upc.filter=status:pending\n";
  print $fh "report.upc.sort=countdown_compact+\n";

  print $fh "report.downc.description=countdown_compact- report\n";
  print $fh "report.downc.columns=id,countdown_compact,description\n";
  print $fh "report.downc.labels=ID,Countdown,Description\n";
  print $fh "report.downc.filter=status:pending\n";
  print $fh "report.downc.sort=countdown_compact-\n";

  close $fh;
  ok (-r 'countdown.rc', 'Created countdown.rc');
}

# Create a variety of pending tasks with increasingly higher due dates
# to ensure proper sort order.
qx{../src/task rc:countdown.rc add one       due:-1.2y};
qx{../src/task rc:countdown.rc add two       due:-9mo};
qx{../src/task rc:countdown.rc add three     due:-2mo};
qx{../src/task rc:countdown.rc add four      due:-3wk};
qx{../src/task rc:countdown.rc add five      due:-7d};
qx{../src/task rc:countdown.rc add six       due:-2d};
qx{../src/task rc:countdown.rc add seven     due:-1d};
qx{../src/task rc:countdown.rc add eight     due:-12h};
qx{../src/task rc:countdown.rc add nine      due:-6h};
qx{../src/task rc:countdown.rc add ten       due:-1h};
qx{../src/task rc:countdown.rc add eleven    due:-30s};
qx{../src/task rc:countdown.rc add twelve    due:1h};
qx{../src/task rc:countdown.rc add thirteen  due:6h};
qx{../src/task rc:countdown.rc add fourteen  due:12h};
qx{../src/task rc:countdown.rc add fifteen   due:1d};
qx{../src/task rc:countdown.rc add sixteen   due:2d};
qx{../src/task rc:countdown.rc add seventeen due:7d};
qx{../src/task rc:countdown.rc add eighteen  due:3wk};
qx{../src/task rc:countdown.rc add nineteen  due:2mo};
qx{../src/task rc:countdown.rc add twenty    due:9mo};
qx{../src/task rc:countdown.rc add twentyone due:1.2y};

my $output = qx{../src/task rc:countdown.rc up};
like ($output, qr/\bone\b.+\btwo\b/ms,          'up: one < two');
like ($output, qr/\btwo\b.+\bthree/ms,          'up: two < three');
like ($output, qr/\bthree\b.+\bfour/ms,         'up: three < four');
like ($output, qr/\bfour\b.+\bfive/ms,          'up: four < five');
like ($output, qr/\bfive\b.+\bsix/ms,           'up: five < six');
like ($output, qr/\bsix\b.+\bseven/ms,          'up: six < seven');
like ($output, qr/\bseven\b.+\beight/ms,        'up: seven < eight');
like ($output, qr/\beight\b.+\bnine/ms,         'up: eight < nine');
like ($output, qr/\bnine\b.+\bten/ms,           'up: nine < ten');
like ($output, qr/\bten\b.+\beleven/ms,         'up: ten < eleven');
like ($output, qr/\beleven\b.+\btwelve/ms,      'up: eleven < twelve');
like ($output, qr/\btwelve\b.+\bthirteen/ms,    'up: twelve < thirteen');
like ($output, qr/\bthirteen\b.+\bfourteen/ms,  'up: thirteen < fourteen');
like ($output, qr/\bfourteen\b.+\bfifteen/ms,   'up: fourteen < fifteen');
like ($output, qr/\bfifteen\b.+\bsixteen/ms,    'up: fifteen < sixteen');
like ($output, qr/\bsixteen\b.+\bseventeen/ms,  'up: sixteen < seventeen');
like ($output, qr/\bseventeen\b.+\beighteen/ms, 'up: seventeen < eighteen');
like ($output, qr/\beighteen\b.+\bnineteen/ms,  'up: eighteen < nineteen');
like ($output, qr/\bnineteen\b.+\btwenty/ms,    'up: nineteen < twenty');
like ($output, qr/\btwenty\b.+\btwentyone/ms,   'up: twenty < twentyone');

$output = qx{../src/task rc:countdown.rc down};
like ($output, qr/\btwentyone\b.+\btwenty/ms,   'down: twentyone < twenty');
like ($output, qr/\btwenty\b.+\bnineteen/ms,    'down: twenty < nineteen');
like ($output, qr/\bnineteen\b.+\beighteen/ms,  'down: nineteen < eighteen');
like ($output, qr/\beighteen\b.+\bseventeen/ms, 'down: eighteen < seventeen');
like ($output, qr/\bseventeen\b.+\bsixteen/ms,  'down: seventeen < sixteen');
like ($output, qr/\bsixteen\b.+\bfifteen/ms,    'down: sixteen < fifteen');
like ($output, qr/\bfifteen\b.+\bfourteen/ms,   'down: fifteen < fourteen');
like ($output, qr/\bfourteen\b.+\bthirteen/ms,  'down: fourteen < thirteen');
like ($output, qr/\bthirteen\b.+\btwelve/ms,    'down: thirteen < twelve');
like ($output, qr/\btwelve\b.+\beleven/ms,      'down: twelve < eleven');
like ($output, qr/\beleven\b.+\bten/ms,         'down: eleven < ten');
like ($output, qr/\bten\b.+\bnine/ms,           'down: ten < nine');
like ($output, qr/\bnine\b.+\beight/ms,         'down: nine < eight');
like ($output, qr/\beight\b.+\bseven/ms,        'down: eight < seven');
like ($output, qr/\bseven\b.+\bsix/ms,          'down: seven < six');
like ($output, qr/\bsix\b.+\bfive/ms,           'down: six < five');
like ($output, qr/\bfive\b.+\bfour/ms,          'down: five < four');
like ($output, qr/\bfour\b.+\bthree/ms,         'down: four < three');
like ($output, qr/\bthree\b.+\btwo/ms,          'down: three < two');
like ($output, qr/\btwo\b.+\bone\b/ms,          'down: two < one');

$output = qx{../src/task rc:countdown.rc upc};
like ($output, qr/\bone\b.+\btwo\b/ms,          'upc: one < two');
like ($output, qr/\btwo\b.+\bthree/ms,          'upc: two < three');
like ($output, qr/\bthree\b.+\bfour/ms,         'upc: three < four');
like ($output, qr/\bfour\b.+\bfive/ms,          'upc: four < five');
like ($output, qr/\bfive\b.+\bsix/ms,           'upc: five < six');
like ($output, qr/\bsix\b.+\bseven/ms,          'upc: six < seven');
like ($output, qr/\bseven\b.+\beight/ms,        'upc: seven < eight');
like ($output, qr/\beight\b.+\bnine/ms,         'upc: eight < nine');
like ($output, qr/\bnine\b.+\bten/ms,           'upc: nine < ten');
like ($output, qr/\bten\b.+\beleven/ms,         'upc: ten < eleven');
like ($output, qr/\beleven\b.+\btwelve/ms,      'upc: eleven < twelve');
like ($output, qr/\btwelve\b.+\bthirteen/ms,    'upc: twelve < thirteen');
like ($output, qr/\bthirteen\b.+\bfourteen/ms,  'upc: thirteen < fourteen');
like ($output, qr/\bfourteen\b.+\bfifteen/ms,   'upc: fourteen < fifteen');
like ($output, qr/\bfifteen\b.+\bsixteen/ms,    'upc: fifteen < sixteen');
like ($output, qr/\bsixteen\b.+\bseventeen/ms,  'upc: sixteen < seventeen');
like ($output, qr/\bseventeen\b.+\beighteen/ms, 'upc: seventeen < eighteen');
like ($output, qr/\beighteen\b.+\bnineteen/ms,  'upc: eighteen < nineteen');
like ($output, qr/\bnineteen\b.+\btwenty/ms,    'upc: nineteen < twenty');
like ($output, qr/\btwenty\b.+\btwentyone/ms,   'upc: twenty < twentyone');

$output = qx{../src/task rc:countdown.rc downc};
like ($output, qr/\btwentyone\b.+\btwenty/ms,   'downc: twentyone < twenty');
like ($output, qr/\btwenty\b.+\bnineteen/ms,    'downc: twenty < nineteen');
like ($output, qr/\bnineteen\b.+\beighteen/ms,  'downc: nineteen < eighteen');
like ($output, qr/\beighteen\b.+\bseventeen/ms, 'downc: eighteen < seventeen');
like ($output, qr/\bseventeen\b.+\bsixteen/ms,  'downc: seventeen < sixteen');
like ($output, qr/\bsixteen\b.+\bfifteen/ms,    'downc: sixteen < fifteen');
like ($output, qr/\bfifteen\b.+\bfourteen/ms,   'downc: fifteen < fourteen');
like ($output, qr/\bfourteen\b.+\bthirteen/ms,  'downc: fourteen < thirteen');
like ($output, qr/\bthirteen\b.+\btwelve/ms,    'downc: thirteen < twelve');
like ($output, qr/\btwelve\b.+\beleven/ms,      'downc: twelve < eleven');
like ($output, qr/\beleven\b.+\bten/ms,         'downc: eleven < ten');
like ($output, qr/\bten\b.+\bnine/ms,           'downc: ten < nine');
like ($output, qr/\bnine\b.+\beight/ms,         'downc: nine < eight');
like ($output, qr/\beight\b.+\bseven/ms,        'downc: eight < seven');
like ($output, qr/\bseven\b.+\bsix/ms,          'downc: seven < six');
like ($output, qr/\bsix\b.+\bfive/ms,           'downc: six < five');
like ($output, qr/\bfive\b.+\bfour/ms,          'downc: five < four');
like ($output, qr/\bfour\b.+\bthree/ms,         'downc: four < three');
like ($output, qr/\bthree\b.+\btwo/ms,          'downc: three < two');
like ($output, qr/\btwo\b.+\bone\b/ms,          'downc: two < one');

# Cleanup.
unlink 'pending.data';
ok (!-r 'pending.data', 'Removed pending.data');

unlink 'completed.data';
ok (!-r 'completed.data', 'Removed completed.data');

unlink 'undo.data';
ok (!-r 'undo.data', 'Removed undo.data');

unlink 'countdown.rc';
ok (!-r 'countdown.rc', 'Removed countdown.rc');

exit 0;
