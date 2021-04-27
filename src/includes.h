#ifndef __INCLUDES__
#define __INCLUDES__
/*
 * Generally required include files
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>


/*
 * Defines
 */
#define MATCH(x,y) strcmp(x,y)==0
#define MATCH(x,y) strcmp(x,y)==0
#define MATCHN(x,y,z) strncmp(x,y,z)==0
#define VERSION "0.7.0 (991215)"
#define FILELEN 255
#define DEF_SUBNET "255.255.255.0"
#define DEF_BROADCAST "192.168.100.255"
#define DEF_ROUTER "192.168.100.1"
#define DEF_DNS DEF_ROUTER
#define MAX_FIXED 20			/* number of supported fixed-hosts */
#define MAX_SUBNETS 20			/* number of supported subnets */
#define HOSTNAME_LEN 26			/* 25 chars + term */
#define HWADDR_LEN 18			/* 17 chars + term */
#define IPADDR_LEN 16			/* 15 chars + term */


/*
 * Globally useful variables
 */
extern enum errlist {
  NO_PERMSSN		/* Cannot access specified location */
} errno;
char errstr[FILELEN];

#endif

/** -- end of file: includes.h -- **/
