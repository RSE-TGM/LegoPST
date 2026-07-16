# Development Environment

## Required Dependencies (Ubuntu/Debian)
```bash
# Motif and Tcl/Tk
sudo apt install libmrm4 tcl tk tix libmotif-dev

# X11 libraries  
sudo apt install libxmu-dev freeglut3-dev libxext-dev libxi-dev

# Other essentials
sudo apt install libbsd-dev libsqlite3-dev libgdbm-compat-dev
```

## Compiler Configuration
- **C Compiler**: gcc with `-fcommon` flag for multiple definitions
- **Fortran Compiler**: gfortran with legacy support
- **Key Flags**: `-fno-second-underscore -std=legacy -finit-local-zero`

## Directory Structure Conventions
- All makefiles use `Makefile.mk` naming
- Libraries built as static archives (`.a` files)
- Source organization by functional area
- Shared includes in `libinclude/` directories

## Threading and IPC
- Uses DCE threads compatibility layer
- Extensive shared memory usage
- Message queues for inter-process communication
- Semaphores for synchronization
