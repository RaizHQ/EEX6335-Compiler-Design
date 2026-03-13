// codegen.c - Simple Code Generator with Local Register Allocation

#include "compiler.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// --- Global Descriptors (Simulated) ---
// Note: In a full project, these would be managed via data structures 
// that interface with the symbol table and register file.
AddrDescriptor ADT[MAX_VARIABLES]; // Maps var name/id to its location(s)
RegDescriptor RDT[R_COUNT];        // Maps register index to var name(s)

// --- Helper Functions (Simulated Stubs) ---

// Emits a single assembly instruction.
void emit_asm(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    printf("\n");
    va_end(args);
}

// Checks global flow analysis results (simplified stub).
// For local allocation, checks if variable is used later in the block.
bool is_live_on_exit(const char* var_name) {
    // Placeholder logic: assume all source variables are live on exit for safety
    return var_name[0] != 't'; // Assume temps are not live outside the block
}

// Retrieves the memory location string (e.g., "[RBP - 16]" or "global_var")
// from the symbol table or descriptor.
const char* get_memory_location(const char* var_name) {
    // In a real implementation, this would retrieve the offset and format the string.
    if (var_name[0] == 't') return "TEMP_SPILL_LOC"; // Example spill location
    return var_name; // Assuming source variables are globally addressable for simplicity
}

// Finds the first register holding a variable's value, or NULL.
int find_register_for_var(const char* var_name) {
    for (int r = 0; r < R_COUNT; r++) {
        // Simplified check: if ADT says var is in register r
        // In a full implementation, check RDT[r] for var_name
        // For this example, we assume ADT provides the best location
    }
    return -1; // Not found
}


// --- Core Register Management ---

/**
 * @brief Selects and manages registers for an operand.
 * @param op_name The name of the operand (y or z).
 * @param is_dest True if this is the destination (x), false otherwise.
 * @param needed_later True if op_name is live after the current instruction.
 * @param preferred_reg_id If > -1, try to use this register first.
 * @return The ID of the physical register allocated.
 */
int getReg(const char* op_name, bool is_dest, bool needed_later) {
    int r = -1;

    // 1. Optimal Reuse: Check if the operand is already in a register.
    // (Simplification: In a full ADT, we would check the set of locations).
    // For now, we rely on ADT/RDT simulation logic.

    // Try to find a register that already holds the operand's value
    for (int i = 0; i < R_COUNT; i++) {
        if (strcmp(RDT[i].var_name, op_name) == 0) {
            r = i;
            break;
        }
    }

    if (r != -1) {
        // Register found, but handle potential re-use for destination
        if (is_dest && !needed_later) {
            // It's the destination, and the old value isn't needed: OK to reuse
        }
        return r;
    }
    
    // 2. Find an empty or dead register (Greedy Strategy).
    for (int i = 0; i < R_COUNT; i++) {
        if (RDT[i].is_empty) {
            r = i; // Empty register found
            break;
        }
        if (!RDT[i].needed_later && !is_dest) {
            // Reuse dead register (complex logic simplified here)
            // In a full model, this checks if ALL variables in RDT[i] are dead.
            r = i;
            // (Spill code would go here if needed, but we assume it's safe for dead vars)
            break;
        }
    }

    if (r != -1) {
        // If a register was selected for a source, load the value.
        if (!is_dest) {
             emit_asm("\tMOV R%d, %s", r, get_memory_location(op_name));
        }
        // Update RDT and ADT (simplified)
        // RDT[r].var_name = op_name; RDT[r].is_empty = false;
        // ADT[op_name] = register r;
        return r;
    }

    // 3. Spill a live register (Fallback/Simplification).
    // In a real implementation, complex heuristics are used to pick the best register to spill.
    r = R_COUNT - 1; // Arbitrarily pick the last register to spill

    // In a real spill:
    // char* spilled_var = RDT[r].var_name;
    // emit_asm("\tMOV %s, R%d", get_memory_location(spilled_var), r);
    
    // Load the current operand into the spilled register
    if (!is_dest) {
        emit_asm("\tMOV R%d, %s", r, get_memory_location(op_name));
    }
    
    return r;
}

// --- Main Code Generation Loop (Simulated) ---

void code_gen_basic_block(TACInstruction* block_start) {
    // --- Initialization ---
    // Clear/initialize RDT and ADT for the start of the basic block
    memset(RDT, 0, sizeof(RDT)); 
    // ... Initialize ADT with memory locations for source variables ...
    
    TACInstruction* I = block_start;

    emit_asm("L_BLOCK_%d:", block_start->id);

    // --- Process Instructions ---
    while (I != NULL) {
        // (1) Use data-flow analysis results to determine liveness/next-use 
        // These results would be attached to I or obtained via a lookup function.
        bool y_needed = I->op == TAC_COPY ? is_live_on_exit(I->op1) : I->op1[0] != 't'; // Simplification
        bool z_needed = I->op == TAC_COPY ? false : I->op2[0] != 't'; // Simplification

        // (2) Get registers for y and z (sources)
        int Ry = getReg(I->op1, false, y_needed);
        int Rz = (I->op2) ? getReg(I->op2, false, z_needed) : -1;

        // (3) Get register for x (destination)
        int Rx = getReg(I->result, true, is_live_on_exit(I->result));

        // (4) Generate Instruction
        switch (I->op) {
            case TAC_ADD:
            case TAC_SUB:
                // Move Ry to Rx if they are different (x86-64 destination must be one operand)
                if (Rx != Ry) {
                    emit_asm("\tMOV R%d, R%d", Rx, Ry);
                }
                emit_asm("\t%s R%d, R%d", (I->op == TAC_ADD ? "ADD" : "SUB"), Rx, Rz);
                break;
            case TAC_COPY:
                // Copy is handled implicitly by getReg updates for descriptors
                break;
            // ... other operations like MUL, DIV, JUMP ...
        }

        // (5) Update Descriptors: x now lives in Rx, and potentially memory.
        // ADT[I->result].location = Rx;
        // RDT[Rx].var_name = I->result;
        
        I = I->next;
    }
    
    // --- Basic Block Coda ---
    for (int r = 0; r < R_COUNT; r++) {
        // Spill all live-on-exit variables if they are only in registers
        if (!RDT[r].is_empty && is_live_on_exit(RDT[r].var_name)) {
            // Find the true memory location and store
            emit_asm("\tMOV %s, R%d", get_memory_location(RDT[r].var_name), r);
        }
    }
}

