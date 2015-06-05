//
//  parser.h
//  Sieve_P1
//
//  Created by Jordan Sieve on 11/08/14.
//  Copyright (c) 2014 Jordan Sieve. All rights reserved.
//

#ifndef __Sieve_P1__parser__
#define __Sieve_P1__parser__

#include "scanner.h"

struct node {
    string label;
    vector <string> tokenInfo;
    vector <token> storedTokens;
    int depthCount;
    struct node *left;
    struct node *center;
    struct node *right;
};

// Prototypes
node* program(token& token);
node* block(token& token);
node* var(token& token);
node* type(token& token);
node* mvars(token& token);
node* expr(token& token);
node* T(token& token);
node* F(token& token);
node* R(token& token);
node* stats(token& token);
node* mStat(token& token);
node* stat(token& token);
node* inStat(token& token);
node* outStat(token& token);
node* ifStat(token& token);
node* loop(token& token);
node* assign(token& token);
void RO(node* node, token& token);
node* nodeMaker(string nodeName);
void error(string expectedToken, token& token);
void traversePreorder(node* traversePTR);
void addTokenInfo(node* node, token& token);

#endif /* defined(__Sieve_P1__parser__) */
