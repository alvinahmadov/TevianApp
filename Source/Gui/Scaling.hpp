/**
 *  Copyright (C) 2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 */
#pragma once


#include <memory>

#include <QString>
#include <QPair>
#include <QComboBox>


namespace Tevian
{
	namespace Helpers
	{
		using ZoomBox = std::shared_ptr<QComboBox>;
		
		/**
		 * \author Alvin Ahmadov
		 * \namespace Tevian::Helpers
		 *
		 * \brief Holds image scaling properties, actions and zoombox.
		 */
		class Scaling
		{
		public:
			enum ZoomDirection
			{
				In,
				Out,
				Fit,
				Reset,
				FitWidth,
				FitHeight
			};
			
			using ZoomOption = QPair<QString, QPair<float, float>>;
			
			/**
			 * \param _factor zooming value.
			 * \param w parent widget of zoombox.
			 *
			 * \brief Holds image scaling properties, actions and zoombox.
			 * */
			explicit Scaling(float _factor = 1.0, QWidget* w = nullptr);
			
			/**
			 * \author Alvin Ahmadov
			 * \namespace Tevian::Helpers
			 *
			 * \param _factor zooming value.
			 * \param option option value holding zoom details, like it's
			 * representation and zooming factors.
			 * \param w parent widget of zoombox.
			 * */
			explicit Scaling(float _factor, ZoomOption&& option, QWidget* w = nullptr);
			
			// Mutators
			
			void addComboAction(QAction* action);
			
			QComboBox* zoomBox();
			
			/**
			 * \brief Resets zooming value
			 * */
			void reset(float value = 1.0);
			
			/**
			 * \brief Adds action to action table.
			 * */
			QAction* addAction(ZoomDirection direction, QAction* action);
			
			/**
			 * \returns action from action table.
			 * \param direction aktion key
			 * */
			QAction* action(ZoomDirection direction);
			
			/**
			 * \brief Enables/disables action depending on
			 * zooming limits and current value
			 * */
			bool toggle(ZoomDirection direction);
			
			// Accessors
			/**
			 * \returns Zooming value string
			 * */
			const QString& caption() const;
			
			/**
			 * \returns Minimum zooming factor
			 * */
			const float& first() const;
			
			/**
			 * \returns Maximum zooming factor
			 * */
			const float& last() const;
			
			/**
			 * \returns Minimum zooming limit.
			 *
			 * Returned value depends on the value of \c first()
			 * */
			float min() const;
			
			/**
			 * \returns Maximum zooming limit.
			 *
			 * Returned value depends on the value of \c last()
			 * */
			float max() const;
			
			/**
			 * \returns value.
			 * */
			float value() const;
		
		private:
			float m_value;
			
			float m_max;
			
			float m_min;
			
			ZoomOption m_zoomOption;
			
			ZoomBox m_zoomBox;
			
			QMap<ZoomDirection, QAction*>
					m_actionTable;
		};
	}// namespace Helpers
}// namespace Tevian