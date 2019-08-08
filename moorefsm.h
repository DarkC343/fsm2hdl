#ifndef MOOREFSM_H
#define MOOREFSM_H

#include <QObject>
#include <QMultiHash>
#include "fsm.h"
#include "moorestate.h"

class MooreFSM : public FSM
{
    typedef QMultiHash<QPair<MooreState *, MooreState *>, QString> Connections;
    QList<MooreState *> m_states;
    // ( (fromState, toState) , input )
    Connections m_connections;
    QString m_failSafeName;

public:
    MooreFSM();
    void addState(const QString &name, const QString &output);
    void setFailSafe(const QString &name);
    void connect(const QString &fromeSate, const QString &toState, const QString &input);
    void generateVerilog();
    QString printDetails();
    QString printConnections();
};

#endif // MOOREFSM_H
