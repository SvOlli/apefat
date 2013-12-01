/**
 * BarPreviewWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef BARPREVIEWWIDGET_HPP
#define BARPREVIEWWIDGET_HPP BARPREVIEWWIDGET_HPP

/* base class */
#include <QWidget>

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QBoxLayout;
class QCheckBox;

/* forward declaration of local classes */
class PitchComboBox;
class SlocumSong;
class SlocumNote;
class SoundComboBox;

/*!
 \brief widget for showing the name and the names of the beats of a \ref SlocumBar

 UNUSED RIGHT NOW
*/
class BarPreviewWidget : public QWidget
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param slocumSong pointer to global song data
    \param channel number of channel (0 or 1)
    \param index index of the bar inside the channel
    \param parent
   */
   explicit BarPreviewWidget( const SlocumSong *slocumSong, int channel, int index, QWidget *parent = 0 );
   /*!
    \brief destructor

   */
   virtual ~BarPreviewWidget();
   /*!
    \brief return the defined mime type

    replacement for a #define MIMETYPE_BARPREVIEWWIDGET

    \return const char
   */
   static const char *mimeType();
   /*!
    \brief index for drag'n'drop purposes

    \return int
   */
   int index() const;

   /*!
    \brief set if widget image for drag'n'drop should be created

    \param on
   */
   static void setCreateDragImage( bool on );

public slots:
   /*!
    \brief set the \ref SlocumSong and \ref SlocumNote pointers to new values

    \param slocumSong
    \param slocumNote
   */
   void setFromSong( const SlocumSong *slocumSong, int channel, int index );

protected:
   /*!
    \brief handle a mouse press event

    Used to create an instance for drag'n'drop.

    \param event
   */
   void mousePressEvent( QMouseEvent *event );

   /*!
    \brief overloaded to draw border

    \param event
   */
   void paintEvent( QPaintEvent *event );

private:
   Q_DISABLE_COPY( BarPreviewWidget )

   static const char       cMimeType[]; /*!< \brief mime type */
   static bool             cDragImage; /*!< \brief set by \ref setCreateDragImage */

   const SlocumSong        *mpSlocumSong; /*!< \brief pointer to whole song data */
   const SlocumNote        *mpSlocumNote; /*!< \brief \todo TODO */
   int                     mChannel; /*!< \brief \todo  */
   int                     mIndex; /*!< \brief \todo TODO */
   QBoxLayout              *mpLayout; /*!< \brief \todo TODO */
};

#endif // BARPREVIEWWIDGET_HPP
