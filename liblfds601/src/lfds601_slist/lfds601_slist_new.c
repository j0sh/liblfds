#include "lfds601_slist_internal.h"





/****************************************************************************/
int lfds601_slist_new( struct lfds601_slist_state **ss, void (*user_data_delete_function)(void *user_data, void *user_state), void *user_state )
{
  int
    rv = 0;

  assert( ss != NULL );
  // TRD : user_data_delete_function can be NULL
  // TRD : user_state can be NULL

  *ss = (struct lfds601_slist_state *) lfds601_abstraction_aligned_malloc( sizeof(struct lfds601_slist_state), LFDS601_ALIGN_SINGLE_POINTER );

  if( *ss != NULL )
  {
    lfds601_slist_internal_init_slist( *ss, user_data_delete_function, user_state );
    rv = 1;
  }

  return( rv );
}





/****************************************************************************/
void lfds601_slist_internal_init_slist( struct lfds601_slist_state *ss, void (*user_data_delete_function)(void *user_data, void *user_state), void *user_state )
{
  assert( ss != NULL );
  // TRD : user_data_delete_function can be NULL
  // TRD : user_state can be NULL

  ss->head = NULL;
  ss->user_data_delete_function = user_data_delete_function;
  ss->user_state = user_state;

  return;
}





/****************************************************************************/
struct lfds601_slist_element *lfds601_slist_new_head( struct lfds601_slist_state *ss, void *user_data )
{
  LFDS601_ALIGN(LFDS601_ALIGN_SINGLE_POINTER) struct lfds601_slist_element
    *volatile se;

  assert( ss != NULL );
  // TRD : user_data can be NULL

  se = (struct lfds601_slist_element *) lfds601_abstraction_aligned_malloc( sizeof(struct lfds601_slist_element), LFDS601_ALIGN_DOUBLE_POINTER );

  if( se != NULL )
  {
    se->user_data_and_flags[LFDS601_SLIST_USER_DATA] = user_data;
    se->user_data_and_flags[LFDS601_SLIST_FLAGS] = LFDS601_SLIST_NO_FLAGS;

    lfds601_slist_internal_link_element_to_head( ss, se );
  }

  return( (struct lfds601_slist_element *) se );
}





/****************************************************************************/
struct lfds601_slist_element *lfds601_slist_new_next( struct lfds601_slist_element *se, void *user_data )
{
  LFDS601_ALIGN(LFDS601_ALIGN_SINGLE_POINTER) struct lfds601_slist_element
    *volatile se_next;

  assert( se != NULL );
  // TRD : user_data can be NULL

  se_next = (struct lfds601_slist_element *) lfds601_abstraction_aligned_malloc( sizeof(struct lfds601_slist_element), LFDS601_ALIGN_DOUBLE_POINTER );

  if( se_next != NULL )
  {
    se_next->user_data_and_flags[LFDS601_SLIST_USER_DATA] = user_data;
    se_next->user_data_and_flags[LFDS601_SLIST_FLAGS] = LFDS601_SLIST_NO_FLAGS;

    lfds601_slist_internal_link_element_after_element( se, se_next );
  }

  return( (struct lfds601_slist_element *) se_next );
}

