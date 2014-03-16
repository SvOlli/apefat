/**
 * PsmkInstrumentWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef PSMKINSTRUMENTWIDGET_HPP
#define PSMKINSTRUMENTWIDGET_HPP PSMKINSTRUMENTWIDGET_HPP

/* base class */
#include <QGroupBox>

/* system headers */

/* Qt headers */
#include <QVariant>

/* local library headers */

/* local headers */
#include "PsmkConfig.hpp"

/* forward declaration of Qt classes */
class QCheckBox;
class QGridLayout;
class QLabel;
class QLineEdit;
class QSpinBox;
class QSignalMapper;

/* forward declaration of local classes */
class PsmkToneComboBox;

/*!
 \brief widget for setting \ref SlocumSong

 Set the sound configuration for a song: 8 "instruments" using one combination
 of a type (AUDCx) with an attenuation.

*/
class PsmkInstrumentsWidget : public QGroupBox
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param parent
   */
   explicit PsmkInstrumentsWidget( QWidget *parent = 0 );
   /*!
    \brief constructor

    \param data
    \param parent
   */
   explicit PsmkInstrumentsWidget( const QVariantMap &variantMap, QWidget *parent = 0 );
   /*!
    \brief destructor

   */
   virtual ~PsmkInstrumentsWidget();

   /*!
    \brief return the defined mime type

    \return const char
   */
   static const char *mimeType();
   /*!
    \brief \todo

    \return QVariantList
   */
   QVariantMap toVariantMap() const;
   /*!
    \brief \todo

    \param data
   */
   bool fromVariantMap( const QVariantMap &variantMap );
   /*!
    \brief \todo

    format 8 bytes type, 8 bytes attenuation

    \return QByteArray
   */
   QByteArray toBinary();
   /*!
    \brief \todo

    \param index
    \return quint8
   */
   quint8 instrument( int index );

public slots:
   /*!
    \brief set all text labels

    triggered internally by constructor, must be retriggered after translation
    changes

   */
   void setTexts();
   /*!
    \brief handler for reading a changed type configuration

    \param index
   */
   void typeChanged( int index );

signals:
   /*!
    \brief \todo

    \param index
    \param value
   */
   void instrumentChanged( int index, quint8 value );

protected:
   /*!
    \brief handle the context menu

    \param event
   */
   void contextMenuEvent( QContextMenuEvent *event );

private:
   Q_DISABLE_COPY( PsmkInstrumentsWidget )

   static const char       cMimeType[]; /*!< \brief mime type for dragging */

   /*!
    \brief \todo

   */
   void setup();

   QSignalMapper           *mpTypesMapper; /*!< \brief mapper for \ref typeChanged */
   QGridLayout             *mpMainLayout; /*!< \brief layout */
   QLabel                  *mpLabelName; /*!< \brief label for name */
   QLabel                  *mpLabelID; /*!< \brief label for id */
   QLabel                  *mpLabelType; /*!< \brief label for type */
   QLabel                  *mpLabelAtten; /*!< \brief label for atten */
   QLineEdit               *mpValueName; /*!< \brief line edit for name */
   PsmkToneComboBox        *mpTypes[PsmkConfig::InstrumentsInSong]; /*!< \brief edit widget for type */
   QSpinBox                *mpAttens[PsmkConfig::InstrumentsInSong]; /*!< \brief edit widgets for attenuations */
};

#endif // PSMKINSTRUMENTWIDGET_HPP
