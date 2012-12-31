#include "lfds610_abstraction_internal_body.h"





/****************************************************************************/
#if (defined _WIN64 && defined _MSC_VER)

  /* TRD : 64 bit Windows (user-mode or kernel) on any CPU with the Microsoft C compiler

           _WIN64    indicates 64 bit Windows
           _MSC_VER  indicates Microsoft C compiler
  */

  static LFDS610_INLINE lfds610_atom_t lfds610_abstraction_increment( volatile lfds610_atom_t *value )
  {
    assert( value != NULL );

    return( (lfds610_atom_t) _InterlockedIncrement64((__int64 *) value) );
  }

#endif





/****************************************************************************/
#if (!defined _WIN64 && defined _WIN32 && defined _MSC_VER)

  /* TRD : 32 bit Windows (user-mode or kernel) on any CPU with the Microsoft C compiler

           (!defined _WIN64 && defined _WIN32)  indicates 32 bit Windows
           _MSC_VER                             indicates Microsoft C compiler
  */

  static LFDS610_INLINE lfds610_atom_t lfds610_abstraction_increment( volatile lfds610_atom_t *value )
  {
    assert( value != NULL );

    return( (lfds610_atom_t) _InterlockedIncrement((long int *) value) );
  }

#endif





/****************************************************************************/
#if (__GNUC__ >= 4 && __GNUC_MINOR__ >= 1 && __GNUC_PATCHLEVEL__ >= 0)

  /* TRD : any OS on any CPU with GCC 4.1.0 or better

           GCC 4.1.0 introduced the __sync_*() atomic intrinsics

           __GNUC__ / __GNUC_MINOR__ / __GNUC_PATCHLEVEL__  indicates GCC and which version
  */

  static LFDS610_INLINE lfds610_atom_t lfds610_abstraction_increment( volatile lfds610_atom_t *value )
  {
    assert( value != NULL );

    // TRD : no need for casting here, GCC has a __sync_add_and_fetch() for all native types

    return( (lfds610_atom_t) __sync_add_and_fetch(value, 1) );
  }

#endif

