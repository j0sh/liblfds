#include "lfds600_stack_internal.h"





/****************************************************************************/
int lfds600_stack_push( struct lfds600_stack_state *ss, void *user_data )
{
  LFDS600_ALIGN(LFDS600_ALIGN_DOUBLE_POINTER) struct lfds600_stack_element
    *se[LFDS600_STACK_PAC_SIZE];

  assert( ss != NULL );
  // TRD : user_data can be NULL

  lfds600_stack_internal_new_element_from_freelist( ss, se, user_data );

  if( se[LFDS600_STACK_POINTER] == NULL )
    return( 0 );

  lfds600_stack_internal_push( ss, se );

  return( 1 );
}





/****************************************************************************/
int lfds600_stack_guaranteed_push( struct lfds600_stack_state *ss, void *user_data )
{
  LFDS600_ALIGN(LFDS600_ALIGN_DOUBLE_POINTER) struct lfds600_stack_element
    *se[LFDS600_STACK_PAC_SIZE];

  assert( ss != NULL );
  // TRD : user_data can be NULL

  /* TRD : this function allocated a new lfds600_freelist element and uses that
           to push onto the lfds600_stack, guaranteeing success (unless malloc()
           fails of course)
  */

  lfds600_stack_internal_new_element( ss, se, user_data );

  // TRD : malloc failed
  if( se[LFDS600_STACK_POINTER] == NULL )
    return( 0 );

  lfds600_stack_internal_push( ss, se );

  return( 1 );
}





/****************************************************************************/
void lfds600_stack_internal_push( struct lfds600_stack_state *ss, struct lfds600_stack_element *se[LFDS600_STACK_PAC_SIZE] )
{
  LFDS600_ALIGN(LFDS600_ALIGN_DOUBLE_POINTER) struct lfds600_stack_element
    *original_se_next[LFDS600_STACK_PAC_SIZE];

  assert( ss != NULL );
  assert( se != NULL );

  original_se_next[LFDS600_STACK_POINTER] = ss->top[LFDS600_STACK_POINTER];
  original_se_next[LFDS600_STACK_COUNTER] = ss->top[LFDS600_STACK_COUNTER];

  do
  {
    se[LFDS600_STACK_POINTER]->next[LFDS600_STACK_POINTER] = original_se_next[LFDS600_STACK_POINTER];
    se[LFDS600_STACK_POINTER]->next[LFDS600_STACK_COUNTER] = original_se_next[LFDS600_STACK_COUNTER];
  }
  while( 0 == lfds600_abstraction_dcas((volatile lfds600_atom_t *) ss->top, (lfds600_atom_t *) se, (lfds600_atom_t *) original_se_next) );

  return;
}





/****************************************************************************/
int lfds600_stack_pop( struct lfds600_stack_state *ss, void **user_data )
{
  LFDS600_ALIGN(LFDS600_ALIGN_DOUBLE_POINTER) struct lfds600_stack_element
    *se[LFDS600_STACK_PAC_SIZE];

  assert( ss != NULL );
  assert( user_data != NULL );

  se[LFDS600_STACK_COUNTER] = ss->top[LFDS600_STACK_COUNTER];
  se[LFDS600_STACK_POINTER] = ss->top[LFDS600_STACK_POINTER];

  do
  {
    if( se[LFDS600_STACK_POINTER] == NULL )
      return( 0 );
  }
  while( 0 == lfds600_abstraction_dcas((volatile lfds600_atom_t *) ss->top, (lfds600_atom_t *) se[LFDS600_STACK_POINTER]->next, (lfds600_atom_t *) se) );

  *user_data = se[LFDS600_STACK_POINTER]->user_data;

  lfds600_freelist_push( ss->fs, se[LFDS600_STACK_POINTER]->fe );

  return( 1 );
}

