/**
 * PsmkPatternWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef PSMKPATTERNWIDGET_HPP
#define PSMKPATTERNWIDGET_HPP PSMKPATTERNWIDGET_HPP

/* base class */
#include <QtGlobal>
#if QT_VERSION < 0x050000
#include <QWidget>
#else
#include <QtWidgets/QWidget>
#endif

/* system headers */

/* Qt headers */
#include <QVariantMap>

/* local library headers */

/* local headers */
#include "PsmkConfig.hpp"

/* forward declaration of Qt classes */
class QBoxLayout;
class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;

/* forward declaration of local classes */
class PsmkBeatWidget;
class PsmkPacker;
class QxtLabel;

/*!
 \brief widget for editing a pattern

 a pattern consists of a sequence of 4 beats putting them together into one
 bar/measure

*/
class PsmkPatternWidget : public QWidget
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param parent
   */
   explicit PsmkPatternWidget( QWidget *parent = 0 );
   /*!
    \brief constructor

    \param instruments
    \param parent
   */
   explicit PsmkPatternWidget( quint8 *instruments,
                               QWidget *parent = 0 );
   /*!
    \brief destructor

   */
   virtual ~PsmkPatternWidget();
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

    format: 1 byte of hi/lo indication, 4 x 9 byte of beat data

    \return QByteArray
   */
   QByteArray toBinary() const;
   /*!
    \brief \todo

    get the names of the pattern and beats in a list

    \return QStringList
   */
   QStringList names( const PsmkPacker *psmkPacker = 0 ) const;
   /*!
    \brief \todo

    \return PsmkBeatWidget*
   */
   PsmkBeatWidget *beat( int i ) const;
   /*!
    \brief \todo

    \return PsmkBeatWidget*
   */
   PsmkPatternWidget *getClone( QWidget *parent = 0 ) const;

public slots:
   /*!
    \brief \todo

    \param index
    \param tone
   */
   void setInstrument( int index, quint8 tone );
   /*!
    \brief \todo

    \param tones[]
   */
   void setInstruments( quint8 tones[8] );
   /*!
    \brief set translatable texts

   */
   void setTexts();

signals:
   /*!
    \brief \todo

   */
   void changed();

protected:
   /*!
    \brief handler reimplemented for dropping beats

    \param event
   */
   void dragEnterEvent( QDragEnterEvent *event );
   /*!
    \brief handler reimplemented for dropping beats

    \param event
   */
   void dragMoveEvent( QDragMoveEvent *event );
   /*!
    \brief handler reimplemented for dropping beats

    \param event
   */
   void dragLeaveEvent( QDragLeaveEvent *event );
   /*!
    \brief handler reimplemented for dropping beats

    \param event
   */
   void dropEvent( QDropEvent *event );
   /*!
    \brief handle the context menu

    \param event
   */
   void contextMenuEvent( QContextMenuEvent *event );

private:
   Q_DISABLE_COPY( PsmkPatternWidget )

   /*!
    \brief \todo

    \param instruments
   */
   void setup( quint8 *instruments = 0 );
   /*!
    \brief used internally for moving a beat inside a bar

    \param from
    \param to
   */
   void moveBeat( int from, int to );
   /*!
    \brief used internally for finding a \ref PsmkBeatWidget by its position inside \ref PsmkPatternWidget

    \param pos
    \return BeatWidget
   */
   PsmkBeatWidget *beatWidgetAt( const QPoint &pos );

   static const char       cMimeType[]; /*!< \brief mime type */

   QxtLabel                *mpTypeName; /*!< \brief \todo TODO */
   QLabel                  *mpLabelName; /*!< \brief label for name */
   QLineEdit               *mpValueName; /*!< \brief value for name */
   QCheckBox               *mpLowVolume; /*!< \brief value for low volume bar */
   PsmkBeatWidget          *mpBeats[PsmkConfig::BeatsInPattern]; /*!< \brief list of beats in this bar */
   PsmkPacker              *mpPacker; /*! \brief for obtaining statistics */
   QVariantMap             mSavePattern; /*!< \brief bar saved for undoing drag'n'drop */
};

#endif // PSMKPATTERNWIDGET_HPP
