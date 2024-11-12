# Arduino_Mega2560_Command_Line

## Libraries
    
    Using Arduino Wire Library (for 'i2cscan' command)
		
## Board / Connections
    
    No connections other than USB serial
		
## Command Line's three major components:
    
    * cmd_table[] - array of COMMAND_ITEM data structures, containing:
       command name
       comment concerning the command (usage)
       minimal argument count (usually just 1)
       pointer to function to execute the command
    
    * cl_parseArgcArgv() - break the command string, with arguments, into "words"
    
    * cl_process_buffer() - search cmd_table for "command" supplied, and if found,
       determine if enough parameters are provided.  Call associated function.
	
## Notes
    
    The goal of this project is to provide a printf(), command-line framework,
    allowing many "commands" to be created, each supporting optional arguments.
    See function, cl_add(), which demonstrates a simple multi-argument command.
