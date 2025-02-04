/* ***************************************************************** 
    MESQUITE -- The Mesh Quality Improvement Toolkit

    Copyright 2004 Sandia Corporation and Argonne National
    Laboratory.  Under the terms of Contract DE-AC04-94AL85000 
    with Sandia Corporation, the U.S. Government retains certain 
    rights in this software.

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License 
    (lgpl.txt) along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 
    diachin2@llnl.gov, djmelan@sandia.gov, mbrewer@sandia.gov, 
    pknupp@sandia.gov, tleurent@mcs.anl.gov, tmunson@mcs.anl.gov,
    kraftche@cae.wisc.edu         
   
  ***************************************************************** */
/*!
  \file   PlanarDomain.hpp
  \brief  


  \author Thomas Leurent
  \date   2002-01-17
*/


#ifndef MSQ_PLANAR_DOMAIN_HPP
#define MSQ_PLANAR_DOMAIN_HPP

#include "Mesquite_MeshInterface.hpp"
#include "Mesquite_Vector3D.hpp"

namespace MESQUITE_NS
{
  /*! \class PlanarDomain
       This is a template for a planar domain.
       It will provide the normal information necessary for surface mesh optimization.
    */
  class MESQUITE_EXPORT PlanarDomain : public Mesquite::MeshDomain
  {
  public:
    enum Plane{ XY = 2, XZ = 1, YZ = 0 };
  
    inline PlanarDomain( Plane orient, double offset = 0.0 )
    {
      Vector3D normal(0,0,0), point(0,0,0);
      normal[orient] = 1.0;
      point[orient] = offset;
      set_plane( normal, point );
    }
    
    PlanarDomain() {}
  
    inline PlanarDomain(const Vector3D& normal, const Vector3D& point)
      { set_plane( normal, point ); }
    
    virtual ~PlanarDomain();
      
    void fit_vertices( Mesh* mesh, 
                       MsqError& err,
                       double epsilon = 0.0 );
      
    void fit_vertices( Mesh* mesh, 
                       const Mesh::VertexHandle* vertex_array,
                       size_t vertex_array_length,
                       MsqError& err,
                       double epsilon = 0.0 );

    void set_plane( const Vector3D& normal, const Vector3D& point );

    void flip();

    const Vector3D& get_normal() const { return mNormal; }
    
    double get_coeff() { return mCoeff; }
    
    Vector3D get_origin() const { return -mCoeff * mNormal; }
    
    virtual void snap_to(Mesh::VertexHandle entity_handle,
                         Vector3D &coordinate) const;
    
    virtual void vertex_normal_at(Mesh::VertexHandle entity_handle,
                                  Vector3D &coordinate) const;
    
    virtual void element_normal_at(Mesh::ElementHandle entity_handle,
                                   Vector3D &coordinate) const;
    
    virtual void vertex_normal_at(const Mesh::VertexHandle* handle,
                                  Vector3D coords[],
                                  unsigned count,
                                  MsqError& err) const;

    virtual void closest_point( Mesh::VertexHandle handle,
                                const Vector3D& position,
                                Vector3D& closest,
                                Vector3D& normal,
                                MsqError& err ) const;

    virtual void domain_DoF( const Mesh::VertexHandle* handle_array,
                             unsigned short* dof_array,
                             size_t num_vertices,
                             MsqError& err ) const;

  private:
    Vector3D mNormal;
    double mCoeff;
  };
}

#endif
