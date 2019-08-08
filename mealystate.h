#ifndef MEALYSTATE_H
#define MEALYSTATE_H

#include <QObject>

class MealyState : public QObject
{
    Q_OBJECT
    QString m_name;
    MealyState() {}
public:
    explicit MealyState(const QString &name);

    QString name() const;
    void setName(const QString &name);

signals:

public slots:
};

#endif // MEALYSTATE_H
