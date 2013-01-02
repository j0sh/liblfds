#include "lfds601_slist_internal.h"





/****************************************************************************/
int lfds601_slist_get_user_data_from_element( struct lfds601_slist_element *se, void **user_data )
{
  int
    rv = 1;

  assert( se != NULL );
  assert( user_data != NULL );

  *user_data = (void *) se->user_data_and_flags[LFDS601_SLIST_USER_DATA];

  if( (lfds601_atom_t) se->user_data_and_flags[LFDS601_SLIST_FLAGS] & LFDS601_SLIST_FLAG_DELETED )
    rv = 0;

  return( rv );
}





/****************************************************************************/
int lfds601_slist_set_user_data_in_element( struct lfds601_slist_element *se, void *user_data )
{
  LFDS601_ALIGN(LFDS601_ALIGN_DOUBLE_POINTER) volatile void
    *user_data_and_flags[2],
    *new_user_data_and_flags[2];

  int
    rv = 1;

  assert( se != NULL );
  // TRD : user_data can be NULL

  user_data_and_flags[LFDS601_SLIST_USER_DATA] = se->user_data_and_flags[LFDS601_SLIST_USER_DATA];
  user_data_and_flags[LFDS601_SLIST_FLAGS] = se->user_data_and_flags[LFDS601_SLIST_FLAGS];

  new_user_data_and_flags[LFDS601_SLIST_USER_DATA] = user_data;

  do
  {
    new_user_data_and_flags[LFDS601_SLIST_FLAGS] = user_data_and_flags[LFDS601_SLIST_FLAGS];
  }
  while( !((lfds601_atom_t) user_data_and_flags[LFDS601_SLIST_FLAGS] & LFDS601_SLIST_FLAG_DELETED) and 0 == lfds601_abstraction_dcas((volatile lfds601_atom_t *) se->user_data_and_flags, (lfds601_atom_t *) new_user_data_and_flags, (lfds601_atom_t *) user_data_and_flags) );

  if( (lfds601_atom_t) user_data_and_flags[LFDS601_SLIST_FLAGS] & LFDS601_SLIST_FLAG_DELETED )
    rv = 0;

  return( rv );
}





/****************************************************************************/
struct lfds601_slist_element *lfds601_slist_get_head( struct lfds601_slist_state *ss, struct lfds601_slist_element **se )
{
  assert( ss != NULL );
  assert( se != NULL );

  *se = (struct lfds601_slist_element *) ss->head;

  lfds601_slist_internal_move_to_first_undeleted_element( se );

  return( *se );
}





/****************************************************************************/
struct lfds601_slist_element *lfds601_slist_get_next( struct lfds601_slist_element *se, struct lfds601_slist_element **next_se )
{
  assert( se != NULL );
  assert( next_se != NULL );

  *next_se = (struct lfds601_slist_element *) se->next;

  lfds601_slist_internal_move_to_first_undeleted_element( next_se );

  return( *next_se );
}





/****************************************************************************/
struct lfds601_slist_element *lfds601_slist_get_head_and_then_next( struct lfds601_slist_state *ss, struct lfds601_slist_element **se )
{
  assert( ss != NULL );
  assert( se != NULL );

  if( *se == NULL )
    lfds601_slist_get_head( ss, se );
  else
    lfds601_slist_get_next( *se, se );

  return( *se );
}





/****************************************************************************/
void lfds601_slist_internal_move_to_first_undeleted_element( struct lfds601_slist_element **se )
{
  assert( se != NULL );

  while( *se != NULL and (lfds601_atom_t) (*se)->user_data_and_flags[LFDS601_SLIST_FLAGS] & LFDS601_SLIST_FLAG_DELETED )
    (*se) = (struct lfds601_slist_element *) (*se)->next;

  return;
}

