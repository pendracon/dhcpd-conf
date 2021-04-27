#ifndef __GLOBALS__
#define __GLOBALS__

extern struct parms {
  /*
   * "Common Settings" data
   */
  char min_ls[6],			/* Minimum lease time */
    max_ls[6],				/* Maximum lease time */
    domain[100],			/* Default domain name */
    subnet[16],				/* Default subnet-mask */
    broad[16],				/* Default broadcast address */
    routers[50],			/* Default gateway(s) */
    dns[50];				/* Default dns server(s) */
  /*
   * "Fixed-hosts Settings" data
   */
					/* Hostnames */
  char hostnames[MAX_FIXED][HOSTNAME_LEN],
    hwaddrs[MAX_FIXED][HWADDR_LEN],	/* Hardware addresses */
    ipaddrs[MAX_FIXED][IPADDR_LEN];	/* IP Addresses */
  int count;				/* Number of current fixed hosts */
  /*
   * "Subnet Settings" data
   */
  char subnets[MAX_SUBNETS][IPADDR_LEN],/* Subnet base addresses */
    sranges[MAX_SUBNETS][IPADDR_LEN],	/* Subnet range start addresses */
    eranges[MAX_SUBNETS][IPADDR_LEN];	/* Subnet range end addresses */
  int subcnt;				/* Number of current subnets */

} config;


#endif

/** -- end of file: globals.h -- **/
