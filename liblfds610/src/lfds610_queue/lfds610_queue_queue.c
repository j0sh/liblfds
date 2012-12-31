#include "lfds610_queue_internal.h"





/****************************************************************************/
int lfds610_queue_enqueue( struct lfds610_queue_state *qs, void *user_data )
{
  LFDS610_ALIGN(LFDS610_ALIGN_DOUBLE_POINTER) struct lfds610_queue_element
    *qe[LFDS610_QUEUE_PAC_SIZE];

  assert( qs != NULL );
  // TRD : user_data can be NULL

  lfds610_queue_internal_new_element_from_freelist( qs, qe, user_data );

  if( qe[LFDS610_QUEUE_POINTER] == NULL )
    return( 0 );

  lfds610_queue_internal_queue( qs, qe );

  return( 1 );
}





/****************************************************************************/
int lfds610_queue_guaranteed_enqueue( struct lfds610_queue_state *qs, void *user_data )
{
  LFDS610_ALIGN(LFDS610_ALIGN_DOUBLE_POINTER) struct lfds610_queue_element
    *qe[LFDS610_QUEUE_PAC_SIZE];

  assert( qs != NULL );
  // TRD : user_data can be NULL

  lfds610_queue_internal_guaranteed_new_element_from_freelist( qs, qe, user_data );

  if( qe[LFDS610_QUEUE_POINTER] == NULL )
    return( 0 );

  lfds610_queue_internal_queue( qs, qe );

  return( 1 );
}





/****************************************************************************/
void lfds610_queue_internal_queue( struct lfds610_queue_state *qs, struct lfds610_queue_element *qe[LFDS610_QUEUE_PAC_SIZE] )
{
  LFDS610_ALIGN(LFDS610_ALIGN_DOUBLE_POINTER) struct lfds610_queue_element
    *enqueue[LFDS610_QUEUE_PAC_SIZE],
    *next[LFDS610_QUEUE_PAC_SIZE];

  unsigned char
    cas_result = 0;

  assert( qs != NULL );
  assert( qe != NULL );

  // TRD : the DCAS operation issues a read and write barrier, so we don't need a read barrier in the do() loop

  LFDS610_BARRIER_LOAD;

  do
  {
    enqueue[LFDS610_QUEUE_POINTER] = qs->enqueue[LFDS610_QUEUE_POINTER];
    enqueue[LFDS610_QUEUE_COUNTER] = qs->enqueue[LFDS610_QUEUE_COUNTER];

    next[LFDS610_QUEUE_POINTER] = enqueue[LFDS610_QUEUE_POINTER]->next[LFDS610_QUEUE_POINTER];
    next[LFDS610_QUEUE_COUNTER] = enqueue[LFDS610_QUEUE_POINTER]->next[LFDS610_QUEUE_COUNTER];

    /* TRD : this if() ensures that the next we read, just above,
             really is from qs->enqueue (which we copied into enqueue)
    */

    LFDS610_BARRIER_LOAD;

    if( qs->enqueue[LFDS610_QUEUE_POINTER] == enqueue[LFDS610_QUEUE_POINTER] and qs->enqueue[LFDS610_QUEUE_COUNTER] == enqueue[LFDS610_QUEUE_COUNTER] )
    {
      if( next[LFDS610_QUEUE_POINTER] == NULL )
      {
        qe[LFDS610_QUEUE_COUNTER] = next[LFDS610_QUEUE_COUNTER] + 1;
        cas_result = lfds610_abstraction_dcas( (volatile lfds610_atom_t *) enqueue[LFDS610_QUEUE_POINTER]->next, (lfds610_atom_t *) qe, (lfds610_atom_t *) next );
      }
      else
      {
        next[LFDS610_QUEUE_COUNTER] = enqueue[LFDS610_QUEUE_COUNTER] + 1;
        lfds610_abstraction_dcas( (volatile lfds610_atom_t *) qs->enqueue, (lfds610_atom_t *) next, (lfds610_atom_t *) enqueue );
      }
    }
  }
  while( cas_result == 0 );

  qe[LFDS610_QUEUE_COUNTER] = enqueue[LFDS610_QUEUE_COUNTER] + 1;
  lfds610_abstraction_dcas( (volatile lfds610_atom_t *) qs->enqueue, (lfds610_atom_t *) qe, (lfds610_atom_t *) enqueue );

  return;
}





/****************************************************************************/
int lfds610_queue_dequeue( struct lfds610_queue_state *qs, void **user_data )
{
  LFDS610_ALIGN(LFDS610_ALIGN_DOUBLE_POINTER) struct lfds610_queue_element
    *enqueue[LFDS610_QUEUE_PAC_SIZE],
    *dequeue[LFDS610_QUEUE_PAC_SIZE],
    *next[LFDS610_QUEUE_PAC_SIZE];

  unsigned char
    cas_result = 0;

  int
    rv = 1,
    state = LFDS610_QUEUE_STATE_UNKNOWN,
    finished_flag = LOWERED;

  assert( qs != NULL );
  assert( user_data != NULL );

  // TRD : the DCAS operation issues a read and write barrier, so we don't need a read barrier in the do() loop

  LFDS610_BARRIER_LOAD;

  do
  {
    dequeue[LFDS610_QUEUE_POINTER] = qs->dequeue[LFDS610_QUEUE_POINTER];
    dequeue[LFDS610_QUEUE_COUNTER] = qs->dequeue[LFDS610_QUEUE_COUNTER];

    enqueue[LFDS610_QUEUE_POINTER] = qs->enqueue[LFDS610_QUEUE_POINTER];
    enqueue[LFDS610_QUEUE_COUNTER] = qs->enqueue[LFDS610_QUEUE_COUNTER];

    next[LFDS610_QUEUE_POINTER] = dequeue[LFDS610_QUEUE_POINTER]->next[LFDS610_QUEUE_POINTER];
    next[LFDS610_QUEUE_COUNTER] = dequeue[LFDS610_QUEUE_POINTER]->next[LFDS610_QUEUE_COUNTER];

    /* TRD : confirm that dequeue didn't move between reading it
             and reading its next pointer
    */

    LFDS610_BARRIER_LOAD;

    if( dequeue[LFDS610_QUEUE_POINTER] == qs->dequeue[LFDS610_QUEUE_POINTER] and dequeue[LFDS610_QUEUE_COUNTER] == qs->dequeue[LFDS610_QUEUE_COUNTER] )
    {
      if( enqueue[LFDS610_QUEUE_POINTER] == dequeue[LFDS610_QUEUE_POINTER] and next[LFDS610_QUEUE_POINTER] == NULL )
        state = LFDS610_QUEUE_STATE_EMPTY;

      if( enqueue[LFDS610_QUEUE_POINTER] == dequeue[LFDS610_QUEUE_POINTER] and next[LFDS610_QUEUE_POINTER] != NULL )
        state = LFDS610_QUEUE_STATE_ENQUEUE_OUT_OF_PLACE;

      if( enqueue[LFDS610_QUEUE_POINTER] != dequeue[LFDS610_QUEUE_POINTER] )
        state = LFDS610_QUEUE_STATE_ATTEMPT_DELFDS610_QUEUE;

      switch( state )
      {
        case LFDS610_QUEUE_STATE_EMPTY:
          *user_data = NULL;
          rv = 0;
          finished_flag = RAISED;
        break;

        case LFDS610_QUEUE_STATE_ENQUEUE_OUT_OF_PLACE:
          next[LFDS610_QUEUE_COUNTER] = enqueue[LFDS610_QUEUE_COUNTER] + 1;
          lfds610_abstraction_dcas( (volatile lfds610_atom_t *) qs->enqueue, (lfds610_atom_t *) next, (lfds610_atom_t *) enqueue );
        break;

        case LFDS610_QUEUE_STATE_ATTEMPT_DELFDS610_QUEUE:
          *user_data = next[LFDS610_QUEUE_POINTER]->user_data;

          next[LFDS610_QUEUE_COUNTER] = dequeue[LFDS610_QUEUE_COUNTER] + 1;
          cas_result = lfds610_abstraction_dcas( (volatile lfds610_atom_t *) qs->dequeue, (lfds610_atom_t *) next, (lfds610_atom_t *) dequeue );

          if( cas_result == 1 )
            finished_flag = RAISED;
        break;
      }
    }
  }
  while( finished_flag == LOWERED );

  if( cas_result == 1 )
    lfds610_freelist_push( qs->fs, dequeue[LFDS610_QUEUE_POINTER]->fe );

  return( rv );
}

