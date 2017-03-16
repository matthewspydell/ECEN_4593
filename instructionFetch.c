/* @author matthewspydell
 *
 */

#include "instructionFetch.h"

void instructionFetch(uint32_t* pc) {
  IF_ID.instructionShadow = *pc;
}
