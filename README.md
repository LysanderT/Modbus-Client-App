# Modbus Client App
--- 
Modbus Client App is a robust and cross-platform implementation for modbus master to test modbus server.

Given that there is currently no open-source & cross-platform Modbus client software with GUI available online (either not free or can only be running on Windows), I implemented one with minimal dependencies (Qt library).

## Developing Process

1.FUNCTION
- [x] construct modbus frame
- [x] send data
- [x] determine the time interval of sending data
- [x] receive data
- [x] parse and analyze the received data

2.MODE

- [x] RTU-serial
- [ ] ASCII-serial
- [x] TCP

3.SUPPORTED FUNCTION CODE
- [x] 0x01
- [x] 0x02
- [x] 0x03
- [x] 0x04
- [ ] 0x05
- [ ] 0x06
- ...

4.FEATURES

- [x] export data as csv/excel/txt/xml file
    - [x] csv
    - [x] excel
    - [x] txt
    - [ ] xml
- [x] 'real' progress bar
- [x] automatically ignore invalid input under TCP mode
- [x] catch the case when server actively terminate the connection
- [ ] ...

5.BUILD
- [x] qmake
- [x] cmake

6.TEST
- [x] simulation - (utilized [unslave.exe](https://unserver.xyz/docs/unslave/#tcp-section) installed on a Windows PC)
- [x] on real device

7.TODO
- [x] decoupling using MVC design pattern (table/model/**delegate** in qt)
- [x] modelize the code
- [x] page the setup widget according to the mode
- [x] minimize the cell width to make the window look better
- [ ] merge the successive query to speed up
- [ ] use multitread programming to speed up
- [ ] add write mode
- [ ] 16-bit register read/write (process data)
- [ ] page the data according to different display order or [sorting](https://blog.51cto.com/quantfabric/1879125)
- [ ] add Demonstration(gif/video) to readme
- [ ] release (.dmg/.exe/.deb/.rpm) using CMake



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

## Demonstration

should be a video or gif here

## Developing Environment

* MacOS Ventura 13.1
* Qt 6.2.3
* QtCreator 7.0.2
* CMake 3.26.4

## To contribute

1. fork the repo to your local repo
2. write/change something in the local repo
3. back to the original repo and raise the Pull Request

👍 any contribution is welcome!


[//]: 能否在click+/-button时知道自己的index？？？
[//]: 可以在tableview发出信号clicked(index)的时候，用index.col()确定是否是-/+号，用index.row()来判断删/增哪一行
[//]: 加号就放最下面吧，省力。。。。
