/**
 *  Copyright (C) 2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 */
#pragma once


#include <memory>

#include "Gui/ImageBook.hpp"
#include "PreferenceDialog.hpp"

#include <QMainWindow>
#include <QMap>
#include <QProgressBar>


class QAction;

namespace Tevian
{
	namespace Gui
	{
		
		class TEVIAN_API Window : public QMainWindow
		{
		Q_OBJECT
		public:
			Window(bool enable_statusbar = false);
			
			~Window() Q_DECL_OVERRIDE;
			
			bool load(const QString& file);
		
		private slots:
			
			void open();
			
			void openDirectory();
			
			void prefs();
			
			void about();
		
		signals:
			
			void showTabs(bool s);
		
		private:
			void init();
		
		private:
			bool m_enableStatusbar;
			
			ImageBook* m_imgBook;
			
			QProgressBar* m_progressBar;
			
			PreferenceDialog* m_preferenceDialog;
		};
	}// namespace Gui
}// namespace Tevian