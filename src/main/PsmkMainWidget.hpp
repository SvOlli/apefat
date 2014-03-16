/**
 * PsmkMainWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef PSMKMAINWIDGET_HPP
#define PSMKMAINWIDGET_HPP PSMKMAINWIDGET_HPP

/* base class */
#include <QWidget>

/* system headers */

/* Qt headers */
#include <QVariant>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QLabel;
class QLineEdit;
class QPushButton;
class QSpinBox;
class QStackedWidget;

/* forward declaration of local classes */
class PsmkHiHatWidget;
class PsmkInstrumentsWidget;
class PsmkPacker;
class PsmkPatternSelector;
class PsmkPlayerWidget;


/*!
 \brief representing a tab for a bar

 This widget is used to show a tab for a bar with all the buttons for moving,
 etc.

*/
class PsmkMainWidget : public QWidget
{
   Q_OBJECT

   friend class PsmkPacker;

public:
   /*!
    \brief constructor

    \param slocumSong
    \param parent
   */
   explicit PsmkMainWidget( QWidget *parent = 0 );
   /*!
    \brief destructor

   */
   virtual ~PsmkMainWidget();
   /*!
    \brief \todo

    \return QVariantList
   */
   QVariantMap toVariantMap();
   /*!
    \brief \todo

    \param list
    \return success
   */
   bool fromVariantMap( const QVariantMap &variantMap );
   /*!
    \brief \todo

    \param main
    \return QByteArray
   */
   QByteArray toSourceCode();

public slots:
   /*!
    \brief \todo

   */
   void updateSong();
   /*!
    \brief set translatable texts

   */
   void setTexts();
   /*!
    \brief \todo

   */
   void updatePacker();

protected:

signals:
   /*!
    \brief \todo

    \param bar
   */
   void startPlay( int bar );

private:
   Q_DISABLE_COPY( PsmkMainWidget )

   void setup();
   bool fromVariantMapSlocum( const QVariantMap &variantMap );
   bool fromVariantMapPsmk( const QVariantMap &variantMap );

   QLabel                  *mpLabelName;
   QLineEdit               *mpValueName;
   QLabel                  *mpLabelDelay;
   QSpinBox                *mpValueDelay;
   PsmkPatternSelector     *mpPsmkPatternSelector;
   PsmkPacker              *mpPsmkPacker;
   PsmkInstrumentsWidget   *mpPsmkInstrumentsWidget;
   PsmkHiHatWidget         *mpPsmkHiHatWidget;
   PsmkPlayerWidget        *mpPsmkPlayerWidget;
   QLabel                  *mpLeadInLabel; /*!< \brief label for left text */
   QLabel                  *mpLeadOutLabel; /*!< \brief label for right text */
};

#endif // PSMKMAINWIDGET_HPP
