# dcs-elog
Logbook for NA61/SHINE experiment


## Running

### 1. Clone
 - `git clone https://github.com/xxlukasz11/dcs-elog.git`

### 2. Setup SQLite3
#### Program requires SQLite 3.8.3 or newer. If you already have it installed, proceed to next step.
- Download amalgamiation version from https://www.sqlite.org/download.html
- paste `sqlite3.c` and `sqlite3.h` files to `sqlite` folder

### 3. Build
- create build directory `mkdir build`
- enter it, and type: `cmake ..`
- If you ommited step 2, add NOSQLITE flag: `-DNOSQLITE=True`. CMake will then link against your system sqlite3 library
- If you want to build project without unit tests, add NOTESTS flag: `-DNOTESTS=True`
- Build command: `make -j 4`

### 4. Run
- Execute `scripts/start_elog.sh`
- Run all tests: `ctest all`
- Run single test: `ctest <test_name>` or with more detailed output: `../bin/<test_name>`` di

To stop, execute: `scripts/elog_stop.sh`

Behaviour of the program can be adjusted by editing parameter file: `resources/parameters.conf`
