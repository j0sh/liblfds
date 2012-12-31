#include "lfds610_queue_internal.h"





/****************************************************************************/
void lfds610_queue_delete( struct lfds610_queue_state *qs, void (*user_data_delete_function)(void *user_data, void *user_state), void *user_state )
{
  void
    *user_data;

  assert( qs != NULL );
  // TRD : user_data_delete_function can be NULL
  // TRD : user_state can be NULL

  // TRD : leading load barrier not required as it will be performed by the dequeue

  while( lfds610_queue_dequeue(qs, &user_data) )
    if( user_data_delete_function != NULL )
      user_data_delete_function( user_data, user_state );

  /* TRD : fully dequeuing will leave us
           with a single dummy element
           which both qs->enqueue and qs->dequeue point at
           we push this back onto the lfds610_freelist
           before we delete the lfds610_freelist
  */

  lfds610_freelist_push( qs->fs, qs->enqueue[LFDS610_QUEUE_POINTER]->fe );

  lfds610_freelist_delete( qs->fs, lfds610_queue_internal_freelist_delete_function, NULL );

  lfds610_liblfds_aligned_free( qs );

  return;
}





/****************************************************************************/
#pragma warning( disable : 4100 )

void lfds610_queue_internal_freelist_delete_function( void *user_data, void *user_state )
{
  assert( user_data != NULL );
  assert( user_state == NULL );

  lfds610_liblfds_aligned_free( user_data );

  return;
}

#pragma warning( default : 4100 )

