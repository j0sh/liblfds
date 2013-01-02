#include "lfds601_queue_internal.h"





/****************************************************************************/
int lfds601_queue_new( struct lfds601_queue_state **qs, lfds601_atom_t number_elements )
{
  int
    rv = 0;

  struct lfds601_queue_element
    *qe[LFDS601_QUEUE_PAC_SIZE];

  assert( qs != NULL );
  // TRD : number_elements can be any value in its range

  *qs = (struct lfds601_queue_state *) lfds601_abstraction_aligned_malloc( sizeof(struct lfds601_queue_state), LFDS601_ALIGN_DOUBLE_POINTER );

  if( *qs != NULL )
  {
    // TRD : the size of the lfds601_freelist is the size of the lfds601_queue (+1 for the leading dummy element, which is hidden from the caller)
    lfds601_freelist_new( &(*qs)->fs, number_elements+1, lfds601_queue_internal_freelist_init_function, NULL );

    if( (*qs)->fs != NULL )
    {
      lfds601_queue_internal_new_element_from_freelist( *qs, qe, NULL );
      (*qs)->enqueue[LFDS601_QUEUE_POINTER] = (*qs)->dequeue[LFDS601_QUEUE_POINTER] = qe[LFDS601_QUEUE_POINTER];
      (*qs)->aba_counter = 0;
      rv = 1;
    }

    if( (*qs)->fs == NULL )
    {
      lfds601_abstraction_aligned_free( *qs );
      *qs = NULL;
    }
  }

  return( rv );
}





/****************************************************************************/
#pragma warning( disable : 4100 )

int lfds601_queue_internal_freelist_init_function( void **user_data, void *user_state )
{
  int
    rv = 0;

  assert( user_data != NULL );
  assert( user_state == NULL );

  *user_data = lfds601_abstraction_aligned_malloc( sizeof(struct lfds601_queue_element), LFDS601_ALIGN_DOUBLE_POINTER );

  if( *user_data != NULL )
    rv = 1;

  return( rv );
}

#pragma warning( default : 4100 )





/****************************************************************************/
void lfds601_queue_internal_new_element_from_freelist( struct lfds601_queue_state *qs, struct lfds601_queue_element *qe[LFDS601_QUEUE_PAC_SIZE], void *user_data )
{
  struct lfds601_freelist_element
    *fe;

  assert( qs != NULL );
  assert( qe != NULL );
  // TRD : user_data can be any value in its range

  qe[LFDS601_QUEUE_POINTER] = NULL;

  lfds601_freelist_pop( qs->fs, &fe );

  if( fe != NULL )
    lfds601_queue_internal_init_element( qs, qe, fe, user_data );

  return;
}





/****************************************************************************/
void lfds601_queue_internal_guaranteed_new_element_from_freelist( struct lfds601_queue_state *qs, struct lfds601_queue_element *qe[LFDS601_QUEUE_PAC_SIZE], void *user_data )
{
  struct lfds601_freelist_element
    *fe;

  assert( qs != NULL );
  assert( qe != NULL );
  // TRD : user_data can be any value in its range

  qe[LFDS601_QUEUE_POINTER] = NULL;

  lfds601_freelist_guaranteed_pop( qs->fs, &fe );

  if( fe != NULL )
    lfds601_queue_internal_init_element( qs, qe, fe, user_data );

  return;
}





/****************************************************************************/
void lfds601_queue_internal_init_element( struct lfds601_queue_state *qs, struct lfds601_queue_element *qe[LFDS601_QUEUE_PAC_SIZE], struct lfds601_freelist_element *fe, void *user_data )
{
  assert( qs != NULL );
  assert( qe != NULL );
  assert( fe != NULL );
  // TRD : user_data can be any value in its range

  lfds601_freelist_get_user_data_from_element( fe, (void **) &qe[LFDS601_QUEUE_POINTER] );
  qe[LFDS601_QUEUE_COUNTER] = (struct lfds601_queue_element *) lfds601_abstraction_increment( (lfds601_atom_t *) &qs->aba_counter );

  qe[LFDS601_QUEUE_POINTER]->next[LFDS601_QUEUE_POINTER] = NULL;
  qe[LFDS601_QUEUE_POINTER]->next[LFDS601_QUEUE_COUNTER] = (struct lfds601_queue_element *) lfds601_abstraction_increment( (lfds601_atom_t *) &qs->aba_counter );

  qe[LFDS601_QUEUE_POINTER]->fe = fe;
  qe[LFDS601_QUEUE_POINTER]->user_data = user_data;

  return;
}

