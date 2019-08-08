#include "mealystate.h"

MealyState::MealyState(const QString &name)
{
    setName(name);
}

QString MealyState::name() const
{
    return m_name;
}

void MealyState::setName(const QString &name)
{
    m_name = name;
}
