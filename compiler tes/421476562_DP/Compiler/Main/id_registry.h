#ifndef ID_REGISTRY_H
#define ID_REGISTRY_H

/* Returns the unique integer ID for a given string.
   If seen before, returns the old ID.
   If new, assigns a new ID and returns it. */
int getOrCreateID(const char* lexeme);

#endif