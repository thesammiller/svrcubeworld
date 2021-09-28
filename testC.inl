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
// /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_array/array_ci.cpp:147

TAO_BEGIN_VERSIONED_NAMESPACE_DECL


ACE_INLINE
void
TAO::Array_Traits<Simple_Server::HeadPoseArray_forany>::free (
    Simple_Server::HeadPoseArray_slice * _tao_slice)
{
  Simple_Server::HeadPoseArray_free (_tao_slice);
}

ACE_INLINE
Simple_Server::HeadPoseArray_slice *
TAO::Array_Traits<Simple_Server::HeadPoseArray_forany>::dup (
    const Simple_Server::HeadPoseArray_slice * _tao_slice)
{
  return Simple_Server::HeadPoseArray_dup (_tao_slice);
}

ACE_INLINE
void
TAO::Array_Traits<Simple_Server::HeadPoseArray_forany>::copy (
    Simple_Server::HeadPoseArray_slice * _tao_to,
    const Simple_Server::HeadPoseArray_slice * _tao_from)
{
  Simple_Server::HeadPoseArray_copy (_tao_to, _tao_from);
}

ACE_INLINE
void
TAO::Array_Traits<Simple_Server::HeadPoseArray_forany>::zero (
    Simple_Server::HeadPoseArray_slice * _tao_slice)
{
  // Zero each individual element.
  for ( ::CORBA::ULong i0 = 0; i0 < 7; ++i0)
    {
      _tao_slice[i0] = ::CORBA::Float ();
    }
}

ACE_INLINE
Simple_Server::HeadPoseArray_slice *
TAO::Array_Traits<Simple_Server::HeadPoseArray_forany>::alloc ()
{
  return Simple_Server::HeadPoseArray_alloc ();
}


TAO_END_VERSIONED_NAMESPACE_DECL


// TAO_IDL - Generated from
// /home/smiller/Development/Vandy/Term4/SoftEng/renderer/svr-cubeworld/GLFW-CMake-starter/extern/ACE_wrappers/build/default/TAO/TAO_IDL/be/be_visitor_interface/interface_ci.cpp:59

ACE_INLINE
Simple_Server::Simple_Server (
    TAO_Stub *objref,
    ::CORBA::Boolean _tao_collocated,
    TAO_Abstract_ServantBase *servant,
    TAO_ORB_Core *oc)
  : ::CORBA::Object (objref, _tao_collocated, servant, oc)
{
}

ACE_INLINE
Simple_Server::Simple_Server (
    ::IOP::IOR *ior,
    TAO_ORB_Core *oc)
  : ::CORBA::Object (ior, oc)
{
}
