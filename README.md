# It’s All Warped

![CI](https://github.com/dlalic/warped/workflows/CI/badge.svg)
[![CodeFactor](https://www.codefactor.io/repository/github/dlalic/warped/badge?s=6b96a8453062e056892cc190fefe4cfa98ec514f)](https://www.codefactor.io/repository/github/dlalic/warped)

## Usage

### Setup

Set up submodule:
```
git submodule update --init --remote --force
```

Set up CMake:
```
mkdir build && cd build && cmake -DCMAKE_BUILD_TYPE=release ..
```

### Build
```
cmake --build build
```

### Run
```
build/main
```

### Test
```
cd build && ctest
```

### Run integration tests in Python

Optionally, set up [Python virtual environment](https://docs.python.org/3/tutorial/venv.html) not to use the system Python.

```
cd tests && pip install -r requirements.txt && pytest test.py
```

---

Project template courtesy of [cpp-project](https://github.com/bsamseth/cpp-project). 