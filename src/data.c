/*
**  d a t a . c
**  -----------
**
**  Project: DHCP Configurator
**   Author: Vee Schade
**     Date: December 10, 1999
**
**  Description: This file provides all database oriented routines
**	for DHCP Configurator.  'dhcp-conf' configuration options
**	are maintained with the help of this file.  All configuration
**	options are maintained in the database file /etc/dhcp-conf.dat.
**
**	Database management is obtained via use of the GNU dbm
**	API.
*/
/*
 * Includes
 */
#include <gdbm.h>
#include "includes.h"
#include "globals.h"

/*
 * Globals
 */
GDBM_FILE dhcp;			/* Database handler for config data */


/*
 * Prototypes
 */
#include "data-proto.h"


/****************************/
/*** Function definitions ***/
/****************************/
/**
 * Opens the /etc/dhcp-conf.dat database file.  If it does not
 * exist it is created.
 *
 * @return		0 on success, -1 on failure
 */
int open_db( void )
{
  int succ = -1;

#ifdef __TRACE__
  fprintf( stderr, "TRACE (open_db): Entered...\n" );
#endif
  dhcp = gdbm_open( "/etc/dhcp-conf.dat", 0, GDBM_WRCREAT,
		    S_IRUSR|S_IWUSR, NULL );
  if( dhcp ) {
    succ = 0;
  }
#ifdef __TRACE__
  fprintf( stderr, "TRACE (open_db): Leaving...\n" );
#endif
  return succ;

} /* End of function: int open_db(void); */


/**
 * Loads the configuration data from the database.
 *
 * @return		o on success, -1 on failure.
 */
int load_data( void )
{
  int succ = 0;
  datum key, data;

#ifdef __TRACE__
  fprintf( stderr, "TRACE (load_data): Entered...\n" );
#endif
  /*
   * Get the data and load it into the master config.
   */
  key = gdbm_firstkey( dhcp );
  if( key.dptr ) {
    data = gdbm_fetch( dhcp, key );
    if( ! data.dptr ) {
      succ = -1;
    } else {
      copy_config( &config, (struct parms*) data.dptr );
    }
  }

#ifdef __TRACE__
  fprintf( stderr, "TRACE (load_data): Leaving...\n" );
#endif
  return succ;

} /* End of function: int load_data(void); */


/**
 * Saves the configuration data into the database.
 * 
 * @return		0 on success, -1 on failure
 */
int save_data( void )
{
  datum key, value;
  int succ = -1;

#ifdef __TRACE__
  fprintf( stderr, "TRACE (save_data): Entered...\n" );
#endif
  /*
   * Set-up the key/value pair for storage
   */
  key.dptr = "dhcp-conf";
  key.dsize = 10;
  value.dptr = (char*) &config;
  value.dsize = sizeof(struct parms);

  /*
   * Add the ROM to the current category replacing
   * an existing instance
   */
  succ = gdbm_store( dhcp, key, value, GDBM_REPLACE );

#ifdef __TRACE__
  fprintf( stderr, "TRACE (save_data): Leaving...\n" );
#endif
  return succ;

} /* End of function: int save_data(void); */


/**
 * Closes the database.
 */
void close_db( void )
{

#ifdef __TRACE__
  fprintf( stderr, "TRACE (close_db): Entered...\n" );
#endif
  if( dhcp ) {
    gdbm_close( dhcp );
    dhcp = NULL;
  }
#ifdef __TRACE__
  fprintf( stderr, "TRACE (close_db): Leaving...\n" );
#endif

} /* End of function: void close_db(void); */


/*** ------ end of data.c ------ ***/
