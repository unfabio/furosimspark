#!/bin/bash
#
# Kill script for 3D Simulation Competitions
#

# Kill agents
echo "kill rcssagent3d"
AGENT="rcssagent3d"
killall -9 $AGENT 2> /dev/null
sleep 1
killall -9 rcsoccersim3d 2> /dev/null
killall -9 rcssserver3d 2> /dev/null
killall -9 rcssmonitor3d 2> /dev/null
