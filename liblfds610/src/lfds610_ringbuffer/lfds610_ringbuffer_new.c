#include "lfds610_ringbuffer_internal.h"





/****************************************************************************/
int lfds610_ringbuffer_new( struct lfds610_ringbuffer_state **rs, lfds610_atom_t number_elements, int (*user_data_init_function)(void **user_data, void *user_state), void *user_state )
{
  int
    rv = 0;

  assert( rs != NULL );
  // TRD : number_elements can be any value in its range
  // TRD : user_data_init_function can be NULL
  // TRD : user_state can be NULL

  *rs = (struct lfds610_ringbuffer_state *) lfds610_liblfds_aligned_malloc( sizeof(struct lfds610_ringbuffer_state), LFDS610_ALIGN_DOUBLE_POINTER );

  if( *rs != NULL )
  {
    lfds610_freelist_new( &(*rs)->fs, number_elements, user_data_init_function, user_state );

    if( (*rs)->fs != NULL )
    {
      lfds610_queue_new( &(*rs)->qs, number_elements );

      if( (*rs)->qs != NULL )
        rv = 1;

      if( (*rs)->qs == NULL )
      {
        lfds610_liblfds_aligned_free( *rs );
        *rs = NULL;
      }
    }

    if( (*rs)->fs == NULL )
    {
      lfds610_liblfds_aligned_free( *rs );
      *rs = NULL;
    }
  }

  LFDS610_BARRIER_STORE;

  return( rv );
}





/****************************************************************************/
#pragma warning( disable : 4100 )

void lfds610_ringbuffer_use( struct lfds610_ringbuffer_state *rs )
{
  assert( rs != NULL );

  LFDS610_BARRIER_LOAD;

  return;
}

#pragma warning( default : 4100 )

