#pragma once

#include "../VMTDDevice.h"

namespace VMTDLib
{
    class VMTDNodeDeviceForm;

    class VMTD_SHARED VMTDNodeDevice : public VMTDDevice
    {
        Q_OBJECT

    public:

        VMTDNodeDevice(QObject *parent, VMTDSettings *settings, int id);
        ~VMTDNodeDevice();

        // ЛОГИКА

        QJsonObject toJson() const override;
        void      fromJson(const QJsonObject &jsonObj) override;

        // ДАННЫЕ

        QString name() const override;

        QString ip() const;
        void setIp(const QString &ip);

        QString domainName() const;
        void setDomainName(const QString &domainName);

    signals:

        void appendRequestsSignal(const QList<QPair<QString, QJsonObject>> &requests);

    public slots:

        void showFormSlot() override;

    private:

        QPointer<VMTDNodeDeviceForm> m_form;

        QString m_ip;
        QString m_domainName;
    };
}
