// -*- C++ -*-
/**
 * Code generated by the The ACE ORB (TAO) IDL Compiler v3.0.3
 * TAO and the TAO IDL Compiler have been developed by:
 *       Center for Distributed Object Computing
 *       Washington University
 *       St. Louis, MO
 *       USA
 * and
 *       Distributed Object Computing Laboratory
 *       University of California at Irvine
 *       Irvine, CA
 *       USA
 * and
 *       Institute for Software Integrated Systems
 *       Vanderbilt University
 *       Nashville, TN
 *       USA
 *       https://www.isis.vanderbilt.edu/
 *
 * Information about TAO is available at:
 *     https://www.dre.vanderbilt.edu/~schmidt/TAO.html
 **/

// TAO_IDL - Generated from
// /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_codegen.cpp:366


#include "testC.h"
#include "tao/AnyTypeCode/Null_RefCount_Policy.h"
#include "tao/AnyTypeCode/TypeCode_Constants.h"
#include "tao/AnyTypeCode/Alias_TypeCode_Static.h"
#include "tao/AnyTypeCode/Objref_TypeCode_Static.h"
#include "tao/AnyTypeCode/Sequence_TypeCode_Static.h"
#include "tao/AnyTypeCode/Struct_TypeCode_Static.h"
#include "tao/AnyTypeCode/TypeCode_Struct_Field.h"
#include "tao/CDR.h"
#include "tao/Exception_Data.h"
#include "tao/Invocation_Adapter.h"
#include "tao/Object_T.h"
#include "tao/ORB_Core.h"
#include "tao/CDR.h"
#include "tao/AnyTypeCode/Any.h"
#include "tao/AnyTypeCode/Any_Impl_T.h"
#include "tao/AnyTypeCode/Any_Dual_Impl_T.h"
#include "tao/AnyTypeCode/Any_Array_Impl_T.h"
#include "cstring"

#if !defined (__ACE_INLINE__)
#include "testC.inl"
#endif /* !defined INLINE */

// TAO_IDL - Generated from
// /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_interface/interface_cs.cpp:48
TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Traits specializations for Simple_Server.

Simple_Server_ptr
TAO::Objref_Traits<Simple_Server>::duplicate (
    Simple_Server_ptr p)
{
  return Simple_Server::_duplicate (p);
}

void
TAO::Objref_Traits<Simple_Server>::release (
    Simple_Server_ptr p)
{
  ::CORBA::release (p);
}

Simple_Server_ptr
TAO::Objref_Traits<Simple_Server>::nil ()
{
  return Simple_Server::_nil ();
}

::CORBA::Boolean
TAO::Objref_Traits<Simple_Server>::marshal (
    const Simple_Server_ptr p,
    TAO_OutputCDR & cdr)
{
  return ::CORBA::Object::marshal (p, cdr);
}

TAO_END_VERSIONED_NAMESPACE_DECL



// TAO_IDL - Generated from
// /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_array/array_cs.cpp:94

Simple_Server::HeadPoseArray_slice *
Simple_Server::HeadPoseArray_dup (const Simple_Server::HeadPoseArray_slice *_tao_src_array)
{
  Simple_Server::HeadPoseArray_slice *_tao_dup_array = Simple_Server::HeadPoseArray_alloc ();
  
  if (_tao_dup_array)
    {
      Simple_Server::HeadPoseArray_copy (_tao_dup_array, _tao_src_array);
    }
  
  return _tao_dup_array;
}

Simple_Server::HeadPoseArray_slice *
Simple_Server::HeadPoseArray_alloc ()
{
  Simple_Server::HeadPoseArray_slice *retval {};
  ACE_NEW_RETURN (retval, ::CORBA::Float[7], nullptr);
  return retval;
}

void
Simple_Server::HeadPoseArray_free (Simple_Server::HeadPoseArray_slice *_tao_slice)
{
  delete [] _tao_slice;
}

void
Simple_Server::HeadPoseArray_copy (
    Simple_Server::HeadPoseArray_slice * _tao_to,
    const Simple_Server::HeadPoseArray_slice *_tao_from)
{
  // Copy each individual element.
  for ( ::CORBA::ULong i0 = 0; i0 < 7; ++i0)
    {
      _tao_to[i0] = _tao_from[i0];
    }
}

// TAO_IDL - Generated from
// /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_typecode/alias_typecode.cpp:49



// TAO_IDL - Generated from
// /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_typecode/typecode_defn.cpp:298


#ifndef _TAO_TYPECODE_Simple_Server_HeadPoseArray_GUARD
#define _TAO_TYPECODE_Simple_Server_HeadPoseArray_GUARD

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace TypeCode
  {
    namespace
    {
      TAO::TypeCode::Sequence< ::CORBA::TypeCode_ptr const *,
                              TAO::Null_RefCount_Policy>
        Simple_Server_HeadPoseArray_7 (
          ::CORBA::tk_array,
          &CORBA::_tc_float,
          7U);
        
      ::CORBA::TypeCode_ptr const tc_Simple_Server_HeadPoseArray =
        &Simple_Server_HeadPoseArray_7;
    }
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* _TAO_TYPECODE_Simple_Server_HeadPoseArray_GUARD */
static TAO::TypeCode::Alias<char const *,
                            ::CORBA::TypeCode_ptr const *,
                            TAO::Null_RefCount_Policy>
  _tao_tc_Simple_Server_HeadPoseArray (
    ::CORBA::tk_alias,
    "IDL:Simple_Server/HeadPoseArray:1.0",
    "HeadPoseArray",
    &TAO::TypeCode::tc_Simple_Server_HeadPoseArray);
  
::CORBA::TypeCode_ptr const Simple_Server::_tc_HeadPoseArray =
  &_tao_tc_Simple_Server_HeadPoseArray;

// TAO_IDL - Generated from
// /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_sequence/sequence_cs.cpp:105

#if !defined (_SIMPLE_SERVER_PIXELS_CS_)
#define _SIMPLE_SERVER_PIXELS_CS_

Simple_Server::pixels::pixels ()
{}

Simple_Server::pixels::pixels (
    ::CORBA::ULong length,
    ::CORBA::Octet * buffer,
    ::CORBA::Boolean release)
  : ::TAO::bounded_value_sequence< ::CORBA::Octet,300000>
    (length, buffer, release)
{}

Simple_Server::pixels::~pixels ()
{}

void Simple_Server::pixels::_tao_any_destructor (
    void * _tao_void_pointer)
{
  pixels * _tao_tmp_pointer =
    static_cast<pixels *> (_tao_void_pointer);
  delete _tao_tmp_pointer;
}

#endif /* end #if !defined */

// TAO_IDL - Generated from
// /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_typecode/alias_typecode.cpp:49



// TAO_IDL - Generated from
// /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_typecode/typecode_defn.cpp:461


#ifndef _TAO_TYPECODE_Simple_Server_pixels_GUARD
#define _TAO_TYPECODE_Simple_Server_pixels_GUARD

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace TypeCode
  {
    namespace
    {
      TAO::TypeCode::Sequence< ::CORBA::TypeCode_ptr const *,
                              TAO::Null_RefCount_Policy>
        Simple_Server_pixels_300000 (
          ::CORBA::tk_sequence,
          &CORBA::_tc_octet,
          300000U);
        
      ::CORBA::TypeCode_ptr const tc_Simple_Server_pixels_300000 =
        &Simple_Server_pixels_300000;
    }
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* _TAO_TYPECODE_Simple_Server_pixels_GUARD */

static TAO::TypeCode::Alias<char const *,
                            ::CORBA::TypeCode_ptr const *,
                            TAO::Null_RefCount_Policy>
  _tao_tc_Simple_Server_pixels (
    ::CORBA::tk_alias,
    "IDL:Simple_Server/pixels:1.0",
    "pixels",
    &TAO::TypeCode::tc_Simple_Server_pixels_300000);
  
::CORBA::TypeCode_ptr const Simple_Server::_tc_pixels =
  &_tao_tc_Simple_Server_pixels;

// TAO_IDL - Generated from
// /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_sequence/sequence_cs.cpp:105

#if !defined (_SIMPLE_SERVER_HEADER_CS_)
#define _SIMPLE_SERVER_HEADER_CS_

Simple_Server::header::header ()
{}

Simple_Server::header::header (
    ::CORBA::ULong length,
    ::CORBA::Octet * buffer,
    ::CORBA::Boolean release)
  : ::TAO::bounded_value_sequence< ::CORBA::Octet,100000>
    (length, buffer, release)
{}

Simple_Server::header::~header ()
{}

void Simple_Server::header::_tao_any_destructor (
    void * _tao_void_pointer)
{
  header * _tao_tmp_pointer =
    static_cast<header *> (_tao_void_pointer);
  delete _tao_tmp_pointer;
}

#endif /* end #if !defined */

// TAO_IDL - Generated from
// /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_typecode/alias_typecode.cpp:49



// TAO_IDL - Generated from
// /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_typecode/typecode_defn.cpp:461


#ifndef _TAO_TYPECODE_Simple_Server_header_GUARD
#define _TAO_TYPECODE_Simple_Server_header_GUARD

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace TypeCode
  {
    namespace
    {
      TAO::TypeCode::Sequence< ::CORBA::TypeCode_ptr const *,
                              TAO::Null_RefCount_Policy>
        Simple_Server_header_100000 (
          ::CORBA::tk_sequence,
          &CORBA::_tc_octet,
          100000U);
        
      ::CORBA::TypeCode_ptr const tc_Simple_Server_header_100000 =
        &Simple_Server_header_100000;
    }
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* _TAO_TYPECODE_Simple_Server_header_GUARD */

static TAO::TypeCode::Alias<char const *,
                            ::CORBA::TypeCode_ptr const *,
                            TAO::Null_RefCount_Policy>
  _tao_tc_Simple_Server_header (
    ::CORBA::tk_alias,
    "IDL:Simple_Server/header:1.0",
    "header",
    &TAO::TypeCode::tc_Simple_Server_header_100000);
  
::CORBA::TypeCode_ptr const Simple_Server::_tc_header =
  &_tao_tc_Simple_Server_header;

// TAO_IDL - Generated from
// /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_typecode/struct_typecode.cpp:86



// TAO_IDL - Generated from
// /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_typecode/typecode_defn.cpp:461



// TAO_IDL - Generated from
// /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_typecode/typecode_defn.cpp:461

static TAO::TypeCode::Struct_Field<
  char const *,
  ::CORBA::TypeCode_ptr const *> const 
    _tao_fields_Simple_Server_frameData[] =
      {
        { "m_pixels", &Simple_Server::_tc_pixels },
        { "m_header", &Simple_Server::_tc_header },
        { "m_pixelSize", &CORBA::_tc_long },
        { "m_headerSize", &CORBA::_tc_long }
      };
  
static TAO::TypeCode::Struct<
  char const *,
  ::CORBA::TypeCode_ptr const *,
  TAO::TypeCode::Struct_Field<
    char const *,
    ::CORBA::TypeCode_ptr const *> const *,
  TAO::Null_RefCount_Policy>
_tao_tc_Simple_Server_frameData (
  ::CORBA::tk_struct,
  "IDL:Simple_Server/frameData:1.0",
  "frameData",
  _tao_fields_Simple_Server_frameData,
  4);

::CORBA::TypeCode_ptr const Simple_Server::_tc_frameData =
  &_tao_tc_Simple_Server_frameData;

// TAO_IDL - Generated from
// /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_structure/structure_cs.cpp:56

void
Simple_Server::frameData::_tao_any_destructor (
    void *_tao_void_pointer)
{
  frameData *_tao_tmp_pointer =
    static_cast<frameData *> (_tao_void_pointer);
  delete _tao_tmp_pointer;
}

// TAO_IDL - Generated from
// /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_operation/operation_cs.cpp:88

void
Simple_Server::send_data (
  ::CORBA::Long microsecond,
  const ::Simple_Server::HeadPoseArray headpose)
{
  if (!this->is_evaluated ())
    {
      ::CORBA::Object::tao_object_initialize (this);
    }
  
  TAO::Arg_Traits< void>::ret_val _tao_retval;
  TAO::Arg_Traits< ::CORBA::Long>::in_arg_val _tao_microsecond (microsecond);
  TAO::Arg_Traits< ::Simple_Server::HeadPoseArray_tag>::in_arg_val _tao_headpose (headpose);

  TAO::Argument *_the_tao_operation_signature [] =
    {
      std::addressof(_tao_retval),
      std::addressof(_tao_microsecond),
      std::addressof(_tao_headpose)
    };

  TAO::Invocation_Adapter _tao_call (
      this,
      _the_tao_operation_signature,
      3,
      "send_data",
      9,
      TAO::TAO_CO_NONE | TAO::TAO_CO_THRU_POA_STRATEGY,
      TAO::TAO_ONEWAY_INVOCATION
      
    );

  _tao_call.invoke (nullptr, 0);
}

// TAO_IDL - Generated from
// /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_operation/operation_cs.cpp:88

::Simple_Server::frameData *
Simple_Server::sendFrameData ()
{
  if (!this->is_evaluated ())
    {
      ::CORBA::Object::tao_object_initialize (this);
    }
  
  TAO::Arg_Traits< ::Simple_Server::frameData>::ret_val _tao_retval;

  TAO::Argument *_the_tao_operation_signature [] =
    {
      std::addressof(_tao_retval)
    };

  TAO::Invocation_Adapter _tao_call (
      this,
      _the_tao_operation_signature,
      1,
      "sendFrameData",
      13,
      TAO::TAO_CO_NONE | TAO::TAO_CO_THRU_POA_STRATEGY,
      TAO::TAO_TWOWAY_INVOCATION
      ,
      TAO::TAO_SYNCHRONOUS_INVOCATION,
      false
    );

  _tao_call.invoke (nullptr, 0);

  return _tao_retval.retn ();
}

Simple_Server::Simple_Server ()
{
}

Simple_Server::~Simple_Server ()
{
}

void
Simple_Server::_tao_any_destructor (void *_tao_void_pointer)
{
  Simple_Server *_tao_tmp_pointer =
    static_cast<Simple_Server *> (_tao_void_pointer);
  ::CORBA::release (_tao_tmp_pointer);
}

Simple_Server_ptr
Simple_Server::_narrow (
    ::CORBA::Object_ptr _tao_objref)
{
  return TAO::Narrow_Utils<Simple_Server>::narrow (_tao_objref, "IDL:Simple_Server:1.0");
}

Simple_Server_ptr
Simple_Server::_unchecked_narrow (
    ::CORBA::Object_ptr _tao_objref)
{
  return TAO::Narrow_Utils<Simple_Server>::unchecked_narrow (_tao_objref);
}

Simple_Server_ptr
Simple_Server::_nil ()
{
  return nullptr;
}

Simple_Server_ptr
Simple_Server::_duplicate (Simple_Server_ptr obj)
{
  if (! ::CORBA::is_nil (obj))
    {
      obj->_add_ref ();
    }
  return obj;
}

void
Simple_Server::_tao_release (Simple_Server_ptr obj)
{
  ::CORBA::release (obj);
}

::CORBA::Boolean
Simple_Server::_is_a (const char *value)
{
  if (
      std::strcmp (value, "IDL:Simple_Server:1.0") == 0 ||
      std::strcmp (value, "IDL:omg.org/CORBA/Object:1.0") == 0
      )
    {
      return true; // success using local knowledge
    }
  else
    {
      return this->::CORBA::Object::_is_a (value);
    }
}

const char* Simple_Server::_interface_repository_id () const
{
  return "IDL:Simple_Server:1.0";
}

::CORBA::Boolean
Simple_Server::marshal (TAO_OutputCDR &cdr)
{
  return (cdr << this);
}

// TAO_IDL - Generated from
// /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_typecode/objref_typecode.cpp:70

static TAO::TypeCode::Objref<char const *,
                             TAO::Null_RefCount_Policy>
  _tao_tc_Simple_Server (
    ::CORBA::tk_objref,
    "IDL:Simple_Server:1.0",
    "Simple_Server");
  
::CORBA::TypeCode_ptr const _tc_Simple_Server =
  &_tao_tc_Simple_Server;

// TAO_IDL - Generated from
// /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_interface/any_op_cs.cpp:38
TAO_BEGIN_VERSIONED_NAMESPACE_DECL



namespace TAO
{
  template<>
  ::CORBA::Boolean
  Any_Impl_T<Simple_Server>::to_object (
      ::CORBA::Object_ptr &_tao_elem) const
  {
    _tao_elem = ::CORBA::Object::_duplicate (this->value_);
    return true;
  }
}
TAO_END_VERSIONED_NAMESPACE_DECL


TAO_BEGIN_VERSIONED_NAMESPACE_DECL



/// Copying insertion.
void
operator<<= (
  ::CORBA::Any &_tao_any,
  Simple_Server_ptr _tao_elem)
{
  Simple_Server_ptr _tao_objptr =
    Simple_Server::_duplicate (_tao_elem);
  _tao_any <<= &_tao_objptr;
}

/// Non-copying insertion.
void
operator<<= (
  ::CORBA::Any &_tao_any,
  Simple_Server_ptr *_tao_elem)
{
  TAO::Any_Impl_T<Simple_Server>::insert (
    _tao_any,
    Simple_Server::_tao_any_destructor,
    _tc_Simple_Server,
    *_tao_elem);
}

::CORBA::Boolean
operator>>= (
    const ::CORBA::Any &_tao_any,
    Simple_Server_ptr &_tao_elem)
{
  return
    TAO::Any_Impl_T<Simple_Server>::extract (
        _tao_any,
        Simple_Server::_tao_any_destructor,
        _tc_Simple_Server,
        _tao_elem);
}

TAO_END_VERSIONED_NAMESPACE_DECL



// TAO_IDL - Generated from
// /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_array/any_op_cs.cpp:37


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void operator<<= (
    ::CORBA::Any &_tao_any,
    const Simple_Server::HeadPoseArray_forany &_tao_elem
  )
{
  TAO::Any_Array_Impl_T<
      Simple_Server::HeadPoseArray_slice,
      Simple_Server::HeadPoseArray_forany
    >::insert (
        _tao_any,
        Simple_Server::HeadPoseArray_forany::_tao_any_destructor,
        Simple_Server::_tc_HeadPoseArray,
        _tao_elem.nocopy ()
          ? _tao_elem.ptr ()
          : Simple_Server::HeadPoseArray_dup (_tao_elem.in ())
      );
}

::CORBA::Boolean operator>>= (
    const ::CORBA::Any &_tao_any,
    Simple_Server::HeadPoseArray_forany &_tao_elem
  )
{
  return
    TAO::Any_Array_Impl_T<
        Simple_Server::HeadPoseArray_slice,
        Simple_Server::HeadPoseArray_forany
      >::extract (
          _tao_any,
          Simple_Server::HeadPoseArray_forany::_tao_any_destructor,
          Simple_Server::_tc_HeadPoseArray,
          _tao_elem.out ()
        );
}
TAO_END_VERSIONED_NAMESPACE_DECL



// TAO_IDL - Generated from
// /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_sequence/any_op_cs.cpp:45


TAO_BEGIN_VERSIONED_NAMESPACE_DECL


TAO_END_VERSIONED_NAMESPACE_DECL


TAO_BEGIN_VERSIONED_NAMESPACE_DECL


/// Copying insertion.
void operator<<= (
  ::CORBA::Any &_tao_any,
  const Simple_Server::pixels &_tao_elem)
{
  TAO::Any_Dual_Impl_T<Simple_Server::pixels>::insert_copy (
    _tao_any,
    Simple_Server::pixels::_tao_any_destructor,
    Simple_Server::_tc_pixels,
    _tao_elem);
}

/// Non-copying insertion.
void operator<<= (
  ::CORBA::Any &_tao_any,
  Simple_Server::pixels *_tao_elem)
{
  TAO::Any_Dual_Impl_T<Simple_Server::pixels>::insert (
    _tao_any,
    Simple_Server::pixels::_tao_any_destructor,
    Simple_Server::_tc_pixels,
    _tao_elem);
}

/// Extraction to const pointer.
::CORBA::Boolean operator>>= (
  const ::CORBA::Any &_tao_any,
  const Simple_Server::pixels *&_tao_elem)
{
  return TAO::Any_Dual_Impl_T<Simple_Server::pixels>::extract (
    _tao_any,
    Simple_Server::pixels::_tao_any_destructor,
    Simple_Server::_tc_pixels,
    _tao_elem);
}
TAO_END_VERSIONED_NAMESPACE_DECL



// TAO_IDL - Generated from
// /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_sequence/any_op_cs.cpp:45


TAO_BEGIN_VERSIONED_NAMESPACE_DECL


TAO_END_VERSIONED_NAMESPACE_DECL


TAO_BEGIN_VERSIONED_NAMESPACE_DECL


/// Copying insertion.
void operator<<= (
  ::CORBA::Any &_tao_any,
  const Simple_Server::header &_tao_elem)
{
  TAO::Any_Dual_Impl_T<Simple_Server::header>::insert_copy (
    _tao_any,
    Simple_Server::header::_tao_any_destructor,
    Simple_Server::_tc_header,
    _tao_elem);
}

/// Non-copying insertion.
void operator<<= (
  ::CORBA::Any &_tao_any,
  Simple_Server::header *_tao_elem)
{
  TAO::Any_Dual_Impl_T<Simple_Server::header>::insert (
    _tao_any,
    Simple_Server::header::_tao_any_destructor,
    Simple_Server::_tc_header,
    _tao_elem);
}

/// Extraction to const pointer.
::CORBA::Boolean operator>>= (
  const ::CORBA::Any &_tao_any,
  const Simple_Server::header *&_tao_elem)
{
  return TAO::Any_Dual_Impl_T<Simple_Server::header>::extract (
    _tao_any,
    Simple_Server::header::_tao_any_destructor,
    Simple_Server::_tc_header,
    _tao_elem);
}
TAO_END_VERSIONED_NAMESPACE_DECL



// TAO_IDL - Generated from
// /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_structure/any_op_cs.cpp:43


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/// Copying insertion.
void operator<<= (
  ::CORBA::Any &_tao_any,
  const Simple_Server::frameData &_tao_elem)
{
  TAO::Any_Dual_Impl_T<Simple_Server::frameData>::insert_copy (
    _tao_any,
    Simple_Server::frameData::_tao_any_destructor,
    Simple_Server::_tc_frameData,
    _tao_elem);
}

/// Non-copying insertion.
void operator<<= (
  ::CORBA::Any &_tao_any,
  Simple_Server::frameData *_tao_elem)
{
  TAO::Any_Dual_Impl_T<Simple_Server::frameData>::insert (
    _tao_any,
    Simple_Server::frameData::_tao_any_destructor,
    Simple_Server::_tc_frameData,
    _tao_elem);
}

/// Extraction to const pointer.
::CORBA::Boolean operator>>= (
  const ::CORBA::Any &_tao_any,
  const Simple_Server::frameData *&_tao_elem)
{
  return TAO::Any_Dual_Impl_T<Simple_Server::frameData>::extract (
    _tao_any,
    Simple_Server::frameData::_tao_any_destructor,
    Simple_Server::_tc_frameData,
    _tao_elem);
}
TAO_END_VERSIONED_NAMESPACE_DECL



// TAO_IDL - Generated from
// /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_array/cdr_op_cs.cpp:163


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

::CORBA::Boolean operator<< (
    TAO_OutputCDR &strm,
    const Simple_Server::HeadPoseArray_forany &_tao_array)
{
  return strm.write_float_array (
      reinterpret_cast <const ACE_CDR::Float *> (_tao_array.in ()),
      7);
}

::CORBA::Boolean operator>> (
    TAO_InputCDR &strm,
    Simple_Server::HeadPoseArray_forany &_tao_array)
{
  return strm.read_float_array (
      reinterpret_cast <ACE_CDR::Float *> (_tao_array.out ()),
      7);
}

TAO_END_VERSIONED_NAMESPACE_DECL



// TAO_IDL - Generated from
// /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_sequence/cdr_op_cs.cpp:93
#if !defined _TAO_CDR_OP_Simple_Server_pixels_CPP_
#define _TAO_CDR_OP_Simple_Server_pixels_CPP_

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

::CORBA::Boolean operator<< (
    TAO_OutputCDR &strm,
    const Simple_Server::pixels &_tao_sequence)
{
  return TAO::marshal_sequence(strm, _tao_sequence);
}

::CORBA::Boolean operator>> (
    TAO_InputCDR &strm,
    Simple_Server::pixels &_tao_sequence)
{
  return TAO::demarshal_sequence(strm, _tao_sequence);
}


TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* _TAO_CDR_OP_Simple_Server_pixels_CPP_ */

// TAO_IDL - Generated from
// /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_sequence/cdr_op_cs.cpp:93
#if !defined _TAO_CDR_OP_Simple_Server_header_CPP_
#define _TAO_CDR_OP_Simple_Server_header_CPP_

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

::CORBA::Boolean operator<< (
    TAO_OutputCDR &strm,
    const Simple_Server::header &_tao_sequence)
{
  return TAO::marshal_sequence(strm, _tao_sequence);
}

::CORBA::Boolean operator>> (
    TAO_InputCDR &strm,
    Simple_Server::header &_tao_sequence)
{
  return TAO::demarshal_sequence(strm, _tao_sequence);
}


TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* _TAO_CDR_OP_Simple_Server_header_CPP_ */

// TAO_IDL - Generated from
// /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_structure/cdr_op_cs.cpp:49

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

::CORBA::Boolean operator<< (
    TAO_OutputCDR &strm,
    const Simple_Server::frameData &_tao_aggregate)
{
  return
    (strm << _tao_aggregate.m_pixels) &&
    (strm << _tao_aggregate.m_header) &&
    (strm << _tao_aggregate.m_pixelSize) &&
    (strm << _tao_aggregate.m_headerSize);
}

::CORBA::Boolean operator>> (
    TAO_InputCDR &strm,
    Simple_Server::frameData &_tao_aggregate)
{
  return
    (strm >> _tao_aggregate.m_pixels) &&
    (strm >> _tao_aggregate.m_header) &&
    (strm >> _tao_aggregate.m_pixelSize) &&
    (strm >> _tao_aggregate.m_headerSize);
}

TAO_END_VERSIONED_NAMESPACE_DECL



// TAO_IDL - Generated from
// /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_interface/cdr_op_cs.cpp:51

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

::CORBA::Boolean operator<< (
    TAO_OutputCDR &strm,
    const Simple_Server_ptr _tao_objref)
{
  ::CORBA::Object_ptr _tao_corba_obj = _tao_objref;
  return (strm << _tao_corba_obj);
}

::CORBA::Boolean operator>> (
    TAO_InputCDR &strm,
    Simple_Server_ptr &_tao_objref)
{
  ::CORBA::Object_var obj;

  if (!(strm >> obj.inout ()))
    {
      return false;
    }
  
  // Narrow to the right type.
  _tao_objref = TAO::Narrow_Utils< ::Simple_Server>::unchecked_narrow (obj.in ());

  return true;
}

TAO_END_VERSIONED_NAMESPACE_DECL



