/***** public prototypes *****/
#include "liblfds610.h"

/***** defines *****/
#define and &&
#define or  ||

#define RAISED   1
#define LOWERED  0

#define NO_FLAGS 0x0

/***** private prototypes *****/
void *lfds610_liblfds_aligned_malloc( size_t size, size_t align_in_bytes );
void lfds610_liblfds_aligned_free( void *memory );

static LFDS610_INLINE lfds610_atom_t lfds610_abstraction_cas( volatile lfds610_atom_t *destination, lfds610_atom_t exchange, lfds610_atom_t compare );
static LFDS610_INLINE unsigned char lfds610_abstraction_dcas( volatile lfds610_atom_t *destination, lfds610_atom_t *exchange, lfds610_atom_t *compare );
static LFDS610_INLINE lfds610_atom_t lfds610_abstraction_increment( volatile lfds610_atom_t *value );

/***** inlined code *****/
#include "lfds610_abstraction/lfds610_abstraction_cas.c"
#include "lfds610_abstraction/lfds610_abstraction_dcas.c"
#include "lfds610_abstraction/lfds610_abstraction_increment.c"

