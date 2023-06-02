# Modbus Client App
--- 
a cross-platform implementation for modbus client used for testing modbus server

## Developing Environment

* MacOS 13.1
* Qt 6.2.3
* QtCreator 7.0.2


## Developing Process

1.FUNCTION
- [x] construct modbus frame
- [x] send data
- [x] receive data
- [ ] parse received data

2.MODE

- [x] RTU
- [ ] ASCII
- [ ] TCP

3.FEATURES

- [x] GUI
- [x] export data as csv/excel/txt file
- [ ] ...



## Perequisites

```c++
void prerequisites(){
    download and install QtCreator; 
    // not necessarily include QSerialPort module
    if(the version u chose does not include QSerialPort module){
        find QT maintenance tool in the Qt folder;
        use the tool to add the QSerialPort module;
    }
}
```

## Usage

```c++
void use(){
    double click the ModbusQT.pro file;
    click the run button;
}