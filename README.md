
---

# Project
**Trade-5G Simulation using NS3**

Below are the steps to install, configure, and build the simulation environment.

---

## Installation Guide

### 1. Install NS3 Requirements
Follow the instructions to install the necessary dependencies for NS3:
- Visit [NS3 Installation Guide](https://www.nsnam.org/wiki/Installation) for details.

### 2. Download and Install NS3
1. Create a directory for NS3:
   ```bash
   mkdir ns3
   cd ns3
   ```
2. Clone the NS3 repository:
   ```bash
   git clone https://gitlab.com/nsnam/ns-3-allinone.git
   ```
3. Download the specific version of NS3 used in this project (v3.29):
   ```bash
   ./download.py -n ns-3.29
   ```
   - You can check for other versions [here](https://www.nsnam.org/).

### 3. Add RapidJSON (for Serialization/Deserialization of Packets)
1. Return to the workspace root:
   ```bash
   cd ..
   ```
2. Clone the RapidJSON library:
   ```bash
   git clone https://github.com/Tencent/rapidjson.git
   ```
3. Run the installation script:
   ```bash
   ./install_script.sh
   ```

### 4. Build NS3 using WAF
Follow the instructions to build NS3 with the WAF build tool:
1. Navigate to the NS3 directory:
   ```bash
   cd ns3/ns-allinone-3.29/ns-3.29
   ```
2. Configure the build environment:
   ```bash
   CXXFLAGS="-std=c++14" ./waf configure --build-profile=optimized --out=build/optimized --enable-static
   ```
3. Build NS3:
   ```bash
   ./waf
   ```

### 5. Build the Simulator
1. Unzip the simulator files to your workspace.
2. Navigate to the application model directory:
   ```bash
   cd simulation/src/application/model/
   ```
3. Update the `NS_BUILD_DIR` in `model.pro` if necessary.
4. Set the `LD_LIBRARY_PATH` environment variable:
   ```bash
   export LD_LIBRARY_PATH= NS_BUILD_DIR/lib
   ```
   - Replace `NS_BUILD_DIR` with the actual path used in `model.pro`.
5. Reload environment variables:
   ```bash
   source ~/.bashrc
   ```
6. Install Qt5 for building the GUI:
   ```bash
   sudo apt-get install qt5-default
   ```
7. Build the simulator:
   ```bash
   qmake; make clean; make
   ```

### 6. Run the Simulator
**[TO DO]:** Instructions for running the simulator will be provided.

---

## To Do List
- [ ] Add detailed steps for running the simulator.
- [ ] Provide full documentation for configuring and executing different protocol simulations.
- [ ] Include examples and additional setup details for the simulation environment.

---
