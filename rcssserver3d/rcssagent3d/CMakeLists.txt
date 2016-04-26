########### next target ###############

set(rcssagent3d_SRCS
   main.cpp
   behavior.h
   playmode.h
   estrategias/soccerperceptor.h
   estrategias/soccerperceptor.cpp
   estrategias/soccerbehavior.h
   estrategias/soccerbehavior.cpp
   estrategias/crearJugador.h
   estrategias/crearJugador.cpp
   estrategias/leftFuroBlue/sbarquerol.h
   estrategias/leftFuroBlue/sbarquerol.cpp
   estrategias/leftFuroBlue/sbdefensal.h
   estrategias/leftFuroBlue/sbdefensal.cpp
   estrategias/leftFuroBlue/sbdelanterol.h
   estrategias/leftFuroBlue/sbdelanterol.cpp
   estrategias/rightFuroRed/sbarqueror.h
   estrategias/rightFuroRed/sbarqueror.cpp
   estrategias/rightFuroRed/sbdefensar.h
   estrategias/rightFuroRed/sbdefensar.cpp
   estrategias/rightFuroRed/sbdelanteror.h
   estrategias/rightFuroRed/sbdelanteror.cpp
)

add_executable(rcssagent3d ${rcssagent3d_SRCS})

if (WIN32)
 set(NET_LIBS ws2_32.lib)
endif (WIN32)

target_link_libraries(rcssagent3d
    ${RCSSNET3D_LIBRARIES} ${NET_LIBS}
    debug ${SPARK_LIBRARY_DEBUG}
    debug ${SALT_LIBRARY_DEBUG}
    debug ${ZEITGEIST_LIBRARY_DEBUG}
    debug ${OXYGEN_LIBRARY_DEBUG}
    debug ${KEROSIN_LIBRARY_DEBUG}
    optimized ${SPARK_LIBRARY_RELEASE}
    optimized ${SALT_LIBRARY_RELEASE}
    optimized ${ZEITGEIST_LIBRARY_RELEASE}
    optimized ${OXYGEN_LIBRARY_RELEASE}
    optimized ${KEROSIN_LIBRARY_RELEASE}
)

set_target_properties(rcssagent3d PROPERTIES
    INSTALL_RPATH "${RPATH_DIRECTORIES}")
install(TARGETS rcssagent3d DESTINATION ${BINDIR})
