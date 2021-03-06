#ifndef CONFIG
#define CONFIG

#define CMAKE_SOURCE_DIR "/home/student/projects/snode.c/"

#define SERVERROOT CMAKE_SOURCE_DIR "docs/html"

#define SERVERCAFILE CMAKE_SOURCE_DIR "certs/snode.c_-_Root_CA.crt"
#define SERVERCERTF CMAKE_SOURCE_DIR "certs/snode.c_-_server.pem"
#define SERVERKEYF CMAKE_SOURCE_DIR "certs/snode.c_-_server.key.encrypted.pem"

#define CLIENTCAFILE CMAKE_SOURCE_DIR "certs/snode.c_-_Root_CA.crt"
#define CLIENTCERTF CMAKE_SOURCE_DIR "certs/snode.c_-_client.pem"
#define CLIENTKEYF CMAKE_SOURCE_DIR "certs/snode.c_-_client.key.encrypted.pem"

#define KEYFPASS "snode.c"

#endif
