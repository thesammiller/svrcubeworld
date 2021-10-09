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
// /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_codegen.cpp:149

#ifndef _TAO_IDL_TESTC_V8IMSS_H_
#define _TAO_IDL_TESTC_V8IMSS_H_


#include /**/ "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


#include "tao/AnyTypeCode/AnyTypeCode_methods.h"
#include "tao/AnyTypeCode/Any.h"
#include "tao/ORB.h"
#include "tao/SystemException.h"
#include "tao/Basic_Types.h"
#include "tao/ORB_Constants.h"
#include "tao/Object.h"
#include "tao/Sequence_T.h"
#include "tao/Objref_VarOut_T.h"
#include "tao/Seq_Var_T.h"
#include "tao/Seq_Out_T.h"
#include "tao/Array_VarOut_T.h"
#include "tao/Arg_Traits_T.h"
#include "tao/Basic_Arguments.h"
#include "tao/Special_Basic_Arguments.h"
#include "tao/Any_Insert_Policy_T.h"
#include "tao/Fixed_Size_Argument_T.h"
#include "tao/Var_Size_Argument_T.h"
#include "tao/Object_Argument_T.h"
#include "tao/Fixed_Array_Argument_T.h"
#include "tao/Var_Array_Argument_T.h"
#include /**/ "tao/Version.h"
#include /**/ "tao/Versioned_Namespace.h"

#if TAO_MAJOR_VERSION != 3 || TAO_MINOR_VERSION != 0 || TAO_MICRO_VERSION != 3
#error This file should be regenerated with TAO_IDL
#endif

// TAO_IDL - Generated from
// /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_root/root_ch.cpp:157
TAO_BEGIN_VERSIONED_NAMESPACE_DECL



namespace TAO
{
  template<typename T> class Narrow_Utils;
}
TAO_END_VERSIONED_NAMESPACE_DECL



// TAO_IDL - Generated from
// /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_interface.cpp:748

#if !defined (_SIMPLE_SERVER__VAR_OUT_CH_)
#define _SIMPLE_SERVER__VAR_OUT_CH_

class Simple_Server;
using Simple_Server_ptr = Simple_Server*;
using Simple_Server_var = TAO_Objref_Var_T<Simple_Server>;
using Simple_Server_out = TAO_Objref_Out_T<Simple_Server>;

#endif /* end #if !defined */

// TAO_IDL - Generated from
// /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_interface/interface_ch.cpp:40

class  Simple_Server
  : public virtual ::CORBA::Object
{
public:
  friend class TAO::Narrow_Utils<Simple_Server>;

  // TAO_IDL - Generated from
  // /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_type.cpp:299

  using _ptr_type = Simple_Server_ptr;
  using _var_type = Simple_Server_var;
  using _out_type = Simple_Server_out;

  static void _tao_any_destructor (void *);

  // The static operations.
  static Simple_Server_ptr _duplicate (Simple_Server_ptr obj);

  static void _tao_release (Simple_Server_ptr obj);

  static Simple_Server_ptr _narrow (::CORBA::Object_ptr obj);
  static Simple_Server_ptr _unchecked_narrow (::CORBA::Object_ptr obj);
  static Simple_Server_ptr _nil ();

  // TAO_IDL - Generated from
  // /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_array/array_ch.cpp:52

  typedef ::CORBA::Float HeadPoseArray[7];
  typedef ::CORBA::Float HeadPoseArray_slice;
  struct HeadPoseArray_tag {};
  

  typedef
    TAO_FixedArray_Var_T<
        HeadPoseArray,
        HeadPoseArray_slice,
        HeadPoseArray_tag
      >
    HeadPoseArray_var;

  typedef
    HeadPoseArray
    HeadPoseArray_out;

  typedef
    TAO_Array_Forany_T<
        HeadPoseArray,
        HeadPoseArray_slice,
        HeadPoseArray_tag
      >
    HeadPoseArray_forany;

  static HeadPoseArray_slice *
  HeadPoseArray_alloc (void);

  static void
  HeadPoseArray_free (
      HeadPoseArray_slice *_tao_slice);
  
  static HeadPoseArray_slice *
  HeadPoseArray_dup (
      const HeadPoseArray_slice *_tao_slice);
  
  static void
  HeadPoseArray_copy (
      HeadPoseArray_slice *_tao_to,
      const HeadPoseArray_slice *_tao_from);

  // TAO_IDL - Generated from
  // /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_typecode/typecode_decl.cpp:34

  static ::CORBA::TypeCode_ptr const _tc_HeadPoseArray;

  virtual void send_data (
    ::CORBA::Long microsecond,
    const ::Simple_Server::HeadPoseArray headpose);

  // TAO_IDL - Generated from
  // /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_sequence/sequence_ch.cpp:99

#if !defined (_SIMPLE_SERVER_PIXELS_CH_)
#define _SIMPLE_SERVER_PIXELS_CH_

  class pixels;
  typedef ::TAO_FixedSeq_Var_T<pixels> pixels_var;
  typedef ::TAO_Seq_Out_T<pixels> pixels_out;
  

  class  pixels
    : public
        ::TAO::bounded_value_sequence< ::CORBA::Octet,100000>
  {
  public:
    pixels ();
    pixels (
      ::CORBA::ULong length,
      ::CORBA::Octet* buffer,
      ::CORBA::Boolean release = false);
    pixels (const pixels &) = default;
    pixels (pixels &&) = default;
    pixels& operator= (const pixels &) = default;
    pixels& operator= (pixels &&) = default;
    virtual ~pixels ();
    

    // TAO_IDL - Generated from
    // /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_type.cpp:299

    
    using _var_type = pixels_var;
    using _out_type = pixels_out;

    static void _tao_any_destructor (void *);
  };

#endif /* end #if !defined */

  // TAO_IDL - Generated from
  // /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_typecode/typecode_decl.cpp:34

  static ::CORBA::TypeCode_ptr const _tc_pixels;

  // TAO_IDL - Generated from
  // /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_sequence/sequence_ch.cpp:99

#if !defined (_SIMPLE_SERVER_HEADER_CH_)
#define _SIMPLE_SERVER_HEADER_CH_

  class header;
  typedef ::TAO_FixedSeq_Var_T<header> header_var;
  typedef ::TAO_Seq_Out_T<header> header_out;
  

  class  header
    : public
        ::TAO::bounded_value_sequence< ::CORBA::Octet,100000>
  {
  public:
    header ();
    header (
      ::CORBA::ULong length,
      ::CORBA::Octet* buffer,
      ::CORBA::Boolean release = false);
    header (const header &) = default;
    header (header &&) = default;
    header& operator= (const header &) = default;
    header& operator= (header &&) = default;
    virtual ~header ();
    

    // TAO_IDL - Generated from
    // /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_type.cpp:299

    
    using _var_type = header_var;
    using _out_type = header_out;

    static void _tao_any_destructor (void *);
  };

#endif /* end #if !defined */

  // TAO_IDL - Generated from
  // /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_typecode/typecode_decl.cpp:34

  static ::CORBA::TypeCode_ptr const _tc_header;

  virtual ::Simple_Server::pixels * sendImageData ();

  virtual ::Simple_Server::header * sendHeaderData ();

  virtual ::CORBA::Long sendJpegSize ();

  virtual ::CORBA::Long sendHeaderSize ();

  // TAO_IDL - Generated from
  // /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_interface/interface_ch.cpp:137

  virtual ::CORBA::Boolean _is_a (const char *type_id);
  virtual const char* _interface_repository_id () const;
  virtual ::CORBA::Boolean marshal (TAO_OutputCDR &cdr);

protected:
  // Concrete interface only.
  Simple_Server ();

  // Concrete non-local interface only.
  Simple_Server (
      ::IOP::IOR *ior,
      TAO_ORB_Core *orb_core);
  
  // Non-local interface only.
  Simple_Server (
      TAO_Stub *objref,
      ::CORBA::Boolean _tao_collocated = false,
      TAO_Abstract_ServantBase *servant = nullptr,
      TAO_ORB_Core *orb_core = nullptr);

  virtual ~Simple_Server ();

private:
  // Private and unimplemented for concrete interfaces.
  Simple_Server (const Simple_Server &) = delete;
  Simple_Server (Simple_Server &&) = delete;
  Simple_Server &operator= (const Simple_Server &) = delete;
  Simple_Server &operator= (Simple_Server &&) = delete;
};

// TAO_IDL - Generated from
// /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_typecode/typecode_decl.cpp:34

extern  ::CORBA::TypeCode_ptr const _tc_Simple_Server;

// TAO_IDL - Generated from
// /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_arg_traits.cpp:66

TAO_BEGIN_VERSIONED_NAMESPACE_DECL


// Arg traits specializations.
namespace TAO
{

  // TAO_IDL - Generated from
  // /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_arg_traits.cpp:145

#if !defined (_SIMPLE_SERVER__ARG_TRAITS_)
#define _SIMPLE_SERVER__ARG_TRAITS_

  template<>
  class  Arg_Traits< ::Simple_Server>
    : public
        Object_Arg_Traits_T<
            ::Simple_Server_ptr,
            ::Simple_Server_var,
            ::Simple_Server_out,
            TAO::Objref_Traits<Simple_Server>,
            TAO::Any_Insert_Policy_Stream
          >
  {
  };

#endif /* end #if !defined */

  template<>
  class Arg_Traits< ::Simple_Server::HeadPoseArray_tag>
    : public
        Fixed_Array_Arg_Traits_T<
            ::Simple_Server::HeadPoseArray_var,
            ::Simple_Server::HeadPoseArray_forany,
            TAO::Any_Insert_Policy_Stream
          >
  {
  };

  // TAO_IDL - Generated from
  // /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_arg_traits.cpp:683

  template<>
  class Arg_Traits< ::Simple_Server::pixels>
    : public
        Var_Size_Arg_Traits_T<
            ::Simple_Server::pixels,
            TAO::Any_Insert_Policy_Stream
          >
  {
  };

  // TAO_IDL - Generated from
  // /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_arg_traits.cpp:683

  template<>
  class Arg_Traits< ::Simple_Server::header>
    : public
        Var_Size_Arg_Traits_T<
            ::Simple_Server::header,
            TAO::Any_Insert_Policy_Stream
          >
  {
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL



// TAO_IDL - Generated from
// /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_traits.cpp:60

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Traits specializations.
namespace TAO
{

#if !defined (_SIMPLE_SERVER__TRAITS_)
#define _SIMPLE_SERVER__TRAITS_

  template<>
  struct  Objref_Traits< ::Simple_Server>
  {
    static ::Simple_Server_ptr duplicate (
        ::Simple_Server_ptr p);
    static void release (
        ::Simple_Server_ptr p);
    static ::Simple_Server_ptr nil ();
    static ::CORBA::Boolean marshal (
        const ::Simple_Server_ptr p,
        TAO_OutputCDR & cdr);
  };

#endif /* end #if !defined */
  template<>
  struct  Array_Traits<
      Simple_Server::HeadPoseArray_forany
    >
  {
    static void free (
        Simple_Server::HeadPoseArray_slice * _tao_slice);
    static Simple_Server::HeadPoseArray_slice * dup (
        const Simple_Server::HeadPoseArray_slice * _tao_slice);
    static void copy (
        Simple_Server::HeadPoseArray_slice * _tao_to,
        const Simple_Server::HeadPoseArray_slice * _tao_from);
    static Simple_Server::HeadPoseArray_slice * alloc ();
    static void zero (
        Simple_Server::HeadPoseArray_slice * _tao_slice);
  };
}
TAO_END_VERSIONED_NAMESPACE_DECL



// TAO_IDL - Generated from
// /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_interface/any_op_ch.cpp:41


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

 void operator<<= (::CORBA::Any &, Simple_Server_ptr); // copying
 void operator<<= (::CORBA::Any &, Simple_Server_ptr *); // non-copying
 ::CORBA::Boolean operator>>= (const ::CORBA::Any &, Simple_Server_ptr &);
TAO_END_VERSIONED_NAMESPACE_DECL



// TAO_IDL - Generated from
// /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_array/any_op_ch.cpp:37


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

 void operator<<= (::CORBA::Any &, const Simple_Server::HeadPoseArray_forany &);
 ::CORBA::Boolean operator>>= (const ::CORBA::Any &, Simple_Server::HeadPoseArray_forany &);
TAO_END_VERSIONED_NAMESPACE_DECL



// TAO_IDL - Generated from
// /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_sequence/any_op_ch.cpp:47


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

 void operator<<= ( ::CORBA::Any &, const Simple_Server::pixels &); // copying version
 void operator<<= ( ::CORBA::Any &, Simple_Server::pixels*); // noncopying version
 ::CORBA::Boolean operator>>= (const ::CORBA::Any &, const Simple_Server::pixels *&);
TAO_END_VERSIONED_NAMESPACE_DECL



// TAO_IDL - Generated from
// /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_sequence/any_op_ch.cpp:47


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

 void operator<<= ( ::CORBA::Any &, const Simple_Server::header &); // copying version
 void operator<<= ( ::CORBA::Any &, Simple_Server::header*); // noncopying version
 ::CORBA::Boolean operator>>= (const ::CORBA::Any &, const Simple_Server::header *&);
TAO_END_VERSIONED_NAMESPACE_DECL



// TAO_IDL - Generated from
// /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_interface/cdr_op_ch.cpp:41

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

 ::CORBA::Boolean operator<< (TAO_OutputCDR &, const Simple_Server_ptr );
 ::CORBA::Boolean operator>> (TAO_InputCDR &, Simple_Server_ptr &);

TAO_END_VERSIONED_NAMESPACE_DECL



// TAO_IDL - Generated from
// /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_array/cdr_op_ch.cpp:99


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

 CORBA::Boolean operator<< (TAO_OutputCDR &strm, const Simple_Server::HeadPoseArray_forany &_tao_array);
 ::CORBA::Boolean operator>> (TAO_InputCDR &, Simple_Server::HeadPoseArray_forany &_tao_array);

TAO_END_VERSIONED_NAMESPACE_DECL


// TAO_IDL - Generated from
// /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_sequence/cdr_op_ch.cpp:65

#if !defined _TAO_CDR_OP_Simple_Server_pixels_H_
#define _TAO_CDR_OP_Simple_Server_pixels_H_
TAO_BEGIN_VERSIONED_NAMESPACE_DECL


 ::CORBA::Boolean operator<< (
    TAO_OutputCDR &strm,
    const Simple_Server::pixels &_tao_sequence);
 ::CORBA::Boolean operator>> (
    TAO_InputCDR &strm,
    Simple_Server::pixels &_tao_sequence);

TAO_END_VERSIONED_NAMESPACE_DECL



#endif /* _TAO_CDR_OP_Simple_Server_pixels_H_ */

// TAO_IDL - Generated from
// /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_sequence/cdr_op_ch.cpp:65

#if !defined _TAO_CDR_OP_Simple_Server_header_H_
#define _TAO_CDR_OP_Simple_Server_header_H_
TAO_BEGIN_VERSIONED_NAMESPACE_DECL


 ::CORBA::Boolean operator<< (
    TAO_OutputCDR &strm,
    const Simple_Server::header &_tao_sequence);
 ::CORBA::Boolean operator>> (
    TAO_InputCDR &strm,
    Simple_Server::header &_tao_sequence);

TAO_END_VERSIONED_NAMESPACE_DECL



#endif /* _TAO_CDR_OP_Simple_Server_header_H_ */

// TAO_IDL - Generated from
// /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_codegen.cpp:1685
#if defined (__ACE_INLINE__)
#include "testC.inl"
#endif /* defined INLINE */

#endif /* ifndef */

