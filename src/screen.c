/*
**  s c r e e n . c
**  ---------------
**
**  Project: dhcp-conf (DHCPD Configurator)
**   Author: Vee Schade, aka Tony Cook, Vee Schade Int'l
**     Date: August 18, 1999
**
**  Description: This file provides the main menu interface and entry
**	screens for dhcp-conf.  All interfaces are done with RedHat
**	Software's 'newt' toolkit.  The main menu is composed of three
**	buttons for selecting one the remaining three screens: "Common
**	Settings" - settings common to most entries; "Fixed Hosts" -
**	for entering information about hosts with fixed IP addresses;
**	"Dynamic IPs" - for entering information about a single subnet.
**
**	dhcp-conf uses the GNU DBM library for storing configuration
**	information.
**
*/
/*
 * Includes
 */
#include <newt.h>
#include "includes.h"
#include "globals.h"


/*
 * Function prototypes
 */
#include "screen-proto.h"
#include "help-proto.h"
#include "main-proto.h"


/****************************/
/*** Function definitions ***/
/****************************/
/**
 * Initializes screen.
 */
void init_screen( void )
{

  newtInit();
  newtCls();

} /* End of function: void init_screen(void); */


/**
 * Displays the main menu.
 */
void main_menu( void )
{
  int loop = 1;				/* Loop control */
  newtComponent form,			/* Form for main menu */
    la_Common,				/* Desc. for "Common Settings" item */
    bt_Common,				/* "Common Settings" button */
    la_Fixed,				/* Desc. for "Fixed Hosts" item */
    bt_Fixed,				/* "Fixed Hosts" button */
    la_Dynamic,				/* Desc. for "Dynamic IPs" item */
    bt_Dynamic,				/* "Dynamic IPs" button */
    bt_Save,				/* Save current settings */
    bt_About,				/* Show splash screen */
    bt_Help,				/* Show help for this screen */
    bt_Exit,				/* Exit application */
    selected;				/* The selected component */

  /*
   *-- Create the form
   *
   * First the menu buttons
   */
  bt_Common = newtButton( 10, 3, "Common" );
  la_Common = newtLabel( 25, 4, "Change settings common to all IPs" );
  bt_Fixed = newtButton( 10, 8, "Fixed " );
  la_Fixed = newtLabel( 25, 9, "Change fixed-host entries" );
  bt_Dynamic = newtButton( 10, 13, "Subnet" );
  la_Dynamic = newtLabel( 25, 14, "Change dynamic-IP subnet" );
  /*
   * Now the control buttons
   */
  bt_Save = newtCompactButton( 6, 19, "Save" );
  bt_Help = newtCompactButton( 22, 19, "Help" );
  bt_About = newtCompactButton( 38, 19, "About" );
  bt_Exit = newtCompactButton( 54, 19, "Exit" );

  /*
   * Display and run the form
   */
  form = newtForm( NULL, NULL, 0 );
  newtFormAddComponents( form,
			 bt_Common, la_Common,
			 bt_Fixed, la_Fixed,
			 bt_Dynamic, la_Dynamic,
			 bt_Save, bt_Help, bt_About, bt_Exit,
			 NULL );
  while( loop ) {			/* Start of menu loop */
    /*
     * Open the form window
     */
    newtCenteredWindow( 70, 20, "DHCPD Configurator - Main Menu" );
    selected = newtRunForm( form );

    /*
     * Parse widget selection
     */
    if( selected == bt_Common ) {	/* Display common settings form */
      common_settings();
    }
    if( selected == bt_Fixed ) {	/* Display fixed-host settings form */
      fixed_settings();
    }
    if( selected == bt_Dynamic ) {	/* Display subnet settings form */
      subnet_settings();
    }
    if( selected == bt_Save ) {		/* Save and write config data */
      save();
      loop = 0;
    }
    if( selected == bt_About ) {	/* Show About box */
      about();
    }
    if( selected == bt_Help ) {		/* Show help window */
      help_main_menu();
    }
    if( selected == bt_Exit ) {		/* Exit application */
      loop = 0;
    }
    newtCls();
  }					/* End of menu loop */
  /*
   * Clean-up and return for next run
   */
  newtFormDestroy( form );

} /* End of function: void main_menu(void); */


/**
 * Displays "Common Settings" form.
 */
void common_settings( void )
{
  int loop = 1, err;
  newtComponent form,			/* The form for this screen */
    la_MinLs,				/* Minimum (default) lease label */
    eb_MinLs,				/* Minimum lease time entrybox */
    la_MaxLs,				/* Maximum lease time label */
    eb_MaxLs,				/* Maximum lease time entrybox */
    la_Domain,				/* Default domain name label */
    eb_Domain,				/* Default domain name entrybox */
    la_Subnet,				/* Default subnet netmask label */
    eb_Subnet,				/* Default subnet netmask entrybox */
    la_Broad,				/* Default broadcast IP label */
    eb_Broad,				/* Default broadcast IP entrybox */
    la_Route,				/* Default router IP label */
    eb_Route,				/* Default router IP entrybox */
    la_DNS,				/* Default DNS IP label */
    eb_DNS,				/* Default DNS IP entrybox */
    bt_Save,				/* "Save and exit" ctrl button */
    bt_Reset,				/* Reset all fields ctrl button */
    bt_Help,				/* Display help ctrl button */
    bt_Exit,				/* "Exit w/o save" ctrl button */
    selected;				/* The selected widget */
  char *min_ls,				/* Pointer to eb_MinLs value */
    *max_ls,				/* Pointer to eb_MaxLs value */
    *subnet,				/* Pointer to eb_Subnet value */
    *broad,				/* Pointer to eb_Broad value */
    *route,				/* Pointer to eb_Route value */
    *dns,				/* Pointer to eb_DNS value */
    *domain;				/* Pointer to eb_Domain value */
  struct parms cconfig;			/* Temporary config copy */


  /*
   *-- Create the form
   *
   * First the entry fields
   */
					/* Minimum ("default") lease time */
  la_MinLs = newtLabel( 2, 2, "Lease time (in seconds) - Minimum:" );
  eb_MinLs = newtEntry( 37, 2, config.min_ls, 10, &min_ls, 0 );
  newtEntrySetFilter( eb_MinLs, num_entry_filter, NULL );
					/* Maximum lease time */
  la_MaxLs = newtLabel( 49, 2, "Maximum:" );
  eb_MaxLs = newtEntry( 58, 2, config.max_ls, 10, &max_ls, 0 );
  newtEntrySetFilter( eb_MaxLs, num_entry_filter, NULL );
					/* Default domain name */
  la_Domain = newtLabel( 2, 5, "Default domain name......:" );
  eb_Domain = newtEntry( 29, 5, config.domain, 39, &domain,
			 NEWT_ENTRY_SCROLL );
  newtEntrySetFilter( eb_Domain, domain_entry_filter, NULL );
					/* Default subnet-mask */
  la_Subnet = newtLabel( 2, 8, "Default subnet netmask...:" );
  eb_Subnet = newtEntry( 29, 8, config.subnet, 16, &subnet, 0 );
  newtEntrySetFilter( eb_Subnet, ip_entry_filter, NULL );
					/* Default broadcast IP */
  la_Broad = newtLabel( 2, 11, "Default broadcast address:" );
  eb_Broad = newtEntry( 29, 11, config.broad, 16, &broad, 0 );
  newtEntrySetFilter( eb_Broad, ip_entry_filter, NULL );
					/* Default router IP */
  la_Route = newtLabel( 2, 14, "Default gateway(s).......:" );
  eb_Route = newtEntry( 29, 14, config.routers, 39, &route,
			NEWT_ENTRY_SCROLL );
  newtEntrySetFilter( eb_Route, ip_entry_filter, NULL );
					/* Default primary DNS */
  la_DNS = newtLabel( 2, 17, "Default DNS server(s)....:" );
  eb_DNS = newtEntry( 29, 17, config.dns, 39, &dns, NEWT_ENTRY_SCROLL );
  newtEntrySetFilter( eb_DNS, ip_entry_filter, NULL );
  /*
   * Now the control buttons
   */
  bt_Save = newtCompactButton( 6, 19, "Keep" );
  bt_Reset = newtCompactButton( 22, 19, "Clear" );
  bt_Help = newtCompactButton( 38, 19, "Help" );
  bt_Exit = newtCompactButton( 54, 19, "Exit" );

  /*
   * Display and run the form
   */
  form = newtForm( NULL, NULL, 0 );
  newtFormAddComponents( form,
			 la_MinLs, eb_MinLs,
			 la_MaxLs, eb_MaxLs,
			 la_Domain, eb_Domain,
			 la_Subnet, eb_Subnet,
			 la_Broad, eb_Broad,
			 la_Route, eb_Route,
			 la_DNS, eb_DNS,
			 bt_Save, bt_Reset, bt_Help, bt_Exit,
			 NULL );

  while( loop ) {
    /*
     * Open the form window
     */
    newtCenteredWindow( 70, 20, "DHCPD Configurator - Common Settings" );
    selected = newtRunForm( form );

    /*
     * Parse widget selection
     */
    if( selected == bt_Save ) {		/* Keep entries and return */
      strcpy( cconfig.min_ls, min_ls );
      strcpy( cconfig.max_ls, max_ls );
      strncpy( cconfig.domain, domain, 99 );
      strcpy( cconfig.subnet, subnet );
      strcpy( cconfig.broad, broad );
      strncpy( cconfig.routers, route, 49 );
      strncpy( cconfig.dns, dns, 49 );
      if( ( err = validate_common_settings( &cconfig ) ) > 0 ) newtBell();
      switch( err ) {
      case 1:				/* Minimum Lease time */
	newtFormSetCurrent( form, eb_MinLs );
	break;
      case 2:				/* Maximum Lease time */
	newtFormSetCurrent( form, eb_MaxLs );
	break;
      case 3:				/* Domain name */
	newtFormSetCurrent( form, eb_Domain );
	break;
      case 4:				/* Subnet IP */
	newtFormSetCurrent( form, eb_Subnet );
	break;
      case 5:				/* Broadcast IP */
	newtFormSetCurrent( form, eb_Broad );
	break;
      case 6:				/* Router IP(s) */
	newtFormSetCurrent( form, eb_Route );
	break;
      case 7:				/* DNS IP(s) */
	newtFormSetCurrent( form, eb_DNS );
	break;
      default:
	update_common_settings( &cconfig );
	loop = 0;
      }
    }
    if( selected == bt_Reset ) {	/* Clear all entry fields */
      newtEntrySet( eb_MinLs, "", 0 );
      newtEntrySet( eb_MaxLs, "", 0 );
      newtEntrySet( eb_Domain, "", 0 );
      newtEntrySet( eb_Subnet, "", 0 );
      newtEntrySet( eb_Broad, "", 0 );
      newtEntrySet( eb_Route, "", 0 );
      newtEntrySet( eb_DNS, "", 0 );
    }
    if( selected == bt_Help ) {		/* Display help for this screen */
      help_common_settings();
    }
    if( selected == bt_Exit ) {		/* Discard and return to main menu */
      loop = 0;
    }
    newtCls();
  }

  /*
   * Clean-up and return
   */
  newtFormDestroy( form );

} /* End of function: void common_settings(void); */


/**
 * Displays "Fixed-host Settings" form.
 */
void fixed_settings() {
  int loop = 1, i;
  char buff[65];
  newtComponent form,			/* The form for this screen */
    la_Current,				/* "Current Fixed-hosts" label */
    lb_Current,				/* "Current Fixed-hosts" listbox */
    la_Hostname,			/* New host's name entry label */
    eb_Hostname,			/* New host's name entrybox */
    la_Hardware,			/* New host's ethernet addr. label */
    eb_Hardware,			/* New host's ethernet addr. entry */
    la_Address,				/* New host's assigned IP label */
    eb_Address,				/* New host's assigned IP entry */
    bt_Keep,				/* "Keep current settings" button */
    bt_Del,				/* Delete selected entry button */
    bt_Help,				/* Display screen help button */
    bt_Exit,				/* Exit without keeping changes */
    selected;				/* Widget selection */
  char *hostname = NULL,
    *hardware = NULL,
    *address = NULL;
  struct parms backup;

  /*
   * Backup configuration data for discarding changes
   */
  copy_config( &backup, &config );

  /*
   *-- Create the form
   *
   * First the "Current" listbox
   */
  sprintf( buff, "%-25s   %-17s   %-15s",
	   "Current Hostname", "HW Address", "Assigned IP" );
  la_Current = newtLabel( 2, 1, buff );
  lb_Current = newtListbox( 2, 2, 10,
			    NEWT_FLAG_SCROLL | NEWT_FLAG_RETURNEXIT );
  newtListboxSetWidth( lb_Current, 66 );
  if( config.count > 0 ) {
    update_fixed_list( lb_Current );
  }
  /*
   * Now the new-entry fields
   */
  la_Hostname = newtLabel( 3, 14, "New entry hostname..:" );
  eb_Hostname = newtEntry( 25, 14, NULL, 26, &hostname, 0 );
  newtEntrySetFilter( eb_Hostname, domain_entry_filter, NULL );
  la_Hardware = newtLabel( 3, 15, "New entry HW address:" );
  eb_Hardware = newtEntry( 25, 15, NULL, 18, &hardware, 0 );
  newtEntrySetFilter( eb_Hardware, hw_entry_filter, NULL );
  la_Address  = newtLabel( 3, 16, "New entry IP address:" );
  eb_Address  = newtEntry( 25, 16, NULL, 16, &address,
			   NEWT_FLAG_RETURNEXIT );
  newtEntrySetFilter( eb_Address, ip_entry_filter, NULL );
  /*
   * Finally the control buttons
   */
  bt_Keep = newtCompactButton( 6, 19, "Keep" );
  bt_Del  = newtCompactButton( 22, 19, "Delete" );
  bt_Help = newtCompactButton( 38, 19, "Help" );
  bt_Exit = newtCompactButton( 54, 19, "Exit" );

  /*
   * Display and run the form
   */
  form = newtForm( NULL, NULL, 0 );
  newtFormAddComponents( form,
			 la_Current, lb_Current,
			 la_Hostname, eb_Hostname,
			 la_Hardware, eb_Hardware,
			 la_Address, eb_Address,
			 bt_Keep, bt_Del, bt_Help, bt_Exit,
			 NULL );
  if( config.count == 0 ) newtFormSetCurrent( form, eb_Hostname );

  while( loop ) {
    /*
     * Open the form window
     */
    newtCenteredWindow( 70, 20, "DHCPD Configurator - Fixed-hosts Settings" );
    selected = newtRunForm( form );

    /*
     * Parse widget selection
     */
    if( selected == lb_Current ) {	/* Edit current listbox selection */
      i = get_fixed_selected( lb_Current, NULL );
      if( i != -1 ) {
	newtEntrySet( eb_Hostname, config.hostnames[i], 1 );
	newtEntrySet( eb_Hardware, config.hwaddrs[i], 1 );
	newtEntrySet( eb_Address, config.ipaddrs[i], 1 );
	newtFormSetCurrent( form, eb_Hostname );
      }
    }
    if( selected == eb_Address ) {	/* Add new entry to list */
      /*
       * Retrieve entry values
       */
      hostname = newtEntryGetValue( eb_Hostname );
      hardware = newtEntryGetValue( eb_Hardware );
      address = newtEntryGetValue( eb_Address );
      /*
       * Validate the entry values
       */
      switch( validate_fixed_entry( hostname, hardware, address ) ) {
      case 1:
	selected = eb_Hostname;
	break;
      case 2:
	selected = eb_Hardware;
	break;
      case 3:
	selected = eb_Address;
	break;
      default:
	selected = NULL;
      }
      /*
       * On error prompt user to errant field, otherwise
       * add the entry to the list.
       */
      if( selected || config.count >= MAX_FIXED ) {
	if( selected ) newtFormSetCurrent( form, selected );
	newtBell();
      } else {
	strcpy( config.hostnames[config.count], hostname );
	strcpy( config.hwaddrs[config.count], hardware );
	strcpy( config.ipaddrs[config.count], address );
	config.count++;
	update_fixed_list( lb_Current );
	newtEntrySet( eb_Hostname, "", 0 );
	newtEntrySet( eb_Hardware, "", 0 );
	newtFormSetCurrent( form, eb_Hostname );
      }
    }
    if( selected == bt_Keep ) {		/* Exit screen keeping changes */
      loop = 0;
    }
    if( selected == bt_Del ) {		/* Remove selected list item */
      i = get_fixed_selected( lb_Current, NULL );
      if( i != -1 ) {
	for( i += 1; i < config.count; i++ ) {
	  strcpy( config.hostnames[i-1], config.hostnames[i] );
	  strcpy( config.hwaddrs[i-1], config.hwaddrs[i] );
	  strcpy( config.ipaddrs[i-1], config.ipaddrs[i] );
	}
	config.count--;
	config.hostnames[config.count][0] = '\0';
	config.hwaddrs[config.count][0] = '\0';
	config.ipaddrs[config.count][0] = '\0';
	update_fixed_list( lb_Current );
      }
      newtEntrySet( eb_Hostname, "", 0 );
      newtEntrySet( eb_Hardware, "", 0 );
      newtEntrySet( eb_Address, "", 0 );
    }
    if( selected == bt_Help ) {		/* Display help for this screen */
      help_fixed_settings();
    }
    if( selected == bt_Exit ) {		/* Exit screen discarding changes */
      copy_config( &config, &backup );
      loop = 0;
    }
    newtCls();
  }

  /*
   * Clean-up and return
   */
  newtFormDestroy( form );
  
} /* End of function: void fixed_settings(void); */


/**
 * Displays "Subnet Settings" form.
 */
void subnet_settings() {
  int loop = 1, i;
  char buff[65];
  newtComponent form,			/* The form for this screen */
    la_Current,				/* "Current Subnet" label */
    lb_Current,				/* "Current Subnet" listbox */
    la_Subnet,				/* New subnet entry label */
    eb_Subnet,				/* New subnet entrybox */
    la_Start,				/* New subnet range start label */
    eb_Start,				/* New subnet range start entrybox */
    la_End,				/* New subnet range end label */
    eb_End,				/* New subnet range end entrybox */
    bt_Keep,				/* "Keep current settings" button */
    bt_Del,				/* Delete selected entry button */
    bt_Help,				/* Display screen help button */
    bt_Exit,				/* Exit without keeping changes */
    selected;				/* Widget selection */
  char *subnet = NULL,
    *start = NULL,
    *end = NULL;
  struct parms backup;

  /*
   * Backup configuration data for discarding changes
   */
  copy_config( &backup, &config );

  /*
   *-- Create the form
   *
   * First the "Current" listbox
   */
  sprintf( buff, "%-25s   %-17s   %-15s",
	   "Current Subnet", "Range Start", "Range End" );
  la_Current = newtLabel( 2, 1, buff );
  lb_Current = newtListbox( 2, 2, 10,
			    NEWT_FLAG_SCROLL | NEWT_FLAG_RETURNEXIT );
  newtListboxSetWidth( lb_Current, 66 );
  if( config.subcnt > 0 ) {
    update_subnet_list( lb_Current );
  }
  /*
   * Now the new-entry fields
   */
  la_Subnet = newtLabel( 5, 14, "New range subnet....:" );
  eb_Subnet = newtEntry( 27, 14, NULL, 16, &subnet, 0 );
  newtEntrySetFilter( eb_Subnet, ip_entry_filter, NULL );
  la_Start = newtLabel( 5, 15, "New range start IP..:" );
  eb_Start = newtEntry( 27, 15, NULL, 16, &start, 0 );
  newtEntrySetFilter( eb_Start, ip_entry_filter, NULL );
  la_End  = newtLabel( 5, 16, "New range end IP....:" );
  eb_End  = newtEntry( 27, 16, NULL, 16, &end,
			   NEWT_FLAG_RETURNEXIT );
  newtEntrySetFilter( eb_End, ip_entry_filter, NULL );
  /*
   * Finally the control buttons
   */
  bt_Keep = newtCompactButton( 6, 19, "Keep" );
  bt_Del  = newtCompactButton( 22, 19, "Delete" );
  bt_Help = newtCompactButton( 38, 19, "Help" );
  bt_Exit = newtCompactButton( 54, 19, "Exit" );

  /*
   * Display and run the form
   */
  form = newtForm( NULL, NULL, 0 );
  newtFormAddComponents( form,
			 la_Current, lb_Current,
			 la_Subnet, eb_Subnet,
			 la_Start, eb_Start,
			 la_End, eb_End,
			 bt_Keep, bt_Del, bt_Help, bt_Exit,
			 NULL );
  if( config.subcnt == 0 ) newtFormSetCurrent( form, eb_Subnet );

  while( loop ) {
    /*
     * Open the form window
     */
    newtCenteredWindow( 70, 20, "DHCPD Configurator - Subnets Settings" );
    selected = newtRunForm( form );

    /*
     * Parse widget selection
     */
    if( selected == lb_Current ) {	/* Edit current listbox selection */
      i = get_subnet_selected( lb_Current, NULL );
      if( i != -1 ) {
	newtEntrySet( eb_Subnet, config.subnets[i], 1 );
	newtEntrySet( eb_Start, config.sranges[i], 1 );
	newtEntrySet( eb_End, config.eranges[i], 1 );
	newtFormSetCurrent( form, eb_Subnet );
      }
    }
    if( selected == eb_End ) {	/* Add new entry to list */
      /*
       * Retrieve entry values
       */
      subnet = newtEntryGetValue( eb_Subnet );
      start = newtEntryGetValue( eb_Start );
      end = newtEntryGetValue( eb_End );
      /*
       * Validate the entry values
       */
      switch( validate_subnet_entry( subnet, start, end ) ) {
      case 1:
	selected = eb_Subnet;
	break;
      case 2:
	selected = eb_Start;
	break;
      case 3:
	selected = eb_End;
	break;
      default:
	selected = NULL;
      }
      /*
       * On error prompt user to errant field, otherwise
       * add the entry to the list.
       */
      if( selected || config.subcnt >= MAX_SUBNETS ) {
	if( selected ) newtFormSetCurrent( form, selected );
	newtBell();
      } else {
	strcpy( config.subnets[config.subcnt], subnet );
	strcpy( config.sranges[config.subcnt], start );
	strcpy( config.eranges[config.subcnt], end );
	config.subcnt++;
	update_subnet_list( lb_Current );
	newtEntrySet( eb_Start, "", 0 );
	newtEntrySet( eb_End, "", 0 );
	newtFormSetCurrent( form, eb_Start );
      }
    }
    if( selected == bt_Keep ) {		/* Exit screen keeping changes */
      loop = 0;
    }
    if( selected == bt_Del ) {		/* Remove selected list item */
      i = get_subnet_selected( lb_Current, NULL );
      if( i != -1 ) {
	for( i += 1; i < config.subcnt; i++ ) {
	  strcpy( config.subnets[i-1], config.subnets[i] );
	  strcpy( config.sranges[i-1], config.sranges[i] );
	  strcpy( config.eranges[i-1], config.eranges[i] );
	}
	config.subcnt--;
	config.subnets[config.subcnt][0] = '\0';
	config.sranges[config.subcnt][0] = '\0';
	config.eranges[config.subcnt][0] = '\0';
	update_subnet_list( lb_Current );
      }
      newtEntrySet( eb_Subnet, "", 0 );
      newtEntrySet( eb_Start, "", 0 );
      newtEntrySet( eb_End, "", 0 );
    }
    if( selected == bt_Help ) {		/* Display help for this screen */
      help_subnet_settings();
    }
    if( selected == bt_Exit ) {		/* Exit screen discarding changes */
      copy_config( &config, &backup );
      loop = 0;
    }
    newtCls();
  }

  /*
   * Clean-up and return
   */
  newtFormDestroy( form );
  
} /* End of function: void subnet_settings(void); */


/**
 * Validates a new "Fixed-hosts Settings" entry.
 *
 * @param  hostname	the hostname entry value.
 * @param  hardware	the HW address entry value.
 * @param  address	the IP address entry value.
 * @return		the index of the failed argument, else 0.
 */
int validate_fixed_entry( char *hostname, char *hardware, char *address )
{
  int i, idx = 0;

  /*
   * Validation rules:
   *	1 - all fields require an entry (strlen != 0)
   *	2 - where appropriate, entry value must be properly formatted
   *	3 - entry value must not already exist.
   */
  if( strlen( hostname ) == 0 ) idx = 1;
  if( strlen( hardware ) == 0 ) idx = 2;
  if( strlen( address ) == 0 || ip_entry_validate( address ) < 0 )
    idx = 3;

  if( ! idx ) {
    for( i = 0; i < config.count; i++ ) {
      if( MATCH( hostname, config.hostnames[i] ) ) {
	idx = 1;
	break;
      }
      if( MATCH( hardware, config.hwaddrs[i] ) ) {
	idx = 2;
	break;
      }
      if( MATCH( address, config.ipaddrs[i] ) ) {
	idx = 3;
	break;
      }
    }
  }

  return idx;

} /* End of function: int validate_fixed_entry(char*,char*,char*); */
    

/**
 * Validates a new "Subnet(s) Settings" entry.
 *
 * @param  subnet	the subnet IP entry value.
 * @param  start	the range start IP entry value.
 * @param  end		the range end IP entry value.
 * @return		the index of the failed argument, else 0.
 */
int validate_subnet_entry( char *subnet, char *start, char *end )
{
  int i, idx = 0, sub, sr, er;
  
  /*
   * Validation rules:
   *	1 - all fields require an entry (strlen != 0)
   *	2 - all fields must be properly formatted (xxx.xxx.xxx)
   *	3 - both start and end range must be greater than subnet
   *	4 - end range must be >= start range
   *	5 - where appropriate, entry value must not already exist.
   * ---
   * Validate for values and formatting
   */
  if( strlen( subnet ) == 0 || ip_entry_validate( subnet ) < 0) {
    idx = 1;
  } else {
    sub = calc_ip_value( subnet );
  }
  if( strlen( start ) == 0 || ip_entry_validate( start ) < 0) {
    idx = 2;
  } else {
    sr = calc_ip_value( start );
  }
  if( strlen( end ) == 0 || ip_entry_validate( end ) < 0 ) {
    idx = 3;
  } else {
    er = calc_ip_value( end );
  }
  /*
   * Validate for ordering
   */
  if( ! idx ) {
    if( sr <= sub ) idx = 2;
    if( er < sr ) idx = 3;
  }
  /*
   * Validate against pre-existence
   */
  if( ! idx ) {
    for( i = 0; i < config.subcnt; i++ ) {
      if( MATCH( start, config.sranges[i] ) ) {
	idx = 2;
	break;
      }
      if( MATCH( end, config.eranges[i] ) ) {
	idx = 3;
	break;
      }
    }
  }

  return idx;

} /* End of function: int validate_subnet_entry(char*,char*,char*); */
    

/**
 * Returns the index of the currently selected "Fixed-host Settings"
 * listbox item.
 *
 * @param  listbox	the Listbox to return selected.
 * @param  sel		if not NULL passes-back the selected item's key.
 * @return		the selected item's index or -1 on failure.
 */
int get_fixed_selected( newtComponent listbox, char *sel )
{
  int i;
  char *key;

  key = (char *) newtListboxGetCurrent( listbox );
  for( i = 0; i < MAX_FIXED; i++ ) {
    if( strlen( config.hostnames[i] ) == 0 ) {
      i = -1;
      newtBell();
      break;
    }
    if( MATCH( config.hostnames[i], key ) ) {
      if( sel != NULL ) sel = key;
      break;
    }
  }

  return i;

} /* End of function: int get_fixed_selected(newtComponent,char*) */


/**
 * Returns the index of the currently selected "Subnets Settings"
 * listbox item.
 *
 * @param  listbox	the Listbox to return selected.
 * @param  sel		if not NULL passes-back the selected item's key.
 * @return		the selected item's index or -1 on failure.
 */
int get_subnet_selected( newtComponent listbox, char *sel )
{
  int i;
  char *key;

  key = (char *) newtListboxGetCurrent( listbox );
  for( i = 0; i < MAX_SUBNETS; i++ ) {
    if( strlen( config.eranges[i] ) == 0 ) {
      i = -1;
      newtBell();
      break;
    }
    if( MATCH( config.eranges[i], key ) ) {
      if( sel != NULL ) sel = key;
      break;
    }
  }

  return i;

} /* End of function: int get_subnet_selected(newtComponent,char*) */


/**
 * Updates Fixed-hosts listbox with current/new entries.
 *
 * @param  listbox	the Listbox to update.
 */
void update_fixed_list( newtComponent listbox )
{
  int i, j;
  char hosts[MAX_FIXED][HOSTNAME_LEN],
    hwaddrs[MAX_FIXED][HWADDR_LEN],
    ipaddrs[MAX_FIXED][IPADDR_LEN];
  char buff[65];

  /*
   *-- Sort entries.
   *
   * Build and sort settings work list
   */
  for( i = 0; i < config.count; i++ ) {
    strcpy( hosts[i], config.hostnames[i] );
  }
  qsort( hosts, config.count, HOSTNAME_LEN, strcmp );
  /*
   * Sort hw/ip addresses into work lists
   */
  for( i = 0; i < config.count; i++ ) {
    for( j = 0; j < config.count; j++ ) {
      if( MATCH( hosts[i], config.hostnames[j] ) ) {
	strcpy( hwaddrs[i], config.hwaddrs[j] );
	strcpy( ipaddrs[i], config.ipaddrs[j] );
	break;
      }
    }
  }
  /*
   * Return sorted lists to storage
   */
  for( i = 0; i < config.count; i++ ) {
    strcpy( config.hostnames[i], hosts[i] );
    strcpy( config.hwaddrs[i], hwaddrs[i] );
    strcpy( config.ipaddrs[i], ipaddrs[i] );
  }

  /*
   * Update listbox with sorted entries
   */
  newtListboxClear( listbox );
  for( i = 0; i < config.count; i++ ) {
    sprintf( buff, "%-25s   %-17s   %-15s",
	     config.hostnames[i], config.hwaddrs[i], config.ipaddrs[i] );
    newtListboxAppendEntry( listbox, buff, config.hostnames[i] );
  }

} /* End of function: void update_fixed_list(newtComponent); */


/**
 * Updates Subnets listbox with current/new entries.
 *
 * @param  listbox	the Listbox to update.
 */
void update_subnet_list( newtComponent listbox )
{
  int i, j;
  char subnets[MAX_SUBNETS][IPADDR_LEN],
    sranges[MAX_SUBNETS][IPADDR_LEN],
    eranges[MAX_SUBNETS][IPADDR_LEN],
    slist[MAX_SUBNETS][(IPADDR_LEN * 3) + 2];
  char buff[65];

  /*
   *-- Sort entries.
   *
   * Build and sort settings work list: build a sortable
   * list with all three subnet elements since multiple
   * ranges are assignable to a single subnet.
   */
  for( i = 0; i < config.subcnt; i++ ) {
    sprintf( slist[i], "%s-%s-%s",
	     config.subnets[i], config.sranges[i], config.eranges[i] );
  }
  qsort( slist, config.subcnt, (IPADDR_LEN * 3) + 2, strcmp );
  /*
   * Return sorted lists to storage
   */
  for( i = 0; i < config.subcnt; i++ ) {
    strcpy( config.subnets[i], strtok( slist[i], "-" ) );
    strcpy( config.sranges[i], strtok( NULL, "-" ) );
    strcpy( config.eranges[i], strtok( NULL, "-" ) );
  }

  /*
   * Update listbox with sorted entries
   */
  newtListboxClear( listbox );
  for( i = 0; i < config.subcnt; i++ ) {
    sprintf( buff, "%-25s   %-17s   %-15s",
	     config.subnets[i], config.sranges[i], config.eranges[i] );
    newtListboxAppendEntry( listbox, buff, config.eranges[i] );
  }

} /* End of function: void update_subnet_list(newtComponent); */


/**
 * Filters data entered into number only fields.
 *
 * @param  field	the entry field to filter.
 * @param  data		pointer to the data (not used).
 * @param  ch		the character to filter.
 * @param  pos		the position of the filtered character.
 * @return		the character if valid, otherwise 0.
 */
int num_entry_filter( newtComponent field, void *data, int ch, int pos )
{
  int rv = ch;

  /*
   * Filtering rules:
   *   = digits are allowed (0 - 9)
   *   = line-edit keys are allowed
   */
  if( ( ch > 31 && ch < 48 ) || ( ch > 57 && ch < 127 ) ) {
    rv = 0;
    newtBell();
  }

  return rv;

} /* End of function: int num_entry_filter(...); */


/**
 * Filters data entered into domain-name entry fields.
 *
 * @param  field	the entry field to filter.
 * @param  data		pointer to the data (not used).
 * @param  ch		the character to filter.
 * @param  pos		the position of the filtered character.
 * @return		the character if valid, otherwise 0.
 */
int domain_entry_filter( newtComponent field, void *data, int ch, int pos )
{
  int rv = ch;

  /*
   * Filtering rules:
   *   = alpha-numeric characters are allowed (a-z,A-Z,0-9)
   *   = period '.' allowed
   *   = dash '-' allowed
   *   = underscore '_' allowed
   *   = line-edit keys are allowed
   */
  if( ( ch > 31 && ch < 45 ) || 
      ch == 47 ||
      ( ch > 57 && ch < 65 ) ||
      ( ch > 90 && ch < 95 ) ||
      ch == 96 || 
      ( ch > 122 && ch < 127 ) ) {
    rv = 0;
    newtBell();
  }

  return rv;

} /* End of function: int domain_entry_filter(...); */


/**
 * Filters data entered into an IP Address entry field.
 *
 * @param  field	the entry field to filter.
 * @param  data		pointer to the data (not used).
 * @param  ch		the character to filter.
 * @param  pos		the position of the filtered character.
 * @return		the character if valid, otherwise 0.
 */
int ip_entry_filter( newtComponent field, void *data, int ch, int pos )
{
  int rv = ch;

  /*
   * Filtering rules:
   *   = non-numeric characters not allowed
   *   = only '.' allowed for separation character
   *   = ',' allowed for multiple entries
   *   = line-editing keys (BS, CR, etc ) preserved;
   */
  if( ( ch > 32 && ch < 48 && ch != 44 && ch != 46 ) ||
      ( ch > 57 && ch < 127 ) )	rv = 0;
  /*
   * Conversion rules:
   *   = a space character inserts separator
   */
  if( (char) ch == ' ' ) rv = (int) '.';

  /*
   * Alert user to error
   */
  if( ! rv ) newtBell();

  return rv;

} /* End of function: int ip_entry_filter(...); */


/**
 * Filters data entered into a HW Address entry field.
 *
 * @param  field	the entry field to filter.
 * @param  data		pointer to the data (not used).
 * @param  ch		the character to filter.
 * @param  pos		the position of the filtered character.
 * @return		the character if valid, otherwise 0.
 */
int hw_entry_filter( newtComponent field, void *data, int ch, int pos )
{
  int rv = ch;
  float loc;

  /*
   * Filtering rules:
   *   = non-hex characters not allowed
   *   = only ':' allowed for separation character
   *   = line-editing keys (BS, CR, etc ) preserved;
   */
  if( ( ch > 32 && ch < 48 ) || ( ch > 58 && ch < 65 ) ||
      ( ch > 70 && ch < 97 ) || ( ch > 102 && ch < 127 ) ) rv = 0;
  /*
   * Conversion rules:
   *   = a space character inserts separator
   */
  if( (char) ch == ' ' ) rv = (int) ':';
  /*
   * Positioning rules:
   *   = positions 2,5,8,11,14 requires separator
   */
  if( ch > 31 && ch < 127 && rv != 0 ) {
    switch( pos ) {
    case 2:
    case 5:
    case 8:
    case 11:
    case 14:
      if( rv != (int) ':' ) rv = 0;
      break;
    default:
      if( rv == (int) ':' ) rv = 0;
    }
  }

  /*
   * Alert user to error
   */
  if( ! rv ) newtBell();

  return rv;

} /* End of function: int ip_entry_filter(...); */


/**
 * Releases the screen from newt's control.
 */
void close_screen( void )
{

  newtFinished();

} /* End of function: void close_screen(void); */


/** -- end of file: screen.c -- **/
