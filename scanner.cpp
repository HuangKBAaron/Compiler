//
//  scanner.cpp
//  Sieve_P1
//
//  Created by Jordan Sieve on 10/20/14.
//  Copyright (c) 2014 Jordan Sieve. All rights reserved.
//
//  Run a scanner acroess a file to tokenize input language
//

#include "scanner.h"

token scanner() {
    
    // Declare needed variables
    int currentState = 0;
    int nextState = 0;
    int stateTableColumn = 0;
    static int lineNumber = 1; // Static to initialize once
    static streamoff currentPosition = 0; // Static to initialize once
    string tokenName;
    char lookAhead;
    token currentToken;
    ifstream inFile;
    
    // Open file and read from last spot
    inFile.open("tempfile.dat");
    inFile.seekg(currentPosition);
    
    // Grab the first character from the file
    lookAhead = inFile.get();
    
    // Grab type of character
    stateTableColumn = lookUpTable(lookAhead);
    
    // Loop thru file until an end state is reached
    while (currentState != -1) {
        
        // Check for new lines and spaces
        while (lookAhead == '\n' || isspace(lookAhead)) {
        
            // Increment the line number counter
            if (lookAhead == '\n') {
                lineNumber++;
            }
            
            // Reset state to input
            currentState = 0;
            
            // Skip past new line character
            lookAhead = inFile.get();
            stateTableColumn = lookUpTable(lookAhead);
        }
        
        // Get the next state from table
        nextState = stateTable[currentState][stateTableColumn];
        
        // Check for errors
        if (nextState == -2) {
            cerr << "Error on line " << lineNumber << ". Invalid input: " << lookAhead << endl;
            exit(1);
        }
        
        // Check for end of file
        else if (nextState == -9) {
            
            // Get EOF line number correct to fix incorrect numbering
            lineNumber--;
            
            // Send EOF token
            currentToken.token = "EOF";
            currentToken.tokenType = EOFtk;
            currentToken.lineNumber = lineNumber;
            currentPosition = inFile.tellg();
            inFile.close();
            return currentToken;
        }
        
        // Test for other states
        else if (nextState != -1) {
            
            // Move current state up
            currentState = nextState;

            // Add characters to string
            if (!isspace(lookAhead))
                tokenName += lookAhead;
            
            if (inFile.peek() == '\n' || isspace(lookAhead)) {
                nextState = -1;
                break;
            }
            
            // Get the next character
            lookAhead = inFile.get();
            
            stateTableColumn = lookUpTable(lookAhead);
            nextState = stateTable[currentState][stateTableColumn];
            
            // If following step is exit status and non WS then remove last character added
            if (nextState == -1 && !isspace(lookAhead)) {
                inFile.unget();
            }
        }
        
        // Check for end state
        if (nextState == -1) {
            
            // Save token name is not added yet
            if (tokenName.length() == 0) {
                tokenName = lookAhead;
            }
            
            // Change current state to exit loop
            currentState = -1;
        }
    }

    // *** Think about better soluton here *** //
    // Go thru and assign ID and NUM token type
    for (int i = 0; i < 41; i++) {
        if (isalpha(tokenName[0])) {
            currentToken.tokenType = IDtk;
        }
        else if (isdigit(tokenName[0])) {
            currentToken.tokenType = NUMtk;
        }
    }
    
    // Compare states to normal token types
    for (int i = 0; i < 41; i++) {
        if (keywordLookUpTable[i].token == tokenName) {
            currentToken.tokenType = keywordLookUpTable[i].tokenType;
        }
    }
    
    // Add other info to the token
    currentToken.token = tokenName;
    currentToken.lineNumber = lineNumber;
    
    currentPosition = inFile.tellg();
    inFile.close();
    return currentToken;
}


// Function to see what letter it is
int lookUpTable(char currentCharacter) {
    
    // Holds the character type and reference to the state table
    typedef struct characterInput {
        int key;
        int stateTableColumn;
    } characterInput;
    
    characterInput lookUpTable[19] = {
        
        // 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18    19  20 21  22    23
        // = < > ! : + - * / %  .  (  )  ,  {  }  ;  [  ] alpha num WS EOF other
        
        {'=', 0 },
        {'<', 1 },
        {'>', 2 },
        {'!', 3 },
        {':', 4 },
        {'+', 5 },
        {'-', 6 },
        {'*', 7 },
        {'/', 8 },
        {'%', 9 },
        {'.', 10},
        {'(', 11},
        {')', 12},
        {',', 13},
        {'{', 14},
        {'}', 15},
        {';', 16},
        {'[', 17},
        {']', 18},
    };
    
    // Test to see if character a character
    if (isalpha(currentCharacter)) {
        return 19;
    }
    
    // Test to see if character a number
    else if (isdigit(currentCharacter)) {
        return 20;
    }
    
    // Test to see if character is a space
    else if (isspace(currentCharacter)) {
        return 21;
    }
    
    // Test to see if character is EOF
    else if (currentCharacter == EOF) {
        return 22;
    }
    
    // Check to see if character is in look up table
    else {
        
        // Iterate thru the look up table options
        for (int i = 0; i < 19; i++) {
            
            // Compare the input to the look up table options
            if (lookUpTable[i].key == currentCharacter) {
                return lookUpTable[i].stateTableColumn;
            }
        }
    }
    
    // If not a alphabet, number, space, EOF, or look up table - finish
    return 23;
}
