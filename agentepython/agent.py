 #!/usr/bin/python

import socket
import time
import struct
import sexpr
import random
import sys

def send_effector(msg):
    '''Each message is prefixed with the length of the payload message.
    The length prefix is a 32 bit unsigned integer in network order'''
    print "S:", msg
    sock.send( struct.pack("!I", len(msg)) + msg )
def recieve_perceptors():
    length_no=sock.recv(4)
    length=struct.unpack("!I", length_no)
    sexprstr=sock.recv(length[0])
    sexprlist = sexpr.str2sexpr(sexprstr)
    print "sexprlist:", sexprlist, "\n"
    '''
        perceptors={}
        for subl in sexprlist:
            if('HJ' in subl):
                perceptors[subl[1][1]] = float(subl[2][1])
        print "R: hj1", perceptors["hj1"], "\n"
    '''
    return sexprlist
HOST = 'localhost'
PORT = 3100
beamCoordsX = -10.0
beamCoordsY = 0
beamCoordsRot = 90

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect((HOST, PORT))

#send_effector( '(scene rsg/agent/nao/nao.rsg)' )
send_effector( '(scene rsg/agent/furo7x7.rsg)' )
# send_effector( '(scene rsg/agent/soccerbotcomp.rsg)' )


#send_effector( '(scene rsg/agent/newfuro.rsg)' )
recieve_perceptors()
send_effector( "(init (unum 0)(teamname Furo"+sys.argv[1]+"))" )
recieve_perceptors()
send_effector("(beam " + str(beamCoordsX) + " " + str(beamCoordsY) + " " + str(beamCoordsRot) + ")");
recieve_perceptors()
send_effector("(syn)");
recieve_perceptors()

random.seed(time.time())
Vel = 0
Giro = 0

perceptors=recieve_perceptors()
while True:
#    if( perceptors["hj1"] > 120.0 ):
#        send_effector( '(he1 -1)' )
#    if( perceptors["hj1"] < -120 ):
        send_effector( '(he1 1)' )
        #send_effector( '(er1 4)(er3 3)' )

        if( perceptors[1][4][1] == "PlayOn" ):
                Vel += random.random()-.45
                Vel=Vel * 0.999
                Giro= random.random()-.5
                send_effector( '(er1 ' + str(Vel+Giro) + ')(er3 ' +  str(Vel-Giro) + ')')
        else:
                Vel=Vel * 0.99
                send_effector( '(er1 0)(er3 0)')
        print "2:", perceptors[2]
        print "3:", perceptors[3]
        print "4:", perceptors[4]
        print "5:", perceptors[5]

        send_effector( '(syn)' )
        perceptors=recieve_perceptors()
