/**
 *  Copyright (C) 2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 */

#include <cmath>

#include "Scaling.hpp"

#include <QVariantList>
#include <QException>
#include <QMap>
#include <QAction>


namespace Tevian
{
	namespace Helpers
	{
		Scaling::Scaling(float _factor, QWidget* w)
				: Scaling(_factor, ZoomOption({ "25%", { 0.75, 1.25 } }), w)
		{ }
		
		Scaling::Scaling(float _factor, ZoomOption&& option, QWidget* w)
				: m_value(_factor),
				  m_zoomOption(std::move(option)),
				  m_zoomBox(new QComboBox(w)),
				  m_actionTable { }
		{
			m_max = std::pow(m_zoomOption.second.second, 3);
			m_min = m_zoomOption.second.first;
			m_zoomBox->setInsertPolicy(QComboBox::InsertPolicy::InsertAfterCurrent);
		}
		
		const QString&
		Scaling::caption() const
		{
			return m_zoomOption.first;
		}
		
		const float&
		Scaling::first() const
		{
			return m_zoomOption.second.first;
		}
		
		const float&
		Scaling::last() const
		{
			return m_zoomOption.second.second;
		}
		
		float Scaling::max() const
		{
			return m_max;
		}
		
		float Scaling::value() const
		{
			return m_value;
		}
		
		void Scaling::addComboAction(QAction* action)
		{
			
			m_zoomBox->addAction(action);
			m_zoomBox->actions().front()->setText(action->text());
		}
		
		QComboBox* Scaling::zoomBox()
		{
			return m_zoomBox.get();
		}
		
		float Scaling::min() const
		{
			return m_min;
		}
		
		void Scaling::reset(float value)
		{
			m_value = value;
		}
		
		QAction* Scaling::addAction(ZoomDirection direction, QAction* action)
		{
			Q_ASSERT(action);
			
			m_actionTable.insert(direction, action);
			return action;
		}
		
		QAction*
		Scaling::action(ZoomDirection direction)
		{
			Q_ASSERT(m_actionTable.contains(direction));
			return m_actionTable[direction];
		}
		
		bool Scaling::toggle(ZoomDirection direction)
		{
			if (!m_actionTable.contains(direction))
			{
				return false;
			}
			
			bool checked = m_actionTable[direction]->isChecked();
			m_actionTable[direction]->setEnabled(!checked);
			
			return checked;
		}
		
	}// namespace Helpers
}// namespace Tevian