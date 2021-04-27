/*
**  h e l p . c
**  -----------
**
**  Project: dhcp-conf (DHCPD Configurator)
**   Author: Vee Schade, aka Tony Cook, Vee Schade Int'l
**     Date: August 18, 1999
**
**  Description: This file provides the help and 'about' screens
**	for dhcp-conf.  All interfaces are done with RedHat
**	Software's 'newt' toolkit.
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


/****************************/
/*** Function definitions ***/
/****************************/
/**
 * Displays splash screen for dhcp-conf.
 */
void about( void )
{
  char *buff;
  char text[] = "                D H C P D   C o n f i g u r a t o r\n"
    "                ===================================\n"
    "\n"
    " Author: Vee Schade, aka Tony Cook, Vee Schade Int'l\n"
    " E-mail: vschade@mindless.com\n"
    "Version: %s\n"
    "\n\n"
    "This application assists with creating /etc/dhcpd.conf for the\n"
    "Internet Software Consortium's DHCP version 2 distribution.\n"
    "\n"
    "LICENSE:\n"
    "   This application is released under the terms of the\n"
    "   GNU Public License (GPL).  See the file 'COPYING' for\n"
    "   details.  No warranty of fitness for any kind is alleged\n"
    "   by the author for this product.\n";

  newtComponent form,			/* Form for this screen */
    tb_About,				/* About Textbox */
    bt_Exit;				/* Exit about screen */

  buff = malloc( strlen( text ) + strlen( VERSION ) + 1 );
  sprintf( buff, text, VERSION );

  /*
   *-- Create the form
   *
   * First the textbox
   */
  tb_About = newtTextbox( 1, 2, 64, 16, 0 );
  newtTextboxSetText( tb_About, buff );
  /*
   * Now the control button
   */
  bt_Exit = newtCompactButton( 31, 19, " OK " );

  /*
   * Display and run the form
   */
  form = newtForm( NULL, NULL, 0 );
  newtFormAddComponents( form, tb_About, bt_Exit, NULL );
  /*
   * Open the form window
   */
  newtCenteredWindow( 70, 20, "DHCP Configurator - About box" );
  newtRunForm( form );

  /*
   * Clean-up and return
   */
  newtFormDestroy( form );

} /* End of function: void about(void); */

  
/**
 * Displays help for the main menu.
 */
void help_main_menu( void )
{
  char text[] = "                D H C P D   C o n f i g u r a t o r\n"
    "                          Main Menu Help\n"
    "                ===================================\n"
    "\n"
    "The main menu presents a series of three selection buttons and\n"
    "four control buttons.\n"
    "\n"
    "SELECTION BUTTONS\n"
    "-----------------\n"
    "Common settings: To view and/or change the settings common to\n"
    "both Fixed and Dynamic IP clients select the button labeled\n"
    "\"Common\".  Common settings include default/max lease times,\n"
    "domain name, router(s), name-server(s), etc.\n"
    "\n"
    "Fixed host settings: Select the button labeled \"Fixed\" to\n"
    "view and/or change hosts with fixed (static) IPs.  Examples of\n"
    "\"Fixed\" hosts include, web servers, e-mail servers, routers,\n"
    "THIS dhcp server <G>, etc.\n"
    "\n"
    "Subnet settings: Typically, small-to-medium sized LANs require\n"
    "a single subnet (range of assignable IPs) for it's dynamic-IP\n"
    "clients.  Select \"Subnet\" to assign this range.\n"
    "\n"
    "CONTROL BUTTONS\n"
    "---------------\n"
    " \"Save\": Save current settings to /etc/dhcpd.conf and exit.\n"
    "\n"
    " \"Help\": Display this help information.\n"
    "\n"
    "\"About\": Display the about box.\n"
    "\n"
    " \"Exit\": Exit without saving.\n"
    "\n";

  help( text, "Main Menu" );

} /* End of function: void help_main_menu(void); */


/**
 * Displays help information for Common Settings form.
 */
void help_common_settings( void )
{
  char text[] = "                D H C P D   C o n f i g u r a t o r\n"
    "                          Common Settings\n"
    "                ===================================\n"
    "\n"
    "Common settings for DHCP client connections.  These settings\n"
    "primarily provide information to the clients about their\n"
    "network and lease-time information to the server.  This screen\n"
    "is composed of 7 entry fields and 4 control buttons.\n"
    "\n"
    "ENTRY FIELDS\n"
    "------------\n"
    "Minimum lease time: This field tells the server the minimum\n"
    "amount of time to keep IP's assigned before the client can\n"
    "refresh it's IP number lease.  The default for this field is\n"
    "600 seconds (or 10 minutes).\n"
    "\n"
    "Maximum lease time: This field tells the server to maximum\n"
    "amount of time IP's may stay assigned until the client must\n"
    "refresh it's IP number lease.  The default for this field is\n"
    "7200 seconds (or 120 minutes).\n"
    "\n"
    "Domain name: This field provides DHCP clients with the domain\n"
    "name of their network.  This field does not have a default\n"
    "value and is limited to 99 characters (WARNING: data beyond 99\n"
    "characters is cropped!).\n"
    "\n"
    "Subnet-mask: This field provides DHCP clients with the netmask\n"
    "of their network.  The default for this field is 255.255.255.0\n"
    "as most LANs are likely to be Class C private networks.\n"
    "\n"
    "Broadcast address: This field provides DHCP clients with the\n"
    "broadcast IP of their network.  The default for this field is\n"
    "192.168.100.255 as most LANs are likely to be Class C private.\n"
    "networks.\n"
    "\n"
    "Router (gateway) IP(s): This field provides DHCP clients with\n"
    "the router (\"default gateway\") address(es) of their network.\n"
    "Multiple addresses may be specified comma-separated (ie.\n"
    "router1_ip, router2_ip, router3_ip); upto 3 addresses are\n"
    "supported.  The default for this field is 192.168.100.1 as most\n"
    "LANs are likely to be Class C private networks.\n"
    "\n"
    "Name Server (DNS) IP(s): This field provides DHCP clients with\n"
    "the name-server address(es) of their network.  Multiple\n"
    "addresses may be specified comma-separated (see 'Router IP'\n"
    "above); upto 3 addresses are supported.  The default for this\n"
    "field is the same as for 'Router IP' above.\n"
    "\n"
    "CONTROL BUTTONS\n"
    "---------------\n"
    " \"Keep\": Keep current settings for saving to /etc/dhcpd.conf\n"
    "and return to main menu.\n"
    "\n"
    "\"Clear\": Clear all fields of data.\n"
    "\n"
    " \"Help\": Display this help information.\n"
    "\n"
    " \"Exit\": Return to main menu discarding changes.\n"
    "\n";

  help( text, "Common Settings" );

} /* End of function: void help_common_settings(void); */


/**
 * Displays help information for Fixed Host Settings form.
 */
void help_fixed_settings( void )
{
  char *buff;
  char text[] = "                D H C P D   C o n f i g u r a t o r\n"
    "                          Fixed Settings\n"
    "                ===================================\n"
    "\n"
    "Fixed-host settings for DHCP client connections:  These\n"
    "settings reserve fixed (static) IP's for those hosts needing\n"
    "them.  Typical \"fixed-ip hosts\" include internet servers\n"
    "reachable via DNS lookups, e.g.: web servers, mail servers,\n"
    "routers, etc.  This screen is composed of 3 entry fields, an\n"
    "item selectable listbox, and 4 control buttons.\n"
    "\n"
    "ENTRY FIELDS\n"
    "------------\n"
    "New entry hostname..: This field provides the hostname of the\n"
    "fixed-ip host.  This field is combined with the domain name\n"
    "setting in the Common Settings form to create a fully qualified\n"
    "domain name (FQDN).  Input is limited to %i alpha-numeric\n"
    "characters which should be sufficient for most needs.\n"
    "\n"
    "New entry HW address: This field provides the hardware address\n"
    "of the network interface card (NIC) for this host.  Input is\n"
    "limited to 17 characters: 5 colon separated hex byte pairs in\n"
    "accordance with standard notation for NIC addresses.\n"
    "\n"
    "New entry IP address: This field provides the internet protocol\n"
    "(IP) fixed address of this host.  Input is limited to 15\n"
    "characters: 3 period ('dot') separated digital numbers in the\n"
    "range of 0 - 255 each in accordance with standard notation for\n"
    "IP addresses.  Pressing <CR> in this field adds the new entry\n"
    "to the list, clears the previous two entry fields, and places\n"
    "the cursor in the hostname entry field for the next host.  The\n"
    "previous input to this field is maintained for faster entry\n"
    "(editing) of the next host's IP.\n"
    "\n"
    "SELECTABLE LISTBOX\n"
    "------------------\n"
    "This field displays the current (and newly entered ;) fixed-ip\n"
    "hosts configured with this form.  Fields may be selected for\n"
    "editing or deleting by moving the highlight bar with the cursor\n"
    "(arrow) keys.  Pressing <CR> on a highlighted entry inserts the\n"
    "highlighted information into the entry fields for editing.\n"
    "Pressing the <Delete> control button removes the highlighted\n"
    "entry from the current list.\n"
    "\n"
    "CONTROL BUTTONS\n"
    "---------------\n"
    "  \"Keep\": Keep current settings for saving to /etc/dhcpd.conf\n"
    "and return to main menu.\n"
    "\n"
    "\"Delete\": Remove the currently selected list item.\n"
    "\n"
    "  \"Help\": Display this help information.\n"
    "\n"
    "  \"Exit\": Return to main menu discarding changes.\n"
    "\n";

  buff = malloc( strlen( text ) + HOSTNAME_LEN + 1 );
  sprintf( buff, text, HOSTNAME_LEN );
  help( buff, "Fixed-hosts Settings" );
  free( buff );

} /* End of function: void help_fixed_settings(void) */


/**
 * Displays help information for Subnets Settings form.
 */
void help_subnet_settings( void )
{
  char *buff;
  char text[] = "                D H C P D   C o n f i g u r a t o r\n"
    "                           Subnet Settings\n"
    "                ===================================\n"
    "\n"
    "Subnet(s) settings for DHCP client connections:  These\n"
    "settings establish subnet (dynamic) IP ranges for those\n"
    "systems configured for dynamic IP assignments.  Typically\n"
    "these include local workstations and/or remote access client\n"
    "systems.  Currently, a maximum of %i 'ranges' are supported\n"
    "by this tool.  Any number of supported 'ranges' may be assigned\n"
    "to a subnet, though every 'subnet' must have a minimum of 1\n"
    "subnet 'ranges' assigned to it.  This screen is composed of 3\n"
    "entry fields, an item selectable listbox, and 4 control buttons.\n"
    "\n"
    "ENTRY FIELDS\n"
    "------------\n"
    "NOTE - In all of the following entry fields, input is limited\n"
    "to 15 characters: 3 period ('dot') separated digital numbers in\n"
    "the range of 0 - 255 each in accordance with standard notation\n"
    "for IP addresses.\n"
    "\n"
    "New range subnet....: This field provides the current subnet\n"
    "for the following range input.  All subnets currently use the\n"
    "global netmask assigned in the Common Settings form.  All new\n"
    "ranges must have a corresponding subnet entry including\n"
    "multiple ranges assigned to a single subnet.  The previous\n"
    "input to this field is maintained for faster entry of multiple\n"
    "IP ranges per subnet (the norm in small to medium LAN sizes).\n"
    "\n"
    "New range start IP..: This field provides the starting address\n"
    "of the new dynamic IP range to be assigned to this subnet.\n"
    "\n"
    "New range end IP....: This field provides the ending address\n"
    "of the new dynamic IP range to be assigned to this subnet.\n"
    "Pressing <CR> in this field adds the new entry to the list,\n"
    "clears the range entry fields, and places the cursor in the\n"
    "range start entry field for entering the next range to add to\n"
    "this subnet.  If a new subnet is desired, pressing <CURSOR-UP>\n"
    "will place the cursor in the subnet field for editing a new\n"
    "subnet.\n"
    "\n"
    "SELECTABLE LISTBOX\n"
    "------------------\n"
    "This field displays the current (and newly entered ;) subnet(s)\n"
    "and their ranges configured with this form.  Fields may be\n"
    "selected for editing or deleting by moving the highlight bar\n"
    "with the cursor (arrow) keys.  Pressing <CR> on a highlighted\n"
    "entry inserts the highlighted information into the entry fields\n"
    "for editing.  Pressing the <Delete> control button removes the\n"
    "highlighted entry from the current list.\n"
    "\n"
    "CONTROL BUTTONS\n"
    "---------------\n"
    "  \"Keep\": Keep current settings for saving to /etc/dhcpd.conf\n"
    "and return to main menu.\n"
    "\n"
    "\"Delete\": Remove the currently selected list item.\n"
    "\n"
    "  \"Help\": Display this help information.\n"
    "\n"
    "  \"Exit\": Return to main menu discarding changes.\n"
    "\n";

  buff = malloc( strlen( text ) + MAX_SUBNETS + 1 );
  sprintf( buff, text, MAX_SUBNETS );
  help( buff, "Subnet(s) Settings" );
  free( buff );

} /* End of function: void help_subnet_settings(void) */


/** -- end of file: help.c -- **/
