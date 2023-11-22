#include "VMTDController.h"
#include "VMTDControllerForm.h"

#include "VMTDRepo.h"

#include <QCoreApplication>

namespace VMTDLib
{
    VMTDController::VMTDController(QObject *parent, VMTDSettings *settings)
        : QThread{parent}
        , m_settings(settings)
    {
        m_settings->debugOut(VN_S(VMTDController) + " was created");

        connect(this, &VMTDController::started, this, &VMTDController::startedSlot);
        connect(this, &VMTDController::finished, this, &VMTDController::finishedSlot);

        m_netManager = new QNetworkAccessManager(nullptr);

        loadNxApiAdapters();
    }

    VMTDController::~VMTDController()
    {
        m_settings->debugOut(VN_S(VMTDController) + " was deleted");

        if (m_form != nullptr)
            delete m_form;

        for (int i = m_nxApiAdapters.size() - 1; i >= 0; --i)
            deleteNxApiAdapter(i);
    }

    void VMTDController::showForm()
    {
        if (m_form == nullptr)
            m_form = new VMTDControllerForm(nullptr, this);

        m_form->show();
        m_form->raise();
        m_form->activateWindow();
    }

    VMTDSettings *VMTDController::settings() const
    {
        return m_settings;
    }

    void VMTDController::startController()
    {
        start();
    }
    void VMTDController::stopController()
    {
        quit();
        wait();
    }

    const QVector<VMTDNxApiAdapter *> &VMTDController::nxApiAdapters() const
    {
        return m_nxApiAdapters;
    }

    void VMTDController::saveNxApiAdapters()
    {
        QJsonObject jsonObj;

        for (int i = 0; i < m_nxApiAdapters.size(); ++i)
            jsonObj[QString("nxApiAdapter_%1").arg(i)] = m_nxApiAdapters.at(i)->toJson();

        m_settings->setNxApiAdaptersParams(jsonObj);
        m_settings->save();
    }
    void VMTDController::loadNxApiAdapters()
    {
        for (int i = m_nxApiAdapters.size() - 1; i >= 0; --i)
            deleteNxApiAdapter(i);

        auto jsonObj = m_settings->nxApiAdaptersParams();

        for (int i = 0; i < jsonObj.size(); ++i)
        {
            auto nxApiAdapter = createNxApiAdapter();
            nxApiAdapter->fromJson(jsonObj[QString("nxApiAdapter_%1").arg(i)].toObject());
        }
    }

    VMTDNxApiAdapter *VMTDController::createNxApiAdapter()
    {
        auto nxApiAdapter = new VMTDNxApiAdapter(nullptr, m_settings, m_netManager);
        m_nxApiAdapters.append(nxApiAdapter);

        if (isRunning())
            nxApiAdapter->moveToThread(this);

        return nxApiAdapter;
    }
    void VMTDController::deleteNxApiAdapter(int index)
    {
        if (index < 0 || index >= m_nxApiAdapters.size())
        {
            m_settings->debugOut(VN_S(VMTDController) + " | Bad NX-API adapter index");
            return;
        }

        auto nxApiAdapter = m_nxApiAdapters.at(index);

        if (isRunning())
            nxApiAdapter->moveToThread(qApp->thread());

        delete nxApiAdapter;

        m_nxApiAdapters.removeAt(index);
    }

    void VMTDController::run()
    {
        m_netManager->moveToThread(this);

        for (auto nxApiAdapter : qAsConst(m_nxApiAdapters))
            nxApiAdapter->moveToThread(this);

        exec();

        for (auto nxApiAdapter : qAsConst(m_nxApiAdapters))
            nxApiAdapter->moveToThread(qApp->thread());

        m_netManager->moveToThread(qApp->thread());
    }

    void VMTDController::startedSlot()
    {
        m_settings->debugOut(VN_S(VMTDController) + " started");
    }
    void VMTDController::finishedSlot()
    {
        m_settings->debugOut(VN_S(VMTDController) + " finished");
    }
}
