#ifndef MOORESTATE_H
#define MOORESTATE_H

#include <QObject>

class MooreState : public QObject
{
    Q_OBJECT
    QString m_name, m_output;
public:
    explicit MooreState(const QString &name, const QString &output);

    QString name() const;
    void setName(const QString &name);

    QString output() const;
    void setOutput(const QString &output);

signals:

public slots:
};

#endif // MOORESTATE_H
