/****************************************************/
/* File: parse.c                                    */
/* The parser implementation for the TINY compiler  */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/
#include "pch.h"
#include "globals.h"
#include "util.h"
#include "scan.h"
#include "parse.h"

static TokenTypeT token; /* holds current token */

/* function prototypes for recursive calls */
static TreeNode* program(void);
static TreeNode* declaration_list(void);
static TreeNode* declaration(void);
static TreeNode* var_declaration(void);
static TreeNode* type_specifier(void);
static TreeNode* fun_declaration(void);
static TreeNode* params(void);
static TreeNode* params_list(void);
static TreeNode* param(void);
static TreeNode* compound_stmt(void);
static TreeNode* local_declarations(void);
static TreeNode* statement_list(void);
static TreeNode* statement(void);
static TreeNode* expression_stmt(void);
static TreeNode* selection_stmt(void);
static TreeNode* iteration_stmt(void);
static TreeNode* return_stmt(void);
static TreeNode* expression(void);
static TreeNode* var(void);
static TreeNode* simple_expression(void);
static TreeNode* relop(void);
static TreeNode* additive_expression(void);
static TreeNode* addop(void);
static TreeNode* term(void);
static TreeNode* mulop(void);
static TreeNode* factor(void);
static TreeNode* call(void);
static TreeNode* args(void);
static TreeNode* arg_list(void);

static void syntaxError(char* message)
{
    fprintf(listing, "\n>>> ");
    fprintf(listing, "Syntax error at line %d: %s", lineno, message);
    Error = TRUE;
}

static void match(TokenTypeT expected)
{
    if (token == expected) token = getToken();
    else {
        syntaxError("unexpected token -> ");
        printToken(token, tokenString);
        fprintf(listing, "\n");
    }
}



/*---------------------------------------------------------------------------
-----------------------------------MiniC----------------------------------------
----------------------------------------------------------------------------*/
static TreeNode* program(void) {
    TreeNode* t = declaration_list();
    return t;
}
//-01/

static TreeNode* declaration_list(void) {
    TreeNode* t = declaration();
    TreeNode* p = t;
    TreeNode* q = NULL;
    while ((token == Void || token == Int || token == LBRACES) && p != NULL) {
        q = declaration();
        p->sibling = q;
        p = q;
    }
    return t;
}
//-02/

static TreeNode* declaration(void){
    TreeNode* t;
    if (token == LBRACES)
        t = compound_stmt();   //直接调用fun_decl()中的compound_stmt()
    else {
        t = type_specifier();
        t->attr.name = copyString(tokenString);
        match(ID);
        if (token == SEMI) {   //不直接调用var_decl()中的typer_specifier ID ;
            t->kind.stmt = VarDeclK;
            t->kind.exp = IdK;
            match(token);
        }
        else if (token == LBRACK) {   //不直接调用var_decl()中的typer_specifier ID[NUM] ;
            t->kind.stmt = VarDeclK;
            t->kind.exp = ArrayK;
            match(token);
            t->attr.arrayT = newExpNode(NumK);
            t->attr.arrayT->attr.val = atoi(tokenString);
            match(NUM);
            match(RBRACK);
        }
        else if (token == LPAREN) {   //不直接调用fun_decl()中的typer_specifier ID(param) ;
            t->kind.stmt = FunDeclK;
            t->kind.exp = FunK;
            match(token);
            t->attr.funT = params();
            match(RPAREN);
        }
    }
    return t;
}
//-03/

static TreeNode* var_declaration(void) {
    TreeNode* t;
    t = type_specifier();
    t->attr.name = copyString(tokenString);
    match(ID);
    if (token == SEMI) {
        t->kind.exp = IdK;
        match(token);
    }
    else if (token == LBRACK) {
        match(LBRACK);
        t->kind.exp = ArrayK;
        t->attr.arrayT = newExpNode(NumK);
        match(NUM);
        match(RBRACK);
        match(SEMI);
    }
    return t;
}
//-04/

static TreeNode* type_specifier(void) {
    TreeNode* t = newStmtNode(ParamK);
    if (t != NULL) {
        if (token == Void) {
            t->type = VoidT;
            match(Void);
        }
        else if (token == Int) {
            t->type = IntT;
            match(Int);
        }
        else {
            syntaxError("unknown type -> ");
            printToken(token, tokenString);
            fprintf(listing, "\n");
        }
    }
    return t;
}
//-05/

static TreeNode* fun_declaration(void){
    TreeNode* t;
    if (token == LBRACES)
        t = compound_stmt();
    else {
        t = type_specifier();
        if (t != NULL) {
            t->attr.name = copyString(tokenString);
            match(ID);
            match(LPAREN);
            t->kind.exp = FunK;
            t->attr.funT = params();
            match(RPAREN);
        }
    }
    return t;
}
//-06/

static TreeNode* params(void){
    TreeNode* t;
    if (token == Void) {
        match(token);
        t = nullptr;
    }
    else {
        t = params_list();
    }
    return t;
}
//-07/

static TreeNode* params_list(void){
    TreeNode* t = param();
    TreeNode* p = t;
    TreeNode* q = nullptr;
    while (token == COMMA) {
        match(token);
        q = param();
        p->sibling = q;
        p = q;
    }
    return t;
}
//-08/

static TreeNode* param(void){
    TreeNode* t = type_specifier();
    if (t != NULL) {
        t->attr.name = copyString(tokenString);
        match(ID);
        if (token == LBRACK) {
            match(LBRACK);
            match(RBRACK);
        }
    }
    return t;
}
//-09/

static TreeNode* compound_stmt(void) {
    TreeNode* t;
    match(LBRACES);
    t = newStmtNode(CompoundK);
    if (t != NULL) {
        t->child[0] = local_declarations();
        t->child[1] = statement_list();
    }
    match(RBRACES);
    return t;
}
//-10/

static TreeNode* local_declarations(void) {
    TreeNode* t = nullptr;
    TreeNode* p = t;
    TreeNode* q;
    if (token == Int || token == Void)
        p = var_declaration();
    while(token == Int || token == Void) {
        q = var_declaration();
        p->sibling = q;
        p = q;
    }
    return t;
}
//-11/

static TreeNode* statement_list(void){
    TreeNode* t = nullptr;
    TreeNode* p = t;
    TreeNode* q;
    if (token == SEMI || token == LPAREN || token == ID ||
        token == NUM || token == LBRACES || token == IF ||
        token == WHILE || token == RETURN)
        p = statement();
    while (token == SEMI || token == LPAREN || token == ID ||
        token == NUM || token == LBRACES || token == IF ||
        token == WHILE || token == RETURN) {
        q = statement();
        p->sibling = q;
        p = q;
    }
    return t;
}
//-12/

static TreeNode* statement(void){
    TreeNode* t = nullptr;
    //SEMI,(,ID,NUM,{,IF,WHILE,RETURN
    switch (token) {
    case SEMI:
    case LPAREN:
    case ID:
    case NUM:
        t = expression_stmt();
        break;
    case LBRACES:
        t = compound_stmt();
        break;
    case IF:
        t = selection_stmt();
        break;
    case WHILE:
        t = iteration_stmt();
        break;
    case RETURN:
        t = return_stmt();
        break;
    default:
        syntaxError("unexpected token -> ");
        printToken(token, tokenString);
        token = getToken();
        break;
    }
    /*if (token == SEMI || token == LPAREN || token == ID || token == NUM)
        t = expression_stmt();
    if (token == LBRACES)
        t = compound_stmt();
    if (token == IF)
        t = selection_stmt();
    if (token == WHILE)
        t = iteration_stmt();
    if (token == RETURN)
        t = return_stmt();*/
    return t;
}
//-13/

static TreeNode* expression_stmt(void){
    TreeNode* t = nullptr;
    if (token == SEMI) {
        match(token);
    }
    else if (t != NULL) {
        t = expression();
        match(SEMI);
    }
    return t;
}
//-14/

static TreeNode* selection_stmt(void){
    TreeNode* t = newStmtNode(IfK);
    match(IF);
    if (t != NULL) {
        match(LPAREN);
        t->child[0] = expression();
        match(RPAREN);
        t->child[1] = statement();
        if (token == ELSE) {
            match(token);
            t->child[2] = statement();
        }
    }
    return t;
}
//-15/

static TreeNode* iteration_stmt(void){
    TreeNode* t = newStmtNode(WhileK);
    match(WHILE);
    match(LPAREN);
    if (t != NULL) {
        t->child[0] = expression();
        match(RPAREN);
        t->child[1] = statement();
    }
    return t;
}
//-16/

static TreeNode* return_stmt(void){
    TreeNode* t = newStmtNode(ReturnK);
    match(RETURN);
    if (token == SEMI)
        match(token);
    else if (t != NULL) {
        t->child[0] = expression();
        match(SEMI);
    }
    return t;
}
//-17/

static TreeNode* expression(void){
    TreeNode* t = nullptr;
    TreeNode* p = nullptr;
    char* ch;
    switch (token) {
    case ID:
        p = simple_expression();//var()
        if (token == ASSIGN) {
            t = newStmtNode(AssignK);
            match(token);
            t->child[0] = p;
            t->child[1] = expression();
        }
        else
            t = p;
        break;
    case LPAREN:
    case NUM:
        t = simple_expression();
        break;
    default:
        syntaxError("unexpected token -> ");
        printToken(token, tokenString);
        token = getToken();
        break;
    }
    /*if (token == ID) {
        p = simple_expression();//var()
        if (token == ASSIGN) {
            t = newStmtNode(AssignK);
            match(token);
            t->child[0] = p;
            t->child[1] = expression();
        }
        else
            t = p;
    }
    else if (token == LPAREN || token == NUM)
        t = simple_expression();*/
    return t;
}
//-18/

static TreeNode* var(void){
    TreeNode* t = newExpNode(IdK);
    t->attr.name = copyString(tokenString);
    match(ID);
    if (token == LBRACK) {
        t->kind.exp = ArrayK;
        match(token);
        t->attr.arrayT = expression();
        match(RBRACK);
    }
    return t;
}
//-19/

static TreeNode* simple_expression(void){
    TreeNode* t = additive_expression();
    if (token == LE || token == LT || token == GT || token == GE
        || token == EQ || token == NE) {
        TreeNode* p = relop();
        if (p != NULL) {
            p->child[0] = t;
            t = p;
            p->child[1] = additive_expression();
        }
    }
    return t;
}
//-20/

static TreeNode* relop(void){
    TreeNode* t = newExpNode(OpK);
    if (t != NULL) {
        t->attr.op = token;
        match(token);
    }
    return t;
}
//-21/

static TreeNode* additive_expression(void){
    TreeNode* t = term();
    while (token == PLUS || token == MINUS) {
        TreeNode* p = addop();
        if (p != NULL) {
            p->child[0] = t;
            t = p;
            p->child[1] = term();
        }
    }
    return t;
}
//-22/

static TreeNode* addop(void){
    TreeNode* t = newExpNode(OpK);
    if (t != NULL) {
        t->attr.op = token;
        match(token);
    }
    return t;
}
//-23/

static TreeNode* term(void){
    TreeNode* t = factor();
    TreeNode* p;
    while (token == TIMES || token == OVER) {
        p = mulop();
        if (p != NULL) {
            p->child[0] = t;
            t = p;
            p->child[1] = factor();
        }
    }
    return t;
}
//-24/

static TreeNode* mulop(void){
    TreeNode* t = newExpNode(OpK);
    if (t != NULL) {
        t->attr.op = token;
        match(token);
    }
    return t;
}
//-25/

static TreeNode* factor(void){
    TreeNode* t = nullptr;
    char* ch;
    switch (token) {
    case NUM:
        t = newExpNode(NumK);
        if ((t != NULL) && (token == NUM))
            t->attr.val = atoi(tokenString);
        match(NUM);
        break;
    case ID:
        t = newExpNode(IdK);
        t->attr.name = copyString(tokenString);
        match(ID);
        if (token == LBRACK) {
            t->kind.exp = ArrayK;
            match(token);
            t->attr.arrayT = expression();
            match(RBRACK);
        }
        else if (token == LPAREN) {
            t->kind.exp = FunK;
            match(token);
            t->attr.funT = args();
            match(RPAREN);
        }
        break;
    case LPAREN:
        t = expression();
        match(RPAREN);
        break;
    }
    return t;
}
//-26/

static TreeNode* call(void){
    TreeNode* t = newExpNode(FunK);
    match(LPAREN);
    t->attr.funT = args();
    match(RPAREN);
    return t;
}
//-27/

static TreeNode* args(void){
    TreeNode* t = nullptr;
    if (token == ID || token == NUM || token == LPAREN)
        t = arg_list();
    return t;
}
//-28/

static TreeNode* arg_list(void){
    TreeNode* t = expression();
    TreeNode* p = t;
    TreeNode* q = nullptr;
    while (token == COMMA) {
        match(token);
        q = expression();
        p->sibling = q;
        p = q;
    }
    return t;
}
//-29/
//-------------------------------------------------------------------------------



/****************************************/
/* the primary function of the parser   */
/****************************************/
/* Function parse returns the newly
 * constructed syntax tree
 */
TreeNode* parse(void)
{
    TreeNode* t;
    token = getToken();
    t = program();
    if (token != ENDFILE)
        syntaxError("Code ends before file\n");
    return t;
}
