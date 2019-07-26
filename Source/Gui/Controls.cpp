/**
 *  Copyright (C) 2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 */

#include "Defines.hpp"
#include "Gui/Controls.hpp"
#include "FaceDetector.hpp"
#include "Gui/DetectionRenderer.hpp"

#include "Settings.hpp"

#include <QRadioButton>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QButtonGroup>
#include <QGroupBox>
#include <QDebug>
#include <QSlider>
#include <QLabel>
#include <QLineEdit>
#include <QMouseEvent>
#include <QDir>
#include <QtWidgets/QFormLayout>


namespace Tevian
{
	namespace Gui
	{
		using RadioPair = std::pair<QRadioButton*, QRadioButton*>;
		
		Controls::Controls(QWidget* parent, DetectionRenderer* renderer,
		                   Client::FaceApi* api)
				: QWidget(parent)
		{
			setVisible(true);
			initControls(renderer);
			setBackgroundRole(QPalette::ColorRole::Shadow);
		}
		
		void Controls::initControls(DetectionRenderer* renderer)
		{
			auto mainGroup = new QGroupBox(this);
			setFixedWidth(300);
			setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
			
			mainGroup->setTitle(tr("Face line"));
			
			initDataGroup(mainGroup);
			initStyleGroup(mainGroup, renderer);
			
			auto submitButton = new QPushButton(tr("Detect"), this);
			auto clearButton = new QPushButton(tr("Clear"), this);
			
			auto mainLayout = new QVBoxLayout();
			mainLayout->setMargin(0);
			mainLayout->addWidget(mainGroup);
			
			auto mainGroupLayout = new QVBoxLayout(mainGroup);
			mainGroupLayout->setMargin(3);
			
			mainGroupLayout->addWidget(m_dataGroup);
			mainGroupLayout->addWidget(m_styleGroup);
			
			auto buttonLayout = new QHBoxLayout();
			buttonLayout->addWidget(submitButton);
			buttonLayout->addWidget(clearButton);
			
			mainLayout->addLayout(buttonLayout);
			
			connect(submitButton, SIGNAL(clicked()), SLOT(sendData()));
			connect(clearButton, SIGNAL(clicked()), SLOT(clearData()));
			
			setLayout(mainLayout);
		}
		
		void Controls::initDataGroup(QWidget* parent)
		{
			m_dataGroup = new QGroupBox(parent);
			m_dataGroup->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
			auto minEdit = new QLineEdit(m_dataGroup);
			auto maxEdit = new QLineEdit(m_dataGroup);
			auto thresholdEdit = new QLineEdit(m_dataGroup);
			auto facebox1Edit = new QLineEdit(m_dataGroup);
			minEdit->setPlaceholderText("Minimal size");
			maxEdit->setPlaceholderText("Maximal size");
			thresholdEdit->setPlaceholderText("Threshold");
			facebox1Edit->setPlaceholderText(tr("x, y, width, height"));
			auto demographicsCheck = new QCheckBox(tr("Demographics"), m_dataGroup);
			auto attributesCheck = new QCheckBox(tr("Attributes"), m_dataGroup);
			auto landmarksCheck = new QCheckBox(tr("Landmarks"), m_dataGroup);
			
			auto dataGroupLayout = new QFormLayout(m_dataGroup);
			dataGroupLayout->addRow(minEdit);
			dataGroupLayout->addRow(maxEdit);
			dataGroupLayout->addWidget(thresholdEdit);
			dataGroupLayout->addWidget(facebox1Edit);
			dataGroupLayout->addWidget(demographicsCheck);
			dataGroupLayout->addWidget(attributesCheck);
			dataGroupLayout->addWidget(landmarksCheck);
			
			// Default:
			connect(minEdit, SIGNAL(textEdited(
					                        const QString&)),
			        this, SLOT(minSizeEdited(
					                   const QString&)));
			
			connect(maxEdit, SIGNAL(textEdited(
					                        const QString&)),
			        this, SLOT(maxSizeEdited(
					                   const QString&)));
			
			connect(thresholdEdit, SIGNAL(textEdited(
					                              const QString&)),
			        this, SLOT(thresholdEdited(
					                   const QString&)));
			
			connect(facebox1Edit, SIGNAL(textEdited(
					                             const QString&)),
			        this, SLOT(face1Edited(
					                   const QString&)));
			
			// connect(facebox2Edit, SIGNAL(textEdited(const QString&)),
			//         this, SLOT(face2Edited(const QString&)));
			
			connect(demographicsCheck, SIGNAL(toggled(bool)),
			        this, SLOT(demographicsToggled(bool)));
			
			connect(attributesCheck, SIGNAL(toggled(bool)),
			        this, SLOT(attributesToggled(bool)));
			
			connect(landmarksCheck, SIGNAL(toggled(bool)),
			        this, SLOT(landmarksToggled(bool)));
			
			landmarksCheck->setChecked(true);
			demographicsCheck->setChecked(true);
		}
		
		void Controls::initStyleGroup(QWidget* parent, DetectionRenderer* renderer)
		{
			// Init:
			m_styleGroup = new QGroupBox(parent);
			auto solidLine = new QRadioButton(m_styleGroup);
			auto dashLine = new QRadioButton(m_styleGroup);
			auto dotLine = new QRadioButton(m_styleGroup);
			auto dashDotLine = new QRadioButton(m_styleGroup);
			auto dashDotDotLine = new QRadioButton(m_styleGroup);
			m_styleGroup->setTitle(tr("Pen Style"));
			
			// Set child parameters:
			QPixmap line_solid(":Resources/images/line_solid.png");
			solidLine->setIcon(line_solid);
			solidLine->setIconSize(line_solid.size());
			QPixmap line_dashed(":Resources/images/line_dashed.png");
			dashLine->setIcon(line_dashed);
			dashLine->setIconSize(line_dashed.size());
			QPixmap line_dotted(":Resources/images/line_dotted.png");
			dotLine->setIcon(line_dotted);
			dotLine->setIconSize(line_dotted.size());
			QPixmap line_dash_dot(":Resources/images/line_dash_dot.png");
			dashDotLine->setIcon(line_dash_dot);
			dashDotLine->setIconSize(line_dash_dot.size());
			QPixmap line_dash_dot_dot(":Resources/images/line_dash_dot_dot.png");
			dashDotDotLine->setIcon(line_dash_dot_dot);
			dashDotDotLine->setIconSize(line_dash_dot_dot.size());
			
			int fixedHeight = 50;
			solidLine->setFixedHeight(fixedHeight);
			dashLine->setFixedHeight(fixedHeight);
			dotLine->setFixedHeight(fixedHeight);
			dashDotLine->setFixedHeight(fixedHeight);
			dashDotDotLine->setFixedHeight(fixedHeight);
			
			// Set Layout:
			auto styleGroupLayout = new QVBoxLayout(m_styleGroup);
			styleGroupLayout->addWidget(solidLine);
			styleGroupLayout->addWidget(dashLine);
			styleGroupLayout->addWidget(dotLine);
			styleGroupLayout->addWidget(dashDotLine);
			styleGroupLayout->addWidget(dashDotDotLine);
			
			// Connections:
			connect(solidLine, SIGNAL(clicked()), renderer, SLOT(setSolidLine()));
			connect(dashLine, SIGNAL(clicked()), renderer, SLOT(setDashLine()));
			connect(dotLine, SIGNAL(clicked()), renderer, SLOT(setDotLine()));
			connect(dashDotLine, SIGNAL(clicked()), renderer, SLOT(setDashDotLine()));
			connect(dashDotDotLine, SIGNAL(clicked()), renderer, SLOT(setDashDotDotLine()));
			
			// Default:
			solidLine->setChecked(true);
		}
		
		void Controls::sendData()
		{
			emit submitData(m_controlData);
		}
		
		void Controls::clearData()
		{
			emit clearData(true);
		}
		
		void Controls::minSizeEdited(const QString& text)
		{
			m_controlData._min_size = text.toInt();
		}
		
		void Controls::maxSizeEdited(const QString& text)
		{
			m_controlData._max_size = text.toInt();
		}
		
		void Controls::thresholdEdited(const QString& text)
		{
			m_controlData._threshold = text.toFloat();
		}
		
		void Controls::face1Edited(const QString& text)
		{
			auto chunks = text.split(',');
			if (chunks.size() == 4)
			{
				for (int i = 0; i < chunks.size(); ++i)
				{
					m_controlData._face[i] = chunks[i].toInt();
				}
			}
		}
		
		void Controls::demographicsToggled(bool b)
		{
			m_controlData._demographics = b;
		}
		
		void Controls::attributesToggled(bool b)
		{
			m_controlData._attributes = b;
		}
		
		void Controls::landmarksToggled(bool b)
		{
			m_controlData._landmarks = b;
		}
		
	}// namespace Gui
}// namespace Tevian

#include <moc_Controls.cpp>