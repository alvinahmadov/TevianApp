/**
 *  Copyright (C) 2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 */
#pragma once


#include "FaceDetector.hpp"
#include "Gui/Scaling.hpp"
#include "Gui/Controls.hpp"

#include <QWidget>
#include <QtGui/QStaticText>


class QToolButton;

class QGraphicsScene;

class QGridLayout;

class QAction;

class QGraphicsSimpleTextItem;

class QHBoxLayout;

namespace Tevian
{
	namespace Client
	{
		class FaceApi;
	}
	
	namespace Gui
	{
		class ViewPort;
		
		class DetectionRenderer;
		
		/**
		 * \author Alvin Ahmadov
		 * \namespace Tevian::Gui
		 *
		 * \brief Image viewer
		 * */
		class TEVIAN_API ImageViewTab : public QWidget
		{
		Q_OBJECT
		
		public:
			/// ctor
			explicit ImageViewTab(const QString& imagefile,
			                      QWidget* parent = nullptr);
			
			/// dtor
			~ImageViewTab() Q_DECL_OVERRIDE;
			
			bool setDetector(FaceDetector* detector)
			{
				m_faceDetector = detector;
				return m_faceDetector != nullptr;
			}
		
		public slots:
			void render(bool render)
			{
				setVisible(render);
			};
			
		private:
			void init();
			
			void initActions(QLayout* layout);
			
			void zoomImage(float factor);
			
			void updateActions();
		
		private slots:
			/**
			 * \brief Function that detects face lines
			 * */
			void start(const ControlData& data);
			
			void reset(bool);
			
			void zoomIn();
			
			void zoomOut();
			
			void zoomReset();
			
			void zoomFit();
			
			void wheelEvent(QWheelEvent* e) Q_DECL_OVERRIDE;
		
		private:
			QString m_file;
			
			Helpers::Scaling*
					m_scaling;
			
			FaceDetector* m_faceDetector;
			
			ViewPort* m_view;
			
			QGraphicsScene* m_scene;
			
			DetectionRenderer* m_renderer;
			
			QToolButton* m_annotateButton;
			
			QGridLayout* m_topLayout;
			
			QHBoxLayout* m_widgetLayout;
			
			QMap<QString, QAction*> m_actionTable;
			
			Controls* m_controls;
			
			QGraphicsSimpleTextItem* m_demographicsText;
			bool draw;
		};
	}// namespace Gui
}// namespace Tevian