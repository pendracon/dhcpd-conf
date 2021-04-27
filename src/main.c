/*
**  m a i n . c
**  -----------
**
**  Project: dhcp-conf (DHCPD Configurator)
**   Author: Vee Schade, aka Tony Cook, Vee Schade Int'l
**     Date: August 18, 1999
**
**  Description: This file provides the main entry point and utility
**	routines for dhcp-conf.  The main entry point parses the
**	command-line, displays the splash screen, and invokes the
**	main menu screen.
**
*/
/*
 * Includes
 */
#include <newt.h>
#include <time.h>
#include "includes.h"


/*
 * Globals
 */
struct parms {
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


/*
 * Function prototypes
 */
#include "main-proto.h"
#include "screen-proto.h"
#include "help-proto.h"
#include "data-proto.h"


/****************************/
/*** ------ main() ------ ***/
/****************************/
int main( int argc, char *argv[] )
{

  init();
  about();
  main_menu();
  close_screen();

} /* End of main() */


/****************************/
/*** Function definitions ***/
/****************************/
/**
 * Initializes program.
 */
void init( void )
{
  int ld = 0;

  /*
   * First try loading data from the database file.
   */
  if( open_db() != -1 ) {
    ld = load_data();
    close_db();
  }

  if( ld == -1 ) {
    /*
     * Set defaults for "Common Settings"
     */
    strcpy( config.min_ls, "600" );
    strcpy( config.max_ls, "7200" );
    strcpy( config.subnet, (const char *) DEF_SUBNET );
    strcpy( config.broad, (const char *) DEF_BROADCAST );
    strcpy( config.routers, (const char *) DEF_ROUTER );
    strcpy( config.dns, (const char *) DEF_DNS );
    config.domain[0] = '\0';
    /*
     * Set defaults for "Fixed-hosts Settings"
     */
    config.hostnames[0][0] = '\0';
    config.hwaddrs[0][0] = '\0';
    config.ipaddrs[0][0] = '\0';
    config.count = 0;
    /*
     * Set defaults for "Subnet Settings"
     */
    config.subnets[0][0] = '\0';
    config.sranges[0][0] = '\0';
    config.eranges[0][0] = '\0';
    config.subcnt = 0;
  }

  /*
   * Initialize the screen
   */
  init_screen();

} /* End of function: void init(void); */


/**
 * Copies configuration data.
 *
 * @param  dconfig	config structure to receive copy.
 * @param  sconfig	config structure to copy.
 * @return		0 on success, -1 on error.
 */
int copy_config( struct parms *dconfig, struct parms *sconfig )
{
  int rv = 0, i;

#ifdef __TRACE__
  fprintf( stderr, "TRACE (copy_config): Entered...\n" );
#endif

  if( dconfig == NULL || sconfig == NULL ) {
    rv = -1;
  } else {
    /*
     * Copy "Common Settings"
     */
    strcpy( dconfig->min_ls, sconfig->min_ls );
    strcpy( dconfig->max_ls, sconfig->max_ls );
    strcpy( dconfig->subnet, sconfig->subnet );
    strcpy( dconfig->broad, sconfig->broad );
    strcpy( dconfig->routers, sconfig->routers );
    strcpy( dconfig->dns, sconfig->dns );
    strcpy( dconfig->domain, sconfig->domain );
    /*
     * Copy "Fixed-hosts Settings"
     */
    for( i = 0; i < sconfig->count; i++ ) {
      strcpy( dconfig->hostnames[i], sconfig->hostnames[i] );
      strcpy( dconfig->hwaddrs[i], sconfig->hwaddrs[i] );
      strcpy( dconfig->ipaddrs[i], sconfig->ipaddrs[i] );
    }
    dconfig->count = sconfig->count;
    /*
     * Copy "Subnet Settings"
     */
    for( i = 0; i < sconfig->subcnt; i++ ) {
      strcpy( dconfig->subnets[i], sconfig->subnets[i] );
      strcpy( dconfig->sranges[i], sconfig->sranges[i] );
      strcpy( dconfig->eranges[i], sconfig->eranges[i] );
    }
    dconfig->subcnt = sconfig->subcnt;
  }

#ifdef __TRACE__
  fprintf( stderr, "TRACE (copy_config): Leaving...\n" );
#endif

  return rv;

} /* End of function: int copy_config(struct parms*, struct parms*) */


/**
 * Displays the given text in a help window.
 *
 * @param  text		the help text to display.
 * @param  screen	the calling screen.
 */
void help( char *text, char *screen )
{
  char title[61];
  newtComponent form,			/* The help-window form */
    ta_Text,				/* The text window displaying the
					   given text */
    bt_Exit;				/* Exit button */


  /*
   *-- Create the form
   *
   * First the textbox
   */
  ta_Text = newtTextbox( 1, 2, 64, 16, NEWT_FLAG_WRAP|NEWT_FLAG_SCROLL );
  newtTextboxSetText( ta_Text, text );
  /*
   * Now the control button
   */
  bt_Exit = newtCompactButton( 31, 19, " OK " );

  /*
   * Display and run the form
   */
  form = newtForm( NULL, NULL, 0 );
  newtFormAddComponents( form, ta_Text, bt_Exit, NULL );
  sprintf( title, "DHCP Configurator - Help: %s", screen );
  newtCenteredWindow( 70, 20, title );
  newtRunForm( form );

  /*
   * Clean-up and return
   */
  newtFormDestroy( form );

} /* End of function: void help(char*,char*); */


/**
 * Validates "Common Settings" fields of 'config' data structure.
 *
 * @param  conf		pointer to config data to validate.
 * @return		0 if everything validates, otherwise the
 *			  failed field number.
 */
int validate_common_settings( struct parms *conf )
{
  int rv = 0;
  char ch;

  do {
    /*
     *-- Validation rules:
     *
     * Both lease times must 0 or greater,
     * and Max must be greater than Min.
     */
    if( atoi( conf->min_ls ) < 0 ) {
      rv = 1;
      break;
    }
    if( atoi( conf->max_ls ) <= atoi( conf->min_ls ) ) {
      rv = 2;
      break;
    }
    /*
     * Domain names must end in an alpha-numeric character.
     */
    ch = conf->domain[ strlen( conf->domain ) - 1 ];
    if( ! isalnum( ch ) ) {
      rv = 3;
      break;
    }
    /*
     * Remaining fields must pass IP validation.
     */
    if( ip_entry_validate( conf->subnet ) == -1 ) {
      rv = 4;
      break;
    }
    if( ip_entry_validate( conf->broad ) == -1 ) {
      rv = 5;
      break;
    }
    if( ip_entry_validate( conf->routers ) == -1 ) {
      rv = 6;
      break;
    }
    if( ip_entry_validate( conf->dns ) == -1 ) {
      rv = 7;
      break;
    }
  } while( 0 );

  return rv;

} /* End of function: int validate_common_settings(struct config*); */


/**
 * Validates IP entry fields.
 *
 * @param  field	the field (data) to validate.
 * @return		0 if the field passes validates, otherwise -1.
 */
int ip_entry_validate( char *field )
{
  int rv = 0, cnt = 0, val;
  char *sub;
  char buff[IPADDR_LEN*3];

  /*
   * Validation rules:
   *   = field must match IP address formatting (xxx.xxx.xxx.xxx)
   *   = each subfield must be between the range of 0 to 255
   *   = multi-value fields must be comma separated
   */
  strcpy( buff, field );
  sub = strtok( buff, ".," );
  while( sub ) {
    val = atoi( sub );
    if( val < 0 || val > 255 ) {
      rv = -1;
      break;
    }
    cnt++;
    sub = strtok( NULL, ".," );
  }

  /*
   * We're only accepting upto 3 addresses so total sub-part
   * count should be equal to ([1-3] * 4)
   */
  if( cnt != 4 && cnt != 8 && cnt != 12 ) rv = -1;

  return rv;

} /* End of function: int ip_entry_validate(char*); */


/**
 * Calculates an integer 'value' for a specified IP address.
 *
 * @param  ipaddr	the IP address to calculate.
 * @return		the value of the address.
 */
int calc_ip_value( char *ipaddr ) {
  int val = 0;
  char buff[16];

  strcpy( buff, ipaddr );
  val += atoi( strtok( buff, "." ) );
  val += atoi( strtok( NULL, "." ) );
  val += atoi( strtok( NULL, "." ) );
  val += atoi( strtok( NULL, "." ) );

  return val;

} /* End of function: int calc_ip_value(char*); */
  

/**
 * Updates "Common Settings" fields of 'config' data structure with
 * data from specified temporary data structure.
 *
 * @param  conf		pointer to temporary config data.
 */
void update_common_settings( struct parms *conf )
{

  strcpy( config.min_ls, conf->min_ls );
  strcpy( config.max_ls, conf->max_ls );
  strcpy( config.domain, conf->domain );
  strcpy( config.subnet, conf->subnet );
  strcpy( config.broad, conf->broad );
  strcpy( config.routers, conf->routers );
  strcpy( config.dns, conf->dns );

} /* End of function: void update_common_settings(struct parms*); */


/**
 * Saves configuration data to the configuration database and writes
 * a new /etc/dhcpd.conf file.
 */
void save( void )
{
  FILE *fp;

  /*
   * Save data to database file.
   */
  if( open_db() != -1 ) {
    save_data();
    close_db();
  }

  /*
   * Write out new /etc/dhcpd.conf file.
   */
  if( (fp = fopen( "/etc/dhcpd.conf", "w" )) != NULL ) {
    write_header( fp );
    write_common_settings( fp );
    write_fixed_settings( fp );
    write_subnet_settings( fp );
    fclose( fp );
  }
  
} /* End of function: void save(void); */


/**
 * Writes an informational header to the config file.
 *
 * @param  fp		handle to the opened file.
 */
void write_header( FILE *fp )
{
  time_t curr;

  curr = time( NULL );
  fprintf( fp, "# -------------------------------------------------------\n" );
  fprintf( fp, "# dhcpd.conf\n" );
  fprintf( fp, "#\n" );
  fprintf( fp, "# Config file for DHCPd server daemon.\n" );
  fprintf( fp, "# This file was generated with 'dhcp-conf', the\n" );
  fprintf( fp, "# DHCP Configurator by Vee Schade, Vee Schade Int'l.\n" );
  fprintf( fp, "#\n" );
  fprintf( fp, "# Generated on: %s", ctime( &curr ) );
  fprintf( fp, "# -------------------------------------------------------\n" );
  fprintf( fp, "\n" );

} /* End of function: void write_header(FILE*); */


/**
 * Writes "Common Settings" data to the global block of the config file.
 *
 * @param  fp		handle to the opened file.
 */
void write_common_settings( FILE *fp )
{

  fprintf( fp, "#\n" );
  fprintf( fp, "# Global settings\n" );
  fprintf( fp, "#\n" );
  fprintf( fp, "default-lease-time %s;\n", config.min_ls );
  fprintf( fp, "max-lease-time %s;\n", config.max_ls );
  fprintf( fp, "option subnet-mask %s;\n", config.subnet );
  fprintf( fp, "option broadcast-address %s;\n", config.broad );
  fprintf( fp, "option routers %s;\n", config.routers );
  fprintf( fp, "option domain-name-servers %s;\n", config.dns );
  fprintf( fp, "option domain-name %s;\n", config.domain );
  fprintf( fp, "\n" );

} /* End of function: void write_common_settings(FILE*); */


/**
 * Writes "Fixed-host Settings" to fixed host(s) block of config file.
 *
 * @param  fp		handle to the opened file.
 */
void write_fixed_settings( FILE *fp )
{
  int i;

  if( config.count ) {
    fprintf( fp, "#\n" );
    fprintf( fp, "# Fixed-host settings\n" );
    fprintf( fp, "#\n" );
  }

  for( i = 0; i < config.count; i++ ) {
    fprintf( fp, "host %s {\n", config.hostnames[i] );
    fprintf( fp, "\thardware ethernet %s;\n", config.hwaddrs[i] );
    fprintf( fp, "\tfixed-address %s;\n", config.ipaddrs[i] );
    fprintf( fp, "}\n\n" );
  }

} /* End of function: void write_fixed_settings(FILE*); */


/**
 * Writes "Subnet(s) Settings" to subnet block of config file.
 *
 * @param  fp		handle to the opened file.
 */
void write_subnet_settings( FILE *fp )
{
  int i, first = 1;
  char addr[IPADDR_LEN];

  if( config.subcnt ) {
    fprintf( fp, "#\n" );
    fprintf( fp, "# Subnet settings\n" );
    fprintf( fp, "#\n" );
  }

  for( i = 0; i < config.subcnt; i++ ) {
    if( ! MATCH( addr, config.subnets[i] ) ) {
      if( ! first ) fprintf( fp, "}\n\n" );
      else first = 0;
      fprintf( fp, "subnet %s netmask %s {\n", config.subnets[i],
	       config.subnet );
      strcpy( addr, config.subnets[i] );
    }
    fprintf( fp, "\trange %s %s;\n", config.sranges[i], config.eranges[i] );
  }
  if( ! first ) fprintf( fp, "}\n\n" );
  fprintf( fp, "# --- End of file: dhcpd.conf\n" );

} /* End of function: void write_subnet_settings(FILE*); */


/** -- end of file: main.c -- **/
