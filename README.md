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

- [x] basic GUI
- [x] tableView
- [x] export data as csv/excel/txt file
- [x] 'real' progress bar
- [x] automatically ignore invalid input under TCP mode
- [ ] query continous address
- [x] catch the case when server actively terminate the connection
- [ ] modelize the code
- [ ] release (.dmg/.exe)
- [ ] ...

5.TEST
- [x] simulation - (utilized [unslave.exe](https://unserver.xyz/docs/unslave/#tcp-section) installed on a Windows PC)
- [x] on real device


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

## Demonstration

should be a video here

## Developing Environment

* MacOS Ventura 13.1
* Qt 6.2.3
* QtCreator 7.0.2

## To contribute

1. fork the repo to your local repo
2. write/change something in the local repo
3. back to the original repo and raise the Pull Request

👍 any contribution is welcome!
