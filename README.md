# Modbus Client App
--- 
a cross-platform implementation for modbus client used for testing modbus server

Given that there is currently no open-source & cross-platform Modbus client software available online, I plan to implement one with minimal dependencies (Qt library or QSerialport module).

## Developing Process

1.FUNCTION
- [x] construct modbus frame
- [x] send data
- [x] receive data
- [ ] parse and analyze received data
- [ ] handle the case when server actively terminate the connection

2.MODE

- [x] RTU-serial
- [ ] ASCII-serial
- [x] TCP

3.FEATURES

- [x] basic GUI
- [x] tableView
- [x] export data as csv/excel/txt file
- [ ] progress bar
- [ ] query continous address
- [ ] modelize the code
- [ ] release (.dmg/.exe)
- [ ] ...

4.TEST
- [ ] unit test
- [ ] simulation
- [ ] real device


## Perequisites

```c++
void prerequisites(){
    download and install QtCreator; 
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

```c++
void use(){
    double click the ModbusQT.pro file;
    click the run button;
}
```

You can also package the project into an executable file (which is easy using QT Creator) and run it directly.

## Developing Environment

* MacOS 13.1
* Qt 6.2.3
* QtCreator 7.0.2

## To contribute

1. fork the repo to your local repo
2. write/change something in the local repo 
3. back to the original repo and send the Pull Request

👍 any contribution is welcome :)
