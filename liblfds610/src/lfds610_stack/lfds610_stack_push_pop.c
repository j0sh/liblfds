#include "lfds610_stack_internal.h"





/****************************************************************************/
int lfds610_stack_push( struct lfds610_stack_state *ss, void *user_data )
{
  LFDS610_ALIGN(LFDS610_ALIGN_DOUBLE_POINTER) struct lfds610_stack_element
    *se[LFDS610_STACK_PAC_SIZE];

  assert( ss != NULL );
  // TRD : user_data can be NULL

  lfds610_stack_internal_new_element_from_freelist( ss, se, user_data );

  if( se[LFDS610_STACK_POINTER] == NULL )
    return( 0 );

  lfds610_stack_internal_push( ss, se );

  return( 1 );
}





/****************************************************************************/
int lfds610_stack_guaranteed_push( struct lfds610_stack_state *ss, void *user_data )
{
  LFDS610_ALIGN(LFDS610_ALIGN_DOUBLE_POINTER) struct lfds610_stack_element
    *se[LFDS610_STACK_PAC_SIZE];

  assert( ss != NULL );
  // TRD : user_data can be NULL

  /* TRD : this function allocated a new lfds610_freelist element and uses that
           to push onto the lfds610_stack, guaranteeing success (unless malloc()
           fails of course)
  */

  lfds610_stack_internal_new_element( ss, se, user_data );

  // TRD : malloc failed
  if( se[LFDS610_STACK_POINTER] == NULL )
    return( 0 );

  lfds610_stack_internal_push( ss, se );

  return( 1 );
}





/****************************************************************************/
void lfds610_stack_internal_push( struct lfds610_stack_state *ss, struct lfds610_stack_element *se[LFDS610_STACK_PAC_SIZE] )
{
  LFDS610_ALIGN(LFDS610_ALIGN_DOUBLE_POINTER) struct lfds610_stack_element
    *original_se_next[LFDS610_STACK_PAC_SIZE];

  assert( ss != NULL );
  assert( se != NULL );

  LFDS610_BARRIER_LOAD;

  original_se_next[LFDS610_STACK_POINTER] = ss->top[LFDS610_STACK_POINTER];
  original_se_next[LFDS610_STACK_COUNTER] = ss->top[LFDS610_STACK_COUNTER];

  do
  {
    se[LFDS610_STACK_POINTER]->next[LFDS610_STACK_POINTER] = original_se_next[LFDS610_STACK_POINTER];
    se[LFDS610_STACK_POINTER]->next[LFDS610_STACK_COUNTER] = original_se_next[LFDS610_STACK_COUNTER];
  }
  while( 0 == lfds610_abstraction_dcas((volatile lfds610_atom_t *) ss->top, (lfds610_atom_t *) se, (lfds610_atom_t *) original_se_next) );

  return;
}





/****************************************************************************/
int lfds610_stack_pop( struct lfds610_stack_state *ss, void **user_data )
{
  LFDS610_ALIGN(LFDS610_ALIGN_DOUBLE_POINTER) struct lfds610_stack_element
    *se[LFDS610_STACK_PAC_SIZE];

  assert( ss != NULL );
  assert( user_data != NULL );

  LFDS610_BARRIER_LOAD;

  se[LFDS610_STACK_COUNTER] = ss->top[LFDS610_STACK_COUNTER];
  se[LFDS610_STACK_POINTER] = ss->top[LFDS610_STACK_POINTER];

  do
  {
    if( se[LFDS610_STACK_POINTER] == NULL )
      return( 0 );
  }
  while( 0 == lfds610_abstraction_dcas((volatile lfds610_atom_t *) ss->top, (lfds610_atom_t *) se[LFDS610_STACK_POINTER]->next, (lfds610_atom_t *) se) );

  *user_data = se[LFDS610_STACK_POINTER]->user_data;

  lfds610_freelist_push( ss->fs, se[LFDS610_STACK_POINTER]->fe );

  return( 1 );
}

