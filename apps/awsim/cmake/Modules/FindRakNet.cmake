# - Try to find RakNet
# Once done this will define
#
#  RakNet_FOUND - system has RakNet
#  RakNet_INCLUDES - the RakNet include directory
#  RakNet_LIBRARY - Link these to use RakNet

find_path(RakNet_INCLUDES RakPeerInterface.h
    PATHS "/usr/local/include/raknet/"
    "/usr/include/raknet/"
)

find_library(RakNet_LIBRARY
    NAMES libRakNetDLL RakNetDLL
    PATHS "/usr/local/lib/"
    "/usr/lib/"
    NO_DEFAULT_PATH
)

if(RakNet_INCLUDES AND RakNet_LIBRARY)
    set(RakNet_FOUND TRUE)
endif()

if (RakNet_FOUND)
    message(STATUS "RakNet found!")
endif()
