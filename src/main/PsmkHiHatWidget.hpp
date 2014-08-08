/**
 * PsmkHiHatWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef PSMKHIHATWIDGET_HPP
#define PSMKHIHATWIDGET_HPP PSMKHIHATWIDGET_HPP

/* base class */
#include <QtGlobal>
#if QT_VERSION < 0x050000
#include <QGroupBox>
#else
#include <QtWidgets/QGroupBox>
#endif

/* system headers */

/* Qt headers */
#include <QVariant>

/* local library headers */

/* local headers */
#include "PsmkConfig.hpp"

/* forward declaration of Qt classes */
class QBoxLayout;
class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QSpinBox;
class QSignalMapper;

/* forward declaration of local classes */
class PsmkToneComboBox;
class PsmkPitchComboBox;

/*!
 \brief widget for editing \ref SlocumHiHat

*/
class PsmkHiHatWidget : public QGroupBox
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param slocumSong
    \param parent
   */
   explicit PsmkHiHatWidget( QWidget *parent = 0 );
   /*!
    \brief constructor

    Not used right now, intended for editing a hihat configuration outside of
    a song.

    \param slocumHiHat
    \param parent
   */
   explicit PsmkHiHatWidget( const QVariantMap &variantMap, QWidget *parent = 0 );
   /*!
    \brief destrutor

   */
   virtual ~PsmkHiHatWidget();

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

    \param variantMap
   */
   bool fromVariantMap( const QVariantMap &variantMap );
   /*!
    \brief \todo

    format: 1 byte type, 1 byte pitch, 1 byte volume, 4 bytes pattern

    \return QByteArray
   */
   QByteArray toBinary();

public slots:
   /*!
    \brief set text of widgets

    internally used by constructor, must be re-called when translations change

   */
   void setTexts();

signals:
   /*!
    \brief \todo

   */
   void changed();

protected:
   /*!
    \brief handle the context menu

    \param event
   */
   void contextMenuEvent( QContextMenuEvent *event );

private:
   Q_DISABLE_COPY( PsmkHiHatWidget )

   static const char       cMimeType[]; /*!< \brief mime type for dragging */

   /*!
    \brief internally used by constructors

   */
   void setup();

   QLabel            *mpLabelName; /*!< \brief \todo */
   QLabel            *mpLabelTypePitch; /*!< \brief \todo */
   QLabel            *mpLabelVolume; /*!< \brief \todo */
   QLineEdit         *mpValueName; /*!< \brief value for name */
   PsmkToneComboBox  *mpValueTone; /*!< \brief value for sound */
   PsmkPitchComboBox *mpValuePitch; /*!< \brief value for pitch */
   QSpinBox          *mpValueVolume; /*!< \brief value for volume */
   QCheckBox         *mpPattern[PsmkConfig::BeatsInHiHat]; /*!< \brief checkboxes for hihat pattern */
};

#endif // PSMKHIHATWIDGET_HPP
