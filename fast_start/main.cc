#include<iostream>
#include<string>
#include "contact.pb.h"

int main()
{
    std::string str;
   {
        //对一个人的信息使用PB进行序列化，并将结果打印出来：
        contact::PeopleInfo people;
        people.set_name("呆胶布");
        people.set_email("3484725023@qq.com");
        people.set_phone("18971285890");
        if(people.SerializeToString(&str)==false)
        {
            std::cout<<"wrong!"<<std::endl;
        }
   }

   {
        contact::PeopleInfo people;
        if(people.ParseFromString(str)==false)
        {
            std::cout<<"wrong!"<<std::endl;
        }
        std::cout<<people.name()<<std::endl;
        std::cout<<people.email()<<std::endl;
        std::cout<<people.phone()<<std::endl;
   }

    return 0;
}