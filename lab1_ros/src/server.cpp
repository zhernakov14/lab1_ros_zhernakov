#include "ros/ros.h"
#include "lab1_ros/table.h"
#include "std_msgs/String.h"
#include <iostream>
#include <string>

using namespace std;
using namespace ros;

Publisher g_publisher;
Subscriber g_subscriber;

bool game(lab_1::table::Request &req, 
          lab_1::table::Response &res)
{
  // задание выбора игрока
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
  
  // определение победы или поражения, когда введены цвет и число
  if (player_color == "green")
    is_win = (true_number == 0) && (true_color == "green");
  else if (player_color == "red")
    is_win = (true_number == player_number) && (true_color == "red");
  else if (player_color == "black")
    is_win = (true_number == player_number) && (true_color == "black");
  else
    res.result = "error";
  
  // определение ответного значения и вывод в терминал введенного цвета и числа
  if (res.result != "error")
  {
    if (is_win)
      res.result = "win";
    else
      res.result = "lose";
    ROS_INFO("color = %s, number = %d", req.color.c_str(), req.number);
  }
  
  // отправка результата в топик
  std_msgs::String answer;
  answer.data = res.result;
  g_publisher.publish(answer);

  return true;
}

// функция обработки сообщения из топика
void casino_callback(const std_msgs::String::ConstPtr& msg)
{
  if (msg->data == "win")
    ROS_INFO("win");
  else if (msg->data == "lose")
    ROS_INFO("lose");
  else
    ROS_INFO("error");
}

int main(int argc, char **argv)
{
  init(argc, argv, "casino_server");
  NodeHandle n;

  string topic_name;
  n.getParam("/casino_topic_name", topic_name);
  g_publisher = n.advertise<std_msgs::String>(topic_name, 100);
  g_subscriber = n.subscribe(topic_name, 100, casino_callback);

  ServiceServer service = n.advertiseService("table", game);

  ROS_INFO("READY TO PLAY");
  spin(); 

  return 0; 
}
