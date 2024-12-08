typedef struct {
    char* name;
    void (*ptr)(char*);
    char* description;
} command;

command commands[] = {
    {"help", &command_help, "Show available commands."},
    {"reboot", &command_reboot, "Reboot the server."},
    {"shutdown", &command_shutdown, "Shutdown the server."}
};

void execute(char *commandName, char *result){
    printf("Executing Command: %s\n", commandName);
    int length = sizeof(commands) / sizeof(commands[0]);
    for(int i = 0; i < length; i++){
        if(equals(commandName, commands[i].name)){
            (commands[i].ptr)(result);
            strcat(result, "\n");
            return;
        }
    }
    command_default(result);
    strcat(result, "\n");
}

void command_default(char * result){
    strcpy(result, "Command Not Found.");
}

void command_help(char * result){
    int length = sizeof(commands) / sizeof(commands[0]);
    for(int i = 0; i < length; i++){
        strcat(result, "\n");
        strcat(result, commands[i].name);
        strcat(result, ": ");
        strcat(result, commands[i].description);
    }
}

void command_reboot(char * result){
    strcpy(result, "Rebooting...");
}

void command_shutdown(char * result){
    strcpy(result, "Shutting Down...");
}