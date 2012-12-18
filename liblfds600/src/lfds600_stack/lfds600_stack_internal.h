/***** the library wide include file *****/
#include "lfds600_internal.h"

/***** pragmas *****/

/***** defines *****/
#define LFDS600_STACK_POINTER 0
#define LFDS600_STACK_COUNTER 1
#define LFDS600_STACK_PAC_SIZE 2

/***** structures *****/
#pragma pack( push, LFDS600_ALIGN_DOUBLE_POINTER )

struct lfds600_stack_state
{
  // TRD : must come first for alignment
  struct lfds600_stack_element
    *volatile top[LFDS600_STACK_PAC_SIZE];

  lfds600_atom_t
    aba_counter;

  struct lfds600_freelist_state
    *fs;
};

struct lfds600_stack_element
{
  struct lfds600_stack_element
    *next[LFDS600_STACK_PAC_SIZE];

  struct lfds600_freelist_element
    *fe;

  void
    *user_data;
};

#pragma pack( pop )

/***** private prototypes *****/
int lfds600_stack_internal_freelist_init_function( void **user_data, void *user_state );
void lfds600_stack_internal_freelist_delete_function( void *user_data, void *user_state );

void lfds600_stack_internal_new_element_from_freelist( struct lfds600_stack_state *ss, struct lfds600_stack_element *se[LFDS600_STACK_PAC_SIZE], void *user_data );
void lfds600_stack_internal_new_element( struct lfds600_stack_state *ss, struct lfds600_stack_element *se[LFDS600_STACK_PAC_SIZE], void *user_data );
void lfds600_stack_internal_init_element( struct lfds600_stack_state *ss, struct lfds600_stack_element *se[LFDS600_STACK_PAC_SIZE], struct lfds600_freelist_element *fe, void *user_data );

void lfds600_stack_internal_push( struct lfds600_stack_state *ss, struct lfds600_stack_element *se[LFDS600_STACK_PAC_SIZE] );

