#include "compiler.h"




/* --- Globals for the parser --- */
static TokenType g_lookahead;
static char* g_last_lexeme = NULL;
extern Token g_current_token; // Defined in lex.yy.c
extern int yylineno;          // Defined in lex.yy.c
// --- External Declarations (from symboltable.c, lex.yy.c, etc.) ---
//extern Token *lookahead;
//extern SymbolTable *symtab_stack;

/* --- Forward declarations for ALL non-terminal functions --- */
static ASTNode* prog(void);
static ASTNode* classOrImplOrFunc_ZM(void);
static ASTNode* classOrImplOrFunc(void);
static ASTNode* classDecl(void);
static ASTNode* isa_Id_ZO(void);
static ASTNode* commaID_ZM(void);
static ASTNode* visibility_memberDecl_ZM(void);
static ASTNode* implDef(void);
static ASTNode* funcDef_ZM(void);
static ASTNode* funcDef(void);
static NodeType visibility(void);
static ASTNode* memberDecl(TokenType visibility);
static ASTNode* funcDecl(void);
static ASTNode* funcHead(void);
static ASTNode* funcBody(void);
static ASTNode* varDeclOrStmt_ZM(void);
static ASTNode* varDeclOrStmt(void);
static ASTNode* attributeDecl(void);
static ASTNode* localVarDecl(void);
static ASTNode* varDecl(void);
static ASTNode* arraySize_ZM(void);
static ASTNode* statement(void);
static ASTNode* varOrFunc_Head(void);
static ASTNode* statement_tail(ASTNode* head);
static ASTNode* statBlock(void);
static ASTNode* statement_ZM(void);
static ASTNode* expr(void);
static ASTNode* relExpr_L(ASTNode* left);
static ASTNode* arithExpr(void);
static ASTNode* arithExpr_L(ASTNode* left);
static NodeType sign(void);
static ASTNode* term(void);
static ASTNode* term_L(ASTNode* left);
static ASTNode* factor(void);
static ASTNode* factor_tail(ASTNode* head);
static ASTNode* variable(void);
static ASTNode* idnest_ZM(void);
static ASTNode* indice_ZM(void);
static ASTNode* idnest(void);
static ASTNode* idnest_L(ASTNode* idOrSelf);
static ASTNode* idOrSelf(void);
static ASTNode* indice(void);
static ASTNode* arraySize(void);
static ASTNode* arraySize_L(void);
static ASTNode* type(void);
static ASTNode* returnType(void);
static ASTNode* fParams(void);
static ASTNode* fParamsTail_ZM(void);
static ASTNode* aParams(void);
static ASTNode* aParamsTail_ZM(void);
static ASTNode* fParamsTail(void);
static ASTNode* aParamsTail(void);
static NodeType assignOp(void);
static NodeType relOp(void);
static NodeType addOp(void);
static NodeType multOp(void);
ASTNode *Program();


/* --- Helper Functions --- */

// Advance to the next token
static void advance(void) {

    if (g_last_lexeme) {
        free(g_last_lexeme);
        g_last_lexeme = NULL;
    }
    
    // 2. Load the next token (yylex updates g_current_token)
    g_lookahead = (TokenType)yylex();
    
    // 3. Save the new lexeme address for future cleanup
    g_last_lexeme = g_current_token.lexeme;

    
}

// Check current token and advance
static void match(TokenType expected) {
    if (g_lookahead == expected) {
        advance();
    } else {
        // Simple error reporting
        char msg[200];
        snprintf(msg, 199, "Syntax Error: Expected token %d but got %d ('%s')", 
                 expected, g_lookahead, g_current_token.lexeme);
        record_error(yylineno, ERROR_SYNTAX, msg);
        
        // Simple panic-mode recovery: skip until a token that can start a statement or definition
        while (g_lookahead != TOK_EOF && 
               g_lookahead != TOK_SEMI && 
               g_lookahead != TOK_RBRACE &&
               g_lookahead != TOK_CLASS &&
               g_lookahead != TOK_IMPLEMENT &&
               g_lookahead != TOK_FUNC &&
               g_lookahead != TOK_IF &&
               g_lookahead != TOK_WHILE &&
               g_lookahead != TOK_ID)
        {
            advance();
        }
    }
}

/* --- Public API --- */

ASTNode* parse_program(void) {
    advance(); // Load the first token (sets g_lookahead)
    ASTNode* root = prog();

    // Final cleanup of the last token's lexeme memory
    if (g_last_lexeme) {
        free(g_last_lexeme);
        g_last_lexeme = NULL;
    }

    if (g_lookahead != TOK_EOF) {
        record_error(yylineno, ERROR_SYNTAX, "Expected EOF but found '%s'", g_current_token.lexeme);
    }
    return root;
}


/* --- Grammar Rule Implementations --- */

// prog -> classOrImplOrFunc_ZM
static ASTNode* prog(void) {
    int line = yylineno;
    ASTNode* list = classOrImplOrFunc_ZM();
    // We wrap the top-level list in a STMT_LIST for consistency
    return create_node(NODE_STMT_LIST, line, list, NULL, NULL);
}

// classOrImplOrFunc_ZM -> classOrImplOrFunc classOrImplOrFunc_ZM | ϵ
static ASTNode* classOrImplOrFunc_ZM(void) {
    // FIRST(classOrImplOrFunc) = { class, implement, func, constructor }
    if (g_lookahead == TOK_CLASS || 
        g_lookahead == TOK_IMPLEMENT || 
        g_lookahead == TOK_FUNC ||
        g_lookahead == TOK_CONSTRUCTOR) 
    {
        ASTNode *head = classOrImplOrFunc();
        ASTNode *tail = classOrImplOrFunc_ZM();
        head->next_sibling = tail;
        return head;
    } else {
        // Epsilon case (FOLLOW(classOrImplOrFunc_ZM) = { EOF })
        return NULL;
    }
}

// classOrImplOrFunc -> classDecl | implDef | funcDef
static ASTNode* classOrImplOrFunc(void) {
    switch (g_lookahead) {
        case TOK_CLASS:
            return classDecl();
        case TOK_IMPLEMENT:
            return implDef();
        case TOK_FUNC:
        case TOK_CONSTRUCTOR:
            return funcDef();
        default:
            record_error(yylineno, ERROR_SYNTAX, "Expected class, implement, or func definition");
            advance(); // panic
            return NULL;
    }
}

// classDecl -> class id isa_Id_ZO { visibility_memberDecl_ZM } ;
static ASTNode* classDecl(void) {
    int line = yylineno;
    match(TOK_CLASS);
    
    ASTNode* class_id = create_leaf_id(yylineno, g_current_token.lexeme);
    match(TOK_ID);
    
    ASTNode* inheritance_list = isa_Id_ZO();
    
    match(TOK_LBRACE);
    ASTNode* members = visibility_memberDecl_ZM();
    match(TOK_RBRACE);
    match(TOK_SEMI);
    
    // Use left for ID, right for inheritance, third for members
    return create_node(NODE_CLASS_DECL, line, class_id, inheritance_list, members);
}

// isa_Id_ZO -> isa id commaID_ZM | ϵ
static ASTNode* isa_Id_ZO(void) {
    // FIRST = { isa }
    if (g_lookahead == TOK_ISA) {
        //int line = yylineno;
        match(TOK_ISA);
        
        ASTNode* head = create_leaf_id(yylineno, g_current_token.lexeme);
        match(TOK_ID);
        
        ASTNode* tail = commaID_ZM();
        head->next_sibling = tail;
        
        // Return the list of parent IDs
        return head;
    } else {
        // Epsilon case (FOLLOW = { { })
        return NULL;
    }
}

// commaID_ZM -> , id commaID_ZM | ϵ
static ASTNode* commaID_ZM(void) {
    // FIRST = { , }
    if (g_lookahead == TOK_COMMA) {
        match(TOK_COMMA);
        
        ASTNode* head = create_leaf_id(yylineno, g_current_token.lexeme);
        match(TOK_ID);
        
        ASTNode* tail = commaID_ZM();
        head->next_sibling = tail;
        return head;
    } else {
        // Epsilon case (FOLLOW = { { })
        return NULL;
    }
}

// visibility_memberDecl_ZM -> visibility memberDecl visibility_memberDecl_ZM | ϵ
static ASTNode* visibility_memberDecl_ZM(void) {
    // FIRST = { public, private }
    if (g_lookahead == TOK_PUBLIC || g_lookahead == TOK_PRIVATE) {
        TokenType vis = visibility(); // Store visibility
        //NodeType vis = visibility();
        ASTNode* head = memberDecl(vis);
        
        ASTNode* tail = visibility_memberDecl_ZM();
        head->next_sibling = tail;
        return head;
    } else {
        // Epsilon case (FOLLOW = { } })
        return NULL;
    }
}

// implDef -> implement id { funcDef_ZM }
static ASTNode* implDef(void) {
    int line = yylineno;
    match(TOK_IMPLEMENT);
    
    ASTNode* impl_id = create_leaf_id(yylineno, g_current_token.lexeme);
    match(TOK_ID);
    
    match(TOK_LBRACE);
    ASTNode* funcs = funcDef_ZM();
    match(TOK_RBRACE);
    
    // Use left for ID, right for func list
    return create_node(NODE_IMPLEMENT_DEF, line, impl_id, funcs, NULL);
}

// funcDef_ZM -> funcDef funcDef_ZM | ϵ
static ASTNode* funcDef_ZM(void) {
    // FIRST = { func, constructor }
    if (g_lookahead == TOK_FUNC || g_lookahead == TOK_CONSTRUCTOR) {
        ASTNode* head = funcDef();
        ASTNode* tail = funcDef_ZM();
        head->next_sibling = tail;
        return head;
    } else {
        // Epsilon case (FOLLOW = { } })
        return NULL;
    }
}

// funcDef -> funcHead funcBody
static ASTNode* funcDef(void) {
    int line = yylineno;
    ASTNode* head = funcHead();
    ASTNode* body = funcBody();
    
    // funcHead is left, funcBody is right
    return create_node(NODE_FUNC_DEF, line, head, body, NULL);
}

// visibility -> public | private
static NodeType visibility(void) {
        if (g_lookahead == TOK_PUBLIC) {
                match(TOK_PUBLIC);
                return TOK_PUBLIC;
            } else if (g_lookahead == TOK_PRIVATE) {
                match(TOK_PRIVATE);
                return TOK_PRIVATE;
            }
            // Should not be reached
            return TOK_UNKNOWN;
}

// memberDecl -> funcDecl | attributeDecl
static ASTNode* memberDecl(TokenType visibility) {
    // We pass visibility to the sub-rules
    ASTNode* decl = NULL;
    // FIRST(funcDecl) = { func, constructor }
    // FIRST(attributeDecl) = { attribute }
    switch (g_lookahead) {
        case TOK_FUNC:
        case TOK_CONSTRUCTOR:
            decl = funcDecl();
            break;
        case TOK_ATTRIBUTE:
            decl = attributeDecl();
            break;
        default:
            record_error(yylineno, ERROR_SYNTAX, "Expected function or attribute declaration");
            advance(); // panic
            return NULL;
    }
    // We would attach visibility to the node here
    // decl->visibility = vis;
    return decl;
}

// funcDecl -> funcHead ;
static ASTNode* funcDecl(void) {
    int line = yylineno;
    ASTNode* head = funcHead();
    match(TOK_SEMI);
    
    // funcHead is left
    return create_node(NODE_FUNC_DECL, line, head, NULL, NULL);
}

// funcHead -> func id ( fParams ) => returnType | constructor ( fParams )
static ASTNode* funcHead(void) {
    int line = yylineno;
    if (g_lookahead == TOK_FUNC) {
        match(TOK_FUNC);
        ASTNode* func_id = create_leaf_id(yylineno, g_current_token.lexeme);
        match(TOK_ID);
        
        match(TOK_LPAREN);
        ASTNode* params = fParams();
        match(TOK_RPAREN);
        
        match(TOK_ARROW);
        ASTNode* retType = returnType();
        
        // left=id, right=params, third=retType
        return create_node(NODE_FUNC_DECL, line, func_id, params, retType);
        
    } else if (g_lookahead == TOK_CONSTRUCTOR) {
        match(TOK_CONSTRUCTOR);
        
        match(TOK_LPAREN);
        ASTNode* params = fParams();
        match(TOK_RPAREN);
        
        // left=params
        return create_node(NODE_CONSTRUCTOR_DEF, line, params, NULL, NULL);
    }
    
    record_error(line, ERROR_SYNTAX, "Expected 'func' or 'constructor'");
    advance();
    return NULL;
}

// funcBody -> { varDeclOrStmt_ZM }
static ASTNode* funcBody(void) {
    int line = yylineno;
    match(TOK_LBRACE);
    ASTNode* body = varDeclOrStmt_ZM();
    match(TOK_RBRACE);
    
    // Wrap statement list in a STMT_LIST node
    return create_node(NODE_STMT_LIST, line, body, NULL, NULL);
}

// varDeclOrStmt_ZM -> varDeclOrStmt varDeclOrStmt_ZM | ϵ
static ASTNode* varDeclOrStmt_ZM(void) {
    // FIRST(varDeclOrStmt) = { local, id, self, if, while, read, write, return }
    if (g_lookahead == TOK_LOCAL || g_lookahead == TOK_ID || 
        g_lookahead == TOK_SELF || g_lookahead == TOK_IF || 
        g_lookahead == TOK_WHILE || g_lookahead == TOK_READ || 
        g_lookahead == TOK_WRITE || g_lookahead == TOK_RETURN)
    {
        ASTNode* head = varDeclOrStmt();
        ASTNode* tail = varDeclOrStmt_ZM();
        head->next_sibling = tail;
        return head;
    } else {
        // Epsilon case (FOLLOW = { } })
        return NULL;
    }
}

// varDeclOrStmt -> localVarDecl | statement
static ASTNode* varDeclOrStmt(void) {
    switch (g_lookahead) {
        case TOK_LOCAL:
            return localVarDecl();
        case TOK_ID:
        case TOK_SELF:
        case TOK_IF:
        case TOK_WHILE:
        case TOK_READ:
        case TOK_WRITE:
        case TOK_RETURN:
            return statement();
        default:
            record_error(yylineno, ERROR_SYNTAX, "Expected variable declaration or statement");
            advance();
            return NULL;
    }
}

// attributeDecl -> attribute varDecl
static ASTNode* attributeDecl(void) {
    int line = yylineno;
    match(TOK_ATTRIBUTE);
    ASTNode* var = varDecl();
    // left = varDecl node
    return create_node(NODE_ATTRIBUTE_DECL, line, var, NULL, NULL);
}

// localVarDecl -> local varDecl
static ASTNode* localVarDecl(void) {
    int line = yylineno;
    match(TOK_LOCAL);
    ASTNode* var = varDecl();
    // left = varDecl node
    return create_node(NODE_LOCAL_VAR_DECL, line, var, NULL, NULL);
}

// varDecl -> id : type arraySize_ZM ;
static ASTNode* varDecl(void) {
    int line = yylineno;
    ASTNode* var_id = create_leaf_id(yylineno, g_current_token.lexeme);
    match(TOK_ID);
    
    match(TOK_COLON);
    ASTNode* var_type = type();
    
    // Handle array declarations
    ASTNode* arrays = arraySize_ZM();
    if (arrays) {
        // Build a chain of array types
        ASTNode* base_type = var_type;
        ASTNode* current_array = arrays;
        while (current_array) {
            // left=base_type, right=size_expr (from arraySize)
            current_array->left = base_type; 
            base_type = current_array;
            current_array = current_array->next_sibling;
        }
        var_type = base_type; // The final type is the outermost array
    }

    match(TOK_SEMI);
    
    // left=id, right=type
    return create_node(NODE_VAR_DECL, line, var_id, var_type, NULL);
}

// arraySize_ZM -> arraySize arraySize_ZM | ϵ
static ASTNode* arraySize_ZM(void) {
    // FIRST = { [ }
    if (g_lookahead == TOK_LBRACKET) {
        ASTNode* head = arraySize();
        ASTNode* tail = arraySize_ZM();
        head->next_sibling = tail;
        return head;
    } else {
        // Epsilon case (FOLLOW = { ;, , ))
        return NULL;
    }
}

// statement -> varOrFunc_Head statement_tail
// | if ( relExpr ) then statBlock else statBlock ;
// | while ( relExpr ) statBlock ; | read ( variable ) ; | write ( expr ) ;
// | return ( expr ) ;
static ASTNode* statement(void) {
    int line = yylineno;
    switch (g_lookahead) {
        // Case 1: varOrFunc_Head ...
        case TOK_ID:
        case TOK_SELF:
        {
            ASTNode* head = varOrFunc_Head();
            // statement_tail builds the final node (assign or call)
            return statement_tail(head);
        }
        
        // Case 2: if ...
        case TOK_IF:
        {
            match(TOK_IF);
            match(TOK_LPAREN);
            ASTNode* cond = expr(); // Use expr, not relExpr
            match(TOK_RPAREN);
            match(TOK_THEN);
            ASTNode* then_block = statBlock();
            match(TOK_ELSE);
            ASTNode* else_block = statBlock();
            match(TOK_SEMI);
            
            // left=cond, right=then, third=else
            return create_node(NODE_IF_STMT, line, cond, then_block, else_block);
        }
        
        // Case 3: while ...
        case TOK_WHILE:
        {
            match(TOK_WHILE);
            match(TOK_LPAREN);
            ASTNode* cond = expr(); // Use expr
            match(TOK_RPAREN);
            ASTNode* body = statBlock();
            match(TOK_SEMI);
            
            // left=cond, right=body
            return create_node(NODE_WHILE_STMT, line, cond, body, NULL);
        }
        
        // Case 4: read ...
        case TOK_READ:
        {
            match(TOK_READ);
            match(TOK_LPAREN);
            ASTNode* var = variable();
            match(TOK_RPAREN);
            match(TOK_SEMI);
            
            // left=variable
            return create_node(NODE_READ_STMT, line, var, NULL, NULL);
        }
        
        // Case 5: write ...
        case TOK_WRITE:
        {
            match(TOK_WRITE);
            match(TOK_LPAREN);
            ASTNode* exp = expr();
            match(TOK_RPAREN);
            match(TOK_SEMI);
            
            // left=expr
            return create_node(NODE_WRITE_STMT, line, exp, NULL, NULL);
        }

        // Case 6: return ...
        case TOK_RETURN:
        {
            match(TOK_RETURN);
            match(TOK_LPAREN);
            ASTNode* exp = expr();
            match(TOK_RPAREN);
            match(TOK_SEMI);
            
            // left=expr
            return create_node(NODE_RETURN_STMT, line, exp, NULL, NULL);
        }
        
        default:
            record_error(line, ERROR_SYNTAX, "Expected statement");
            advance();
            return NULL;
    }
}

// varOrFunc_Head -> idnest_ZM id
static ASTNode* varOrFunc_Head(void) {
    // This is tricky. We build the "call chain" (member access, array index)
    // and the *last* id is the "head" of the expression.
    
    // We parse idnest_ZM first
    ASTNode* chain_head = idnest_ZM();
    
    ASTNode* final_id = create_leaf_id(yylineno, g_current_token.lexeme);
    match(TOK_ID);

    if (!chain_head) {
        return final_id; // Simple ID
    }
    
    // Append the final_id to the end of the idnest_ZM chain
    ASTNode* current = chain_head;
    while (current->left) { // The 'left' child is the next link in the chain
        current = current->left;
    }
    
    // The chain is built of NODE_MEMBER_ACCESS or NODE_FUNC_CALL
    // We set the *final* access/call to point to this new ID
    // idnest_L -> ... .  | ... (aParams)
    // The 'left' child of the last node in the chain is NULL
    current->left = final_id;
    
    return chain_head;
}

// statement_tail -> indice_ZM assignOp expr ; | ( aParams ) ;
static ASTNode* statement_tail(ASTNode* head) {
    int line = head->lineno;
    // FIRST(indice_ZM) = { [ }
    // FIRST(assignOp) = { := }
    if (g_lookahead == TOK_LBRACKET || g_lookahead == TOK_ASSIGN) {
        // Assignment case
        
        // This is varOrFunc_Head [indices] := expr
        // We need to build the variable node first
        ASTNode* indices = indice_ZM();
        ASTNode* var_node = head;
        
        if (indices) {
            // Attach indices to the variable
            ASTNode* current_index = indices;
            var_node = create_node(NODE_ARRAY_ACCESS, line, head, current_index, NULL);
            current_index = current_index->next_sibling;
            while(current_index) {
                var_node = create_node(NODE_ARRAY_ACCESS, line, var_node, current_index, NULL);
                current_index = current_index->next_sibling;
            }
        }
        
        (void)assignOp(); // Consume :=
        ASTNode* exp = expr();
        match(TOK_SEMI);
        
        // left=var, right=expr
        return create_node(NODE_ASSIGN_STMT, line, var_node, exp, NULL);
        
    } 
    // FIRST( (aParams) ) = { ( }
    else if (g_lookahead == TOK_LPAREN) {
        // Function call case
        match(TOK_LPAREN);
        ASTNode* args = aParams();
        match(TOK_RPAREN);
        match(TOK_SEMI);
        
        // left=func_head, right=args
        return create_node(NODE_CALL_STMT, line, head, args, NULL);
    }
    
    record_error(line, ERROR_SYNTAX, "Expected assignment or function call");
    advance();
    return NULL;
}

// statBlock -> { statement_ZM } | statement | ϵ
static ASTNode* statBlock(void) {
    int line = yylineno;
    // FIRST({ statement_ZM }) = { { }
    if (g_lookahead == TOK_LBRACE) {
        match(TOK_LBRACE);
        ASTNode* list = statement_ZM();
        match(TOK_RBRACE);
        // Wrap in a STMT_LIST
        return create_node(NODE_STMT_LIST, line, list, NULL, NULL);
    }
    // FIRST(statement) = { id, self, if, while, read, write, return }
    else if (g_lookahead == TOK_ID || g_lookahead == TOK_SELF ||
             g_lookahead == TOK_IF || g_lookahead == TOK_WHILE ||
             g_lookahead == TOK_READ || g_lookahead == TOK_WRITE ||
             g_lookahead == TOK_RETURN)
    {
        ASTNode* stmt = statement();
        // Wrap single statement in a STMT_LIST for consistency
        return create_node(NODE_STMT_LIST, line, stmt, NULL, NULL);
    }
    else {
        // Epsilon case (FOLLOW = { ;, else })
        return NULL;
    }
}

// statement_ZM -> statement statement_ZM | ϵ
static ASTNode* statement_ZM(void) {
    // FIRST(statement) = { id, self, if, while, read, write, return }
    if (g_lookahead == TOK_ID || g_lookahead == TOK_SELF ||
        g_lookahead == TOK_IF || g_lookahead == TOK_WHILE ||
        g_lookahead == TOK_READ || g_lookahead == TOK_WRITE ||
        g_lookahead == TOK_RETURN)
    {
        ASTNode* head = statement();
        ASTNode* tail = statement_ZM();
        head->next_sibling = tail;
        return head;
    } else {
        // Epsilon case (FOLLOW = { } })
        return NULL;
    }
}

// expr -> arithExpr relExpr_L
static ASTNode* expr(void) {
    ASTNode* left = arithExpr();
    return relExpr_L(left);
}

// relExpr_L -> relOp arithExpr relExpr_L | ϵ
static ASTNode* relExpr_L(ASTNode* left) {
    // FIRST(relOp) = { ==, <, >, <>, <=, >= }
    if (g_lookahead == TOK_EQ || g_lookahead == TOK_LT || g_lookahead == TOK_GT ||
        g_lookahead == TOK_NEQ || g_lookahead == TOK_LE || g_lookahead == TOK_GE)
    {
        int line = yylineno;
        NodeType op = relOp();
        ASTNode* right = arithExpr();
        
        // Build the op node
        ASTNode* op_node = create_node(NODE_BINARY_OP, line, left, right, NULL);
        op_node->ival = op; // Store the specific token type
        
        // Recurse for the rest of the expression
        return relExpr_L(op_node);
    } else {
        // Epsilon case
        return left; // Pass up the left-hand side
    }
}

// arithExpr -> term arithExpr_L
static ASTNode* arithExpr(void) {
    ASTNode* left = term();
    return arithExpr_L(left);
}

// arithExpr_L -> addOp term arithExpr_L | ϵ
static ASTNode* arithExpr_L(ASTNode* left) {
    // FIRST(addOp) = { +, -, or }
    if (g_lookahead == TOK_PLUS || g_lookahead == TOK_MINUS || g_lookahead == TOK_OR) {
        int line = yylineno;
        NodeType op = addOp();
        ASTNode* right = term();

        ASTNode* op_node = create_node(NODE_BINARY_OP, line, left, right, NULL);
        op_node->ival = op;
        
        return arithExpr_L(op_node);
    } else {
        // Epsilon case
        return left;
    }
}

// sign -> + | -
static NodeType sign(void) {
    if (g_lookahead == TOK_PLUS) {
        match(TOK_PLUS);
        return (NodeType)TOK_PLUS;
    } else if (g_lookahead == TOK_MINUS) {
        match(TOK_MINUS);
        return (NodeType)TOK_MINUS;
    }
    record_error(yylineno, ERROR_SYNTAX, "Expected '+' or '-'");
    return TOK_UNKNOWN;
}

// term -> factor term_L
static ASTNode* term(void) {
    ASTNode* left = factor();
    return term_L(left);
}

// term_L -> multOp factor term_L | ϵ
static ASTNode* term_L(ASTNode* left) {
    // FIRST(multOp) = { *, /, and }
    if (g_lookahead == TOK_MULTIPLY || g_lookahead == TOK_DIVIDE || g_lookahead == TOK_AND) {
        int line = yylineno;
        NodeType op = multOp();
        ASTNode* right = factor();
        
        ASTNode* op_node = create_node(NODE_BINARY_OP, line, left, right, NULL);
        op_node->ival = op;
        
        return term_L(op_node);
    } else {
        // Epsilon case
        return left;
    }
}

// factor -> varOrFunc_Head factor_tail | intLit | floatLit | ( arithExpr ) | not factor | sign factor
static ASTNode* factor(void) {
    int line = yylineno;
    
        // varOrFunc_Head

        // Check for low-ASCII tokens (no switch needed)
    if (g_lookahead == TOK_LPAREN) {
        match(TOK_LPAREN);
        ASTNode* n = expr();
        match(TOK_RPAREN);
        return n;
    } else if (g_lookahead == TOK_NOT) {
        match(TOK_NOT);
        ASTNode* operand = factor();
        ASTNode* op_node = create_node(NODE_UNARY_OP, line, operand, NULL, NULL);
        op_node->ival = TOK_NOT;
        return op_node;
  
    } else if (g_lookahead == TOK_PLUS || g_lookahead == TOK_MINUS) {
        NodeType op = sign();
        ASTNode* operand = factor();
        ASTNode* op_node = create_node(NODE_UNARY_OP, line, operand, NULL, NULL);
        op_node->ival = op;
        return op_node;
        
    }


    switch (g_lookahead) {

        case TOK_ID:
        case TOK_SELF:
        {
            ASTNode* head = varOrFunc_Head();
            return factor_tail(head);
        }
        
        case TOK_INTLIT:
        {
            ASTNode* n = create_leaf_intlit(line, atoi(g_current_token.lexeme), g_current_token.lexeme);
            advance();
            return n;
        }
        
        case TOK_FLOATLIT:
        {
            ASTNode* n = create_leaf_floatlit(line, atof(g_current_token.lexeme), g_current_token.lexeme);
            advance();
            return n;
        }
                     
        default:
            record_error(line, ERROR_SYNTAX, "Expected a factor (literal, id, expression)");
            advance();
            return NULL;
    }
}

// factor_tail -> indice_ZM | ( aParams ) | ϵ
static ASTNode* factor_tail(ASTNode* head) {
    int line = head->lineno;
    // FIRST(indice_ZM) = { [ }
    if (g_lookahead == TOK_LBRACKET) {
        ASTNode* indices = indice_ZM();
        
        // Build array access chain
        ASTNode* current_index = indices;
        ASTNode* var_node = head;
        while(current_index) {
            var_node = create_node(NODE_ARRAY_ACCESS, line, var_node, current_index->left, NULL); // indice's expr is in left
            current_index = current_index->next_sibling;
        }
        return var_node;
    }
    // FIRST( (aParams) ) = { ( }
    else if (g_lookahead == TOK_LPAREN) {
        match(TOK_LPAREN);
        ASTNode* args = aParams();
        match(TOK_RPAREN);
        
        // left=func_head, right=args
        return create_node(NODE_FUNC_CALL, line, head, args, NULL);
    }
    else {
        // Epsilon case
        return head; // Just return the varOrFunc_Head
    }
}

// variable -> idnest_ZM id indice_ZM
static ASTNode* variable(void) {
    int line = yylineno;
    // This is varOrFunc_Head + indice_ZM
    ASTNode* head = varOrFunc_Head();
    ASTNode* indices = indice_ZM();

    if (!indices) {
        return head; // It's just a variable
    }

    // Build array access chain
    ASTNode* current_index = indices;
    ASTNode* var_node = head;
    while(current_index) {
        var_node = create_node(NODE_ARRAY_ACCESS, line, var_node, current_index->left, NULL); // indice's expr is in left
        current_index = current_index->next_sibling;
    }
    return var_node;
}

// idnest_ZM -> idnest idnest_ZM | ϵ
static ASTNode* idnest_ZM(void) {
    // FIRST(idnest) = { id, self }
    if (g_lookahead == TOK_ID || g_lookahead == TOK_SELF) {
        ASTNode* head = idnest();
        ASTNode* tail = idnest_ZM();
        
        // The chain is built in the 'left' child
        if (head) {
           ASTNode* current = head;
           while (current->left) {
               current = current->left;
           }
           current->left = tail;
           return head;
        }
        return tail;
    } else {
        // Epsilon case (FOLLOW = { id })
        return NULL;
    }
}

// indice_ZM -> indice indice_ZM | ϵ
static ASTNode* indice_ZM(void) {
    // FIRST = { [ }
    if (g_lookahead == TOK_LBRACKET) {
        ASTNode* head = indice();
        ASTNode* tail = indice_ZM();
        head->next_sibling = tail;
        return head;
    } else {
        // Epsilon case (FOLLOW = { ., :=, ... })
        return NULL;
    }
}

// idnest -> idOrSelf idnest_L
static ASTNode* idnest(void) {
    ASTNode* id = idOrSelf();
    return idnest_L(id);
}

// idnest_L -> indice_ZM . | ( aParams ) .
static ASTNode* idnest_L(ASTNode* idOrSelf) {
    int line = idOrSelf->lineno;
    
    // FIRST(indice_ZM) = { [ }
    // FIRST( (aParams) ) = { ( }
    
    if (g_lookahead == TOK_LBRACKET) {
        // Array access: id[...][...].
        ASTNode* indices = indice_ZM();
        match(TOK_DOT);
        
        // Build array access chain
        ASTNode* current_index = indices;
        ASTNode* var_node = idOrSelf;
        while(current_index) {
            var_node = create_node(NODE_ARRAY_ACCESS, line, var_node, current_index->left, NULL);
            current_index = current_index->next_sibling;
        }
        
        // Wrap in a MEMBER_ACCESS, right child is the ID to be parsed next
        // The parser for idnest_ZM will append to the left
        return create_node(NODE_MEMBER_ACCESS, line, NULL, var_node, NULL); 
        
    } else if (g_lookahead == TOK_LPAREN) {
        // Function call: id(params).
        match(TOK_LPAREN);
        ASTNode* args = aParams();
        match(TOK_RPAREN);
        match(TOK_DOT);
        
        ASTNode* call_node = create_node(NODE_FUNC_CALL, line, idOrSelf, args, NULL);
        
        // Wrap in a MEMBER_ACCESS
        return create_node(NODE_MEMBER_ACCESS, line, NULL, call_node, NULL);

    } else {
        record_error(line, ERROR_SYNTAX, "Expected array index '[]' or function call '()' before '.'");
        advance();
        return NULL;
    }
}

// idOrSelf -> id | self
static ASTNode* idOrSelf(void) {
    int line = yylineno;
    if (g_lookahead == TOK_ID) {
        ASTNode* n = create_leaf_id(line, g_current_token.lexeme);
        advance();
        return n;
    } else if (g_lookahead == TOK_SELF) {
        ASTNode* n = create_node(NODE_SELF, line, NULL, NULL, NULL);
        advance();
        return n;
    }
    
    record_error(line, ERROR_SYNTAX, "Expected 'id' or 'self'");
    advance();
    return NULL;
}

// indice -> [ arithExpr ]
static ASTNode* indice(void) {
    int line = yylineno;
    match(TOK_LBRACKET);
    ASTNode* exp = arithExpr();
    match(TOK_RBRACKET);
    
    // We create a dummy node. The list builder (indice_ZM) will use it.
    // We store the expression in the 'left' child.
    return create_node(NODE_ARRAY_ACCESS, line, exp, NULL, NULL);
}

// arraySize -> [ arraySize_L
static ASTNode* arraySize(void) {
    //int line = yylineno;
    match(TOK_LBRACKET);
    return arraySize_L();
}

// arraySize_L -> intLit ] | ]
static ASTNode* arraySize_L(void) {
    int line = yylineno;
    if (g_lookahead == TOK_INTLIT) {
        ASTNode* size_expr = create_leaf_intlit(line, atoi(g_current_token.lexeme), g_current_token.lexeme);
        advance();
        match(TOK_RBRACKET);
        // left=size_expr
        return create_node(NODE_TYPE_ARRAY, line, size_expr, NULL, NULL);
    } else if (g_lookahead == TOK_RBRACKET) {
        match(TOK_RBRACKET);
        // No size (e.g., in parameter)
        return create_node(NODE_TYPE_ARRAY, line, NULL, NULL, NULL);
    }
    
    record_error(line, ERROR_SYNTAX, "Expected 'intLit' or ']'");
    advance();
    return NULL;
}

// type -> integer | float | id
static ASTNode* type(void) {
    int line = yylineno;
    if (g_lookahead == TOK_INTEGER) {
        advance();
        return create_node(NODE_TYPE_INT, line, NULL, NULL, NULL);
    } else if (g_lookahead == TOK_FLOAT) {
        advance();
        return create_node(NODE_TYPE_FLOAT, line, NULL, NULL, NULL);
    } else if (g_lookahead == TOK_ID) {
        ASTNode* id_node = create_leaf_id(line, g_current_token.lexeme);
        advance();
        // left=id
        return create_node(NODE_TYPE_ID, line, id_node, NULL, NULL);
    }
    
    record_error(line, ERROR_SYNTAX, "Expected 'integer', 'float', or 'id' as a type");
    advance();
    return NULL;
}

// returnType -> type | void
static ASTNode* returnType(void) {
    int line = yylineno;
    if (g_lookahead == TOK_VOID) {
        advance();
        return create_node(NODE_TYPE_VOID, line, NULL, NULL, NULL);
    } else {
        // FIRST(type) = { integer, float, id }
        return type();
    }
}

// fParams -> id : type arraySize_ZM fParamsTail_ZM | ϵ
static ASTNode* fParams(void) {
    // FIRST = { id }
    if (g_lookahead == TOK_ID) {
        int line = yylineno;
        ASTNode* param_id = create_leaf_id(yylineno, g_current_token.lexeme);
        match(TOK_ID);
        
        match(TOK_COLON);
        ASTNode* param_type = type();
        
        // Handle array types
        ASTNode* arrays = arraySize_ZM();
        if (arrays) {
            ASTNode* base_type = param_type;
            ASTNode* current_array = arrays;
            while (current_array) {
                current_array->left = base_type;
                base_type = current_array;
                current_array = current_array->next_sibling;
            }
            param_type = base_type;
        }
        
        // Create the param node (which is a var_decl)
        ASTNode* head = create_node(NODE_VAR_DECL, line, param_id, param_type, NULL);
        
        ASTNode* tail = fParamsTail_ZM();
        head->next_sibling = tail;
        
        // Wrap in a PARAM_LIST
        return create_node(NODE_PARAM_LIST, line, head, NULL, NULL);
    } else {
        // Epsilon case (FOLLOW = { ) })
        return NULL;
    }
}

// fParamsTail_ZM -> fParamsTail fParamsTail_ZM | ϵ
static ASTNode* fParamsTail_ZM(void) {
    // FIRST = { , }
    if (g_lookahead == TOK_COMMA) {
        ASTNode* head = fParamsTail();
        ASTNode* tail = fParamsTail_ZM();
        head->next_sibling = tail;
        return head;
    } else {
        // Epsilon case (FOLLOW = { ) })
        return NULL;
    }
}

// aParams -> expr aParamsTail_ZM | ϵ
static ASTNode* aParams(void) {
    // FIRST(expr) = { id, self, intLit, floatLit, (, not, +, - }
    if (g_lookahead == TOK_ID || g_lookahead == TOK_SELF ||
        g_lookahead == TOK_INTLIT || g_lookahead == TOK_FLOATLIT ||
        g_lookahead == TOK_LPAREN || g_lookahead == TOK_NOT ||
        g_lookahead == TOK_PLUS || g_lookahead == TOK_MINUS)
    {
        int line = yylineno;
        ASTNode* head = expr();
        ASTNode* tail = aParamsTail_ZM();
        head->next_sibling = tail;
        
        // Wrap in an ARG_LIST
        return create_node(NODE_ARG_LIST, line, head, NULL, NULL);
    } else {
        // Epsilon case (FOLLOW = { ) })
        return NULL;
    }
}

// aParamsTail_ZM -> aParamsTail aParamsTail_ZM | ϵ
static ASTNode* aParamsTail_ZM(void) {
    // FIRST = { , }
    if (g_lookahead == TOK_COMMA) {
        ASTNode* head = aParamsTail();
        ASTNode* tail = aParamsTail_ZM();
        head->next_sibling = tail;
        return head;
    } else {
        // Epsilon case (FOLLOW = { ) })
        return NULL;
    }
}

// fParamsTail -> , id : type arraySize_ZM
static ASTNode* fParamsTail(void) {
    match(TOK_COMMA);
    int line = yylineno;
    
    ASTNode* param_id = create_leaf_id(yylineno, g_current_token.lexeme);
    match(TOK_ID);
    
    match(TOK_COLON);
    ASTNode* param_type = type();
    
    // Handle array types
    ASTNode* arrays = arraySize_ZM();
    if (arrays) {
        ASTNode* base_type = param_type;
        ASTNode* current_array = arrays;
        while (current_array) {
            current_array->left = base_type;
            base_type = current_array;
            current_array = current_array->next_sibling;
        }
        param_type = base_type;
    }
    
    // Create the param node (a var_decl)
    return create_node(NODE_VAR_DECL, line, param_id, param_type, NULL);
}

// aParamsTail -> , expr
static ASTNode* aParamsTail(void) {
    match(TOK_COMMA);
    return expr();
}

// assignOp -> :=
static NodeType assignOp(void) {
    match(TOK_ASSIGN);
    return (NodeType)TOK_ASSIGN;
}

// relOp -> == | < | > | <> | <= | >=
static NodeType relOp(void) {


    // Use if/else if for single-character relational operators
    if (g_lookahead == TOK_LT) {
        match(TOK_LT); 
        return (NodeType)TOK_LT; // Returns ASCII 60
    } else if (g_lookahead == TOK_GT) {
        match(TOK_GT); 
        return (NodeType)TOK_GT; // Returns ASCII 62
    }
    // Compound checks (High-numbered TOK_ constants)
    else if (g_lookahead == TOK_EQ) { 
        match(TOK_EQ); 
        return (NodeType)TOK_EQ;
    } else if (g_lookahead == TOK_NEQ) {
        match(TOK_NEQ); 
        return (NodeType)TOK_NEQ;
    } else if (g_lookahead == TOK_LE) {
        match(TOK_LE); 
        return (NodeType)TOK_LE;
    } else if (g_lookahead == TOK_GE) {
        match(TOK_GE); 
        return (NodeType)TOK_GE;
    }

    record_error(yylineno, ERROR_SYNTAX, "Expected relational operator");
    advance();
    return TOK_UNKNOWN;

}

// addOp -> + | - | or
static NodeType addOp(void) {

    if (g_lookahead == TOK_PLUS) {
        match(TOK_PLUS); 
        return (NodeType)TOK_PLUS;
    } else if (g_lookahead == TOK_MINUS) {
        match(TOK_MINUS); 
        return (NodeType)TOK_MINUS;
    } else if (g_lookahead == TOK_OR) { 
        // TOK_OR is a high-numbered constant, safe to keep as a case/if
        match(TOK_OR); 
        return (NodeType)TOK_OR;
    }
// Original error handling
    record_error(yylineno, ERROR_SYNTAX, "Expected additive operator (+, -, or)");
    advance();
    return TOK_UNKNOWN;

}

// multOp -> * | / | and
static NodeType multOp(void) {  

   if (g_lookahead == TOK_MULTIPLY) { // ASCII 42
        match(TOK_MULTIPLY); 
        return (NodeType)TOK_MULTIPLY;
    } else if (g_lookahead == TOK_DIVIDE) { // ASCII 47
        match(TOK_DIVIDE); 
        return (NodeType)TOK_DIVIDE;
    } else if (g_lookahead == TOK_AND) { // High-numbered constant
        match(TOK_AND); 
        return (NodeType)TOK_AND;
    }
    
    record_error(yylineno, ERROR_SYNTAX, "Expected multiplicative operator (*, /, or and)");
    advance();
    return TOK_UNKNOWN;

}


// --- Helper Functions for Code Generation Orchestration ---

/**
 * @brief Checks if a TAC instruction is a leader (start of a Basic Block).
 * @param instr The current TAC instruction.
 * @param is_first True if this is the very first instruction of the program.
 */
bool is_leader(TACInstruction *instr, bool is_first) {
    // Rule 1: The first instruction is a leader.
    if (is_first) return true;
    
    // Rule 2 & 3: Target of jump, or instruction following a jump.
    // NOTE: This simple version relies on explicit jump targets (labels).
    if (instr->is_target) return true; // Assuming TACInstruction has 'is_target' flag
    if (instr->op == TAC_JUMP || instr->op == TAC_IFGOTO) return true;
    
    // In a real implementation, we'd need to track jump targets first.
    return false;
}

/**
 * @brief Partitions a stream of TAC instructions into Basic Blocks.
 * @param tac_list The head of the sequential list of all TAC instructions.
 * @return A list/array of pointers to the start of each Basic Block.
 */
TACInstruction **partition_into_basic_blocks(TACInstruction *tac_list) {
    // This is a simplified list implementation. A real one might use vectors/dynamic arrays.
    static TACInstruction *bb_list[MAX_BLOCKS]; 
    int bb_count = 0;
    TACInstruction *current = tac_list;
    bool is_first = true;

    while (current != NULL) {
        if (is_first || is_leader(current, false)) {
            if (bb_count < MAX_BLOCKS) {
                bb_list[bb_count++] = current;
                current->is_leader = true; // Mark as start of BB
            } else {
                fprintf(stderr, "Error: Too many basic blocks.\n");
                break;
            }
        }

        // Rule 3: The instruction following a jump is a leader.
        if (current->op == TAC_JUMP || current->op == TAC_IFGOTO) {
            if (current->next != NULL && !current->next->is_leader) {
                current->next->is_target = true; // Simulating pre-analysis for rule 2
            }
        }
        
        current = current->next;
        is_first = false;
    }

    if (bb_count == 0 && tac_list != NULL) {
        // Should not happen if Rule 1 is correctly implemented, but safety net.
        bb_list[bb_count++] = tac_list;
    }
    
    bb_list[bb_count] = NULL; // Null-terminate the list of BBs
    return bb_list;
}

// --- Main Compilation Orchestration Function ---

ASTNode *Program() {
    ASTNode *root;
    TACInstruction *tac_list;
    TACInstruction **basic_blocks;

    // 1. Syntax Analysis & AST Construction (Recursive Descent)
    root = StmtList(); // Assuming program starts with a statement list

    // 2. Semantic Analysis & Type Checking
    // NOTE: This must be run on the complete AST before ICG.
    // Assume check_sem() also computes live/next-use information if needed.
    // check_sem(root); 

    // 3. Intermediate Code Generation (Sequential TAC Stream)
    // Assume tac_list is generated by traversing the AST (e.g., a function in ast.c)
    // tac_list = Generate_Sequential_TAC(root);
    
    // Placeholder for TAC generation.
    // In reality, this function would traverse the AST and return a linked list of TAC.
    tac_list = (TACInstruction *)malloc(sizeof(TACInstruction)); 
    tac_list->op = TAC_LABEL;
    tac_list->next = NULL;

    // 4. Basic Block Partitioning 
    // The control flow is simplified into basic blocks for optimization.
    basic_blocks = partition_into_basic_blocks(tac_list);

    // 5. Code Generation (Initiate for each Basic Block)
    // This calls the function defined in codegen.c for each block sequentially.
    for (int i = 0; basic_blocks[i] != NULL; i++) {
        CodeGen_BasicBlock(basic_blocks[i]);
    }
    
    // (Final step: Emit HALT instruction or jump to exit)
    emit_asm("\tHALT");

    return root;
}

// --- Placeholder/Example Function for Assembly Emit (Needed by CodeGen) ---

void emit_asm(const char* format, ...) {
    // This should print the generated assembly code to the desired output stream
    va_list args;
    va_start(args, format);
    vfprintf(stdout, format, args);
    fprintf(stdout, "\n");
    va_end(args);
}