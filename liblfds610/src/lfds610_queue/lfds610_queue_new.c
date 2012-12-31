#include "lfds610_queue_internal.h"





/****************************************************************************/
int lfds610_queue_new( struct lfds610_queue_state **qs, lfds610_atom_t number_elements )
{
  int
    rv = 0;

  struct lfds610_queue_element
    *qe[LFDS610_QUEUE_PAC_SIZE];

  assert( qs != NULL );
  // TRD : number_elements can be any value in its range

  *qs = (struct lfds610_queue_state *) lfds610_liblfds_aligned_malloc( sizeof(struct lfds610_queue_state), LFDS610_ALIGN_DOUBLE_POINTER );

  if( *qs != NULL )
  {
    // TRD : the size of the lfds610_freelist is the size of the lfds610_queue (+1 for the leading dummy element, which is hidden from the caller)
    lfds610_freelist_new( &(*qs)->fs, number_elements+1, lfds610_queue_internal_freelist_init_function, NULL );

    if( (*qs)->fs != NULL )
    {
      lfds610_queue_internal_new_element_from_freelist( *qs, qe, NULL );
      (*qs)->enqueue[LFDS610_QUEUE_POINTER] = (*qs)->dequeue[LFDS610_QUEUE_POINTER] = qe[LFDS610_QUEUE_POINTER];
      (*qs)->enqueue[LFDS610_QUEUE_COUNTER] = (*qs)->dequeue[LFDS610_QUEUE_COUNTER] = 0;
      (*qs)->aba_counter = 0;
      rv = 1;
    }

    if( (*qs)->fs == NULL )
    {
      lfds610_liblfds_aligned_free( *qs );
      *qs = NULL;
    }
  }

  LFDS610_BARRIER_STORE;

  return( rv );
}





/****************************************************************************/
#pragma warning( disable : 4100 )

void lfds610_queue_use( struct lfds610_queue_state *qs )
{
  assert( qs != NULL );

  LFDS610_BARRIER_LOAD;

  return;
}

#pragma warning( default : 4100 )





/****************************************************************************/
#pragma warning( disable : 4100 )

int lfds610_queue_internal_freelist_init_function( void **user_data, void *user_state )
{
  int
    rv = 0;

  assert( user_data != NULL );
  assert( user_state == NULL );

  *user_data = lfds610_liblfds_aligned_malloc( sizeof(struct lfds610_queue_element), LFDS610_ALIGN_DOUBLE_POINTER );

  if( *user_data != NULL )
    rv = 1;

  return( rv );
}

#pragma warning( default : 4100 )





/****************************************************************************/
void lfds610_queue_internal_new_element_from_freelist( struct lfds610_queue_state *qs, struct lfds610_queue_element *qe[LFDS610_QUEUE_PAC_SIZE], void *user_data )
{
  struct lfds610_freelist_element
    *fe;

  assert( qs != NULL );
  assert( qe != NULL );
  // TRD : user_data can be any value in its range

  qe[LFDS610_QUEUE_POINTER] = NULL;

  lfds610_freelist_pop( qs->fs, &fe );

  if( fe != NULL )
    lfds610_queue_internal_init_element( qs, qe, fe, user_data );

  return;
}





/****************************************************************************/
void lfds610_queue_internal_guaranteed_new_element_from_freelist( struct lfds610_queue_state *qs, struct lfds610_queue_element *qe[LFDS610_QUEUE_PAC_SIZE], void *user_data )
{
  struct lfds610_freelist_element
    *fe;

  assert( qs != NULL );
  assert( qe != NULL );
  // TRD : user_data can be any value in its range

  qe[LFDS610_QUEUE_POINTER] = NULL;

  lfds610_freelist_guaranteed_pop( qs->fs, &fe );

  if( fe != NULL )
    lfds610_queue_internal_init_element( qs, qe, fe, user_data );

  return;
}





/****************************************************************************/
void lfds610_queue_internal_init_element( struct lfds610_queue_state *qs, struct lfds610_queue_element *qe[LFDS610_QUEUE_PAC_SIZE], struct lfds610_freelist_element *fe, void *user_data )
{
  assert( qs != NULL );
  assert( qe != NULL );
  assert( fe != NULL );
  // TRD : user_data can be any value in its range

  lfds610_freelist_get_user_data_from_element( fe, (void **) &qe[LFDS610_QUEUE_POINTER] );
  qe[LFDS610_QUEUE_COUNTER] = (struct lfds610_queue_element *) lfds610_abstraction_increment( (lfds610_atom_t *) &qs->aba_counter );

  qe[LFDS610_QUEUE_POINTER]->next[LFDS610_QUEUE_POINTER] = NULL;
  qe[LFDS610_QUEUE_POINTER]->next[LFDS610_QUEUE_COUNTER] = (struct lfds610_queue_element *) lfds610_abstraction_increment( (lfds610_atom_t *) &qs->aba_counter );

  qe[LFDS610_QUEUE_POINTER]->fe = fe;
  qe[LFDS610_QUEUE_POINTER]->user_data = user_data;

  return;
}

