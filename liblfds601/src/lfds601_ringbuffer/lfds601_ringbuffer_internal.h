/***** the library wide include file *****/
#include "lfds601_internal.h"

/***** defines *****/

/***** structures *****/
#pragma pack( push, LFDS601_ALIGN_DOUBLE_POINTER )

struct lfds601_ringbuffer_state
{
  struct lfds601_queue_state
    *qs;

  struct lfds601_freelist_state
    *fs;
};

#pragma pack( pop )

/***** externs *****/

/***** private prototypes *****/
void lfds601_ringbuffer_internal_validate( struct lfds601_ringbuffer_state *rs, struct lfds601_validation_info *vi, enum lfds601_data_structure_validity *lfds601_queue_validity, enum lfds601_data_structure_validity *lfds601_freelist_validity );

