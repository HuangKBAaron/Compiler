//
//  parser.cpp
//  Sieve_P1
//
//  Created by Jordan Sieve on 11/08/14.
//  Copyright (c) 2014 Jordan Sieve. All rights reserved.
//

#include "parser.h"

node* program(token& token) {
    
    // Create new node for tree
    node *newNode = nodeMaker("program()");
    
    // <program> -> <var> do <block> return
    newNode->left = var(token);
    
    if (token.tokenType == DOtk) {
        
        token = scanner();
        
        // Put block token in left leaf if no variables are provided
        if (newNode->left == NULL) {
            newNode->left = block(token);
        }
        
        else {
            newNode->center = block(token);
        }
        
        if (token.tokenType == RETURNtk) {
            
            token = scanner();
            
            if (token.tokenType == EOFtk) {
                
                return newNode;
            }
            
            else {
                error("EOFtk", token);
                exit(1);
            }
        }
        
        else {
            // Print an error message
            error("RETURNtk", token);
            exit(1);
        }
    }
 
    else {
        // Print an error message
        error("DOtk", token);
        exit(1);
    }
}

node* block(token& token) {
    
    // Create new node for tree
    node *newNode = nodeMaker("block()");

    // <block> -> start <var> <stats> finish
    if (token.tokenType == STARTtk) {
        
        token = scanner();
        newNode->left = var(token);
        newNode->center = stats(token);
        
        if (token.tokenType == FINISHtk) {
            
            token = scanner();
            return newNode;
        }
        
        else {
            // Print an error message
            error("FINISHtk", token);
            exit(1);
        }
    }
    else {
        // Print an error message
        error("STARTtk", token);
        exit(1);
    }
}

node* var(token& token) {
    
    // Create new node for tree
    node *newNode = nodeMaker("var()");

    // <var> -> <type> ID <mvars> .
    if (token.tokenType == VARtk) {
        
        // token = scanner();
        newNode->left = type(token);
        
        if (token.tokenType == IDtk) {
            
            // Add token info to the nodes
            addTokenInfo(newNode, token);
            
            token = scanner();
            newNode->center = mvars(token);
            
            if (token.tokenType == PERIODtk) {
                
                token = scanner();
                return newNode;
            }
            
            else {
                // Print an error message
                error("PERIODtk", token);
                exit(1);
            }
        }
        
        else {
            // Print an error message
            error("IDtk", token);
            exit(1);
        }
    }

    // <var> -> empty
    else {
        return NULL;
    }
}

node* type(token& token) {
    
    // Create new node for tree
    node *newNode = nodeMaker("type()");
    
    token = scanner();
    return newNode;
}

node* mvars(token& token) {
    
    // Create new node for tree
    node *newNode = nodeMaker("mvars()");
    
    // <mvars> -> : ID <mvars>
    if (token.tokenType == COLONtk) {
        
        token = scanner();
        
        if (token.tokenType == IDtk) {
            
            // Add token info to the nodes
            addTokenInfo(newNode, token);
            
            token = scanner();
            newNode->left = mvars(token);
            return newNode;
        }
        
        else {
            error("IDtk", token);
            exit(1);
        }
    }
    
    // <mvars> -> empty
    else {
        return NULL;
    }
}

node* expr(token& token) {
    
    // Create new node for tree
    node *newNode = nodeMaker("expr()");
    
    // <expr> -> <T> * <expr> or <expr> -> <T> / <expr>
    newNode->left = T(token);
    
    if (token.tokenType == STARtk || token.tokenType == SLASHtk) {
        
        // Add token info to the nodes
        addTokenInfo(newNode, token);
        
        token = scanner();
        newNode->center = expr(token);
        return newNode;
    }
    
    // <expr> -> <T>
    else {
        return newNode;
    }
}

node* T(token& token) {
    
    // Create new node for tree
    node *newNode = nodeMaker("T()");
    
    // <T> -> <F> + <T> or <F> - <T>
    newNode->left = F(token);
    
    if (token.tokenType == PLUStk || token.tokenType == MINUStk) {
        
        // Add token info to the nodes
        addTokenInfo(newNode, token);
        
        token = scanner();
        newNode->center = T(token);
        return newNode;
    }
    
    // <T> -> <F>
    else {
        return newNode;
    }
}

node* F(token& token) {
    
    // Create new node for tree
    node *newNode = nodeMaker("F()");
    
    // <F> -> - <F>
    if (token.tokenType == MINUStk) {
        
        // Add token info to the nodes
        addTokenInfo(newNode, token);
        
        token = scanner();
        newNode->left = F(token);
        return newNode;
    }
    
    // <F> -> <R>
    else {
        newNode->left = R(token);
        return newNode;
    }
}

node* R(token& token) {
    
    // Create new node for tree
    node *newNode = nodeMaker("R()");
    
    // <R> -> ( <expr> )
    if (token.tokenType == LEFTPARENTHESEtk) {
        
        token = scanner();
        newNode->left = expr(token);
        
        if (token.tokenType == RIGHTPARENTHESEtk) {
            
            token = scanner();
            return newNode;
        }
        
        else {
            error("RIGHTPARENTHESEtk", token);
            exit(1);
        }
    }
    
    // <R> -> ID
    else if (token.tokenType == IDtk) {
        
        // Add token info to the nodes
        addTokenInfo(newNode, token);
        
        token = scanner();
        return newNode;
    }
    
    // <R> -> Number
    else if (token.tokenType == NUMtk) {
        
        // Add token info to the nodes
        addTokenInfo(newNode, token);
        
        token = scanner();
        return newNode;
    }
    
    else {
        error("LEFTPARENTHESEtk, IDtk, or NUMtk", token);
        exit(1);
    }
}

node* stats(token& token) {
    
    // Create new node for tree
    node *newNode = nodeMaker("stats()");
    
    // <stats> -> <stat> <mStat>
    newNode->left = stat(token);
    newNode->center = mStat(token);
    return newNode;
}

node* mStat(token& token) {
    
    // Create new node for tree
    node *newNode = nodeMaker("mStat()");
    
    // <mStat> -> <stat> <mStat>
    if (token.tokenType != FINISHtk) {

        newNode->left = stat(token);
        newNode->center = mStat(token);
        return newNode;
    }
    
    // <mStat> -> empty
    else {
        return NULL;
    }
}

node* stat(token& token) {
    
    // Create new node for tree
    node *newNode = nodeMaker("stat()");
    
    // <stat> -> <in>
    if (token.tokenType == READtk) {
        
        newNode->left = inStat(token);
        return newNode;
    }
    
    // <stat> -> <out>
    else if (token.tokenType == PRINTtk) {
        
        newNode->left = outStat(token);
        return newNode;
    }
             
    // <stat> -> <block>
    else if (token.tokenType == STARTtk) {
        
        newNode->left = block(token);
        return newNode;
    }
    
    // <stat> -> <if>
    else if (token.tokenType == IFtk) {
        
        newNode->left = ifStat(token);
        return newNode;
    }
    
    // <stat> -> <loop>
    else if (token.tokenType == REPEATtk) {
        
        newNode->left = loop(token);
        return newNode;
    }
    
    // <stat> -> <assign>
    else if (token.tokenType == IDtk) {
        
        newNode->left = assign(token);
        return newNode;
    }
    
    else {
        error("READtk, PRINTtk, STARTtk, IFtk, REPEATtk, or IDtk", token);
        exit(1);
    }
}

node* inStat(token& token) {
    
    // Create new node for tree
    node *newNode = nodeMaker("inStat()");
    
    // <in> -> read ID .
    token = scanner();
    
    if (token.tokenType == IDtk) {
        
        // Add token info to the nodes
        addTokenInfo(newNode, token);
        
        token = scanner();
        
        if (token.tokenType == PERIODtk) {
            
            // Add token info to the nodes
            // addTokenInfo(newNode, token);
                            
            token = scanner();
            return newNode;
        }
        
        else {
            error("PERIODtk", token);
            exit(1);
        }
    }
    
    else {
        error("IDtk", token);
        exit(1);
    }
}

node* outStat(token& token) {
    
    // Create new node for tree
    node *newNode = nodeMaker("outStat()");
    
    // Add token info to the nodes
    addTokenInfo(newNode, token);
    
    token = scanner();
    newNode->left = expr(token);
    
    if (token.tokenType == PERIODtk) {
        
        // Add token info to the nodes
        // addTokenInfo(newNode, token);
        
        token = scanner();
        return newNode;
    }
    
    else {
        error("PERIODtk", token);
        exit(1);
    }
}

node* ifStat(token& token) {
    
    // Create new node for tree
    node *newNode = nodeMaker("ifStat()");
    
    // <if> -> if [ <expr> <RO> <expr> ] <block>
    token = scanner();
    
    if (token.tokenType == LEFTBRACKETtk) {
        
        // Add token info to the nodes
        // addTokenInfo(newNode, token);
        
        token = scanner();
        newNode->left = expr(token);
        RO(newNode, token);
        newNode->center = expr(token);
        
        if (token.tokenType == RIGHTBRACKETtk) {
            
            // Add token info to the nodes
            // addTokenInfo(newNode, token);
            
            token = scanner();
            newNode->right = block(token);
            return newNode;
        }
        else {
            error("RIGHTBRACKETtk", token);
            exit(1);
        }
    }
    else {
        error("LEFTBRACKETtk", token);
        exit(1);
    }
}

node* loop(token& token) {
    
    // Create new node for tree
    node *newNode = nodeMaker("loop()");
    
    // <loop> -> repeat [ <expr> <RO> <expr> ] <block>
    token = scanner();
    
    if (token.tokenType == LEFTBRACKETtk) {
        
        // Add token info to the nodes
        // addTokenInfo(newNode, token);
        
        token = scanner();
        newNode->left = expr(token);
        RO(newNode, token);
        newNode->center = expr(token);
        
        if (token.tokenType == RIGHTBRACKETtk) {
            
            // Add token info to the nodes
            // addTokenInfo(newNode, token);
            
            token = scanner();
            newNode->right = block(token);
            return newNode;
        }
        
        else {
            error("RIGHTBRACKETtk", token);
            exit(1);
        }
        
    }
    
    else {
        error("LEFTBRACKETtk", token);
        exit(1);
    }
}

node* assign(token& token) {
    
    // Create new node for tree
    node *newNode = nodeMaker("assign()");
    
    // <assign> -> ID = <expr> .
    addTokenInfo(newNode, token);
        
    token = scanner();
    
    if (token.tokenType == EQUALtk) {
        
        // Add token info to the nodes
        addTokenInfo(newNode, token);
        
        token = scanner();
        newNode->left = expr(token);
        
        if (token.tokenType == PERIODtk) {
            
            // Add token info to the nodes
            // addTokenInfo(newNode, token);
            
            token = scanner();
            return newNode;
        }
        
        else {
            error("PERIODtk", token);
            exit(1);
        }
    }
    else {
        error("EQUALtk", token);
        exit(1);
    }
}

void RO(node* newNode, token& token) {
    
    // <RO> -> =< or => or == or > or < or =!=
    if (token.tokenType == EQUALLESSTHANtk || token.tokenType == EQUALGREATERTHANtk || token.tokenType == EQUALEQUALtk || token.tokenType == GREATERTHANtk || token.tokenType == LESSTHANtk || token.tokenType == EQUALEXCLAMATIONEQUALtk) {
        
        // Add token info to the nodes
        addTokenInfo(newNode, token);
        
        token = scanner();
        return;
    }
    
    // Check for error
    else {
        error("EQUALLESSTHANtk, EQUALGREATERTHANtk, EQUALEQUALtk, GREATERTHANtk, LESSTHANtk, or EQUALEXCLAMATIONEQUALtk", token);
        exit(1);
    }
}

// Simplify creating nodes with 1 function
node* nodeMaker(string nodeName) {
    
    node *newNode = new node;
    newNode->left = NULL;
    newNode->center = NULL;
    newNode->right = NULL;
    newNode->depthCount = 0;
//    newNode->tokenCounter = 0;
    newNode->label = nodeName;
    
    return newNode;
}

void addTokenInfo(node* node, token& token) {
    stringstream buffer;
    string tempText;
    
    buffer << "Line number: " << token.lineNumber << ", type: " << enumValues[token.tokenType - 1] << ", token: " << token.token << "\n";
    tempText = buffer.str();
    
    node->tokenInfo.push_back(tempText);
    
    // Store the token in the right spot
    node->storedTokens.push_back(token);
}

// Simplify error process with 1 function
void error(string expectedToken, token& token) {
    
    // Print an error message
    cerr << "Expecting " << expectedToken << " but received " << enumValues[token.tokenType - 1] << " on line " << token.lineNumber << ".\n";
    remove("tempfile.dat");
}

