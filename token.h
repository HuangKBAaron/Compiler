//
//  token.h
//  Sieve_P1
//
//  Created by Jordan Sieve on 10/20/14.
//  Copyright (c) 2014 Jordan Sieve. All rights reserved.
//

#ifndef __Sieve_P1__token__
#define __Sieve_P1__token__

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
#include <sstream>
#include <algorithm>

// Include since all from standard library
using namespace std;


// ******************************************************************* //
//                                Tokens                               //
// ******************************************************************* //


// List of token options
typedef enum {
    EOFtk = 1,
    IDtk = 2,
    NUMtk = 3,
    EQUALEQUALtk = 4,
    LESSTHANtk = 5,
    GREATERTHANtk = 6,
    EQUALEXCLAMATIONEQUALtk = 7,
    EQUALGREATERTHANtk = 8,
    EQUALLESSTHANtk = 9,
    EQUALtk = 10,
    COLONtk = 11,
    PLUStk = 12,
    MINUStk = 13,
    STARtk = 14,
    SLASHtk = 15,
    PERCENTtk = 16,
    NOTtk = 17,
    PERIODtk = 18,
    LEFTPARENTHESEtk = 19,
    RIGHTPARENTHESEtk = 20,
    COMMAtk = 21,
    LEFTBRACEtk = 22,
    RIGHTBRACEtk = 23,
    SEMICOLONtk = 24,
    LEFTBRACKETtk = 25,
    RIGHTBRACKETtk = 26,
    STARTtk = 27,   // Start of keywords
    FINISHtk = 28,
    THENtk = 29,
    IFtk = 30,
    REPEATtk = 31,
    VARtk = 32,
    INTtk = 33,
    FLOATtk = 34,
    DOtk = 35,
    READtk = 36,
    PRINTtk = 37,
    VOIDtk = 38,
    RETURNtk = 39,
    DUMMYtk = 40,
    PROGRAMtk = 41,
    EMPTY = 42
} tokenIDRef;

// Array to reference enum values based on number
const string enumValues[41] = {"EOFtk", "IDtk", "NUMtk", "EQUALEQUALtk", "LESSTHANtk", "GREATERTHANtk", "EQUALEXCLAMATIONEQUALtk", "EQUALGREATERTHANtk", "EQUALLESSTHANtk", "EQUALtk", "COLONtk", "PLUStk", "MINUStk", "STARtk", "SLASHtk", "PERCENTtk", "NOTtk", "PERIODtk", "LEFTPARENTHESEtk", "RIGHTPARENTHESEtk", "COMMAtk", "LEFTBRACEtk", "RIGHTBRACEtk", "SEMICOLONtk", "LEFTBRACKETtk", "RIGHTBRACKETtk", "STARTtk", "FINISHtk", "THENtk", "IFtk", "REPEATtk", "VARtk", "INTtk", "FLOATtk", "DOtk", "READtk", "PRINTtk", "VOIDtk", "RETURNtk", "DUMMYtk", "PROGRAMtk"};

// Token data fields
typedef struct {
    tokenIDRef tokenType;
    string token;
    int lineNumber;
} token;


// ******************************************************************* //
//                                Tables                               //
// ******************************************************************* //

// State table to determine tokens from input - usage: [row][col]
const int stateTable[27][24] = {
    // 0  1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18   19  20 21  22  23
    // =  <	  >	  !	  :	  +   -   *   /	  %	  .	  (	  )	  ,	  {	  }	  ;	  [	  ]	 alp num WS	EOF	other
    {10,  5,  6, 17, 11, 12, 13, 14, 15, 16, 18, 19, 20, 21, 22, 23, 24, 25, 26,  2,  3,  0, -9, -2}, // Input
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -9, -2}, // EOFtk - not needed
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  2, -1, -1, -9, -2}, // IDtk
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  3, -1, -9, -2}, // NUMtk
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -9, -2}, // EQUALEQUALtk
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -9, -2}, // LESSTHANtk
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -9, -2}, // GREATERTHANtk
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -9, -2}, // EQUALEXCLAMATIONEQUALtk
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -9, -2}, // EQUALGREATERTHANtk
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -9, -2}, // EQUALLESSTHANtk
    { 4,  9,  8, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -9, -2}, // EQUALtk
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -9, -2}, // COLONtk
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -9, -2}, // PLUStk
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -9, -2}, // MINUStk
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -9, -2}, // STARtk
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -9, -2}, // SLASHtk
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -9, -2}, // PERCENTtk
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -9, -2}, // NOTtk
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -9, -2}, // PERIODtk
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -9, -2}, // LEFTPARENTHESEtk
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -9, -2}, // RIGHTPARENTHESEtk
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -9, -2}, // COMMAtk
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -9, -2}, // LEFTBRACEtk
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -9, -2}, // RIGHTBRACEtk
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -9, -2}, // SEMICOLONtk
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -9, -2}, // LEFTBRACKETtk
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -9, -2}  // RIGHTBRACKETtk
};

// Check ID tokens to language keywords
const token keywordLookUpTable[41] = {
    
    // ID token,   keyword, lineNum
    {                  EOFtk,     "EOF",  0},
    {                   IDtk,        "",  0},
    {                  NUMtk,        "",  0},
    {           EQUALEQUALtk,      "==",  0},
    {             LESSTHANtk,       "<",  0},
    {          GREATERTHANtk,       ">",  0},
    {EQUALEXCLAMATIONEQUALtk,     "=!=",  0},
    {     EQUALGREATERTHANtk,      "=>",  0},
    {        EQUALLESSTHANtk,      "=<",  0},
    {                EQUALtk,       "=",  0},
    {                COLONtk,       ":",  0},
    {                 PLUStk,       "+",  0},
    {                MINUStk,       "-",  0},
    {                 STARtk,       "*",  0},
    {                SLASHtk,       "/",  0},
    {              PERCENTtk,       "%",  0},
    {                  NOTtk,       "!",  0},
    {               PERIODtk,       ".",  0},
    {       LEFTPARENTHESEtk,       "(",  0},
    {      RIGHTPARENTHESEtk,       ")",  0},
    {                COMMAtk,       ",",  0},
    {            LEFTBRACEtk,       "{",  0},
    {           RIGHTBRACEtk,       "}",  0},
    {            SEMICOLONtk,       ";",  0},
    {          LEFTBRACKETtk,       "[",  0},
    {         RIGHTBRACKETtk,       "]",  0},
    {                STARTtk,   "start",  0},
    {               FINISHtk,  "finish",  0},
    {                 THENtk,    "then",  0},
    {                   IFtk,      "if",  0},
    {               REPEATtk,  "repeat",  0},
    {                  VARtk,     "var",  0},
    {                  INTtk,     "int",  0},
    {                FLOATtk,   "float",  0},
    {                   DOtk,      "do",  0},
    {                 READtk,    "read",  0},
    {                PRINTtk,   "print",  0},
    {                 VOIDtk,    "void",  0},
    {               RETURNtk,  "return",  0},
    {                DUMMYtk,   "dummy",  0},
    {              PROGRAMtk, "program",  0}
};


#endif /* defined(__Sieve_P1__token__) */
