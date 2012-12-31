#include "lfds610_slist_internal.h"





/****************************************************************************/
int lfds610_slist_new( struct lfds610_slist_state **ss, void (*user_data_delete_function)(void *user_data, void *user_state), void *user_state )
{
  int
    rv = 0;

  assert( ss != NULL );
  // TRD : user_data_delete_function can be NULL
  // TRD : user_state can be NULL

  *ss = (struct lfds610_slist_state *) lfds610_liblfds_aligned_malloc( sizeof(struct lfds610_slist_state), LFDS610_ALIGN_SINGLE_POINTER );

  if( *ss != NULL )
  {
    lfds610_slist_internal_init_slist( *ss, user_data_delete_function, user_state );
    rv = 1;
  }

  LFDS610_BARRIER_STORE;

  return( rv );
}





/****************************************************************************/
#pragma warning( disable : 4100 )

void lfds610_slist_use( struct lfds610_slist_state *ss )
{
  assert( ss != NULL );

  LFDS610_BARRIER_LOAD;

  return;
}

#pragma warning( default : 4100 )





/****************************************************************************/
void lfds610_slist_internal_init_slist( struct lfds610_slist_state *ss, void (*user_data_delete_function)(void *user_data, void *user_state), void *user_state )
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
struct lfds610_slist_element *lfds610_slist_new_head( struct lfds610_slist_state *ss, void *user_data )
{
  LFDS610_ALIGN(LFDS610_ALIGN_SINGLE_POINTER) struct lfds610_slist_element
    *volatile se;

  assert( ss != NULL );
  // TRD : user_data can be NULL

  se = (struct lfds610_slist_element *) lfds610_liblfds_aligned_malloc( sizeof(struct lfds610_slist_element), LFDS610_ALIGN_DOUBLE_POINTER );

  if( se != NULL )
  {
    se->user_data_and_flags[LFDS610_SLIST_USER_DATA] = user_data;
    se->user_data_and_flags[LFDS610_SLIST_FLAGS] = LFDS610_SLIST_NO_FLAGS;

    lfds610_slist_internal_link_element_to_head( ss, se );
  }

  return( (struct lfds610_slist_element *) se );
}





/****************************************************************************/
struct lfds610_slist_element *lfds610_slist_new_next( struct lfds610_slist_element *se, void *user_data )
{
  LFDS610_ALIGN(LFDS610_ALIGN_SINGLE_POINTER) struct lfds610_slist_element
    *volatile se_next;

  assert( se != NULL );
  // TRD : user_data can be NULL

  se_next = (struct lfds610_slist_element *) lfds610_liblfds_aligned_malloc( sizeof(struct lfds610_slist_element), LFDS610_ALIGN_DOUBLE_POINTER );

  if( se_next != NULL )
  {
    se_next->user_data_and_flags[LFDS610_SLIST_USER_DATA] = user_data;
    se_next->user_data_and_flags[LFDS610_SLIST_FLAGS] = LFDS610_SLIST_NO_FLAGS;

    lfds610_slist_internal_link_element_after_element( se, se_next );
  }

  return( (struct lfds610_slist_element *) se_next );
}

