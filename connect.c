#include "includes/headers.h"
#include "includes/functions.c"
#include "includes/commands.c"

int main(int argc, char **argv){

    int network_socket;
    char server_response[1024];
    char client_message[1024] = "hello";

    // Create the Socket
    network_socket = socket(AF_INET, SOCK_STREAM, 0);

    // Define Connection parameters    
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8060);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // Connect
    int connected = connect(network_socket, (struct sockaddr *) &server_address, sizeof(server_address));
    if(connected == -1){
        printMessage(MESSAGE_CONNECTION_ERROR, 1);
        return 0;
    } else {
        printMessage(MESSAGE_CONNECTED, 1);
    }
    printMessage("Hello", 1);

    // ################ Authentication Mode ################    
    while(1){
        
        // Receive message from server
        recv(network_socket, server_response, sizeof(server_response), 0);

        if(equals(server_response, COMMAND_AUTH_REQ) || equals(server_response, COMMAND_AUTH_ERROR)){
            
            char login_username[256], login_password[256], token[512]; 
            clean(token);
            
            if(equals(server_response, COMMAND_AUTH_ERROR)){
                printMessage(MESSAGE_AUTHENTICATION_FAILED, 1);
            }

            // Ask for Username and Password
            printMessage(MESSAGE_ENTER_USERNAME, 0);
            askUserInput(login_username);
            printMessage(MESSAGE_ENTER_PASSWORD, 0);
            askPassword(login_password);
            
            // Build Token based on the Username and Password
            buildToken(token, login_username, login_password);

            // Send Authentication token to Server
            send(network_socket, token, sizeof(token), 0);

        } else if(equals(server_response, COMMAND_AUTH_OK)){
            printMessage(MESSAGE_AUTHENTICATED, 1);
            break;
        }

        clean(server_response);
    }
    
    // ################ Command Mode ################
    while(!equals(client_message, COMMAND_CLOSE)){

        // Ask the user for Command
        printMessage(MESSAGE_COMMAND, 0);
        askUserInput(client_message);

        // Send Command to Server
        send(network_socket, client_message, sizeof(client_message), 0);

        // Read Server Response
        recv(network_socket, server_response, sizeof(server_response), 0);
        printMessage(server_response, 0);

        if(equals(client_message, COMMAND_CLOSE)){
            close(network_socket);
            break;
        }
    }

    return 0;
}