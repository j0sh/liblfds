#include "lfds601_freelist_internal.h"





/****************************************************************************/
void lfds601_freelist_delete( struct lfds601_freelist_state *fs, void (*user_data_delete_function)(void *user_data, void *user_state), void *user_state )
{
  struct lfds601_freelist_element
    *fe;

  void
    *user_data;

  assert( fs != NULL );
  // TRD : user_data_delete_function can be NULL
  // TRD : user_state can be NULL

  while( lfds601_freelist_pop(fs, &fe) )
  {
    if( user_data_delete_function != NULL )
    {
      lfds601_freelist_get_user_data_from_element( fe, &user_data );
      user_data_delete_function( user_data, user_state );
    }

    lfds601_abstraction_aligned_free( fe );
  }

  lfds601_abstraction_aligned_free( fs );

  return;
}

