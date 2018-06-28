#!/usr/bin/env python

# Written in 2018 by Lynn Kirby <lynn@arrak.is>
#
# To the extent possible under law, the author has dedicated all copyright and
# related and neighboring rights to this software to the public domain worldwide.
# This software is distributed without any warranty.
#
# You should have received a copy of the CC0 Public Domain Dedication along with
# this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

import ctypes
import os
from tempfile import TemporaryDirectory
from subprocess import run, check_output

this_dir = os.path.dirname(__file__)
helper_file = os.path.abspath(os.path.join(this_dir, 'helper.c'))
out_file = os.path.join(this_dir, '../../include/lpd_win2errno.h')
template = os.path.join(this_dir, 'win2errno.template.h')

# Must be VS 2013.
# Later versions removed `_dosmaperr()` and friends from the C runtime.
command = 'call "C:/Program Files (x86)/Microsoft Visual Studio 12.0/VC/vcvarsall.bat"'
command += ' && cl ' + helper_file

with TemporaryDirectory() as build_dir:
    run(command, shell=True, cwd=build_dir, check=True)

    helper = os.path.join(build_dir, 'helper.exe')
    result = check_output(helper, encoding='utf8')

    with open(template) as temp:
        with open(out_file, 'w') as out:
            for line in temp:
                if line.startswith('// INSERT HERE'):
                    out.write(result)
                else:
                    out.write(line)
