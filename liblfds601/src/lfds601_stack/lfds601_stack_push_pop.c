#include "lfds601_stack_internal.h"





/****************************************************************************/
int lfds601_stack_push( struct lfds601_stack_state *ss, void *user_data )
{
  LFDS601_ALIGN(LFDS601_ALIGN_DOUBLE_POINTER) struct lfds601_stack_element
    *se[LFDS601_STACK_PAC_SIZE];

  assert( ss != NULL );
  // TRD : user_data can be NULL

  lfds601_stack_internal_new_element_from_freelist( ss, se, user_data );

  if( se[LFDS601_STACK_POINTER] == NULL )
    return( 0 );

  lfds601_stack_internal_push( ss, se );

  return( 1 );
}





/****************************************************************************/
int lfds601_stack_guaranteed_push( struct lfds601_stack_state *ss, void *user_data )
{
  LFDS601_ALIGN(LFDS601_ALIGN_DOUBLE_POINTER) struct lfds601_stack_element
    *se[LFDS601_STACK_PAC_SIZE];

  assert( ss != NULL );
  // TRD : user_data can be NULL

  /* TRD : this function allocated a new lfds601_freelist element and uses that
           to push onto the lfds601_stack, guaranteeing success (unless malloc()
           fails of course)
  */

  lfds601_stack_internal_new_element( ss, se, user_data );

  // TRD : malloc failed
  if( se[LFDS601_STACK_POINTER] == NULL )
    return( 0 );

  lfds601_stack_internal_push( ss, se );

  return( 1 );
}





/****************************************************************************/
void lfds601_stack_internal_push( struct lfds601_stack_state *ss, struct lfds601_stack_element *se[LFDS601_STACK_PAC_SIZE] )
{
  LFDS601_ALIGN(LFDS601_ALIGN_DOUBLE_POINTER) struct lfds601_stack_element
    *original_se_next[LFDS601_STACK_PAC_SIZE];

  assert( ss != NULL );
  assert( se != NULL );

  original_se_next[LFDS601_STACK_POINTER] = ss->top[LFDS601_STACK_POINTER];
  original_se_next[LFDS601_STACK_COUNTER] = ss->top[LFDS601_STACK_COUNTER];

  do
  {
    se[LFDS601_STACK_POINTER]->next[LFDS601_STACK_POINTER] = original_se_next[LFDS601_STACK_POINTER];
    se[LFDS601_STACK_POINTER]->next[LFDS601_STACK_COUNTER] = original_se_next[LFDS601_STACK_COUNTER];
  }
  while( 0 == lfds601_abstraction_dcas((volatile lfds601_atom_t *) ss->top, (lfds601_atom_t *) se, (lfds601_atom_t *) original_se_next) );

  return;
}





/****************************************************************************/
int lfds601_stack_pop( struct lfds601_stack_state *ss, void **user_data )
{
  LFDS601_ALIGN(LFDS601_ALIGN_DOUBLE_POINTER) struct lfds601_stack_element
    *se[LFDS601_STACK_PAC_SIZE];

  assert( ss != NULL );
  assert( user_data != NULL );

  se[LFDS601_STACK_COUNTER] = ss->top[LFDS601_STACK_COUNTER];
  se[LFDS601_STACK_POINTER] = ss->top[LFDS601_STACK_POINTER];

  do
  {
    if( se[LFDS601_STACK_POINTER] == NULL )
      return( 0 );
  }
  while( 0 == lfds601_abstraction_dcas((volatile lfds601_atom_t *) ss->top, (lfds601_atom_t *) se[LFDS601_STACK_POINTER]->next, (lfds601_atom_t *) se) );

  *user_data = se[LFDS601_STACK_POINTER]->user_data;

  lfds601_freelist_push( ss->fs, se[LFDS601_STACK_POINTER]->fe );

  return( 1 );
}

