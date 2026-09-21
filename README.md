# LegoPST - Lego Power System Technology


![Version](https://img.shields.io/badge/version-2.0-blue)
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
- **Runtime HMI**: Live supervision on the model drawing (`lghmi`, `draw2gr`) with values, engineering units, command mode and operator faceplates
- **FMI 2.0 Export**: Any task can be exported as an FMU - a light variant for machines running LegoPST, or a self-contained bundle that runs on a bare Linux box - for co-simulation with third-party tools

## 🏗️ Architecture

### Main Components

```
LegoPST/
├── AlgLib/           # Core algorithm libraries
├── Alg_mmi/          # Man-Machine Interface
├── Alg_rt/           # Runtime System (simulation engine, HMI, FMU export)
├── Alg_legopc/       # Models Building Tools (legopc, draw2gr, lghmi)
├── legocad/          # Sources of the process and control modules
├── lego_big/         # Component libraries (linked as legocad/lego_big)
├── kprocedure/       # Administrative scripts (sources)
├── kbin/             # Administrative scripts (installed commands)
├── kutil/            # Administrative utilities
├── docker/           # Docker build and install scripts
├── demo/             # Demo work area (tarball)
├── docs/             # Guides: build, commands, configuration files
├── util97/           # Legacy utilities
├── util2007/         # Other legacy utilities
├── util2025/         # Modern utilities
└── VERSION           # Project version (used by build and installer)
```

### Core Components

**AlgLib/** - Core algorithm libraries
- Contains fundamental libraries (libRt.a, libcom.a, libsim.a, etc.)
- Threading support via POSIX threads (pthreads)
- Database support via SQLite
- Shared memory and IPC utilities

**Alg_mmi/** - Man-Machine Interface
- MMI client/server architecture
- Configuration tools for graphical interfaces
- Widget libraries and drawing tools
- Conversion utilities for legacy formats

**Alg_rt/** - Runtime System
- Real-time simulation engine
- Process control and monitoring
- Network simulation components
- Session management

**Alg_legopc/** - Model Building Tools
- `legopc`: graphical editor for process models, pages and operator elements
- `draw2gr`: runtime HMI drawn on the model itself
- `lghmi`: task selector, HMI and simulator launcher
- Model dialogs, module libraries, PDF/PNG export

**legocad/** - Module sources and libraries
- Fortran sources of the process modules (`libut`) and of the control modules (`libut_reg`)
- Component libraries shared with `lego_big/`

**kprocedure/** - Administrative Scripts
- System management utilities
- Process control scripts
- Database maintenance tools
- User management and configuration

### Key Libraries Structure
- **libcom.a**: Communication and event handling
- **libsim.a**: Simulation core functions
- **libnet.a**: Network communication
- **libipc.a**: Inter-process communication
- **libdispatcher.a**: Message dispatching system
- **libmanovra.a**: Control operations
- **libutil.a**: General utilities

### Configuration System
The system uses hierarchical configuration:
1. Environment variables set in `.profile_legoroot`
2. OS detection via `uname` (the supported platform is Linux x86_64)
3. Extension-based directory structure
4. User-specific settings in home directories

## 🚀 Install and Run
The quickest way to run LegoPST is to [launch it in a Docker container](#option-1-quick-start---docker-container-execution), without installing the package and without having a machine running the Fedora 41 Linux distribution. In this case the host machine can be a generic Linux distribution running on a X86-64, Intel or AMD platform.
Alternatively, if you want a stable installation on your Fedora 41 machine, you can [download and install directly into your Fedora](#option-2-running-into-a-fully-configured-linux-fedora-41-distribution).


### Option 1: Quick Start - Docker Container Execution

**The easiest way to run LegoPST** - No installation required! Just Docker and a single command.

#### Prerequisites

**Docker** is required, plus a working X11 display on the host: a native X server on
Linux, or WSLg on Windows/WSL. `lgrun --socat` additionally needs `socat` on the host.
Install Docker on your system:

```bash
# Ubuntu/Debian/WSL
sudo apt-get update
sudo apt-get install docker-ce docker-ce-cli containerd.io

# Fedora/RHEL
sudo dnf install -y dnf-plugins-core
sudo dnf config-manager addrepo \
  --from-repofile=https://download.docker.com/linux/fedora/docker-ce.repo
sudo dnf install -y docker-ce docker-ce-cli containerd.io docker-compose-plugin

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
sudo bash -c 'cat >> /etc/wsl.conf << EOF
[boot]
systemd=true
EOF'
# Then restart WSL: wsl --shutdown

sudo systemctl enable --now docker

# Verify
sudo docker run hello-world
```
</details>

#### LegoPST container installation

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


### Option 2: Running into a fully configured linux Fedora 41 distribution
#### Prerequisites

Fedora 41 running into a :
- bare-metal
- virtual machine on-premis or in cloud
- Windows WSL 2
```bash
# Fedora 41 dependencies

sudo dnf update -y && \
sudo dnf install -y --setopt=install_weak_deps=False \
    git \
    gcc gfortran make \
    fastfetch \
    xterm xfce4-terminal xclock xhost xauth \
    hostname \
    glibc-langpack-en \
    libX11-devel \
    libXmu-devel \
    libXext-devel \
    libXi-devel \
    freeglut-devel \
    motif-devel \
    sqlite-devel \
    libbsd-devel \
    tcl tk tix \
    ncurses-devel \
    mousepad \
    gdbm-devel \
    ksh \
    which \
    procps-ng \
    cpio \
    zip \
    rsync \
    xdg-utils \
    ghostscript \
    drawing \
    evince \
    falkon \
    unix2dos \
    xorg-x11-fonts-misc \
    xorg-x11-fonts-100dpi \
    xorg-x11-fonts-75dpi \
    libXcursor \
    adwaita-cursor-theme && \
sudo dnf clean all


```

> **Choosing a terminal.** LegoPST opens terminal windows through `lgterm`, which
> defaults to the spartan `xterm`. To use a better one, install it (`xfce4-terminal`
> is in the list above) and pick it in `legopc` under *File -> Settings -> Terminal*.
> The choice is read at every launch, so it applies to programs that are already
> running. `lgterm --list` shows which terminals are installed and which one wins.

> **Optional, only for the FMU features**: `python3` with `fmpy` (a virtualenv is
> enough) to run an exported FMU, and Docker to test a bundle in a clean container.
> See [Alg_rt/lg_fmu/USAGE.md](Alg_rt/lg_fmu/USAGE.md).

#### Download LegoPST package and set up user environment

```bash
# From the user HOME folder
cd $HOME 
# Clone the repository, git is the prerequisite
git clone https://github.com/RSE-TGM/LegoPST.git
cd LegoPST
source .profile_legoroot # Environment setup
# The environment variable LEGOROOT will be defined as LEGOROOT=$HOME/LegoPST

# For a stable LEGOROOT installation, it is recommend to add LEGOROOT set up to .bashrc with this command:
echo "source $LEGOROOT/.profile_legoroot " >> $HOME/.bashrc

```

#### Create your work area

The package brings the tools, not the models. `legocad` and `sked` in your home are
**symbolic links** to a *work area*, a `legopst_*` directory holding models and
simulators; `lgswitch` switches the two links between areas, so a demo, a real plant
and a trial can sit side by side. Start from the demo area shipped with the repository:

```bash
cd $HOME
# Creates ~/legopst_userstd, with legocad/ (models) and sked/ (simulators)
tar xzf $LEGOROOT/demo/legopst_userstd.tgz
# Points ~/legocad and ~/sked at it
lgswitch legopst_userstd
```

`lgswitch` with no argument lists the areas it finds in the current directory and asks
which one to use; `lgswitch -l` prints the current state without changing anything.
The same switch is available from `lghmi`, under *File -> Work area*.

### Restart shell session
The system automatically detects:
- **LEGOROOT**: Project root path
- **Platform**: operating system, via `uname` (Linux x86_64)
- **Compiler flags**: gcc/gfortran configuration
- **Database paths**: SQLite and threading

## Compilation by source
LegoPST is provided as a pre-compiled package, ready for immediate use upon download. For users who wish to customize the software or build from the latest source code, the project can also be fully recompiled. 
To do so, set up into a [fully configured linux Fedora 41 distribution](#option-2-running-into-a-fully-configured-linux-fedora-41-distribution), clone the repository and follow these build instructions.
```bash

# Clone the repository
git clone https://github.com/RSE-TGM/LegoPST.git
cd LegoPST

# Setup environment
source .profile_legoroot

# Clean build
make -f Makefile.mk clean

# Full build: compiles every subproject and installs the commands.
# It does NOT build the Docker image, so it needs no Docker on the machine.
make -f Makefile.mk

# Build the Docker image (optional, ~4.5 GB, needs Docker installed and running)
make -f Makefile.mk docker

# Build the Docker image and push it to the registry
make -f Makefile.mk docker-push

# List every target
make -f Makefile.mk help

# then go to
# Option 2 - Running in a fully configured Fedora 41
```

> Without `source .profile_legoroot` the include paths stay empty (`-I -I`) and the
> build fails in a way that does not say why. See [docs/BUILD.md](docs/BUILD.md).

## 🎮 Usage

### Where to start: `lghmi`

```bash
lghmi
```

`lghmi` is the control desk of the simulator, and nearly everything is reachable from
there:

- the **tasks of the current simulator**, read from its `S01`: pick one and its HMI
  (`draw2gr`) opens on the model drawing itself, with live values, engineering units
  and command mode;
- **start the simulation** (`net_startup`) and follow its log in the same window;
- **command faceplates** (`xstaz`), which open even with no simulation running - handy
  while building and configuring the stations;
- *File -> Work area*: switch work area, the `lgswitch` above;
- *File -> Current simulator*: the simulator you are looking at is the one you work on,
  the one `KSIM` and every `k*` command refer to;
- *Tools*: edit the model, the `kUpSim` and `kCompile` submenus, and a terminal opened
  in the current directory.

The whole tour is in [Alg_legopc/LGHMI.md](Alg_legopc/LGHMI.md).

### Simulator directory structure

`legocad` and `sked` are the two symbolic links set by `lgswitch`
([Create your work area](#create-your-work-area)): they point inside the work area you
picked, so this is what you see from your home.

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

# or, from anywhere, naming the model
lgpc pmod1
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
A simulator is a directory under `~/sked`: it declares which tasks it is made of, how
they run and how they are wired to each other.

#### Simulator Main Files
- **al_sim.conf**: what the simulator is made of - tasks, integration steps, MMI page
  names, connection rules. `creasim` installs it, then it is edited by hand. See
  [docs/AL_SIM_CONF.md](docs/AL_SIM_CONF.md)
- **S01**: Simulator topology - tasks, types, paths, steps and input/output connections,
  generated from `al_sim.conf` by `kConnex`
- **Simulator**: sizing - `MAX_CAMPIONI`, `NUM_VAR`, snapshot, backtrack
- **task directories**: one per model, each with its executable, data and `out/`

`creasim` creates a simulator, `ksims` lists them, `ksetsim <name>` picks the current
one (`KSIM`), which is the one every `k*` command acts upon - in `lghmi` it follows the
directory you are looking at. See [docs/BUILD.md](docs/BUILD.md).

## 📚 Documentation

| Topic | Document |
|---|---|
| **Index of all the documentation** (md, html, pdf, doc, txt) | [INDICE_DOCUMENTAZIONE.html](INDICE_DOCUMENTAZIONE.html) |
| Build, versioning, choosing the current simulator | [docs/BUILD.md](docs/BUILD.md) |
| Dependencies, compilers, directory conventions | [Environment_setup.md](Environment_setup.md) |
| The `lg*` commands: `lgpc`, `lghmi`, `lgswitch`, `lgterm`, ... | [docs/COMANDI_LG.md](docs/COMANDI_LG.md) |
| Graphical CAD `legopc`: libraries, model dialogs, pages, export | [Alg_legopc/README.md](Alg_legopc/README.md) |
| `lghmi`: tasks, HMI, work areas, current simulator, `S01` format | [Alg_legopc/LGHMI.md](Alg_legopc/LGHMI.md) |
| `al_sim.conf`: how a simulator is composed | [docs/AL_SIM_CONF.md](docs/AL_SIM_CONF.md) |
| MMI pages and configuration | [Alg_mmi/README.md](Alg_mmi/README.md) |
| FMU export and co-simulation (FMI 2.0) | [Alg_rt/lg_fmu/USAGE.md](Alg_rt/lg_fmu/USAGE.md) |
| Command faceplates: `compstaz`, `xstaz` | [Alg_rt/grafica/xstaz/README.md](Alg_rt/grafica/xstaz/README.md) |
| The `kprocedure` commands | [kbin/README.md](kbin/README.md) |
| Code conventions | [CONVENTIONS.md](CONVENTIONS.md) |

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
- **Version Management**: The project version is defined in the `VERSION` file at the repository root. The build system (`Makefile.mk`) reads this file and injects the version into all scripts (`lgdock`, `lgrun`). The file `version.h` is auto-generated at build time with git commit hash and build number for C/Fortran code.
- **Target**: Linux x86_64 (Fedora 41 is the reference platform)

### Other features

- **POSIX threads**: pthreads compatibility layer
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
