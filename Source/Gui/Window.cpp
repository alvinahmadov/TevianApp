#include "Commons.hpp"
#include "Gui/Window.hpp"
#include "Gui/PreferenceDialog.hpp"

#include <QStandardPaths>
#include <QDirIterator>
#include <QString>
#include <QImageReader>
#include <QImageWriter>
#include <QScreen>
#include <QPainter>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>
#include <QLabel>
#include <QPushButton>
#include <QToolButton>
#include <QVBoxLayout>
#include <QApplication>
#include <QDialog>
#include <QMessageBox>
#include <QCommonStyle>
#include <QProgressBar>

#include <QDebug>


namespace Tevian
{
	namespace Gui
	{
		using namespace Math;
		
		static bool firstDialog = true;
		
		static void
		filterImages(QByteArrayList& supportedMimeTypes, QFileDialog::AcceptMode acceptMode)
		{
			supportedMimeTypes = acceptMode == QFileDialog::AcceptOpen
			                     ? QImageReader::supportedMimeTypes()
			                     : QImageWriter::supportedMimeTypes();
		}
		
		/**
		 * \brief Initializes default open folder and filters
		 * supported image files (eg. jpeg, png etc.)
		 * */
		static void initializeImageFileDialog(QFileDialog& dialog, QFileDialog::AcceptMode acceptMode)
		{
			// Default open folder is OS's Pictures folder
			if (firstDialog)
			{
				firstDialog = false;
				
				QString picturesLocation = QDir::toNativeSeparators("../../../../Data");
				
				dialog.setDirectory(picturesLocation.isEmpty()
				                    ? QDir::currentPath()
				                    : picturesLocation);
			}
			
			QStringList mimeTypeFilters;
			QByteArrayList supportedMimeTypes;
			filterImages(supportedMimeTypes, acceptMode);
			mimeTypeFilters << "application/octet-stream";
			
			
			dialog.setMimeTypeFilters(mimeTypeFilters);
			
			if (acceptMode == QFileDialog::AcceptSave)
			{
				dialog.selectMimeTypeFilter("image/jpeg");
			}
			
		}
		
		Window::Window(bool enable_statusbar)
				: m_enableStatusbar { enable_statusbar },
				  m_imgBook(new ImageBook(this)),
				  m_progressBar(new QProgressBar())
		{
			init();
		}
		
		Window::~Window()
		{
		}
		
		bool Window::load(const QString& file)
		{
			
			if (!QImageReader::imageFormat(file).isEmpty())
			{
				m_imgBook->loadImage(file);
			}
			
			if (m_enableStatusbar)
			{
				const QString message = tr("Opened \"%1\"").arg(QDir::toNativeSeparators(file));
				statusBar()->showMessage(message);
			}
			return true;
		}
		
		void Window::open()
		{
			auto dialog = new QFileDialog(this, tr("&Open File"));
			dialog->setFileMode(QFileDialog::FileMode::ExistingFiles);
			dialog->setViewMode(QFileDialog::ViewMode::Detail);
			initializeImageFileDialog(*dialog, QFileDialog::AcceptOpen);
			
			dialog->setNameFilter(tr("Image Files (*.png *.jpg *.jpeg *.bmp *webp)"));
			
			if (dialog->exec() == QDialog::Accepted)
			{
				// Exclude unsupported.
				size_t filesCount {};
				
				for (auto& file : dialog->selectedFiles())
				{
					if (!QImageReader::imageFormat(file).isEmpty())
					{
						++filesCount;
					}
				}
				int current { };
				
				m_imgBook->hide();
				m_progressBar->show();
				
				for (const auto& selectedFile : dialog->selectedFiles())
				{
					load(selectedFile);
					m_progressBar->setValue((++current) * filesCount);
				}
				
				m_imgBook->show();
				m_progressBar->hide();
				
			}
			delete dialog;
		}
		
		void Window::openDirectory()
		{
			auto dialog = new QFileDialog(this, tr("&Open Directory"));
			dialog->setFileMode(QFileDialog::FileMode::DirectoryOnly);
			dialog->setViewMode(QFileDialog::ViewMode::Detail);
			
			initializeImageFileDialog(*dialog, QFileDialog::AcceptOpen);
			
			if (dialog->exec() == QFileDialog::Accepted)
			{
				
				dialog->setNameFilter(tr("Image Files (*.png *.jpg *.jpeg *.bmp *webp)"));
				m_imgBook->hide();
				m_progressBar->show();
				QDirIterator diriter(dialog->selectedFiles().first());
				while (diriter.hasNext())
				{
					auto file = diriter.next();
					if (!file.isEmpty() && file.at(file.length() - 1) != '.')
					{
						load(file);
					}
				}
				
				m_imgBook->show();
				m_progressBar->hide();
			}
			
			delete dialog;
		}
		
		/*
		// bool Window::save(const QString& fileName)
		// {
		// 	Q_UNUSED(fileName)
		QImageWriter writer(fileName);

		if (!writer.write(*m_images.top()))
		{
			QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
									 tr("Cannot write %1: %2")
											 .arg(QDir::toNativeSeparators(fileName)), writer.errorString());
			return false;
		}

		if (m_enableStatusbar)
		{
			const QString message = tr("Wrote \"%1\"").arg(QDir::toNativeSeparators(fileName));
			statusBar()->showMessage(message);
		}

		return true;
		
		// 	return true;
		// }
		//
		*/
		
		void Window::init()
		{
			setCentralWidget(m_imgBook);
			auto toolbar = new QToolBar(this);
			m_imgBook->setVisible(false);
			toolbar->setFixedHeight(size().height() / 9);
			toolbar->setIconSize(QSize(24, 24));
			toolbar->layout()->setSpacing(20);
			toolbar->setMovable(false);
			addToolBar(toolbar);
			m_imgBook->hide();
			m_progressBar->hide();
			m_progressBar->setFixedSize(500,100);
			m_progressBar->setWindowModality(Qt::ApplicationModal);
			m_progressBar->setAlignment(Qt::AlignCenter);
			m_progressBar->adjustSize();
			
			resize(QGuiApplication::primaryScreen()->availableSize());
			
			// Initialize buttons
			// Open files
			auto openBtn = new QToolButton(this);
			openBtn->setIcon(QIcon::fromTheme("image-x-generic"));
			openBtn->setToolTip("Open files");
			openBtn->setShortcut(tr("Ctrl+O"));
			toolbar->addWidget(openBtn);
			
			// Open directory
			auto openDirBtn = new QToolButton(this);
			openDirBtn->setIcon(QIcon::fromTheme("folder-open"));
			openDirBtn->setToolTip(tr("Open Directory"));
			openDirBtn->setShortcut(tr("Ctrl+Shift+O"));
			toolbar->addWidget(openDirBtn);
			
			// Save
			auto saveBtn = new QToolButton(this);
			saveBtn->setIcon(QIcon::fromTheme("document-save"));
			saveBtn->setToolTip(tr("Save"));
			saveBtn->setShortcut(tr("Ctrl+Shift+S"));
			toolbar->addWidget(saveBtn);
			
			// Settings
			auto settingsBtn = new QToolButton(this);
			settingsBtn->setIcon(QIcon::fromTheme("settings"));
			settingsBtn->setToolTip(tr("Preferences"));
			settingsBtn->setShortcut(tr("Alt+S"));
			toolbar->addWidget(settingsBtn);
			
			// About
			auto aboutBtn = new QToolButton(this);
			aboutBtn->setIcon(QIcon::fromTheme("help"));
			aboutBtn->setToolTip(tr("Help"));
			aboutBtn->setShortcut(tr("Ctrl+F1"));
			toolbar->addWidget(aboutBtn);
			
			// Quit
			auto exitBtn = new QToolButton(this);
			exitBtn->setIcon(QIcon::fromTheme("exit"));
			exitBtn->setToolTip(tr("Quit"));
			exitBtn->setShortcut(tr("Ctrl+Q"));
			toolbar->addWidget(exitBtn);
			
			// Connections
			connect(openBtn, SIGNAL(clicked(bool)), this, SLOT(open()));
			connect(openDirBtn, SIGNAL(clicked(bool)), this, SLOT(openDirectory()));
			connect(settingsBtn, SIGNAL(clicked(bool)), this, SLOT(prefs()));
			connect(aboutBtn, SIGNAL(clicked(bool)), this, SLOT(about()));
			connect(exitBtn, SIGNAL(clicked(bool)), this, SLOT(close()));
		}
		
		void Window::prefs()
		{
			PreferenceDialog* preferenceDialog = new PreferenceDialog(nullptr, "Preferences");
			
			preferenceDialog->show();
			preferenceDialog->deleteLater();
		}
		
		void Window::about()
		{
			QMessageBox::about(this, tr("About Face Detector"),
			                   tr("<p>The <b>Face Detector</b> program detects properties"
			                      " of image like number"));
		}
	} // namespace Gui
} // namespace Tevian

#include <Gui/moc_Window.cpp>