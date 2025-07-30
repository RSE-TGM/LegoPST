# LegoPST - Lego Power System Technology


![Version](https://img.shields.io/badge/version-2025-blue)
![Platform](https://img.shields.io/badge/platform-WSL%20Fedora%2041-orange)
![Language](https://img.shields.io/badge/language-C%2FC%2B%2B%2FFortran-green)
![GUI](https://img.shields.io/badge/GUI-X11%2FMotif-red)

**LegoPST** (Lego Power System Technology) is a modular simulation and control environment for thermal, hydraulic, and electrical processes in energy production and transport. Designed for training power plant operators and industrial control systems.

## ✨ Key Features

- **Multi-Physics Simulation**: Supports thermal, hydraulic, and electrical processes
- **Modular Environment**: Reusable component architecture
- **Graphical Interfaces**: MMI (Man-Machine Interface) editor with X11/Motif support
- **Configuration System**: Hierarchical management of pages and components
- **Real-Time Simulation**: Simulation engine for operator training
- **Integrated CAD**: Design tools for process schematics

## 🏗️ Architecture

### Main Components

```
LegoPST/
├── AlgLib/           # Core algorithm libraries
├── Alg_mmi/          # Man-Machine Interface
├── Alg_rt/           # Runtime System
├── legocad/          # CAD Tools
├── lego_big/         # Component libraries
├── kprocedure/       # Administrative scripts
├── util97/           # Legacy utilities
└── util2007/         # Modern utilities
```

### Core Libraries

- **libcom.a**: Communication and event handling
- **libsim.a**: Core simulation functions  
- **libnet.a**: Network communication
- **libipc.a**: Inter-process communication
- **libdispatcher.a**: Message dispatching system
- **libmanovra.a**: Control operations

## 🚀 Quick Start

### Prerequisites

```bash
# Fedora 41 dependencies
sudo dnf update
sudo dnf install libmrm4 tcl tk tix libmotif-dev
sudo dnf install libxmu-dev freeglut3-dev libxext-dev libxi-dev
sudo dnf install libbsd-dev libsqlite3-dev libgdbm-compat-dev
sudo dnf install gcc gfortran make
```

### Compilation by source

```bash
# Clone the repository
git clone remotepath/to/LegoPST2010A_WSL_DARHE6_64.git
cd LegoPST2010A_WSL_DARHE6_64

# Setup environment
source .profile_legoroot

# Full build
make -f Makefile.mk

# Clean build
make -f Makefile.mk clean
```

### Environment Configuration and Installation

```bash
# Edit .profile_legoroot with correct path
export LEGOROOT=/home/user/LegoPST2010A_WSL_DARHE6_64

# Add to .bashrc
echo "source ~/.profile_legoroot" >> ~/.bashrc

# Restart shell session
```

The system automatically detects:
- **LEGOROOT**: Project root path
- **Platform**: 32/64-bit Linux detection
- **Compiler flags**: gcc/gfortran configuration
- **Database paths**: SQLite and threading

## 🎮 Usage

### Starting the Configurator

```bash
etc
etc
```

### Creating New Models

1. lgpc - Launch the process cad configurator
2. etc  
3. etc
4. etc



## 🔧 Configuration

### Main Files

- **Context.ctx**: etc
- **\*.pag**: Page resources
- **\*.bkg**: Graphical backgrounds  
- **\*.lib**: Object libraries
- **\*.reg**: Regulation pages

### Environment Variables

etc

## 🏭 Use Cases

- **Operator Training**: Power plant simulation
- **System Design**: CAD for P&I diagrams
- **Procedure Testing**: Operational sequence validation
- **SCADA Training**: Advanced operator interfaces

## 📋 Page Types

- **Synoptic**: Overall plant overview
- **Stations**: Specific section control  
- **Regulation**: Automatic control algorithms
- **Teleperm**: DCS-type interfaces
- **Library**: Reusable component collections

## 🛠️ Development

### Build System

- **Recursive Makefiles**: Modular build
- **Static Libraries**: Optimized linking  
- **Version Management**: Git integration
- **Cross-compilation**: Multi-arch support

### etc

- **pThreads**: Compatibility layer
- **Shared Memory**: Efficient IPC
- **Message Queues**: Asynchronous communication
- **Semaphores**: Process synchronization

## 📊 Data System

- **SQLite**: Embedded database
- **XrmDatabase**: X11 resource management
- **Binary files**: Object serialization
- **Context files**: Textual configurations

## 🔍 Debug & Monitoring

etc

## 📜 License

Proprietary - Industrial and educational use

## 🤝 Contributing

Legacy system under maintenance. Contact maintainer for critical changes.

---

**LegoPST** - *Powering Industrial Training Since 2010*