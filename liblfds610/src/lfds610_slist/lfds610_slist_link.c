#include "lfds610_slist_internal.h"





/****************************************************************************/
void lfds610_slist_internal_link_element_to_head( struct lfds610_slist_state *ss, struct lfds610_slist_element *volatile se )
{
  LFDS610_ALIGN(LFDS610_ALIGN_SINGLE_POINTER) struct lfds610_slist_element
    *se_next;

  assert( ss != NULL );
  assert( se != NULL );

  LFDS610_BARRIER_LOAD;

  se_next = ss->head;

  do
  {
    se->next = se_next;
  }
  while( se->next != (se_next = (struct lfds610_slist_element *) lfds610_abstraction_cas((volatile lfds610_atom_t *) &ss->head, (lfds610_atom_t) se, (lfds610_atom_t) se->next)) );

  return;
}





/****************************************************************************/
void lfds610_slist_internal_link_element_after_element( struct lfds610_slist_element *volatile lfds610_slist_in_list_element, struct lfds610_slist_element *volatile se )
{
  LFDS610_ALIGN(LFDS610_ALIGN_SINGLE_POINTER) struct lfds610_slist_element
    *se_prev,
    *se_next;

  assert( lfds610_slist_in_list_element != NULL );
  assert( se != NULL );

  LFDS610_BARRIER_LOAD;

  se_prev = (struct lfds610_slist_element *) lfds610_slist_in_list_element;

  se_next = se_prev->next;

  do
  {
    se->next = se_next;
  }
  while( se->next != (se_next = (struct lfds610_slist_element *) lfds610_abstraction_cas((volatile lfds610_atom_t *) &se_prev->next, (lfds610_atom_t) se, (lfds610_atom_t) se->next)) );

  return;
}

