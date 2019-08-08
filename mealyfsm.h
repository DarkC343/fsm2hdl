#ifndef MEALYFSM_H
#define MEALYFSM_H

#include <QObject>
#include <QMultiHash>
#include "fsm.h"
#include "mealystate.h"

class MealyFSM : public FSM
{
    typedef QMultiHash<QPair<MealyState *, MealyState *>, QPair<QString, QString>> Connections;
    QList<MealyState *> m_states;
    // ( (fromState, toState) , (input, output) )
    Connections m_connections;
    QString m_failSafeName;
    QString m_failSafeOutput;
public:
    MealyFSM();
    void addState(const QString &name);
    void setFailSafe(const QString &name, const QString &defaultOutput);
    void connect(const QString &fromeSate, const QString &toState, const QString &input, const QString &output);
    void generateVerilog();
    QString printDetails();
    QString printConnections();
};

#endif // MEALYFSM_H
