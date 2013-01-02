#include "lfds601_stack_internal.h"





/****************************************************************************/
void lfds601_stack_delete( struct lfds601_stack_state *ss, void (*user_data_delete_function)(void *user_data, void *user_state), void *user_state )
{
  void
    *user_data;

  assert( ss != NULL );
  // TRD : user_data_delete_function can be NULL
  // TRD : user_state can be NULL

  while( lfds601_stack_pop(ss, &user_data) )
    if( user_data_delete_function != NULL )
      user_data_delete_function( user_data, user_state );

  lfds601_freelist_delete( ss->fs, lfds601_stack_internal_freelist_delete_function, NULL );

  lfds601_abstraction_aligned_free( ss );

  return;
}





/****************************************************************************/
void lfds601_stack_clear( struct lfds601_stack_state *ss, void (*user_data_clear_function)(void *user_data, void *user_state), void *user_state )
{
  void
    *user_data;

  assert( ss != NULL );
  // TRD : user_data_clear_function can be NULL
  // TRD : user_state can be NULL

  while( lfds601_stack_pop(ss, &user_data) )
    if( user_data_clear_function != NULL )
      user_data_clear_function( user_data, user_state );

  return;
}





/****************************************************************************/
#pragma warning( disable : 4100 )

void lfds601_stack_internal_freelist_delete_function( void *user_data, void *user_state )
{
  assert( user_data != NULL );
  assert( user_state == NULL );

  lfds601_abstraction_aligned_free( user_data );

  return;
}

#pragma warning( default : 4100 )

