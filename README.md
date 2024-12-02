### 项目介绍

本项目对比了四种开源的配置管理理服务BoostPropertyTree、Inih、JsonCpp、LibConfig的各自的特点，以及一些性能消耗。
配置管理服务：项目配置服务能帮助我们在不同的开发、测试和生产环境中保持一致的配置，让我们的项目能够动态的变化项目配置，而不用去手动的修改代码，现在网上有很多开源的配置管理服务，比较出名的有BoostPropertyTree、Inih、JsonCpp、LibConfig，下面是这四个配置服务的各自特点：

**Boost.PropertyTree**：Boost.PropertyTree是C++Boost库的一部分，它提供了一种通用的数据结构来表示层次化的键值对数据。它可以解析和生成多种格式的数据文件，如INI、JSON和XML。使用PropertyTree可以轻松地将配置数据从文件加载到内存中，并且可以方便地进行查询和更新。
**Inih**：Inih是一个轻量级的INI文件解析库，专注于处理简单的配置文件。INI文件是一种常见的文本文件格式，用于存储结构化数据，特别是配置信息。Inih提供了简单易用的API来读取和写入INI文件，非常适合那些不需要复杂功能的应用场景。
**JsonCpp**：JsonCpp是一个用于处理JSON数据的C++库。JSON是一种轻量级的数据交换格式，易于人阅读和编写，同时也易于机器解析和生成。JsonCpp支持JSON数据的序列化和反序列化，使得应用程序能够方便地与支持JSON格式的其他系统或服务进行交互。对于需要处理JSON配置文件的应用来说，JsonCpp是一个很好的选择。
**LibConfig**：LibConfig是一个用于处理配置文件的C/C++库，它支持一种类似于JSON的格式，但又具有自己的特点。这种格式比纯文本格式更结构化，比XML更简洁。LibConfig库提供了强大的API来解析和生成配置文件，适用于需要高效处理复杂配置信息的应用。

### 功能对比

| 配置库  支持的数据格式           | 跨平台支持                   | 依赖的库 | 编译是否方便                   |
|------------------------|-------------------------|------|--------------------------|
|     Boost.PropertyTree | 支持XML、JSON、INI和INFO文件格式 | 是    | Boost.Spirit、Boost.Regex | 不太方便 |
|     Jsoncpp            | JSON文件                  | 是    | 无其他依赖的库                  | 方便   |
|     Inih               | INI文件                   | 是    | 无其他依赖的库                  | 方便   |
|     Libconfig          | 支持结构化、层次化的配置（.cfg）文件    | 是    | 有基本的依赖                   | 方便   |

---

### 环境依赖：

```
//安装Boost.PropertyTree相关的依赖
sudo apt-get install libboost-all-dev

// 安装JasonCpp所需的依赖：
sudo apt-get install libjsoncpp-dev

// 安装LibConfig所需的依赖
sudo apt-get install libconfig++-dev

//Inih不需要其他的依赖，其源代码在Inih/lib下
```

### 构建方式

```
本项目的构建方式如下：
├── Readme.md                   
├── CMakeLists                        
├── BoostPropertyTree                      
│   ├── config.json
│   ├── BoostPropertyTree.cpp                
│   └── CMakeLists                
├── JsonCpp                      
│   ├── config.json
│   ├── JsonCpp.cpp                
│   └── CMakeLists                 
├── Inih                      
│   ├── lib
        └── init.h
│   ├── config.ini
│   ├── Inih.cpp               
│   └── CMakeLists                 
└── LibConfig                      
    ├── config.cfg
    ├── LibConfig.cpp                
    └──CMakeLists
```

### 测试环境

```
    操作系统 ： Ubuntu 20.04
    Cmake 版本 ： 3.16.3
    编译器 ： GCC 9.4.0
```

### 编译方式

```
    mkdir build
    cd build
    cmake ..
    make
```

完成之后编译好的文件在build目录下的各个分目录下

### 性能对比



本项目比较了这四个配置管理的在运行时的性能指标，我们对比了这四个库在加载数据时的响应时间，编译后的二进制大小，运行时的内存占用，比较结果如以下：

| 性能指标          | Boost.PropertyTree | Jsoncpp | Inih  | Libconfig |
|---------------|--------------------|---------|-------|-----------|
|     响应时间（ms）  | 155                | 90      | 64    | 54        |
|     编译后的二进制大小 | 1.5M               | 151K    | 312K  | 55K       |
|     内存占用      | 6028K              | 5912K   | 6104K | 5988K     |



