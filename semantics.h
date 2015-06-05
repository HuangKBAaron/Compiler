//
//  semantics.h
//  Sieve_P1
//
//  Created by Jordan Sieve on 12/05/14.
//  Copyright (c) 2014 Jordan Sieve. All rights reserved.
//

#ifndef __Sieve_P1__semantics__
#define __Sieve_P1__semantics__

#include "scanner.h"
#include "parser.h"

// Global variable used for code generation
typedef enum { VAR, LABEL } nameType;

// Prototypes
void semantics(node *node);
void codeGen(node *node, ofstream &outputFile);
void error(token &token, bool exists);
void push(token &token, ofstream &outputFile);
void pop(int scopeStart, ofstream &outputFile);
int find(string varName);
string createVarName(nameType varType);

#endif /* defined(__Sieve_P1__semantics__) */
