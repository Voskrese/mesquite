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


/** \file PMeanPTemplate.hpp
 *  \brief previous name: PowerMeanP.hpp
 *  \author Jason Kraftcheck 
 */

#ifndef MSQ_P_MEAN_P_TEMPLATE_HPP
#define MSQ_P_MEAN_P_TEMPLATE_HPP

#include "Mesquite.hpp"
#include "ObjectiveFunctionTemplate.hpp"
#include "Exponent.hpp"
#include "Matrix3D.hpp"

namespace Mesquite {

/**\brief \f$\frac{1}{n}\sum_{i=1}^n\mu(s_i)^p\f$
 *
 * This class implements an objective function that is the 
 * power-mean of the quality metric evalutations raised to the
 * power-mean power.  That is, the sum of each quality metric value
 * raised to a power, divided by the totoal number of quality metric
 * values.
 */
class MESQUITE_EXPORT PMeanPTemplate : public ObjectiveFunctionTemplate
{
  public:
  
      /**
       *\param power   The exponent to use for the power-mean
       *\param qm      The quality metric.
       */
    PMeanPTemplate( double power, QualityMetric* qm = 0 ) 
      : ObjectiveFunctionTemplate(qm)
    { 
      clear(); 
      set_power( power );
    }
    
      /**\brief copy constructor 
       *
       * Define a copy constructor because the compiler-provided 
       * default one would also copy the temporary arrays, which
       * would be a waste of time.
       */
    PMeanPTemplate( const PMeanPTemplate& copy )
      : ObjectiveFunctionTemplate( copy ),
        mPower( copy.mPower ),
        mCount( copy.mCount ),
        mPowSum( copy.mPowSum ),
        saveCount( copy.saveCount ),
        savePowSum( copy.savePowSum )
      {}
    
    virtual ~PMeanPTemplate() 
      {}
    
    double get_power() const 
      { return mPower.value(); }
      
    void set_power( double p ) 
      { mPower = p; }
    
    virtual bool evaluate( EvalType type, 
                           PatchData& pd,
                           double& value_out,
                           bool free,
                           MsqError& err ); 

    virtual bool evaluate_with_gradient( EvalType type, 
                                         PatchData& pd,
                                         double& value_out,
                                         msq_std::vector<Vector3D>& grad_out,
                                         MsqError& err ); 
    
    virtual bool evaluate_with_Hessian( EvalType type, 
                                        PatchData& pd,
                                        double& value_out,
                                        msq_std::vector<Vector3D>& grad_out,
                                        MsqHessian& Hessian_out,
                                        MsqError& err ); 

    virtual ObjectiveFunction* clone() const;

    virtual void clear();
  
  protected:
  
    /**\brief Handle EvalType for all eval functions, return OF value 
     *
     * This function implements the common handling of the EvalType
     * argument for all forms of the 'evaluate' method.  
     *
     * NOTE:  This function modifies accumulated values depenending
     *        on the value of EvalType.
     *\param power_sum  The sum over the current patch
     *\param count      The number of qm evaluations for the current patch
     *\param type       The evaluation type passed to 'evaluate'
     *\param global_count The total, accumulated number of QM evaluations
     *\return The objective function value to return from 'evaluate'
     */
    double get_value( double power_sum, size_t count, EvalType type, size_t& global_count );
  
    Exponent mPower;  /**< The power to use */
 
  private:
     
    size_t mCount;    /**< The number of accumulated entires */
    double mPowSum;   /**< The accumulated sum of values to the mPower */
    size_t saveCount; /**< Saved count from previous patch */
    double savePowSum;/**< Saved sum from previous patch */
  
  protected:
    
    /** Temporary storage for qm sample handles */
    mutable msq_std::vector<size_t> qmHandles;
    /** Temporary storage for qm vertex indices */
    mutable msq_std::vector<size_t> mIndices;
    /** Temporary storage for qm gradient */
    mutable msq_std::vector<Vector3D> mGradient;
     /** Temporary storage for qm Hessian */
    mutable msq_std::vector<Matrix3D> mHessian;
};

} // namespace Mesquite

#endif