# Project
    Blockchain protocols simulation using NS3
# Installation
 1. Install NS3 requirements (https://www.nsnam.org/wiki/Installation)
 
 2. Download NS3:
    2.1. mkdir ns3
    2.2. cd ns3
    2.2. git clone  git clone https://gitlab.com/nsnam/ns-3-allinone.git
    2.3. ./download.py -n ns-3.29 (the version we have used; see different release here : https://www.nsnam.org/)

 3. Add rapidjason (https://github.com/Tencent/rapidjson) (used for Serialization/Deserialization of packets)
    3.1. cd .. (back to workspace)
    3.1. git clone https://github.com/Tencent/rapidjson.git
    3.2  run install_script.sh

 4. Build ns3 using waf (https://www.nsnam.org/wiki/Installation)
    4.1. cd ns3/ns-allinone-3.29/ns-3.29
    4.2. CXXFLAGS="-std=c++14" ./waf configure --build-profile=optimized --out=build/optimized --enable-static 
    4.3. ./waf

 5. Build simulator
    5.1. Unzip simulation to workspace
    5.2. cd simulation/src/application/model/
    5.3. in model.pro update NS_BUILD_DIR if necessary
    5.4. export LD_LIBRARY_PATH= NS_BUILD_DIR\lib (NS_BUILD_DIR, the same in model.pro)
    5.5. source ~/.bashrc
    5.6. sudo apt-get install qt5-default
    5.6. qmake; make clean; make

 6. Run simulator : 

   


 