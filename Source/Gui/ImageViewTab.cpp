/**
 *  Copyright (C) 2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 */

#include "FaceApi.hpp"
#include "Gui/ImageViewTab.hpp"
#include "Gui/DetectionRenderer.hpp"

#include <QLabel>
#include <QMenu>
#include <QLayout>
#include <QToolBar>
#include <QMenuBar>
#include <QComboBox>
#include <QImageReader>
#include <QPushButton>
#include <QActionGroup>
#include <QGridLayout>
#include <QToolButton>
#include <QGraphicsLayout>
#include <QApplication>
#include <QMessageBox>
#include <QGraphicsTextItem>
#include <QGraphicsSimpleTextItem>


namespace Tevian
{
	namespace Gui
	{
		using namespace Helpers;
		
		ImageViewTab::ImageViewTab(const QString& imagefile,
		                           QWidget* parent)
				: QWidget(parent),
				  m_file { imagefile },
				  m_scaling { new Scaling(1.5, { "50", { 0.6, 1.5 } }) },
				  m_view { new ViewPort(this) },
				  m_scene { new QGraphicsScene(this) }
		{
			setVisible(false);
			auto faceApi = new Client::FaceApi(g_settingsManager->url(), g_settingsManager->path());
			m_faceDetector = new FaceDetector(m_file, faceApi);
			QImageReader reader(m_file);
			reader.setAutoTransform(true);
			auto image = new QImage(reader.read());
			m_faceDetector->setParent(this);
			m_renderer = new DetectionRenderer(image);
			m_controls = new Controls(this, m_renderer);
			init();
		}
		
		ImageViewTab::~ImageViewTab()
		{
			delete m_scaling;
			delete m_renderer;
			delete m_controls;
			delete m_topLayout;
			delete m_demographicsText;
		}
		
		void ImageViewTab::zoomImage(float factor)
		{
			m_scaling->reset(m_scaling->value() * factor);
			m_view->scale(factor);
			m_scaling->action(Scaling::In)->setEnabled(m_scaling->value() < m_scaling->max());
			m_scaling->action(Scaling::Out)->setEnabled(m_scaling->value() > m_scaling->min());
		}
		
		void ImageViewTab::init()
		{
			m_topLayout = new QGridLayout(this);
			m_widgetLayout = new QHBoxLayout();
			m_demographicsText = new QGraphicsSimpleTextItem();
			m_scene->addWidget(m_renderer);
			m_scene->addItem(m_demographicsText);
			m_view->fitInView(m_scene->sceneRect(), Qt::KeepAspectRatioByExpanding);
			m_view->setBackgroundRole(QPalette::ColorRole::Dark);
			m_view->setScene(m_scene);
			m_view->show();
			m_scaling->reset();
			m_widgetLayout->addWidget(m_view);
			m_widgetLayout->addWidget(m_controls);
			
			initActions(m_topLayout);
			
			m_topLayout->addItem(m_widgetLayout, 1, 0);
			m_scaling->action(Scaling::Fit)->setEnabled(true);
			setLayout(m_topLayout);
			updateActions();
		}
		
		void ImageViewTab::initActions(QLayout* layout)
		{
			auto toolBar = new QToolBar(this);
			toolBar->setOrientation(Qt::Orientation::Horizontal);
			toolBar->setForegroundRole(QPalette::ColorRole::Dark);
			m_actionTable = { };
			auto zoomInAct = m_scaling->addAction(Scaling::In,
			                                      toolBar->addAction(QIcon::fromTheme("image-zoom-in"),
			                                                         tr("Zoom &In (%1)").arg(m_scaling->caption()),
			                                                         this, &ImageViewTab::zoomIn));
			
			zoomInAct->setShortcut(QKeySequence::ZoomIn);
			zoomInAct->setEnabled(false);
			toolBar->addAction(zoomInAct);
			
			//Annotation
			auto zoomFitAct = m_scaling->addAction(Scaling::Fit,
			                                       new QAction("Fit to Window", this));
			zoomFitAct->setEnabled(false);
			zoomFitAct->setCheckable(true);
			zoomFitAct->setShortcut(tr("Ctrl+F"));
			
			auto zoomResetAct = m_scaling->addAction(Scaling::Reset,
			                                         new QAction("Reset", this));
			zoomResetAct->setShortcut(QKeySequence::Backspace);
			zoomResetAct->setEnabled(false);
			
			auto zoomOutAct = m_scaling->addAction(Scaling::Out,
			                                       toolBar->addAction(QIcon::fromTheme("image-zoom-out"),
			                                                          tr("Zoom &Out (%1)").arg(m_scaling->caption()),
			                                                          this, &ImageViewTab::zoomOut));
			
			zoomOutAct->setShortcut(QKeySequence::ZoomOut);
			zoomOutAct->setEnabled(false);
			
			m_scaling->addComboAction(zoomFitAct);
			m_scaling->addComboAction(zoomResetAct);
			
			toolBar->addAction(zoomOutAct);
			toolBar->addSeparator();
			
			connect(zoomFitAct, SIGNAL(triggered(bool)), this, SLOT(zoomFit()));
			connect(zoomResetAct, SIGNAL(triggered(bool)), this, SLOT(zoomReset()));
			connect(m_controls, SIGNAL(submitData(
					                           const ControlData&)),
			        this, SLOT(start(
					                   const ControlData&)));
			connect(m_controls, SIGNAL(clearData(bool)),
			        this, SLOT(reset(bool)));
			
			layout->addWidget(toolBar);
		}
		
		void ImageViewTab::updateActions()
		{
			m_scaling->toggle(Scaling::In);
			m_scaling->toggle(Scaling::Out);
			m_scaling->toggle(Scaling::Reset);
		}
		
		void ImageViewTab::zoomIn()
		{
			zoomImage(m_scaling->last());
		}
		
		void ImageViewTab::zoomOut()
		{
			zoomImage(m_scaling->first());
		}
		
		void ImageViewTab::zoomReset()
		{
			m_scaling->reset();
			m_view->scale(m_scaling->value());
		}
		
		void ImageViewTab::zoomFit()
		{
			if (!m_scaling->toggle(Scaling::Fit))
			{
				zoomReset();
				updateActions();
			}
		}
		
		void ImageViewTab::wheelEvent(QWheelEvent* event)
		{
			if (event->delta() > 0
			    && m_scaling->action(Scaling::In)->isEnabled())
			{
				zoomIn();
			} else if (event->delta() < 0
			           && m_scaling->action(Scaling::Out)->isEnabled())
			{
				zoomOut();
			}
		}
		
		void ImageViewTab::start(const ControlData& data)
		{
			m_faceDetector->refetch(true, data);
			
			if (m_faceDetector->fetch())
			{
				m_faceDetector->run();
				m_renderer->setPoints(m_faceDetector->getLandmarks());
				m_renderer->setBox(m_faceDetector->getBox());
				m_demographicsText->setText(m_faceDetector->getDemographics().getAsText());
				
				if (!m_faceDetector->getLandmarks().isEmpty())
				{
					auto coordinates = Math::minmax4D(m_faceDetector->getLandmarks());
					m_demographicsText->setScale(0.5);
					m_demographicsText->setPen(QPen(m_controls->currentColor()));
					m_demographicsText->setPos(
							coordinates.first().x(),
							coordinates.first().y() - 50
					);
				} else
				{
					m_demographicsText->setScale(2);
					m_demographicsText->setText("Undefined");
					m_demographicsText->setPos(m_renderer->center());
				}
				m_demographicsText->show();
			} else
			{
				m_renderer->update();
			}
		}
		
		void ImageViewTab::reset(bool reset)
		{
			m_renderer->clear(reset);
			m_demographicsText->setPen(Qt::NoPen);
			m_demographicsText->hide();
		}
		
	}// namespace Gui
}// namespace Tevian