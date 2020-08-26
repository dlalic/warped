import os
import random
import shlex
import subprocess

import pytest
from scipy.interpolate import interp1d


def calculate_interpolation(x_values, y_values, value):
    f = interp1d(x_values, y_values)
    return f(value)


def execute(commands):
    dir_path = os.path.dirname(os.path.abspath(__file__))
    executable = os.path.join(os.path.dirname(dir_path), 'build', 'main')
    executable = shlex.quote(executable)
    with subprocess.Popen(executable, shell=True, stdin=subprocess.PIPE, stdout=subprocess.PIPE) as proc:
        (stdout, stderr) = proc.communicate(input='\n'.join(commands).encode(), timeout=1.0)
        return float(stdout.decode('utf-8'))


def test_b2s(markers):
    valid_b2s_value = random.uniform(min(markers)[0], max(markers)[0])
    commands = ['marker {} {}'.format(marker[0], marker[1]) for marker in markers]
    commands.append('b2s {}'.format(valid_b2s_value))
    result = execute(commands)
    expected = calculate_interpolation([m[0] for m in markers], [m[1] for m in markers], valid_b2s_value)
    assert expected == pytest.approx(result, 0.01)


def test_s2b(markers):
    markers.sort(key=lambda m: m[1])
    valid_s2b_value = random.uniform(min(markers)[1], markers[-1][1])
    commands = ['marker {} {}'.format(marker[0], marker[1]) for marker in markers]
    commands.append('s2b {}'.format(valid_s2b_value))
    result = execute(commands)
    expected = calculate_interpolation([m[1] for m in markers], [m[0] for m in markers], valid_s2b_value)
    assert expected == pytest.approx(result, 0.01)
