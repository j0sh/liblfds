#include "lfds610_slist_internal.h"





/****************************************************************************/
int lfds610_slist_get_user_data_from_element( struct lfds610_slist_element *se, void **user_data )
{
  int
    rv = 1;

  assert( se != NULL );
  assert( user_data != NULL );

  LFDS610_BARRIER_LOAD;

  *user_data = (void *) se->user_data_and_flags[LFDS610_SLIST_USER_DATA];

  if( (lfds610_atom_t) se->user_data_and_flags[LFDS610_SLIST_FLAGS] & LFDS610_SLIST_FLAG_DELETED )
    rv = 0;

  return( rv );
}





/****************************************************************************/
int lfds610_slist_set_user_data_in_element( struct lfds610_slist_element *se, void *user_data )
{
  LFDS610_ALIGN(LFDS610_ALIGN_DOUBLE_POINTER) void
    *user_data_and_flags[2],
    *new_user_data_and_flags[2];

  int
    rv = 1;

  assert( se != NULL );
  // TRD : user_data can be NULL

  LFDS610_BARRIER_LOAD;

  user_data_and_flags[LFDS610_SLIST_USER_DATA] = se->user_data_and_flags[LFDS610_SLIST_USER_DATA];
  user_data_and_flags[LFDS610_SLIST_FLAGS] = se->user_data_and_flags[LFDS610_SLIST_FLAGS];

  new_user_data_and_flags[LFDS610_SLIST_USER_DATA] = user_data;

  do
  {
    new_user_data_and_flags[LFDS610_SLIST_FLAGS] = user_data_and_flags[LFDS610_SLIST_FLAGS];
  }
  while( !((lfds610_atom_t) user_data_and_flags[LFDS610_SLIST_FLAGS] & LFDS610_SLIST_FLAG_DELETED) and
         0 == lfds610_abstraction_dcas((volatile lfds610_atom_t *) se->user_data_and_flags, (lfds610_atom_t *) new_user_data_and_flags, (lfds610_atom_t *) user_data_and_flags) );

  if( (lfds610_atom_t) user_data_and_flags[LFDS610_SLIST_FLAGS] & LFDS610_SLIST_FLAG_DELETED )
    rv = 0;

  LFDS610_BARRIER_STORE;

  return( rv );
}





/****************************************************************************/
struct lfds610_slist_element *lfds610_slist_get_head( struct lfds610_slist_state *ss, struct lfds610_slist_element **se )
{
  assert( ss != NULL );
  assert( se != NULL );

  LFDS610_BARRIER_LOAD;

  *se = (struct lfds610_slist_element *) ss->head;

  lfds610_slist_internal_move_to_first_undeleted_element( se );

  return( *se );
}





/****************************************************************************/
struct lfds610_slist_element *lfds610_slist_get_next( struct lfds610_slist_element *se, struct lfds610_slist_element **next_se )
{
  assert( se != NULL );
  assert( next_se != NULL );

  LFDS610_BARRIER_LOAD;

  *next_se = (struct lfds610_slist_element *) se->next;

  lfds610_slist_internal_move_to_first_undeleted_element( next_se );

  return( *next_se );
}





/****************************************************************************/
struct lfds610_slist_element *lfds610_slist_get_head_and_then_next( struct lfds610_slist_state *ss, struct lfds610_slist_element **se )
{
  assert( ss != NULL );
  assert( se != NULL );

  if( *se == NULL )
    lfds610_slist_get_head( ss, se );
  else
    lfds610_slist_get_next( *se, se );

  return( *se );
}





/****************************************************************************/
void lfds610_slist_internal_move_to_first_undeleted_element( struct lfds610_slist_element **se )
{
  assert( se != NULL );

  while( *se != NULL and (lfds610_atom_t) (*se)->user_data_and_flags[LFDS610_SLIST_FLAGS] & LFDS610_SLIST_FLAG_DELETED )
    (*se) = (struct lfds610_slist_element *) (*se)->next;

  return;
}

