#include "lfds601_freelist_internal.h"





/****************************************************************************/
void lfds601_freelist_query( struct lfds601_freelist_state *fs, enum lfds601_freelist_query_type query_type, void *query_input, void *query_output )
{
  assert( fs != NULL );
  // TRD : query type can be any value in its range
  // TRD : query_input can be NULL in some cases
  assert( query_output != NULL );

  switch( query_type )
  {
    case LFDS601_FREELIST_QUERY_ELEMENT_COUNT:
      assert( query_input == NULL );

      *(lfds601_atom_t *) query_output = fs->element_count;
    break;

    case LFDS601_FREELIST_QUERY_VALIDATE:
      // TRD : query_input can be NULL

      lfds601_freelist_internal_validate( fs, (struct lfds601_validation_info *) query_input, (enum lfds601_data_structure_validity *) query_output );
    break;
  }

  return;
}





/****************************************************************************/
void lfds601_freelist_internal_validate( struct lfds601_freelist_state *fs, struct lfds601_validation_info *vi, enum lfds601_data_structure_validity *lfds601_freelist_validity )
{
  struct lfds601_freelist_element
    *fe,
    *fe_slow,
    *fe_fast;

  lfds601_atom_t
    element_count = 0;

  assert( fs != NULL );
  // TRD : vi can be NULL
  assert( lfds601_freelist_validity != NULL );

  *lfds601_freelist_validity = LFDS601_VALIDITY_VALID;

  fe_slow = fe_fast = (struct lfds601_freelist_element *) fs->top[LFDS601_FREELIST_POINTER];

  /* TRD : first, check for a loop
           we have two pointers
           both of which start at the top of the lfds601_freelist
           we enter a loop
           and on each iteration
           we advance one pointer by one element
           and the other by two

           we exit the loop when both pointers are NULL
           (have reached the end of the lfds601_freelist)

           or

           if we fast pointer 'sees' the slow pointer
           which means we have a loop
  */

  if( fe_slow != NULL )
    do
    {
      fe_slow = fe_slow->next[LFDS601_FREELIST_POINTER];

      if( fe_fast != NULL )
        fe_fast = fe_fast->next[LFDS601_FREELIST_POINTER];

      if( fe_fast != NULL )
        fe_fast = fe_fast->next[LFDS601_FREELIST_POINTER];
    }
    while( fe_slow != NULL and fe_fast != fe_slow );

  if( fe_fast != NULL and fe_slow != NULL and fe_fast == fe_slow )
    *lfds601_freelist_validity = LFDS601_VALIDITY_INVALID_LOOP;

  /* TRD : now check for expected number of elements
           vi can be NULL, in which case we do not check
           we know we don't have a loop from our earlier check
  */

  if( *lfds601_freelist_validity == LFDS601_VALIDITY_VALID and vi != NULL )
  {
    fe = (struct lfds601_freelist_element *) fs->top[LFDS601_FREELIST_POINTER];

    while( fe != NULL )
    {
      element_count++;
      fe = (struct lfds601_freelist_element *) fe->next[LFDS601_FREELIST_POINTER];
    }

    if( element_count < vi->min_elements )
      *lfds601_freelist_validity = LFDS601_VALIDITY_INVALID_MISSING_ELEMENTS;

    if( element_count > vi->max_elements )
      *lfds601_freelist_validity = LFDS601_VALIDITY_INVALID_ADDITIONAL_ELEMENTS;
  }

  return;
}

