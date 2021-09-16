#ifndef WEB_WEBSOCKET_CONFIG_H_IN
#define WEB_WEBSOCKET_CONFIG_H_IN

#define CMAKE_INSTALL_PREFIX "/usr/local/lib/"
#define CMAKE_BINARY_DIR "/home/student/projects/snode.c/build/"

#define RELATIVE_SUBPROTOCOL_SERVER_PATH "web/websocket/subprotocol/server"
#define RELATIVE_SUBPROTOCOL_CLIENT_PATH "web/websocket/subprotocol/client"

#define SUBPROTOCOL_SERVER_COMPILE_PATH CMAKE_BINARY_DIR RELATIVE_SUBPROTOCOL_SERVER_PATH
#define SUBPROTOCOL_CLIENT_COMPILE_PATH CMAKE_BINARY_DIR RELATIVE_SUBPROTOCOL_CLIENT_PATH

#define SUBPROTOCOL_SERVER_INSTALL_PATH CMAKE_INSTALL_PREFIX "snode.c/" RELATIVE_SUBPROTOCOL_SERVER_PATH
#define SUBPROTOCOL_CLIENT_INSTALL_PATH CMAKE_INSTALL_PREFIX "snode.c/" RELATIVE_SUBPROTOCOL_CLIENT_PATH

#endif // WEB_WEBSOCKET_CONFIG_H_IN