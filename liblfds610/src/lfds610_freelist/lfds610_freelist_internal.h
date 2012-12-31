/***** the library wide include file *****/
#include "liblfds610_internal.h"

/***** defines *****/
#define LFDS610_FREELIST_POINTER 0
#define LFDS610_FREELIST_COUNTER 1
#define LFDS610_FREELIST_PAC_SIZE 2

/***** structures *****/
#pragma pack( push, LFDS610_ALIGN_DOUBLE_POINTER )

struct lfds610_freelist_state
{
  struct lfds610_freelist_element
    *volatile top[LFDS610_FREELIST_PAC_SIZE];

  int
    (*user_data_init_function)( void **user_data, void *user_state );

  void
    *user_state;

  lfds610_atom_t
    aba_counter,
    element_count;
};

struct lfds610_freelist_element
{
  struct lfds610_freelist_element
    *next[LFDS610_FREELIST_PAC_SIZE];

  void
    *user_data;
};

#pragma pack( pop )

/***** private prototypes *****/
lfds610_atom_t lfds610_freelist_internal_new_element( struct lfds610_freelist_state *fs, struct lfds610_freelist_element **fe );
void lfds610_freelist_internal_validate( struct lfds610_freelist_state *fs, struct lfds610_validation_info *vi, enum lfds610_data_structure_validity *lfds610_freelist_validity );

