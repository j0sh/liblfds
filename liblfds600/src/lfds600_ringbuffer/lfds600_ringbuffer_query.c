#include "lfds600_ringbuffer_internal.h"





/****************************************************************************/
#pragma warning( disable : 4100 )

void lfds600_ringbuffer_query( struct lfds600_ringbuffer_state *rs, enum lfds600_ringbuffer_query_type query_type, void *query_input, void *query_output )
{
  assert( rs != NULL );
  // TRD : query_type can be any value in its range
  // TRD : query_input can be NULL
  assert( query_output != NULL );

  switch( query_type )
  {
    case LFDS600_RINGBUFFER_QUERY_VALIDATE:
      // TRD : query_input can be NULL

      lfds600_ringbuffer_internal_validate( rs, (struct lfds600_validation_info *) query_input, (enum data_structure_validity *) query_output, ((enum data_structure_validity *) query_output)+2 );
    break;
  }

  return;
}

#pragma warning( default : 4100 )





/****************************************************************************/
void lfds600_ringbuffer_internal_validate( struct lfds600_ringbuffer_state *rs, struct lfds600_validation_info *vi, enum data_structure_validity *lfds600_queue_validity, enum data_structure_validity *lfds600_freelist_validity )
{
  assert( rs != NULL );
  // TRD : vi can be NULL
  assert( lfds600_queue_validity != NULL );
  assert( lfds600_freelist_validity != NULL );

  lfds600_queue_query( rs->qs, LFDS600_QUEUE_QUERY_VALIDATE, vi, lfds600_queue_validity );

  if( vi != NULL )
  {
    struct lfds600_validation_info
      lfds600_freelist_vi;

    lfds600_atom_t
      total_elements;

    lfds600_freelist_query( rs->fs, LFDS600_FREELIST_QUERY_ELEMENT_COUNT, NULL, (void *) &total_elements );
    lfds600_freelist_vi.min_elements = total_elements - vi->max_elements;
    lfds600_freelist_vi.max_elements = total_elements - vi->min_elements;
    lfds600_freelist_query( rs->fs, LFDS600_FREELIST_QUERY_VALIDATE, (void *) &lfds600_freelist_vi, (void *) lfds600_freelist_validity );
  }

  if( vi == NULL )
    lfds600_freelist_query( rs->fs, LFDS600_FREELIST_QUERY_VALIDATE, NULL, (void *) lfds600_freelist_validity );

  return;
}

