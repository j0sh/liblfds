#include "lfds601_ringbuffer_internal.h"





/****************************************************************************/
#pragma warning( disable : 4100 )

void lfds601_ringbuffer_query( struct lfds601_ringbuffer_state *rs, enum lfds601_ringbuffer_query_type query_type, void *query_input, void *query_output )
{
  assert( rs != NULL );
  // TRD : query_type can be any value in its range
  // TRD : query_input can be NULL
  assert( query_output != NULL );

  switch( query_type )
  {
    case LFDS601_RINGBUFFER_QUERY_VALIDATE:
      // TRD : query_input can be NULL

      lfds601_ringbuffer_internal_validate( rs, (struct lfds601_validation_info *) query_input, (enum lfds601_data_structure_validity *) query_output, ((enum lfds601_data_structure_validity *) query_output)+2 );
    break;
  }

  return;
}

#pragma warning( default : 4100 )





/****************************************************************************/
void lfds601_ringbuffer_internal_validate( struct lfds601_ringbuffer_state *rs, struct lfds601_validation_info *vi, enum lfds601_data_structure_validity *lfds601_queue_validity, enum lfds601_data_structure_validity *lfds601_freelist_validity )
{
  assert( rs != NULL );
  // TRD : vi can be NULL
  assert( lfds601_queue_validity != NULL );
  assert( lfds601_freelist_validity != NULL );

  lfds601_queue_query( rs->qs, LFDS601_QUEUE_QUERY_VALIDATE, vi, lfds601_queue_validity );

  if( vi != NULL )
  {
    struct lfds601_validation_info
      lfds601_freelist_vi;

    lfds601_atom_t
      total_elements;

    lfds601_freelist_query( rs->fs, LFDS601_FREELIST_QUERY_ELEMENT_COUNT, NULL, (void *) &total_elements );
    lfds601_freelist_vi.min_elements = total_elements - vi->max_elements;
    lfds601_freelist_vi.max_elements = total_elements - vi->min_elements;
    lfds601_freelist_query( rs->fs, LFDS601_FREELIST_QUERY_VALIDATE, (void *) &lfds601_freelist_vi, (void *) lfds601_freelist_validity );
  }

  if( vi == NULL )
    lfds601_freelist_query( rs->fs, LFDS601_FREELIST_QUERY_VALIDATE, NULL, (void *) lfds601_freelist_validity );

  return;
}

