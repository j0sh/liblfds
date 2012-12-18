#include "lfds600_stack_internal.h"





/****************************************************************************/
#pragma warning( disable : 4100 )

void lfds600_stack_query( struct lfds600_stack_state *ss, enum lfds600_stack_query_type query_type, void *query_input, void *query_output )
{
  assert( ss != NULL );
  // TRD : query_type can be any value in its range
  // TRD : query_iput can be NULL
  assert( query_output != NULL );

  switch( query_type )
  {
    case LFDS600_STACK_QUERY_ELEMENT_COUNT:
      assert( query_input == NULL );

      lfds600_freelist_query( ss->fs, LFDS600_FREELIST_QUERY_ELEMENT_COUNT, NULL, query_output );
    break;
  }

  return;
}

#pragma warning( default : 4100 )

