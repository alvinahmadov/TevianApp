/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 */
#include "PreferenceDialog.hpp"
#include "Settings.hpp"

#include <QLabel>
#include <QTabWidget>
#include <QGroupBox>
#include <QApplication>
#include <QGridLayout>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QDebug>
#include <QStackedLayout>


namespace Tevian
{
	namespace Gui
	{
		PreferenceDialog::PreferenceDialog(QWidget* parent, const QString& title)
				: QDialog(parent),
				  m_tabWidget { new QTabWidget(this) },
				  m_loginPanel { new QWidget(this) },
				  m_serverPanel { new QWidget(this) }
		{
			init();
			setWindowTitle(title);
		}
		
		void PreferenceDialog::init()
		{
			setFixedSize(QSize(800, 800));
			setModal(true);
			loginPanel();
			serverPanel();
			m_tabWidget->addTab(m_loginPanel, tr("&Credentials"));
			m_tabWidget->addTab(m_serverPanel, tr("&Server Backend"));
			
			m_tabWidget->show();
			m_dialogButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
			                                         Qt::Horizontal, this);
			
			connect(m_dialogButtonBox, SIGNAL(accepted()), SLOT(accept()));
			connect(m_dialogButtonBox, SIGNAL(rejected()), SLOT(reject()));
			
			setLayout(new QGridLayout(this));
			layout()->addWidget(m_tabWidget);
			layout()->addWidget(m_dialogButtonBox);
		}
		
		void PreferenceDialog::loginPanel()
		{
			m_loginPanel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
			auto loginBox = new QGroupBox("Login", m_loginPanel);
			
			loginBox->setFixedWidth(width());
			
			auto loginLayout = new QHBoxLayout();
			loginLayout->setObjectName("loginLayout");
			loginLayout->setContentsMargins(60, 30, 60, 30);
			loginLayout->setSizeConstraint(QFormLayout::SetNoConstraint);
			// Forms
			auto inputLayout = new QFormLayout();
			m_loginPanel->setBackgroundRole(QPalette::ColorRole::Background);
			inputLayout->setObjectName("m_credentialsLayout");
			
			auto emailLabel = new QLabel(tr("Email: "), loginBox);
			auto emailInput = new QLineEdit(loginBox);
			auto passwordLabel = new QLabel(tr("Password: "), loginBox);
			auto passwordInput = new QLineEdit(loginBox);
			auto tokenLabel = new QLabel(tr("Token: "), loginBox);
			auto tokenInput = new QLineEdit(loginBox);
			
			// Echo modes
			passwordInput->setEchoMode(QLineEdit::EchoMode::Password);
			tokenInput->setEchoMode(QLineEdit::EchoMode::Password);
			
			inputLayout->addRow(emailLabel, emailInput);
			inputLayout->addRow(passwordLabel, passwordInput);
			inputLayout->addRow(tokenLabel, tokenInput);
			
			// Default/Placeholder values
			emailInput->setText(g_settingsManager->email());
			passwordInput->setText(g_settingsManager->password());
			tokenInput->setPlaceholderText("Optional");
			
			// auto apiLayout = new Q
			
			// Connections
			connect(emailInput, SIGNAL(textEdited(
					                           const QString&)),
			        g_settingsManager, SLOT(setEmail(
					                                const QString&)));
			
			connect(passwordInput, SIGNAL(textEdited(
					                              const QString&)),
			        g_settingsManager, SLOT(setPassword(
					                                const QString&)));
			
			connect(tokenInput, SIGNAL(textEdited(
					                           const QString&)),
			        g_settingsManager, SLOT(setToken(
					                                const QString&)));
			
			// Finishing
			loginLayout->addLayout(inputLayout);
			loginBox->setLayout(loginLayout);
		}
		
		void PreferenceDialog::serverPanel()
		{
			auto serverBox = new QGroupBox("Server", m_serverPanel);
			
			serverBox->setFixedWidth(width());
			
			auto apiLayout = new QHBoxLayout();
			apiLayout->setContentsMargins(60, 30, 60, 30);
			
			// Forms
			auto inputLayout = new QFormLayout();
			m_serverPanel->setBackgroundRole(QPalette::ColorRole::Background);
			inputLayout->setObjectName("m_credentialsLayout");
			
			auto backendUrlLabel = new QLabel(tr("Server Backend"), serverBox);
			auto backendUrlInput = new QLineEdit(serverBox);
			auto pathLabel = new QLabel(tr("Path to json"), serverBox);
			auto pathInput = new QLineEdit(serverBox);
			
			inputLayout->addRow(backendUrlLabel, backendUrlInput);
			inputLayout->addRow(pathLabel, pathInput);
			
			// Default/Placeholder values
			backendUrlInput->setText(g_settingsManager->url());
			pathInput->setText(g_settingsManager->path());
			
			connect(backendUrlInput, SIGNAL(textEdited(
					                                const QString&)),
			        g_settingsManager, SLOT(setBackendUrl(
					                                const QString&)));
			connect(pathInput, SIGNAL(textEdited(
					                          const QString&)),
			        g_settingsManager, SLOT(setApiPath(
					                                const QString&)));
			
			// Finishing
			apiLayout->addLayout(inputLayout);
			serverBox->setLayout(apiLayout);
		}
		
		void PreferenceDialog::accept()
		{
			g_settingsManager->save();
			QDialog::accept();
		}
		
		void PreferenceDialog::reject()
		{
			QDialog::reject();
		}
		
	}// namespace Gui
}// namespace Tevian


#include <moc_PreferenceDialog.cpp>