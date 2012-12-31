/***** the library wide include file *****/
#include "liblfds610_internal.h"

/***** pragmas *****/

/***** defines *****/
#define LFDS610_STACK_POINTER 0
#define LFDS610_STACK_COUNTER 1
#define LFDS610_STACK_PAC_SIZE 2

/***** structures *****/
#pragma pack( push, LFDS610_ALIGN_DOUBLE_POINTER )

struct lfds610_stack_state
{
  // TRD : must come first for alignment
  struct lfds610_stack_element
    *volatile top[LFDS610_STACK_PAC_SIZE];

  lfds610_atom_t
    aba_counter;

  struct lfds610_freelist_state
    *fs;
};

struct lfds610_stack_element
{
  struct lfds610_stack_element
    *next[LFDS610_STACK_PAC_SIZE];

  struct lfds610_freelist_element
    *fe;

  void
    *user_data;
};

#pragma pack( pop )

/***** private prototypes *****/
int lfds610_stack_internal_freelist_init_function( void **user_data, void *user_state );
void lfds610_stack_internal_freelist_delete_function( void *user_data, void *user_state );

void lfds610_stack_internal_new_element_from_freelist( struct lfds610_stack_state *ss, struct lfds610_stack_element *se[LFDS610_STACK_PAC_SIZE], void *user_data );
void lfds610_stack_internal_new_element( struct lfds610_stack_state *ss, struct lfds610_stack_element *se[LFDS610_STACK_PAC_SIZE], void *user_data );
void lfds610_stack_internal_init_element( struct lfds610_stack_state *ss, struct lfds610_stack_element *se[LFDS610_STACK_PAC_SIZE], struct lfds610_freelist_element *fe, void *user_data );

void lfds610_stack_internal_push( struct lfds610_stack_state *ss, struct lfds610_stack_element *se[LFDS610_STACK_PAC_SIZE] );

void lfds610_stack_internal_validate( struct lfds610_stack_state *ss, struct lfds610_validation_info *vi, enum lfds610_data_structure_validity *stack_validity, enum lfds610_data_structure_validity *freelist_validity );

