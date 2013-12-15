/*
 * src/apps/Partyman/PlaylistContentWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef VOICEWIDGET_HPP
#define VOICEWIDGET_HPP VOICEWIDGET_HPP

/* base class */
#include <QListView>

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QDragEnterEvent;
class QDropEvent;
class QTimer;

/* forward declaration of local classes */
class SlocumSong;
class VoiceModel;


/*!
 \brief \todo complete documentation

 \dotfile "graphs/apps/Partyman/PlaylistContentWidget_connect.dot" "Connect Graph"
*/
class VoiceWidget : public QListView
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param slocumSong
    \param voice
    \param parent
   */
   explicit VoiceWidget( SlocumSong *slocumSong, int voice, QWidget *parent = 0 );
   /*!
    \brief destructor

   */
   virtual ~VoiceWidget();

   /*!
    \brief update data from song

    \param slocumSong
   */
   void setFromSong( SlocumSong *slocumSong );

public slots:
   /*!
    \brief \todo

   */
#if 1
   void setText();
   void copy();
   void cut();
   void paste();

signals:

protected:
   /*!
    \brief for handling key presses

    \param event
   */
   virtual void keyPressEvent( QKeyEvent *event );
   virtual void contextMenuEvent( QContextMenuEvent *event );
#endif

private:
   Q_DISABLE_COPY( VoiceWidget )

   SlocumSong        *mpSlocumSong; /*!< \brief \todo TODO */
   VoiceModel        *mpVoiceModel; /*!< \brief \todo TODO */
};

#endif
