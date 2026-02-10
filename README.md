# LegoPST - Lego Power System Technology


![Version](https://img.shields.io/badge/version-2025-blue)
![Platform](https://img.shields.io/badge/platform-Fedora%2041%2FWSL/Docker-orange)
![Language](https://img.shields.io/badge/language-C%2FC%2B%2B%2FFortran-green)
![GUI](https://img.shields.io/badge/GUI-X11%2FMotif%2FTcl,Tk,Tix-red)


**LegoPST (Lego Power System Technology): A Comprehensive Modeling and Simulation Environment for Process and Control of Complex Energy Systems and Power Plants.**

LegoPST provides a robust, modular framework for modeling the complex interactions of thermal, hydraulic, and electrical processes inherent to conventional, nuclear or renewable energy sources generation and distribution networks. Its core strength lies in developing real-time dynamic simulators and high-fidelity models that serve three critical applications:

1.  **Operator Training Simulators:** Creating immersive training simulator where power plant operators can master both nominal and emergency scenarios in a risk-free setting. The simulator can run transients strictly in real-time but also faster or slower the real-time.
2.  **Control System Design & Validation:** Designing control strategy, prototyping, testing, and verifying industrial control logic, also in real time,  within a realistic virtual environment before live deployment.
3.  **Digital Twin Development:** Building the "simulation core" of a digital twin for a complete power plant or its key subsystems. These  models can run in real-time and can be connected to live plant data for performance monitoring, predictive maintenance, and operational optimization.



## ✨ Key Features

- **Multi-Physics Static and Dynamic Simulation**: Supports thermal, hydraulic, nuclear, RES and electrical processes
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
├── Alg_legopc/       # Models Building Tools
├── lego_big/         # Component libraries
├── kprocedure/       # Administrative scripts
├── util97/           # Legacy utilities
├── util2007/         # Other legacy utilities
└── util2025/         # Modern utilities
```

## 🚀 Install and Run
The quickest way to run LegoPST is to [launch it in a Docker container](#option-1-quick-start---docker-container-execution), without installing the package and without having a machine running the Fedora 41 Linux distribution. In this case the host machine can be a generic Linux distribution running on a X86-64, Intel or AMD platform.
Alternatively, if you want a stable installation on your Fedora 41 machine, you can [download and install directly into your Fedora](#option-2-running-into-a-fully-configured-linux-fedora-41-distribution).


### Option 1: Quick Start - Docker Container Execution

**The easiest way to run LegoPST** - No installation required! Just Docker and a single command.

#### Prerequisites

Only **Docker** is required. Install it on your system:

```bash
# Ubuntu/Debian/WSL
sudo apt-get update
sudo apt-get install docker.io

# Fedora/RHEL
sudo dnf install docker

# Start Docker service
sudo systemctl enable --now docker

# Add your user to docker group (to avoid sudo)
sudo usermod -aG docker $USER
newgrp docker

# Verify installation
docker --version
docker run hello-world
```

<details>
<summary>Detailed Docker installation for Fedora 41 / WSL (click to expand)</summary>

```bash
# Fedora 41 / WSL Docker installation
sudo dnf update
sudo dnf -y install dnf-plugins-core
sudo dnf config-manager --add-repo https://download.docker.com/linux/fedora/docker-ce.repo
sudo dnf install docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin

# For WSL: Activate systemd
# Edit /etc/wsl.conf and add:
#   [boot]
#   systemd=true
# Then restart WSL: wsl --shutdown

sudo systemctl enable --now docker

# Verify
sudo docker run hello-world
```
</details>

#### Installation

Install LegoPST with a single command - this creates the `lgrun` command:

```bash
bash -c "$(curl -fsSL https://raw.githubusercontent.com/RSE-TGM/LegoPST/master/docker/install_legopst_dock.sh)"
```

This installer:
- ✅ Downloads the LegoPST Docker launcher
- ✅ Creates the `lgrun` command in `~/.local/bin/`
- ✅ Configures your PATH automatically
- ✅ No need to install LegoPST locally!

After installation, **restart your terminal** or run:
```bash
source ~/.bashrc
```

#### Usage

Launch LegoPST with the `lgrun` command:

```bash
# Standard launch (local X11 display)
lgrun

# Launch with demo model included
lgrun --demo

# If you are having trouble with the X11 display, try launching via socat (for SSH/remote connections).
lgrun --socat

# Combine options
lgrun --demo --socat

# Check for updates and pull new Docker image if available
lgrun --pull

# Show all options
lgrun --help
```

The container automatically:
- 🚀 Starts Fedora 41 with all LegoPST dependencies
- 👤 Creates a user matching your host UID/GID
- 📂 Mounts your home directory as `/host_home`
- 🖥️ Configures X11 for graphical applications
- ⚙️ Sets up a fully configured LegoPST environment

#### Data Persistence

All your work is saved on the host machine:
- **Models**: `~/legopst_userstd/legocad/`
- **Simulators**: `~/legopst_userstd/sked/`
- **Config**: `~/defaults/`

Data persists even after closing the container.

#### Alternative: One-time Execution (no installation)

If you prefer not to install `lgrun`, you can execute directly:

```bash
# Standard X11
bash -c "$(curl -fsSL https://gist.githubusercontent.com/aguag/d7c030f939f69b07784a309889b8510a/raw/lgdock.sh)"

# With socat for X11 tunneling
bash -c "$(curl -fsSL https://gist.githubusercontent.com/aguag/83c887ef78610842508b9f972130d3e1/raw/lgdock_socat.sh)"
```


### Option 2: Running into a fully configured linux Fedora 41 distribution
#### Prerequisites

Fedora 41 running into a :
- bare-metal
- virtual machine on-premis or in cloud
- Windows WSL 2
```bash
# Fedora 41 dependencies
sudo dnf update -y && \
sudo dnf install -y \
        git \
        gcc gfortran make \
        fastfetch \
        xterm xclock xhost xauth \
        hostname \
        glibc-langpack-en \
        libX11-devel \
        motif-devel \
        sqlite-devel \
        libbsd-devel \
        tcl tk tix \
        ncurses-devel \
        leafpad \
        gdbm-devel \
        ksh \
        which \
        sudo \
        ps \
        passwd \
        cpio \
        shadow-utils && \
sudo dnf clean all
sudo dnf update
sudo dnf install libmrm4 tcl tk tix libmotif-dev
sudo dnf install libxmu-dev freeglut3-dev libxext-dev libxi-dev
sudo dnf install libbsd-dev libsqlite3-dev libgdbm-compat-dev
sudo dnf install gcc gfortran make


```

### Download package and set up environment

```bash
# From the user HOME folder
cd $HOME 
# Clone the repository, git is the prerequisite
git clone remotepath/to/LegoPST.git
cd LegoPST
source .profile_legoroot # Environment setup
# The environment variable LEGOROOT will be defined as LEGOROOT=$HOME/LegoPST

# For a stable LEGOROOT installation, it is recommend to add LEGOROOT set up to .bashrc with this command:
echo "source $LEGOROOT/.profile_legoroot " >> $HOME/.bashrc

```




#### Environment Configuration and Installation

```bash
# Edit .profile_legoroot with correct path
If "/home/user" was the $HOME path:
export LEGOROOT=/home/user/LegoPST

# Add to .bashrc
echo "source $LEGOROOT/.profile_legoroot" >> ~/.bashrc

# Critical Prerequisite: libgdbm.so.2
#The LegoPST control configurator tool, config, has a critical dependency on the dbmftc2 utility. 
#This utility, in turn, requires a specific and obsolete version of the GDBM library: libgdbm.so.2. 
#To install this required dependency, execute the following script:
sudo sh $LEGOROOT/gdbm-install/install.sh


# Restart shell session
```
The system automatically detects:
- **LEGOROOT**: Project root path
- **Platform**: 32/64-bit Linux detection
- **Compiler flags**: gcc/gfortran configuration
- **Database paths**: SQLite and threading

```bash
# Note: If you have Docker installed, you can run LegoPST in a preconfigured container:
lgrun                    # After installing via install_legopst_dock.sh
# or
lgrun --demo            # Run with demo model

```

### Compilation by source
LegoPST is provided as a pre-compiled package, ready for immediate use upon download. For users who wish to customize the software or build from the latest source code, the project can also be fully recompiled. To do so, clone the repository and follow the following build instructions.
```bash

# Clone the repository
git clone remotepath/to/LegoPST.git
cd LegoPST

# Setup environment
source .profile_legoroot

# Clean build
make -f Makefile.mk clean
# Full build
make -f Makefile.mk

# Build Docker image (local only)
make -f Makefile.mk docker

# Build Docker image and push to registry
make -f Makefile.mk docker-push

# then go to
# Option 2 - Running in a fully configured Fedora 41
```

## 🎮 Usage

### Simulator directory structure

```
cd /home/user/

legocad/
├── libut/         # process modules libraries
├── libut_reg/     # control scheme libraries
|   └── libreg/    # control elementary modules libraries
├── pmod1/         # Process model task
├── reg1/          # Control model task
└── .../           # Other process or control tasks

sked/
├── simul1/        # Simulator configuration
└── .../           # Other simulators
```
### Creating New Models
#### Launch the Process Model Configurator

```bash
cd /home/user/legocad/pmod1
lgpc
```
#### Process modeling Main Files
- **\*.tom**: Model Topology   
- **f01.dat**: Model internal topology
- **f14.dat**: Model data
- **\*.a**: Object libraries
#### Launch the Control System Model Configurator

```bash
cd /home/user/legocad/reg1
config
```
#### Control system modeling Main Files
- **Context.ctx**: Control schemes task configuration
- **\*.pag**: Page resources
- **\*.bkg**: Graphical backgrounds  
- **\*.a**: Object libraries
- **\*.reg**: Regulation page
## 🔧 Simulator Configuration
#### Simulator Main Files
- **S01**: Simulator topology
- **etc**: 

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

## 🛠️ LegoPST Development

### Build System

- **Recursive Makefiles**: Modular build
- **Static Libraries**: Optimized linking  
- **Version Management**: Git integration
- **Cross-compilation**: Multi-arch support

### Other features

- **pThreads**: Compatibility layer
- **Shared Memory**: Efficient IPC
- **Message Queues**: Asynchronous communication
- **Semaphores**: Process synchronization

## 📊 Data System

- **SQLite**: Embedded database
- **XrmDatabase**: X11 resource management
- **Binary files**: Object serialization
- **Context files**: Textual configurations


## 📜 License

Proprietary - Industrial and educational use

## 🤝 Contributing

Legacy system under maintenance. Contact maintainer for critical changes.

---

**LegoPST** - *Powering Industrial Training Since 2010*
