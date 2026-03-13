// ast.c - AST Construction and Sequential TAC Generation
// NOTE: This implementation assumes memory allocation (malloc) is handled correctly.

#include "compiler.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Global counter for generating unique temporary variable names (t1, t2, ...)
static int temp_count = 0;

// Global counter for generating unique instruction labels (L1, L2, ...)
static int label_count = 0; 

// --- AST Node Constructors ---

ASTNode *new_node(ASTNodeType type, int linenum) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    if (!node) { fprintf(stderr, "Memory allocation failed for ASTNode.\n"); exit(1); }
    memset(node, 0, sizeof(ASTNode));
    node->type = type;
    node->linenum = linenum;
    return node;
}

ASTNode *new_program_node(ASTNode *stmt_list) {
    ASTNode *node = new_node(NODE_PROGRAM, 0);
    node->u.stmt_list.body = stmt_list;
    return node;
}

ASTNode *new_stmt_list_node(ASTNode *stmt, ASTNode *next) {
    ASTNode *node = new_node(NODE_STMT_LIST, stmt->linenum);
    node->u.stmt_list.body = stmt;
    node->u.stmt_list.next = next;
    return node;
}

ASTNode *new_assign_node(ASTNode *lhs, ASTNode *rhs) {
    ASTNode *node = new_node(NODE_ASSIGN, lhs->linenum);
    node->u.expr_op.left = lhs;
    node->u.expr_op.right = rhs;
    return node;
}

ASTNode *new_if_node(ASTNode *cond, ASTNode *stmt_true, ASTNode *stmt_false) {
    ASTNode *node = new_node(NODE_IF, cond->linenum);
    node->u.if_stmt.cond = cond;
    node->u.if_stmt.stmt_true = stmt_true;
    node->u.if_stmt.stmt_false = stmt_false;
    return node;
}

ASTNode *new_id_node(char *name) {
    // Note: Type and offset are set during semantic analysis (semantics.c)
    ASTNode *node = new_node(NODE_ID, 0); 
    node->u.id.name = strdup(name);
    return node;
}

ASTNode *new_binop_node(char *op, ASTNode *l, ASTNode *r) {
    ASTNode *node = new_node(NODE_BINOP, l->linenum);
    node->u.expr_op.op = strdup(op);
    node->u.expr_op.left = l;
    node->u.expr_op.right = r;
    return node;
}

ASTNode *new_num_node_int(int val) {
    ASTNode *node = new_node(NODE_NUM, 0);
    node->dataType = TYPE_INT;
    node->u.num.ival = val;
    return node;
}

// --- TAC Generation Helpers ---

/**
 * @brief Generates a unique temporary variable name (e.g., t1, t2).
 */
char *new_temp_name() {
    char *name = (char *)malloc(MAX_VAR_NAME_LEN);
    if (!name) { fprintf(stderr, "Memory allocation failed for temp name.\n"); exit(1); }
    sprintf(name, "t%d", ++temp_count);
    return name;
}

/**
 * @brief Generates a unique label name (e.g., L1, L2).
 */
char *new_label_name() {
    char *name = (char *)malloc(MAX_VAR_NAME_LEN);
    if (!name) { fprintf(stderr, "Memory allocation failed for label name.\n"); exit(1); }
    sprintf(name, "L%d", ++label_count);
    return name;
}

/**
 * @brief Appends a new TAC instruction to the end of a list.
 */
TACInstruction *append_tac(TACInstruction *head, TACInstruction *new_instr) {
    if (head == NULL) return new_instr;
    
    TACInstruction *current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_instr;
    new_instr->prev = current;
    return head;
}

// --- Recursive TAC Generation Functions ---

// Forward declaration of the recursive functions
TACInstruction *codegen_expr(ASTNode *node, char **result_name);
TACInstruction *codegen_stmt(ASTNode *node, char *label_true, char *label_false);

/**
 * @brief Generates TAC for an expression (returns the list head and the result name).
 */
TACInstruction *codegen_expr(ASTNode *node, char **result_name) {
    if (node == NULL) {
        *result_name = NULL;
        return NULL;
    }

    // Leaf Nodes (Constants, IDs)
    if (node->type == NODE_ID) {
        *result_name = node->u.id.name; // Use ID name directly as address
        return NULL; // No TAC generated
    }
    if (node->type == NODE_NUM) {
        // Constants are used directly as operands; store its string representation
        *result_name = (char *)malloc(16);
        if (!(*result_name)) exit(1);
        sprintf(*result_name, "#%d", node->u.num.ival);
        return NULL;
    }

    // Binary Operation Nodes (e.g., ADD, SUB, MUL)
    if (node->type == NODE_BINOP) {
        char *op_str = node->u.expr_op.op;
        TACOp op_type;
        
        // Map operator string to TAC op enum
        if (strcmp(op_str, "+") == 0) op_type = TAC_PLUS;
        else if (strcmp(op_str, "-") == 0) op_type = TAC_MINUS;
        else if (strcmp(op_str, "*") == 0) op_type = TAC_MUL;
        else if (strcmp(op_str, "/") == 0) op_type = TAC_DIV;
        else { /* error */ op_type = TAC_COPY; }

        char *left_name, *right_name;
        TACInstruction *left_tac = codegen_expr(node->u.expr_op.left, &left_name);
        TACInstruction *right_tac = codegen_expr(node->u.expr_op.right, &right_name);
        
        *result_name = new_temp_name();
        TACInstruction *new_instr = new_tac(op_type, *result_name, left_name, right_name);
        
        // Combine TAC: left_tac -> right_tac -> new_instr
        TACInstruction *head = left_tac;
        if (head == NULL) head = right_tac; else head = append_tac(head, right_tac);
        head = append_tac(head, new_instr);
        
        return head;
    }

    // Default: Return NULL if no code is generated
    *result_name = NULL;
    return NULL;
}

/**
 * @brief Generates TAC for a statement (returns the list head).
 */
TACInstruction *codegen_stmt(ASTNode *node, char *label_begin, char *label_next) {
    if (node == NULL) return NULL;
    
    TACInstruction *head = NULL;
    
    if (node->type == NODE_STMT_LIST) {
        head = codegen_stmt(node->u.stmt_list.body, label_begin, label_next);
        head = append_tac(head, codegen_stmt(node->u.stmt_list.next, label_begin, label_next));
        return head;
    }

    if (node->type == NODE_ASSIGN) {
        char *result_name;
        head = codegen_expr(node->u.expr_op.right, &result_name);
        
        // Assignment TAC: LHS = result of RHS (copy operation)
        TACInstruction *assign_instr = new_tac(TAC_COPY, 
                                              node->u.expr_op.left->u.id.name, 
                                              result_name, 
                                              NULL);
        head = append_tac(head, assign_instr);
        return head;
    }

    if (node->type == NODE_IF) {
        // 
        // This is a simplified jumping code implementation (similar to Section 6.6)
        
        char *label_true = new_label_name();
        char *label_false = node->u.if_stmt.stmt_false ? new_label_name() : label_next;
        char *label_next_temp = node->u.if_stmt.stmt_false ? new_label_name() : label_next;

        // 1. Condition
        char *cond_name, *op1, *op2, *relop_str;
        TACInstruction *cond_tac = codegen_expr(node->u.if_stmt.cond, &cond_name);
        
        // Assume condition result is stored in cond_name by expr generation
        TACInstruction *if_instr = new_tac(TAC_IFGOTO, cond_name, label_true, label_false);
        head = append_tac(cond_tac, if_instr);

        // 2. Stmt_True
        TACInstruction *true_head = new_tac(TAC_LABEL, label_true, NULL, NULL);
        head = append_tac(head, true_head);
        
        TACInstruction *stmt1_tac = codegen_stmt(node->u.if_stmt.stmt_true, label_true, label_next_temp);
        head = append_tac(head, stmt1_tac);
        
        // 3. Stmt_False (if exists)
        if (node->u.if_stmt.stmt_false) {
            TACInstruction *goto_next = new_tac(TAC_GOTO, label_next_temp, NULL, NULL);
            head = append_tac(head, goto_next);

            TACInstruction *false_head = new_tac(TAC_LABEL, label_false, NULL, NULL);
            head = append_tac(head, false_head);
            
            TACInstruction *stmt2_tac = codegen_stmt(node->u.if_stmt.stmt_false, label_false, label_next);
            head = append_tac(head, stmt2_tac);
        }

        // 4. Next label
        TACInstruction *next_label_instr = new_tac(TAC_LABEL, label_next_temp, NULL, NULL);
        head = append_tac(head, next_label_instr);
        
        return head;
    }

    // ... other statement types (WHILE, DO_WHILE, etc.) would be here ...

    return NULL;
}

/**
 * @brief Top-level function to generate the complete sequential TAC stream.
 */
TACInstruction *Generate_Sequential_TAC(ASTNode *root) {
    char *label_begin = new_label_name();
    char *label_exit = new_label_name();
    
    TACInstruction *head = new_tac(TAC_LABEL, label_begin, NULL, NULL);
    
    // Assume root->u.stmt_list.body holds the full statement list
    TACInstruction *stmt_tac = codegen_stmt(root->u.stmt_list.body, label_begin, label_exit);
    head = append_tac(head, stmt_tac);
    
    head = append_tac(head, new_tac(TAC_LABEL, label_exit, NULL, NULL));
    // Optional: append HALT instruction if this is the main function
    
    return head;
}