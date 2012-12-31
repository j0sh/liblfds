#include "lfds610_freelist_internal.h"





/****************************************************************************/
int lfds610_freelist_new( struct lfds610_freelist_state **fs, lfds610_atom_t number_elements, int (*user_data_init_function)(void **user_data, void *user_state), void *user_state )
{
  int
    rv = 0;

  lfds610_atom_t
    element_count;

  assert( fs != NULL );
  // TRD : number_elements can be any value in its range
  // TRD : user_data_init_function can be NULL

  *fs = (struct lfds610_freelist_state *) lfds610_liblfds_aligned_malloc( sizeof(struct lfds610_freelist_state), LFDS610_ALIGN_DOUBLE_POINTER );

  if( (*fs) != NULL )
  {
    (*fs)->top[LFDS610_FREELIST_POINTER] = NULL;
    (*fs)->top[LFDS610_FREELIST_COUNTER] = 0;
    (*fs)->user_data_init_function = user_data_init_function;
    (*fs)->user_state = user_state;
    (*fs)->aba_counter = 0;
    (*fs)->element_count = 0;

    element_count = lfds610_freelist_new_elements( *fs, number_elements );

    if( element_count == number_elements )
      rv = 1;

    if( element_count != number_elements )
    {
      lfds610_liblfds_aligned_free( (*fs) );
      *fs = NULL;
    }
  }

  LFDS610_BARRIER_STORE;

  return( rv );
}





/****************************************************************************/
#pragma warning( disable : 4100 )

void lfds610_freelist_use( struct lfds610_freelist_state *fs )
{
  assert( fs != NULL );

  LFDS610_BARRIER_LOAD;

  return;
}

#pragma warning( default : 4100 )





/****************************************************************************/
lfds610_atom_t lfds610_freelist_new_elements( struct lfds610_freelist_state *fs, lfds610_atom_t number_elements )
{
  struct lfds610_freelist_element
    *fe;

  lfds610_atom_t
    loop,
    count = 0;

  assert( fs != NULL );
  // TRD : number_elements can be any value in its range
  // TRD : user_data_init_function can be NULL

  for( loop = 0 ; loop < number_elements ; loop++ )
    if( lfds610_freelist_internal_new_element(fs, &fe) )
    {
      lfds610_freelist_push( fs, fe );
      count++;
    }

  return( count );
}





/****************************************************************************/
lfds610_atom_t lfds610_freelist_internal_new_element( struct lfds610_freelist_state *fs, struct lfds610_freelist_element **fe )
{
  lfds610_atom_t
    rv = 0;

  assert( fs != NULL );
  assert( fe != NULL );

  /* TRD : basically, does what you'd expect;

           allocates an element
           calls the user init function
           if anything fails, cleans up,
           sets *fe to NULL
           and returns 0
  */

  *fe = (struct lfds610_freelist_element *) lfds610_liblfds_aligned_malloc( sizeof(struct lfds610_freelist_element), LFDS610_ALIGN_DOUBLE_POINTER );

  if( *fe != NULL )
  {
    if( fs->user_data_init_function == NULL )
    {
      (*fe)->user_data = NULL;
      rv = 1;
    }

    if( fs->user_data_init_function != NULL )
    {
      rv = fs->user_data_init_function( &(*fe)->user_data, fs->user_state );

      if( rv == 0 )
      {
        lfds610_liblfds_aligned_free( *fe );
        *fe = NULL;
      }
    }
  }

  if( rv == 1 )
    lfds610_abstraction_increment( (lfds610_atom_t *) &fs->element_count );

  return( rv );
}

