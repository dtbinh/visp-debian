/****************************************************************************
 *
 * This file is part of the ViSP software.
 * Copyright (C) 2005 - 2015 by Inria. All rights reserved.
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * ("GPL") version 2 as published by the Free Software Foundation.
 * See the file LICENSE.txt at the root directory of this source
 * distribution for additional information about the GNU GPL.
 *
 * For using ViSP with software that can not be combined with the GNU
 * GPL, please contact Inria about acquiring a ViSP Professional
 * Edition License.
 *
 * See http://visp.inria.fr for more information.
 *
 * This software was developed at:
 * Inria Rennes - Bretagne Atlantique
 * Campus Universitaire de Beaulieu
 * 35042 Rennes Cedex
 * France
 *
 * If you have questions regarding the use of this file, please contact
 * Inria at visp@inria.fr
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Description:
 * Interface for the ADEPT Viper 650 robot.
 *
 * Authors:
 * Fabien Spindler
 *
 *****************************************************************************/

/*!

  \file vpViper650.cpp

  Modelisation of the ADEPT Viper 650 robot.

*/

#include <visp3/core/vpDebug.h>
#include <visp3/robot/vpViper650.h>
#include <visp3/core/vpMath.h>
#include <visp3/core/vpXmlParserCamera.h>

static const char *opt_viper650[] = {"CAMERA", "eMc_ROT_XYZ","eMc_TRANS_XYZ",
                                     NULL};

#ifdef VISP_HAVE_ACCESS_TO_NAS
const char * const vpViper650::CONST_EMC_MARLIN_F033C_WITHOUT_DISTORTION_FILENAME
#if defined(_WIN32)
= "Z:/robot/Viper650/current/include/const_eMc_MarlinF033C_without_distortion_Viper650.cnf";
#else
= "/udd/fspindle/robot/Viper650/current/include/const_eMc_MarlinF033C_without_distortion_Viper650.cnf";
#endif

const char * const vpViper650::CONST_EMC_MARLIN_F033C_WITH_DISTORTION_FILENAME
#if defined(_WIN32) 
= "Z:/robot/Viper650/current/include/const_eMc_MarlinF033C_with_distortion_Viper650.cnf";
#else
= "/udd/fspindle/robot/Viper650/current/include/const_eMc_MarlinF033C_with_distortion_Viper650.cnf";
#endif

const char * const vpViper650::CONST_EMC_PTGREY_FLEA2_WITHOUT_DISTORTION_FILENAME
#if defined(_WIN32)
= "Z:/robot/Viper650/current/include/const_eMc_PTGreyFlea2_without_distortion_Viper650.cnf";
#else
= "/udd/fspindle/robot/Viper650/current/include/const_eMc_PTGreyFlea2_without_distortion_Viper650.cnf";
#endif

const char * const vpViper650::CONST_EMC_PTGREY_FLEA2_WITH_DISTORTION_FILENAME
#if defined(_WIN32) 
= "Z:/robot/Viper650/current/include/const_eMc_PTGreyFlea2_with_distortion_Viper650.cnf";
#else
= "/udd/fspindle/robot/Viper650/current/include/const_eMc_PTGreyFlea2_with_distortion_Viper650.cnf";
#endif

const char * const vpViper650::CONST_EMC_SCHUNK_GRIPPER_WITHOUT_DISTORTION_FILENAME
#if defined(_WIN32)
= "Z:/robot/Viper650/current/include/const_eMc_schunk_gripper_without_distortion_Viper650.cnf";
#else
= "/udd/fspindle/robot/Viper650/current/include/const_eMc_schunk_gripper_without_distortion_Viper650.cnf";
#endif

const char * const vpViper650::CONST_EMC_SCHUNK_GRIPPER_WITH_DISTORTION_FILENAME
#if defined(_WIN32)
= "Z:/robot/Viper650/current/include/const_eMc_schunk_gripper_with_distortion_Viper650.cnf";
#else
= "/udd/fspindle/robot/Viper650/current/include/const_eMc_schunk_gripper_with_distortion_Viper650.cnf";
#endif

const char * const vpViper650::CONST_EMC_GENERIC_WITHOUT_DISTORTION_FILENAME
#if defined(_WIN32)
= "Z:/robot/Viper650/current/include/const_eMc_generic_without_distortion_Viper650.cnf";
#else
= "/udd/fspindle/robot/Viper650/current/include/const_eMc_generic_without_distortion_Viper650.cnf";
#endif

const char * const vpViper650::CONST_EMC_GENERIC_WITH_DISTORTION_FILENAME
#if defined(_WIN32)
= "Z:/robot/Viper650/current/include/const_eMc_generic_with_distortion_Viper650.cnf";
#else
= "/udd/fspindle/robot/Viper650/current/include/const_eMc_generic_with_distortion_Viper650.cnf";
#endif


const char * const vpViper650::CONST_CAMERA_FILENAME
#if defined(_WIN32) 
= "Z:/robot/Viper650/current/include/const_camera_Viper650.xml";
#else
= "/udd/fspindle/robot/Viper650/current/include/const_camera_Viper650.xml";
#endif


#endif // VISP_HAVE_ACCESS_TO_NAS

const char * const vpViper650::CONST_MARLIN_F033C_CAMERA_NAME = "Marlin-F033C-12mm";
const char * const vpViper650::CONST_PTGREY_FLEA2_CAMERA_NAME = "PTGrey-Flea2-6mm";
const char * const vpViper650::CONST_SCHUNK_GRIPPER_CAMERA_NAME = "Schunk-Gripper-PTGrey-Flea2-6mm";
const char * const vpViper650::CONST_GENERIC_CAMERA_NAME = "Generic-camera";

const vpViper650::vpToolType vpViper650::defaultTool = vpViper650::TOOL_PTGREY_FLEA2_CAMERA;



/*!

  Default constructor.
  Sets the specific parameters like the Denavit Hartenberg parameters.

*/
vpViper650::vpViper650()
  : tool_current(vpViper650::defaultTool), projModel(vpCameraParameters::perspectiveProjWithoutDistortion)
{
  // Denavit Hartenberg parameters
  a1 = 0.075;
  a2 = 0.270;
  a3 = 0.090;
  d1 = 0.335;
  d4 = 0.295;
  d6 = 0.080 + 0.1016; // To take into account the offset to go to the tool changer
  c56 = -341.33 / 9102.22;

  // Software joint limits in radians
  joint_min[0] = vpMath::rad(-170);
  joint_min[1] = vpMath::rad(-190);
  joint_min[2] = vpMath::rad(-29);
  joint_min[3] = vpMath::rad(-190);
  joint_min[4] = vpMath::rad(-120);
  joint_min[5] = vpMath::rad(-360);
  joint_max[0] = vpMath::rad(170);
  joint_max[1] = vpMath::rad(45);
  joint_max[2] = vpMath::rad(256);
  joint_max[3] = vpMath::rad(190);
  joint_max[4] = vpMath::rad(120);
  joint_max[5] = vpMath::rad(360);

  init(); // Set the default tool
}

/*!

  Initialize the robot with the default tool vpViper650::defaultTool.
 */
void
vpViper650::init (void)
{
  this->init(vpViper650::defaultTool);
  return;
}

/*!

  Read files containing the constant parameters related to the robot
  tools in order to set the end-effector to tool transformation.

  \param camera_extrinsic_parameters : Filename containing the camera
  extrinsic parameters.

*/
#ifdef VISP_HAVE_ACCESS_TO_NAS
void
vpViper650::init (const char *camera_extrinsic_parameters)
{
  //vpTRACE ("Parse camera file \""%s\"".", camera_filename);
  this->parseConfigFile (camera_extrinsic_parameters);

  return ;
}
#endif

/*!

  Set the constant parameters related to the robot kinematics and to
  the end-effector to camera transformation (\f$^e{\bf M}c\f$)
  corresponding to the camera extrinsic parameters. These last
  parameters depend on the camera and projection model in use and are
  loaded from predefined files or parameters.

  \warning If the macro VISP_HAVE_ACCESS_TO_NAS is defined in vpConfig.h
  this function reads the camera extrinsic parameters from the file
  corresponding to the specified camera type and projection type.
  Otherwise corresponding default parameters are loaded.

  \param tool : Camera in use.

  \param proj_model : Projection model of the camera.

  \sa init(vpViper650::vpToolType, const std::string&),
  init(vpViper650::vpToolType, const vpHomogeneousMatrix&)

*/
void
vpViper650::init (vpViper650::vpToolType tool,
                  vpCameraParameters::vpCameraParametersProjType proj_model)
{
  
  this->projModel = proj_model;
  
#ifdef VISP_HAVE_ACCESS_TO_NAS
  // Read the robot parameters from files
  char filename_eMc [FILENAME_MAX];
  switch (tool) {
  case vpViper650::TOOL_MARLIN_F033C_CAMERA: {
    switch(projModel) {
    case vpCameraParameters::perspectiveProjWithoutDistortion :
#if !defined(_WIN32) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))) // UNIX
      snprintf(filename_eMc, FILENAME_MAX, "%s",
               CONST_EMC_MARLIN_F033C_WITHOUT_DISTORTION_FILENAME);
#else // _WIN32
      _snprintf(filename_eMc, FILENAME_MAX, "%s",
                CONST_EMC_MARLIN_F033C_WITHOUT_DISTORTION_FILENAME);
#endif
      break;
    case vpCameraParameters::perspectiveProjWithDistortion :
#if !defined(_WIN32) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))) // UNIX
      snprintf(filename_eMc, FILENAME_MAX, "%s",
               CONST_EMC_MARLIN_F033C_WITH_DISTORTION_FILENAME);
#else // _WIN32
      _snprintf(filename_eMc, FILENAME_MAX, "%s",
                CONST_EMC_MARLIN_F033C_WITH_DISTORTION_FILENAME);
#endif
      break;
    }
    break;
  }
  case vpViper650::TOOL_PTGREY_FLEA2_CAMERA: {
    switch(projModel) {
    case vpCameraParameters::perspectiveProjWithoutDistortion :
#if !defined(_WIN32) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))) // UNIX
      snprintf(filename_eMc, FILENAME_MAX, "%s",
               CONST_EMC_PTGREY_FLEA2_WITHOUT_DISTORTION_FILENAME);
#else // _WIN32
      _snprintf(filename_eMc, FILENAME_MAX, "%s",
                CONST_EMC_PTGREY_FLEA2_WITHOUT_DISTORTION_FILENAME);
#endif
      break;
    case vpCameraParameters::perspectiveProjWithDistortion :
#if !defined(_WIN32) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))) // UNIX
      snprintf(filename_eMc, FILENAME_MAX, "%s",
               CONST_EMC_PTGREY_FLEA2_WITH_DISTORTION_FILENAME);
#else // _WIN32
      _snprintf(filename_eMc, FILENAME_MAX, "%s",
                CONST_EMC_PTGREY_FLEA2_WITH_DISTORTION_FILENAME);
#endif
      break;
    }
    break;
  }
  case vpViper650::TOOL_SCHUNK_GRIPPER_CAMERA: {
    switch(projModel) {
    case vpCameraParameters::perspectiveProjWithoutDistortion :
#if !defined(_WIN32) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))) // UNIX
      snprintf(filename_eMc, FILENAME_MAX, "%s",
               CONST_EMC_SCHUNK_GRIPPER_WITHOUT_DISTORTION_FILENAME);
#else // _WIN32
      _snprintf(filename_eMc, FILENAME_MAX, "%s",
                CONST_EMC_SCHUNK_GRIPPER_WITHOUT_DISTORTION_FILENAME);
#endif
      break;
    case vpCameraParameters::perspectiveProjWithDistortion :
#if !defined(_WIN32) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))) // UNIX
      snprintf(filename_eMc, FILENAME_MAX, "%s",
               CONST_EMC_SCHUNK_GRIPPER_WITH_DISTORTION_FILENAME);
#else // _WIN32
      _snprintf(filename_eMc, FILENAME_MAX, "%s",
                CONST_EMC_SCHUNK_GRIPPER_WITH_DISTORTION_FILENAME);
#endif
      break;
    }
    break;
  }
  case vpViper650::TOOL_GENERIC_CAMERA: {
    switch(projModel) {
    case vpCameraParameters::perspectiveProjWithoutDistortion :
#if !defined(_WIN32) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))) // UNIX
      snprintf(filename_eMc, FILENAME_MAX, "%s",
               CONST_EMC_GENERIC_WITHOUT_DISTORTION_FILENAME);
#else // _WIN32
      _snprintf(filename_eMc, FILENAME_MAX, "%s",
                CONST_EMC_GENERIC_WITHOUT_DISTORTION_FILENAME);
#endif
      break;
    case vpCameraParameters::perspectiveProjWithDistortion :
#if !defined(_WIN32) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))) // UNIX
      snprintf(filename_eMc, FILENAME_MAX, "%s",
               CONST_EMC_GENERIC_WITH_DISTORTION_FILENAME);
#else // _WIN32
      _snprintf(filename_eMc, FILENAME_MAX, "%s",
                CONST_EMC_GENERIC_WITH_DISTORTION_FILENAME);
#endif
      break;
    }
    break;
  }
  case vpViper650::TOOL_CUSTOM: {
    vpERROR_TRACE ("No predefined file available for a custom tool"
                   "You should use init(vpViper650::vpToolType, const std::string&) or"
                   "init(vpViper650::vpToolType, const vpHomogeneousMatrix&) instead");
    throw vpRobotException (vpRobotException::badValue, "No predefined file available for a custom tool");
    break;
  }
  default: {
    vpERROR_TRACE ("This error should not occur!");
    //       vpERROR_TRACE ("Si elle survient malgre tout, c'est sans doute "
    // 		   "que les specs de la classe ont ete modifiee, "
    // 		   "et que le code n'a pas ete mis a jour "
    // 		   "correctement.");
    //       vpERROR_TRACE ("Verifiez les valeurs possibles du type "
    // 		   "vpViper650::vpViper650ToolType, et controlez que "
    // 		   "tous les cas ont ete pris en compte dans la "
    // 		   "fonction init(camera).");
    break;
  }
  }
  
  this->init (filename_eMc);

#else // VISP_HAVE_ACCESS_TO_NAS

  // Use here default values of the robot constant parameters.
  switch (tool) {
  case vpViper650::TOOL_MARLIN_F033C_CAMERA: {
    switch(projModel) {
    case vpCameraParameters::perspectiveProjWithoutDistortion :
      erc[0] = vpMath::rad(0.07); // rx
      erc[1] = vpMath::rad(2.76); // ry
      erc[2] = vpMath::rad(-91.50); // rz
      etc[0] = -0.0453; // tx
      etc[1] =  0.0005; // ty
      etc[2] =  0.0728; // tz
      break;
    case vpCameraParameters::perspectiveProjWithDistortion :
      erc[0] = vpMath::rad(0.26); // rx
      erc[1] = vpMath::rad(2.12); // ry
      erc[2] = vpMath::rad(-91.31); // rz
      etc[0] = -0.0444; // tx
      etc[1] = -0.0005; // ty
      etc[2] =  0.1022; // tz
      break;
    }
    break;
  }
  case vpViper650::TOOL_PTGREY_FLEA2_CAMERA:
  case vpViper650::TOOL_SCHUNK_GRIPPER_CAMERA:  {
    switch(projModel) {
    case vpCameraParameters::perspectiveProjWithoutDistortion :
      erc[0] = vpMath::rad(0.15); // rx
      erc[1] = vpMath::rad(1.28); // ry
      erc[2] = vpMath::rad(-90.8); // rz
      etc[0] = -0.0456; // tx
      etc[1] = -0.0013; // ty
      etc[2] =  0.001; // tz
      break;
    case vpCameraParameters::perspectiveProjWithDistortion :
      erc[0] = vpMath::rad(0.72); // rx
      erc[1] = vpMath::rad(2.10); // ry
      erc[2] = vpMath::rad(-90.5); // rz
      etc[0] = -0.0444; // tx
      etc[1] = -0.0012; // ty
      etc[2] =  0.078; // tz
      break;
    }
    break;
  }
  case vpViper650::TOOL_GENERIC_CAMERA: {
    // Set eMc to identity
    switch(projModel) {
    case vpCameraParameters::perspectiveProjWithoutDistortion :
    case vpCameraParameters::perspectiveProjWithDistortion :
      erc[0] = 0; // rx
      erc[1] = 0; // ry
      erc[2] = 0; // rz
      etc[0] = 0; // tx
      etc[1] = 0; // ty
      etc[2] = 0; // tz
      break;
    }
    break;
  }
  case vpViper650::TOOL_CUSTOM: {
    vpERROR_TRACE ("No predefined parameters available for a custom tool"
                   "You should use init(vpViper650::vpToolType, const std::string&) or"
                   "init(vpViper650::vpToolType, const vpHomogeneousMatrix&) instead");
    throw vpRobotException (vpRobotException::badValue, "No predefined parameters available for a custom tool");
    break;
  }
  }
  vpRotationMatrix eRc(erc);
  this->eMc.buildFrom(etc, eRc);
#endif // VISP_HAVE_ACCESS_TO_NAS

  setToolType(tool);
  return ;
}

/*!

  Set the type of tool attached to the robot and transformation
  between the end-effector and the tool (\f$^e{\bf M}c\f$).
  This last parameter is loaded from a file.

  \param tool : Type of tool in use.

  \param filename : Path of the configuration file containing the
  transformation between the end-effector frame and the tool frame.

  The configuration file should have the form below:

  \code
# Start with any number of consecutive lines
# beginning with the symbol '#'
#
# The 3 following lines contain the name of the camera,
# the rotation parameters of the geometric transformation
# using the Euler angles in degrees with convention XYZ and
# the translation parameters expressed in meters
CAMERA CameraName
eMc_ROT_XYZ 10.0 -90.0 20.0
eMc_TRANS_XYZ  0.05 0.01 0.06
    \endcode

  \sa init(vpViper650::vpToolType,
  vpCameraParameters::vpCameraParametersProjType),
  init(vpViper650::vpToolType, const vpHomogeneousMatrix&)
*/
void
vpViper650::init (vpViper650::vpToolType tool,
                  const std::string &filename)
{
    this->setToolType(tool);
    this->parseConfigFile(filename.c_str());
}

/*!

  Set the type of tool attached to the robot and the transformation
  between the end-effector and the tool (\f$^e{\bf M}c\f$).

  \param tool : Type of tool in use.

  \param eMc_ : Homogeneous matrix representation of the transformation
  between the end-effector frame and the tool frame.

  \sa init(vpViper650::vpToolType,
  vpCameraParameters::vpCameraParametersProjType),
  init(vpViper650::vpToolType, const std::string&)

*/
void
vpViper650::init (vpViper650::vpToolType tool,
                  const vpHomogeneousMatrix &eMc_)
{
    this->setToolType(tool);
    this->set_eMc(eMc_);
}

/*!

  This function gets the robot constant parameters from a file.

  \param filename : File name containing the robot constant
  parameters, like the hand-to-eye transformation.

*/
void
vpViper650::parseConfigFile (const char * filename)
{
  size_t            dim;
  int               code;
  char              Ligne[FILENAME_MAX];
  char              namoption[100];
  FILE *            fdtask;
  int               numLn = 0;
  vpRxyzVector rot_eMc; // rotation
  vpTranslationVector trans_eMc; // translation
  bool get_rot_eMc = false;
  bool get_trans_eMc = false;

  //vpTRACE("Read the config file for constant parameters %s.", filename);
  if ((fdtask = fopen(filename, "r" )) == NULL)
  {
    vpERROR_TRACE ("Impossible to read the config file %s.",
                   filename);
    throw vpRobotException (vpRobotException::readingParametersError,
                            "Impossible to read the config file.");
  }

  while (fgets(Ligne, FILENAME_MAX, fdtask) != NULL) {
    numLn ++;
    if ('#' == Ligne[0]) { continue; }
    if (sscanf(Ligne, "%s", namoption) != 1) {
      fclose (fdtask);
      throw(vpException(vpException::badValue,
                        "Cannot parse configuration file %s to retrieve option name"));
    }

    dim = strlen(namoption);

    for (code = 0;
         NULL != opt_viper650[code];
         ++ code)
    {
      if (strncmp(opt_viper650[code], namoption, dim) == 0)
      {
        break;
      }
    }

    switch(code) {
    case 0:
      break; // Nothing to do: camera name

    case 1:
      if (sscanf(Ligne, "%s %lf %lf %lf", namoption,
                 &rot_eMc[0], &rot_eMc[1], &rot_eMc[2]) != 4) {
        fclose (fdtask);
        throw(vpException(vpException::badValue,
                          "Cannot parse configuration file %s to retrieve translation"));
      }

      // Convert rotation from degrees to radians
      rot_eMc[0] *= M_PI / 180.0;
      rot_eMc[1] *= M_PI / 180.0;
      rot_eMc[2] *= M_PI / 180.0;
      get_rot_eMc = true;
      break;

    case 2:
      if (sscanf(Ligne, "%s %lf %lf %lf", namoption,
                 &trans_eMc[0], &trans_eMc[1], &trans_eMc[2]) != 4) {
        fclose (fdtask);
        throw(vpException(vpException::badValue,
                          "Cannot parse configuration file %s to retrieve rotation"));
      }
      get_trans_eMc = true;
      break;

    default:
      vpERROR_TRACE ("Bad configuration file %s  "
                     "ligne #%d.", filename, numLn);
    } /* SWITCH */
  } /* WHILE */

  fclose (fdtask);

  // Compute the eMc matrix from the translations and rotations
  if (get_rot_eMc && get_trans_eMc) {
    this->set_eMc(trans_eMc,rot_eMc);
  }
  else {
    vpERROR_TRACE ("Could not read translation and rotation parameters from config file %s.",
                     filename);
    throw vpRobotException (vpRobotException::readingParametersError,
                              "Could not read translation and rotation parameters from config file ");
  }

  return;
}


/*!
  Get the current intrinsic camera parameters obtained by calibration.

  \warning This method needs XML library to parse the file defined in
  vpViper650::CONST_CAMERA_FILENAME and containing the camera
  parameters. If XML is detected by ViSP, VISP_HAVE_XML2 macro is
  defined in include/visp3/core/vpConfig.h file.

  \warning Thid method needs also an access to the file located on
  Inria's NAS server and containing the camera parameters in XML
  format. This access is available if VISP_HAVE_ACCESS_TO_NAS macro is
  defined in include/visp3/core/vpConfig.h file.

  - If VISP_HAVE_ACCESS_TO_NAS and VISP_HAVE_XML2 macros are defined,
  this method gets the camera parameters from
  /udd/fspindle/robot/Viper650/current/include/const_camera_Viper650.xml
  config file.

  - If these two macros are not defined, this method set the camera parameters
  to default one.

  \param cam : In output, camera parameters to fill.
  \param image_width : Image width used to compute camera calibration.
  \param image_height : Image height used to compute camera calibration.

  The code below shows how to get the camera parameters of the camera
  attached to the robot.

  \code
#include <visp3/core/vpConfig.h>
#include <visp3/core/vpImage.h>
#include <visp3/sensor/vp1394TwoGrabber.h>
#include <visp3/robot/vpViper650.h>
#include <visp3/robot/vpRobotViper650.h>

int main()
{
  vpImage<unsigned char> I(480, 640);

#ifdef VISP_HAVE_DC1394
  vp1394TwoGrabber g;

  // Acquire an image to update image structure
  g.acquire(I) ;
#endif

#ifdef VISP_HAVE_VIPER650
  vpRobotViper650 robot;
#else
  vpViper650 robot;
#endif

  vpCameraParameters cam ;
  // Get the intrinsic camera parameters depending on the image size
  // Camera parameters are read from
  // /udd/fspindle/robot/Viper650/current/include/const_camera_Viper650.xml
  // if VISP_HAVE_ACCESS_TO_NAS and VISP_HAVE_XML2 macros are defined
  // in vpConfig.h file
  try {
    robot.getCameraParameters (cam, I.getWidth(), I.getHeight());
  }
  catch(...) {
    std::cout << "Cannot get camera parameters for image: " << I.getWidth() << " x " << I.getHeight() << std::endl;
  }
  std::cout << "Camera parameters: " << cam << std::endl;
}
  \endcode

  \exception vpRobotException::readingParametersError : If the camera parameters are not found.

*/

void
vpViper650::getCameraParameters (vpCameraParameters &cam,
                                 const unsigned int &image_width,
                                 const unsigned int &image_height) const
{
#if defined(VISP_HAVE_XML2) && defined (VISP_HAVE_ACCESS_TO_NAS)
  vpXmlParserCamera parser;
  switch (getToolType()) {
  case vpViper650::TOOL_MARLIN_F033C_CAMERA: {
    std::cout << "Get camera parameters for camera \""
              << vpViper650::CONST_MARLIN_F033C_CAMERA_NAME << "\"" << std::endl
              << "from the XML file: \""
              << vpViper650::CONST_CAMERA_FILENAME << "\""<< std::endl;
    if (parser.parse(cam,
                     vpViper650::CONST_CAMERA_FILENAME,
                     vpViper650::CONST_MARLIN_F033C_CAMERA_NAME,
                     projModel,
                     image_width, image_height) != vpXmlParserCamera::SEQUENCE_OK) {
      throw vpRobotException (vpRobotException::readingParametersError,
                              "Impossible to read the camera parameters.");
    }
    break;
  }
  case vpViper650::TOOL_PTGREY_FLEA2_CAMERA: {
    std::cout << "Get camera parameters for camera \""
              << vpViper650::CONST_PTGREY_FLEA2_CAMERA_NAME << "\"" << std::endl
              << "from the XML file: \""
              << vpViper650::CONST_CAMERA_FILENAME << "\""<< std::endl;
    if (parser.parse(cam,
                     vpViper650::CONST_CAMERA_FILENAME,
                     vpViper650::CONST_PTGREY_FLEA2_CAMERA_NAME,
                     projModel,
                     image_width, image_height) != vpXmlParserCamera::SEQUENCE_OK) {
      throw vpRobotException (vpRobotException::readingParametersError,
                              "Impossible to read the camera parameters.");
    }
    break;
  }
  case vpViper650::TOOL_SCHUNK_GRIPPER_CAMERA: {
    std::cout << "Get camera parameters for camera \""
              << vpViper650::CONST_SCHUNK_GRIPPER_CAMERA_NAME << "\"" << std::endl
              << "from the XML file: \""
              << vpViper650::CONST_CAMERA_FILENAME << "\""<< std::endl;
    if (parser.parse(cam,
                     vpViper650::CONST_CAMERA_FILENAME,
                     vpViper650::CONST_SCHUNK_GRIPPER_CAMERA_NAME,
                     projModel,
                     image_width, image_height) != vpXmlParserCamera::SEQUENCE_OK) {
      throw vpRobotException (vpRobotException::readingParametersError,
                              "Impossible to read the camera parameters.");
    }
    break;
  }
  case vpViper650::TOOL_GENERIC_CAMERA: {
    std::cout << "Get camera parameters for camera \""
              << vpViper650::CONST_GENERIC_CAMERA_NAME << "\"" << std::endl
              << "from the XML file: \""
              << vpViper650::CONST_CAMERA_FILENAME << "\""<< std::endl;
    if (parser.parse(cam,
                     vpViper650::CONST_CAMERA_FILENAME,
                     vpViper650::CONST_GENERIC_CAMERA_NAME,
                     projModel,
                     image_width, image_height) != vpXmlParserCamera::SEQUENCE_OK) {
      throw vpRobotException (vpRobotException::readingParametersError,
                              "Impossible to read the camera parameters.");
    }
    break;
  }
  case vpViper650::TOOL_CUSTOM: {
    vpERROR_TRACE ("No intrinsic parameters available for a custom tool");
    throw vpRobotException (vpRobotException::badValue, "No intrinsic parameters available for a custom tool");
    break;
  }
  default: {
    vpERROR_TRACE ("This error should not occur!");
    //       vpERROR_TRACE ("Si elle survient malgre tout, c'est sans doute "
    //        "que les specs de la classe ont ete modifiee, "
    //        "et que le code n'a pas ete mis a jour "
    //        "correctement.");
    //       vpERROR_TRACE ("Verifiez les valeurs possibles du type "
    //        "vpViper650::vpViper650ToolType, et controlez que "
    //        "tous les cas ont ete pris en compte dans la "
    //        "fonction init(camera).");
    throw vpRobotException (vpRobotException::readingParametersError,
                            "Impossible to read the camera parameters.");
    break;
  }
  }
#else
  // Set default parameters
  switch (getToolType()) {
  case vpViper650::TOOL_MARLIN_F033C_CAMERA: {
    // Set default intrinsic camera parameters for 640x480 images
    if (image_width == 640 && image_height == 480) {
      std::cout << "Get default camera parameters for camera \""
                << vpViper650::CONST_MARLIN_F033C_CAMERA_NAME << "\"" << std::endl;
      switch(this->projModel) {
      case vpCameraParameters::perspectiveProjWithoutDistortion :
        cam.initPersProjWithoutDistortion(1232.0, 1233.0, 317.7, 253.9);
        break;
      case vpCameraParameters::perspectiveProjWithDistortion :
        cam.initPersProjWithDistortion(1214.0, 1213.0, 323.1, 240.0, -0.1824, 0.1881);
        break;
      }
    }
    else {
      vpTRACE("Cannot get default intrinsic camera parameters for this image resolution");
      throw vpRobotException (vpRobotException::readingParametersError,
                              "Impossible to read the camera parameters.");
    }
    break;
  }
  case vpViper650::TOOL_PTGREY_FLEA2_CAMERA:
  case vpViper650::TOOL_SCHUNK_GRIPPER_CAMERA: {
    // Set default intrinsic camera parameters for 640x480 images
    if (image_width == 640 && image_height == 480) {
      std::cout << "Get default camera parameters for camera \""
                << vpViper650::CONST_PTGREY_FLEA2_CAMERA_NAME << "\"" << std::endl;
      switch(this->projModel) {
      case vpCameraParameters::perspectiveProjWithoutDistortion :
        cam.initPersProjWithoutDistortion(868.0, 869.0, 314.8, 254.1);
        break;
      case vpCameraParameters::perspectiveProjWithDistortion :
        cam.initPersProjWithDistortion(831.3, 831.6, 322.7, 265.8, -0.1955, 0.2047);
        break;
      }
    }
    else {
      vpTRACE("Cannot get default intrinsic camera parameters for this image resolution");
      throw vpRobotException (vpRobotException::readingParametersError,
                              "Impossible to read the camera parameters.");
    }
    break;
  }
  case vpViper650::TOOL_GENERIC_CAMERA: {
    // Set default intrinsic camera parameters for 640x480 images
    if (image_width == 640 && image_height == 480) {
      std::cout << "Get default camera parameters for camera \""
                << vpViper650::CONST_GENERIC_CAMERA_NAME << "\"" << std::endl;
      switch(this->projModel) {
      case vpCameraParameters::perspectiveProjWithoutDistortion :
        cam.initPersProjWithoutDistortion(868.0, 869.0, 314.8, 254.1);
        break;
      case vpCameraParameters::perspectiveProjWithDistortion :
        cam.initPersProjWithDistortion(831.3, 831.6, 322.7, 265.8, -0.1955, 0.2047);
        break;
      }
    }
    else {
      vpTRACE("Cannot get default intrinsic camera parameters for this image resolution");
      throw vpRobotException (vpRobotException::readingParametersError,
                              "Impossible to read the camera parameters.");
    }
    break;
  }
  case vpViper650::TOOL_CUSTOM: {
    vpERROR_TRACE ("No intrinsic parameters available for a custom tool");
    throw vpRobotException (vpRobotException::badValue, "No intrinsic parameters available for a custom tool");
    break;
  }
  default:
    vpERROR_TRACE ("This error should not occur!");
    throw vpRobotException (vpRobotException::readingParametersError,
                            "Impossible to read the camera parameters.");
    break;
  }
#endif
  return;
}

/*!
  Get the current intrinsic camera parameters obtained by calibration.

  \warning This method needs XML library to parse the file defined in
  vpViper650::CONST_CAMERA_FILENAME and containing the camera
  parameters. If XML is detected by ViSP, VISP_HAVE_XML2 macro is
  defined in include/visp3/core/vpConfig.h file.

  \warning Thid method needs also an access to the file located on
  Inria's NAS server and containing the camera parameters in XML
  format. This access is available if VISP_HAVE_ACCESS_TO_NAS macro is
  defined in include/visp3/core/vpConfig.h file.

  - If VISP_HAVE_ACCESS_TO_NAS and VISP_HAVE_XML2 macros are defined,
  this method gets the camera parameters from
  /udd/fspindle/robot/Viper650/current/include/const_camera_Viper650.xml
  config file.

  - If these two macros are not defined, this method set the camera parameters
  to default one.

  \param cam : In output, camera parameters to fill.
  \param I : A B&W image send by the current camera in use.

  \code
#include <visp3/core/vpConfig.h>
#include <visp3/core/vpImage.h>
#include <visp3/sensor/vp1394TwoGrabber.h>
#include <visp3/robot/vpViper650.h>
#include <visp3/robot/vpRobotViper650.h>

int main()
{
  vpImage<unsigned char> I(480, 640);

#ifdef VISP_HAVE_DC1394
  vp1394TwoGrabber g;

  // Acquire an image to update image structure
  g.acquire(I) ;
#endif

#ifdef VISP_HAVE_VIPER650
  vpRobotViper650 robot;
#else
  vpViper650 robot;
#endif

  vpCameraParameters cam ;
  // Get the intrinsic camera parameters depending on the image size
  try {
    robot.getCameraParameters (cam, I);
  }
  catch(...) {
    std::cout << "Cannot get camera parameters for image: " << I.getWidth() << " x " << I.getHeight() << std::endl;
  }
  std::cout << "Camera parameters: " << cam << std::endl;
}
  \endcode

  \exception vpRobotException::readingParametersError : If the camera parameters are not found.

*/
void
vpViper650::getCameraParameters (vpCameraParameters &cam,
                                 const vpImage<unsigned char> &I) const
{
  getCameraParameters(cam,I.getWidth(),I.getHeight());
}
/*!
  \brief Get the current intrinsic camera parameters obtained by calibration.

    \warning This method needs XML library to parse the file defined in
  vpViper650::CONST_CAMERA_FILENAME and containing the camera
  parameters. If XML is detected by ViSP, VISP_HAVE_XML2 macro is
  defined in include/visp3/core/vpConfig.h file.

  \warning Thid method needs also an access to the file located on
  Inria's NAS server and containing the camera parameters in XML
  format. This access is available if VISP_HAVE_ACCESS_TO_NAS macro is
  defined in include/visp3/core/vpConfig.h file.

  - If VISP_HAVE_ACCESS_TO_NAS and VISP_HAVE_XML2 macros are defined,
  this method gets the camera parameters from
  /udd/fspindle/robot/Viper650/current/include/const_camera_Viper650.xml
  config file.

  - If these two macros are not defined, this method set the camera parameters
  to default one.

  \param cam : In output, camera parameters to fill.
  \param I : A color image send by the current camera in use.

  \code
#include <visp3/core/vpConfig.h>
#include <visp3/core/vpImage.h>
#include <visp3/sensor/vp1394TwoGrabber.h>
#include <visp3/robot/vpViper650.h>
#include <visp3/robot/vpRobotViper650.h>

int main()
{
  vpImage<vpRGBa> I(480, 640);

#ifdef VISP_HAVE_DC1394
  vp1394TwoGrabber g;

  // Acquire an image to update image structure
  g.acquire(I) ;
#endif

#ifdef VISP_HAVE_VIPER650
  vpRobotViper650 robot;
#else
  vpViper650 robot;
#endif

  vpCameraParameters cam ;
  // Get the intrinsic camera parameters depending on the image size
  try {
    robot.getCameraParameters (cam, I);
  }
  catch(...) {
    std::cout << "Cannot get camera parameters for image: " << I.getWidth() << " x " << I.getHeight() << std::endl;
  }
  std::cout << "Camera parameters: " << cam << std::endl;
}
  \endcode

  \exception vpRobotException::readingParametersError : If the camera parameters are not found.

*/

void
vpViper650::getCameraParameters (vpCameraParameters &cam,
                                 const vpImage<vpRGBa> &I) const
{
  getCameraParameters(cam,I.getWidth(),I.getHeight());
}
