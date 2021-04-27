#ifndef __SCREEN_PROTO__
#define __SCREEN_PROTO__

/*
 * Function prototypes for screen.c
 * - dhcp-conf interface routines
 */
/* Initialize the screen */
void init_screen( void );

/* The About/Splash screen */
void about( void );

/* The main menu */
void main_menu( void );

/* Display Common Settings form */
void common_settings( void );

/* Display Fixed-host Settings form */
void fixed_settings( void );

/* Display Fixed-host Settings form */
void subnet_settings( void );

/* Validates a "fixed-host" entry */
int validate_fixed_entry( char*, char*, char* );

/* Validates a "subnet" entry */
int validate_subnet_entry( char*, char*, char* );

/* Return the currently selected fixed-host listbox entry */
int get_fixed_selected( newtComponent, char* );

/* Return the currently selected subnet listbox entry */
int get_subnet_selected( newtComponent, char* );

/* Update Fixed-hosts listbox with current/new entries */
void update_fixed_list( newtComponent );

/* Update subnets listbox with current/new entries */
void update_subnet_list( newtComponent );

/* Filters num-only entry fields */
int num_entry_filter( newtComponent, void*, int, int );

/* Filters domain-name entry fields */
int domain_entry_filter( newtComponent, void*, int, int );

/* Filters IP entry fields */
int ip_entry_filter( newtComponent, void*, int, int );

/* Filters HW entry fields */
int hw_entry_filter( newtComponent, void*, int, int );

/* Closes the screen */
void close_screen( void );

#endif
/** -- end of file: screen-proto.h -- **/
