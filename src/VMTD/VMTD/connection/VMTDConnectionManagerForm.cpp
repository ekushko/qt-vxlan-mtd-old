#include    "VMTDConnectionManagerForm.h"
#include "ui_VMTDConnectionManagerForm.h"

#include "../VMTDRepo.h"

namespace VMTDLib
{
    VMTDConnectionManagerForm::VMTDConnectionManagerForm(QWidget *parent,
                                                         VMTDConnectionManager *manager) :
        QWidget(parent),
        ui(new Ui::VMTDConnectionManagerForm),
        m_manager(manager),
        m_settings(manager->settings())
    {
        m_settings->creationOut(VN_S(VMTDConnectionManagerForm) + " | Constructor called");

        ui->setupUi(this);

        if (parent != nullptr && parent->layout() != nullptr)
            parent->layout()->addWidget(this);

        setAttribute(Qt::WA_DeleteOnClose, true);
        setWindowTitle("Connections");

        initializeView();

        m_settings->creationOut(VN_S(VMTDConnectionManagerForm) + " | Constructor finished");
    }

    VMTDConnectionManagerForm::~VMTDConnectionManagerForm()
    {
        m_settings->creationOut(VN_S(VMTDConnectionManagerForm) + " | Destructor called");

        delete ui;

        m_settings->creationOut(VN_S(VMTDConnectionManagerForm) + " | Destructor finished");
    }

    void VMTDConnectionManagerForm::initializeView()
    {
        connect(ui->pbSave, &QPushButton::clicked,
                m_manager, &VMTDConnectionManager::saveSlot);
        connect(ui->pbLoad, &QPushButton::clicked,
                m_manager, &VMTDConnectionManager::loadSlot);

        connect(ui->pbReload, &QPushButton::clicked,
                this, &VMTDConnectionManagerForm::pbReloadClicked);
        connect(ui->pbCreate, &QPushButton::clicked,
                this, &VMTDConnectionManagerForm::pbCreateClicked);

        connect(ui->cbDevice, QOverload<int>::of(&QComboBox::currentIndexChanged),
                this, &VMTDConnectionManagerForm::cbDeviceCurrentIndexChanged);
        connect(ui->cbDevice_1, QOverload<int>::of(&QComboBox::currentIndexChanged),
                this, &VMTDConnectionManagerForm::cbDeviceCurrentIndexChanged_1);
        connect(ui->cbDevice_2, QOverload<int>::of(&QComboBox::currentIndexChanged),
                this, &VMTDConnectionManagerForm::cbDeviceCurrentIndexChanged_2);

        reinitializeView();
    }
    void VMTDConnectionManagerForm::reinitializeView()
    {
        fillDevices(ui->cbDevice);
        fillDevices(ui->cbDevice_1);
        fillDevices(ui->cbDevice_2);
    }

    void VMTDConnectionManagerForm::fillDevices(QComboBox *cb)
    {
        cb->clear();

        const auto &devices = m_manager->deviceManager()->devices();

        for (auto it = devices.begin(); it != devices.end(); ++it)
        {
            auto device = *it;

            cb->addItem(device->name(), device->id());
        }
    }
    void VMTDConnectionManagerForm::fillInterfaces(QComboBox *cb, VMTDDevice *device)
    {
        cb->clear();

        const auto &interfaces = device->interfaceManager()->interfaces();

        for (auto it = interfaces.begin(); it != interfaces.end(); ++it)
        {
            auto interface = *it;

            cb->addItem(interface->name(), interface->id());
        }
    }
    void VMTDConnectionManagerForm::fillConnections(QWidget *w, VMTDDevice *device)
    {
        qDeleteAll(m_connForms);
        m_connForms.clear();

        const auto &interfaces = device->interfaceManager()->interfaces();

        for (auto it = interfaces.begin(); it != interfaces.end(); ++it)
        {
            auto interface = *it;
            auto connectedDeviceInterface = m_manager->connectedDeviceInterface(device, interface);
            auto connectedDevice = connectedDeviceInterface.first;
            auto connectedInterface = connectedDeviceInterface.second;

            if (connectedDevice != nullptr && connectedInterface != nullptr)
            {
                auto connForm = new VMTDConnectionForm(w, device, interface, connectedDevice, connectedInterface);
                connect(connForm, &VMTDConnectionForm::removeSignal, this, &VMTDConnectionManagerForm::removeSlot);

                m_connForms.append(connForm);
            }
        }
    }

    void VMTDConnectionManagerForm::removeSlot()
    {
        auto connForm = dynamic_cast<VMTDConnectionForm *>(sender());

        if (connForm == nullptr)
            return;

        m_manager->removeConnection(connForm->device_1(), connForm->interface_1(),
                                    connForm->device_2(), connForm->interface_2());

        cbDeviceCurrentIndexChanged(ui->cbDevice->currentIndex());
    }

    void VMTDConnectionManagerForm::pbReloadClicked()
    {
        fillDevices(ui->cbDevice);
        fillDevices(ui->cbDevice_1);
        fillDevices(ui->cbDevice_2);
    }

    void VMTDConnectionManagerForm::pbCreateClicked()
    {
        auto device_1 = m_manager->deviceManager()->device(ui->cbDevice_1->currentData().toInt());
        auto interface_1 = device_1->interfaceManager()->interface(
                                   ui->cbInterface_1->currentData().toInt());

        auto device_2 = m_manager->deviceManager()->device(ui->cbDevice_2->currentData().toInt());
        auto interface_2 = device_2->interfaceManager()->interface(
                                   ui->cbInterface_2->currentData().toInt());

        m_manager->createConnection(device_1, interface_1, device_2, interface_2);

        cbDeviceCurrentIndexChanged(ui->cbDevice->currentIndex());
    }

    void VMTDConnectionManagerForm::cbDeviceCurrentIndexChanged(int index)
    {
        Q_UNUSED(index)

        const auto id = ui->cbDevice->currentData().toInt();

        auto device = m_manager->deviceManager()->device(id);

        if (device != nullptr)
            fillConnections(ui->wConnections, device);
    }
    void VMTDConnectionManagerForm::cbDeviceCurrentIndexChanged_1(int index)
    {
        Q_UNUSED(index)

        const auto id = ui->cbDevice_1->currentData().toInt();

        auto device = m_manager->deviceManager()->device(id);

        if (device != nullptr)
            fillInterfaces(ui->cbInterface_1, device);

    }
    void VMTDConnectionManagerForm::cbDeviceCurrentIndexChanged_2(int index)
    {
        Q_UNUSED(index)

        const auto id = ui->cbDevice_2->currentData().toInt();

        auto device = m_manager->deviceManager()->device(id);

        if (device != nullptr)
            fillInterfaces(ui->cbInterface_2, device);
    }
}
