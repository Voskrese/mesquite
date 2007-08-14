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

#ifndef MSQ_LINEAR_HEXAHEDRON_HPP
#define MSQ_LINEAR_HEXAHEDRON_HPP

#include "Mesquite.hpp"
#include "MappingFunction.hpp"

namespace Mesquite {

/**\brief Linear mapping function for a hexahedral element
 *
 * This class implements the MappingFunction interface, providing
 * a Linear shape function for hexahedral elements.
 *
 * \f$\vec{x}(\xi,\eta,\zeta) = \sum_{i=0}^{7} N_i(\xi,\eta,\zeta) \vec{x_i}\f$
 * 
 * \f$N_0(\xi,\eta,\zeta) = \frac{1}{8}(1-\xi)(1-\eta)(1-\zeta)\f$
 * 
 * \f$N_1(\xi,\eta,\zeta) = \frac{1}{8}(1+\xi)(1-\eta)(1-\zeta)\f$
 * 
 * \f$N_2(\xi,\eta,\zeta) = \frac{1}{8}(1+\xi)(1+\eta)(1-\zeta)\f$
 * 
 * \f$N_3(\xi,\eta,\zeta) = \frac{1}{8}(1-\xi)(1+\eta)(1-\zeta)\f$
 * 
 * \f$N_4(\xi,\eta,\zeta) = \frac{1}{8}(1-\xi)(1-\eta)(1+\zeta)\f$
 * 
 * \f$N_5(\xi,\eta,\zeta) = \frac{1}{8}(1+\xi)(1-\eta)(1+\zeta)\f$
 * 
 * \f$N_6(\xi,\eta,\zeta) = \frac{1}{8}(1+\xi)(1+\eta)(1+\zeta)\f$
 * 
 * \f$N_7(\xi,\eta,\zeta) = \frac{1}{8}(1-\xi)(1+\eta)(1+\zeta)\f$
 * 
 */ 
class LinearHexahedron : public MappingFunction
{
public:

  virtual
  EntityTopology element_topology() const;
  
  virtual 
  void coefficients_at_corner( unsigned corner, 
                               unsigned nodebits,
                               msq_std::vector<double>& coeff_out,
                               MsqError& err ) const; 

  virtual 
  void coefficients_at_mid_edge( unsigned edge, 
                                 unsigned nodebits,
                                 msq_std::vector<double>& coeff_out,
                                 MsqError& err ) const;

  virtual 
  void coefficients_at_mid_face( unsigned face, 
                                 unsigned nodebits,
                                 msq_std::vector<double>& coeff_out,
                                 MsqError& err ) const;

  virtual 
  void coefficients_at_mid_elem( unsigned nodebits,
                                 msq_std::vector<double>& coeff_out,
                                 MsqError& err ) const;

  virtual 
  void derivatives_at_corner( unsigned corner, 
                              unsigned nodebits,
                              msq_std::vector<size_t>& vertex_indices_out,
                              msq_std::vector<double>& d_coeff_d_xi_out,
                              MsqError& err ) const;

  virtual 
  void derivatives_at_mid_edge( unsigned edge, 
                                unsigned nodebits,
                                msq_std::vector<size_t>& vertex_indices_out,
                                msq_std::vector<double>& d_coeff_d_xi_out,
                                MsqError& err ) const;

  virtual 
  void derivatives_at_mid_face( unsigned face, 
                                unsigned nodebits,
                                msq_std::vector<size_t>& vertex_indices_out,
                                msq_std::vector<double>& d_coeff_d_xi_out,
                                MsqError& err ) const;

  virtual 
  void derivatives_at_mid_elem( unsigned nodebits,
                                msq_std::vector<size_t>& vertex_indices_out,
                                msq_std::vector<double>& d_coeff_d_xi_out,
                                MsqError& err ) const;
};



} // namespace Mesquite

#endif