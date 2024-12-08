void askUserInput(char *userInput){
    scanf("%s", userInput);
}

void askPassword(char *password){
    char *passwordInput = getpass("");
    strcpy(password, passwordInput);
}

void printMessage(char * messageToPrint, int addNewLine){
    printf("%s", messageToPrint);
    if(addNewLine == 1){
        printf("\n");
    }
}

bool isEmpty(char *text){
    int length = strlen(text);
    char singleChar;
    for(int i = 0; i < length; i++){
        singleChar = *(text + i);
        if(singleChar != 0 || singleChar != '\0' || singleChar != '\n' || singleChar != '\r'){
            return false;
        }
    }
    return true;
}

void clean(char * text){
    for(int i = 0; i < strlen(text); i++){
        text[i] = '\0';
    }
}

void buildToken(char* token, char * username, char * password){
    strcat(token, username);
    strcat(token, password);
}

bool equals(char * text1, char * text2){
    return strcmp(text1, text2) == 0;
}