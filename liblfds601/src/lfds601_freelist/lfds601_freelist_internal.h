/***** the library wide include file *****/
#include "lfds601_internal.h"

/***** defines *****/
#define LFDS601_FREELIST_POINTER 0
#define LFDS601_FREELIST_COUNTER 1
#define LFDS601_FREELIST_PAC_SIZE 2

/***** structures *****/
#pragma pack( push, LFDS601_ALIGN_DOUBLE_POINTER )

struct lfds601_freelist_state
{
  struct lfds601_freelist_element
    *volatile top[LFDS601_FREELIST_PAC_SIZE];

  int
    (*user_data_init_function)( void **user_data, void *user_state );

  void
    *user_state;

  lfds601_atom_t
    aba_counter,
    element_count;
};

struct lfds601_freelist_element
{
  struct lfds601_freelist_element
    *next[LFDS601_FREELIST_PAC_SIZE];

  void
    *user_data;
};

#pragma pack( pop )

/***** private prototypes *****/
lfds601_atom_t lfds601_freelist_internal_new_element( struct lfds601_freelist_state *fs, struct lfds601_freelist_element **fe );
void lfds601_freelist_internal_validate( struct lfds601_freelist_state *fs, struct lfds601_validation_info *vi, enum lfds601_data_structure_validity *lfds601_freelist_validity );

