#include <iostream>
#include "climate.hh"

using namespace std;

//-----------------------------------------------------------------------------

// Show the details for a thermometer or thermostat.

static ostream & operator<<(ostream & os, CCS::Thermometer_ptr t)
{
   // Check for nil.
   if(CORBA::is_nil(t))
   {
      os << "Cannot show state for nil reference." << endl;
      return os;
   }
   
   //Try to narrow and print what kind of device it is.
   CCS::Thermostat_var tmstat = CCS::Thermostat::_narrow(t);
   os << (CORBA::is_nil(tmstat) ? "Thermometer:" : "Thermostat:") <<endl;
   
   // Show attribute values
   CCS::ModelType_var model = t->model();
   CCS::LocType_var location = t->location();
   os << "\tAsset number: " << t->assetNum() << endl;
   os << "\tModel       : " << model << endl;
   os << "\tLocation    : " << location << endl;
   os << "\tTemperature : " << t->temperature() << endl;
   
   // If device is a thermostat, show nominal temperature.
   if(!CORBA::is_nil(tmstat))
   {
      os << "\tNominal temp: " << tmstat->getNominal() <<endl;
   }
   return os;
}

//-----------------------------------------------------------------------------

// Show the information in BtData struct.

static ostream & operator<<(ostream & os, const CCS::Thermostat::BtData & btd)
{
   os << "CCS::Thermostat::BtData details:" << endl;
   os << "\trequested    : " <<btd.requested <<endl;
   os << "\tmin_permitted: " <<btd.minPermitted << endl;
   os << "\tmax_permitted: " <<btd.maxPermitted << endl;
   os << "\terror_msg    : " <<btd.errorMsg << endl;
   return os;
}

//-----------------------------------------------------------------------------

// Loop over the sequence of records in an EChange exception and show the
// details of each record.

static ostream & operator<<(ostream & os, const CCS::Controller::EChange & ec)
{
   for(CORBA::ULong i = 0; i < ec.errors.length(); i++)
   {
      os << "Change failed:" << endl;
      os << ec.errors[i].tmstatRef;   // Overloaded <<
      os << ec.errors[i].info <<endl;  // Overloaded <<
   }
   
   return os;
}

//-----------------------------------------------------------------------------

// Generic ostream inserter for exceptions. Inserts the exception name, if 
// available, and the repository ID otherwise.

static ostream & operator<<(ostream & os, const CORBA::Exception & e)
{
   CORBA::Any tmp;
   tmp<<=e;
   CORBA::TypeCode_var tc = tmp.type();
   const char * p = tc->name();
   if(*p != '\0')
   {
      os << p;
   }
   else
   {
      os << tc->id();
   }
   
   return os;
}

//-----------------------------------------------------------------------------

// Change the temperature of a thermostat.

static void set_temp(CCS::Thermostat_ptr tmstat, CCS::TempType new_temp)
{
   if(CORBA::is_nil(tmstat))  // Don't call via nil reference
   {
      return;
   }
   
   CCS::AssetType anum = tmstat->assetNum();
   try
   {
      cout << " Setting thermostat " << anum
           << " to " << new_temp << " degrees." <<endl;
      CCS::TempType old_nominal = tmstat->setNominal(new_temp);
      cout << "Old nominal temperature was: "
           << old_nominal << endl;
      cout << "New nominal temperature is : "
           << tmstat->getNominal() << endl;
   }
   catch(const CCS::Thermostat::BadTemp & bt)
   {
      cerr << "Setting of nominal temperature failed." << endl;
      cerr << bt.details << endl;             // Overloaded <<
   }
}

//-----------------------------------------------------------------------------

int main(int argc, char * argv [])
{
   try
   {
      // Intialize the orb
      CORBA::ORB_var orb = CORBA::ORB_init(argc,argv);
      
      // Check Arguments
      if(argc != 2)
      {
         cerr << "Usage: client IOR_string" << endl;
         throw 0;
      }
      
      // Get controller reference from argv and convert to object
      CORBA::Object_var obj = orb->string_to_object(argv[1]);
      if(CORBA::is_nil(obj))
      {
         cerr << "Nil controller reference" << endl;
         throw 0;
      }
      
      // Try to narrow to CCS::Controller.
      CCS::Controller_var ctrl;
      try
      {
         ctrl = CCS::Controller::_narrow(obj);
      }
      catch(const CORBA::SystemException & se)
      {
         cerr << "Cannot narrow controller reference: "
              << se << endl;
         throw 0;
      }
      if(CORBA::is_nil(ctrl))
      {
         cerr << "Wrong type for controller ref." <<endl;
         throw 0;
      }
      
      // Get list of devices
      CCS::Controller::ThermometerSeq_var list = ctrl->list();
      
      // Show number of devices.
      CORBA::ULong len = list->length();
      cout << "Controller has " <<len << " device";
      if(len !=1)
      {
         cout << "s";
      }
      cout << "." << endl;
      
      // If there are no devices at all, we are finished
      if(len == 0)
      {
         return 0;     
      }
      
      // Show details for each device
      for(CORBA::ULong i = 0; i < list->length(); i++)
      {
         cout << list[i];
      }
      cout << endl;
      
      // Change the location of first device in the list
      CCS::AssetType anum = list[0]->assetNum();
      cout << "Changing location of device " 
           << anum << "." <<endl;
      list[0]->location("Earth");
      // Check that the location was updated
      cout << "New details for device "
           << anum << " are:" <<endl;
      cout << list[0] << endl;
      
      // Find first thermostat in list.
      CCS::Thermostat_var tmstat;
      for(CORBA::ULong i = 0; i < list->length() && CORBA::is_nil(tmstat);i++)
      {
         tmstat = CCS::Thermostat::_narrow(list[i]);
      }
      
      
      // Check that we found a thermostat on the list.
      if(CORBA::is_nil(tmstat))
      {
         cout << "No thermostat devices in list." <<endl;
      }
      else
      {
         // Set temperature of thermostat to 50 degrees (should work).
         set_temp(tmstat,50);
         cout << endl;
         
         //Set temperature of thermostat to -10 degrees (should fail).
         set_temp(tmstat, -10);
         cout <<endl;
      }
      
      // Look for devices in the rooms Earth and Hal. This must find at least 
      // one device because we earlier changed the location of the first device
      // to room Earth.
      cout << "Looking for devices in Earth and Hal." << endl;
      CCS::Controller::SearchSeq ss;
      ss.length(2);
      ss[0].key.loc(CORBA::string_dup("Earth"));
      ss[1].key.loc(CORBA::string_dup("Hal"));
      ctrl->find(ss);
      
      // Show the devices found in that room
      for(CORBA::ULong i=0; i<ss.length();i++)
      {
         cout << ss[i].device;            // overloaded <<
      } 
      cout << endl;
      
      // Increase the temperature of all thermostats by 40 degrees. First, make
      // a list (tss) containing only a thermostats.
      cout << "Increasing the temperature by 40 degrees." <<endl;
      CCS::Controller::ThermostatSeq tss;
      for(CORBA::ULong i=0; i<list->length();i++)
      {
         tmstat = CCS::Thermostat::_narrow(list[i]);
         if(CORBA::is_nil(tmstat))
         {
            continue;
         }
         
         len = tss.length();
         tss.length(len+1);
         tss[len] = tmstat;
      }
      
      // Try to change all thermostats.
      try
      {
         ctrl->change(tss,40);
      }
      catch(const CCS::Controller::EChange & ec)
      {
         cerr << ec;         // overloaded <<
      }
   }
   catch(const CORBA::Exception & e)
   {
      cerr << "Uncaught CORBA exception: " << e << endl;
   }
   catch(...)
   {
      return 1;  
   }
   
   return 0;
}
