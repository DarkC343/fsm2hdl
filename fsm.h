#ifndef FSM_H
#define FSM_H

#include <QObject>

enum EdgeEnum
{
    FallingEdge,
    RisingEdge
};

enum ResetModeEnum
{
    Asynchronous,
    Synchronous
};

enum UnitEnum
{
    Seconds,
    Milliseconds,
    Microseconds,
    Nanoseconds,
    Picoseconds,
    Femtoseconds
};

class FSM : public QObject
{
    Q_OBJECT
    size_t m_inputSize, m_outputSize;
    EdgeEnum m_clockEgde;
    EdgeEnum m_resetEdge;
    ResetModeEnum m_resetMode;
    QString m_timeUnit;
    QString m_timePrecisionUnit;
    QString m_moduleName;
    QString m_inputSignalName;
    QString m_outputSignalName;
    QString m_clockSignalName;
    QString m_resetSignalName;
    QString m_currentStateRegisterName;
    QString m_nextStateRegisterName;
    FSM() {}
    const QString getTimeUnitString(UnitEnum unit);
protected:
    explicit FSM(QObject *parent = nullptr);
    size_t  getInputSize() const;
    size_t  getOutputSize() const;
    QString getTimeUnit() const;
    QString getTimePrecisionUnit() const;
    QString getModuleName() const;
    QString getInputSignalName() const;
    QString getOutputSignalName() const;
    QString getClockSignalName() const;
    QString getResetSignalName() const;
    QString getCurrentStateRegisterName() const;
    QString getNextStateRegisterName() const;
    EdgeEnum getClockEgde() const;
    EdgeEnum getResetEdge() const;
    ResetModeEnum getResetMode() const;
public:
    void setSignals();
    void setCodeSettings(EdgeEnum clockEgde = RisingEdge, EdgeEnum resetEdge = FallingEdge, ResetModeEnum resetMode = Asynchronous, UnitEnum timeUnit = Nanoseconds, UnitEnum timePrecisionUnit = Picoseconds);

    void setInputSize(const size_t inputSize); // add control for 0
    void setOutputSize(const size_t outputSize); // add control for 0

    void setSignalNames(const QString &inputSignalName, const QString &outputSignalName, const QString &clockSignalName, const QString &resetSignalName, const QString &currentStateRegisterName, const QString &nextStateRegisterName);
    void setModuleName(const QString &moduleName);


signals:

public slots:
};

#endif // FSM_H
