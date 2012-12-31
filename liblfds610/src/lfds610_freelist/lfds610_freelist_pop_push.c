#include "lfds610_freelist_internal.h"





/****************************************************************************/
struct lfds610_freelist_element *lfds610_freelist_pop( struct lfds610_freelist_state *fs, struct lfds610_freelist_element **fe )
{
  LFDS610_ALIGN(LFDS610_ALIGN_DOUBLE_POINTER) struct lfds610_freelist_element
    *fe_local[LFDS610_FREELIST_PAC_SIZE];

  assert( fs != NULL );
  assert( fe != NULL );

  LFDS610_BARRIER_LOAD;

  fe_local[LFDS610_FREELIST_COUNTER] = fs->top[LFDS610_FREELIST_COUNTER];
  fe_local[LFDS610_FREELIST_POINTER] = fs->top[LFDS610_FREELIST_POINTER];

  /* TRD : note that lfds610_abstraction_dcas loads the original value of the destination (fs->top) into the compare (fe_local)
           (this happens of course after the CAS itself has occurred inside lfds610_abstraction_dcas)
  */

  do
  {
    if( fe_local[LFDS610_FREELIST_POINTER] == NULL )
    {
      *fe = NULL;
      return( *fe );
    }
  }
  while( 0 == lfds610_abstraction_dcas((volatile lfds610_atom_t *) fs->top, (lfds610_atom_t *) fe_local[LFDS610_FREELIST_POINTER]->next, (lfds610_atom_t *) fe_local) );

  *fe = (struct lfds610_freelist_element *) fe_local[LFDS610_FREELIST_POINTER];

  return( *fe );
}





/****************************************************************************/
struct lfds610_freelist_element *lfds610_freelist_guaranteed_pop( struct lfds610_freelist_state *fs, struct lfds610_freelist_element **fe )
{
  assert( fs != NULL );
  assert( fe != NULL );

  lfds610_freelist_internal_new_element( fs, fe );

  return( *fe );
}





/****************************************************************************/
void lfds610_freelist_push( struct lfds610_freelist_state *fs, struct lfds610_freelist_element *fe )
{
  LFDS610_ALIGN(LFDS610_ALIGN_DOUBLE_POINTER) struct lfds610_freelist_element
    *fe_local[LFDS610_FREELIST_PAC_SIZE],
    *original_fe_next[LFDS610_FREELIST_PAC_SIZE];

  assert( fs != NULL );
  assert( fe != NULL );

  LFDS610_BARRIER_LOAD;

  fe_local[LFDS610_FREELIST_POINTER] = fe;
  fe_local[LFDS610_FREELIST_COUNTER] = (struct lfds610_freelist_element *) lfds610_abstraction_increment( (lfds610_atom_t *) &fs->aba_counter );

  original_fe_next[LFDS610_FREELIST_POINTER] = fs->top[LFDS610_FREELIST_POINTER];
  original_fe_next[LFDS610_FREELIST_COUNTER] = fs->top[LFDS610_FREELIST_COUNTER];

  /* TRD : note that lfds610_abstraction_dcas loads the original value of the destination (fs->top) into the compare (original_fe_next)
           (this happens of course after the CAS itself has occurred inside lfds610_abstraction_dcas)
           this then causes us in our loop, should we repeat it, to update fe_local->next to a more
           up-to-date version of the head of the lfds610_freelist
  */

  do
  {
    fe_local[LFDS610_FREELIST_POINTER]->next[LFDS610_FREELIST_POINTER] = original_fe_next[LFDS610_FREELIST_POINTER];
    fe_local[LFDS610_FREELIST_POINTER]->next[LFDS610_FREELIST_COUNTER] = original_fe_next[LFDS610_FREELIST_COUNTER];
  }
  while( 0 == lfds610_abstraction_dcas((volatile lfds610_atom_t *) fs->top, (lfds610_atom_t *) fe_local, (lfds610_atom_t *) original_fe_next) );

  return;
}

