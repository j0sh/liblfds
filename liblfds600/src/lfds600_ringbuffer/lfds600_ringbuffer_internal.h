/***** the library wide include file *****/
#include "lfds600_internal.h"

/***** defines *****/

/***** structures *****/
#pragma pack( push, LFDS600_ALIGN_DOUBLE_POINTER )

struct lfds600_ringbuffer_state
{
  struct lfds600_queue_state
    *qs;

  struct lfds600_freelist_state
    *fs;
};

#pragma pack( pop )

/***** externs *****/

/***** private prototypes *****/
void lfds600_ringbuffer_internal_validate( struct lfds600_ringbuffer_state *rs, struct lfds600_validation_info *vi, enum data_structure_validity *lfds600_queue_validity, enum data_structure_validity *lfds600_freelist_validity );

