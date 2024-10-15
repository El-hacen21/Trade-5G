#!/bin/bash

# Configure the following values
RAPIDJSON_FOLDER=~/simulator/rapidjson
NS3_FOLDER=~/simulator/ns3/ns-3-allinone/ns-3.30
# Do not change
mkdir $NS3_FOLDER/rapidjson
cp  -r $RAPIDJSON_FOLDER/include/rapidjson/* $NS3_FOLDER/rapidjson/
cp  protocol/src/applications/model/* $NS3_FOLDER/src/applications/model/
cp  protocol/src/applications/helper/* $NS3_FOLDER/src/applications/helper/

