#ifndef __MAIN_PROTO__
#define __MAIN_PROTO__

/*
 * Function prototypes for main.c
 * - dhcp-conf utility and help routines
 */
/* Initializes program */
void init( void );

/* Copies configuration data */
int copy_config( struct parms*, struct parms* );

/* Displays given text in a help window */
void help( char*, char* );

/* Validates "Common Settings" fields of data structure */
int validate_common_settings( struct parms* );

/* Validates IP entry fields for conformance. */
int ip_entry_validate( char* );

/* Calculates a value for and IP address */
int calc_ip_value( char* );

/* Updates "Common Settings" fields of data structure from temp data */
void update_common_settings( struct parms* );

/* Saves and writes the configuration data */
void save( void );

/* Writes informational header to config file */
void write_header( FILE* );

/* Writes "Common Settings" to config file */
void write_common_settings( FILE* );

/* Writes "Fixed-host Settings" to config file */
void write_fixed_settings( FILE* );

/* Writes "Subnet Settings" to config file */
void write_subnet_settings( FILE* );

#endif

/** -- end of file: main-proto.h -- **/
