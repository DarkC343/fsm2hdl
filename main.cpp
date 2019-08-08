#include <QCoreApplication>
#include "mealyfsm.h"
#include "moorefsm.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Mealy FSM example #1
    MealyFSM myMealyFSM;

    myMealyFSM.setInputSize(2);
    myMealyFSM.setOutputSize(1);

    myMealyFSM.addState("S0");
    myMealyFSM.addState("S1");
    myMealyFSM.addState("S2");
    myMealyFSM.addState("S3");
    myMealyFSM.addState("S4");

    myMealyFSM.connect("S0", "S0", "00", "0");
    myMealyFSM.connect("S0", "S1", "10", "1");
    myMealyFSM.connect("S0", "S2", "11", "1");
    myMealyFSM.connect("S0", "S4", "01", "1");

    myMealyFSM.connect("S1", "S0", "00", "0");
    myMealyFSM.connect("S1", "S1", "x1", "0");
    myMealyFSM.connect("S1", "S2", "10", "0");

    myMealyFSM.connect("S2", "S1", "0x", "0");
    myMealyFSM.connect("S2", "S3", "1x", "1");

    myMealyFSM.connect("S3", "S3", "x0", "1");
    myMealyFSM.connect("S3", "S4", "x1", "1");

    myMealyFSM.connect("S4", "S4", "11", "1");
    myMealyFSM.connect("S4", "S0", "10", "1");
    myMealyFSM.connect("S4", "S0", "0x", "0");

    myMealyFSM.setFailSafe("S0", "0");
    myMealyFSM.setCodeSettings(RisingEdge, FallingEdge, Asynchronous, Nanoseconds, Picoseconds);
    myMealyFSM.setSignalNames("in", "out", "clk", "rst", "curr_s", "next_s");
    myMealyFSM.setModuleName("fsm2hdl_mealy");
    myMealyFSM.generateVerilog();

    // Moore FSM example #1
    MooreFSM myMooreFSM;

    myMooreFSM.setInputSize(1);
    myMooreFSM.setOutputSize(1);

    myMooreFSM.addState("S0", "0");
    myMooreFSM.addState("S1", "0");
    myMooreFSM.addState("S11", "0");
    myMooreFSM.addState("S110", "0");
    myMooreFSM.addState("S1101", "1");

    myMooreFSM.connect("S0", "S0", "0");
    myMooreFSM.connect("S0", "S1", "1");

    myMooreFSM.connect("S1", "S0", "0");
    myMooreFSM.connect("S1", "S11", "1");


    myMooreFSM.connect("S11", "S110", "0");
    myMooreFSM.connect("S11", "S11", "1");

    myMooreFSM.connect("S110", "S0", "0");
    myMooreFSM.connect("S110", "S1101", "1");

    myMooreFSM.connect("S1101", "S0", "0");
    myMooreFSM.connect("S1101", "S11", "1");

    myMooreFSM.setFailSafe("S0");
    myMooreFSM.setCodeSettings(RisingEdge, FallingEdge, Asynchronous, Nanoseconds, Picoseconds);
    myMooreFSM.setSignalNames("in", "out", "clk", "rst", "curr_s", "next_s");
    myMooreFSM.setModuleName("fsm2hdl_moore");
    myMooreFSM.generateVerilog();

    return a.exec();
}
