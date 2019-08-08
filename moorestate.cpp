#include "moorestate.h"

MooreState::MooreState(const QString &name, const QString &output)
{
    setName(name);
    setOutput(output);
}

QString MooreState::name() const
{
    return m_name;
}

void MooreState::setName(const QString &name)
{
    m_name = name;
}

QString MooreState::output() const
{
    return m_output;
}

void MooreState::setOutput(const QString &output)
{
    m_output = output;
}
