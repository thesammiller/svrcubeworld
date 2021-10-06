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
// /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_codegen.cpp:451

#ifndef _TAO_IDL_TESTS_RFEKKP_H_
#define _TAO_IDL_TESTS_RFEKKP_H_


#include "testC.h"
#include "tao/PortableServer/Basic_SArguments.h"
#include "tao/PortableServer/Special_Basic_SArguments.h"
#include "tao/PortableServer/Fixed_Size_SArgument_T.h"
#include "tao/PortableServer/Var_Size_SArgument_T.h"
#include "tao/PortableServer/Object_SArg_Traits.h"
#include "tao/PortableServer/UB_String_SArguments.h"
#include "tao/PortableServer/Fixed_Array_SArgument_T.h"
#include "tao/PortableServer/Var_Array_SArgument_T.h"
#include "tao/PortableServer/get_arg.h"
#include "tao/Special_Basic_Arguments.h"
#include "tao/UB_String_Arguments.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/PortableServer.h"
#include "tao/PortableServer/Servant_Base.h"

// TAO_IDL - Generated from
// /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_arg_traits.cpp:66

TAO_BEGIN_VERSIONED_NAMESPACE_DECL


// Arg traits specializations.
namespace TAO
{

  // TAO_IDL - Generated from
  // /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_arg_traits.cpp:145

#if !defined (_SIMPLE_SERVER__SARG_TRAITS_)
#define _SIMPLE_SERVER__SARG_TRAITS_

  template<>
  class  SArg_Traits< ::Simple_Server>
    : public
        Object_SArg_Traits_T<
            ::Simple_Server_ptr,
            ::Simple_Server_var,
            ::Simple_Server_out,
            TAO::Any_Insert_Policy_Stream
          >
  {
  };

#endif /* end #if !defined */

  template<>
  class SArg_Traits< ::Simple_Server::HeadPoseArray_tag>
    : public
        Fixed_Array_SArg_Traits_T<
            ::Simple_Server::HeadPoseArray_var,
            ::Simple_Server::HeadPoseArray_forany,
            TAO::Any_Insert_Policy_Stream
          >
  {
  };

  template<>
  class SArg_Traits< ::Simple_Server::pixels_tag>
    : public
        Fixed_Array_SArg_Traits_T<
            ::Simple_Server::pixels_var,
            ::Simple_Server::pixels_forany,
            TAO::Any_Insert_Policy_Stream
          >
  {
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL



// TAO_IDL - Generated from
// /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_interface/interface_sh.cpp:73

class POA_Simple_Server;
typedef POA_Simple_Server *POA_Simple_Server_ptr;

class  POA_Simple_Server
  : public virtual PortableServer::ServantBase
{
protected:
  POA_Simple_Server ();

public:
  /// Useful for template programming.
  typedef ::Simple_Server _stub_type;
  typedef ::Simple_Server_ptr _stub_ptr_type;
  typedef ::Simple_Server_var _stub_var_type;

  POA_Simple_Server (const POA_Simple_Server& rhs);
  virtual ~POA_Simple_Server ();

  virtual ::CORBA::Boolean _is_a (const char* logical_type_id);

  virtual void _dispatch (
      TAO_ServerRequest & req,
      TAO::Portable_Server::Servant_Upcall *servant_upcall);
  
  ::Simple_Server *_this ();
  
  virtual const char* _interface_repository_id () const;

  // TAO_IDL - Generated from
  // /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_operation/operation_sh.cpp:36

  virtual void send_data (
    ::CORBA::Long microsecond,
    const ::Simple_Server::HeadPoseArray headpose) = 0;

  static void send_data_skel (
      TAO_ServerRequest &server_request,
      TAO::Portable_Server::Servant_Upcall *servant_upcall,
      TAO_ServantBase *servant);

  // TAO_IDL - Generated from
  // /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_operation/operation_sh.cpp:36

  virtual ::Simple_Server::pixels_slice * sendImageData () = 0;

  static void sendImageData_skel (
      TAO_ServerRequest &server_request,
      TAO::Portable_Server::Servant_Upcall *servant_upcall,
      TAO_ServantBase *servant);

  // TAO_IDL - Generated from
  // /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_operation/operation_sh.cpp:36

  virtual ::CORBA::Long sendJpegSize () = 0;

  static void sendJpegSize_skel (
      TAO_ServerRequest &server_request,
      TAO::Portable_Server::Servant_Upcall *servant_upcall,
      TAO_ServantBase *servant);
};

#endif /* ifndef */

