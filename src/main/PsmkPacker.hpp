/**
 * PsmkPacker.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef PSMKPACKER_HPP
#define PSMKPACKER_HPP PSMKPACKER_HPP

/* base class */
#include <QGroupBox>

/* system headers */

/* Qt headers */
#include <QByteArray>
#include <QList>
#include <QMap>

/* local library headers */

/* local headers */
#include "PsmkPlayerWidget.hpp"

/* forward declaration of Qt classes */
class QLabel;

/* forward declaration of local classes */
class PsmkBeatWidget;
class PsmkPatternWidget;
class PsmkSongWidget;


/*!
 \brief \todo

*/
class PsmkPacker : public QGroupBox
{
   Q_OBJECT

public:
   explicit PsmkPacker( QWidget *parent = 0 );
   virtual ~PsmkPacker();

   int indexOfBeat( const QByteArray &beat ) const;
   int indexOfBeat( const PsmkBeatWidget *beat ) const;
   int indexOfPattern( const QByteArray &pattern ) const;
   int indexOfPattern( const PsmkPatternWidget *pattern ) const;
   QByteArray toSourceCode( const PsmkSongWidget *main );
   void toSongBinary( SongBinary *songBinary, const PsmkSongWidget *main );

   int beatCount( const QByteArray &beat ) const;
   int beatCount( const PsmkBeatWidget *beat ) const;

   static QString toHex( quint8 value );
   static QString toBin( quint8 value );
   static QStringList toHex( const QByteArray &value );
   static QStringList toBin( const QByteArray &value );
   static QStringList toDec( const QByteArray &value );
   static QString toList( const QStringList &list, const QString &opcode,
                          int entriesPerLine, const QString &prefix = QString() );
   void update( const PsmkSongWidget *main );

public slots:
   void setTexts();
   void clearStorage();
   void updateMessage();

private:
   Q_DISABLE_COPY( PsmkPacker )

   int addPattern( const QByteArray &pattern );
   int addPattern( const PsmkPatternWidget *pattern );
   int addBeat( const QByteArray &beat );
   int addBeat( const PsmkBeatWidget *beat );

   QLabel            *mpMessage1;
   int               mPatternCount;
   QList<QByteArray> mBeatStore;
   QList<QByteArray> mHighPatternStore;
   QList<QByteArray> mLowPatternStore;
   QMap<int,int>     mBeatCounter;
   QByteArray        mVoice[2];
   int               mBeatIndex[0x80][5]; // pattern,inuse+beat
   //! \todo remove magic 0x80 with something in PsmkConfig
};

#endif
