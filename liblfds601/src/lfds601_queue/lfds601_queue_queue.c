#include "lfds601_queue_internal.h"





/****************************************************************************/
int lfds601_queue_enqueue( struct lfds601_queue_state *qs, void *user_data )
{
  LFDS601_ALIGN(LFDS601_ALIGN_DOUBLE_POINTER) struct lfds601_queue_element
    *qe[LFDS601_QUEUE_PAC_SIZE];

  assert( qs != NULL );
  // TRD : user_data can be NULL

  lfds601_queue_internal_new_element_from_freelist( qs, qe, user_data );

  if( qe[LFDS601_QUEUE_POINTER] == NULL )
    return( 0 );

  lfds601_queue_internal_queue( qs, qe );

  return( 1 );
}





/****************************************************************************/
int lfds601_queue_guaranteed_enqueue( struct lfds601_queue_state *qs, void *user_data )
{
  LFDS601_ALIGN(LFDS601_ALIGN_DOUBLE_POINTER) struct lfds601_queue_element
    *qe[LFDS601_QUEUE_PAC_SIZE];

  assert( qs != NULL );
  // TRD : user_data can be NULL

  lfds601_queue_internal_guaranteed_new_element_from_freelist( qs, qe, user_data );

  if( qe[LFDS601_QUEUE_POINTER] == NULL )
    return( 0 );

  lfds601_queue_internal_queue( qs, qe );

  return( 1 );
}





/****************************************************************************/
void lfds601_queue_internal_queue( struct lfds601_queue_state *qs, struct lfds601_queue_element *qe[LFDS601_QUEUE_PAC_SIZE] )
{
  LFDS601_ALIGN(LFDS601_ALIGN_DOUBLE_POINTER) struct lfds601_queue_element
    *enqueue[LFDS601_QUEUE_PAC_SIZE],
    *next[LFDS601_QUEUE_PAC_SIZE];

  unsigned char
    cas_result = 0;

  assert( qs != NULL );
  assert( qe != NULL );

  do
  {
    enqueue[LFDS601_QUEUE_POINTER] = qs->enqueue[LFDS601_QUEUE_POINTER];
    enqueue[LFDS601_QUEUE_COUNTER] = qs->enqueue[LFDS601_QUEUE_COUNTER];

    next[LFDS601_QUEUE_POINTER] = enqueue[LFDS601_QUEUE_POINTER]->next[LFDS601_QUEUE_POINTER];
    next[LFDS601_QUEUE_COUNTER] = enqueue[LFDS601_QUEUE_POINTER]->next[LFDS601_QUEUE_COUNTER];

    /* TRD : this if() ensures that the next we read, just above,
             really is from qs->enqueue (which we copied into enqueue)
    */

    if( qs->enqueue[LFDS601_QUEUE_POINTER] == enqueue[LFDS601_QUEUE_POINTER] and qs->enqueue[LFDS601_QUEUE_COUNTER] == enqueue[LFDS601_QUEUE_COUNTER] )
    {
      if( next[LFDS601_QUEUE_POINTER] == NULL )
      {
        qe[LFDS601_QUEUE_COUNTER] = next[LFDS601_QUEUE_COUNTER] + 1;
        cas_result = lfds601_abstraction_dcas( (volatile lfds601_atom_t *) enqueue[LFDS601_QUEUE_POINTER]->next, (lfds601_atom_t *) qe, (lfds601_atom_t *) next );
      }
      else
      {
        next[LFDS601_QUEUE_COUNTER] = enqueue[LFDS601_QUEUE_COUNTER] + 1;
        lfds601_abstraction_dcas( (volatile lfds601_atom_t *) qs->enqueue, (lfds601_atom_t *) next, (lfds601_atom_t *) enqueue );
      }
    }
  }
  while( cas_result == 0 );

  qe[LFDS601_QUEUE_COUNTER] = enqueue[LFDS601_QUEUE_COUNTER] + 1;
  lfds601_abstraction_dcas( (volatile lfds601_atom_t *) qs->enqueue, (lfds601_atom_t *) qe, (lfds601_atom_t *) enqueue );

  return;
}





/****************************************************************************/
int lfds601_queue_dequeue( struct lfds601_queue_state *qs, void **user_data )
{
  LFDS601_ALIGN(LFDS601_ALIGN_DOUBLE_POINTER) struct lfds601_queue_element
    *enqueue[LFDS601_QUEUE_PAC_SIZE],
    *dequeue[LFDS601_QUEUE_PAC_SIZE],
    *next[LFDS601_QUEUE_PAC_SIZE];

  unsigned char
    cas_result = 0;

  int
    rv = 1,
    state = LFDS601_QUEUE_STATE_UNKNOWN,
    finished_flag = LOWERED;

  assert( qs != NULL );
  assert( user_data != NULL );

  do
  {
    dequeue[LFDS601_QUEUE_POINTER] = qs->dequeue[LFDS601_QUEUE_POINTER];
    dequeue[LFDS601_QUEUE_COUNTER] = qs->dequeue[LFDS601_QUEUE_COUNTER];

    enqueue[LFDS601_QUEUE_POINTER] = qs->enqueue[LFDS601_QUEUE_POINTER];
    enqueue[LFDS601_QUEUE_COUNTER] = qs->enqueue[LFDS601_QUEUE_COUNTER];

    next[LFDS601_QUEUE_POINTER] = dequeue[LFDS601_QUEUE_POINTER]->next[LFDS601_QUEUE_POINTER];
    next[LFDS601_QUEUE_COUNTER] = dequeue[LFDS601_QUEUE_POINTER]->next[LFDS601_QUEUE_COUNTER];

    /* TRD : confirm that dequeue didn't move between reading it
             and reading its next pointer
    */

    if( dequeue[LFDS601_QUEUE_POINTER] == qs->dequeue[LFDS601_QUEUE_POINTER] and dequeue[LFDS601_QUEUE_COUNTER] == qs->dequeue[LFDS601_QUEUE_COUNTER] )
    {
      if( enqueue[LFDS601_QUEUE_POINTER] == dequeue[LFDS601_QUEUE_POINTER] and next[LFDS601_QUEUE_POINTER] == NULL )
        state = LFDS601_QUEUE_STATE_EMPTY;

      if( enqueue[LFDS601_QUEUE_POINTER] == dequeue[LFDS601_QUEUE_POINTER] and next[LFDS601_QUEUE_POINTER] != NULL )
        state = LFDS601_QUEUE_STATE_ENQUEUE_OUT_OF_PLACE;

      if( enqueue[LFDS601_QUEUE_POINTER] != dequeue[LFDS601_QUEUE_POINTER] )
        state = LFDS601_QUEUE_STATE_ATTEMPT_DELFDS601_QUEUE;

      switch( state )
      {
        case LFDS601_QUEUE_STATE_EMPTY:
          *user_data = NULL;
          rv = 0;
          finished_flag = RAISED;
        break;

        case LFDS601_QUEUE_STATE_ENQUEUE_OUT_OF_PLACE:
          next[LFDS601_QUEUE_COUNTER] = enqueue[LFDS601_QUEUE_COUNTER] + 1;
          lfds601_abstraction_dcas( (volatile lfds601_atom_t *) qs->enqueue, (lfds601_atom_t *) next, (lfds601_atom_t *) enqueue );
        break;

        case LFDS601_QUEUE_STATE_ATTEMPT_DELFDS601_QUEUE:
          *user_data = next[LFDS601_QUEUE_POINTER]->user_data;

          next[LFDS601_QUEUE_COUNTER] = dequeue[LFDS601_QUEUE_COUNTER] + 1;
          cas_result = lfds601_abstraction_dcas( (volatile lfds601_atom_t *) qs->dequeue, (lfds601_atom_t *) next, (lfds601_atom_t *) dequeue );

          if( cas_result == 1 )
            finished_flag = RAISED;
        break;
      }
    }
  }
  while( finished_flag == LOWERED );

  if( cas_result == 1 )
    lfds601_freelist_push( qs->fs, dequeue[LFDS601_QUEUE_POINTER]->fe );

  return( rv );
}

