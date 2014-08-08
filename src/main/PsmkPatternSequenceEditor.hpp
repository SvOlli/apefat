/**
 * PsmkPatternSequenceEditor.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef PSMKPATTERNSEQUENCEEDITOR_HPP
#define PSMKPATTERNSEQUENCEEDITOR_HPP PSMKPATTERNSEQUENCEEDITOR_HPP

/* base class */
#include <QtGlobal>
#if QT_VERSION < 0x050000
#include <QWidget>
#else
#include <QtWidgets/QWidget>
#endif

/* system headers */

/* Qt headers */
#include <QList>
#include <QString>
#include <QVariant>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QCheckBox;
class QGridLayout;
class QLabel;
class QPushButton;
class QSignalMapper;
class QWidget;

/* forward declaration of local classes */
class PsmkPacker;
class PsmkPatternProxyWidget;
class PsmkPatternWidget;

/*!
 \brief \todo

*/
class PsmkPatternSequenceEditor : public QWidget
{
   Q_OBJECT

public:
   explicit PsmkPatternSequenceEditor( const QList<PsmkPatternWidget*> &voice0,
                                       const QList<PsmkPatternWidget*> &voice1,
                                       const QList<bool> &hihat,
                                       const PsmkPacker *psmkPacker,
                                       QWidget *parent = 0 );
   virtual ~PsmkPatternSequenceEditor();

   QList<PsmkPatternWidget*> getVoicePatterns( int voice ) const;
   QList<bool> getHiHat() const;

public slots:
   void setTexts();
   void handleClone( QWidget *widget );
   void handleRemove( QWidget *widget );
   void handleSwap0( QWidget *widget );
   void handleSwap1( QWidget *widget );
   void handleSwap01( QWidget *widget );
   void handleSwapAll( QWidget *widget );

private:
   Q_DISABLE_COPY( PsmkPatternSequenceEditor )

   static const int LABELROW;
   static const int LABELCOL;
   static const int PROXY0ROW;
   static const int PROXY0COL;
   static const int PROXY1ROW;
   static const int PROXY1COL;
   static const int HIHATROW;
   static const int HIHATCOL;
   static const int SWAP0ROW;
   static const int SWAP0COL;
   static const int SWAP1ROW;
   static const int SWAP1COL;
   static const int SWAPALLROW;
   static const int SWAPALLCOL;
   static const int SWAP01ROW;
   static const int SWAP01COL;
   static const int CLONEROW;
   static const int CLONECOL;
   static const int REMOVEROW;
   static const int REMOVECOL;

   void updateLists();
   void updateButtonList( QList<QPushButton*> *list, int row, int column );
   void insertLine( int index, PsmkPatternWidget *voice0, PsmkPatternWidget *voice1, bool hihat );
   PsmkPatternWidget *getVoice(int row, int column );
   void updateLayout();

   QGridLayout                      *mpLayout;
   QSignalMapper                    *mpCloneMapper;
   QSignalMapper                    *mpRemoveMapper;
   QSignalMapper                    *mpSwap0Mapper;
   QSignalMapper                    *mpSwap1Mapper;
   QSignalMapper                    *mpSwapAllMapper;
   QSignalMapper                    *mpSwap01Mapper;
   const PsmkPacker                 *mpPsmkPacker;
   QList<QLabel*>                   mLabels;
   QList<PsmkPatternProxyWidget*>   mProxy0Widgets;
   QList<PsmkPatternProxyWidget*>   mProxy1Widgets;
   QList<QCheckBox*>                mHiHatWidgets;
   QList<QPushButton*>              mCloneButtons;
   QList<QPushButton*>              mRemoveButtons;
   QList<QPushButton*>              mSwap0Buttons;
   QList<QPushButton*>              mSwap1Buttons;
   QList<QPushButton*>              mSwap01Buttons;
   QList<QPushButton*>              mSwapAllButtons;
};

#endif
