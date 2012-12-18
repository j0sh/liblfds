#include "lfds600_queue_internal.h"





/****************************************************************************/
int lfds600_queue_enqueue( struct lfds600_queue_state *qs, void *user_data )
{
  LFDS600_ALIGN(LFDS600_ALIGN_DOUBLE_POINTER) struct lfds600_queue_element
    *qe[LFDS600_QUEUE_PAC_SIZE];

  assert( qs != NULL );
  // TRD : user_data can be NULL

  lfds600_queue_internal_new_element_from_freelist( qs, qe, user_data );

  if( qe[LFDS600_QUEUE_POINTER] == NULL )
    return( 0 );

  lfds600_queue_internal_queue( qs, qe );

  return( 1 );
}





/****************************************************************************/
int lfds600_queue_guaranteed_enqueue( struct lfds600_queue_state *qs, void *user_data )
{
  LFDS600_ALIGN(LFDS600_ALIGN_DOUBLE_POINTER) struct lfds600_queue_element
    *qe[LFDS600_QUEUE_PAC_SIZE];

  assert( qs != NULL );
  // TRD : user_data can be NULL

  lfds600_queue_internal_guaranteed_new_element_from_freelist( qs, qe, user_data );

  if( qe[LFDS600_QUEUE_POINTER] == NULL )
    return( 0 );

  lfds600_queue_internal_queue( qs, qe );

  return( 1 );
}





/****************************************************************************/
void lfds600_queue_internal_queue( struct lfds600_queue_state *qs, struct lfds600_queue_element *qe[LFDS600_QUEUE_PAC_SIZE] )
{
  LFDS600_ALIGN(LFDS600_ALIGN_DOUBLE_POINTER) struct lfds600_queue_element
    *enqueue[LFDS600_QUEUE_PAC_SIZE],
    *next[LFDS600_QUEUE_PAC_SIZE];

  unsigned char
    cas_result = 0;

  assert( qs != NULL );
  assert( qe != NULL );

  do
  {
    enqueue[LFDS600_QUEUE_POINTER] = qs->enqueue[LFDS600_QUEUE_POINTER];
    enqueue[LFDS600_QUEUE_COUNTER] = qs->enqueue[LFDS600_QUEUE_COUNTER];

    next[LFDS600_QUEUE_POINTER] = enqueue[LFDS600_QUEUE_POINTER]->next[LFDS600_QUEUE_POINTER];
    next[LFDS600_QUEUE_COUNTER] = enqueue[LFDS600_QUEUE_POINTER]->next[LFDS600_QUEUE_COUNTER];

    /* TRD : this if() ensures that the next we read, just above,
             really is from qs->enqueue (which we copied into enqueue)
    */

    if( qs->enqueue[LFDS600_QUEUE_POINTER] == enqueue[LFDS600_QUEUE_POINTER] and qs->enqueue[LFDS600_QUEUE_COUNTER] == enqueue[LFDS600_QUEUE_COUNTER] )
    {
      if( next[LFDS600_QUEUE_POINTER] == NULL )
      {
        qe[LFDS600_QUEUE_COUNTER] = next[LFDS600_QUEUE_COUNTER] + 1;
        cas_result = lfds600_abstraction_dcas( (volatile lfds600_atom_t *) enqueue[LFDS600_QUEUE_POINTER]->next, (lfds600_atom_t *) qe, (lfds600_atom_t *) next );
      }
      else
      {
        next[LFDS600_QUEUE_COUNTER] = enqueue[LFDS600_QUEUE_COUNTER] + 1;
        lfds600_abstraction_dcas( (volatile lfds600_atom_t *) qs->enqueue, (lfds600_atom_t *) next, (lfds600_atom_t *) enqueue );
      }
    }
  }
  while( cas_result == 0 );

  qe[LFDS600_QUEUE_COUNTER] = enqueue[LFDS600_QUEUE_COUNTER] + 1;
  lfds600_abstraction_dcas( (volatile lfds600_atom_t *) qs->enqueue, (lfds600_atom_t *) qe, (lfds600_atom_t *) enqueue );

  return;
}





/****************************************************************************/
int lfds600_queue_dequeue( struct lfds600_queue_state *qs, void **user_data )
{
  LFDS600_ALIGN(LFDS600_ALIGN_DOUBLE_POINTER) struct lfds600_queue_element
    *enqueue[LFDS600_QUEUE_PAC_SIZE],
    *dequeue[LFDS600_QUEUE_PAC_SIZE],
    *next[LFDS600_QUEUE_PAC_SIZE];

  unsigned char
    cas_result = 0;

  int
    rv = 1,
    state = LFDS600_QUEUE_STATE_UNKNOWN,
    finished_flag = LOWERED;

  assert( qs != NULL );
  assert( user_data != NULL );

  do
  {
    dequeue[LFDS600_QUEUE_POINTER] = qs->dequeue[LFDS600_QUEUE_POINTER];
    dequeue[LFDS600_QUEUE_COUNTER] = qs->dequeue[LFDS600_QUEUE_COUNTER];

    enqueue[LFDS600_QUEUE_POINTER] = qs->enqueue[LFDS600_QUEUE_POINTER];
    enqueue[LFDS600_QUEUE_COUNTER] = qs->enqueue[LFDS600_QUEUE_COUNTER];

    next[LFDS600_QUEUE_POINTER] = dequeue[LFDS600_QUEUE_POINTER]->next[LFDS600_QUEUE_POINTER];
    next[LFDS600_QUEUE_COUNTER] = dequeue[LFDS600_QUEUE_POINTER]->next[LFDS600_QUEUE_COUNTER];

    /* TRD : confirm that dequeue didn't move between reading it
             and reading its next pointer
    */

    if( dequeue[LFDS600_QUEUE_POINTER] == qs->dequeue[LFDS600_QUEUE_POINTER] and dequeue[LFDS600_QUEUE_COUNTER] == qs->dequeue[LFDS600_QUEUE_COUNTER] )
    {
      if( enqueue[LFDS600_QUEUE_POINTER] == dequeue[LFDS600_QUEUE_POINTER] and next[LFDS600_QUEUE_POINTER] == NULL )
        state = LFDS600_QUEUE_STATE_EMPTY;

      if( enqueue[LFDS600_QUEUE_POINTER] == dequeue[LFDS600_QUEUE_POINTER] and next[LFDS600_QUEUE_POINTER] != NULL )
        state = LFDS600_QUEUE_STATE_ENQUEUE_OUT_OF_PLACE;

      if( enqueue[LFDS600_QUEUE_POINTER] != dequeue[LFDS600_QUEUE_POINTER] )
        state = LFDS600_QUEUE_STATE_ATTEMPT_DELFDS600_QUEUE;

      switch( state )
      {
        case LFDS600_QUEUE_STATE_EMPTY:
          *user_data = NULL;
          rv = 0;
          finished_flag = RAISED;
        break;

        case LFDS600_QUEUE_STATE_ENQUEUE_OUT_OF_PLACE:
          next[LFDS600_QUEUE_COUNTER] = enqueue[LFDS600_QUEUE_COUNTER] + 1;
          lfds600_abstraction_dcas( (volatile lfds600_atom_t *) qs->enqueue, (lfds600_atom_t *) next, (lfds600_atom_t *) enqueue );
        break;

        case LFDS600_QUEUE_STATE_ATTEMPT_DELFDS600_QUEUE:
          *user_data = next[LFDS600_QUEUE_POINTER]->user_data;

          next[LFDS600_QUEUE_COUNTER] = dequeue[LFDS600_QUEUE_COUNTER] + 1;
          cas_result = lfds600_abstraction_dcas( (volatile lfds600_atom_t *) qs->dequeue, (lfds600_atom_t *) next, (lfds600_atom_t *) dequeue );

          if( cas_result == 1 )
            finished_flag = RAISED;
        break;
      }
    }
  }
  while( finished_flag == LOWERED );

  if( cas_result == 1 )
    lfds600_freelist_push( qs->fs, dequeue[LFDS600_QUEUE_POINTER]->fe );

  return( rv );
}

