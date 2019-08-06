#include <QCoreApplication>
#include "fsm.h"
#include "state.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

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
//    myFSM.print();
    myFSM.generateVerilog();

    return a.exec();
}
