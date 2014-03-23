/**
 * PsmkPatternProxyWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef PSMKPATTERNPROXYWIDGET_HPP
#define PSMKPATTERNPROXYWIDGET_HPP PSMKPATTERNPROXYWIDGET_HPP

/* base class */
#include <QGroupBox>

/* system headers */

/* Qt headers */
#include <QString>
#include <QVariant>

/* local library headers */

/* local headers */
#include "PsmkConfig.hpp"

/* forward declaration of Qt classes */
class QLabel;
class QTextEdit;

/* forward declaration of local classes */
class PsmkPatternWidget;

/*!
 \brief \todo

*/
class PsmkPatternProxyWidget : public QGroupBox
{
   Q_OBJECT

public:
   explicit PsmkPatternProxyWidget( PsmkPatternWidget *patternWidget, QWidget *parent = 0 );
   virtual ~PsmkPatternProxyWidget();

   PsmkPatternWidget *patternWidget() const;

public slots:
   void setTexts();

signals:

private:
   Q_DISABLE_COPY( PsmkPatternProxyWidget )

   PsmkPatternWidget       *mpPsmkPatternWidget;
   QLabel                  *mpLabelBeat[PsmkConfig::BeatsInPattern];
};

#endif
