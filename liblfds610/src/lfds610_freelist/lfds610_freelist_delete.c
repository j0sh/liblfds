#include "lfds610_freelist_internal.h"





/****************************************************************************/
void lfds610_freelist_delete( struct lfds610_freelist_state *fs, void (*user_data_delete_function)(void *user_data, void *user_state), void *user_state )
{
  struct lfds610_freelist_element
    *fe;

  void
    *user_data;

  assert( fs != NULL );
  // TRD : user_data_delete_function can be NULL
  // TRD : user_state can be NULL

  // TRD : leading load barrier not required as it will be performed by the pop

  while( lfds610_freelist_pop(fs, &fe) )
  {
    if( user_data_delete_function != NULL )
    {
      lfds610_freelist_get_user_data_from_element( fe, &user_data );
      user_data_delete_function( user_data, user_state );
    }

    lfds610_liblfds_aligned_free( fe );
  }

  lfds610_liblfds_aligned_free( fs );

  return;
}

