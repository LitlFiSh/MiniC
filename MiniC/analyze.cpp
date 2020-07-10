#include "pch.h"
#include"analyze.h"
#include"globals.h"
#include"parse.h"
#include"scan.h"
#include"util.h"

/* allocate global variables */
int lineno = 0;
FILE* source;
FILE* listing;
FILE* code;

/* allocate and set tracing flags */
int EchoSource = FALSE;
int TraceScan = FALSE;
int TraceParse = TRUE;
int TraceAnalyze = FALSE;
int TraceCode = FALSE;
int Error = FALSE;

void analyze(string path, string str) {
    TreeNode* syntaxTree;
    char pgm[120]; /* source code file name */
    strcpy(pgm, path.c_str());
    if (strchr(pgm, '.') == NULL)
        strcat(pgm, ".tny");
    source = fopen(pgm, "r");
    if (source == NULL) {
        exit(1);
    }
    listing = fopen(str.c_str(), "w");

    int linepos = 0;
    int bufsize = 0;
    EOF_flag = FALSE;
    syntaxTree = parse();

    printTree(syntaxTree);
    fclose(listing);
    fclose(source);
}

void clear() {
    lineno = 0;
}