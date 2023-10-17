# Modbus Client
--- 
Modbus Client is a robust, cross-platform and elegant implementation for modbus master to test modbus server.

Given that there is currently no open-source & cross-platform Modbus client software with GUI available online (either not free or <.exe> type that can only be running on Windows), I implemented one with minimal dependencies (Qt library).

## Table of Content

- [Modbus Client](#modbus-client)
  - [Table of Content](#table-of-content)
  - [Developing Process](#developing-process)
  - [Perequisites](#perequisites)
  - [Usage](#usage)
    - [Float32](#float32)
  - [Demonstration](#demonstration)
  - [Developing Environment](#developing-environment)
  - [To contribute](#to-contribute)

## Developing Process

1.FUNCTION
- [x] construct modbus frame
- [x] send data
- [x] determine the time interval of sending data
- [x] receive data
- [x] parse and analyze the received data

2.MODE

- [x] RTU-serial
- [x] TCP

3.SUPPORTED FUNCTION CODE
- [x] 0x01
- [x] 0x02
- [x] 0x03
- [x] 0x04
- [x] 0x05
- [x] 0x06
- ...

4.FEATURES

- [x] export data as csv/excel/txt/xml file
    - [x] csv
    - [x] excel
    - [x] txt
    - [x] xml
- [x] 'real' progress bar
- [x] catch the case when server actively terminate the TCP connection
- [x] clearable send/read logs
- [x] interactively sorting tableview
- [x] Automatically detects how the client data is stored (little/big endianness)
- [x] Choose how the server stores float32 data (little/big endianness)
- [ ] ...

5.BUILD
- [x] qmake
- [x] cmake

6.SUPPORTED DATA TYPE
- [x] int
- [x] bool
- [x] float32

7.TEST
- [x] simulation - (utilized [unslave.exe](https://unserver.xyz/docs/unslave/#tcp-section) installed on a Windows PC)
- [x] on real device

8.OPTIMIZATION
- [x] Decoupling using MVC design pattern (table/model/**delegate** in qt)
- [x] Modelize the code
- [x] Page the setup widget according to the mode
- [x] Minimize the cell width to make the window look better
- [x] Merge multiple successive single reading query into one
- [x] Use the Dom Tree to generate xml file

9.TODO
- [x] 16/32/64-bit register read/write (process data)
- [x] page the data according to different display order or sorting
- [x] export table data as .xml file
- [x] process float32
- [x] identify the os/compiler using macro to make it compatible
- [ ] add Demonstration(gif/video) to readme

## Perequisites

```c++
void prerequisites(){
    download and install the QtCreator; 
    // high-version qt may not have QSerialPort module installed
    if(the version u chose does not include QSerialPort module){
        find QT maintenance tool in the Qt folder;
        use the tool to add the QSerialPort module;
    }
}
```

You can also use other IDEs like VSCode, but you need to configure the environment by yourself.

For VSCode, you can refer to [this](https://www.cnblogs.com/zhiyiYo/p/14877952.html) to configure the environment.

## Usage

```
git clone -b main https://github.com/LysanderT/Modbus-Client-App ;
```

```c++
void run_with_qmake(){
    double click the ModbusQT.pro file in the Modbus-Client-App folder (which will open the project in Qt Creator);

    click the run button in the bottom left corner of Qt Creator;
}

void run_with_cmake(){
    double click the CMakeLists.txt file in the Modbus-Client-App folder (which will open the project in Qt Creator)
    
    click the run button in the bottom left corner of Qt Creator;
}
```

You can also package the project into an executable file (which is easy using QT Creator) and run it directly.

### Float32

if you want to use float32 data type, you should specify the endianness of the slave (little-edian by default, the endianness of the master if automatically specified).

if you don't know the endianness of the slave, the following code may help.

```
bool IsBigEndian()
{
　　int a = 0x1234;
　　char b = *(char *)&a;
　　if( b == 0x12)
　　{
　　　　return true;
　　}
　　return false;
}
```

to use this data type, the register should be at least 32 bit.

## Demonstration

should be a video or .gif here

## Developing Environment

* MacOS Ventura 13.1
* Qt 6.2.3
* QtCreator 7.0.2
* CMake 3.26.4

## To contribute

1. fork the repo to your local repo
2. write/change something in the local repo
3. back to the original repo and raise Pull Request

👍 any contribution is welcome!
