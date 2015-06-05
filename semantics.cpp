//
//  semantics.cpp
//  Sieve_P1
//
//  Created by Jordan Sieve on 12/05/14.
//  Copyright (c) 2014 Jordan Sieve. All rights reserved.
//

#include "semantics.h"

// Global used variables
vector <string> temp;
vector <string> scope;
int distanceFromTop = -1;
int scopeStart = 0;
int variableCounter = 0;
int labelCounter = 0;


// Code generation function
void codeGen(node *node, ofstream &outputFile) {
    
    string label;
    string label2;
    string argRight;
    
    if (node == NULL) {
        return;
    }
        
    // <program>
    if (node->label == "program()") {
        
		// Go to program() children
        codeGen(node->left, outputFile);
        codeGen(node->center, outputFile);
        
        // Put stop into output file
        outputFile << "\tSTOP\n";
        
        // Loop thru to add lines to clear variables
        for (int i = 0; i < temp.size(); i++) {
            outputFile << temp[i] << " 0\n";
        }
    }
    
    // <block>
    else if (node->label == "block()") {

		// Grab current scope size to track current place
        int start = (int)scope.size();
        
        codeGen(node->left, outputFile);
        codeGen(node->center, outputFile);

		// Send scope position to pop function to pop correct amount
		pop(start, outputFile);
    }
    
    // <var> and <mvars>
    else if (node->label == "var()" || node->label == "mvars()") {

		// Check for variable to save current scope point
		if (node->label == "var()") {
			scopeStart = (int)scope.size();
		}

        // Add token to stack
		push(node->storedTokens[0], outputFile);
        
        // Traverse children nodes
        codeGen(node->left, outputFile);
        codeGen(node->center, outputFile);
    }
    
    // <type>
    else if (node->label == "type()") {
       
       // var reached, return to var()
        return;
    }
    
    // <expr>
    else if (node->label == "expr()") {
        
        // Check if node has tokens stored in it
        if (node->storedTokens.size() > 0) {
        
            // If * star token was received
            if (node->storedTokens[0].tokenType == STARtk) {
                
                // Right hand evaluation
                codeGen(node->center, outputFile);

                // Copy new var name into argRight
                argRight = createVarName(VAR);
                
                // Add to asm output file
                outputFile << "\tSTORE " << argRight << "\n";
                
                // Left hand evalutation
                codeGen(node->left, outputFile);
                
                // Add to asm output file
                outputFile << "\tMULT " << argRight << "\n";
            }
            
            // If / slash token was received
            else if (node->storedTokens[0].tokenType == SLASHtk) {
                
                // Right hand evaluation
                codeGen(node->center, outputFile);

                // Copy new var name into argRight
                argRight = createVarName(VAR);
                
                // Add to asm output file
                outputFile << "\tSTORE " << argRight << "\n";
                
                // Left hand evalutation
                codeGen(node->left, outputFile);
                
                // Add to asm output file
                outputFile << "\tDIV " << argRight << "\n";
            }
        }
        
        else {
            
            // Go to children nodes
            codeGen(node->left, outputFile);
        }
    }
    
    // <T>
    else if (node->label == "T()") {
        
        // Check if node has tokens stored in it
        if (node->storedTokens.size() > 0) {
        
            // If + plus token was received
            if (node->storedTokens[0].tokenType == PLUStk) {
                
                // Right hand evaluation
                codeGen(node->center, outputFile);

                // Copy new var name into argRight
                argRight = createVarName(VAR);
                
                // Add to asm output file
                outputFile << "\tSTORE " << argRight << "\n";
                
                // Left hand evalutation
                codeGen(node->left, outputFile);
                
                // Add to asm output file
                outputFile << "\tADD " << argRight << "\n";
            }
            
            // If - minus token was received
            else if (node->storedTokens[0].tokenType == MINUStk) {
                
                // Right hand evaluation
                codeGen(node->center, outputFile);

                // Copy new var name into argRight
                argRight = createVarName(VAR);
                
                // Add to asm output file
                outputFile << "\tSTORE " << argRight << "\n";
                
                // Left hand evalutation
                codeGen(node->left, outputFile);
                
                // Add to asm output file
                outputFile << "\tSUB " << argRight << "\n";
            }
        }
        
        else {
            
            // Go to children nodes
            codeGen(node->left, outputFile);
        }
    }
    
    // <F>
    else if (node->label == "F()") {
        
        // Check if node has tokens stored in it
        if (node->storedTokens.size() > 0) {
        
            // Check for negation
            if (node->storedTokens[0].tokenType == MINUStk) {
                
                // Traverse lower to get value
                codeGen(node->left, outputFile);
                
                // Add negation to asm file
                outputFile << "\tMULT -1\n";
            }
        }
        
        // If no negation, traverse lower
        else {
            codeGen(node->left, outputFile);
        }
        
    }
    
    // <R>
    else if (node->label == "R()") {
        
        // Check if node has tokens stored in it
        if (node->storedTokens.size() > 0) {
        
            // Check for ID
            if (node->storedTokens[0].tokenType == IDtk) {
                
                // Check variable depth in stack
                distanceFromTop = find(node->storedTokens[0].token);
                
                // Add ID to stack to be used later
                if (distanceFromTop == -1) {

                    // Variable does not exist
                    error(node->storedTokens[0], false);
                }
                
                // Add to outputfile
                outputFile << "\tSTACKR " << distanceFromTop << "\n";
            }
            
            // Check for NUM
            else if (node->storedTokens[0].tokenType == NUMtk) {

                // NUM requires no validation, write to asm file
                outputFile << "\tLOAD " << node->storedTokens[0].token << "\n";
            }
        }

        // Check for (
        else {
            codeGen(node->left, outputFile);
        }
    }
    
    // <stats>
    else if (node->label == "stats()") {

        // Check children nodes
        codeGen(node->left, outputFile);
        codeGen(node->center, outputFile);
    }
    
    // <mStat>
    else if (node->label == "mStat()") {
        
        // Check children nodes
        codeGen(node->left, outputFile);
        codeGen(node->center, outputFile);
    }
    
    // <stat>
    else if (node->label == "stat()") {

        // Check children nodes
        codeGen(node->left, outputFile);
    }
    
    // <in>
    else if (node->label == "inStat()") {
        
        argRight = createVarName(VAR);
        
        // Get depth of IDtk
        distanceFromTop = find(node->storedTokens[0].token);
        
        if (distanceFromTop == -1) {
            error(node->storedTokens[0], false);
        }

        outputFile << "\tREAD " << argRight << "\n";
        outputFile << "\tLOAD " << argRight << "\n";
        outputFile << "\tSTACKW " << distanceFromTop << "\n";
    }
    
    // <out>
    else if (node->label == "outStat()") {

        // Evaluation expr
        codeGen(node->left, outputFile);
        
        // Copy new var name into argRight
        argRight = createVarName(VAR);
        
        // Add lines to asm out
        outputFile << "\tSTORE " << argRight << "\n";
        outputFile << "\tWRITE " << argRight << "\n";
    }
    
    // <if>
    else if (node->label == "ifStat()") {

        // Copy new label/var name into label/argRight
        label = createVarName(LABEL);
        argRight = createVarName(VAR);
        
        // Evaluate right hand side inside brackets
        codeGen(node->center, outputFile);
        
        outputFile << "\tSTORE " << argRight << "\n";
        
        // Evaluate left hand side inside brackets
        codeGen(node->left, outputFile);
        
        outputFile << "\tSUB " << argRight << "\n";
        
        // RO token option: =<
        if (node->storedTokens[0].tokenType == EQUALLESSTHANtk) {
            outputFile << "\tBRPOS " << label << "\n";
        }
        
        // RO token option: =>
        else if (node->storedTokens[0].tokenType == EQUALGREATERTHANtk) {
            outputFile << "\tBRNEG " << label << "\n";
        }
        
        // RO token option: ==
        else if (node->storedTokens[0].tokenType == EQUALEQUALtk) {
            outputFile << "\tBRPOS " << label << "\n";
            outputFile << "\tBRNEG " << label << "\n";
        }
        
        // RO token option: >
        else if (node->storedTokens[0].tokenType == GREATERTHANtk) {
            outputFile << "\tBRZNEG " << label << "\n";
        }
        
        // RO token option: <
        else if (node->storedTokens[0].tokenType == LESSTHANtk) {
            outputFile << "\tBRZPOS " << label << "\n";
        }
        
        // RO token option: =!=
        else if (node->storedTokens[0].tokenType == EQUALEXCLAMATIONEQUALtk) {
            outputFile << "\tBRZERO " << label << "\n";
        }
        
        // Go to block after if statement
        codeGen(node->right, outputFile);
        
        // Output asm language
        outputFile << "\n" << label << ": NOOP\n";
    }
    
    // <loop>
    else if (node->label == "loop()") {

        // Copy labels and vars
        label = createVarName(LABEL);
        label2 = createVarName(LABEL);
        argRight = createVarName(VAR);
        
        // Insert break point label
        outputFile << "\n" << label << ": NOOP\n";
        
        // Evaluate right side inside bracket
        codeGen(node->center, outputFile);
        
        // Insert asm storage
        outputFile << "\tSTORE " << argRight << "\n";
        
        // Evaluate left hand side inside bracket
        codeGen(node->left, outputFile);
        
        // Insert asm language
        outputFile << "\tSUB " << argRight << "\n";
        
        // RO token option: =<
        if (node->storedTokens[0].tokenType == EQUALLESSTHANtk) {
            outputFile << "\tBRPOS " << label2 << "\n";
        }
        
        // RO token option: =>
        else if (node->storedTokens[0].tokenType == EQUALGREATERTHANtk) {
            outputFile << "\tBRNEG " << label2 << "\n";
        }
        
        // RO token option: ==
        else if (node->storedTokens[0].tokenType == EQUALEQUALtk) {
            outputFile << "\tBRPOS " << label2 << "\n";
            outputFile << "\tBRNEG " << label2 << "\n";
        }
        
        // RO token option: >
        else if (node->storedTokens[0].tokenType == GREATERTHANtk) {
            outputFile << "\tBRZNEG " << label2 << "\n";
        }
        
        // RO token option: <
        else if (node->storedTokens[0].tokenType == LESSTHANtk) {
            outputFile << "\tBRZPOS " << label2 << "\n";
        }
        
        // RO token option: =!=
        else if (node->storedTokens[0].tokenType == EQUALEXCLAMATIONEQUALtk) {
            outputFile << "\tBRZERO " << label2 << "\n";
        }
        
        // Go to block after brackets of loop
        codeGen(node->right, outputFile);
        
        // Add asm information
        outputFile << "\tBR " << label << "\n";
        outputFile << "\n" << label2 << ": NOOP\n";
    }
    
    // <assign>
    else if (node->label == "assign()") {

        // Right hand evaluation of <expr>
        codeGen(node->left, outputFile);
        
        // Get variable depth
        distanceFromTop = find(node->storedTokens[0].token);
        
        // Add lines to asm file
        if (distanceFromTop == -1) {
            
            error(node->storedTokens[0], false);
        }

        // Print into asm file
        outputFile << "\tSTACKW " << distanceFromTop << "\n";
    }
    
    // <RO> - will never run, no seperate RO nodes
    else {
        
        // RO token information is part of the if/loop nodes
        return;
    }
    
}

void error(token &token, bool exists) {
    
    // Print appropriate response depending on if it exists or not
    if (exists == true) {
        cerr << "Error on line " << token.lineNumber << " - " << token.token << " is already declared in this block.\n";
    }
    else {
        cerr << "Error on line " << token.lineNumber << " - " << token.token << " not declared in this block.\n";
    }
    
    // Exit program
    exit(1);
}

// Add variable to stack
void push(token &token, ofstream &outputFile) {
    
    // Loop thru elements to check if variables already exist in scope
    for (int i = scopeStart; i < scope.size(); i++) {

        // Check elements of vector
        if (scope[i] == token.token) {

			// Print error message
            cerr << token.token << " exists in scope. Defined on line " << token.lineNumber;

            // Exit program with error
            exit(1);
        }
    }

    // Add variable name to stack
    scope.push_back(token.token);

	// Add push command to output file
	outputFile << "\tPUSH\n";
}

// Remove variable from stack
void pop(int scopeStart, ofstream &outputFile) {

	// Loop thru stack and pop stack
	for (int i = (int)scope.size(); i > scopeStart; i--) {

		// Remove last element from stack
		scope.pop_back();

		// Add pop command to output file
		outputFile << "\tPOP\n";
	}

    // Remove last variable from stack
//    variableStack.pop_back();
}

// Used for find variable in stack
int find(string varName) {

	// Loop thru the scope
    for (int i = (int)(scope.size() - 1); i > -1; i--) {

		// Check for matching variable
		if (scope[i] == varName) {

			// If variable is found, return its place
			return (int)(scope.size() - 1 - i);
		}
	}
    
    // Check compiler in check
    return -1;
}

// Used for creating variable names in order
string createVarName(nameType varType) {
    
    ostringstream varName;
    
    // Assign incrementing names to variables and labels
    if (varType == VAR) {
        varName << "V" << variableCounter++;
        temp.push_back(varName.str());
    }
    
    else {
        varName << "L" << labelCounter++;
    }

    return varName.str();
}
