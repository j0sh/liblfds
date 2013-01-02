#include "lfds601_slist_internal.h"





/****************************************************************************/
void lfds601_slist_internal_link_element_to_head( struct lfds601_slist_state *ss, struct lfds601_slist_element *volatile se )
{
  LFDS601_ALIGN(LFDS601_ALIGN_SINGLE_POINTER) struct lfds601_slist_element
    *se_next;

  assert( ss != NULL );
  assert( se != NULL );

  se_next = ss->head;

  do
  {
    se->next = se_next;
  }
  while( se->next != (se_next = (struct lfds601_slist_element *) lfds601_abstraction_cas((volatile lfds601_atom_t *) &ss->head, (lfds601_atom_t) se, (lfds601_atom_t) se->next)) );

  return;
}





/****************************************************************************/
void lfds601_slist_internal_link_element_after_element( struct lfds601_slist_element *volatile lfds601_slist_in_list_element, struct lfds601_slist_element *volatile se )
{
  LFDS601_ALIGN(LFDS601_ALIGN_SINGLE_POINTER) struct lfds601_slist_element
    *se_prev,
    *se_next;

  assert( lfds601_slist_in_list_element != NULL );
  assert( se != NULL );

  se_prev = (struct lfds601_slist_element *) lfds601_slist_in_list_element;

  se_next = se_prev->next;

  do
  {
    se->next = se_next;
  }
  while( se->next != (se_next = (struct lfds601_slist_element *) lfds601_abstraction_cas((volatile lfds601_atom_t *) &se_prev->next, (lfds601_atom_t) se, (lfds601_atom_t) se->next)) );

  return;
}

