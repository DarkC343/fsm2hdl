#include "fsm.h"
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QLinkedList>
#include <QtMath>
#include <QDate>

size_t FSM::m_inputSize = 0;
size_t FSM::m_outputSize = 0;

static const QString &moduleName = "fsm2hdl";
static const QString &inputSignalName = "in";
static const QString &outputSignalName = "out";
static const QString &resetSignalName = "rst";
static const QString &clockSignalName = "clk";
static const QString &currentStateRegisterName = "curr_s";
static const QString &nextStateRegisterName = "next_s";

size_t FSM::inputSize()
{
    return m_inputSize;
}

void FSM::setInputSize(size_t inputSize)
{
    m_inputSize = inputSize;
}

size_t FSM::outputSize()
{
    return m_outputSize;
}

void FSM::setOutputSize(size_t outputSize)
{
    m_outputSize = outputSize;
}

FSM::FSM(QList<State *> *entries, State *failSafe, EdgeEnum clockEdge, EdgeEnum resetEdge, ResetModeEnum resetMode, UnitEnum timeUnit, UnitEnum timePrecisionUnit, QObject *parent) : QObject(parent), m_failSafe(failSafe), m_entries(entries), m_clockEgde(clockEdge), m_resetEdge(resetEdge), m_resetMode(resetMode), m_timeUnit(timeUnit), m_timePrecisionUnit(timePrecisionUnit)
{
}

void FSM::setInitialState(State *failSafe)
{
    m_failSafe = failSafe;
}

QString FSM::print()
{
    QString result = "";
    for(auto s : *m_entries)
    {
        QHash<QPair<QString, QString>, State *>::iterator i;
        auto q = s->connections();
        for(i = q.begin(); i != q.end(); ++i)
        {
            result += "//\t* State: " + QString(s->name()) + " is connected to " + i.value()->name() + ", Input: " + i.key().first + ", Output: " + i.key().second + "\n";
        }
    }
    return result;
}

void FSM::generateVerilog()
{
    QFile file(moduleName + ".v");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);

    QString timeUnit;
    QString precisionUnit;
    switch (m_timeUnit) {
        case Seconds: timeUnit = "s"; break;
        case Milliseconds: timeUnit = "ms"; break;
        case Microseconds: timeUnit = "us"; break;
        case Nanoseconds: timeUnit = "ns"; break;
        case Picoseconds: timeUnit = "ps"; break;
        case Femtoseconds: timeUnit = "fs"; break;
    }

    switch (m_timePrecisionUnit) {
        case Seconds: precisionUnit = "s"; break;
        case Milliseconds: precisionUnit = "ms"; break;
        case Microseconds: precisionUnit = "us"; break;
        case Nanoseconds: precisionUnit = "ns"; break;
        case Picoseconds: precisionUnit = "ps"; break;
        case Femtoseconds: precisionUnit = "fs"; break;
    }

    // author comments
    out << "////////////////////////////////////////////////////////////////////////////////\n";
    out << "// File	\t\t\t: fsm2hdl.v\n";
    out << "// Author\t\t\t: Amir Mohammad Biuki (DarkC343)\n";
    out << "// First release\t: August 6, 2019\n";
    out << "// Date\t\t\t\t: " << QDate::currentDate().toString() << "\n";
    out << "// Source link\t\t: https://github.com/DarkC343/fsm2hdl\n";
    out << "// University\t\t: Shahed (shahed.ac.ir)\n";
    out << "// Email\t\t\t: dark.c343@gmail.com\n";
    out << "// This verilog module is a auto-generated mealy Finite State Machine using\n";
    out << "// 'fsm2hdl' tool. This tool is currently under development using Qt/C++.\n";
    out << "// Read README.me for installation. New features will be added soon!\n";
    out << "// ------------------------------------------------ \n";
    out << print();
    out << "////////////////////////////////////////////////////////////////////////////////\n\n";

    // directives
    out << "`timescale 1" << timeUnit << " / 1" << precisionUnit << "\n";
    out << "\n";
    // module header
    out << "module " << moduleName << "(" << resetSignalName << ", " << clockSignalName << ", " << inputSignalName << ", " << outputSignalName << ");\n";

    // signals
    if(FSM::inputSize() > 1)
    {
        out << "\tinput [" << FSM::inputSize()-1 << ":0] " << inputSignalName << ";\n";
    }
    else
    {
        out << "\tinput " << inputSignalName << ";\n";
    }
    out << "\tinput " << resetSignalName << ", " << clockSignalName << ";\n";
    if(FSM::outputSize() > 1)
    {
        out << "\toutput reg [" << FSM::outputSize()-1 << ":0] " << inputSignalName << ";\n";
    }
    else
    {
        out << "\toutput reg " << outputSignalName << ";\n";
    }
    out << "\t\n";
    int stateNumbersLog2 = qCeil(qLn(m_entries->size())/qLn(2));
    out << "\treg [" << QString::number(stateNumbersLog2-1) << ":0] " << currentStateRegisterName << ", " << nextStateRegisterName << ";\n";
    int stateNumber = 0;
    for(auto state : *m_entries)
    {
        out << "\tparameter " << state->name() << " = " << stateNumbersLog2 << "'b" << QString("%1").arg(stateNumber, stateNumbersLog2, 2, QChar('0')) << ";\n";
        stateNumber++;
    }
    out << "\n";

    // state register
    out << "\talways @(posedge " << clockSignalName << " or negedge " << resetSignalName << ")\n";
    out << "\t\tif(" << resetSignalName << " == 1'b0)\n";
    out << "\t\t\t" << currentStateRegisterName << " <= " << m_failSafe->name() << ";\n";
    out << "\t\telse\n";
    out << "\t\t\t" << currentStateRegisterName << " <= " << nextStateRegisterName << ";\n";
    out << "\t\n";
    out << "\talways @(" << inputSignalName << " or " << currentStateRegisterName << ")\n";
    out << "\t\tcase(" << currentStateRegisterName << ")\n";

    // next state combinational logic
    for(auto curr_s : *m_entries)
    {
        out << "\t\t\t" << curr_s->name() << ":\n";
        out << "\t\t\t\tcase(" << inputSignalName << ")\n";
        auto connections = curr_s->connections();
        //next_states
        for(auto next_s = connections.begin(); next_s != connections.end(); ++next_s)
        {
            if(next_s.key().first.toLower().contains('x'))
            {
                QLinkedList<QString> list;
                list.push_back(next_s.key().first.toLower());
                for(auto state : list)
                {
                    while(state.contains('x'))
                    {
                        int i = state.indexOf('x');
                        auto stateOne = state; stateOne[i] = '1';
                        auto stateTwo = state; stateTwo[i] = '0';
                        list.removeOne(state);
                        list.push_back(stateOne); list.push_back(stateTwo);
                        break;
                    }
                }
                QString states;
                for(auto state : list)
                {
                    states += QString::number(FSM::inputSize()) + "'b" + state + ", ";
                }
                states.remove(states.size()-2,2);
                out << "\t\t\t\t\t" << states << ": " << nextStateRegisterName << " = " << next_s.value()->name() << ";\n";
            }
            else
            {
                out << "\t\t\t\t\t" << FSM::inputSize() << "'b" << next_s.key().first.toLower() << ": " << nextStateRegisterName << " = " << next_s.value()->name() << ";\n";
            }
        }
        out << "\t\t\t\tendcase\n";
    }
    out << "\t\t\tdefault:\n";
    out << "\t\t\t\t" << nextStateRegisterName << " = " << m_failSafe->name() << ";\n";
    out << "\t\tendcase\n";
    out << "\n";

    // output combinational logic
    out << "\talways @(in or " << currentStateRegisterName << ")\n";
    out << "\t\tcase(" << currentStateRegisterName << ")\n";
    for(auto curr_s : *m_entries)
    {
        out << "\t\t\t" << curr_s->name() << ":";
        QString differentOutputsCode = "";
        auto connections = curr_s->connections();
        int zerosCount = 0, onesCount = 0;
        for(auto next_s = connections.begin(); next_s != connections.end(); ++next_s)
        {
            if(next_s.key().second == "0") zerosCount++;
            else if(next_s.key().second == "1") onesCount++;
            if(next_s.key().first.toLower().contains('x'))
            {
                QLinkedList<QString> list;
                list.push_back(next_s.key().first.toLower());
                for(auto state : list)
                {
                    while(state.contains('x'))
                    {
                        int i = state.indexOf('x');
                        auto stateOne = state; stateOne[i] = '1';
                        auto stateTwo = state; stateTwo[i] = '0';
                        list.removeOne(state);
                        list.push_back(stateOne); list.push_back(stateTwo);
                        break;
                    }
                }
                QString states;
                for(auto state : list)
                {
                    states += QString::number(FSM::inputSize()) + "'b" + state + ", ";
                }
                states.remove(states.size()-2,2);
                differentOutputsCode += "\t\t\t\t\t" + states + ": " + outputSignalName + " = " + QString::number(FSM::outputSize()) + "'b" + next_s.key().second + ";\n";
            }
            else
            {
                differentOutputsCode += "\t\t\t\t\t" + QString::number(FSM::inputSize()) + "'b" + next_s.key().first.toLower() + ": " + outputSignalName + " = " + QString::number(FSM::outputSize()) + "'b" + next_s.key().second + ";\n";
            }
        }
        if(zerosCount == 0) out << " " << outputSignalName << " = 1'b1;\n";
        else if(onesCount == 0) out << " " << outputSignalName << " = 1'b0;\n";
        else
        {
            out << "\n\t\t\t\tcase(" << inputSignalName << ")\n";
            out << differentOutputsCode;
            out << "\t\t\t\tendcase\n";
        }
    }
    out << "\t\t\tdefault: ";
    out << outputSignalName << " = " << m_failSafe->name() << ";\n";
    out << "\t\tendcase\n";

    // end of module
    out << "endmodule\n";
    out << "\n";
}
