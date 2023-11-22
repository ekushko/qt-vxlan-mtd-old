#pragma once

#include <QObject>

class VMTDNodeClient : public QObject
{
    Q_OBJECT
public:
    explicit VMTDNodeClient(QObject *parent = nullptr);

signals:

};

