/**
 *  Copyright (C) 2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 */
#pragma once


#include <QDialog>

class QTabWidget;

class QSettings;

class QGridLayout;

class QVBoxLayout;

class QDialogButtonBox;

class QFormLayout;

namespace Tevian
{
	namespace Gui
	{
		class PreferenceDialog : public QDialog
		{
		Q_OBJECT
		public:
			PreferenceDialog(QWidget* parent, const QString& title = QString("Preferences"));
			
			void loginPanel();
			
			void serverPanel();
		
		public slots:
			
			void accept();
			
			void reject();
		
		private:
			void init();
		
		private:
			QTabWidget* m_tabWidget;
			
			QDialogButtonBox* m_dialogButtonBox;
			
			QWidget* m_loginPanel;
			
			QWidget* m_serverPanel;
		};
	}// namespace Gui
}// namespace Tevian