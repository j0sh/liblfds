#include "lfds601_ringbuffer_internal.h"





/****************************************************************************/
int lfds601_ringbuffer_new( struct lfds601_ringbuffer_state **rs, lfds601_atom_t number_elements, int (*user_data_init_function)(void **user_data, void *user_state), void *user_state )
{
  int
    rv = 0;

  assert( rs != NULL );
  // TRD : number_elements can be any value in its range
  // TRD : user_data_init_function can be NULL
  // TRD : user_state can be NULL

  *rs = (struct lfds601_ringbuffer_state *) lfds601_abstraction_aligned_malloc( sizeof(struct lfds601_ringbuffer_state), LFDS601_ALIGN_DOUBLE_POINTER );

  if( *rs != NULL )
  {
    lfds601_freelist_new( &(*rs)->fs, number_elements, user_data_init_function, user_state );

    if( (*rs)->fs != NULL )
    {
      lfds601_queue_new( &(*rs)->qs, number_elements );

      if( (*rs)->qs != NULL )
        rv = 1;

      if( (*rs)->qs == NULL )
      {
        lfds601_abstraction_aligned_free( *rs );
        *rs = NULL;
      }
    }

    if( (*rs)->fs == NULL )
    {
      lfds601_abstraction_aligned_free( *rs );
      *rs = NULL;
    }
  }

  return( rv );
}

