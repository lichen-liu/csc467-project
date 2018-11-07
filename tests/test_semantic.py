# Must be executed in compiler467/

import os
import subprocess
import re

test_semantic_assigned_script = '../compiler467/tests/test_semantic_assigned.py'
test_semantic_const_script = '../compiler467/tests/test_semantic_const.py'
test_semantic_core_script = '../compiler467/tests/test_semantic_core.py'

total_passed = 0

print '0..'

semantic_assigned_passed = True
p = subprocess.Popen(['python', test_semantic_assigned_script], stdout = subprocess.PIPE, stderr = subprocess.PIPE)
run_out, run_err = p.communicate()
total_out = run_err + run_out
print total_out
matchObj = re.search(r'Successful!\sTotal\sPassed:\s([0-9]+)', total_out)
if matchObj:
   total_passed += int(matchObj.group(1))
else:
   semantic_assigned_passed = False

print '0..1..'

semantic_const_passed = True
p = subprocess.Popen(['python', test_semantic_const_script], stdout = subprocess.PIPE, stderr = subprocess.PIPE)
run_out, run_err = p.communicate()
total_out = run_err + run_out
print total_out
matchObj = re.search(r'Successful!\sTotal\sPassed:\s([0-9]+)', total_out)
if matchObj:
   total_passed += int(matchObj.group(1))
else:
   semantic_const_passed = False

print '0..1..2..'

semantic_core_passed = True
p = subprocess.Popen(['python', test_semantic_core_script], stdout = subprocess.PIPE, stderr = subprocess.PIPE)
run_out, run_err = p.communicate()
total_out = run_err + run_out
print total_out
matchObj = re.search(r'Successful!\sTotal\sPassed:\s([0-9]+)', total_out)
if matchObj:
   total_passed += int(matchObj.group(1))
else:
   semantic_core_passed = False


print "===================================="
if semantic_assigned_passed:
    print test_semantic_assigned_script + " Passed!"
else:
    print test_semantic_assigned_script + " Failed!"

if semantic_const_passed:
    print test_semantic_const_script + " Passed!"
else:
    print test_semantic_const_script + " Failed!"

if semantic_core_passed:
    print test_semantic_core_script + " Passed!"
else:
    print test_semantic_core_script + " Failed!"

print "Total Passed: " + str(total_passed) + "!"