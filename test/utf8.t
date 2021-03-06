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
use Test::More tests => 8;

# Create the rc file.
if (open my $fh, '>', 'utf8.rc')
{
  print $fh "data.location=.\n";
  close $fh;
  ok (-r 'utf8.rc', 'Created utf8.rc');
}

# Add a task with UTF8 in the description.
qx{../src/task rc:utf8.rc add Çirçös};
qx{../src/task rc:utf8.rc add Hello world ☺};
qx{../src/task rc:utf8.rc add ¥£€\$¢₡₢₣₤₥₦₧₨₩₪₫₭₮₯};
qx{../src/task rc:utf8.rc add Pchnąć w tę łódź jeża lub ośm skrzyń fig};
qx{../src/task rc:utf8.rc add ๏ เป็นมนุษย์สุดประเสริฐเลิศคุณค่า};
qx{../src/task rc:utf8.rc add イロハニホヘト チリヌルヲ ワカヨタレソ ツネナラム};
qx{../src/task rc:utf8.rc add いろはにほへとちりぬるを};
qx{../src/task rc:utf8.rc add D\\'fhuascail Íosa, Úrmhac na hÓighe Beannaithe, pór Éava agus Ádhaimh};
qx{../src/task rc:utf8.rc add Árvíztűrő tükörfúrógép};
qx{../src/task rc:utf8.rc add Kæmi ný öxi hér ykist þjófum nú bæði víl og ádrepa};
qx{../src/task rc:utf8.rc add Sævör grét áðan því úlpan var ónýt};
qx{../src/task rc:utf8.rc add Quizdeltagerne spiste jordbær med fløde, mens cirkusklovnen Wolther spillede på xylofon.};
qx{../src/task rc:utf8.rc add Falsches Üben von Xylophonmusik quält jeden größeren Zwerg};
qx{../src/task rc:utf8.rc add Zwölf Boxkämpfer jagten Eva quer über den Sylter Deich};
qx{../src/task rc:utf8.rc add Heizölrückstoßabdämpfung};
qx{../src/task rc:utf8.rc add Γαζέες καὶ μυρτιὲς δὲν θὰ βρῶ πιὰ στὸ χρυσαφὶ ξέφωτο};
qx{../src/task rc:utf8.rc add Ξεσκεπάζω τὴν ψυχοφθόρα βδελυγμία};

my $output = qx{../src/task rc:utf8.rc ls};
diag ($output);
like ($output, qr/17/, 'all 17 tasks shown');

qx{../src/task rc:utf8.rc add project:Çirçös utf8 in project};
$output = qx{../src/task rc:utf8.rc ls project:Çirçös};
like ($output, qr/Çirçös.+utf8 in project/, 'utf8 in project works');

qx{../src/task rc:utf8.rc add utf8 in tag +Zwölf};
$output = qx{../src/task rc:utf8.rc ls +Zwölf};
like ($output, qr/utf8 in tag/, 'utf8 in tag works');

# Cleanup.
unlink 'pending.data';
ok (!-r 'pending.data', 'Removed pending.data');

unlink 'completed.data';
ok (!-r 'completed.data', 'Removed completed.data');

unlink 'undo.data';
ok (!-r 'undo.data', 'Removed undo.data');

unlink 'utf8.rc';
ok (!-r 'utf8.rc', 'Removed utf8.rc');

exit 0;

