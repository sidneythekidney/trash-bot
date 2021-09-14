#include "defs.h"
#include "initialize.h"

class Generate{
    private:
        U64 getBishopAttacks(U64 blockers, uint8_t sq);
        U64 getRookAttacks(U64 blockers, uint8_t sq);
};
    
