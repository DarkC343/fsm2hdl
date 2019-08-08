#include "fsm.h"

void FSM::setInputSize(const size_t inputSize)
{
    m_inputSize = inputSize;
}

void FSM::setOutputSize(const size_t outputSize)
{
    m_outputSize = outputSize;
}

void FSM::setSignalNames(const QString &inputSignalName, const QString &outputSignalName, const QString &clockSignalName, const QString &resetSignalName, const QString &currentStateRegisterName, const QString &nextStateRegisterName)
{
    m_inputSignalName = inputSignalName;
    m_outputSignalName = outputSignalName;
    m_clockSignalName = clockSignalName;
    m_resetSignalName = resetSignalName;
    m_currentStateRegisterName = currentStateRegisterName;
    m_nextStateRegisterName = nextStateRegisterName;
}

QString FSM::getTimeUnit() const
{
    return m_timeUnit;
}

QString FSM::getTimePrecisionUnit() const
{
    return m_timePrecisionUnit;
}

QString FSM::getModuleName() const
{
    return m_moduleName;
}

QString FSM::getInputSignalName() const
{
    return m_inputSignalName;
}

QString FSM::getOutputSignalName() const
{
    return m_outputSignalName;
}

QString FSM::getResetSignalName() const
{
    return m_resetSignalName;
}

QString FSM::getClockSignalName() const
{
    return m_clockSignalName;
}

QString FSM::getCurrentStateRegisterName() const
{
    return m_currentStateRegisterName;
}

QString FSM::getNextStateRegisterName() const
{
    return m_nextStateRegisterName;
}

size_t FSM::getInputSize() const
{
    return m_inputSize;
}

size_t FSM::getOutputSize() const
{
    return m_outputSize;
}


void FSM::setModuleName(const QString &moduleName)
{
    m_moduleName = moduleName;
}

EdgeEnum FSM::getClockEgde() const
{
    return m_clockEgde;
}

EdgeEnum FSM::getResetEdge() const
{
    return m_resetEdge;
}

ResetModeEnum FSM::getResetMode() const
{
    return m_resetMode;
}

const QString FSM::getTimeUnitString(UnitEnum unit)
{
    switch (unit) {
    case Seconds: return "s";
        case Milliseconds: return "ms";
        case Microseconds: return "us";
        case Nanoseconds: return "ns";
        case Picoseconds: return "ps";
        case Femtoseconds: return "fs";
    }
}

FSM::FSM(QObject *parent) : QObject(parent)
{

}

void FSM::setCodeSettings(EdgeEnum clockEgde, EdgeEnum resetEdge, ResetModeEnum resetMode, UnitEnum timeUnit, UnitEnum timePrecisionUnit)
{
    m_clockEgde = clockEgde;
    m_resetEdge = resetEdge;
    m_resetMode = resetMode;
    m_timeUnit = getTimeUnitString(timeUnit);
    m_timePrecisionUnit = getTimeUnitString(timePrecisionUnit);
}
