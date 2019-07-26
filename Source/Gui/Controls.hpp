/**
 *  Copyright (C) 2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 */
#pragma once


#include "Defines.hpp"
#include "FaceData.hpp"

#include <QWidget>
#include <QPushButton>


class QRadioButton;

class QWidget;

class QGroupBox;

namespace Tevian
{
	namespace Client
	{
		class FaceApi;
	}
	
	namespace Gui
	{
		class DetectionRenderer;
		
		class TEVIAN_API Controls : public QWidget
		{
		Q_OBJECT
		public:
			Controls(QWidget* parent, DetectionRenderer* renderer, Client::FaceApi* detector);
		
		signals:
			
			/**
			 * \brief Signal emitted when user press ok.
			 *
			 * \details Submitted data will be sent to
			 * parent class to update fetched data
			 * */
			void submitData(const ControlData& data);
			
			void clearData(bool);
		
		private:
			
			void initControls(DetectionRenderer* renderer);
			
			/**
			 * \brief Init api data.
			 * */
			void initDataGroup(QWidget* parent);
			
			void initStyleGroup(QWidget* parent, DetectionRenderer* renderer);
		
		private slots:
			
			/**
			 * \brief emit signal \code submitData(const ControlData&) \endcode
			 *
			 * \see \code Controls::submitData(const ControlData&) \endcode
			 * */
			void sendData();
			
			void clearData();
			
			void minSizeEdited(const QString&);
			
			void maxSizeEdited(const QString&);
			
			void thresholdEdited(const QString&);
			
			void face1Edited(const QString&);
			
			void demographicsToggled(bool);
			
			void attributesToggled(bool);
			
			void landmarksToggled(bool);
		
		private:
			bool m_drag;
			
			QGroupBox* m_styleGroup;
			
			QGroupBox* m_dataGroup;
			
			QGroupBox* m_dataOptionsGroup;
			
			QGroupBox* m_pathModeGroup;
			
			QPushButton* m_okButton;
			
			DetectData m_detectData;
			
			MatchData m_matchData;
			
			ControlData m_controlData;
			
		};
	}   // namespace Gui
}       // namespace Tevian