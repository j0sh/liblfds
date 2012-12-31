#include "lfds610_ringbuffer_internal.h"





/****************************************************************************/
#pragma warning( disable : 4100 )

void lfds610_ringbuffer_query( struct lfds610_ringbuffer_state *rs, enum lfds610_ringbuffer_query_type query_type, void *query_input, void *query_output )
{
  assert( rs != NULL );
  // TRD : query_type can be any value in its range
  // TRD : query_input can be NULL
  assert( query_output != NULL );

  switch( query_type )
  {
    case LFDS610_RINGBUFFER_QUERY_VALIDATE:
      // TRD : query_input can be NULL

      lfds610_ringbuffer_internal_validate( rs, (struct lfds610_validation_info *) query_input, (enum lfds610_data_structure_validity *) query_output, ((enum lfds610_data_structure_validity *) query_output)+2 );
    break;
  }

  return;
}

#pragma warning( default : 4100 )





/****************************************************************************/
void lfds610_ringbuffer_internal_validate( struct lfds610_ringbuffer_state *rs, struct lfds610_validation_info *vi, enum lfds610_data_structure_validity *lfds610_queue_validity, enum lfds610_data_structure_validity *lfds610_freelist_validity )
{
  assert( rs != NULL );
  // TRD : vi can be NULL
  assert( lfds610_queue_validity != NULL );
  assert( lfds610_freelist_validity != NULL );

  lfds610_queue_query( rs->qs, LFDS610_QUEUE_QUERY_VALIDATE, vi, lfds610_queue_validity );

  if( vi != NULL )
  {
    struct lfds610_validation_info
      lfds610_freelist_vi;

    lfds610_atom_t
      total_elements;

    lfds610_freelist_query( rs->fs, LFDS610_FREELIST_QUERY_ELEMENT_COUNT, NULL, (void *) &total_elements );
    lfds610_freelist_vi.min_elements = total_elements - vi->max_elements;
    lfds610_freelist_vi.max_elements = total_elements - vi->min_elements;
    lfds610_freelist_query( rs->fs, LFDS610_FREELIST_QUERY_VALIDATE, (void *) &lfds610_freelist_vi, (void *) lfds610_freelist_validity );
  }

  if( vi == NULL )
    lfds610_freelist_query( rs->fs, LFDS610_FREELIST_QUERY_VALIDATE, NULL, (void *) lfds610_freelist_validity );

  return;
}

