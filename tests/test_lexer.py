# Must be executed in compiler467/

import os
import subprocess

compiler467_exe = './compiler467'
test_dir = './tests/lexer/'
prev_dir = test_dir + 'prev/'

for root, dirs, files in os.walk(test_dir):
    for test_target_file in files:
        print test_target_file + ':'

        p = subprocess.Popen([compiler467_exe, '-Tn', test_dir + test_target_file], stdout = subprocess.PIPE, stderr = subprocess.PIPE)
        run_out, run_err = p.communicate()
        total_out = run_out + run_err

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
                print '===== EXPECTED ====='
                print prev_total_out
                print '===================='
                print '\nThere are probably some bugs with the compiler, or update the EXPECTED in ' + prev_dir + test_target_file + '!'
                print
                raise Exception(test_target_file + ' failed to produce same output as expected!')