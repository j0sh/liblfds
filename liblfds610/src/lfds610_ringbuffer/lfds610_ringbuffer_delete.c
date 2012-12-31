#include "lfds610_ringbuffer_internal.h"





/****************************************************************************/
void lfds610_ringbuffer_delete( struct lfds610_ringbuffer_state *rs, void (*user_data_delete_function)(void *user_data, void *user_state), void *user_state )
{
  assert( rs != NULL );
  // TRD : user_data_delete_function can be NULL
  // TRD : user_state can be NULL

  lfds610_queue_delete( rs->qs, NULL, NULL );

  lfds610_freelist_delete( rs->fs, user_data_delete_function, user_state );

  lfds610_liblfds_aligned_free( rs );

  return;
}

