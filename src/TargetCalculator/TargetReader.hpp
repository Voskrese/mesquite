/* ***************************************************************** 
    MESQUITE -- The Mesh Quality Improvement Toolkit

    Copyright 2006 Lawrence Livermore National Laboratory.  Under 
    the terms of Contract B545069 with the University of Wisconsin -- 
    Madison, Lawrence Livermore National Laboratory retains certain
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

    (2006) kraftche@cae.wisc.edu    

  ***************************************************************** */


/** \file TargetReader.hpp
 *  \brief 
 *  \author Jason Kraftcheck 
 */

#ifndef MSQ_TARGET_READER_HPP
#define MSQ_TARGET_READER_HPP

#include "Mesquite.hpp"
#include "TargetCalculator.hpp"
#include "ExtraDataUser.hpp"
#include "MeshInterface.hpp"

#ifdef MSQ_USE_OLD_STD_HEADERS
# include <vector.h>
#else
# include <vector>
#endif
#include <string>

namespace Mesquite {

class PatchData;
class MsqError;

/**\brief Internal structure used by TargetReader
 *
 * Store per-PatchData information.
 */
struct TargetReaderData {
  msq_std::vector<TagHandle> handles2D, handles3D; //< tag handles, indexed by #tags/elem
  msq_std::vector< MsqMatrix<3,3> > targets3D; //< cached values for last element
  msq_std::vector< MsqMatrix<3,2> > targets2D; //< cached values for last element
  size_t elementIndex;                      //< element for which values are cached.
};
  

/**\brief Read targets from tag data */
class TargetReader 
 : public TargetCalculator, 
   private ExtraDataUser<TargetReaderData>
{
  public:
  
    TargetReader( bool use_3d_surface_targets,
                  msq_std::string tag_base_name = "MSQ_TARGET_MATRIX" );
    
    virtual ~TargetReader();
    
    virtual bool get_3D_target( PatchData &pd,
                                size_t element,
                                const SamplePoints* pts,
                                unsigned sample,
                                MsqMatrix<3,3>& W_out,
                                MsqError& err );
    
    virtual bool get_2D_target( PatchData &pd,
                                size_t element,
                                const SamplePoints* pts,
                                unsigned sample,
                                MsqMatrix<3,2>& W_out,
                                MsqError& err );

    virtual bool surface_targets_are_3D() const;

  private:
  
    virtual void notify_patch_destroyed( TargetReaderData& data );
    virtual void notify_new_patch( PatchData& pd, TargetReaderData& data );
    virtual void notify_sub_patch( PatchData& pd, TargetReaderData& data,
                                   PatchData& subpatch, const size_t* vert_map,
                                   const size_t* elem_map, MsqError& err );

    msq_std::string tagBaseName;
    const bool use3DSurfaceTargets;
};


} // namespace Mesquite

#endif