#include "ros/ros.h"
#include "lab_1/table.h"
#include <stdlib.h>
#include <iostream>
#include <string>

using namespace std;
using namespace ros;

int main(int argc, char **argv)
{
  init(argc, argv, "casino_publisher");

  NodeHandle n;
  ServiceClient client = n.serviceClient<lab_1::table>("table");
  lab_1::table srv;

  if (argc == 2)
  {
    srv.request.color = argv[1];
    srv.request.number = 37;
  }
  else if (argc == 3)
  {       
    srv.request.color = argv[1];
    srv.request.number = atoll(argv[2]);
  }
  
  while (ok())
  {
    if (client.call(srv))
    {      
      if(srv.response.result == "error")
        ROS_INFO("There are only green, red and black.");
      else if (srv.response.result == "win")
        ROS_INFO("OMG!!! YOU WON!!!");       
      else
      {
        string defeat_responses[3] = {"It's just not your day.", 
                                      "Try again, you'll probably get lucky.", 
                                      "Sorry, but the casino is not for you."};
        int number_defeat_response = rand() % 3;
        ROS_INFO("%s", defeat_responses[number_defeat_response].c_str());
      }
      ROS_INFO("You can play it again.");
      ROS_INFO("To finish, write exit.");

      string new_color;
      int new_number;
      cin >> new_color;
      if(new_color.find("exit") != string::npos)
        return 1;
      cin >> new_number;
      srv.request.color = new_color;
      srv.request.number = new_number;
    }
    else
    {
      ROS_ERROR("Failed!");
      return 1;
    }
  }
  return 0;
}