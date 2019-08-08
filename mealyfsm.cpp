#include "mealyfsm.h"
#include <QRegularExpression>
#include <QFile>
#include <QTextStream>
#include <QDate>
#include <QtMath>
#include <QLinkedList>

MealyFSM::MealyFSM() : FSM(nullptr)
{

}

void MealyFSM::addState(const QString &name)
{
    bool isValid = true;
    for(auto state : m_states)
    {
        if(state->name() == name)
        {
            isValid = false;
            break;
        }
    }
#ifdef QT_DEBUG
    Q_ASSERT(isValid);
#else
    if(!isValid){
        return;
    }
#endif
    m_states << new MealyState(name);
}

void MealyFSM::setFailSafe(const QString &name, const QString &defaultOutput)
{
    static QRegularExpression invalid("[^01xX]");
    bool isValid = false;
    bool isValidFrom = false;
    bool isValidOutput = false;
    for(auto state : m_states)
    {
        if(state->name() == name)
        {
            isValidFrom = true;
            break;
        }
    }
    if(defaultOutput.length() == int(getOutputSize()) && !defaultOutput.contains(invalid)) isValidOutput = true;
    isValid = isValidFrom & isValidOutput;
#ifdef QT_DEBUG
    Q_ASSERT(isValid);
#else
    if(!isValid){
        return;
    }
#endif
    m_failSafeName = name;
    m_failSafeOutput = defaultOutput;
}

void MealyFSM::connect(const QString &fromeSate, const QString &toState, const QString &input, const QString &output)
{
    static QRegularExpression invalid("[^01xX]");
    bool isValidFrom = false;
    bool isValidTo = false;
    bool isValidInput = false;
    bool isValidOutput = false;
    bool isValid = false;
    for(auto state : m_states)
    {
        if(state->name() == fromeSate)
        {
            isValidFrom = true;
        }
        if(state->name() == toState)
        {
            isValidTo = true;
        }
    }
    if(input.length() == int(getInputSize()) && !input.contains(invalid)) isValidInput = true;
    if(output.length() == int(getOutputSize()) && !output.contains(invalid)) isValidOutput = true;
    isValid = isValidFrom & isValidTo & isValidInput & isValidOutput;
#ifdef QT_DEBUG
    Q_ASSERT(isValid);
#else
    if(!isValid){
        return;
    }
#endif
    auto fromPtr = std::find_if(m_states.begin(), m_states.end(), [fromeSate](MealyState* ms) { return ms->name() == fromeSate; });
    auto toPtr = std::find_if(m_states.begin(), m_states.end(), [toState](MealyState* ms) { return ms->name() == toState; });
    m_connections.insert(qMakePair(fromPtr.i->t(), toPtr.i->t()), qMakePair(input, output));
}

void MealyFSM::generateVerilog()
{
    QFile file(getModuleName() + ".v");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream verilog(&file);

    // author comments
    verilog << "////////////////////////////////////////////////////////////////////////////////\n";
    verilog << "// File	\t\t\t: fsm2hdl.v\n";
    verilog << "// Author\t\t\t: Amir Mohammad Biuki (DarkC343)\n";
    verilog << "// First release\t: August 6, 2019\n";
    verilog << "// Date\t\t\t\t: " << QDate::currentDate().toString() << "\n";
    verilog << "// Source link\t\t: https://github.com/DarkC343/fsm2hdl\n";
    verilog << "// University\t\t: Shahed (shahed.ac.ir)\n";
    verilog << "// Email\t\t\t: dark.c343@gmail.com\n";
    verilog << "// This verilog module is a auto-generated Mealy Finite State Machine using\n";
    verilog << "// 'fsm2hdl' tool. This tool is currently under development using Qt/C++.\n";
    verilog << "// Read README.me for installation. New features will be added soon!\n";
    verilog << "// ------------------------------------------------ \n";
    verilog << printDetails();
    verilog << "// ------------------------------------------------ \n";
    verilog << printConnections();
    verilog << "////////////////////////////////////////////////////////////////////////////////\n\n";

    // directives
    verilog << "`timescale 1" << getTimeUnit() << " / 1" << getTimePrecisionUnit() << "\n";
    verilog << "\n";
    // module header
    verilog << "module " << getModuleName() << "(" << getResetSignalName() << ", " << getClockSignalName() << ", " << getInputSignalName() << ", " << getOutputSignalName() << ");\n";

    // signals
    if(getInputSize() > 1)
    {
        verilog << "\tinput [" << getInputSize()-1 << ":0] " << getInputSignalName() << ";\n";
    }
    else
    {
        verilog << "\tinput " << getInputSignalName() << ";\n";
    }
    verilog << "\tinput " << getResetSignalName() << ", " << getClockSignalName() << ";\n";
    if(getOutputSize() > 1)
    {
        verilog << "\toutput reg [" << getOutputSize()-1 << ":0] " << getOutputSignalName() << ";\n";
    }
    else
    {
        verilog << "\toutput reg " << getOutputSignalName() << ";\n";
    }
    verilog << "\t\n";
    int stateNumbersLog2 = qCeil(qLn(m_states.size())/qLn(2));
    verilog << "\treg [" << QString::number(stateNumbersLog2-1) << ":0] " << getCurrentStateRegisterName() << ", " << getNextStateRegisterName() << ";\n";
    int stateNumber = 0;
    for(auto state : m_states)
    {
        verilog << "\tparameter " << state->name() << " = " << stateNumbersLog2 << "'b" << QString("%1").arg(stateNumber, stateNumbersLog2, 2, QChar('0')) << ";\n";
        stateNumber++;
    }
    verilog << "\n";

    // state register
    verilog << "\talways @(" << (getClockEgde() == RisingEdge ? "posedge" : "negedge") << " " << getClockSignalName() << ((getResetMode() == Asynchronous) ? (QString(" or ") + (getResetEdge() == RisingEdge ? "posedge" : "negedge") + " " + getResetSignalName()) : "") << ")\n";
    verilog << "\t\tif(" << getResetSignalName() << " == 1'b" << (getResetEdge() == RisingEdge ? "1" : "0") << ")\n";
    verilog << "\t\t\t" << getCurrentStateRegisterName() << " <= " << m_failSafeName << ";\n";
    verilog << "\t\telse\n";
    verilog << "\t\t\t" << getCurrentStateRegisterName() << " <= " << getNextStateRegisterName() << ";\n";
    verilog << "\t\n";
    verilog << "\talways @(" << getInputSignalName() << " or " << getCurrentStateRegisterName() << ")\n";
    verilog << "\t\tcase(" << getCurrentStateRegisterName() << ")\n";

    // next state combinational logic
    for(auto state : m_states)
    {
        verilog << "\t\t\t" << state->name() << ":\n";
        verilog << "\t\t\t\tcase(" << getInputSignalName() << ")\n";
        //next_states
        for(auto connection = m_connections.begin(); connection != m_connections.end(); ++connection)
        {
            if(connection.key().first != state) continue;
            if(connection.value().first.toLower().contains('x'))
            {
                QLinkedList<QString> list;
                list.push_back(connection.value().first.toLower());
                for(auto value : list)
                {
                    while(value.contains('x'))
                    {
                        int i = value.indexOf('x');
                        auto valueOne = value; valueOne[i] = '1';
                        auto valueTwo = value; valueTwo[i] = '0';
                        list.removeOne(value);
                        list.push_back(valueOne); list.push_back(valueTwo);
                        break;
                    }
                }
                QString values;
                for(auto value : list)
                {
                    values += QString::number(getInputSize()) + "'b" + value + ", ";
                }
                values.remove(values.size()-2,2);
                verilog << "\t\t\t\t\t" << values << ": " << getNextStateRegisterName() << " = " << connection.key().second->name() << ";\n";
            }
            else
            {
                verilog << "\t\t\t\t\t" << getInputSize() << "'b" << connection.value().first.toLower() << ": " << getNextStateRegisterName() << " = " << connection.key().second->name() << ";\n";
            }
        }
        verilog << "\t\t\t\tendcase\n";
    }
    verilog << "\t\t\tdefault: ";
    verilog << getNextStateRegisterName() << " = " << m_failSafeName << ";\n";
    verilog << "\t\tendcase\n";
    verilog << "\n";

    // output combinational logic
    verilog << "\talways @(in or " << getCurrentStateRegisterName() << ")\n";
    verilog << "\t\tcase(" << getCurrentStateRegisterName() << ")\n";
    for(auto state : m_states)
    {
        verilog << "\t\t\t" << state->name() << ":";
        QString differentOutputsCode = "";
        int zerosCount = 0, onesCount = 0;
        for(auto connection = m_connections.begin(); connection != m_connections.end(); ++connection)
        {
            if(connection.key().first != state) continue;
            if(connection.value().second == "0") zerosCount++;
            else if(connection.value().second == "1") onesCount++;
            if(connection.value().first.toLower().contains('x'))
            {
                QLinkedList<QString> list;
                list.push_back(connection.value().first.toLower());
                for(auto value : list)
                {
                    while(value.contains('x'))
                    {
                        int i = value.indexOf('x');
                        auto valueOne = value; valueOne[i] = '1';
                        auto valueTwo = value; valueTwo[i] = '0';
                        list.removeOne(value);
                        list.push_back(valueOne); list.push_back(valueTwo);
                        break;
                    }
                }
                QString values;
                for(auto value : list)
                {
                    values += QString::number(getInputSize()) + "'b" + value + ", ";
                }
                values.remove(values.size()-2,2);
                differentOutputsCode += "\t\t\t\t\t" + values + ": " + getOutputSignalName() + " = " + QString::number(getOutputSize()) + "'b" + connection.value().second + ";\n";
            }
            else
            {
                differentOutputsCode += "\t\t\t\t\t" + QString::number(getInputSize()) + "'b" + connection.value().first.toLower() + ": " + getOutputSignalName() + " = " + QString::number(getOutputSize()) + "'b" + connection.value().second + ";\n";
            }
        }
        if(zerosCount == 0) verilog << " " << getOutputSignalName() << " = 1'b1;\n";
        else if(onesCount == 0) verilog << " " << getOutputSignalName() << " = 1'b0;\n";
        else
        {
            verilog << "\n\t\t\t\tcase(" << getInputSignalName() << ")\n";
            verilog << differentOutputsCode;
            verilog << "\t\t\t\tendcase\n";
        }
    }
    verilog << "\t\t\tdefault: ";
    verilog << getOutputSignalName() << " = " << QString::number(getOutputSize()) << "'b" << m_failSafeOutput << ";\n";
    verilog << "\t\tendcase\n";

    // end of module
    verilog << "endmodule\n";
    verilog << "\n";
}

QString MealyFSM::printDetails()
{
    QString result = "";
    result += "//\tFSM type: Mealy\n";
    result += "//\tNumber of states: " + QString::number(m_states.count()) + "\n";
    result += QString("//\tClock edge: ") + (getClockEgde() == FallingEdge ? "falling edge" : "rising edge") + "\n";
    result += QString("//\tReset edge: ") + (getResetEdge() == FallingEdge ? "falling edge" : "rising edge") + "\n";
    result += QString("//\tReset mode: ") + (getResetMode() == Synchronous ? "synchronous" : "asynchronous") + "\n";
    result += "//\tFail safe state name: " + m_failSafeName + "\n";
    result += "//\tFail safe default output: " + m_failSafeOutput + "\n";
    return result;
}

QString MealyFSM::printConnections()
{
    QString result = "";
    Connections::iterator ci;
    for(ci = m_connections.begin(); ci != m_connections.end(); ++ci)
    {
        result += "//\t* State: " + QString(ci.key().first->name()) + " is connected to " + ci.key().second->name() + " giving input " + ci.value().first + ". Output becomes " + ci.value().second + "\n";
    }
    return result;
}
