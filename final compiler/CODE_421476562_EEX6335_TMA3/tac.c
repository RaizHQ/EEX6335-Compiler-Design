// tac.c - Implementation of sequential Three-Address Code (TAC) generation
// This file assumes definitions from compiler.h, ast.h, and tac.h are available.

#include "compiler.h"
#include "ast.h"
#include "tac.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Global counter for generating unique temporary variables (t1, t2, ...)
static int temp_counter = 0;

// Global counter for generating unique instruction labels (L1, L2, ...)
static int label_counter = 0; 

// --- TAC Instruction Utility Functions ---

/**
 * @brief Generates a unique temporary variable name.
 */
char *new_temp_name() {
    char *name = (char *)malloc(MAX_VAR_NAME_LEN);
    if (!name) return NULL;
    sprintf(name, "t%d", ++temp_counter);
    return name;
}

/**
 * @brief Generates a unique label name.
 */
char *new_label_name() {
    char *name = (char *)malloc(MAX_VAR_NAME_LEN);
    if (!name) return NULL;
    sprintf(name, "L%d", ++label_counter);
    return name;
}

/**
 * @brief Creates a new TAC instruction node.
 */
TACInstruction *new_tac(TACOp op, char *result, char *op1, char *op2) {
    TACInstruction *instr = (TACInstruction *)malloc(sizeof(TACInstruction));
    if (!instr) return NULL;
    
    memset(instr, 0, sizeof(TACInstruction));
    instr->op = op;
    // Note: strdup is needed to ensure the instruction owns the string data
    instr->result = result ? strdup(result) : NULL;
    instr->op1 = op1 ? strdup(op1) : NULL;
    instr->op2 = op2 ? strdup(op2) : NULL;
    
    instr->is_target = false;
    instr->is_leader = false;
    
    return instr;
}

/**
 * @brief Appends a new TAC instruction to the end of a list, updating links.
 */
TACInstruction *append_tac(TACInstruction *head, TACInstruction *new_instr) {
    if (new_instr == NULL) return head;

    if (head == NULL) return new_instr;
    
    TACInstruction *current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_instr;
    new_instr->prev = current;
    return head;
}

// --- Recursive Generation Logic ---

// Forward declaration of the recursive functions
static TACInstruction *tac_gen_expr(ASTNode *node, char **result_name);
static TACInstruction *tac_gen_stmt(ASTNode *node);

/**
 * @brief Generates TAC for an expression (returns instruction list and result name).
 * * For expression nodes, the goal is to evaluate operands into temporaries, 
 * then generate the final operation, storing the result in a new temporary.
 */
static TACInstruction *tac_gen_expr(ASTNode *node, char **result_name) {
    if (node == NULL) { *result_name = NULL; return NULL; }

    // 1. Leaf Nodes (ID, NUM)
    if (node->type == NODE_ID) {
        *result_name = strdup(node->u.id.name); // Use ID name as address/operand
        return NULL;
    }
    if (node->type == NODE_NUM) {
        // Constants are passed as string literals (e.g., "#5")
        *result_name = (char *)malloc(16);
        sprintf(*result_name, "#%d", node->u.num.ival);
        return NULL; 
    }

    // 2. Binary Operation Nodes (Arithmetic)
    if (node->type == NODE_BINOP) {
        char *left_name, *right_name;
        TACInstruction *left_tac = tac_gen_expr(node->u.expr_op.left, &left_name);
        TACInstruction *right_tac = tac_gen_expr(node->u.expr_op.right, &right_name);
        
        TACOp op_type;
        char *op_str = node->u.expr_op.op;

        if (strcmp(op_str, "+") == 0) op_type = TAC_PLUS;
        else if (strcmp(op_str, "-") == 0) op_type = TAC_MINUS;
        else if (strcmp(op_str, "*") == 0) op_type = TAC_MUL;
        else if (strcmp(op_str, "/") == 0) op_type = TAC_DIV;
        else { /* Handle Relational Operators (RELOP) separately */ op_type = TAC_COPY; }

        *result_name = new_temp_name();
        TACInstruction *new_instr = new_tac(op_type, *result_name, left_name, right_name);
        
        // Combine TAC: left_tac -> right_tac -> new_instr
        TACInstruction *head = left_tac;
        head = append_tac(head, right_tac);
        head = append_tac(head, new_instr);
        
        return head;
    }

    // --- Add logic for NODE_UNARY, NODE_RELOP, etc. here ---
    
    *result_name = NULL;
    return NULL;
}

/**
 * @brief Generates TAC for a statement (returns the instruction list).
 */
static TACInstruction *tac_gen_stmt(ASTNode *node) {
    if (node == NULL) return NULL;
    
    TACInstruction *head = NULL;

    switch (node->type) {
        case NODE_STMT_LIST:
            head = tac_gen_stmt(node->u.stmt_list.body);
            head = append_tac(head, tac_gen_stmt(node->u.stmt_list.next));
            break;

        case NODE_ASSIGN: {
            // Assignment: LHS = RHS
            char *result_name;
            head = tac_gen_expr(node->u.expr_op.right, &result_name);
            
            // Assignment TAC: result_name (from RHS) is copied to LHS (ID name)
            char *lhs_name = node->u.expr_op.left->u.id.name;
            TACInstruction *assign_instr = new_tac(TAC_COPY, lhs_name, result_name, NULL);
            head = append_tac(head, assign_instr);
            break;
        }
        
        case NODE_IF: {
            // If-Else: IF (cond) stmt_true [ELSE stmt_false]
            char *label_true = new_label_name();
            char *label_false = node->u.if_stmt.stmt_false ? new_label_name() : new_label_name();
            char *label_next = new_label_name();

            // 1. Condition and Jump
            char *cond_name;
            head = tac_gen_expr(node->u.if_stmt.cond, &cond_name);
            
            TACInstruction *if_instr = new_tac(TAC_IFGOTO, cond_name, label_true, NULL);
            // Assuming expression result is guaranteed to be boolean/numeric 0/1
            
            // This is simplified: IFGOTO should be conditional on boolean result.
            // For now, assume if(t_i) GOTO L_true, and fallback GOTO L_false.
            if_instr->op2 = strdup(label_true); // Using op2 for the true target label
            head = append_tac(head, if_instr);
            
            // Fallthrough to the false path or direct GOTO to it
            head = append_tac(head, new_tac(TAC_GOTO, label_false, NULL, NULL)); 
            
            // 2. True Block
            head = append_tac(head, new_tac(TAC_LABEL, label_true, NULL, NULL));
            head = append_tac(head, tac_gen_stmt(node->u.if_stmt.stmt_true));
            
            // 3. False Block
            if (node->u.if_stmt.stmt_false) {
                head = append_tac(head, new_tac(TAC_GOTO, label_next, NULL, NULL));
                head = append_tac(head, new_tac(TAC_LABEL, label_false, NULL, NULL));
                head = append_tac(head, tac_gen_stmt(node->u.if_stmt.stmt_false));
            } else {
                // If no else, L_false is the L_next label
                head = append_tac(head, new_tac(TAC_LABEL, label_false, NULL, NULL));
            }
            
            // 4. Next (Exit)
            head = append_tac(head, new_tac(TAC_LABEL, label_next, NULL, NULL));
            break;
        }

        // --- Add other statement logic here (WHILE, DO, BREAK) ---

        default:
            break;
    }
    return head;
}

// --- Top-Level Entry Point ---

/**
 * @brief Top-level function to generate the complete sequential TAC stream.
 */
TACInstruction *Generate_Sequential_TAC(ASTNode *root) {
    if (root == NULL || root->type != NODE_PROGRAM) return NULL;
    
    char *label_begin = new_label_name();
    char *label_exit = new_label_name();
    
    // Start with the BEGIN label
    TACInstruction *head = new_tac(TAC_LABEL, label_begin, NULL, NULL);
    
    // Generate TAC for all statements
    TACInstruction *stmt_tac = tac_gen_stmt(root->u.stmt_list.body);
    head = append_tac(head, stmt_tac);
    
    // End with the EXIT label
    head = append_tac(head, new_tac(TAC_LABEL, label_exit, NULL, NULL));
    
    return head;
}

// Note: Function new_tac and append_tac from the previous step are included here
// or assumed to be linked correctly.