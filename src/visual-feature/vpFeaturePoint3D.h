/****************************************************************************
 *
 * $Id: vpFeaturePoint3D.h 5237 2015-01-30 13:52:04Z fspindle $
 *
 * This file is part of the ViSP software.
 * Copyright (C) 2005 - 2014 by INRIA. All rights reserved.
 * 
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * ("GPL") version 2 as published by the Free Software Foundation.
 * See the file LICENSE.txt at the root directory of this source
 * distribution for additional information about the GNU GPL.
 *
 * For using ViSP with software that can not be combined with the GNU
 * GPL, please contact INRIA about acquiring a ViSP Professional 
 * Edition License.
 *
 * See http://www.irisa.fr/lagadic/visp/visp.html for more information.
 * 
 * This software was developed at:
 * INRIA Rennes - Bretagne Atlantique
 * Campus Universitaire de Beaulieu
 * 35042 Rennes Cedex
 * France
 * http://www.irisa.fr/lagadic
 *
 * If you have questions regarding the use of this file, please contact
 * INRIA at visp@inria.fr
 * 
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 *
 * Description:
 * 3D point visual feature.
 *
 * Authors:
 * Eric Marchand
 * Fabien Spindler
 *
 *****************************************************************************/


#ifndef vpFeaturePoint3d_H
#define vpFeaturePoint3d_H

/*!
  \file vpFeaturePoint3D.h
  \brief class that defines the 3D point visual feature.
*/

#include <visp/vpMatrix.h>
#include <visp/vpBasicFeature.h>
#include <visp/vpPoint.h>

#include <visp/vpHomogeneousMatrix.h>
#include <visp/vpRGBa.h>

/*!
  \class vpFeaturePoint3D
  \ingroup VsFeature3
  \brief Class that defines the 3D point visual feature.

  A 3D point visual feature corresponds to a 3D point with \f$
  {\bf X} = (X,Y,Z)\f$ coordinates in the camera frame.

  This class is intended to manipulate the 3D point visual feature
  \f$ s = (X,Y,Z) \f$. The interaction matrix related to \f$ s \f$ is given by:
  \f[
  L = \left[
  \begin{array}{rrrrrr}
  -1 &  0 &  0 &  0 & -Z &  Y \\
   0 & -1 &  0 &  Z &  0 & -X \\
   0 &  0 & -1 & -Y &  X &  0 \\
  \end{array}
  \right]
  \f]

  Two ways are allowed to initialize the feature.

  - The first way by setting the feature values \f$(X,Y,Z)\f$ using
    vpFeaturePoint3D member fonctions like set_X(), set_Y(), set_Z(),
    or also buildFrom().

  - The second by using the feature builder functionalities to
    initialize the feature from a point structure like
    vpFeatureBuilder::create (vpFeaturePoint3D &, const vpPoint &).

  The interaction() method allows to compute the interaction matrix
  \f$ L\f$ associated to the 3D point visual feature, while the
  error() method computes the error vector \f$ (s - s^*)\f$ between the
  current visual feature and the desired one.

  The code below shows how to create a eye-in hand visual servoing
  task using a 3D point feature \f$(X,Y,Z)\f$ that correspond to the
  3D point coordinates in the camera frame. To control six degrees of
  freedom, at least three other features must be considered like
  vpFeatureThetaU visual features. First we create a current (\f$s\f$)
  and desired (\f$s^*\f$) 3D point feature, set the task to use the
  interaction matrix associated to the desired feature \f$L_{s^*}\f$
  and than compute the camera velocity \f$v=-\lambda \; {L_{s^*}}^+ \;
  (s-s^*)\f$. The current feature \f$s\f$ is updated in the while()
  loop while \f$s^*\f$ is set to \f$Z^*=1\f$.

  \code
#include <iostream>
#include <visp/vpFeaturePoint3D.h>
#include <visp/vpHomogeneousMatrix.h>
#include <visp/vpServo.h>

int main()
{
  vpServo task; // Visual servoing task

  // Set the 3D point coordinates in the object frame: oP
  vpPoint point;
  point.setWorldCoordinates(0.1, -0.1, 0);

  vpHomogeneousMatrix cMo; // Pose between the camera and the object frame
  cMo.buildFrom(0, 0, 1.2, 0, 0, 0); 
  // ... cMo need here to be computed from a pose estimation

  point.changeFrame(cMo); // Compute the 3D point coordinates in the camera frame cP = cMo * oP

  // Creation of the current feature s
  vpFeaturePoint3D s;
  s.buildFrom(point); // Initialize the feature from the 3D point coordinates in the camera frame: s=(X,Y,Z)
  s.print();

  // Creation of the desired feature s*. 
  vpFeaturePoint3D s_star;
  s_star.buildFrom(0, 0, 1); // Z*=1 meter
  s_star.print();

  // Set eye-in-hand control law. 
  // The computed velocities will be expressed in the camera frame
  task.setServo(vpServo::EYEINHAND_CAMERA);
  // Interaction matrix is computed with the desired visual features s*
  task.setInteractionMatrixType(vpServo::DESIRED); 
  // Set the constant gain
  double lambda = 0.8;
  task.setLambda(lambda);  

  // Add the 3D point feature to the task
  task.addFeature(s, s_star); 

  // Control loop
  for ( ; ; ) {
    // ... cMo need here to be estimated from for example a pose estimation.
    point.changeFrame(cMo); // Compute the 3D point coordinates in the camera frame cP = cMo * oP
   
    // Update the current 3D point visual feature
    s.buildFrom(point);
    
    // compute the control law
    vpColVector v = task.computeControlLaw(); // camera velocity
  }
}
  \endcode

  If you want to deal only with the \f$(X,Y)\f$ subset feature from the 3D 
  point feature, you have just to modify the addFeature() call in 
  the previous example by the following line. In that case, the dimension 
  of \f$s\f$ is two.

  \code
  // Add the (X,Y) subset feature from the 3D point visual feature to the task
  task.addFeature(s, s_star, vpFeaturePoint3D::selectX() | vpFeaturePoint3D::selectY());
  \endcode

  If you want to build your own control law, this other example shows
  how to create a current (\f$s\f$) and desired (\f$s^*\f$) 3D
  point visual feature, compute the corresponding error
  vector \f$(s-s^*)\f$ and finally build the interaction matrix \f$L_s\f$.

  \code
#include <iostream>
#include <visp/vpFeaturePoint3D.h>
#include <visp/vpHomogeneousMatrix.h>
#include <visp/vpMatrix.h>

int main()
{
  // Set the 3D point coordinates in the object frame: oP
  vpPoint point;
  point.setWorldCoordinates(0.1, -0.1, 0);

  vpHomogeneousMatrix cMo; // Pose between the camera and the object frame
  cMo.buildFrom(0, 0, 1.2, 0, 0, 0); 
  // ... cMo need here to be computed from a pose estimation

  point.changeFrame(cMo); // Compute the 3D point coordinates in the camera frame cP = cMo * oP

  // Creation of the current feature s
  vpFeaturePoint3D s;
  s.buildFrom(point); // Initialize the feature from the 3D point coordinates in the camera frame 
  s.print();

  // Creation of the desired feature s*. 
  vpFeaturePoint3D s_star;
  s_star.buildFrom(0, 0, 1); // Z*=1 meter
  s_star.print();

  // Compute the L_s interaction matrix associated to the current feature
  vpMatrix L = s.interaction();
  std::cout << "L: " << L << std::endl;

  // Compute the error vector (s-s*) for the 3D point feature
  vpColVector e = s.error(s_star); // e = (s-s*)

  std::cout << "e: " << e << std::endl;
}
  \endcode

*/
class VISP_EXPORT vpFeaturePoint3D : public vpBasicFeature

{

public:
  // basic construction
  void init() ;
  // basic constructor
  vpFeaturePoint3D() ;
  //! Destructor. Does nothing.
  virtual ~vpFeaturePoint3D() {}

  /*
    /section Set coordinates
  */

  // build feature from a point (vpPoint)
  void buildFrom(const vpPoint &p) ;
  // set the point XY and Z-coordinates
  void buildFrom(const double X, const double Y, const double Z) ;

  // set the point X-coordinates
  void set_X(const double X) ;
  // set the point Y-coordinates
  void set_Y(const double Y) ;
  // set the point depth (camera frame)
  void set_Z(const double Z) ;
  // set the point XY and Z-coordinates
  void set_XYZ(const double X, const double Y, const double Z) ;

  // get the point X-coordinates
  double get_X()  const ;
  // get the point Y-coordinates
  double get_Y()   const ;
  // get the point depth (camera frame)
  double get_Z() const  ;


  /*
    vpBasicFeature method instantiation
  */

  /*! 

    Function used to select the \f$ X\f$ subset coordinate of the 3D point
    visual feature.

    This function is to use in conjunction with interaction() in order
    to compute the interaction matrix associated to \f$ X\f$ feature.

    See the interaction() method for an usage example.

    This function is also useful in the vpServo class to indicate that
    a subset of the visual feature is to use in the control law:

    \code
    vpFeaturePoint3D p;
    vpServo task;
    ...
    // Add the (X,Y) subset coordinates features from a 3D point to the task
    task.addFeature(p, vpFeaturePoint3D::selectX() | vpFeaturePoint3D::selectY());
    \endcode

    \sa selectY(), selectZ()

  */
  inline static unsigned int selectX()  { return FEATURE_LINE[0] ; }

  /*! 

    Function used to select the \f$ Y\f$ subset coordinate of the 3D point
    visual feature.

    This function is to use in conjunction with interaction() in order
    to compute the interaction matrix associated to \f$ Y\f$ feature.

    See the interaction() method for an usage example.

    This function is also useful in the vpServo class to indicate that
    a subset of the visual feature is to use in the control law:

    \code
    vpFeaturePoint3D p;
    vpServo task;
    ...
    // Add the (X,Y) subset coordinates features from a 3D point to the task
    task.addFeature(p, vpFeaturePoint3D::selectX() | vpFeaturePoint3D::selectY());
    \endcode

    \sa selectX(), selectZ()

  */
  inline static unsigned int selectY()  { return FEATURE_LINE[1] ; }

  /*! 

    Function used to select the \f$ Z\f$ subset coordinate of the 3D point
    visual feature.

    This function is to use in conjunction with interaction() in order
    to compute the interaction matrix associated to \f$ Z\f$ feature.

    See the interaction() method for an usage example.

    This function is also useful in the vpServo class to indicate that
    a subset of the visual feature is to use in the control law:

    \code
    vpFeaturePoint3D p;
    vpServo task;
    ...
    // Add the (Z) subset coordinate feature from a 3D point to the task
    task.addFeature(p, vpFeaturePoint3D::selectZ());
    \endcode

    \sa selectX(), selectY()

  */
  inline static unsigned int selectZ()  { return FEATURE_LINE[2] ; }
  // compute the interaction matrix from a subset a the possible features
  vpMatrix  interaction(const unsigned int select = FEATURE_ALL);
  // compute the error between two visual features from a subset
  // a the possible features
  vpColVector error(const vpBasicFeature &s_star,
                    const unsigned int select = FEATURE_ALL)  ;
  // print the name of the feature
  void print(const unsigned int select=FEATURE_ALL ) const ;

  // feature duplication
  vpFeaturePoint3D *duplicate() const ;

  void display(const vpCameraParameters &cam,
               const vpImage<unsigned char> &I,
               const vpColor &color=vpColor::green,
               unsigned int thickness=1) const ;
  void display(const vpCameraParameters &cam,
               const vpImage<vpRGBa> &I,
               const vpColor &color=vpColor::green,
               unsigned int thickness=1) const ;

} ;



#endif

/*
 * Local variables:
 * c-basic-offset: 2
 * End:
 */
