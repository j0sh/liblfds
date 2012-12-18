/***** the library wide include file *****/
#include "lfds600_internal.h"

/***** defines *****/
#define LFDS600_FREELIST_POINTER 0
#define LFDS600_FREELIST_COUNTER 1
#define LFDS600_FREELIST_PAC_SIZE 2

/***** structures *****/
#pragma pack( push, LFDS600_ALIGN_DOUBLE_POINTER )

struct lfds600_freelist_state
{
  struct lfds600_freelist_element
    *volatile top[LFDS600_FREELIST_PAC_SIZE];

  int
    (*user_data_init_function)( void **user_data, void *user_state );

  void
    *user_state;

  lfds600_atom_t
    aba_counter,
    element_count;
};

struct lfds600_freelist_element
{
  struct lfds600_freelist_element
    *next[LFDS600_FREELIST_PAC_SIZE];

  void
    *user_data;
};

#pragma pack( pop )

/***** private prototypes *****/
lfds600_atom_t lfds600_freelist_internal_new_element( struct lfds600_freelist_state *fs, struct lfds600_freelist_element **fe );
void lfds600_freelist_internal_validate( struct lfds600_freelist_state *fs, struct lfds600_validation_info *vi, enum data_structure_validity *lfds600_freelist_validity );

