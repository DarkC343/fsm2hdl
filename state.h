#ifndef STATE_H
#define STATE_H

#include <QObject>
#include <QHash>
#include <QPair>

class State : public QObject
{
    Q_OBJECT
    const char *m_name;
    // (input,output), toState
    QHash<QPair<QString, QString>, State *> m_connections;

public:
    explicit State(const char * name = "", QObject *parent = nullptr);
    void addTransition(State *nextSate, const char *input, const char *output);

    const char *name() const;
    void setName(const char *name);

    QHash<QPair<QString, QString>, State *> connections() const;

signals:

public slots:
};

#endif // STATE_H
