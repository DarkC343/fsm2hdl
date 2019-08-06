# Introduction
<p align="center"><img src="https://drive.google.com/uc?export=view&id=1GkYwBsoyH1tT6OgE6Rtti-d0xwxdJqNE"></p>
<br />

**fsm2hdl** is a free tool for making auto-generated Verilog HDL sources from FSM desgins. This tool is written in Qt5/C++. Furthur inhancements like visual diagram editor will be added to the project soon. Currently, this version is a console-based application.

## Building
### Linux
#### CMake
```
git clone https://github.com/DarkC343/fsm2hdl.git
cd fsm2hdl
mkdir build
cd build
cmake ..
make -j && make install
```

### Windows
#### Qt Creator >= 5
Open `CMakeLists.txt` file in Qt creator. Select your desired kit and do `Build > Run CMake` and then `Build > Run`

## Example
<p align="center" width="600" height="556"><img src="https://drive.google.com/uc?export=view&id=1oA-CmCuiiuZ8iAPdz9JljpAkYIQVmarQ"></p>

```
FSM::setInputSize(2);
FSM::setOutputSize(1);

State s0("S0");
State s1("S1");
State s2("S2");
State s3("S3");
State s4("S4");

s0.addTransition(&s0, "00", "0");
s0.addTransition(&s1, "10", "1");
s0.addTransition(&s2, "11", "1");
s0.addTransition(&s4, "01", "1");

s1.addTransition(&s0, "00", "0");
s1.addTransition(&s1, "x1", "0");
s1.addTransition(&s2, "10", "0");

s2.addTransition(&s1, "0x", "0");
s2.addTransition(&s3, "1x", "1");

s3.addTransition(&s3, "x0", "1");
s3.addTransition(&s4, "x1", "1");

s4.addTransition(&s4, "11", "1");
s4.addTransition(&s0, "10", "1");
s4.addTransition(&s0, "0x", "0");

QList<State *> entries = { &s0, &s1, &s2, &s3, &s4 };
FSM myFSM(&entries, &s0, RisingEdge, FallingEdge, Asynchronous, Nanoseconds, Picoseconds);
myFSM.generateVerilog();
```
Then you have `fsm2hdl.v` file in your build directory.
