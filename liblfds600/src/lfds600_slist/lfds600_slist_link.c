#include "lfds600_slist_internal.h"





/****************************************************************************/
void lfds600_slist_internal_link_element_to_head( struct lfds600_slist_state *ss, struct lfds600_slist_element *volatile se )
{
  LFDS600_ALIGN(LFDS600_ALIGN_SINGLE_POINTER) struct lfds600_slist_element
    *se_next;

  assert( ss != NULL );
  assert( se != NULL );

  se_next = ss->head;

  do
  {
    se->next = se_next;
  }
  while( se->next != (se_next = (struct lfds600_slist_element *) lfds600_abstraction_cas((volatile lfds600_atom_t *) &ss->head, (lfds600_atom_t) se, (lfds600_atom_t) se->next)) );

  return;
}





/****************************************************************************/
void lfds600_slist_internal_link_element_after_element( struct lfds600_slist_element *volatile lfds600_slist_in_list_element, struct lfds600_slist_element *volatile se )
{
  LFDS600_ALIGN(LFDS600_ALIGN_SINGLE_POINTER) struct lfds600_slist_element
    *se_prev,
    *se_next;

  assert( lfds600_slist_in_list_element != NULL );
  assert( se != NULL );

  se_prev = (struct lfds600_slist_element *) lfds600_slist_in_list_element;

  se_next = se_prev->next;

  do
  {
    se->next = se_next;
  }
  while( se->next != (se_next = (struct lfds600_slist_element *) lfds600_abstraction_cas((volatile lfds600_atom_t *) &se_prev->next, (lfds600_atom_t) se, (lfds600_atom_t) se->next)) );

  return;
}

