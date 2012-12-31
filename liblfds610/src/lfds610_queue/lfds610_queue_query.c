#include "lfds610_queue_internal.h"





/****************************************************************************/
#pragma warning( disable : 4100 )

void lfds610_queue_query( struct lfds610_queue_state *qs, enum lfds610_queue_query_type query_type, void *query_input, void *query_output )
{
  assert( qs != NULL );
  // TRD : query_type can be any value in its range
  // TRD : query_input can be NULL
  assert( query_output != NULL );

  switch( query_type )
  {
    case LFDS610_QUEUE_QUERY_ELEMENT_COUNT:
      assert( query_input == NULL );

      lfds610_freelist_query( qs->fs, LFDS610_FREELIST_QUERY_ELEMENT_COUNT, NULL, query_output );
    break;

    case LFDS610_QUEUE_QUERY_VALIDATE:
      // TRD : query_input can be NULL

      lfds610_queue_internal_validate( qs, (struct lfds610_validation_info *) query_input, (enum lfds610_data_structure_validity *) query_output, ((enum lfds610_data_structure_validity *) query_output)+1 );
    break;
  }

  return;
}

#pragma warning( default : 4100 )





/****************************************************************************/
void lfds610_queue_internal_validate( struct lfds610_queue_state *qs, struct lfds610_validation_info *vi, enum lfds610_data_structure_validity *lfds610_queue_validity, enum lfds610_data_structure_validity *lfds610_freelist_validity )
{
  struct lfds610_queue_element
    *qe,
    *qe_slow,
    *qe_fast;

  lfds610_atom_t
    element_count = 0,
    total_elements;

  struct lfds610_validation_info
    lfds610_freelist_vi;

  assert( qs != NULL );
  // TRD : vi can be NULL
  assert( lfds610_queue_validity != NULL );
  assert( lfds610_freelist_validity != NULL );

  *lfds610_queue_validity = LFDS610_VALIDITY_VALID;

  LFDS610_BARRIER_LOAD;

  qe_slow = qe_fast = (struct lfds610_queue_element *) qs->dequeue[LFDS610_QUEUE_POINTER];

  /* TRD : first, check for a loop
           we have two pointers
           both of which start at the dequeue end of the lfds610_queue
           we enter a loop
           and on each iteration
           we advance one pointer by one element
           and the other by two

           we exit the loop when both pointers are NULL
           (have reached the end of the lfds610_queue)

           or

           if we fast pointer 'sees' the slow pointer
           which means we have a loop
  */

  if( qe_slow != NULL )
    do
    {
      qe_slow = qe_slow->next[LFDS610_QUEUE_POINTER];

      if( qe_fast != NULL )
        qe_fast = qe_fast->next[LFDS610_QUEUE_POINTER];

      if( qe_fast != NULL )
        qe_fast = qe_fast->next[LFDS610_QUEUE_POINTER];
    }
    while( qe_slow != NULL and qe_fast != qe_slow );

  if( qe_fast != NULL and qe_slow != NULL and qe_fast == qe_slow )
    *lfds610_queue_validity = LFDS610_VALIDITY_INVALID_LOOP;

  /* TRD : now check for expected number of elements
           vi can be NULL, in which case we do not check
           we know we don't have a loop from our earlier check
  */

  if( *lfds610_queue_validity == LFDS610_VALIDITY_VALID and vi != NULL )
  {
    qe = (struct lfds610_queue_element *) qs->dequeue[LFDS610_QUEUE_POINTER];

    while( qe != NULL )
    {
      element_count++;
      qe = (struct lfds610_queue_element *) qe->next[LFDS610_QUEUE_POINTER];
    }

    /* TRD : remember there is a dummy element in the lfds610_queue */
    element_count--;

    if( element_count < vi->min_elements )
      *lfds610_queue_validity = LFDS610_VALIDITY_INVALID_MISSING_ELEMENTS;

    if( element_count > vi->max_elements )
      *lfds610_queue_validity = LFDS610_VALIDITY_INVALID_ADDITIONAL_ELEMENTS;
  }

  /* TRD : now we validate the lfds610_freelist

           we may be able to check for the expected number of
           elements in the lfds610_freelist

           if the caller has given us an expected min and max
           number of elements in the lfds610_queue, then the total number
           of elements in the lfds610_freelist, minus that min and max,
           gives us the expected number of elements in the
           lfds610_freelist
  */

  if( vi != NULL )
  {
    lfds610_freelist_query( qs->fs, LFDS610_FREELIST_QUERY_ELEMENT_COUNT, NULL, (void *) &total_elements );

    /* TRD : remember there is a dummy element in the lfds610_queue */
    total_elements--;

    lfds610_freelist_vi.min_elements = total_elements - vi->max_elements;
    lfds610_freelist_vi.max_elements = total_elements - vi->min_elements;

    lfds610_freelist_query( qs->fs, LFDS610_FREELIST_QUERY_VALIDATE, (void *) &lfds610_freelist_vi, (void *) lfds610_freelist_validity );
  }

  if( vi == NULL )
    lfds610_freelist_query( qs->fs, LFDS610_FREELIST_QUERY_VALIDATE, NULL, (void *) lfds610_freelist_validity );

  return;
}

