#include<iostream>
#include<string>
#include"contact.pb.h"
#include<fstream>


void Readcontacts()
{
    c_contact::Contacts contacts;

    std::fstream ifs("../contacts.bin",std::ios::binary|std::ios::in);
    if(ifs.is_open()==false) std::cerr<<"file open failed"<<std::endl;
    if(contacts.ParseFromIstream(&ifs)==false) std::cerr<<"parse failed"<<std::endl; 

    for(int i=0;i<contacts.contacts_size();i++)
    {
        const c_contact::PeopleInfo& people=contacts.contacts(i);
        std::cout<<"name: "<<people.name()<<std::endl;
        for(int j=0;j<people.phone_numbers_size();j++)
        {
            const c_contact::PeopleInfo_phone& phone_number=people.phone_numbers(j);
            std::cout<<j<<" : "<<phone_number.number()<<"("<<phone_number.Type_Name(phone_number.type())<<")"<<std::endl;
        }

        //读取oneof字段
        switch(people.other_types_case())
        {
            //使用OtherTypesCase来判断是哪种类型
            case c_contact::PeopleInfo::OtherTypesCase::kQq: std::cout<<"qq: "<<people.qq()<<std::endl; break;
            case c_contact::PeopleInfo::OtherTypesCase::kWechat: std::cout<<"wechat: "<<people.wechat()<<std::endl; break;
        }

        //读取map字段
        if(people.remark_size())
        {
            for(auto it=people.remark().cbegin();it!=people.remark().cend();++it)
            {
                std::cout<<"remark label: "<<it->first<<std::endl;
                std::cout<<"remark contain: "<<it->second<<std::endl;
            }
        }
        
        //读取未知字段
        const google::protobuf::Reflection* reflection = c_contact::PeopleInfo::GetReflection();
        const google::protobuf::UnknownFieldSet& set=reflection->GetUnknownFields(people);
        for(int j=0;j<set.field_count();j++)
        {
            const google::protobuf::UnknownField& unknown_field= set.field(j);
            std::cout<<"未知字段"<<j+1<<" : "<<"编号: "<<unknown_field.number();
            switch(unknown_field.type())
            {
                case google::protobuf::UnknownField::Type::TYPE_VARINT: std::cout<<unknown_field.varint()<<std::endl; break;
                case google::protobuf::UnknownField::Type::TYPE_FIXED32: std::cout<<unknown_field.fixed32()<<std::endl; break;
                case google::protobuf::UnknownField::Type::TYPE_FIXED64: std::cout<<unknown_field.fixed64()<<std::endl; break;
                case google::protobuf::UnknownField::Type::TYPE_LENGTH_DELIMITED: std::cout<<unknown_field.length_delimited()<<std::endl; break;
            }
        }
    }
}
int main()
{
    Readcontacts();
    return 0;
}