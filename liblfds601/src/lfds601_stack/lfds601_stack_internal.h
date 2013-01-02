/***** the library wide include file *****/
#include "lfds601_internal.h"

/***** pragmas *****/

/***** defines *****/
#define LFDS601_STACK_POINTER 0
#define LFDS601_STACK_COUNTER 1
#define LFDS601_STACK_PAC_SIZE 2

/***** structures *****/
#pragma pack( push, LFDS601_ALIGN_DOUBLE_POINTER )

struct lfds601_stack_state
{
  // TRD : must come first for alignment
  struct lfds601_stack_element
    *volatile top[LFDS601_STACK_PAC_SIZE];

  lfds601_atom_t
    aba_counter;

  struct lfds601_freelist_state
    *fs;
};

struct lfds601_stack_element
{
  struct lfds601_stack_element
    *next[LFDS601_STACK_PAC_SIZE];

  struct lfds601_freelist_element
    *fe;

  void
    *user_data;
};

#pragma pack( pop )

/***** private prototypes *****/
int lfds601_stack_internal_freelist_init_function( void **user_data, void *user_state );
void lfds601_stack_internal_freelist_delete_function( void *user_data, void *user_state );

void lfds601_stack_internal_new_element_from_freelist( struct lfds601_stack_state *ss, struct lfds601_stack_element *se[LFDS601_STACK_PAC_SIZE], void *user_data );
void lfds601_stack_internal_new_element( struct lfds601_stack_state *ss, struct lfds601_stack_element *se[LFDS601_STACK_PAC_SIZE], void *user_data );
void lfds601_stack_internal_init_element( struct lfds601_stack_state *ss, struct lfds601_stack_element *se[LFDS601_STACK_PAC_SIZE], struct lfds601_freelist_element *fe, void *user_data );

void lfds601_stack_internal_push( struct lfds601_stack_state *ss, struct lfds601_stack_element *se[LFDS601_STACK_PAC_SIZE] );

