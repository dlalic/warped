import random

import pytest


@pytest.fixture
def markers(request):
    a = request.params[0] if hasattr(request, 'params') else 0.0
    b = request.params[1] if hasattr(request, 'params') else 100.0
    r = request.params[2] if hasattr(request, 'params') else 1000
    return [(random.uniform(a, b), random.uniform(a, b)) for _ in range(r)]
