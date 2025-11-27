# protobuf

### 介绍

**Protocol Buffers**，它是 Google 开源的一种**语言中立、平台无关、可扩展的结构化数据序列化协议**，核心用于在不同系统（跨语言 / 跨平台）间高效传输、存储结构化数据，常替代 JSON、XML 等文本格式。

简单来说：Protobuf 先通过 `.proto` 文件定义数据结构，再通过编译器生成对应语言的代码，最终实现数据的 “序列化（对象→二进制）” 和 “反序列化（二进制→对象）”，是微服务、RPC 通信（如 gRPC 基于 Protobuf）的核心基础。

### 核心特点

#### 1. 极致高效（体积小、解析快）

* **体积更小**：采用二进制序列化，无 JSON/XML 冗余的文本标记（如 `{}`、`<>`、逗号、引号），同等数据体积比 JSON 小 3\~10 倍，比 XML 小 10\~20 倍；
* **解析更快**：无需像文本格式那样逐字符解析，二进制结构直接映射到内存对象，解析速度是 JSON 的 20\~100 倍，尤其适合带宽 / 性能敏感场景（如物联网、移动端、高频 RPC 调用）。

#### 2. 跨语言、跨平台兼容

* 支持几乎所有主流编程语言：C++、Java、Python、Go、JavaScrip、PHP、C#、Ruby 等；
* 只需编写一份 `.proto` 数据结构定义，通过 Protobuf 编译器（`protoc`）可生成任意支持语言的代码，轻松实现 “Go 服务→Java 服务→Python 脚本” 间的数据互通。

#### 3. 强类型 + 结构化，类型安全

* 必须在 `.proto` 文件中明确声明字段的**类型**（如 `int32`、`string`、`bool`、嵌套对象、枚举等）、**名称**和**唯一编号**；
* 编译时会校验类型合法性，避免 JSON/XML 因类型不匹配导致的运行时错误（比如把字符串 “123” 当数字解析）。

#### 4. 极佳的向后 / 向前兼容性（扩展性）

这是 Protobuf 最核心的优势之一：

* 字段通过**唯一数字编号**标识（而非字段名），序列化时仅传输编号 + 值，字段名仅用于代码层面；
* 新增 / 废弃字段时，旧版本程序会**自动忽略新增字段**，新版本程序能**兼容旧字段**（只要不修改已有字段的编号 / 类型），无需担心 “接口升级导致全链路崩溃”。

#### 5. 代码自动生成，开发效率高

编写完 `.proto` 定义文件后，通过 `protoc` 一键生成对应语言的：

* 数据模型类（对应定义的结构体）；
* 序列化（`SerializeToString`）/ 反序列化（`ParseFromString`）方法；
* 字段读写的封装方法；
  无需手动编写解析二进制数据的代码，大幅减少重复工作。

#### 6. 支持复杂数据结构

除基础类型外，还支持：

* 嵌套消息（比如一个 `User` 消息里嵌套 `Address` 消息）；
* 枚举（`enum`）、Map 类型、OneOf（多字段互斥）；
* 可选字段、必填字段（新版本 Protobuf 3 简化为默认可选）；
  满足复杂业务场景的结构化数据需求。

### Ubuntu安装

1. 依赖库安装

~~~bash
sudo apt-get install autoconf automake libtool curl 1 make g++ unzip -y
~~~

2. 下载protobuf的库文件和编译器

```bash
wget https://github.com/protocolbuffers/protobuf/releases/download/v21.1/protobuf-cpp-3.21.1.zip
```

3. 解压并进行安装

~~~bash
./configure && make && make check && sudo make install
~~~

### 快速上⼿

在快速上⼿中，会编写第⼀版本的通讯录 1.0。在通讯录 1.0 版本中，将实现：

• 对⼀个联系⼈的信息使⽤ PB 进⾏序列化，并将结果打印出来。

• 对序列化后的内容使⽤ PB 进⾏反序列，解析出联系⼈信息并打印出来。

• 联系⼈包含以下信息: 姓名、年龄。

通过通讯录 1.0，我们便能了解使⽤ ProtoBuf 初步要掌握的内容，以及体验到 ProtoBuf 的完整使⽤流程。

#### 步骤1：创建 .proto ⽂件

##### ⽂件规范

• 创建 .proto ⽂件时，⽂件命名应该使⽤全⼩写字⺟命名，多个字⺟之间⽤ \_ 连接。 例如 lower\_snake\_case.proto 。

• 书写 .proto ⽂件代码时，应使⽤ 2 个空格的缩进。

为通讯录 1.0 新建⽂件： contacts.proto

##### 添加注释

向文件添加注释，可使用 // 或者 /\* ... \*/

##### 指定 proto3 语法

Protocol Buffers 语言版本 3，简称 proto3，是 .proto 文件最新的语法版本。proto3 简化了 Protocol Buffers 语言，既易于使用，又可以在更广泛的编程语言中使用。它允许你使用 Java，C++，Python 等多种语言生成 protocol buffer 代码。

在 .proto 文件中，要使用 `syntax = "proto3";` 来指定文件语法为 proto3，并且必须写在除去注释内容的第一行。如果没有指定，编译器会使用 proto2 语法。

在通讯录 1.0 的 contacts.proto 文件中，可以为文件指定 proto3 语法，内容如下：

```cpp
syntax = "proto3";
```

##### package 声明符

package 是一个可选的声明符，能表示 .proto 文件的命名空间，在项目中要有唯一性。它的作用是为了避免我们定义的消息出现冲突。

在通讯录 1.0 的 contacts.proto 文件中，可以声明其命名空间，内容如下：

```cpp
syntax = "proto3";
package contacts;
```

##### 定义消息（message）

消息（message）: 要定义的结构化对象，我们可以给这个结构化对象定义其对应的属性内容。

Protobuf 以 message 的方式来支持定制协议字段，后期帮助我们形成类和方法来使用。在通讯录 1.0 中就需要为联系人定义一个 message。

.proto 文件中定义一个消息类型的格式为：

```cpp
message 消息类型名{

}
```

消息类型命名规范：使用驼峰命名法，首字母大写。

为 contacts.proto（通讯录 1.0）新增联系人 message，内容如下：

```cpp
syntax = "proto3";
package contacts;

// 定义联系人消息
message PeopleInfo {
}
```

##### 定义消息字段

在 message 中我们可以定义其属性字段，字段定义格式为：`字段类型 字段名 = 字段唯一编号;`

* 字段名称命名规范：全小写字母，多个字母之间用 \_ 连接。
* 字段类型分为：标量数据类型 和特殊类型（包括枚举、其他消息类型等）。
* 字段唯一编号：用来标识字段，一旦开始使用就不能够再改变。

该表格展示了定义于消息体中的标量数据类型，以及编译 .proto 文件之后自动生成的类中与之对应的字段类型（以 C++ 语言为例）：


| .proto Type | Notes                                                                            | C++ Type |
| ----------- | -------------------------------------------------------------------------------- | -------- |
| double      |                                                                                  | double   |
| float       |                                                                                  | float    |
| int32       | 使用变长编码 [1]。负数的编码效率较低 —— 若字段可能为负值，应使用 sint32 代替。 | int32    |
| int64       | 使用变长编码 [1]。负数的编码效率较低 —— 若字段可能为负值，应使用 sint64 代替。 | int64    |
| uint32      | 使用变长编码 [1]。                                                               | uint32   |
| uint64      | 使用变长编码 [1]。                                                               | uint64   |
| sint32      | 使用变长编码 [1]。符号整型。负值的编码效率高于常规的 int32 类型。                | int32    |
| sint64      | 使用变长编码 [1]。符号整型。负值的编码效率高于常规的 int64 类型。                | int64    |
| fixed32     | 定长 4 字节。若值常大于 2^28 则会比 uint32 更高效。                              | uint32   |
| fixed64     | 定长 8 字节。若值常大于 2^56 则会比 uint64 更高效。                              | uint64   |
| sfixed32    | 定长 4 字节。                                                                    | int32    |
| sfixed64    | 定长 8 字节。                                                                    | int64    |
| bool        |                                                                                  | bool     |
| string      | 包含 UTF-8 和 ASCII 编码的字符串，长度不能超过 2^32。                            | string   |
| bytes       | 可包含任意的字节序列但长度不能超过 2^32。                                        | string   |

[1] 变长编码是指：经过 protobuf 编码后，原本 4 字节或 8 字节的数可能会被变为其他字节数。

更新 contacts.proto (通讯录 1.0)，新增姓名、年龄字段：

```cpp
syntax = "proto3";
package contacts;

message PeopleInfo {
  string name = 1;
  int32 age = 2;
}
```

在这里还要特别讲解一下字段唯一编号的范围：

1 \~ 536,870,911 (2^29 - 1)，其中 `19000 ~ 19999` 不可用。

文件中使用这些预留标识号，例如将 name 字段的编号设置为 19000，编译时就会报警：

```cpp
// 消息中定义了如下编号，代码会告警：
// Field numbers 19,000 through 19,999 are reserved for the protobuf implementation
string name = 19000;
```

值得一提的是，范围为 1 \~ 15 的字段编号需要一个字节进行编码，16 \~ 2047 内的数字需要两个字节进行编码。编码后的字节不仅只包含了编号，还包含了字段类型。**所以 1 \~ 15 要用来标记出现非常频繁的字段**，**要为将来有可能添加的、频繁出现的字段预留一些出来。**

#### 步骤 2：编译 contacts.proto 文件，生成 C++ 文件

##### 编译命令

编译命令行格式为：

```bash
protoc [--proto_path=IMPORT_PATH] --cpp_out=DST_DIR path/to/file.proto
```

* `protoc`：是 Protocol Buffer 提供的命令行编译工具。
* `--proto_path`：指定被编译的`.proto`文件所在目录，可多次指定，可简写成 `-I IMPORT_PATH`。如不指定该参数，则在当前目录进行搜索；当某个`.proto`文件 `import` 其他 `.proto` 文件，或需要编译的 `.proto` 文件不在当前目录下时，需用`-I`指定搜索目录。
* `--cpp_out=`：指编译后的文件为 C++ 文件。
* `OUT_DIR`：编译后生成文件的目标路径。
* `path/to/file.proto`：要编译的`.proto`文件。

编译`contacts.proto`文件命令如下：

```bash
protoc --cpp_out=. contacts.proto
```

##### 编译 contacts.proto 文件后会生成什么

编译`contacts.proto`文件后，会生成所选择语言的代码（此处选择 C++），因此会生成两个文件：`contacts.pb.h`、`contacts.pb.cc`。

对于编译生成的 C++ 代码，包含以下内容：

* 对于每个 `message`，都会生成一个对应的消息类。
* 在消息类中，编译器为每个字段提供了获取和设置方法，以及其他能够操作字段的方法。
* 编译器会针对每个 `.proto` 文件生成 `.h` 和 `.cc` 文件，分别用来存放类的声明与类的实现。

**下面是一些常用序列化 / 反序列化接口：**


| 接口名称（英文）                            | 核心用途             | 关键说明                                                                                             |
| ------------------------------------------- | -------------------- | ---------------------------------------------------------------------------------------------------- |
| SerializeToString(string\* output)          | 序列化为字符串       | 将包含 Enum/Any 字段的消息序列化为二进制字符串；返回`bool`，`true` 表示成功，适用于内存传输。        |
| ParseFromString(const string& data)         | 从字符串反序列化     | 从二进制字符串解析消息，自动还原 Enum/Any 字段值；返回`bool`，`false` 表示数据损坏 / 格式错误。      |
| SerializeToOstream(ostream\* output)        | 序列化为文件 / 流    | 将消息写入文件流 / 网络流（如`std::ofstream`），适合持久化或跨进程传输，自动处理 Enum/Any 字段。     |
| ParseFromIstream(istream\* input)           | 从文件 / 流反序列化  | 从流中读取数据并解析消息，还原 Enum/Any 字段，需确保流数据完整。                                     |
| SerializeToArray(void\* data, int size)     | 序列化为字节数组     | 将消息写入指定字节数组，入参为数组指针 + 最大长度；返回实际序列化字节数，适合嵌入式 / 底层网络场景。 |
| ParseFromArray(const void\* data, int size) | 从字节数组反序列化   | 从字节数组解析消息，入参为数组指针 + 长度，自动还原 Enum/Any 字段值。                                |
| ByteSizeLong()                              | 获取序列化后字节大小 | 返回`int64_t`，预估 Enum/Any 字段包含在内的消息总字节数，用于提前分配内存。                          |

#### 步骤3：序列化与反序列化的使⽤

创建⼀个测试⽂件 main.cc，⽅法中我们实现：

• 对⼀个联系⼈的信息使⽤ PB 进⾏序列化，并将结果打印出来。

• 对序列化后的内容使⽤ PB 进⾏反序列，解析出联系⼈信息并打印出来。

```cpp
syntax="proto3";//指定proto版本
package contact;//定义包名

message PeopleInfo
{
    string name = 1;//编号代表字段序号
    string email = 2;
    string phone = 3;
}
```

```cpp
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
```

### proto 3 语法详解

在语法详解部分，依旧使用项目推进的方式完成教学。这个部分会对通讯录进行多次升级，使用 2.x 表示升级的版本，最终将会升级如下内容：

* 不再打印联系人的序列化结果，而是将通讯录序列化后并写入文件中。
* 从文件中将通讯录解析出来，并进行打印。
* 新增联系人属性，共包括：姓名、年龄、电话信息、地址、其他联系方式、备注。

#### 字段规则

消息的字段可以用下面几种规则来修饰：

* **singular**：消息中可以包含该字段零次或一次（不超过一次）。proto3 语法中，字段默认使用该规则。
* **repeated**：消息中可以包含该字段任意多次（包括零次），其中重复值的顺序会被保留。可以理解为定义了一个数组。

更新 contacts.proto，`PeopleInfo` 消息中新增 `phone_numbers` 字段，表示一个联系人有多个号码，可将其设置为 repeated，写法如下：

```cpp
syntax = "proto3";
package contacts;

message PeopleInfo {
  string name = 1;
  int32 age = 2;
  repeated string phone_numbers = 3;
}
```

#### 消息类型的定义与使用

#### 定义

在安装 Protobuf 时，其中的 include 目录下查找所有 google 已经定义好的.proto 文件，这些都是已经已定好的类型。

```bash
drw1@iZn4adgxsi96b0p10hxn1fZ:~/protobuf$ ls /usr/local/include/google/protobuf
any.h                  descriptor.h              extension_set_inl.h               has_bits.h               message.h              repeated_field.h      type.pb.h
any.pb.h               descriptor.pb.h           field_access_listener.h           implicit_weak_message.h  message_lite.h         repeated_ptr_field.h  type.proto
any.proto              descriptor.proto          field_mask.pb.h                   inlined_string_field.h   metadata.h             service.h             unknown_field_set.h
api.pb.h               duration.pb.h             field_mask.proto                  io                       metadata_lite.h        source_context.pb.h   util
api.proto              duration.proto            generated_enum_reflection.h       map_entry.h              parse_context.h        source_context.proto  wire_format.h
arena.h                dynamic_message.h         generated_enum_util.h             map_entry_lite.h         port_def.inc           struct.pb.h           wire_format_lite.h
arena_impl.h           empty.pb.h                generated_message_bases.h         map_field.h              port.h                 struct.proto          wrappers.pb.h
arenastring.h          empty.proto               generated_message_reflection.h    map_field_inl.h          port_undef.inc         stubs                 wrappers.proto
arenaz_sampler.h       endian.h                  generated_message_tctable_decl.h  map_field_lite.h         reflection.h           text_format.h
compiler               explicitly_constructed.h  generated_message_tctable_impl.h  map.h                    reflection_internal.h  timestamp.pb.h
descriptor_database.h  extension_set.h           generated_message_util.h          map_type_handler.h       reflection_ops.h       timestamp.proto
```

在单个.proto 文件中可以定义多个消息体，且支持定义嵌套类型的消息（任意多层）。每个消息体中的字段编号可以重复。

更新 contacts.proto，我们可以将 phone\_number 提取出来，单独成为一个消息：

//--------------------- 嵌套写法 ---------------------------

```cpp
syntax = "proto3";
package contacts;

message PeopleInfo {
  string name = 1;
  int32 age = 2;

  message Phone {
    string number = 1;
  }
}
```

//--------------------- 非嵌套写法 ---------------------------

```cpp
syntax = "proto3";
package contacts;

message Phone {
  string number = 1;
}

message PeopleInfo {
  string name = 1;
  int32 age = 2;
}
```

#### 使用

* 消息类型可作为字段类型使用

```cpp
syntax = "proto3";
package contacts;

// 联系人
message PeopleInfo {
  string name = 1;
  int32 age = 2;

  message Phone {
    string number = 1;
  }

  repeated Phone phone = 3;
}
```

* 可导入其他.proto 文件的消息并使用

例如 Phone 消息定义在 phone.proto 文件中：

```cpp
//例如 Phone 消息定义在 phone.proto 文件中：
syntax = "proto3";
package phone;

message Phone {
  string number = 1;
}
```

contacts.proto 中的 `PeopleInfo` 使用 `Phone` 消息：

```cpp
syntax = "proto3";
package contacts;

import "phone.proto";  // 使用 import 将 phone.proto 文件导入进来！！！

message PeopleInfo {
  string name = 1;
  int32 age = 2;

  // 引入的文件声明了package，使用消息时，需要用‘命名空间.消息类型’格式
  repeated phone.Phone phone = 3;
}
```

注：在 proto3 文件中可以导入 proto2 消息类型并使用它们，反之亦然。

#### 创建通讯录 2.0 版本

需求是**向文件中写入通讯录列表**，以上我们只是定义了一个联系人的消息，并不能存放通讯录列表，所以还需要在完善一下 contacts.proto (终版通讯录 2.0):

```cpp
syntax="proto3";
package contact;

message PeopleInfo
{
    string name = 1;

    message phone
    {
        string phone_number = 1;
    }

    repeated phone phone_numbers = 2;
}

 message Contacts
 {
    repeated PeopleInfo contacts = 1;
 }
```

完成.proto文件后编译，读取二进制文件并添加一个用户，写入到文件中，最后读取文件中全部用户信息：

~~~cpp
#include<iostream>
#include<string>
#include "contacts.pb.h"
#include<fstream>


void Addusertocontacts(contact::PeopleInfo * contacts)
{
    std::string name;
    std::cout<<"your name: ";
    getline(std::cin,name);
    contacts->set_name(name);

    std::string phone;
    while(true)
    {
        phone.clear();
        std::cout<<"your phone: ";
        getline(std::cin,phone);
        if(phone.empty()) break;

        contact::PeopleInfo::phone * phone_number=contacts->add_phone_numbers();
        phone_number->set_phone_number(phone);
    }

}

void Writecontacts()
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    //宏: 验证没有意外链接到与编译的头⽂件不兼容的库版本。如果检测到版本不匹配，程序将中⽌。
    //注意，每个 .pb.cc ⽂件在启动时都会⾃动调⽤此宏。在使⽤ C++ Protocol Buffer 库之前执⾏此宏是⼀种很好的做法，但不是绝对必要的。
    contact::Contacts contacts;

    std::fstream ifs("contacts.bin",std::ios::binary|std::ios::in);
    if(ifs.is_open()==false) std::cerr<<"file open failed"<<std::endl;
    if(contacts.ParseFromIstream(&ifs)==false) std::cerr<<"parse failed"<<std::endl;

    Addusertocontacts(contacts.add_contacts());

    std::fstream ofs("contacts.bin",std::ios::out|std::ios::binary);
    if(ofs.is_open()==false) std::cerr<<"file open failed"<<std::endl;
    if(contacts.SerializeToOstream(&ofs)==false) std::cerr<<"serialize failed"<<std::endl;

    google::protobuf::ShutdownProtobufLibrary();
    //在程序结束时调⽤ ShutdownProtobufLibrary()，删除 Protocol Buffer 库分配的所有全局对象。
    //对于⼤多数程序来说这是不必要的，因为该过程⽆论如何都要退出，并且操作系统将负责回收其所有内存。
    //但是，如果你使⽤了内存泄漏检查程序，该程序需要释放每个最后对象，或者你正在编写可以由单个进程多次加载和卸载的库，
    //那么你可能希望强制使⽤ Protocol Buffers 来清理所有内容。
}

void Readcontacts()
{
    contact::Contacts contacts;

    std::fstream ifs("contacts.bin",std::ios::binary|std::ios::in);
    if(ifs.is_open()==false) std::cerr<<"file open failed"<<std::endl;
    if(contacts.ParseFromIstream(&ifs)==false) std::cerr<<"parse failed"<<std::endl; 

    for(int i=0;i<contacts.contacts_size();i++)
    {
        const contact::PeopleInfo& people=contacts.contacts(i);
        std::cout<<"name: "<<people.name()<<std::endl;
        for(int i=0;i<people.phone_numbers_size();i++)
        {
            std::cout<<i<<" : "<<people.phone_numbers(i).phone_number()<<std::endl;
        }
  
    }
}
int main()
{
    Writecontacts();
    Readcontacts();
    return 0;
}
~~~

##### hexdump 工具与 -C 选项详解

hexdump 是 Linux 下分析二进制文件的核心工具，而 `-C`（大写 C）是最常用的选项之一，核心价值是**同时展示十六进制字节和对应的 ASCII 字符**，既保留二进制的精准性，又通过 ASCII 直观识别可打印字符（如字母、数字、符号），非常适合调试 Protobuf 序列化后的二进制文件、网络包、日志等场景。

查看二进制文件：

```bash
hexdump -C [选项] [目标文件/输入流]
```

##### protoc --help

执行 `protoc -h` 后，会在终端输出 `protoc` 的完整参数列表，帮助开发者了解如何通过命令行配置编译行为（如指定文件搜索路径、生成不同语言代码、执行编解码等操作）。

##### 主要选项分类

#### 1. 目录与搜索路径

* `-IPATH` / `--proto_path=PATH`：指定查找 `.proto` 文件的搜索目录，可多次指定，Protobuf 会按顺序搜索。若未指定，默认使用当前工作目录。

#### 2. 帮助与版本信息

* `--version`：显示 `protoc` 的版本信息并退出。
* `-h` / `--help`：显示帮助信息（即所有参数列表）并退出。

#### 3. 编码与解码

* `--encode=MESSAGE_TYPE`：从标准输入读取文本格式的 Protobuf 消息，转换为二进制格式后输出到标准输出。其中 `MESSAGE_TYPE` 需在 `.proto` 文件或其导入中定义。
* `--decode=MESSAGE_TYPE`：从标准输入读取二进制格式的 Protobuf 消息，转换为文本格式后输出到标准输出（ `protoc --decode=contacts.Contacts contacts.proto < contacts.bin` 就是典型用法）。
* `--decode_raw`：读取任意二进制 Protobuf 消息，直接以原始 `tag/value` 对的形式输出，无需指定 `.proto` 文件。

#### 4. 描述文件导入与导出

* `--descriptor_set_in=FILES`：指定包含 `FileDescriptorSet` 的文件，用于加载 `.proto` 描述信息。
* `-oFILE` / `--descriptor_set_out=FILE`：将输入 `.proto` 文件打包成 `FileDescriptorSet` 格式的二进制文件并输出。
* `--include_imports`：配合 `--descriptor_set_out` 使用，将所有依赖的 `.proto` 文件一并包含，使描述集自包含。
* `--include_source_info`：配合 `--descriptor_set_out` 使用，保留源代码信息（如注释、声明位置等）。

#### 5. 语言生成（核心编译功能）

`protoc` 支持为多种语言生成代码，通过 `--<lang>_out=OUT_DIR` 指定输出目录，例如：

* `--cpp_out=OUT_DIR`（生成 C++ 代码）
* `--java_out=OUT_DIR`（生成 Java 代码）
* `--python_out=OUT_DIR`（生成 Python 代码）
* 还有 C#、JavaScript、Kotlin 等语言的类似选项。

### enum 类型

#### 定义规则

语法支持我们定义枚举类型并使用。在.proto 文件中枚举类型的书写规范为：

- 枚举类型名称：使用驼峰命名法，首字母大写。例如：MyEnum
- 常量值名称：全大写字母，多个字母之间用 \_ 连接。例如：ENUM\_CONST = 0;

我们可以定义一个名为 PhoneType 的枚举类型，定义如下：

```plaintext
enum PhoneType {
  MP = 0;  // 移动电话
  TEL = 1;  // 固定电话
}
```

要注意枚举类型的定义有以下几种规则：

1. 0 值常量必须存在，且要作为第一个元素。这是为了与 proto2 的语义兼容：**第一个元素作为默认值，且值为 0**。
2. 枚举类型可以在消息外定义，也可以在消息体内定义（嵌套）。
3. 枚举的常量值在 32 位整数的范围内。但因负值无效因而不建议使用（与编码规则有关）。

### 定义时注意

将两个‘具有相同枚举值名称’的枚举类型放在单个.proto 文件下测试时，编译后会报错：某某某常量已经被定义！所以这里要注意：

* 同级（同层）的枚举类型，各个枚举类型中的常量不能重名。
* 单个.proto 文件下，最外层枚举类型和嵌套枚举类型，不算同级。
* 多个.proto 文件下，若一个文件引入了其他文件，且每个文件都未声明 package，每个 proto 文件中的枚举类型都在最外层，算同级。
* 多个.proto 文件下，若一个文件引入了其他文件，且每个文件都声明了 package，不算同级。

```cpp
 //---------------------- 情况 1：同级枚举类型包含相同枚举值名称 ----------------------

enum PhoneType {

MP = 0; // 移动电话

TEL = 1; // 固定电话

}

enum PhoneTypeCopy {

MP = 0; // 移动电话 // 编译后报错：MP 已经定义

}

//---------------------- 情况 2：不同级枚举类型包含相同枚举值名称 ----------------------

enum PhoneTypeCopy {

MP = 0; // 移动电话 // 用法正确

}

message Phone {

string number = 1; // 电话号码

enum PhoneType {

MP = 0; // 移动电话

TEL = 1; // 固定电话

}

}

//---------------------- 情况 3：多文件下都未声明 package ----------------------

//phone1.proto

import "phone1.proto"

enum PhoneType {

MP = 0; // 移动电话 // 编译后报错：MP 已经定义

TEL = 1; // 固定电话

}

//phone2.proto

enum PhoneTypeCopy {

MP = 0; // 移动电话

}

//---------------------- 情况 4：多文件下都声明了 package ----------------------

//phone1.proto

import "phone1.proto"

package phone1;

enum PhoneType {

MP = 0; // 移动电话 // 用法正确

TEL = 1; // 固定电话

}

//phone2.proto

package phone2;

enum PhoneTypeCopy {

MP = 0; // 移动电话

}
```

**下面是一些常用接口：**


| 接口名称                                            | 核心用途                     | 关键说明                                                                                                                                                    |
| --------------------------------------------------- | ---------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------- |
| set\_<enum\_field\_name>(EnumType value)            | 设置 Enum 字段值             | 消息类自动生成的接口，入参为枚举类型值（如`msg.set_status(Status::FAIL)`），直接为消息的 Enum 字段赋值；仅接受合法枚举值，非法值可能触发未定义行为。        |
| <enum\_field\_name>()                               | 获取 Enum 字段当前值         | 消息类自动生成的接口（如`msg.status()`），返回当前 Enum 字段的枚举值，用于读取字段状态。                                                                    |
| EnumName\_<EnumType>(EnumType value)                | 打印 / 获取 Enum 状态名      | 自动生成的工具函数，入参为枚举值，返回对应字符串（如`EnumName_Status(Status::SUCCESS)` 返回 `"SUCCESS"`）；仅支持合法枚举值，非法值返回空字符串或未知标识。 |
| <EnumType>\_descriptor()                            | 获取 Enum 描述符（基础）     | 返回`const EnumDescriptor*`，是所有 Enum 反射操作的入口（如动态打印名称、校验合法性）。                                                                     |
| EnumDescriptor::FindValueByNumber(int number)       | 校验 Enum 值合法性           | 入参为整数型枚举值，返回`EnumValueDescriptor*`；返回 `nullptr` 表示值非法，常用于设置 Enum 前的校验。                                                       |
| EnumDescriptor::FindValueByName(const string& name) | 字符串转 Enum 值（反向设置） | 入参为枚举名字符串，返回`EnumValueDescriptor*`，可通过 `number()` 转为枚举值后调用 `set_<enum_field_name>()` 设置，需先校验非空。                           |

改进通讯录：

contact.proto

```cpp
syntax="proto3";
package contact;

message PeopleInfo
{
    string name = 1;

    message phone
    {
        string number = 1;
        enum Type
        {
            MP = 0;
            TEL = 1;
        }
        Type type = 2;//枚举字段，非常重要！
    }

    repeated phone phone_numbers = 2;
}

 message Contacts
 {
    repeated PeopleInfo contacts = 1;
 }
```

main.cc

```cpp
void Addusertocontacts(contact::PeopleInfo * contacts)
{
    std::string name;
    std::cout<<"your name: ";
    getline(std::cin,name);
    contacts->set_name(name);

    std::string phone;
    while(true)
    {
        phone.clear();
        std::cout<<"your phone: ";
        getline(std::cin,phone);
        if(phone.empty()) break;

        contact::PeopleInfo_phone* phone_number=contacts->add_phone_numbers();
        phone_number->set_number(phone);

        std::cout<<"your type: ";
        int input; std::cin>>input;
        std::cin.ignore(256,'\n');
        switch(input)
        {
            case 1:phone_number->set_type(contact::PeopleInfo_phone_Type::PeopleInfo_phone_Type_MP); break;
            case 2:phone_number->set_type(contact::PeopleInfo_phone_Type::PeopleInfo_phone_Type_TEL); break;
        }
    }

}

void Writecontacts()
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    //宏: 验证没有意外链接到与编译的头⽂件不兼容的库版本。如果检测到版本不匹配，程序将中⽌。
    //注意，每个 .pb.cc ⽂件在启动时都会⾃动调⽤此宏。在使⽤ C++ Protocol Buffer 库之前执⾏此宏是⼀种很好的做法，但不是绝对必要的。
    contact::Contacts contacts;

    std::fstream ifs("contacts.bin",std::ios::binary|std::ios::in);
    if(ifs.is_open()==false) std::cerr<<"file open failed"<<std::endl;
    if(contacts.ParseFromIstream(&ifs)==false) std::cerr<<"parse failed"<<std::endl;

    Addusertocontacts(contacts.add_contacts());

    std::fstream ofs("contacts.bin",std::ios::out|std::ios::binary);
    if(ofs.is_open()==false) std::cerr<<"file open failed"<<std::endl;
    if(contacts.SerializeToOstream(&ofs)==false) std::cerr<<"serialize failed"<<std::endl;

    google::protobuf::ShutdownProtobufLibrary();
    //在程序结束时调⽤ ShutdownProtobufLibrary()，删除 Protocol Buffer 库分配的所有全局对象。
    //对于⼤多数程序来说这是不必要的，因为该过程⽆论如何都要退出，并且操作系统将负责回收其所有内存。
    //但是，如果你使⽤了内存泄漏检查程序，该程序需要释放每个最后对象，或者你正在编写可以由单个进程多次加载和卸载的库，
    //那么你可能希望强制使⽤ Protocol Buffers 来清理所有内容。
}

void Readcontacts()
{
    contact::Contacts contacts;

    std::fstream ifs("contacts.bin",std::ios::binary|std::ios::in);
    if(ifs.is_open()==false) std::cerr<<"file open failed"<<std::endl;
    if(contacts.ParseFromIstream(&ifs)==false) std::cerr<<"parse failed"<<std::endl; 

    for(int i=0;i<contacts.contacts_size();i++)
    {
        const contact::PeopleInfo& people=contacts.contacts(i);
        std::cout<<"name: "<<people.name()<<std::endl;
        for(int j=0;j<people.phone_numbers_size();j++)
        {
            const contact::PeopleInfo_phone& phone_number=people.phone_numbers(j);
            std::cout<<j<<" : "<<phone_number.number()<<"("<<phone_number.Type_Name(phone_number.type())<<")"<<std::endl;
        }
  
    }
}
int main()
{
    //Writecontacts();
    Readcontacts();
    return 0;
}
```

### Any 类型

字段还可以声明为 Any 类型，可以理解为泛型类型。使用时可以在 Any 中存储任意消息类型。**Any 类型的字段也用 repeated 来修饰**。

**下面是一些常用接口：**


| 接口名称                           | 核心用途                 | 关键说明                                                                                                     |
| ---------------------------------- | ------------------------ | ------------------------------------------------------------------------------------------------------------ |
| PackFrom(const Message& message)   | 打包任意消息到 Any 字段  | 入参为任意 Protobuf 消息对象（如`User`/`Order`），自动填充 `type_url`；打包后的 Any 字段可随消息一起序列化。 |
| UnpackTo(Message\* message) const  | 从 Any 解包为指定消息    | 入参为目标消息类型指针（如`User*`），返回 `bool`；`true` 表示解包成功（类型匹配），需在反序列化消息后调用。  |
| Is<T>() const                      | 判断 Any 存储的消息类型  | 模板接口（如`any_field.Is<User>()`），返回 `bool`；反序列化后先判断类型，再安全解包。                        |
| type\_url() const                  | 获取 Any 中消息类型 URL  | 返回`string`（格式：`type.googleapis.com/包名.消息名`），反序列化后可通过 URL 识别消息类型。                 |
| Clear()                            | 清空 Any 字段            | 重置`type_url` 和存储的消息数据，清空后 Any 字段序列化仅占空字段大小。                                       |
| SerializeToString(string\* output) | Any 单独序列化（极少用） | Any 本身是 Protobuf 消息类型，可单独序列化；通常随外层消息一起序列化，无需单独调用。                         |

**再次修改通讯录，添加any字段：**

contact.proto

```cpp
syntax="proto3";
package contact;
import "google/protobuf/any.proto";

message Address
{
    string home_address = 1;
    string unit_address = 2;
}
message PeopleInfo
{
    string name = 1;

    message phone
    {
        string number = 1;
        enum Type
        {
            MP = 0;
            TEL = 1;
        }
        Type type = 2;//枚举字段，非常重要！
    }

    repeated phone phone_numbers = 2;
    google.protobuf.Any any = 3;//any字段中定义了package，需要这样访问
}

message Contacts
{
repeated PeopleInfo contacts = 1;
}
```

main.cc

```cpp
void Addusertocontacts(contact::PeopleInfo * contacts)
{
    std::string name;
    std::cout<<"your name: ";
    getline(std::cin,name);
    contacts->set_name(name);

    std::string phone;
    while(true)
    {
        phone.clear();
        std::cout<<"your phone: ";
        getline(std::cin,phone);
        if(phone.empty()) break;

        contact::PeopleInfo_phone* phone_number=contacts->add_phone_numbers();
        phone_number->set_number(phone);

        //添加enum字段
        std::cout<<"your type: ";
        int input; std::cin>>input;
        std::cin.ignore(256,'\n');
        switch(input)
        {
            case 1:phone_number->set_type(contact::PeopleInfo_phone_Type::PeopleInfo_phone_Type_MP); break;
            case 2:phone_number->set_type(contact::PeopleInfo_phone_Type::PeopleInfo_phone_Type_TEL); break;
        }

    }


    //添加any字段
    std::string addr;
    contact::Address address;
    std::cout<<"your home_address: ";
    getline(std::cin,addr);
    address.set_home_address(addr);
    addr.clear();
    std::cout<<"your unit_address: ";
    getline(std::cin,addr);
    address.set_unit_address(addr);
    contacts->mutable_any()->PackFrom(address);//设置any类型
    //设置⽅法可以使⽤ mutable_ ⽅法，返回值为Any类型的指针，这类⽅法会为我们开辟好空间，可以直接对这块空间的内容进⾏修改。

}

void Writecontacts()
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    contact::Contacts contacts;

    std::fstream ifs("contacts.bin",std::ios::binary|std::ios::in);
    if(ifs.is_open()==false) std::cerr<<"file open failed"<<std::endl;
    if(contacts.ParseFromIstream(&ifs)==false) std::cerr<<"parse failed"<<std::endl;

    Addusertocontacts(contacts.add_contacts());

    std::fstream ofs("contacts.bin",std::ios::out|std::ios::binary);
    if(ofs.is_open()==false) std::cerr<<"file open failed"<<std::endl;
    if(contacts.SerializeToOstream(&ofs)==false) std::cerr<<"serialize failed"<<std::endl;

    google::protobuf::ShutdownProtobufLibrary();
}

void Readcontacts()
{
    contact::Contacts contacts;

    std::fstream ifs("contacts.bin",std::ios::binary|std::ios::in);
    if(ifs.is_open()==false) std::cerr<<"file open failed"<<std::endl;
    if(contacts.ParseFromIstream(&ifs)==false) std::cerr<<"parse failed"<<std::endl; 

    for(int i=0;i<contacts.contacts_size();i++)
    {
        const contact::PeopleInfo& people=contacts.contacts(i);
        std::cout<<"name: "<<people.name()<<std::endl;
        for(int j=0;j<people.phone_numbers_size();j++)
        {
            const contact::PeopleInfo_phone& phone_number=people.phone_numbers(j);
            std::cout<<j<<" : "<<phone_number.number()<<"("<<phone_number.Type_Name(phone_number.type())<<")"<<std::endl;
        }

        if(people.has_any()&&people.any().Is<contact::Address>())//判断any字段是否有数据，以及是否为address类型数据
        {
            contact::Address address;
            people.any().UnpackTo(&address);
            if(!address.home_address().empty()) std::cout<<"home address: "<<address.home_address()<<std::endl;
            if(!address.unit_address().empty()) std::cout<<"unit address: "<<address.unit_address()<<std::endl;
        }
  
    }
}
int main()
{
    Writecontacts();
    Readcontacts();
    return 0;
}
```

### oneof 类型

如果消息中有很多可选字段， 并且将来同时只有⼀个字段会被设置， 那么就可以使⽤ oneof 加强这个⾏为，也能有节约内存的效果。

**下面是一些常用接口：**


| 方法名称            | 核心用途                                | 返回值类型                                  | 关键说明                                                                                                                                                       |
| ------------------- | --------------------------------------- | ------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `<oneof名>_case()`  | 判断 oneof 中**哪个字段激活**           | 自动生成的枚举类型（如`DemoMsg::DataCase`） | 枚举值由 Protobuf 自动生成，唯一标识当前激活的字段；无激活字段时返回「未设置」枚举值。示例：`DemoMsg msg;``DemoMsg::DataCase case_val = msg.data_case();`      |
| `has_<字段名>()`    | 判断 oneof 中**指定字段是否激活**       | bool                                        | 仅对 oneof 内字段有效；返回`true` 表示该字段是当前激活的字段，`false` 则不是（或无字段激活）。示例：`if (msg.has_int_val()) { ... }` // 判断 int\_val 是否激活 |
| `clear_<oneof名>()` | 清空 oneof 所有字段，恢复「无数据」状态 | void                                        | 执行后`oneof_name_case()` 返回「未设置」枚举值，所有字段均返回类型默认值（如 int=0、string=""）。示例：`msg.clear_data();` // 清空 data 下所有互斥字段         |

contact.proto

```cpp
syntax="proto3";
package contact;
import "google/protobuf/any.proto";

message Address
{
    string home_address = 1;
    string unit_address = 2;
}
message PeopleInfo
{
    string name = 1;

    message phone
    {
        string number = 1;
        enum Type
        {
            MP = 0;
            TEL = 1;
        }
        Type type = 2;//枚举字段，非常重要！
    }

    repeated phone phone_numbers = 2;
    google.protobuf.Any any = 3;//any字段中定义了package，需要这样访问
    oneof other_types
    {
        string qq = 4;//从编号为4，注意不能使用repeated
        string wechat = 5;//编号为5

    }
}

message Contacts
{
repeated PeopleInfo contacts = 1;
}
```

main.cc

```cpp
void Addusertocontacts(contact::PeopleInfo * contacts)
{
    std::string name;
    std::cout<<"your name: ";
    getline(std::cin,name);
    contacts->set_name(name);

    std::string phone;
    while(true)
    {
        phone.clear();
        std::cout<<"your phone: ";
        getline(std::cin,phone);
        if(phone.empty()) break;

        contact::PeopleInfo_phone* phone_number=contacts->add_phone_numbers();
        phone_number->set_number(phone);

        //添加enum字段
        std::cout<<"your type: ";
        int input; std::cin>>input;
        std::cin.ignore(256,'\n');
        switch(input)
        {
            case 1:phone_number->set_type(contact::PeopleInfo_phone_Type::PeopleInfo_phone_Type_MP); break;
            case 2:phone_number->set_type(contact::PeopleInfo_phone_Type::PeopleInfo_phone_Type_TEL); break;
        }

    }


    //添加any字段
    std::string addr;
    contact::Address address;
    std::cout<<"your home_address: ";
    getline(std::cin,addr);
    address.set_home_address(addr);
    addr.clear();
    std::cout<<"your unit_address: ";
    getline(std::cin,addr);
    address.set_unit_address(addr);
    contacts->mutable_any()->PackFrom(address);

    //添加oneof字段
    std::string other;
    std::cout<<"qq: ";
    getline(std::cin,other);
    if(!other.empty())
    {
        contacts->set_qq(other);
    }
    other.clear();
    std::cout<<"wechat: ";
    getline(std::cin,other);
    if(!other.empty())
    {
        contacts->set_wechat(other);
    }
}

void Writecontacts()
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    contact::Contacts contacts;

    std::fstream ifs("contacts.bin",std::ios::binary|std::ios::in);
    if(ifs.is_open()==false) std::cerr<<"file open failed"<<std::endl;
    if(contacts.ParseFromIstream(&ifs)==false) std::cerr<<"parse failed"<<std::endl;

    Addusertocontacts(contacts.add_contacts());

    std::fstream ofs("contacts.bin",std::ios::out|std::ios::binary);
    if(ofs.is_open()==false) std::cerr<<"file open failed"<<std::endl;
    if(contacts.SerializeToOstream(&ofs)==false) std::cerr<<"serialize failed"<<std::endl;

    google::protobuf::ShutdownProtobufLibrary();
}

void Readcontacts()
{
    contact::Contacts contacts;

    std::fstream ifs("contacts.bin",std::ios::binary|std::ios::in);
    if(ifs.is_open()==false) std::cerr<<"file open failed"<<std::endl;
    if(contacts.ParseFromIstream(&ifs)==false) std::cerr<<"parse failed"<<std::endl; 

    for(int i=0;i<contacts.contacts_size();i++)
    {
        const contact::PeopleInfo& people=contacts.contacts(i);
        std::cout<<"name: "<<people.name()<<std::endl;
        for(int j=0;j<people.phone_numbers_size();j++)
        {
            const contact::PeopleInfo_phone& phone_number=people.phone_numbers(j);
            std::cout<<j<<" : "<<phone_number.number()<<"("<<phone_number.Type_Name(phone_number.type())<<")"<<std::endl;
        }

        if(people.has_any()&&people.any().Is<contact::Address>())//判断any字段是否有数据，以及是否为address类型数据
        {
            contact::Address address;
            people.any().UnpackTo(&address);
            if(!address.home_address().empty()) std::cout<<"home address: "<<address.home_address()<<std::endl;
            if(!address.unit_address().empty()) std::cout<<"unit address: "<<address.unit_address()<<std::endl;
        }

        switch(people.other_types_case())
        {
            case contact::PeopleInfo::OtherTypesCase::kQq: std::cout<<"qq: "<<people.qq()<<std::endl; break;
            case contact::PeopleInfo::OtherTypesCase::kWechat: std::cout<<"wechat: "<<people.wechat()<<std::endl; break; 
        }
  
    }
}
int main()
{
    Writecontacts();
    Readcontacts();
    return 0;
}
```

### map 类型

语法⽀持创建⼀个关联映射字段，也就是可以使⽤ map 类型去声明字段类型，格式为：

> map<key\_type, value\_type> map\_field = N;

要注意的是：

• key\_type 是除了 float 和 bytes 类型以外的任意标量类型。 value\_type 可以是任意类型。

• map 字段不可以⽤ repeated 修饰

• map 中存⼊的元素是⽆序的

**下面是一些常用接口：**


| 接口类型          | C++ 接口语法                            | 示例代码                                                                                                                                                                                          | 关键说明                                            |
| ----------------- | --------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | --------------------------------------------------- |
| 获取 map 元素个数 | 字段名\_size () const                   | `// 直接获取remark的元素数量``int remark_cnt = people.remark_size();`                                                                                                                             | Protobuf 生成的便捷接口，无需先调`mutable_remark()` |
| 插入键值对        | mutable\_字段名 ()->insert ({key, val}) | `// 你的示例写法直接复用``contacts->mutable_remark()->insert({remark_label, remark_contain});`                                                                                                    | 键已存在则插入失败（不覆盖），不存在则插入          |
| const 迭代遍历    | 字段名 ().cbegin () / 字段名 ().cend () | `// 你的示例遍历写法直接复用``auto it = people.remark().cbegin();``while (it != people.remark().cend()) {``string label = it->first; // 取key``string contain = it->second; // 取value``++it;``}` | cbegin/cend 返回 const 迭代器，仅可读、不可修改元素 |
| 清空 map          | mutable\_字段名 ()->clear ()            | `// 清空整个remark字典``contacts->mutable_remark()->clear();`                                                                                                                                     | 清空后`people.remark_size()`返回 0                  |
| 覆盖 / 强制插入值 | (\*mutable\_字段名 ())[key] = val       | `// 键存在则覆盖，不存在则插入``(*contacts->mutable_remark())[remark_label] = remark_contain;`                                                                                                    | 相比 insert，可强制覆盖已有键的值                   |
| 可变迭代遍历      | mutable\_字段名 ()->begin () /end ()    | `// 遍历同时修改值``auto it = contacts->mutable_remark()->begin();``while (it != contacts->mutable_remark()->end()) {``it->second = "new_contain"; // 修改value``++it;``}`                        | begin/end 返回可变迭代器，支持修改元素              |
| 判断键是否存在    | 字段名 ().count (key) const             | `// 检查remark_label是否存在``bool exists = (people.remark().count(remark_label) > 0);`                                                                                                           | 返回 0（不存在）/1（存在），const 场景可用          |
| 删除指定键        | mutable\_字段名 ()->erase (key)         | `// 删除指定label的remark``contacts->mutable_remark()->erase(remark_label);`                                                                                                                      | 返回删除个数（0 或 1），仅删除存在的键              |

contact.proto

```cpp
syntax="proto3";
package contact;
import "google/protobuf/any.proto";

message Address
{
    string home_address = 1;
    string unit_address = 2;
}
message PeopleInfo
{
    string name = 1;

    message phone
    {
        string number = 1;
        enum Type
        {
            MP = 0;
            TEL = 1;
        }
        Type type = 2;//枚举字段，非常重要！
    }

    repeated phone phone_numbers = 2;
    google.protobuf.Any any = 3;//any字段中定义了package，需要这样访问
    oneof other_types
    {
        string qq = 4;//从编号为4，注意不能使用repeated
        string wechat = 5;//编号为5

    }

    map<string,string>  remark = 7;//不能使用repeated，无序
}

message Contacts
{
repeated PeopleInfo contacts = 1;
}
```

main.cc:

```cpp
void Addusertocontacts(contact::PeopleInfo * contacts)
{
    std::string name;
    std::cout<<"your name: ";
    getline(std::cin,name);
    contacts->set_name(name);

    std::string phone;
    while(true)
    {
        phone.clear();
        std::cout<<"your phone: ";
        getline(std::cin,phone);
        if(phone.empty()) break;

        contact::PeopleInfo_phone* phone_number=contacts->add_phone_numbers();
        phone_number->set_number(phone);

        //添加enum字段
        std::cout<<"your type: ";
        int input; std::cin>>input;
        std::cin.ignore(256,'\n');
        switch(input)
        {
            case 1:phone_number->set_type(contact::PeopleInfo_phone_Type::PeopleInfo_phone_Type_MP); break;
            case 2:phone_number->set_type(contact::PeopleInfo_phone_Type::PeopleInfo_phone_Type_TEL); break;
        }

    }


    //添加any字段
    std::string addr;
    contact::Address address;
    std::cout<<"your home_address: ";
    getline(std::cin,addr);
    address.set_home_address(addr);
    addr.clear();
    std::cout<<"your unit_address: ";
    getline(std::cin,addr);
    address.set_unit_address(addr);
    contacts->mutable_any()->PackFrom(address);

    //添加oneof字段
    std::string other;
    std::cout<<"qq: ";
    getline(std::cin,other);
    if(!other.empty())
    {
        contacts->set_qq(other);
    }
    other.clear();
    std::cout<<"wechat: ";
    getline(std::cin,other);
    if(!other.empty())
    {
        contacts->set_wechat(other);
    }


    //添加map字段
    std::string remark_label;
    std::string remark_contain;
    while(true)
    {
        remark_label.clear();
        remark_contain.clear();
        std::cout<<"remark label: ";
        getline(std::cin,remark_label);
        if(remark_label.empty()) break;

        std::cout<<"remark contain: ";
        getline(std::cin,remark_contain);
        contacts->mutable_remark()->insert({remark_label,remark_contain});
    }

}

void Writecontacts()
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    contact::Contacts contacts;

    std::fstream ifs("contacts.bin",std::ios::binary|std::ios::in);
    if(ifs.is_open()==false) std::cerr<<"file open failed"<<std::endl;
    if(contacts.ParseFromIstream(&ifs)==false) std::cerr<<"parse failed"<<std::endl;

    Addusertocontacts(contacts.add_contacts());

    std::fstream ofs("contacts.bin",std::ios::out|std::ios::binary);
    if(ofs.is_open()==false) std::cerr<<"file open failed"<<std::endl;
    if(contacts.SerializeToOstream(&ofs)==false) std::cerr<<"serialize failed"<<std::endl;

    google::protobuf::ShutdownProtobufLibrary();
}

void Readcontacts()
{
    contact::Contacts contacts;

    std::fstream ifs("contacts.bin",std::ios::binary|std::ios::in);
    if(ifs.is_open()==false) std::cerr<<"file open failed"<<std::endl;
    if(contacts.ParseFromIstream(&ifs)==false) std::cerr<<"parse failed"<<std::endl; 

    for(int i=0;i<contacts.contacts_size();i++)
    {
        const contact::PeopleInfo& people=contacts.contacts(i);
        std::cout<<"name: "<<people.name()<<std::endl;
        for(int j=0;j<people.phone_numbers_size();j++)
        {
            const contact::PeopleInfo_phone& phone_number=people.phone_numbers(j);
            std::cout<<j<<" : "<<phone_number.number()<<"("<<phone_number.Type_Name(phone_number.type())<<")"<<std::endl;
        }

        //读取any字段
        if(people.has_any()&&people.any().Is<contact::Address>())//判断any字段是否有数据，以及是否为address类型数据
        {
            contact::Address address;
            people.any().UnpackTo(&address);
            if(!address.home_address().empty()) std::cout<<"home address: "<<address.home_address()<<std::endl;
            if(!address.unit_address().empty()) std::cout<<"unit address: "<<address.unit_address()<<std::endl;
        }

        //读取oneof字段
        switch(people.other_types_case())
        {
            //使用OtherTypesCase来判断是哪种类型
            case contact::PeopleInfo::OtherTypesCase::kQq: std::cout<<"qq: "<<people.qq()<<std::endl; break;
            case contact::PeopleInfo::OtherTypesCase::kWechat: std::cout<<"wechat: "<<people.wechat()<<std::endl; break; 
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
    }
}
int main()
{
    Writecontacts();
    Readcontacts();
    return 0;
}
```

### 默认值

反序列化消息时，如果被反序列化的二进制序列中不包含某个字段，反序列化对象中相应字段时，就会设置为该字段的默认值。不同的类型对应的默认值不同：

* 对于字符串，默认值为空字符串。
* 对于字节，默认值为空字节。
* 对于布尔值，默认值为 false。
* 对于数值类型，默认值为 0。
* 对于枚举，默认值是第一个定义的枚举值，必须为 0。
* 对于消息字段，未设置该字段，它的取值是依赖于语言。
* 对于设置了 repeated 的字段的默认值是空的（通常是相应语言的一个空列表）。
* 对于消息字段、oneof 字段和 any 字段，有 has\_方法来检测当前字段是否被设置。

### 更新消息

#### 更新规则

如果现有的消息类型已经不再满足我们的需求，例如需要扩展一个字段，在不破坏任何现有代码的情况下更新消息类型非常简单。遵循如下规则即可：

**新增：**

- 与已有字段不冲突即可

**修改：**

* 禁止修改任何已有字段的字段编号。
* 若是移除老字段，要保证不再使用移除字段的字段编号。正确的做法是保留字段编号（reserved），以确保该编号将不能被重复使用。不建议直接删除或注释掉字段。
* int32，uint32，int64，uint64 和 bool 是完全兼容的。可以从这些类型中的一个改为另一个，而不破坏前后兼容性。若解析出来的数值与相应的类型不匹配，会采用与 C++ 一致的处理方案（例如，若将 64 位整数当做 32 位进行读取，它将被截断为 32 位）。
* sint32 和 sint64 相互兼容但不与其他的整型兼容。
* string 和 bytes 在合法 UTF-8 字节前提下也是兼容的。
* bytes 包含消息编码版本的情况下，嵌套消息与 bytes 也是兼容的。
* fixed32 与 sfixed32 兼容，fixed64 与 sfixed64 兼容。
* enum 与 int32，uint32，int64 和 uint64 兼容（注意若值不匹配会被截断）。但要注意当反序列化消息时会根据语言采用不同的处理方案：例如，未识别的 proto3 枚举类型会被保存在消息中，但是当消息反序列化时如何表示依赖于编程语言的。整型字段总是会保持其的值。
* oneof:
  * 将一个单独的值更改为新 oneof 类型成员之一是安全和二进制兼容的。
  * 若确定没有代码一次性能设置多个值那么将多个字段移入一个新 oneof 类型也是可行的。
  * 将任何字段移入已存在的 oneof 类型是不安全的。

**删除：**

直接删除 protobuf 字段后用相同编号定义新字段，会导致**新旧版本消息解析混乱、数据错误**，因为 protobuf 是通过**字段编号（而非字段名）** 识别字段的，序列化 / 反序列化只认编号，不管字段名和类型。protobuf 的序列化数据中，存储的是 “字段编号 + 数据类型 + 值”，反序列化时仅根据**字段编号**匹配对应字段（忽略字段名）。若重复使用已删除字段的编号，新旧版本会将同编号的不同类型数据互解析，必然导致数据错乱。

假设旧版本 Proto 定义：

```protobuf
// 旧版本User消息：字段10是string类型的用户名
message User {
  string username = 10; // 字段编号10，类型string
}
```

旧程序序列化消息：`username = "Alice"`，此时序列化数据中，字段 10 对应的是string 类型的值 “Alice”。

若直接删除`username`，用编号 10 定义新字段：

```protobuf
// 新版本User消息：同编号10，改为int32类型的年龄
message User {
  int32 age = 10; // 重复使用编号10，类型改为int32
}
```

此时会发生两种错误：

1. **旧消息解析到新版本**：旧消息中字段 10 的 string 数据 “Alice”，会被新版本当成 int32 类型的`age`解析，导致字符串转整数失败（或出现非法数值、乱码）；
2. **新消息解析到旧版本**：新版本序列化的`age = 25`（字段 10 是 int32 类型），会被旧版本当成 string 类型的`username`解析，得到的是二进制对应的乱码字符。

这完全违反了 protobuf 的兼容性规则，正确做法是将旧字段的编号标记为`reserved`，禁止重复使用。


#### 保留字段 reserved

使用 reserved 将指定字段的编号或名称设置为保留项。再使用这些编号或名称时，protocol buffer 的编译器将会警告这些编号或名称不可用。举个例子：

```cpp
syntax = "proto3";

package demo;

message Message {
  // 设置保留项：字段编号
  reserved 100, 101, 200 to 299;
  // 设置保留项：字段名称
  reserved "field3", "field4";
  // 注意：不可在同一行reserved中混合声明编号和名称

  // 合法字段（无告警）
  int32 normal_field = 1;

  // 以下字段会触发编译器告警（示例，实际使用需注释掉）
  // int32 field1 = 100; // 告警: Field 'field1' uses reserved number 100
  // int32 field2 = 101; // 告警: Field 'field2' uses reserved number 101
  // int32 field3 = 102; // 告警: Field name 'field3' is reserved
  // int32 field4 = 103; // 告警: Field name 'field4' is reserved
}
```

这里运用保留字段修改一下代码，将读写分为客户端与服务端，目录结构如下：

```bash
drw1@iZn4adgxsi96b0p10hxn1fZ:~/protobuf/types$ tree .
.
├── client
│   ├── contact.pb.cc
│   ├── contact.pb.h
│   ├── contact.proto
│   ├── main
│   ├── main.cc
│   └── makefile
├── contacts.bin
└── server
    ├── contact.pb.cc
    ├── contact.pb.h
    ├── contact.proto
    ├── main
    ├── main.cc
    └── makefile

3 directories, 13 files
```

client/contact.proto & main.cc

```cpp
//contact.proto
syntax="proto3";
package c_contact;
import "google/protobuf/any.proto";


message PeopleInfo
{
    string name = 1;

    message phone
    {
        string number = 1;
        enum Type
        {
            MP = 0;
            TEL = 1;
        }
        Type type = 2;//枚举字段，非常重要！
    }

    repeated phone phone_numbers = 2;
    oneof other_types
    {
        string qq = 4;//从编号为4，注意不能使用repeated
        string wechat = 5;//编号为5

    }

    map<string,string>  remark = 7;//不能使用repeated，无序

  
}

message Contacts
{
    repeated PeopleInfo contacts = 1;
}

//main.cc
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
    }
}
int main()
{
    Readcontacts();
    return 0;
}


```

server/contact.proto & main.cc

```cpp
//contact.proto
syntax="proto3";
package s_contact;
import "google/protobuf/any.proto";

message Address
{
    string home_address = 1;
    string unit_address = 2;
}
message PeopleInfo
{
    string name = 1;

    message phone
    {
        string number = 1;
        enum Type
        {
            MP = 0;
            TEL = 1;
        }
        Type type = 2;//枚举字段，非常重要！
    }

    //repeated phone phone_numbers = 2;
  
    google.protobuf.Any any = 3;//any字段中定义了package，需要这样访问
    oneof other_types
    {
        string qq = 4;//从编号为4，注意不能使用repeated
        string wechat = 5;//编号为5

    }

    map<string,string>  remark = 7;//不能使用repeated，无序

    reserved 2;

    phone phone_number = 8;
}

message Contacts
{
    repeated PeopleInfo contacts = 1;
}//main.cc
#include<iostream>
#include<string>
#include"contact.pb.h"
#include<fstream>


void Addusertocontacts(s_contact::PeopleInfo * contacts)
{
    std::string name;
    std::cout<<"your name: ";
    getline(std::cin,name);
    contacts->set_name(name);

    std::string phone;
  
    std::cout<<"your phone: ";
    getline(std::cin,phone);
    s_contact::PeopleInfo_phone* phone_number=contacts->mutable_phone_number();
    phone_number->set_number(phone);

    //添加enum字段
    std::cout<<"your type: ";
    int input; std::cin>>input;
    std::cin.ignore(256,'\n');
    switch(input)
    {
        case 1:phone_number->set_type(s_contact::PeopleInfo_phone_Type::PeopleInfo_phone_Type_MP); break;
        case 2:phone_number->set_type(s_contact::PeopleInfo_phone_Type::PeopleInfo_phone_Type_TEL); break;
    }


    //添加oneof字段
    std::string other;
    std::cout<<"qq: ";
    getline(std::cin,other);
    if(!other.empty())
    {
        contacts->set_qq(other);
    }
    other.clear();
    std::cout<<"wechat: ";
    getline(std::cin,other);
    if(!other.empty())
    {
        contacts->set_wechat(other);
    }


    //添加map字段
    std::string remark_label;
    std::string remark_contain;
    while(true)
    {
        remark_label.clear();
        remark_contain.clear();
        std::cout<<"remark label: ";
        getline(std::cin,remark_label);
        if(remark_label.empty()) break;

        std::cout<<"remark contain: ";
        getline(std::cin,remark_contain);
        contacts->mutable_remark()->insert({remark_label,remark_contain});
    }

}

void Writecontacts()
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    s_contact::Contacts contacts;

    std::fstream ifs("../contacts.bin",std::ios::binary|std::ios::in);
    if(ifs.is_open()==false) std::cerr<<"file open failed"<<std::endl;
    if(contacts.ParseFromIstream(&ifs)==false) std::cerr<<"parse failed"<<std::endl;

    Addusertocontacts(contacts.add_contacts());

    std::fstream ofs("../contacts.bin",std::ios::out|std::ios::binary);
    if(ofs.is_open()==false) std::cerr<<"file open failed"<<std::endl;
    if(contacts.SerializeToOstream(&ofs)==false) std::cerr<<"serialize failed"<<std::endl;

    google::protobuf::ShutdownProtobufLibrary();
}


int main()
{
    Writecontacts();
    return 0;
}

```

启动服务端，向contacts.bin中写入数据，再通过客户端读出

---



#### 未知字段

前面向 service 目录下的 contacts.proto 新增了‘`phone_number`’字段，但对于 client 相关的代码并没有任何改动。验证后发现新代码序列化的消息（service）也可以被旧代码（client）解析。并且这里要说的是，新增的字段在旧程序（client）中其实并没有丢失，而是会作为旧程序的未知字段。

* 未知字段：解析结构良好的 protocol buffer 已序列化数据中的未识别字段的表示方式。例如，当旧程序解析带有新字段的数据时，这些新字段就会成为旧程序的未知字段。
* 本来，proto3 在解析消息时总是会丢弃未知字段，但在 3.5 版本中重新引入了对未知字段的保留机制。所以在 3.5 或更高版本中，未知字段在反序列化时会被保留，同时也会包含在序列化的结果中。

##### 从哪获取

先来了解相关类关系图

```plaintext
google::protobuf::MessageLite
├─ SerializeToXXX()
└─ ParseFromXXX()
      ↑（Extends）
google::protobuf::Message ←（Extends） 自定义消息
├─ GetDescriptor() →（Use） google::protobuf::Descriptor
│                        ├─ name()
│                        └─ field_count()
└─ GetReflection() →（Use） google::protobuf::Reflection
                           ├─ GetUnknownFields()
                           ├─ MutableUnknownFields()
                           └─ Set/Get/Add() →（Use） google::protobuf::UnknownFieldSet
                                                    ├─ Clear()
                                                    ├─ empty()
                                                    └─ field(int) →（Use） google::protobuf::UnknownField
                                                                       ├─ varint()
                                                                       ├─ fixed32()
                                                                       ├─ fixed64()
                                                                       └─ length_delimited()

```

##### MessageLite 类介绍（了解）

* MessageLite 从名字看是轻量级的 message，仅仅提供序列化、反序列化功能。
* 类定义在 google 提供的`message_lite.h`中。

##### Message 类介绍（了解）

* 我们自定义的 message 类，都是继承自 Message。
* Message 最重要的两个接口 `GetDescriptor/GetReflection`，可以获取该类型对应的 Descriptor 对象指针和 Reflection 对象指针。
* 类定义在 google 提供的`message.h`中。

```cpp
//google::protobuf::Message 部分代码展示
const Descriptor* GetDescriptor() const;
const Reflection* GetReflection() const;
```

##### Descriptor 类介绍（了解）

* Descriptor：是对 message 类型定义的描述，包括 message 的名字、所有字段的描述、原始 proto 文件内容等。
* 类定义在 google 提供的`descriptor.h`中。

```cpp
// 部分代码展示
class PROTOBUF_EXPORT Descriptor : private internal::SymbolBase {
    string& name() const;
    int field_count() const;
    const FieldDescriptor* field(int index) const;
    const FieldDescriptor* FindFieldByNumber(int number) const;
    const FieldDescriptor* FindFieldByName(const std::string& name) const;
    const FieldDescriptor* FindFieldByLowercaseName(
        const std::string& lowercase_name) const;
    const FieldDescriptor* FindFieldByCamelCaseName(
        const std::string& camelcase_name) const;
    int enum_type_count() const;
    const EnumDescriptor* enum_type(int index) const;
    const EnumDescriptor* FindEnumTypeByName(const std::string& name) const;
    const EnumValueDescriptor* FindEnumValueByName(const std::string& name) const;
}
```

##### Reflection 类介绍（了解）

* Reflection 接口类，主要提供了动态读写消息字段的接口，对消息对象的自动读写主要通过该类完成。
* 提供方法来动态访问 / 修改 message 中的字段，对每种类型，Reflection 都提供了一个单独的接口用于读写字段对应的值：
  ○ 针对所有不同的 field 类型`FieldDescriptor::TYPE_*`，需要使用不同的 `Get*()/Set*()/Add*()` 接口；
  ○ repeated 类型需要使用 `GetRepeated*()/SetRepeated*()` 接口，不可以和非 repeated 类型接口混用；
  ○ message 对象只可以被由它自身的 reflection（`message.GetReflection()`）来操作
* 类还包含了访问 / 修改未知字段的方法。
* 类定义在 google 提供的`message.h`中。

```cpp
// 部分代码展示
class PROTOBUF_EXPORT Reflection final {
    const UnknownFieldSet& GetUnknownFields(const Message& message) const;
    UnknownFieldSet* MutableUnknownFields(Message* message) const;
    bool HasField(const Message& message, const FieldDescriptor* field) const;
    int FieldSize(const Message& message, const FieldDescriptor* field) const;
    void ClearField(Message* message, const FieldDescriptor* field) const;
    bool HasOneof(const Message& message,
        const OneofDescriptor* oneof_descriptor) const;
    void ClearOneof(Message* message,
        const OneofDescriptor* oneof_descriptor) const;
    const FieldDescriptor* GetOneofFieldDescriptor(
        const Message& message, const OneofDescriptor* oneof_descriptor) const;
    // Singular field getters
    // These get the value of a non-repeated field. They return the default
    // value for fields that aren't set.
    int32_t GetInt32(const Message& message, const FieldDescriptor* field) const;
    int64_t GetInt64(const Message& message, const FieldDescriptor* field) const;
    uint32_t GetUInt32(const Message& message,
        const FieldDescriptor* field) const;
    uint64_t GetUInt64(const Message& message,
        const FieldDescriptor* field) const;
    float GetFloat(const Message& message, const FieldDescriptor* field) const;
    double GetDouble(const Message& message, const FieldDescriptor* field) const;
    bool GetBool(const Message& message, const FieldDescriptor* field) const;
    std::string GetString(const Message& message, const FieldDescriptor* field) const;
    const EnumValueDescriptor* GetEnum(const Message& message,
        const FieldDescriptor* field) const;
    int GetEnumValue(const Message& message, const FieldDescriptor* field) const;
    const Message& GetMessage(const Message& message,
        const FieldDescriptor* field,
        MessageFactory* factory = nullptr) const;

    // Singular field mutators
    // These mutate the value of a non-repeated field.
    void SetInt32(Message* message, const FieldDescriptor* field,
        int32_t value) const;
    void SetInt64(Message* message, const FieldDescriptor* field,
        int64_t value) const;
    void SetUInt32(Message* message, const FieldDescriptor* field,
        uint32_t value) const;
    void SetUInt64(Message* message, const FieldDescriptor* field,
        uint64_t value) const;
    void SetFloat(Message* message, const FieldDescriptor* field,
        float value) const;
    void SetDouble(Message* message, const FieldDescriptor* field,
        double value) const;
    void SetBool(Message* message, const FieldDescriptor* field,
        bool value) const;
    void SetString(Message* message, const FieldDescriptor* field,
        std::string value) const;
    void SetEnum(Message* message, const FieldDescriptor* field,
        const EnumValueDescriptor* value) const;
    void SetEnumValue(Message* message, const FieldDescriptor* field,
        int value) const;
    Message* MutableMessage(Message* message, const FieldDescriptor* field,
        MessageFactory* factory = nullptr) const;
    PROTOBUF_NODISCARD Message* ReleaseMessage(
        Message* message, const FieldDescriptor* field,
        MessageFactory* factory = nullptr) const;

    // Repeated field getters
    // These get the value of one element of a repeated field.
    int32_t GetRepeatedInt32(const Message& message, const FieldDescriptor*
    field,
        int index) const;
    int64_t GetRepeatedInt64(const Message& message, const FieldDescriptor*
    field,
        int index) const;
    uint32_t GetRepeatedUInt32(const Message& message,
        const FieldDescriptor* field, int index) const;
    uint64_t GetRepeatedUInt64(const Message& message,
        const FieldDescriptor* field, int index) const;
    float GetRepeatedFloat(const Message& message, const FieldDescriptor* field,
        int index) const;
    double GetRepeatedDouble(const Message& message, const FieldDescriptor*
    field,
        int index) const;
    bool GetRepeatedBool(const Message& message, const FieldDescriptor* field,
        int index) const;
    std::string GetRepeatedString(const Message& message,
        const FieldDescriptor* field, int index) const;
    const EnumValueDescriptor* GetRepeatedEnum(const Message& message,
        const FieldDescriptor* field,
        int index) const;
    int GetRepeatedEnumValue(const Message& message, const FieldDescriptor*
    field,
        int index) const;
    const Message& GetRepeatedMessage(const Message& message, const FieldDescriptor* field,
        int index) const;
    const std::string& GetRepeatedStringReference(const Message& message,
        const FieldDescriptor* field,
        int index,
        std::string* scratch) const;

    // Repeated field mutators
    // These mutate the value of one element of a repeated field.
    void SetRepeatedInt32(Message* message, const FieldDescriptor* field,
        int index, int32_t value) const;
    void SetRepeatedInt64(Message* message, const FieldDescriptor* field,
        int index, int64_t value) const;
    void SetRepeatedUInt32(Message* message, const FieldDescriptor* field,
        int index, uint32_t value) const;
    void SetRepeatedUInt64(Message* message, const FieldDescriptor* field,
        int index, uint64_t value) const;
    void SetRepeatedFloat(Message* message, const FieldDescriptor* field,
        int index, float value) const;
    void SetRepeatedDouble(Message* message, const FieldDescriptor* field,
        int index, double value) const;
    void SetRepeatedBool(Message* message, const FieldDescriptor* field,
        int index, bool value) const;
    void SetRepeatedString(Message* message, const FieldDescriptor* field,
        int index, std::string value) const;
    void SetRepeatedEnum(Message* message, const FieldDescriptor* field,
        int index, const EnumValueDescriptor* value) const;
    void SetRepeatedEnumValue(Message* message, const FieldDescriptor* field,
        int index, int value) const;
    Message* MutableRepeatedMessage(Message* message,
        const FieldDescriptor* field,
        int index) const;

    // Repeated field adders
    // These add an element to a repeated field.
    void AddInt32(Message* message, const FieldDescriptor* field,
        int32_t value) const;
    void AddInt64(Message* message, const FieldDescriptor* field,
        int64_t value) const;
    void AddUInt32(Message* message, const FieldDescriptor* field,
        uint32_t value) const;
    void AddUInt64(Message* message, const FieldDescriptor* field,
        uint64_t value) const;
    void AddFloat(Message* message, const FieldDescriptor* field,
        float value) const;
    void AddDouble(Message* message, const FieldDescriptor* field,
        double value) const;
    void AddBool(Message* message, const FieldDescriptor* field,
        bool value) const;
    void AddString(Message* message, const FieldDescriptor* field,
        std::string value) const;
    void AddEnum(Message* message, const FieldDescriptor* field,
        const EnumValueDescriptor* value) const;
    void AddEnumValue(Message* message, const FieldDescriptor* field,
        int value) const;
    Message* AddMessage(Message* message, const FieldDescriptor* field,
        MessageFactory* factory = nullptr) const;

    const FieldDescriptor* FindKnownExtensionByName(
        const std::string& name) const;
    const FieldDescriptor* FindKnownExtensionByNumber(int number) const;
    bool SupportsUnknownEnumValues() const;
};
```

##### UnknownFieldSet 介绍（重要）

* UnknownFieldSet 包含在分析消息时遇到但未由其类型定义的所有字段。
* 若要将 UnknownFieldSet 附加到任何消息，请调用 `Reflection::GetUnknownFields()`。
* 类定义在`unknown_field_set.h`中。

```cpp
class PROTOBUF_EXPORT UnknownFieldSet {
    inline void Clear();
    void ClearAndFreeMemory();
    inline bool empty() const;
    inline int field_count() const;
    inline const UnknownField& field(int index) const;
    inline UnknownField* mutable_field(int index);

    // Adding fields
    void AddVarint(int number, uint64_t value);
    void AddFixed32(int number, uint32_t value);
    void AddFixed64(int number, uint64_t value);
    void AddLengthDelimited(int number, const std::string& value);
    std::string* AddLengthDelimited(int number);
    UnknownFieldSet* AddGroup(int number);

    // Parsing helpers
    // These work exactly like the similarly-named methods of Message.
    bool MergeFromCodedStream(io::CodedInputStream* input);
    bool ParseFromCodedStream(io::CodedInputStream* input);
    bool ParseFromZeroCopyStream(io::ZeroCopyInputStream* input);
    bool ParseFromArray(const void* data, int size);

    inline bool ParseFromString(const std::string& data) {
        return ParseFromArray(data.data(), static_cast<int>(data.size()));
    }

    // Serialization.
    bool SerializeToString(std::string* output) const;
    bool SerializeToCodedStream(io::CodedOutputStream* output) const;
    static const UnknownFieldSet& default_instance();
};
```

##### UnknownField 类介绍（重要）

* 表示未知字段集中的一个字段。
* 类定义在`unknown_field_set.h`中。

```cpp
class PROTOBUF_EXPORT UnknownField {
public:
    // 未知字段的类型枚举，对应Protobuf不同的字段编码类型
    enum Type {
        TYPE_VARINT,          // 可变长度整数类型（如int32/int64/uint32/uint64/bool/enum等）
        TYPE_FIXED32,         // 32位固定长度类型（如fixed32/float等）
        TYPE_FIXED64,         // 64位固定长度类型（如fixed64/double等）
        TYPE_LENGTH_DELIMITED,// 长度前缀类型（如string/bytes/嵌套message等）
        TYPE_GROUP            // 组类型（已废弃，仅兼容旧版Protobuf）
    };

    // 获取未知字段的编号（对应Proto中定义的字段号）
    inline int number() const;
    // 获取未知字段的类型（对应上述Type枚举）
    inline Type type() const;

    // Accessors - 每个方法仅适用于对应Type类型的UnknownField
    // 获取TYPE_VARINT类型字段的值
    inline uint64_t varint() const;
    // 获取TYPE_FIXED32类型字段的值
    inline uint32_t fixed32() const;
    // 获取TYPE_FIXED64类型字段的值
    inline uint64_t fixed64() const;
    // 获取TYPE_LENGTH_DELIMITED类型字段的值（只读）
    inline const std::string& length_delimited() const;
    // 获取TYPE_GROUP类型字段对应的未知字段集合（只读）
    inline const UnknownFieldSet& group() const;

    // Mutators - 设置对应Type类型字段的值
    // 设置TYPE_VARINT类型字段的值
    inline void set_varint(uint64_t value);
    // 设置TYPE_FIXED32类型字段的值
    inline void set_fixed32(uint32_t value);
    // 设置TYPE_FIXED64类型字段的值
    inline void set_fixed64(uint64_t value);
    // 设置TYPE_LENGTH_DELIMITED类型字段的值
    inline void set_length_delimited(const std::string& value);
    // 获取TYPE_LENGTH_DELIMITED类型字段的可变字符串指针（可直接修改值）
    inline std::string* mutable_length_delimited();
    // 获取TYPE_GROUP类型字段对应的可变未知字段集合指针（可直接修改）
    inline UnknownFieldSet* mutable_group();
};
```


修改client中的读取：

```cpp
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
```


---



### 选项 option

.proto 文件中可以声明许多选项，使用`option`标注。选项能影响 proto 编译器的某些处理方式。

#### 选项分类

选项的完整列表在`google/protobuf/descriptor.proto`中定义。部分代码：

```cpp
syntax = "proto2"; // descriptor.proto 使用 proto2 语法版本

message FileOptions { ... }       // 文件选项 定义在 FileOptions 消息中
message MessageOptions { ... }    // 消息类型选项 定义在 MessageOptions 消息中
message FieldOptions { ... }      // 消息字段选项 定义在 FieldOptions 消息中
message OneofOptions { ... }      // oneof字段选项 定义在 OneofOptions 消息中

message EnumOptions { ... }       // 枚举类型选项 定义在 EnumOptions 消息中
message EnumValueOptions { ... }  // 枚举值选项 定义在 EnumValueOptions 消息中
message ServiceOptions { ... }    // 服务选项 定义在 ServiceOptions 消息中
message MethodOptions { ... }     // 服务方法选项 定义在 MethodOptions 消息中
...
```

由此可见，选项分为 文件级、消息级、字段级 等等，但并没有一种选项能作用于所有的类型。

##### 常用选项列举

* **optimize\_for**：该选项为文件选项，可以设置 protoc 编译器的优化级别，分别为 `SPEED`、`CODE_SIZE`、`LITE_RUNTIME`。受该选项影响，设置不同的优化级别，编译.proto 文件后生成的代码内容不同。

  * `SPEED`：protoc 编译器将生成的代码是高度优化的，代码运行效率高，但是由此生成的代码编译后会占用更多的空间。`SPEED`是默认选项。
  * `CODE_SIZE`：proto 编译器将生成最少的类，会占用更少的空间，是依赖基于反射的代码来实现序列化、反序列化和各种其他操作。但和`SPEED`恰恰相反，它的代码运行效率较低。这种方式适合用在包含大量的.proto 文件，但并不盲目追求速度的应用中。
  * `LITE_RUNTIME`：生成的代码执行效率高，同时生成代码编译后的所占用的空间也是非常少。这是以牺牲 Protocol Buffer 提供的反射功能为代价的，仅仅提供 encoding + 序列化功能，所以在链接 BP 库时仅需链接`libprotobuf-lite`，而非`libprotobuf`。这种模式通常用于资源有限的平台，eg移动手机平台中。

  代码示例：

  ```proto
  option optimize_for = LITE_RUNTIME;
  ```
* **allow\_alias**：允许将相同的常量值分配给不同的枚举常量，用来定义别名。该选项为枚举选项。

  ```proto
  enum PhoneType {
    option allow_alias = true;
    MP = 0;
    TEL = 1;
    LANDLINE = 1; // 若不加 option allow_alias = true; 这一行会编译报错
  }
  ```

### 设置自定义选项

Protobuf 允许自定义选项并使用。该功能大部分场景用不到，在这里不拓展讲解。

有兴趣可以参考：[传送门](https://developers.google.cn/protocol-buffers/docs/proto?hl=zh-cn#customoptions)

---



### Json VS Protobuf


| 列化协议 | 通用性                                         | 格式       | 可读性                                 | 序列化大小                                       | 序列化性能 | 适用场景                                                                            |
| -------- | ---------------------------------------------- | ---------- | -------------------------------------- | ------------------------------------------------ | ---------- | ----------------------------------------------------------------------------------- |
| JSON     | 通用（json、xml 已成为多种行业标准的读写工具） | 文本格式   | 好                                     | 轻量（使用键值对方式，一定缩了一定的数据流空间） | 中         | web 项目。因为浏览器对于 json 数据支持非常好，有很多内建的函数支持。                |
| XML      | 通用                                           | 文本格式   | 好                                     | 重量（数据冗余，因为需要成对的闭合标签）         | 低         | XML 作为一种扩展标记语言，衍生出了 HTML、RDF/RDFS，它强调数据结构化的能力和可读性。 |
| ProtoBuf | 独立（ProtoBuf 只是 Google 公司内部的工具）    | 二进制格式 | 差（只能反序列化后得到真正可读的数据） | 轻量（比 JSON 更轻量，传输和速度会有优化）       | 高         | 适合高性能，对响应速度有要求的数据传输场景。Protobuf 比 XML、JSON 更小、更快。      |

小结：

1. XML、JSON、ProtoBuf 都具有数据结构化和数据序列化的能力。
2. XML、JSON 更注重数据结构化，关注可读性和语义表达能力。ProtoBuf 更注重数据序列化，关注效率、空间、速度，可读性差，语义表达能力不足，为保证极致的效率，会舍弃一部分元信息。
3. ProtoBuf 的应用场景更为明确，XML、JSON 的应用场景更为丰富。

---
