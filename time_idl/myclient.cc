#include <iomanip>
#include <iostream>
#include "time.hh"

using namespace std;

int main (int argc, char * argv[])
{
   try
   {
      // Initialize orb
      CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

      // Check arguments
      if(argc !=2)
      {
         std::cerr << "Usage: client IOR_string" <<endl;
         throw 0;
      }

      // Destringfy argv[1]
      CORBA::Object_var obj = orb->string_to_object(argv[1]);
      if(CORBA::is_nil(obj))
      {
        std::cerr << "Nil Time reference" << endl;
        throw 0;
      }

      // Narrow
      Time_var tm = Time::_narrow(obj);
      if(CORBA::is_nil(tm))
      {
         std::cerr << "Argument is not a time reference" <<endl;
         throw 0;
      }

      // Get time of day
      TimeOfDay tod = tm->get_gmt();
      cout << "Time in Greenwhich is "
           << setw(2) << setfill('0') << tod.hour << ":"
           << setw(2) << setfill('0') << tod.minute << ":"
           << setw(2) << setfill('0') << tod.second << endl;
   }
   catch(const CORBA::Exception &)
   {
      std::cerr << "Uncaught CORBA exception" <<endl;
      return 1;
   }
   catch(...)
   {
      return 1;
   }

   return 0;
}
