#include "includes/headers.h"
#include "includes/functions.c"
#include "includes/commands.c"

char server_message[1024] = "";
char client_message[1024] = "";

void generateAuthToken(char * token, int argc, char **argv){
    char auth_username[512]= "";
    char auth_password[512] = "";

    if(argc < 3){
        printMessage(MESSAGE_DEFINE_USERNAME, 0);
        askUserInput(auth_username);
        printMessage(MESSAGE_DEFINE_PASSWORD, 0);
        askPassword(auth_password);
    } else {
        strcpy(auth_username, argv[1]);
        strcpy(auth_password, argv[2]);
    }
    buildToken(token, auth_username, auth_password);
}

void requireAuthentication(char * token, int client_socket){
    
    strcpy(server_message, COMMAND_AUTH_REQ);
    send(client_socket, server_message, sizeof(client_message), 0);

    // Wait Until User Authenticates
    while(1){
        
        // Wait for client message
        recv(client_socket, client_message, sizeof(client_message), 0);

        if(equals(token, client_message)){
            strcpy(server_message, COMMAND_AUTH_OK);
            send(client_socket, server_message, sizeof(server_message), 0);
            printMessage(MESSAGE_AUTHENTICATED, 1);
            break;
        } else {
            strcpy(server_message, COMMAND_AUTH_ERROR);
            send(client_socket, server_message, sizeof(server_message), 0);
            printMessage(MESSAGE_AUTHENTICATION_FAILED, 1);
        }
    }
}

int main(int argc, char **argv){
    
    char token[512] = "";
    char client_message[1024] = "";
    char command_result[1024] = "";

    // Ask user to define server security
    generateAuthToken(token, argc, argv);

    // Create the Socket
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    // Define Connection parameters    
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8060);
    server_address.sin_addr.s_addr = INADDR_ANY;

    bind(server_socket, (struct sockaddr *) &server_address,sizeof(server_address));
    listen(server_socket, 5);
    printMessage(MESSAGE_SERVER_LISTENING, 1);

    int client_socket;

    // Wait for any incoming connection
    while(1){

        // Ready to Accept Connection
        client_socket = accept(server_socket, NULL, NULL);

        // Require Client to Authenticate
        requireAuthentication(token, client_socket);

        // ##### User Authenticated, Move to Command Mode #####

        printMessage(MESSAGE_READY_FOR_COMMANDS, 1);

        // Read Commands from Client
        clean(client_message);

        while(1){

            // Receive Message from client
            recv(client_socket, client_message, sizeof(client_message), 0);

            if(!isEmpty(client_message)){

                if(equals(client_message, COMMAND_CLOSE)){
                    // Close command requested by the client
                    printMessage(MESSAGE_BYE, 1);
                    strcpy(command_result, MESSAGE_BYE);
                    strcat(command_result, "\n");
                    send(client_socket, command_result, sizeof(command_result), 0);
                    close(client_socket);
                    break;
                } else {
                    // Execute command requested by the client
                    execute(client_message, command_result);
                    // Send the command result back to the client
                    send(client_socket, command_result, sizeof(command_result), 0);
                }

                // Clean Buffer
                clean(client_message);
            }
        }
    }
    return 0;
}