#include <iostream>
#include "icp.h"
#include "server.hh"

//-----------------------------------------------------------------------------

Controller_impl * Thermometer_impl::m_ctrl; //static member

// Helper function to read the model string from a device
CCS::ModelType Thermometer_impl::getModel()
{
   char buf[32];
   if(ICP_get(m_anum, "model", buf, sizeof(buf))!=0)
   {
      abort();
   }
   
   return CORBA::string_dup(buf);
}

// Helper functions to read the location from a device.
CCS::TempType Thermometer_impl::getTemp
{
   char buf[32];
   if(ICP_get(m_anum, "location", buf, sizeof(buf)) !=0)
   {
      abort;
   }
   return CORBA::string_dup(buf);
}

// Helper function to set the location of a device.
void Thermometer_impl::setLoc(const char * loc)
{
   if(
}
