#pragma once

#include  "../VMTDDevice.h"

#include <QUrl>

namespace VMTDLib
{
    class VMTDNxApiDeviceForm;

    class VMTD_SHARED VMTDNxApiDevice : public VMTDDevice
    {
        Q_OBJECT

    public:

        VMTDNxApiDevice(QObject *parent, VMTDSettings *settings, int id);
        ~VMTDNxApiDevice();

        // ЛОГИКА

        QJsonObject toJson() const override;
        void      fromJson(const QJsonObject &jsonObj) override;

        // ДАННЫЕ

        QString name() const override;

        QUrl    url() const;
        void setUrl(const QUrl &url);

    public slots:

        void showFormSlot() override;

    private:

        QPointer<VMTDNxApiDeviceForm> m_form;

        QUrl m_url = QUrl("http://127.0.0.1/ins");
    };
}
