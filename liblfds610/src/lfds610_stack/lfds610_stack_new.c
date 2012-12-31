#include "lfds610_stack_internal.h"





/****************************************************************************/
int lfds610_stack_new( struct lfds610_stack_state **ss, lfds610_atom_t number_elements )
{
  int
    rv = 0;

  assert( ss != NULL );
  // TRD : number_elements can be any value in its range

  *ss = (struct lfds610_stack_state *) lfds610_liblfds_aligned_malloc( sizeof(struct lfds610_stack_state), LFDS610_ALIGN_DOUBLE_POINTER );

  if( *ss != NULL )
  {
    // TRD : the size of the lfds610_freelist is the size of the lfds610_stack
    lfds610_freelist_new( &(*ss)->fs, number_elements, lfds610_stack_internal_freelist_init_function, NULL );

    if( (*ss)->fs == NULL )
    {
      lfds610_liblfds_aligned_free( *ss );
      *ss = NULL;
    }

    if( (*ss)->fs != NULL )
    {
      (*ss)->top[LFDS610_STACK_POINTER] = NULL;
      (*ss)->top[LFDS610_STACK_COUNTER] = 0;
      (*ss)->aba_counter = 0;
      rv = 1;
    }
  }

  LFDS610_BARRIER_STORE;

  return( rv );
}





/****************************************************************************/
#pragma warning( disable : 4100 )

void lfds610_stack_use( struct lfds610_stack_state *ss )
{
  assert( ss != NULL );

  LFDS610_BARRIER_LOAD;

  return;
}

#pragma warning( default : 4100 )





/****************************************************************************/
#pragma warning( disable : 4100 )

int lfds610_stack_internal_freelist_init_function( void **user_data, void *user_state )
{
  int
    rv = 0;

  assert( user_data != NULL );
  assert( user_state == NULL );

  *user_data = lfds610_liblfds_aligned_malloc( sizeof(struct lfds610_stack_element), LFDS610_ALIGN_DOUBLE_POINTER );

  if( *user_data != NULL )
    rv = 1;

  return( rv );
}

#pragma warning( default : 4100 )





/****************************************************************************/
void lfds610_stack_internal_new_element_from_freelist( struct lfds610_stack_state *ss, struct lfds610_stack_element *se[LFDS610_STACK_PAC_SIZE], void *user_data )
{
  struct lfds610_freelist_element
    *fe;

  assert( ss != NULL );
  assert( se != NULL );
  // TRD : user_data can be any value in its range

  lfds610_freelist_pop( ss->fs, &fe );

  if( fe == NULL )
    se[LFDS610_STACK_POINTER] = NULL;

  if( fe != NULL )
    lfds610_stack_internal_init_element( ss, se, fe, user_data );

  return;
}





/****************************************************************************/
void lfds610_stack_internal_new_element( struct lfds610_stack_state *ss, struct lfds610_stack_element *se[LFDS610_STACK_PAC_SIZE], void *user_data )
{
  struct lfds610_freelist_element
    *fe;

  assert( ss != NULL );
  assert( se != NULL );
  // TRD : user_data can be any value in its range

  lfds610_freelist_guaranteed_pop( ss->fs, &fe );

  if( fe == NULL )
    se[LFDS610_STACK_POINTER] = NULL;

  if( fe != NULL )
    lfds610_stack_internal_init_element( ss, se, fe, user_data );

  return;
}





/****************************************************************************/
void lfds610_stack_internal_init_element( struct lfds610_stack_state *ss, struct lfds610_stack_element *se[LFDS610_STACK_PAC_SIZE], struct lfds610_freelist_element *fe, void *user_data )
{
  assert( ss != NULL );
  assert( se != NULL );
  assert( fe != NULL );
  // TRD : user_data can be any value in its range

  lfds610_freelist_get_user_data_from_element( fe, (void **) &se[LFDS610_STACK_POINTER] );

  se[LFDS610_STACK_COUNTER] = (struct lfds610_stack_element *) lfds610_abstraction_increment( (lfds610_atom_t *) &ss->aba_counter );

  se[LFDS610_STACK_POINTER]->next[LFDS610_STACK_POINTER] = NULL;
  se[LFDS610_STACK_POINTER]->next[LFDS610_STACK_COUNTER] = 0;
  se[LFDS610_STACK_POINTER]->fe = fe;
  se[LFDS610_STACK_POINTER]->user_data = user_data;

  return;
}

