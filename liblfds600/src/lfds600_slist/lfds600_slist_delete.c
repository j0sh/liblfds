#include "lfds600_slist_internal.h"





/****************************************************************************/
void lfds600_slist_delete( struct lfds600_slist_state *ss )
{
  lfds600_slist_delete_all_elements( ss );

  lfds600_abstraction_aligned_free( ss );

  return;
}





/****************************************************************************/
void lfds600_slist_delete_element( struct lfds600_slist_state *ss, struct lfds600_slist_element *se )
{
  LFDS600_ALIGN(LFDS600_ALIGN_DOUBLE_POINTER) void
    *volatile user_data_and_flags[2],
    *volatile new_user_data_and_flags[2];

  unsigned char
    cas_rv = 0;

  assert( ss != NULL );
  assert( se != NULL );

  user_data_and_flags[LFDS600_SLIST_USER_DATA] = se->user_data_and_flags[LFDS600_SLIST_USER_DATA];
  user_data_and_flags[LFDS600_SLIST_FLAGS] = se->user_data_and_flags[LFDS600_SLIST_FLAGS];

  do
  {
    new_user_data_and_flags[LFDS600_SLIST_USER_DATA] = user_data_and_flags[LFDS600_SLIST_USER_DATA];
    new_user_data_and_flags[LFDS600_SLIST_FLAGS] = (void *) ((lfds600_atom_t) user_data_and_flags[LFDS600_SLIST_FLAGS] | LFDS600_SLIST_FLAG_DELETED);
  }
  while( !((lfds600_atom_t) user_data_and_flags[LFDS600_SLIST_FLAGS] & LFDS600_SLIST_FLAG_DELETED) and 0 == (cas_rv = lfds600_abstraction_dcas((volatile lfds600_atom_t *) se->user_data_and_flags, (lfds600_atom_t *) new_user_data_and_flags, (lfds600_atom_t *) user_data_and_flags)) );

  if( cas_rv == 1 )
    if( ss->user_data_delete_function != NULL )
      ss->user_data_delete_function( (void *) user_data_and_flags[LFDS600_SLIST_USER_DATA], ss->user_state );

  return;
}





/****************************************************************************/
void lfds600_slist_delete_all_elements( struct lfds600_slist_state *ss )
{
  struct lfds600_slist_element
    *volatile se,
    *volatile se_temp;

  se = ss->head;

  while( se != NULL )
  {
    // TRD : if a non-deleted element and there is a delete function, call the delete function
    if( ss->user_data_delete_function != NULL )
      ss->user_data_delete_function( (void *) se->user_data_and_flags[LFDS600_SLIST_USER_DATA], ss->user_state );

    se_temp = se;
    se = se->next;
    lfds600_abstraction_aligned_free( (void *) se_temp );
  }

  lfds600_slist_internal_init_slist( ss, ss->user_data_delete_function, ss->user_state );

  return;
}

