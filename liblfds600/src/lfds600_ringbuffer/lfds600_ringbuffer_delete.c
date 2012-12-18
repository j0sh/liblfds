#include "lfds600_ringbuffer_internal.h"





/****************************************************************************/
void lfds600_ringbuffer_delete( struct lfds600_ringbuffer_state *rs, void (*user_data_delete_function)(void *user_data, void *user_state), void *user_state )
{
  assert( rs != NULL );
  // TRD : user_data_delete_function can be NULL
  // TRD : user_state can be NULL

  lfds600_queue_delete( rs->qs, NULL, NULL );

  lfds600_freelist_delete( rs->fs, user_data_delete_function, user_state );

  lfds600_abstraction_aligned_free( rs );

  return;
}

