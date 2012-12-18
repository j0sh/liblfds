#include "lfds600_queue_internal.h"





/****************************************************************************/
#pragma warning( disable : 4100 )

void lfds600_queue_query( struct lfds600_queue_state *qs, enum lfds600_queue_query_type query_type, void *query_input, void *query_output )
{
  assert( qs != NULL );
  // TRD : query_type can be any value in its range
  // TRD : query_input can be NULL
  assert( query_output != NULL );

  switch( query_type )
  {
    case LFDS600_QUEUE_QUERY_ELEMENT_COUNT:
      assert( query_input == NULL );

      lfds600_freelist_query( qs->fs, LFDS600_FREELIST_QUERY_ELEMENT_COUNT, NULL, query_output );
    break;

    case LFDS600_QUEUE_QUERY_VALIDATE:
      // TRD : query_input can be NULL

      lfds600_queue_internal_validate( qs, (struct lfds600_validation_info *) query_input, (enum data_structure_validity *) query_output, ((enum data_structure_validity *) query_output)+1 );
    break;
  }

  return;
}

#pragma warning( default : 4100 )





/****************************************************************************/
void lfds600_queue_internal_validate( struct lfds600_queue_state *qs, struct lfds600_validation_info *vi, enum data_structure_validity *lfds600_queue_validity, enum data_structure_validity *lfds600_freelist_validity )
{
  struct lfds600_queue_element
    *qe,
    *qe_slow,
    *qe_fast;

  lfds600_atom_t
    element_count = 0,
    total_elements;

  struct lfds600_validation_info
    lfds600_freelist_vi;

  assert( qs != NULL );
  // TRD : vi can be NULL
  assert( lfds600_queue_validity != NULL );
  assert( lfds600_freelist_validity != NULL );

  *lfds600_queue_validity = VALIDITY_VALID;

  qe_slow = qe_fast = (struct lfds600_queue_element *) qs->dequeue[LFDS600_QUEUE_POINTER];

  /* TRD : first, check for a loop
           we have two pointers
           both of which start at the dequeue end of the lfds600_queue
           we enter a loop
           and on each iteration
           we advance one pointer by one element
           and the other by two

           we exit the loop when both pointers are NULL
           (have reached the end of the lfds600_queue)

           or

           if we fast pointer 'sees' the slow pointer
           which means we have a loop
  */

  if( qe_slow != NULL )
    do
    {
      qe_slow = qe_slow->next[LFDS600_QUEUE_POINTER];

      if( qe_fast != NULL )
        qe_fast = qe_fast->next[LFDS600_QUEUE_POINTER];

      if( qe_fast != NULL )
        qe_fast = qe_fast->next[LFDS600_QUEUE_POINTER];
    }
    while( qe_slow != NULL and qe_fast != qe_slow );

  if( qe_fast != NULL and qe_slow != NULL and qe_fast == qe_slow )
    *lfds600_queue_validity = VALIDITY_INVALID_LOOP;

  /* TRD : now check for expected number of elements
           vi can be NULL, in which case we do not check
           we know we don't have a loop from our earlier check
  */

  if( *lfds600_queue_validity == VALIDITY_VALID and vi != NULL )
  {
    qe = (struct lfds600_queue_element *) qs->dequeue[LFDS600_QUEUE_POINTER];

    while( qe != NULL )
    {
      element_count++;
      qe = (struct lfds600_queue_element *) qe->next[LFDS600_QUEUE_POINTER];
    }

    /* TRD : remember there is a dummy element in the lfds600_queue */
    element_count--;

    if( element_count < vi->min_elements )
      *lfds600_queue_validity = VALIDITY_INVALID_MISSING_ELEMENTS;

    if( element_count > vi->max_elements )
      *lfds600_queue_validity = VALIDITY_INVALID_ADDITIONAL_ELEMENTS;
  }

  /* TRD : now we validate the lfds600_freelist

           we may be able to check for the expected number of
           elements in the lfds600_freelist

           if the caller has given us an expected min and max
           number of elements in the lfds600_queue, then the total number
           of elements in the lfds600_freelist, minus that min and max,
           gives us the expected number of elements in the
           lfds600_freelist
  */

  if( vi != NULL )
  {
    lfds600_freelist_query( qs->fs, LFDS600_FREELIST_QUERY_ELEMENT_COUNT, NULL, (void *) &total_elements );

    /* TRD : remember there is a dummy element in the lfds600_queue */
    total_elements--;

    lfds600_freelist_vi.min_elements = total_elements - vi->max_elements;
    lfds600_freelist_vi.max_elements = total_elements - vi->min_elements;

    lfds600_freelist_query( qs->fs, LFDS600_FREELIST_QUERY_VALIDATE, (void *) &lfds600_freelist_vi, (void *) lfds600_freelist_validity );
  }

  if( vi == NULL )
    lfds600_freelist_query( qs->fs, LFDS600_FREELIST_QUERY_VALIDATE, NULL, (void *) lfds600_freelist_validity );

  return;
}

