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
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 
    diachin2@llnl.gov, djmelan@sandia.gov, mbrewer@sandia.gov, 
    pknupp@sandia.gov, tleurent@mcs.anl.gov, tmunson@mcs.anl.gov      
   
  ***************************************************************** */
 
/*! \file LVQDTargetCalculators.cpp

file for the Mesquite::LVQDTargetCalculator class

  \author Thomas Leurent
  \date   2004-05-31
 */


#include "LVQDTargetCalculator.hpp"
#include "PatchDataUser.hpp"
#include "PatchData.hpp"
#include "MeshSet.hpp"
#include "MsqTimer.hpp"

using namespace Mesquite;



/*! The type of targets computed by this function is selected by the constructor of
    the base classes. */
void LVQDTargetCalculator::compute_target_matrices(PatchData &pd, MsqError &err)
{
  FunctionTimer ft( "LVQDTargetCalculator::compute_target_matrices" );

  // Gets from the reference mesh a patch ref_pd equivalent to the patch pd of the main mesh.
  PatchData ref_pd;
  PatchDataParameters ref_pd_params(*originator);
  ref_pd_params.no_target_calculator();
  refMesh->get_next_patch(ref_pd, ref_pd_params, err); MSQ_ERRRTN(err);
  
  // Make sure topology of ref_pd and pd are equal
  size_t num_elements=pd.num_elements();
  size_t num_vertices=pd.num_vertices();
  if (num_elements != pd.num_elements() || num_vertices != pd.num_vertices())
  {
    MSQ_SETERR(err)(MsqError::INVALID_STATE);
    return;
  }
    
  
  MsqMeshEntity* elems = pd.get_element_array(err); MSQ_ERRRTN(err);
  MsqMeshEntity* elems_ref = ref_pd.get_element_array(err); MSQ_ERRRTN(err);
  pd.allocate_target_matrices(err); MSQ_ERRRTN(err);

  Matrix3D L_guides[MSQ_MAX_NUM_VERT_PER_ENT];
  Matrix3D V_guides[MSQ_MAX_NUM_VERT_PER_ENT];
  Matrix3D Q_guides[MSQ_MAX_NUM_VERT_PER_ENT];
  Matrix3D D_guides[MSQ_MAX_NUM_VERT_PER_ENT];
  Matrix3D V, Q, Delta;
  double Lambda=1;
  
//  std::map<enum guide_type, Matrix3D[MSQ_MAX_NUM_VERT_PER_ENT]> guides;

  if (lambdaBase == AVERAGE)
    Lambda = ref_pd.get_average_Lambda_3d(err); MSQ_ERRRTN(err);
  
  for (size_t i=0; i<num_elements; ++i) {
    MsqTag* tag = elems[i].get_tag();
    int nve = elems[i].vertex_count();
    if (nve != elems_ref[i].vertex_count())
    {
      MSQ_SETERR(err)(MsqError::INVALID_STATE);
      return;
    }

    if (lambdaBase == REGULAR) {
      compute_guide_matrices(guideLambda, ref_pd, i, L_guides, nve, err);
      MSQ_ERRRTN(err); }
    compute_guide_matrices(guideV, ref_pd, i, V_guides, nve, err); MSQ_ERRRTN(err);
    compute_guide_matrices(guideQ, ref_pd, i, Q_guides, nve, err); MSQ_ERRRTN(err);
    compute_guide_matrices(guideDelta, ref_pd, i, D_guides, nve, err); MSQ_ERRRTN(err);

    for (int c=0; c<nve; ++c) {
      if (lambdaBase == REGULAR) Lambda = compute_Lambda(L_guides[c], err); MSQ_ERRRTN(err);
      V = compute_V_3D(V_guides[c], err); MSQ_ERRRTN(err);
      Q = compute_Q_3D(Q_guides[c], err); MSQ_ERRRTN(err);
      Delta = compute_Delta_3D(D_guides[c], err); MSQ_ERRRTN(err);
      tag->target_matrix(c) = Lambda * V * Q * Delta;
    }
  }
}

