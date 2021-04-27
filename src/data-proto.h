#ifndef __DATA_PROTO__
#define __DATA_PROTO__

/*
 * Function prototypes for data.c dhcp-conf database routines
 */
/* Open the database file */
extern int open_db( void );

/* Load the configuration data */
extern int load_data( void );

/* Save the configuration data */
extern int save_data( void );

/* Close the database file */
extern void close_db( void );

#endif
