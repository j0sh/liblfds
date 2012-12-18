#include "lfds600_freelist_internal.h"





/****************************************************************************/
void lfds600_freelist_delete( struct lfds600_freelist_state *fs, void (*user_data_delete_function)(void *user_data, void *user_state), void *user_state )
{
  struct lfds600_freelist_element
    *fe;

  void
    *user_data;

  assert( fs != NULL );
  // TRD : user_data_delete_function can be NULL
  // TRD : user_state can be NULL

  while( lfds600_freelist_pop(fs, &fe) )
  {
    if( user_data_delete_function != NULL )
    {
      lfds600_freelist_get_user_data_from_element( fe, &user_data );
      user_data_delete_function( user_data, user_state );
    }

    lfds600_abstraction_aligned_free( fe );
  }

  lfds600_abstraction_aligned_free( fs );

  return;
}

