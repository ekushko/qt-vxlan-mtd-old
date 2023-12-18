#include    "VMTDControllerForm.h"
#include "ui_VMTDControllerForm.h"

#include "VMTDRepo.h"

namespace VMTDLib
{
    VMTDControllerForm::VMTDControllerForm(QWidget *parent, VMTDController *controller) :
        QWidget(parent),
        ui(new Ui::VMTDControllerForm),
        m_controller(controller)
    {
        m_controller->settings()->debugOut(VN_S(VMTDControllerForm) + " was created");

        ui->setupUi(this);

        setWindowTitle(QString("Контроллер %1").arg(m_controller->settings()->systemName()));

        initialize();

        updateNxApiAdaptersList();
    }

    VMTDControllerForm::~VMTDControllerForm()
    {
        m_controller->settings()->debugOut(VN_S(VMTDControllerForm) + " was deleted");

        delete ui;
    }

    void VMTDControllerForm::initialize()
    {
        connect(ui->pbSettings, &QPushButton::clicked,
                this, &VMTDControllerForm::pbSettingsClicked);
        connect(ui->pbRefresh, &QPushButton::clicked,
                this, &VMTDControllerForm::pbRefreshClicked);
        connect(ui->pbSave, &QPushButton::clicked,
                this, &VMTDControllerForm::pbSaveClicked);
        connect(ui->pbLoad, &QPushButton::clicked,
                this, &VMTDControllerForm::pbLoadClicked);
        connect(ui->pbCreateNxApiAdapter, &QPushButton::clicked,
                this, &VMTDControllerForm::pbCreateNxApiAdapterClicked);
        connect(ui->pbDeleteNxApiAdapter, &QPushButton::clicked,
                this, &VMTDControllerForm::pbDeleteNxApiAdapterClicked);
        connect(ui->lwNxApiAdapters, &QListWidget::itemDoubleClicked,
                this, &VMTDControllerForm::lwNxApiAdaptersItemDoubleClicked);
    }

    void VMTDControllerForm::updateNxApiAdaptersList()
    {
        ui->lwNxApiAdapters->clear();

        for (auto nxApiAdapter : m_controller->nxApiAdapters())
            ui->lwNxApiAdapters->addItem(nxApiAdapter->url().toString());
    }

    void VMTDControllerForm::pbSettingsClicked()
    {
        m_controller->settings()->showForm();
    }

    void VMTDControllerForm::pbRefreshClicked()
    {
        updateNxApiAdaptersList();
    }

    void VMTDControllerForm::pbSaveClicked()
    {
        m_controller->saveNxApiAdapters();
    }
    void VMTDControllerForm::pbLoadClicked()
    {
        m_controller->loadNxApiAdapters();

        updateNxApiAdaptersList();
    }

    void VMTDControllerForm::pbCreateNxApiAdapterClicked()
    {
        auto adapter = m_controller->createNxApiAdapter();
        adapter->setUrl(QUrl(ui->leUrl->text()));

        updateNxApiAdaptersList();
    }
    void VMTDControllerForm::pbDeleteNxApiAdapterClicked()
    {
        for (int i = 0; i < ui->lwNxApiAdapters->selectedItems().size(); ++i)
        {
            const auto row = ui->lwNxApiAdapters->row(ui->lwNxApiAdapters->selectedItems().at(i));
            m_controller->deleteNxApiAdapter(row);
        }

        updateNxApiAdaptersList();
    }

    void VMTDControllerForm::lwNxApiAdaptersItemDoubleClicked(QListWidgetItem *item)
    {
        const auto row = ui->lwNxApiAdapters->row(item);
        m_controller->nxApiAdapters().at(row)->showForm();
    }
}
