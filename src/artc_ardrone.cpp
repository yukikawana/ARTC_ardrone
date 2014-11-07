// -*- C++ -*-
/*!
 * @file  artc_ardrone.cpp
 * @brief ModuleDescription
 * @date $Date$
 *
 * $Id$
 */

#include "artc_ardrone.h"
#include <math.h>

// Module specification
// <rtc-template block="module_spec">
static const char* artc_ardrone_spec[] =
  {
    "implementation_id", "artc_ardrone",
    "type_name",         "artc_ardrone",
    "description",       "ModuleDescription",
    "version",           "1.0.0",
    "vendor",            "VenderName",
    "category",          "Category",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
artc_ardrone::artc_ardrone(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_controldataIn("controldata", m_controldata),
    m_navdataOut("navdata", m_navdata)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
artc_ardrone::~artc_ardrone()
{
}



RTC::ReturnCode_t artc_ardrone::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("controldata", m_controldataIn);
  
  // Set OutPort buffer
  addOutPort("navdata", m_navdataOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t artc_ardrone::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t artc_ardrone::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t artc_ardrone::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t artc_ardrone::onActivated(RTC::UniqueId ec_id)
{
flying=false;
 if (!ardrone.open()) {
        std::cout << "Failed to initialize." << std::endl;
        //return -1;
    }
  return RTC::RTC_OK;
}


RTC::ReturnCode_t artc_ardrone::onDeactivated(RTC::UniqueId ec_id)
{
if(flying){ardrone.landing();flying=false;}
  ardrone.close();
  return RTC::RTC_OK;
}


RTC::ReturnCode_t artc_ardrone::onExecute(RTC::UniqueId ec_id)
{
if(m_controldataIn.isNew()){
	m_controldataIn.read();
	if(m_controldata.onground && !flying) {ardrone.takeoff(); flying=true;}
	else if(!m_controldata.onground && flying){ardrone.landing(); flying=false;}
	 ardrone.move3D(m_controldata.vx, m_controldata.vy, m_controldata.vz, m_controldata.yaw);
	}
       cv::Mat image= ardrone.getImage();

        // Orientation
        double roll  = ardrone.getRoll();
        double pitch = ardrone.getPitch();
        double yaw   = ardrone.getYaw();

        std::cout << "ardrone.roll  = " << roll   << " [RAD]" << std::endl;
        std::cout << "ardrone.pitch = " << pitch << " [RAD]" << std::endl;
        std::cout << "ardrone.yaw   = " << yaw   << " [RAD]" << std::endl;
        

        // Altitude
        double altitude = ardrone.getAltitude();
        std::cout << "ardrone.altitude = " << altitude << " [m]" << std::endl;

        // Velocity
        double vx, vy, vz;
        double velocity = ardrone.getVelocity(&vx, &vy, &vz);
        std::cout << "ardrone.vx = " << vx << " [m/s]" << std::endl;
        std::cout << "ardrone.vy = " << vy << " [m/s]" << std::endl;
        std::cout << "ardrone.vz = " << vz << " [m/s]" << std::endl;

        // Battery
        int battery = ardrone.getBatteryPercentage();
        std::cout << "ardrone.battery = " << battery << " [%%]" << std::endl;
        m_navdata.roll=roll;
        m_navdata.pitch=pitch;
        m_navdata.yaw=yaw;
        m_navdata.altitude=altitude;
        m_navdata.vx=vx;
        m_navdata.vy=vy;
        m_navdata.vz=vz;
        m_navdata.battery=battery;
        if(battery<5) ardrone.landing();
        m_navdata.onground=ardrone.onGround();
        m_navdata.imgwidth=image.cols;
        m_navdata.imgheight=image.rows;
	int len=image.channels()*image.cols*image.rows;
        m_navdata.imgSize=len;
        std::cout << "channels " << image.channels() << " cols " << image.cols << " rows " << image.cols << std::endl;
        m_navdata.imgData.data.length(len);
        memcpy((void *)&(m_navdata.imgData.data[0]),image.data,len);
       //m_navdata.imgData.data=image.data;
        m_navdata.imgType=image.type();
        m_navdataOut.write();
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t artc_ardrone::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t artc_ardrone::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t artc_ardrone::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t artc_ardrone::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t artc_ardrone::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void artc_ardroneInit(RTC::Manager* manager)
  {
    coil::Properties profile(artc_ardrone_spec);
    manager->registerFactory(profile,
                             RTC::Create<artc_ardrone>,
                             RTC::Delete<artc_ardrone>);
  }
  
};


