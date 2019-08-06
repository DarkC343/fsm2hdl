#include "state.h"
#include "fsm.h"

const char *State::name() const
{
    return m_name;
}

void State::setName(const char *name)
{
    m_name = name;
}

QHash<QPair<QString, QString>, State *> State::connections() const
{
    return m_connections;
}

State::State(const char *name, QObject *parent) : QObject(parent)
{
    m_name = name;
}

void State::addTransition(State *nextSate, const char *input, const char *output)
{
    bool isValid = strlen(input) == FSM::inputSize()
                && strlen(output) == FSM::outputSize();
#ifdef QT_DEBUG
    Q_ASSERT(isValid);
#else
    if(!isValid){
        return;
    }
#endif

    m_connections.insert(QPair<const char *, const char *>(input, output), nextSate);
}
