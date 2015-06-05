//
//  main.cpp
//  Sieve_P1
//
//  Created by Jordan Sieve on 11/08/14.
//  Copyright (c) 2014 Jordan Sieve. All rights reserved.
//

#include "scanner.h"
#include "parser.h"
#include "semantics.h"

int main(int argc, const char * argv[]) {
    
    // Declared variables used
    token token;
    ifstream inFile;
    ofstream tempFile, outFile;
    string noExtFileInput, buffer, outputFileName;
    node *parseTree;
    bool typedSubmission = false;
    
    // Test for too many arguments
    if (argc > 2) {
        cerr << "Unexpected arguments. Try again. Usage: comp [file]\n";
        
        // Exit with error status
        return 1;
    }
    
    // Send file in as argument
    else if (argc == 2) {
        
        // Check for file extension and add .lan if not included
        noExtFileInput = argv[1];
        
        // Check if file extension is .lan
        if ((noExtFileInput.length() > 4) && (noExtFileInput.substr(noExtFileInput.length() - 4) == ".lan")) {
            outputFileName = noExtFileInput.length() - 4;
            inFile.open(argv[1]);
        }
        
        // Add .lan to file name if not explicit
        else {
            outputFileName = noExtFileInput;
            noExtFileInput = noExtFileInput + ".lan";
            inFile.open(noExtFileInput.c_str());
        }
        
        // Handle an error if the file does not open/exist
        if (!inFile) {
            cerr << "Error opening file\n";
            exit(1);
        }
        
        // Copy file into a temp file and remove the comments
        else {
            
            // Create temp file
            tempFile.open("tempfile.dat");
            
            // Fill the file with stdin input
            while (getline(inFile, buffer)) {
                
                // Loop over line - remove comments and convert to lowercase
                for (int i = 0; i < buffer.length(); i++) {
                    
                    // Skip over the comments and don't add them to file to be processed
                    if ((i < buffer.length()-1 && buffer[i] == '/' && buffer[i+1] == '/') || buffer[i] == '#') {
                        break;
                    }
                    
                    // Convert uppercase letters to lowercase
                    if (isupper(buffer[i])) {
                        buffer[i] = tolower(buffer[i]);
                    }
                    
                    // Add each character that passes tests to temp file
                    tempFile << buffer[i];
                }
                
                // Input newline character at end of added line
                tempFile << "\n";
            }
            
            // Close files
            inFile.close();
            tempFile.close();
        }
    }
    
    // If no arguments are provided then allow user to enter
    else {
        
        // Prompt user
        cout << "Enter program to be compiled: (A blank line starts program)\n";
        
        // Create temp file
        tempFile.open("tempfile.dat");
        
        // Change bool over for output file naming
        typedSubmission = true;
        
        // Fill the file with stdin input
        while (getline(cin, buffer)) {
            
            // Check for blank line
            if (buffer.length() == 0) {
                break;
            }
            
            // Add lines to temp file as inputted
            else {

                // Loop over line - remove comments and convert to lowercase
                for (int i = 0; i < buffer.length(); i++) {
                    
                    // Skip over the comments and don't add them to file to be processed
                    if ((i < buffer.length()-1 && buffer[i] == '/' && buffer[i+1] == '/') || buffer[i] == '#') {
                        break;
                    }
                    
                    // Convert uppercase letters to lowercase
                    if (isupper(buffer[i])) {
                        buffer[i] = tolower(buffer[i]);
                    }
                    
                    // Add each character that passes tests to temp file
                    tempFile << buffer[i];
                }
                
                // Input newline character at end of added line
                tempFile << "\n";
            }
        }
        
        // Close temp file
        tempFile.close();
    }

    // Get first token from file
    token = scanner();
    
    // Go to parser
    parseTree = program(token);
    
    if (token.tokenType == EOFtk) {
        cout << "Parse OK\n";
        remove("tempfile.dat");
    }
    else {
        cerr << "Error parsing\n";
        error("EOFtk", token);
    }
    
    // Not used for P3
//      cout << "\nPreorder Traversal:\n";
//      traversePreorder(parseTree);
        
    // Open outfile for writing
    if (typedSubmission == true) {
        outFile.open("lan.out");
    }
    else {
        // Correctly name output file based on instructions
        outFile.open(string(outputFileName + ".asm").c_str());
    }
        
    // Go to code generation
    codeGen(parseTree, outFile);
    
    // Display success
    cout << "Code successfully generated.\n";
    
    // Close file after writing asm langauge
    outFile.close();
    
    return 0;
}

void traversePreorder(node *traversePTR) {
    
    if (traversePTR == NULL)
        return;
    
    // Print out function then info in vector
    cout << traversePTR->label << "\n";
    for (int i = 0; i < traversePTR->tokenInfo.size(); i++) {
        
        // Put token info at same spacing
        for (int j = 0; j < traversePTR->depthCount; j++) {
            cout << "  ";
        }
        
        cout << traversePTR->tokenInfo[i];
    }
    
    // Recursively call back functions
    traversePreorder(traversePTR->left);
    traversePreorder(traversePTR->center);
    traversePreorder(traversePTR->right);
}


