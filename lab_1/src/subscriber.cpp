#include "ros/ros.h"
#include "lab_1/table.h"
#include <stdlib.h>
#include <iostream>
#include <string>

using namespace std;
using namespace ros;

bool game(lab_1::table::Request &req, 
          lab_1::table::Response &res)
{
  string player_color = req.color;
  int player_number = req.number;

  // задание случайного цвета и числа
  int true_color_number = rand() % 3;
  int true_number = rand() % 37;
  bool is_win = false;

  // определение выигрышного цвета
  string true_color;
  if (true_color_number == 0)
    true_color = "green";
  else if (true_color_number == 1)
    true_color = "red";
  else 
    true_color = "black";

  // определение победы или поражения, когда введен только цвет
  if (player_number == 37)
  {
    is_win = true_color == player_color;
  }
  
  // определение победы или поражения, когда введены цыет и число
  if (player_color == "green")
  {
    is_win = (true_number == 0) && (true_color == "green");
  }
  else if (player_color == "red")
  {
    is_win = (true_number == player_number) && (true_color == "red");
  }
  else if (player_color == "black")
  {
    is_win = (true_number == player_number) && (true_color == "black");
  }
  else
  {
    res.result = "error";
    ROS_INFO("ERROR");
    return true;
  }

  if (is_win)
    res.result = "win";
  else
    res.result = "lose";

  ROS_INFO("color = %s, number = %d", req.color.c_str(), req.number);
  ROS_INFO("%s", res.result.c_str());

  return true;
}

int main(int argc, char **argv)
{
  init(argc, argv, "casino_server");
  NodeHandle n;
  ServiceServer service = n.advertiseService("table", game);
  ROS_INFO("READY TO PLAY");
  spin();  
}
