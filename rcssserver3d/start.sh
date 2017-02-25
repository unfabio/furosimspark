#!/bin/bash
#
#


AGENT_BINARY=rcssagent3d
BINARY_DIR="rcssagent3d"
LIBS_DIR="./libs"
NUM_PLAYERS=5

team="Furo"
host="localhost"
port=3100

cd build
./kill.sh

rm -f "$BINARY_DIR/$AGENT_BINARY"
make

if [ ! -f "$BINARY_DIR/$AGENT_BINARY" ]; then
echo "Error al compillar el Agente."


else

echo "compilado el Agente."




 rcssserver3d/rcsoccersim3d &


sleep 5
export LD_LIBRARY_PATH=$LIBS_DIR:$LD_LIBRARY_PATHo

mkdir -p log

for ((i=1;i<=$NUM_PLAYERS;i++)); do
	    echo "Ejecutar Agente No. $i -- leftFuroBlue"
	    "$BINARY_DIR/$AGENT_BINARY" $opt --unum=$i --type 4 --team=leftFuroBlue > log/stdblueout$i 2> log/stdblueerr$i&
    sleep 0.2

	    echo "Ejecutar Agente No. $i -- rightFuroRed"
	    "$BINARY_DIR/$AGENT_BINARY" $opt --unum=$i --type 4 --team=rightFuroRed > log/stdredout$i 2> log/stdrederr$i&
    sleep 0.2
done

fi

read -n1 -r -p "Presione una tecla para salir..." key
./kill.sh
