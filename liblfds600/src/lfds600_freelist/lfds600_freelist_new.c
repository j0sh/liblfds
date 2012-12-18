#include "lfds600_freelist_internal.h"





/****************************************************************************/
int lfds600_freelist_new( struct lfds600_freelist_state **fs, lfds600_atom_t number_elements, int (*user_data_init_function)(void **user_data, void *user_state), void *user_state )
{
  int
    rv = 0;

  lfds600_atom_t
    element_count;

  assert( fs != NULL );
  // TRD : number_elements can be any value in its range
  // TRD : user_data_init_function can be NULL

  *fs = (struct lfds600_freelist_state *) lfds600_abstraction_aligned_malloc( sizeof(struct lfds600_freelist_state), LFDS600_ALIGN_DOUBLE_POINTER );

  if( (*fs) != NULL )
  {
    (*fs)->top[LFDS600_FREELIST_POINTER] = NULL;
    (*fs)->top[LFDS600_FREELIST_COUNTER] = 0;
    (*fs)->user_data_init_function = user_data_init_function;
    (*fs)->user_state = user_state;
    (*fs)->aba_counter = 0;
    (*fs)->element_count = 0;

    element_count = lfds600_freelist_new_elements( *fs, number_elements );

    if( element_count == number_elements )
      rv = 1;

    if( element_count != number_elements )
    {
      lfds600_abstraction_aligned_free( (*fs) );
      *fs = NULL;
    }
  }

  return( rv );
}





/****************************************************************************/
lfds600_atom_t lfds600_freelist_new_elements( struct lfds600_freelist_state *fs, lfds600_atom_t number_elements )
{
  struct lfds600_freelist_element
    *fe;

  lfds600_atom_t
    loop,
    count = 0;

  assert( fs != NULL );
  // TRD : number_elements can be any value in its range
  // TRD : user_data_init_function can be NULL

  for( loop = 0 ; loop < number_elements ; loop++ )
    if( lfds600_freelist_internal_new_element(fs, &fe) )
    {
      lfds600_freelist_push( fs, fe );
      count++;
    }

  return( count );
}





/****************************************************************************/
lfds600_atom_t lfds600_freelist_internal_new_element( struct lfds600_freelist_state *fs, struct lfds600_freelist_element **fe )
{
  lfds600_atom_t
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

  *fe = (struct lfds600_freelist_element *) lfds600_abstraction_aligned_malloc( sizeof(struct lfds600_freelist_element), LFDS600_ALIGN_DOUBLE_POINTER );

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
        lfds600_abstraction_aligned_free( *fe );
        *fe = NULL;
      }
    }
  }

  if( rv == 1 )
    lfds600_abstraction_increment( (lfds600_atom_t *) &fs->element_count );

  return( rv );
}

