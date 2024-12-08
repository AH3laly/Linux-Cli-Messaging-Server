/**
 * Execute defined commands
*/
void execute(char *, char *);

/**
 * Fallback function in case command not found
*/
void command_default(char *);

/**
 * Show Available commands
*/
void command_help(char *);

/**
 * Reboot machine
*/
void command_reboot(char *);

/**
 * Shutdown machine
*/
void command_shutdown(char *);