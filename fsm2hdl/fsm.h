#ifndef FSM_H
#define FSM_H

#include <QObject>
#include "state.h"

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
    const State *m_failSafe;
    static size_t m_inputSize;
    static size_t m_outputSize;
    EdgeEnum m_clockEgde;
    EdgeEnum m_resetEdge;
    ResetModeEnum m_resetMode;
    UnitEnum m_timeUnit;
    UnitEnum m_timePrecisionUnit;
    QList<State *> *m_entries;
public:
    explicit FSM(QList<State *> *entries,
                 State *failSafe = nullptr,
                 EdgeEnum clockEdge = RisingEdge,
                 EdgeEnum resetEdge = FallingEdge,
                 ResetModeEnum resetMode = Asynchronous,
                 UnitEnum timeUnit = Nanoseconds,
                 UnitEnum timePrecisionUnit = Picoseconds,
                 QObject *parent = nullptr);

    void setInitialState(State *failSafe);
    QString print();
    void generateVerilog();

    static size_t inputSize();
    static void setInputSize(size_t inputSize);

    static size_t outputSize();
    static void setOutputSize(size_t outputSize);

signals:

public slots:
};

#endif // FSM_H
