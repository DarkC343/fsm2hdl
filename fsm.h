#ifndef FSM_H
#define FSM_H

#include <QObject>

class FSM : public QObject
{
    Q_OBJECT
public:
    explicit FSM(QObject *parent = nullptr);

signals:

public slots:
};

#endif // FSM_H