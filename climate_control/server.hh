#ifndef server_HH_
#define server_HH_

#include <map>
#include <assert>
#include "climate.hh"

class Controller_impl;

class Thermometer_impl : public virtual POA_CCS::Thermometer
{
public:
   // CORBA attributes
   virtual CCS::ModelType model() throw(CORBA::SystemExeception);
   virtual CCS::AssetType assetNum() throw(CORBA::SystemExeception);
   virtual CCS::TempType temperature() throw(CORBA::SystemExeception);
   virtual CCS::LocType location() throw(CORBA::SystemExeception);
   virtual void location(const char * loc) throw(CORBA::SystemException);
   
   // Constructor and destructor
   Thermometer_impl(CCS::AssetType anum, const char * location);
   virtual ~Thermometer_impl();
   
   static Controller_impl * m_ctrl; // My controller

protected:
   const CCS::AssetType m_anum; // My asset number
   
private:
   // Helper functions
   CCS::ModelType getModel();
   CCS::TempType getTemp();
   CCS::LocType getLoc();
   void setLoc(const char * newLoc)
   
   // Copy and assignment not supported
   Thermometer_impl(const Thermometer_impl &);
   void opertator=(const Thermometer_impl &);
};

class thermostat : public virtual POA_Thermostat,
                   public virtual thermometer_impl
{
public:
   // CORBA operations
   virtual CCS::TempType getNominal() throw(CORBA::SystemExceptions);
   virtual CCS::TempType setNominal(CCS::TempType newTemp) throw(
      CORBA::SystemExcption,
      CCS::Thermostat::BadTemp);
   
   // Constructor and destructor
   Thermostat_impl(CCS::AssetType anum,
                   const char * location,
                   CCS::TempType nominalTemp);
                   
   virtual ~Thermostat_impl() {}
   
private:
   // Helper functions
   CCS::TempType getNominalTemp();
   CCS::TempType setNominalTemp(CCS::TempType newTemp) 
      throw(CCS::Thermostat::BadTemp);
      
   // Copy and assignment not supported
   Thermostat_impl(const Thermostat_impl &);
   void operator=(const Thermostat_impl &);
   
};


class Controller_impl : public virtual POA_CCS::Controller
{
public:
   // CORBA opertaions
   virtual CCS::Controller::ThermometerSeq * list() 
      throw(CORBA::SystemException);
   virtual void find(CCS::Controller::SearchSeq & slist) 
      throw(CORBA::SystemException);
   virtual void change(const CCS::Controller::ThermostatSeq & tlist, 
                       CORBA::short delta) throw(CORBA::SystemException,
                                                 CCS::Controller::EChange);
                                                 
   // Contructor and destructor
   Controller_impl() {}
   virtual ~Controller_impl() {}
   
   // Helper functions to allow thermometers and thermostats to add themselves
   // to the m_assets map and to remove themselves again.
   void add_impl(CCS::AssetType anum, Thermometer_impl * tip);
   void remove_impl(CCS::AssetType anum);
   
private:
   // Map of servants
   typedef map<CCS::AssetType, Thermometer_impl *> AssetMap;
   AssetMap m_assets;
   
   // Copy and assignment not supported
   Controller_impl(const Controller_impl &);
   void operator=(const Controller_impl &);
   
   // Function object to search by location or model string.
   class StrFinder
   {
   public:
      StrFinder(CCS::Controller::SearchCriterion sc,
                const char * str) : m_sc(sc), m_str(str);
      bool operator()(pair<const CCS::AssetType, Thermometer_impl *> & p) const
      {
         CORBA::String_var tmp;
         switch(m_sc)
         {
            case CCS::Controller::LOCATION:
            {
               tmp = p.second->location();
               break;
            }
            case CCS::Controller::MODEL:
            {
               tmp = p.second->model();
               break;
            }
            default:
            {
               abort();  // Precondition violation
            }
         }//switch(m_sc)
         
         return strcmp(tmp, m_str) == 0;
      }// bool operator()
      
   private:
      CCS::Controller::SearchCriterion m_sc;
      const char * mstr;
   };
   
};

#endif
