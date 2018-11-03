// Ray Smith - rsmith52 - rays - Nate Hoffman - nhoffman5 -nhoffman
#ifndef BUILDSPECREPR_H_INCLUDED
#define BUILDSPECREPR_H_INCLUDED
/* Include Guards ^^^ */
#include "main.h"
/* Prototypes for Functions */

Spec_Representation * CreateSpec(char ** file_line);

void AddCommand(Spec_Representation * spec_repr, char ** file_line);

#endif
