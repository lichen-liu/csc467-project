# Must be executed in compiler467/

import difflib
import os
import subprocess

compiler467_exe = './compiler467'
test_dir = './tests/lexer/'
prev_dir = test_dir + 'prev/'

print "Rebuild with #define TEST_SCANNER:"
p = subprocess.Popen(['make', 'clean'], stdout = subprocess.PIPE, stderr = subprocess.PIPE)
make_clean_out, make_clean_err = p.communicate()
if make_clean_err:
    print make_clean_out
    print make_clean_err
    raise Exception("make clean failed")
else:
    print "    clean."
# CFLAGS copied from Makefile
p = subprocess.Popen(['make', 'CFLAGS=-g -O0 -Wall -DTEST_SCANNER'], stdout = subprocess.PIPE, stderr = subprocess.PIPE)
make_out, make_err = p.communicate()
print "    make."

for root, dirs, files in os.walk(test_dir):
    for test_target_file in files:
        print test_target_file + ':'

        p = subprocess.Popen([compiler467_exe, '-Tn', test_dir + test_target_file], stdout = subprocess.PIPE, stderr = subprocess.PIPE)
        run_out, run_err = p.communicate()
        total_out = run_out + run_err

        for prev_root, prev_dirs, prev_files in os.walk(prev_dir):
            if test_target_file not in prev_files:
                print '    Error: ' + test_target_file + ' cannot be found in ' + prev_dir
                print '           Maybe it is a new test, consider adding the expected output to prev!\n'
                raise Exception(test_target_file + ' cannot be found in ' + prev_dir)

        with open(prev_dir + test_target_file, 'r') as prev_file:
            prev_total_out = ''.join(prev_file.readlines())
            if prev_total_out == total_out:
                print '    Passed.'
            else:
                print '    Failed.'
                print '    DIFF'
                print '===== ACTUAL ====='
                print total_out
                print '=================='
                print '***** EXPECTED *****'
                print prev_total_out
                print '********************'
                print '----- DIFF -----'
                diff = difflib.unified_diff(prev_total_out, total_out, fromfile = 'expected', tofile = 'actual')
                print ''.join(diff)
                print '----------------'
                print '\nThere are probably some bugs with the compiler, or update the EXPECTED in ' + prev_dir + test_target_file + '!'
                print
                raise Exception(test_target_file + ' failed to produce same output as expected!')