/**
 * PsmkPatternProxyWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "PsmkPatternProxyWidget.hpp"

/* system headers */

/* Qt headers */
#if QT_VERSION < 0x050000
#include <QApplication>
#include <QBoxLayout>
#include <QLabel>
#else
#include <QtWidgets/QApplication>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QLabel>
#endif
#include <QClipboard>
#include <QMimeData>
#include <QTextEdit>

/* local library headers */

/* local headers */
#include "PsmkBeatWidget.hpp"
#include "PsmkHiHatWidget.hpp"
#include "PsmkInstrumentsWidget.hpp"
#include "PsmkNoteWidget.hpp"
#include "PsmkPatternWidget.hpp"


PsmkPatternProxyWidget::PsmkPatternProxyWidget( PsmkPatternWidget *patternWidget,
                                                const PsmkPacker *packer,
                                                QWidget *parent )
: QGroupBox( parent )
, mpPsmkPatternWidget( patternWidget )
, mpLabelBeat()
{
   QStringList names( mpPsmkPatternWidget->names( packer ) );
   Q_ASSERT( names.size() == (PsmkConfig::BeatsInPattern + 1) );
   QBoxLayout *layout = new QVBoxLayout( this );
   setTitle( names.at(0) );
   for( int i = 0; i < PsmkConfig::BeatsInPattern; ++i )
   {
      mpLabelBeat[i] = new QLabel( names.at(i+1), this );
      layout->addWidget( mpLabelBeat[i] );
   }
   setTexts();
}


PsmkPatternProxyWidget::~PsmkPatternProxyWidget()
{
}


PsmkPatternWidget *PsmkPatternProxyWidget::patternWidget() const
{
   return mpPsmkPatternWidget;
}


void PsmkPatternProxyWidget::setTexts()
{
}
